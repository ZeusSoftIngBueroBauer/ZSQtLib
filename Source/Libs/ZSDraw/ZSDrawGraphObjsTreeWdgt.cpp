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

#include "ZSDraw/ZSDrawGraphObjsTreeWdgt.h"
#include "ZSDraw/ZSDrawGraphObjsTreeView.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Trace;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtIdxTreeGraphObjs : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtIdxTreeGraphObjs::CWdgtIdxTreeGraphObjs(
    CModelIdxTreeGraphObjs* i_pModel,
    QWidget*                i_pWdgtParent,
    Qt::WindowFlags         i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_szBtns(24, 24),
    m_pLytMain(nullptr),
    m_pLytHeadLine(nullptr),
    m_pBtnTreeViewResizeRowsAndColumnsToContents(nullptr),
    m_pBtnTreeViewExpandAll(nullptr),
    m_pBtnTreeViewCollapseAll(nullptr),
    m_pBtnSortOrder(nullptr),
    m_pSpcHeadLine(nullptr),
    m_pModel(i_pModel),
    m_pSplitter(nullptr),
    m_pTreeView(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( i_pModel == nullptr ? "IdxTree" : i_pModel->objectName() );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "Model: " + QString(i_pModel == nullptr ? "nullptr" : i_pModel->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pLytMain = new QVBoxLayout;
    setLayout(m_pLytMain);

    // Line with controls to modify optic of tree view
    //================================================

    m_pLytHeadLine = new QHBoxLayout();
    m_pLytMain->addLayout(m_pLytHeadLine);

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

    m_pTreeView = new CTreeViewIdxTreeGraphObjs(m_pModel, this);
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
CWdgtIdxTreeGraphObjs::~CWdgtIdxTreeGraphObjs()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_szBtns = QSize(0, 0);
    m_pLytMain = nullptr;
    m_pLytHeadLine = nullptr;
    m_pBtnTreeViewResizeRowsAndColumnsToContents = nullptr;
    m_pBtnTreeViewExpandAll = nullptr;
    m_pBtnTreeViewCollapseAll = nullptr;
    m_pBtnSortOrder = nullptr;
    m_pSpcHeadLine = nullptr;
    m_pModel = nullptr;
    m_pSplitter = nullptr;
    m_pTreeView = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreeGraphObjs::onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnTreeViewResizeRowsAndColumnsToContentsClicked",
        /* strMthInArgs */ strMthInArgs );

    if( m_pTreeView != nullptr )
    {
        for( int idxClm = 0; idxClm < CModelIdxTree::EColumnCount; idxClm++ )
        {
            m_pTreeView->resizeColumnToContents(idxClm);
        }
    }

} // onBtnTreeViewResizeRowsAndColumnsToContentsClicked

//------------------------------------------------------------------------------
void CWdgtIdxTreeGraphObjs::onBtnTreeViewExpandAllClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnTreeViewExpandAllClicked",
        /* strMthInArgs */ strMthInArgs );

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
void CWdgtIdxTreeGraphObjs::onBtnTreeViewCollapseAllClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnTreeViewCollapseAllClicked",
        /* strMthInArgs */ strMthInArgs );

    if( m_pTreeView != nullptr )
    {
        m_pTreeView->collapseAll();
    }

} // onBtnTreeViewCollapseAllClicked

//------------------------------------------------------------------------------
void CWdgtIdxTreeGraphObjs::onBtnSortOrderClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnSortOrderClicked",
        /* strMthInArgs */ strMthInArgs );

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

    m_pBtnSortOrder->setProperty("SortOrderCurr", static_cast<int>(sortOrderNew));

    QPixmap pxmSortOrder = idxTreeSortOrder2Pixmap(sortOrderNew, m_szBtns);
    pxmSortOrder.setMask(pxmSortOrder.createHeuristicMask());
    m_pBtnSortOrder->setIcon(pxmSortOrder);

} // onBtnSortOrderClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreeGraphObjs::onTreeViewSelectionModelCurrentRowChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& i_modelIdxPrev )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs  = "Curr {" + CModelIdxTree::ModelIdx2Str(i_modelIdxCurr) + "}";
        strMthInArgs += ", Prev {" + CModelIdxTree::ModelIdx2Str(i_modelIdxPrev) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTreeViewSelectionModelCurrentRowChanged",
        /* strMthInArgs */ strMthInArgs );

} // onTreeViewSelectionModelCurrentRowChanged
