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
#include <QtGui/qevent.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qmenu.h>
#else
#include <QtWidgets/qmenu.h>
#endif

#include "ZSSysGUI/ZSSysIdxTreeTableViewBranchContent.h"
#include "ZSSysGUI/ZSSysIdxTreeModelBranchContent.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CTableViewIdxTreeBranchContent : public QTableView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTableViewIdxTreeBranchContent::CTableViewIdxTreeBranchContent(
    CIdxTree* i_pIdxTree,
    QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QTableView(i_pWdgtParent),
    m_pIdxTree(nullptr),
    m_pModel(nullptr),
    //m_strKeyInTreeOfRootEntry(),
    m_pMenuBranchContext(nullptr),
    m_pActionBranchTitle(nullptr),
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
    m_pTrcAdminObj(nullptr)
{
    setObjectName( QString(i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName()) );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    m_pModel = new CModelIdxTreeBranchContent(nullptr, nullptr);

    setModel(m_pModel);

    QObject::connect(
        m_pModel, static_cast<void (CModelIdxTreeBranchContent::*)(EIdxTreeSortOrder)>(&CModelIdxTreeBranchContent::sortOrderChanged),
        this, &CTableViewIdxTreeBranchContent::onModelSortOrderChanged );

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);

    // Context Popup Menu of Branches
    //-------------------------------

    m_pMenuBranchContext = new QMenu("Branch", this);

    m_pActionBranchTitle = new QAction("Branch:", this);

    QFont fntActionTitle = m_pActionBranchTitle->font();
    fntActionTitle.setBold(true);
    m_pActionBranchTitle->setFont(fntActionTitle);
    m_pMenuBranchContext->addAction(m_pActionBranchTitle);

    m_pMenuBranchContext->addSeparator();

    m_pActionBranchCreateNewBranch = new QAction("Create New Branch", this);
    m_pMenuBranchContext->addAction(m_pActionBranchCreateNewBranch);

    QObject::connect(
        m_pActionBranchCreateNewBranch, &QAction::triggered,
        this, &CTableViewIdxTreeBranchContent::onActionBranchCreateNewBranchTriggered );

    m_pActionBranchCreateNewLeave = new QAction("Create New Leave", this);
    m_pMenuBranchContext->addAction(m_pActionBranchCreateNewLeave);

    QObject::connect(
        m_pActionBranchCreateNewLeave, &QAction::triggered,
        this, &CTableViewIdxTreeBranchContent::onActionBranchCreateNewLeaveTriggered );

    m_pActionBranchDelete = new QAction("Delete", this);
    m_pMenuBranchContext->addAction(m_pActionBranchDelete);

    QObject::connect(
        m_pActionBranchDelete, &QAction::triggered,
        this, &CTableViewIdxTreeBranchContent::onActionBranchDeleteTriggered );

    m_pMenuBranchContext->addSeparator();

    m_pActionBranchCut = new QAction("Cut", this);
    m_pMenuBranchContext->addAction(m_pActionBranchCut);

    QObject::connect(
        m_pActionBranchCut, &QAction::triggered,
        this, &CTableViewIdxTreeBranchContent::onActionBranchCutTriggered );

    m_pActionBranchCopy = new QAction("Copy", this);
    m_pMenuBranchContext->addAction(m_pActionBranchCopy);

    QObject::connect(
        m_pActionBranchCopy, &QAction::triggered,
        this, &CTableViewIdxTreeBranchContent::onActionBranchCopyTriggered );

    m_pActionBranchPaste = new QAction("Paste", this);
    m_pMenuBranchContext->addAction(m_pActionBranchPaste);

    QObject::connect(
        m_pActionBranchPaste, &QAction::triggered,
        this, &CTableViewIdxTreeBranchContent::onActionBranchPasteTriggered );

    // Context Popup Menu of Leaves
    //-------------------------------

    m_pMenuLeaveContext = new QMenu("Leave", this);

    m_pActionLeaveTitle = new QAction("Leave:", this);

    m_pActionLeaveTitle->setFont(fntActionTitle);
    m_pMenuLeaveContext->addAction(m_pActionLeaveTitle);

    m_pMenuLeaveContext->addSeparator();

    m_pActionLeaveDelete = new QAction("Delete", this);
    m_pMenuLeaveContext->addAction(m_pActionLeaveDelete);

    QObject::connect(
        m_pActionLeaveDelete, &QAction::triggered,
        this, &CTableViewIdxTreeBranchContent::onActionLeaveDeleteTriggered );

    m_pMenuLeaveContext->addSeparator();

    m_pActionLeaveCut = new QAction("Cut", this);
    m_pMenuLeaveContext->addAction(m_pActionLeaveCut);

    QObject::connect(
        m_pActionLeaveCut, &QAction::triggered,
        this, &CTableViewIdxTreeBranchContent::onActionLeaveCutTriggered );

    m_pActionLeaveCopy = new QAction("Copy", this);
    m_pMenuLeaveContext->addAction(m_pActionLeaveCopy);

    QObject::connect(
        m_pActionLeaveCopy, &QAction::triggered,
        this, &CTableViewIdxTreeBranchContent::onActionLeaveCopyTriggered );

    m_pActionLeavePaste = new QAction("Paste", this);
    m_pMenuLeaveContext->addAction(m_pActionLeavePaste);

    QObject::connect(
        m_pActionLeavePaste, &QAction::triggered,
        this, &CTableViewIdxTreeBranchContent::onActionLeavePasteTriggered );

    if( i_pIdxTree != nullptr )
    {
        setIdxTree(i_pIdxTree);
    }
} // ctor

