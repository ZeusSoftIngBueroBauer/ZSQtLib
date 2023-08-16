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

#include "ZSDraw/TreeView/ZSDrawGraphObjsTreeWdgt.h"
#include "ZSDraw/TreeView/ZSDrawGraphObjsTreeView.h"
#include "ZSDraw/Drawing/ZSDrawingView.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtGraphObjsTree : public QWidget
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

/* enum class EViewMode
==============================================================================*/

static const SEnumEntry s_arEnumStrWdgtIdxTreeViewModes[] = {
    /*  0 */ SEnumEntry( static_cast<int>(CWdgtGraphObjsTree::EViewMode::NavPanelOnly),           "NavPanelOnly" ),
    /*  1 */ SEnumEntry( static_cast<int>(CWdgtGraphObjsTree::EViewMode::NavPanelAndNodeContent), "NavPanelAndNodeContent" )
};

//------------------------------------------------------------------------------
QString CWdgtGraphObjsTree::viewMode2Str( EViewMode i_eVal, int i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrWdgtIdxTreeViewModes, _ZSArrLen(s_arEnumStrWdgtIdxTreeViewModes), static_cast<int>(i_eVal), i_alias);
}

//------------------------------------------------------------------------------
QPixmap CWdgtGraphObjsTree::viewMode2Pixmap( EViewMode i_eVal, const QSize& i_sz )
//------------------------------------------------------------------------------
{
    QString str = viewMode2Str(i_eVal);
    if( i_eVal == EViewMode::NavPanelAndNodeContent ) {
        str = "NavPanelAndBranchContent";
    }
    QPixmap pxm = QPixmap( ":/ZS/TreeView/TreeViewViewMode" + str + ".png" );
    pxm = pxm.scaled(i_sz);
    return pxm;
}

