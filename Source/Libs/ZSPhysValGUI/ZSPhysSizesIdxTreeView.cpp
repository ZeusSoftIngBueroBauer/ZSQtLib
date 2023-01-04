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

#include <QtWidgets/qapplication.h>
#include <QtGui/qevent.h>

#include "ZSPhysValGUI/ZSPhysSizesIdxTreeView.h"
#include "ZSPhysValGUI/ZSPhysSizesIdxTreeModel.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qmenu.h>
#else
#include <QtWidgets/qmenu.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CTreeViewIdxTreePhysSizes : public QTreeView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTreeViewIdxTreePhysSizes::CTreeViewIdxTreePhysSizes(
    CIdxTreePhysSizes* i_pIdxTree,
    QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QTreeView(i_pWdgtParent),
    m_pIdxTree(i_pIdxTree),
    m_pModel(nullptr),
    m_pMenuNameSpaceContext(nullptr),
    m_pActionNameSpaceTitle(nullptr),
    m_pActionNameSpaceExpand(nullptr),
    m_pActionNameSpaceCollapse(nullptr),
    m_modelIdxSelectedOnMousePressEvent(),
    m_modelIdxSelectedOnMouseReleaseEvent()
{
    setObjectName( QString(i_pIdxTree == nullptr ? "IdxTreePhysSizes" : i_pIdxTree->objectName()) );

    m_pModel = new CModelIdxTreePhysSizes(m_pIdxTree, nullptr);

    setModel(m_pModel);

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAlternatingRowColors(true);
    setAllColumnsShowFocus(true);
    //setEditTriggers(QAbstractItemView::AllEditTriggers);

    //hideColumn(CModelIdxTree::EColumnTreeEntryName);
    hideColumn(CModelIdxTree::EColumnTreeEntryNameDecorated);
    hideColumn(CModelIdxTree::EColumnTreeEntryTypeImageUrl);
    hideColumn(CModelIdxTree::EColumnTreeEntryTypeIcon);
    hideColumn(CModelIdxTree::EColumnTreeEntryType);
    //hideColumn(CModelIdxTree::EColumnInternalId);
    //hideColumn(CModelIdxTree::EColumnIdxInTree);
    hideColumn(CModelIdxTree::EColumnIdxInParentBranch);
    hideColumn(CModelIdxTree::EColumnKeyInTree);
    hideColumn(CModelIdxTree::EColumnKeyInParentBranch);

    // Context Popup Menu of Name Space Branches
    //------------------------------------------

    m_pMenuNameSpaceContext = new QMenu("Name Space",this);

    m_pActionNameSpaceTitle = new QAction("Name Space:",this);

    QPixmap pxmExpandAll(":/ZS/TreeView/TreeViewExpandAll.png");
    m_pActionNameSpaceExpand = new QAction(pxmExpandAll, "Expand", this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceExpand);

    QObject::connect(
        m_pActionNameSpaceExpand, &QAction::triggered,
        this, &CTreeViewIdxTreePhysSizes::onActionNameSpaceExpandTriggered);

    QPixmap pxmCollapseAll(":/ZS/TreeView/TreeViewCollapseAll.png");
    m_pActionNameSpaceCollapse = new QAction(pxmCollapseAll, "Collapse", this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceCollapse);

    QObject::connect(
        m_pActionNameSpaceCollapse, &QAction::triggered,
        this, &CTreeViewIdxTreePhysSizes::onActionNameSpaceCollapseTriggered);

} // ctor

//------------------------------------------------------------------------------
CTreeViewIdxTreePhysSizes::~CTreeViewIdxTreePhysSizes()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pModel;
    }
    catch(...)
    {
    }

    m_pIdxTree = nullptr;
    m_pModel = nullptr;
    m_pMenuNameSpaceContext = nullptr;
    m_pActionNameSpaceTitle = nullptr;
    m_pActionNameSpaceExpand = nullptr;
    m_pActionNameSpaceCollapse = nullptr;
    //m_modelIdxSelectedOnMousePressEvent;
    //m_modelIdxSelectedOnMouseReleaseEvent;

} // dtor

