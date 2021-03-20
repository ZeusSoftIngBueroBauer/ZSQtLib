/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#if QT_VERSION < 0x050000
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
#include "ZSSysGUI/ZSSysIdxTreeModelEntries.h"
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
using namespace ZS::Trace;


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
    QPixmap pxm = QPixmap( ":/ZS/TreeView/TreeViewViewMode" + str + ".bmp" );
    pxm = pxm.scaled(i_sz);
    return pxm;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtIdxTree::CWdgtIdxTree(
    CModelIdxTree*  i_pModel,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags,
    int             i_iTrcDetailLevel ) :
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
    m_pSplitter(nullptr),
    m_pTreeView(nullptr),
    m_pTableViewBranchContent(nullptr),
    m_iTrcDetailLevel(i_iTrcDetailLevel)
{
    setObjectName( i_pModel == nullptr ? "IdxTree" : i_pModel->objectName() );

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Model: " + QString(i_pModel == nullptr ? "nullptr" : i_pModel->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    m_pLytMain = new QVBoxLayout;
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

    QPixmap pxmResizeToContents(":/ZS/TreeView/TreeViewResizeToContents.bmp");
    pxmResizeToContents.setMask(pxmResizeToContents.createHeuristicMask());

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

    QPixmap pxmExpandAll(":/ZS/TreeView/TreeViewExpandAll.bmp");
    pxmExpandAll.setMask(pxmExpandAll.createHeuristicMask());

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

    QPixmap pxmCollapseAll(":/ZS/TreeView/TreeViewCollapseAll.bmp");
    pxmCollapseAll.setMask(pxmCollapseAll.createHeuristicMask());

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
    pxmSortOrder.setMask(pxmSortOrder.createHeuristicMask());

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

    m_pTreeView = new CTreeViewIdxTree(m_pModel, this);
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

    // Connect to the signals of the tree view
    //----------------------------------------

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
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ strMthInArgs );

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
    m_pSplitter = nullptr;
    m_pTreeView = nullptr;
    m_pTableViewBranchContent = nullptr;
    m_iTrcDetailLevel = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTree::setViewMode( EViewMode i_viewMode )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "ViewMode: " + viewMode2Str(i_viewMode);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
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
                m_pLytMain->addWidget(m_pTreeView);

                delete m_pSplitter;
                m_pSplitter = nullptr;
                m_pTableViewBranchContent = nullptr;
            }
            m_pLytMain->addWidget(m_pTreeView);

            m_pLytHeadLine->removeWidget(m_pEdtBranch);
            delete m_pEdtBranch;
            m_pEdtBranch = nullptr;

            m_pSpcHeadLine = new QSpacerItem(0, 24, QSizePolicy::Expanding);
            m_pLytHeadLine->addSpacerItem(m_pSpcHeadLine);

            m_pModel->setFilter(EIdxTreeEntryType::Undefined);

        } // if( m_viewMode == EViewMode::NavPanelOnly )

        else if( m_viewMode == EViewMode::NavPanelAndBranchContent )
        {
            m_pLytHeadLine->removeItem(m_pSpcHeadLine);
            delete m_pSpcHeadLine;
            m_pSpcHeadLine = nullptr;

            m_pEdtBranch = new QLineEdit();
            m_pLytHeadLine->addWidget(m_pEdtBranch, 1);

            if( m_pSplitter == nullptr )
            {
                m_pSplitter = new QSplitter(Qt::Horizontal);
                m_pLytMain->addWidget(m_pSplitter, 1);

                m_pSplitter->addWidget(m_pTreeView);

                if( m_pTableViewBranchContent == nullptr )
                {
                    m_pTableViewBranchContent = new CTableViewIdxTreeBranchContent(m_pModel, this);
                }
                m_pSplitter->addWidget(m_pTableViewBranchContent);
            }

            m_pModel->setFilter(EIdxTreeEntryType::Branch);

        } // if( m_viewMode == EViewMode::NavPanelAndBranchContent )

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

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
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

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
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

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
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

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
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

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
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
    pxmSortOrder.setMask(pxmSortOrder.createHeuristicMask());
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

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Curr {" + CModelIdxTree::ModelIdx2Str(i_modelIdxCurr) + "}";
        strMthInArgs += ", Prev {" + CModelIdxTree::ModelIdx2Str(i_modelIdxPrev) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTreeViewSelectionModelCurrentRowChanged",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_modelIdxCurr.isValid() )
    {
        CModelAbstractTreeEntry* pModelTreeEntry = static_cast<CModelAbstractTreeEntry*>(i_modelIdxCurr.internalPointer());

        if( pModelTreeEntry != nullptr )
        {
            CBranchIdxTreeEntry* pBranch = nullptr;

            if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
            {
                pBranch = dynamic_cast<CBranchIdxTreeEntry*>(pModelTreeEntry->treeEntry());
            }

            if( m_pEdtBranch != nullptr )
            {
                if( pBranch != nullptr )
                {
                    m_pEdtBranch->setText(pBranch->keyInTree());
                }
                else
                {
                    m_pEdtBranch->setText("");
                }
            }
            if( m_pTableViewBranchContent != nullptr )
            {
                m_pTableViewBranchContent->setBranch(pBranch);
            }
        } // if( pModelTreeEntry != nullptr )
    } // if( i_modelIdxCurr.isValid() )

} // onTreeViewSelectionModelCurrentRowChanged
