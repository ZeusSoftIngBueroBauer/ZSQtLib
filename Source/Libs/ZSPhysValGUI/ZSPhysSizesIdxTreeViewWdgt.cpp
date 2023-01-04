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
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSPhysValGUI/ZSPhysSizesIdxTreeViewWdgt.h"
#include "ZSPhysValGUI/ZSPhysSizesIdxTreeModel.h"
#include "ZSPhysValGUI/ZSPhysSizesIdxTreeView.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CWdgtIdxTreeViewPhysSizes : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtIdxTreeViewPhysSizes::CWdgtIdxTreeViewPhysSizes(
    CIdxTreePhysSizes* i_pIdxTree,
    QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pIdxTree(i_pIdxTree),
    m_szBtns(24, 24),
    m_pLytMain(nullptr),
    m_pLytHeadLine(nullptr),
    m_pBtnTreeViewResizeRowsAndColumnsToContents(nullptr),
    m_pBtnTreeViewExpandAll(nullptr),
    m_pBtnTreeViewCollapseAll(nullptr),
    m_pTreeView(nullptr)
{
    setObjectName( i_pIdxTree == nullptr ? "IdxTreePhysSizes" : i_pIdxTree->objectName() );

    m_pLytMain = new QVBoxLayout();
    m_pLytMain->setContentsMargins(0,0,0,0);

    setLayout(m_pLytMain);

    // Line with controls to modify optic of tree view
    //================================================

    m_pLytHeadLine = new QHBoxLayout();
    m_pLytMain->addLayout(m_pLytHeadLine);

    // <Button> Resize Columns To Contents
    //------------------------------------

    QPixmap pxmResizeToContents(":/ZS/TreeView/TreeViewResizeToContents.png");

    m_pBtnTreeViewResizeRowsAndColumnsToContents = new QPushButton();
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setIcon(pxmResizeToContents);
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setFixedSize(m_szBtns);
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setToolTip("Press to resize the columns to their contents");
    m_pLytHeadLine->addWidget(m_pBtnTreeViewResizeRowsAndColumnsToContents);

    QObject::connect(
        m_pBtnTreeViewResizeRowsAndColumnsToContents, &QPushButton::clicked,
        this, &CWdgtIdxTreeViewPhysSizes::onBtnTreeViewResizeRowsAndColumnsToContentsClicked );

    m_pLytHeadLine->addSpacing(10);

    // <Button> Expand All
    //--------------------

    QPixmap pxmExpandAll(":/ZS/TreeView/TreeViewExpandAll.png");

    m_pBtnTreeViewExpandAll = new QPushButton();
    m_pBtnTreeViewExpandAll->setIcon(pxmExpandAll);
    m_pBtnTreeViewExpandAll->setFixedSize(m_szBtns);
    m_pBtnTreeViewExpandAll->setToolTip("Press to expand all branches of the tree");
    m_pLytHeadLine->addWidget(m_pBtnTreeViewExpandAll);

    QObject::connect(
        m_pBtnTreeViewExpandAll, &QPushButton::clicked,
        this, &CWdgtIdxTreeViewPhysSizes::onBtnTreeViewExpandAllClicked );

    m_pLytHeadLine->addSpacing(10);

    // <Button> Collapse All
    //----------------------

    QPixmap pxmCollapseAll(":/ZS/TreeView/TreeViewCollapseAll.png");

    m_pBtnTreeViewCollapseAll = new QPushButton();
    m_pBtnTreeViewCollapseAll->setIcon(pxmCollapseAll);
    m_pBtnTreeViewCollapseAll->setFixedSize(m_szBtns);
    m_pBtnTreeViewCollapseAll->setToolTip("Press to collapse all branches of the tree");
    m_pLytHeadLine->addWidget(m_pBtnTreeViewCollapseAll);

    QObject::connect(
        m_pBtnTreeViewCollapseAll, &QPushButton::clicked,
        this, &CWdgtIdxTreeViewPhysSizes::onBtnTreeViewCollapseAllClicked );

    m_pLytHeadLine->addStretch();

    // <TreeView>
    //===============================

    m_pTreeView = new CTreeViewIdxTreePhysSizes(m_pIdxTree, nullptr);
    m_pLytMain->addWidget(m_pTreeView, 1);

    QObject::connect(
        m_pTreeView, &CTreeViewIdxTreePhysSizes::expanded,
        this, &CWdgtIdxTreeViewPhysSizes::onTreeViewExpanded );

    m_pTreeView->resizeColumnToContents(CModelIdxTreePhysSizes::EColumnTreeEntryName);

} // ctor

//------------------------------------------------------------------------------
CWdgtIdxTreeViewPhysSizes::~CWdgtIdxTreeViewPhysSizes()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
    m_szBtns = QSize(0, 0);
    m_pLytMain = nullptr;
    m_pLytHeadLine = nullptr;
    m_pBtnTreeViewResizeRowsAndColumnsToContents = nullptr;
    m_pBtnTreeViewExpandAll = nullptr;
    m_pBtnTreeViewCollapseAll = nullptr;
    m_pTreeView = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreeViewPhysSizes::setIdxTree(CIdxTreePhysSizes* i_pIdxTree)
//------------------------------------------------------------------------------
{
    m_pIdxTree = i_pIdxTree;

    m_pTreeView->setIdxTree(i_pIdxTree);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreeViewPhysSizes::onTreeViewExpanded( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    if( i_modelIdx.isValid() )
    {
        m_pTreeView->resizeColumnToContents(i_modelIdx.column());
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreeViewPhysSizes::onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_pTreeView != nullptr )
    {
        for( int idxClm = 0; idxClm < CModelIdxTreePhysSizes::EColumnCount; idxClm++ )
        {
            m_pTreeView->resizeColumnToContents(idxClm);
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtIdxTreeViewPhysSizes::onBtnTreeViewExpandAllClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_pTreeView != nullptr )
    {
        QObject::disconnect(
            m_pTreeView, &CTreeViewIdxTreePhysSizes::expanded,
            this, &CWdgtIdxTreeViewPhysSizes::onTreeViewExpanded );

        m_pTreeView->expandAll();

        for( int idxClm = 0; idxClm < CModelIdxTreePhysSizes::EColumnCount; idxClm++ )
        {
            m_pTreeView->resizeColumnToContents(idxClm);
        }

        QObject::connect(
            m_pTreeView, &CTreeViewIdxTreePhysSizes::expanded,
            this, &CWdgtIdxTreeViewPhysSizes::onTreeViewExpanded );
    }
}

//------------------------------------------------------------------------------
void CWdgtIdxTreeViewPhysSizes::onBtnTreeViewCollapseAllClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_pTreeView != nullptr )
    {
        m_pTreeView->collapseAll();
    }
}