//------------------------------------------------------------------------------
void CTreeViewIdxTreePhysSizes::setIdxTree( CIdxTreePhysSizes* i_pIdxTree )
//------------------------------------------------------------------------------
{
    if( m_pIdxTree != i_pIdxTree )
    {
        m_pIdxTree = i_pIdxTree;

        m_pModel->setIdxTree(i_pIdxTree);
    }
}

/*==============================================================================
public: // overridable slots of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreePhysSizes::expandAll()
//------------------------------------------------------------------------------
{
    QTreeView::expandAll();
    QModelIndex modelIdxRoot = model()->index(0, 0, QModelIndex());
    expandRecursive(modelIdxRoot);
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreePhysSizes::collapseAll()
//------------------------------------------------------------------------------
{
    QTreeView::collapseAll();
    QModelIndex modelIdxRoot = model()->index(0, 0, QModelIndex());
    collapseRecursive(modelIdxRoot);
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreePhysSizes::expandRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

    if( pModelTreeEntry != nullptr )
    {
        if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
        {
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            CModelIdxTreeEntry* pModelTreeEntryChild;
            QModelIndex         modelIdxChild;

            for( int idxEntry = 0; idxEntry < pModelBranch->count(); ++idxEntry )
            {
                pModelTreeEntryChild = pModelBranch->at(idxEntry);

                if( pModelTreeEntryChild->entryType() == EIdxTreeEntryType::Root || pModelTreeEntryChild->entryType() == EIdxTreeEntryType::Branch )
                {
                    modelIdxChild = model()->index(idxEntry, 0, i_modelIdx);
                    expandRecursive(modelIdxChild);
                }
            }
            expand(i_modelIdx);
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreePhysSizes::collapseRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

    if( pModelTreeEntry != nullptr )
    {
        if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
        {
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            QModelIndex         modelIdx;

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
        }
    }
}

/*==============================================================================
public: // slots (hiding not overridable slots with same name in QTreeView)
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreePhysSizes::expand( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    // If calling "expandAll" the signal "expanded" is not invoked for all
    // nodes of the tree view. This is a workaround to store the "expanded"
    // state of the model tree node in order to restore this state on changing
    // the sort order of the model.
    QTreeView::expand(i_modelIdx);
    if( i_modelIdx.isValid() )
    {
        onExpanded(i_modelIdx);
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreePhysSizes::collapse( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    // If calling "collapseAll" the signal "collapsed" is not invoked for all
    // nodes of the tree view. This is a workaround to store the "expanded"
    // state of the model tree node in order to restore this state on changing
    // the sort order of the model.
    QTreeView::collapse(i_modelIdx);
    if( i_modelIdx.isValid() )
    {
        onCollapsed(i_modelIdx);
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreePhysSizes::onCollapsed( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    if( i_modelIdx.isValid() )
    {
        CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(model());

        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            pModelIdxTree->setIsExpanded(pModelBranch, false);
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreePhysSizes::onExpanded( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
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
    }
}

/*==============================================================================
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreePhysSizes::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    m_modelIdxSelectedOnMousePressEvent = indexAt(i_pEv->pos());

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

        if( pModelTreeEntry != nullptr )
        {
            if( i_pEv->buttons() & Qt::RightButton )
            {
                if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
                {
                    if( m_modelIdxSelectedOnMousePressEvent.column() == CModelIdxTreePhysSizes::EColumnTreeEntryName )
                    {
                        if( m_pMenuNameSpaceContext != nullptr )
                        {
                            m_pActionNameSpaceTitle->setText( "Name Space: " + pModelTreeEntry->name() );
                            m_pMenuNameSpaceContext->popup(QWidget::mapToGlobal(i_pEv->pos()));
                        }
                        bEventHandled = true;
                    }
                }
            } // if( i_pEv->buttons() & Qt::RightButton )
        } // if( pModelTreeEntry != nullptr )
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

    if( !bEventHandled )
    {
        QTreeView::mousePressEvent(i_pEv);
    }

} // mousePressEvent

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreePhysSizes::onActionNameSpaceExpandTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        expandRecursive(m_modelIdxSelectedOnMousePressEvent);
        for( int idxClm = 0; idxClm < CModelIdxTree::EColumnCount; idxClm++ )
        {
            resizeColumnToContents(idxClm);
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreePhysSizes::onActionNameSpaceCollapseTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        collapseRecursive(m_modelIdxSelectedOnMousePressEvent);
    }
}
