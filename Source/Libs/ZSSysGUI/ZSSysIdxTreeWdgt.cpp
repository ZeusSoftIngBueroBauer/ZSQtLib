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
#include "ZSSysGUI/ZSSysIdxTreeViewWdgt.h"
#include "ZSSysGUI/ZSSysIdxTreeTableViewBranchContentWdgt.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysIdxTree.h"
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
    CIdxTree* i_pIdxTree,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pIdxTree(nullptr),
    m_szBtns(24, 24),
    m_viewMode(EViewMode::NavPanelOnly),
    m_pLytMain(nullptr),
    m_pLytHeadLine(nullptr),
    m_pBtnViewMode(nullptr),
    m_pEdtBranch(nullptr),
    m_pSplitter(nullptr),
    m_pWdgtTreeView(nullptr),
    m_pWdgtTableViewBranchContent(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName() );

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

    m_pLytMain = new QVBoxLayout();
    setLayout(m_pLytMain);

    // Line with controls to modify optic of tree view
    //================================================

    m_pLytHeadLine = new QHBoxLayout();
    m_pLytMain->addLayout(m_pLytHeadLine);

    // <Button> View Mode
    //-------------------

    QPixmap pxmViewMode = viewMode2Pixmap(m_viewMode, m_szBtns);

    m_pBtnViewMode = new QPushButton();
    m_pBtnViewMode->setFixedSize(m_szBtns);
    m_pBtnViewMode->setIcon(pxmViewMode);
    m_pBtnViewMode->setToolTip("Press to toggle view mode between NavPanelOnly and NavPanelAndBranchContent");
    m_pLytHeadLine->addWidget(m_pBtnViewMode);
    m_pLytHeadLine->addSpacing(10);

    QObject::connect(
        m_pBtnViewMode, &QPushButton::clicked,
        this, &CWdgtIdxTree::onBtnViewModeClicked );

    // <LineEdit> Branch
    //------------------

    m_pEdtBranch = new QLineEdit();
    m_pLytHeadLine->addWidget(m_pEdtBranch, 1);

    // Split View with Tree View and Branch Content Widget
    //====================================================

    m_pSplitter = new QSplitter(Qt::Horizontal);
    m_pLytMain->addWidget(m_pSplitter, 1);

    // <TreeView>
    //-----------

    m_pWdgtTreeView = new CWdgtIdxTreeView(nullptr, nullptr);
    m_pWdgtTreeView->setExcludeLeaves(m_viewMode != EViewMode::NavPanelOnly);
    m_pWdgtTreeView->setMinimumWidth(50);
    m_pSplitter->addWidget(m_pWdgtTreeView);

    QObject::connect(
        m_pWdgtTreeView, &CWdgtIdxTreeView::currentRowChanged,
        this, &CWdgtIdxTree::onTreeViewCurrentRowChanged );

    // <TableView>
    //------------

    if( m_viewMode == EViewMode::NavPanelAndBranchContent )
    {
        m_pWdgtTableViewBranchContent = new CWdgtIdxTreeTableViewBranchContent(nullptr, nullptr);
        m_pSplitter->addWidget(m_pWdgtTableViewBranchContent);
    }

    if( i_pIdxTree != nullptr )
    {
        setIdxTree(i_pIdxTree);
    }
} // ctor

//------------------------------------------------------------------------------
CWdgtIdxTree::~CWdgtIdxTree()
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

    m_pIdxTree = nullptr;
    m_viewMode = static_cast<EViewMode>(0);
    m_szBtns = QSize(0, 0);
    m_pLytMain = nullptr;
    m_pLytHeadLine = nullptr;
    m_pBtnViewMode = nullptr;
    m_pEdtBranch = nullptr;
    m_pSplitter = nullptr;
    m_pWdgtTreeView = nullptr;
    m_pWdgtTableViewBranchContent = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CTreeViewIdxTree* CWdgtIdxTree::treeView()
//------------------------------------------------------------------------------
{
    return m_pWdgtTreeView->treeView();
}

//------------------------------------------------------------------------------
CTableViewIdxTreeBranchContent* CWdgtIdxTree::tableView()
//------------------------------------------------------------------------------
{
    CTableViewIdxTreeBranchContent* pTableView = nullptr;

    if( m_pWdgtTableViewBranchContent != nullptr )
    {
        pTableView = m_pWdgtTableViewBranchContent->tableView();
    }
    return pTableView;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTree::setIdxTree( CIdxTree* i_pIdxTree )
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
                this, &CWdgtIdxTree::onIdxTreeAboutToBeDestroyed);
        }

        m_pIdxTree = i_pIdxTree;

        if( m_pIdxTree != nullptr )
        {
            QObject::connect(
                m_pIdxTree, &CIdxTree::aboutToBeDestroyed,
                this, &CWdgtIdxTree::onIdxTreeAboutToBeDestroyed);
        }

        m_pWdgtTreeView->setIdxTree(i_pIdxTree);

        if( m_pWdgtTableViewBranchContent != nullptr )
        {
            m_pWdgtTableViewBranchContent->setIdxTree(i_pIdxTree);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTree::setViewMode( EViewMode i_viewMode )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = viewMode2Str(i_viewMode);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setViewMode",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_viewMode != m_viewMode )
    {
        m_viewMode = i_viewMode;

        m_pWdgtTreeView->setExcludeLeaves(m_viewMode != EViewMode::NavPanelOnly);

        if( m_viewMode == EViewMode::NavPanelAndBranchContent )
        {
            m_pWdgtTableViewBranchContent = new CWdgtIdxTreeTableViewBranchContent(m_pIdxTree);
            m_pSplitter->addWidget(m_pWdgtTableViewBranchContent);
        }
        else
        {
            delete m_pWdgtTableViewBranchContent;
            m_pWdgtTableViewBranchContent = nullptr;
        }

        QPixmap pxmViewMode = viewMode2Pixmap(m_viewMode, m_szBtns);
        m_pBtnViewMode->setIcon(pxmViewMode);
    }
} // setViewMode

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTree::onBtnViewModeClicked( bool i_bChecked )
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
        /* strMethod          */ "onBtnViewModeClicked",
        /* strMethodInArgs    */ strMthInArgs );

    EViewMode viewModeNew =
        m_viewMode == EViewMode::NavPanelOnly ?
            EViewMode::NavPanelAndBranchContent : EViewMode::NavPanelOnly;
    setViewMode(viewModeNew);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTree::onTreeViewCurrentRowChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& i_modelIdxPrev )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs  = "Curr {" + CModelIdxTree::modelIdx2Str(i_modelIdxCurr) + "}";
        strMthInArgs += ", Prev {" + CModelIdxTree::modelIdx2Str(i_modelIdxPrev) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onTreeViewCurrentRowChanged",
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
            if( m_pWdgtTableViewBranchContent != nullptr )
            {
                m_pWdgtTableViewBranchContent->setKeyInTreeOfRootEntry(pModelTreeEntryBranch->keyInTree());
            }
        }
        else
        {
            if( m_pEdtBranch != nullptr )
            {
                m_pEdtBranch->setText("");
            }
            if( m_pWdgtTableViewBranchContent != nullptr )
            {
                m_pWdgtTableViewBranchContent->setKeyInTreeOfRootEntry("");
            }
        }
    }
} // onTreeViewCurrentRowChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTree::onIdxTreeAboutToBeDestroyed()
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