//------------------------------------------------------------------------------
CTableViewIdxTreeBranchContent::~CTableViewIdxTreeBranchContent()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    try
    {
        delete m_pModel;
    }
    catch(...)
    {
    }

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }

    m_pIdxTree = nullptr;
    m_pModel = nullptr;
    //m_strKeyInTreeOfRootEntry;
    m_pMenuBranchContext = nullptr;
    m_pActionBranchTitle = nullptr;
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
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::setIdxTree( CIdxTree* i_pIdxTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
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
                this, &CTableViewIdxTreeBranchContent::onIdxTreeAboutToBeDestroyed);
        }

        m_pIdxTree = i_pIdxTree;

        if( m_pIdxTree != nullptr )
        {
            QObject::connect(
                m_pIdxTree, &CIdxTree::aboutToBeDestroyed,
                this, &CTableViewIdxTreeBranchContent::onIdxTreeAboutToBeDestroyed);
        }

        m_pModel->setIdxTree(i_pIdxTree);
    }
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strKeyInTree;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setKeyInTreeOfRootEntry",
        /* strMethodInArgs    */ strMthInArgs );

    m_pModel->setKeyInTreeOfRootEntry(i_strKeyInTree);
}

//------------------------------------------------------------------------------
QString CTableViewIdxTreeBranchContent::getKeyInTreeOfRootEntry() const
//------------------------------------------------------------------------------
{
    return m_pModel->getKeyInTreeOfRootEntry();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::setSortOrder( EIdxTreeSortOrder i_sortOrder )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = idxTreeSortOrder2Str(i_sortOrder);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setSortOrder",
        /* strMethodInArgs    */ strMthInArgs );

    m_pModel->setSortOrder(i_sortOrder);
}

//------------------------------------------------------------------------------
EIdxTreeSortOrder CTableViewIdxTreeBranchContent::sortOrder() const
//------------------------------------------------------------------------------
{
    return m_pModel->sortOrder();
}

/*==============================================================================
protected: // slots
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::onModelSortOrderChanged(EIdxTreeSortOrder i_sortOrder)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = idxTreeSortOrder2Str(i_sortOrder);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onModelSortOrderChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit sortOrderChanged(i_sortOrder);
    emit sortOrderChanged(idxTreeSortOrder2Str(i_sortOrder));
}

/*==============================================================================
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "{" + qKeyEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "keyPressEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;

    if( !bEventHandled )
    {
        QTableView::keyPressEvent(i_pEv);
    }

} // keyPressEvent

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "{" + qMouseEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "mousePressEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;

    m_modelIdxSelectedOnMousePressEvent = indexAt(i_pEv->pos());

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs = "ModelIdxPressed {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
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
                        m_pMenuBranchContext->popup(QWidget::mapToGlobal(i_pEv->pos()));
                    }
                    bEventHandled = true;

                } // if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )

                else if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Leave )
                {
                    if( m_pMenuLeaveContext != nullptr )
                    {
                        m_pActionLeaveTitle->setText( "Leave: " + pModelTreeEntry->name() );
                        m_pMenuLeaveContext->popup(QWidget::mapToGlobal(i_pEv->pos()));
                    }
                    bEventHandled = true;

                } // if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Leave )
            } // if( i_pEv->buttons() & Qt::RightButton )
        } // if( pModelTreeEntry != nullptr )
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

    if( !bEventHandled )
    {
        QTableView::mousePressEvent(i_pEv);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "{" + qMouseEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "mouseReleaseEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;

    m_modelIdxSelectedOnMouseReleaseEvent = indexAt(i_pEv->pos());

    if( m_modelIdxSelectedOnMouseReleaseEvent.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs = "ModelIdxReleased {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedOnMouseReleaseEvent) + "}";
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
        QTableView::mouseReleaseEvent(i_pEv);
    }

} // mouseReleaseEvent

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::onActionBranchCreateNewBranchTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionBranchCreateNewBranchTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionBranchCreateNewBranchTriggered

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::onActionBranchCreateNewLeaveTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionBranchCreateNewLeaveTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionBranchCreateNewLeaveTriggered

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::onActionBranchDeleteTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionBranchDeleteTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionBranchDeleteTriggered

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::onActionBranchCutTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionBranchCutTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionBranchCutTriggered

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::onActionBranchCopyTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionBranchCopyTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionBranchCopyTriggered

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::onActionBranchPasteTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionBranchPasteTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionBranchPasteTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::onActionLeaveDeleteTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionLeaveDeleteTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionLeaveDeleteTriggered

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::onActionLeaveCutTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionLeaveCutTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionLeaveCutTriggered

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::onActionLeaveCopyTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionLeaveCopyTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionLeaveCopyTriggered

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::onActionLeavePasteTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionLeavePasteTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionLeavePasteTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::onIdxTreeAboutToBeDestroyed()
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