//------------------------------------------------------------------------------
CWdgtGraphObjsTree::EViewMode CWdgtGraphObjsTree::str2ViewMode( const QString& i_str )
//------------------------------------------------------------------------------
{
    EViewMode viewMode = EViewMode::Undefined;
    try {
        viewMode = static_cast<EViewMode>(SEnumEntry::str2Enumerator(
            s_arEnumStrWdgtIdxTreeViewModes, _ZSArrLen(s_arEnumStrWdgtIdxTreeViewModes), i_str));
    }
    catch(CException&) {
        viewMode = EViewMode::Undefined;
    }
    return viewMode;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjsTree::CWdgtGraphObjsTree(
    CDrawingView* i_pDrawingView,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pDrawingView(i_pDrawingView),
    m_szBtns(24, 24),
    m_viewMode(EViewMode::NavPanelOnly),
    m_pLytMain(nullptr),
    m_pLytHeadLine(nullptr),
    m_pBtnViewMode(nullptr),
    m_pBtnTreeViewResizeRowsAndColumnsToContents(nullptr),
    m_pBtnTreeViewExpandAll(nullptr),
    m_pBtnTreeViewCollapseAll(nullptr),
    m_pBtnSortOrder(nullptr),
    m_pTreeView(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_pDrawingView->objectName());

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::TreeView", ClassName(), objectName());

    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_pDrawingView->objectName();
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
        this, &CWdgtGraphObjsTree::onBtnViewModeClicked );

    // <Button> Resize Columns To Contents
    //------------------------------------

    QPixmap pxmResizeToContents(":/ZS/TreeView/TreeViewResizeToContents.png");

    m_pBtnTreeViewResizeRowsAndColumnsToContents = new QPushButton();
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setIcon(pxmResizeToContents);
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setFixedSize(m_szBtns);
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setToolTip("Press to resize the columns to their contents");
    m_pLytHeadLine->addWidget(m_pBtnTreeViewResizeRowsAndColumnsToContents);
    m_pLytHeadLine->addSpacing(10);

    QObject::connect(
        m_pBtnTreeViewResizeRowsAndColumnsToContents, &QPushButton::clicked,
        this, &CWdgtGraphObjsTree::onBtnTreeViewResizeRowsAndColumnsToContentsClicked );

    // <Button> Expand All
    //--------------------

    QPixmap pxmExpandAll(":/ZS/TreeView/TreeViewExpandAll.png");

    m_pBtnTreeViewExpandAll = new QPushButton();
    m_pBtnTreeViewExpandAll->setIcon(pxmExpandAll);
    m_pBtnTreeViewExpandAll->setFixedSize(m_szBtns);
    m_pBtnTreeViewExpandAll->setToolTip("Press to expand all branches of the tree");
    m_pLytHeadLine->addWidget(m_pBtnTreeViewExpandAll);
    m_pLytHeadLine->addSpacing(10);

    QObject::connect(
        m_pBtnTreeViewExpandAll, &QPushButton::clicked,
        this, &CWdgtGraphObjsTree::onBtnTreeViewExpandAllClicked );

    // <Button> Collapse All
    //----------------------

    QPixmap pxmCollapseAll(":/ZS/TreeView/TreeViewCollapseAll.png");

    m_pBtnTreeViewCollapseAll = new QPushButton();
    m_pBtnTreeViewCollapseAll->setIcon(pxmCollapseAll);
    m_pBtnTreeViewCollapseAll->setFixedSize(m_szBtns);
    m_pBtnTreeViewCollapseAll->setToolTip("Press to collapse all branches of the tree");
    m_pLytHeadLine->addWidget(m_pBtnTreeViewCollapseAll);
    m_pLytHeadLine->addSpacing(10);

    QObject::connect(
        m_pBtnTreeViewCollapseAll, &QPushButton::clicked,
        this, &CWdgtGraphObjsTree::onBtnTreeViewCollapseAllClicked );

    // <Button> Sort Order
    //----------------------

    QPixmap pxmSortOrder = idxTreeSortOrder2Pixmap(EIdxTreeSortOrder::Config, m_szBtns);

    m_pBtnSortOrder = new QPushButton();
    m_pBtnSortOrder->setFixedSize(m_szBtns);
    m_pBtnSortOrder->setIcon(pxmSortOrder);
    m_pBtnSortOrder->setProperty("SortOrderCurr", QVariant(static_cast<int>(EIdxTreeSortOrder::Config)));
    m_pBtnSortOrder->setToolTip("Press to toggle the sort order between \"As Configured\" and \"Alphabetically Sorted\"");
    m_pLytHeadLine->addWidget(m_pBtnSortOrder);
    m_pLytHeadLine->addStretch();

    QObject::connect(
        m_pBtnSortOrder, &QPushButton::clicked,
        this, &CWdgtGraphObjsTree::onBtnSortOrderClicked );

    // <TreeView>
    //===========

    m_pTreeView = new CTreeViewGraphObjs(m_pDrawingView->drawingScene());
    //m_pTreeView->setAlternatingRowColors(true);
    m_pLytMain->addWidget(m_pTreeView, 1);

    //QObject::connect(
    //    m_pTreeView, static_cast<void (CTreeViewIdxTree::*)(EIdxTreeSortOrder)>(&CTreeViewIdxTree::sortOrderChanged),
    //    this, &CWdgtGraphObjsTree::onTreeViewSortOrderChanged );
    QObject::connect(
        m_pTreeView->selectionModel(), &QItemSelectionModel::currentRowChanged,
        this, &CWdgtGraphObjsTree::onTreeViewCurrentRowChanged );

    m_pTreeView->resizeColumnToContents(CModelIdxTree::EColumnTreeEntryName);

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjsTree::~CWdgtGraphObjsTree()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    //QObject::disconnect(
    //    m_pTreeView, static_cast<void (CTreeViewIdxTree::*)(EIdxTreeSortOrder)>(&CTreeViewIdxTree::sortOrderChanged),
    //    this, &CWdgtGraphObjsTree::onTreeViewSortOrderChanged );

    // When destroying the model the selected, current row may be changed.
    // If we don't disconnect from the currentRowChanged signal of the tree views
    // selection model the application will crash when destroying the index tree model.
    // The tree view is a child of this widget and will be destroyed after this dtor
    // has been called. The signal/slot connection is still alive as also the QObject
    // destructor has not been called yet. But Qt recognizes that the signal endpoint
    // has been destroyed and will throw an exception. Not what we want ...
    QObject::disconnect(
        m_pTreeView->selectionModel(), &QItemSelectionModel::currentRowChanged,
        this, &CWdgtGraphObjsTree::onTreeViewCurrentRowChanged );

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }

    m_pDrawingView = nullptr;
    m_szBtns = QSize(0, 0);
    m_viewMode = static_cast<EViewMode>(0);
    m_pLytMain = nullptr;
    m_pLytHeadLine = nullptr;
    m_pBtnViewMode = nullptr;
    m_pBtnTreeViewResizeRowsAndColumnsToContents = nullptr;
    m_pBtnTreeViewExpandAll = nullptr;
    m_pBtnTreeViewCollapseAll = nullptr;
    m_pBtnSortOrder = nullptr;
    m_pTreeView = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjsTree::saveState(QSettings& i_settings) const
//------------------------------------------------------------------------------
{
    i_settings.setValue(
        ClassName() + "/" + objectName() + "/ViewMode", viewMode2Str(m_viewMode));
}

//------------------------------------------------------------------------------
void CWdgtGraphObjsTree::restoreState(const QSettings& i_settings)
//------------------------------------------------------------------------------
{
    QString strViewMode = i_settings.value(
        ClassName() + "/" + objectName() + "/ViewMode", viewMode2Str(m_viewMode)).toString();
    EViewMode viewMode = str2ViewMode(strViewMode);
    if( viewMode != EViewMode::Undefined && viewMode != m_viewMode ) {
        m_viewMode = viewMode;
        QPixmap pxmViewMode = viewMode2Pixmap(m_viewMode, m_szBtns);
        m_pBtnViewMode->setIcon(pxmViewMode);
        emit viewModeChanged(viewMode2Str(m_viewMode));
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CWdgtGraphObjsTree::setExcludeLeaves( bool i_bExcludeLeaves )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//
//    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
//    {
//        strMthInArgs = bool2Str(i_bExcludeLeaves);
//    }
//
//    CMethodTracer mthTracer(
//        /* pTrcAdminObj       */ m_pTrcAdminObj,
//        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod          */ "setExcludeLeaves",
//        /* strMethodInArgs    */ strMthInArgs );
//
//    m_pTreeView->setExcludeLeaves(i_bExcludeLeaves);
//}
//
////------------------------------------------------------------------------------
//bool CWdgtGraphObjsTree::areLeavesExcluded() const
////------------------------------------------------------------------------------
//{
//    return m_pTreeView->areLeavesExcluded();
//}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjsTree::onBtnViewModeClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnViewModeClicked",
        /* strMthInArgs */ strMthInArgs );

    m_viewMode =
        m_viewMode == EViewMode::NavPanelOnly ?
            EViewMode::NavPanelAndNodeContent : EViewMode::NavPanelOnly;
    QPixmap pxmViewMode = viewMode2Pixmap(m_viewMode, m_szBtns);
    m_pBtnViewMode->setIcon(pxmViewMode);
    emit viewModeChanged(viewMode2Str(m_viewMode));
}

//------------------------------------------------------------------------------
void CWdgtGraphObjsTree::onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked )
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
        /* strMethod          */ "onBtnTreeViewResizeRowsAndColumnsToContentsClicked",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pTreeView != nullptr ) {
        for( int idxClm = 0; idxClm < CModelIdxTree::EColumnCount; idxClm++ ) {
            m_pTreeView->resizeColumnToContents(idxClm);
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjsTree::onBtnTreeViewExpandAllClicked( bool i_bChecked )
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
        /* strMethod          */ "onBtnTreeViewExpandAllClicked",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pTreeView != nullptr ) {
        m_pTreeView->expandAll();
        for( int idxClm = 0; idxClm < CModelIdxTree::EColumnCount; idxClm++ ) {
            m_pTreeView->resizeColumnToContents(idxClm);
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjsTree::onBtnTreeViewCollapseAllClicked( bool i_bChecked )
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
        /* strMethod          */ "onBtnTreeViewCollapseAllClicked",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pTreeView != nullptr ) {
        m_pTreeView->collapseAll();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjsTree::onBtnSortOrderClicked( bool i_bChecked )
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
        /* strMethod          */ "onBtnSortOrderClicked",
        /* strMethodInArgs    */ strMthInArgs );

    EIdxTreeSortOrder sortOrderCurr = static_cast<EIdxTreeSortOrder>(m_pBtnSortOrder->property("SortOrderCurr").toInt());
    EIdxTreeSortOrder sortOrderNew;

    if( sortOrderCurr == EIdxTreeSortOrder::Config ) {
        sortOrderNew = EIdxTreeSortOrder::Ascending;
    } else if (sortOrderCurr == EIdxTreeSortOrder::Ascending ) {
        sortOrderNew = EIdxTreeSortOrder::Descending;
    } else {
        sortOrderNew = EIdxTreeSortOrder::Config;
    }

    if( m_pTreeView != nullptr ) {
        m_pTreeView->setSortOrder(sortOrderNew);
    }

    m_pBtnSortOrder->setProperty("SortOrderCurr", static_cast<int>(sortOrderNew));

    QPixmap pxmSortOrder = idxTreeSortOrder2Pixmap(sortOrderNew, m_szBtns);
    m_pBtnSortOrder->setIcon(pxmSortOrder);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjsTree::onTreeViewSortOrderChanged( EIdxTreeSortOrder i_sortOrder )
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
        /* strMethod          */ "onTableViewSortOrderChanged",
        /* strMethodInArgs    */ strMthInArgs );

    m_pBtnSortOrder->setProperty("SortOrderCurr", static_cast<int>(i_sortOrder));

    QPixmap pxmSortOrder = idxTreeSortOrder2Pixmap(i_sortOrder, m_szBtns);
    m_pBtnSortOrder->setIcon(pxmSortOrder);
}

//------------------------------------------------------------------------------
void CWdgtGraphObjsTree::onTreeViewCurrentRowChanged(
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

    emit_currentRowChanged(i_modelIdxCurr, i_modelIdxPrev);
}

/*==============================================================================
private: // auxiliary methods (tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjsTree::emit_currentRowChanged(
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
        /* strMethod          */ "emit_currentRowChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit currentRowChanged(i_modelIdxCurr, i_modelIdxPrev);
}
