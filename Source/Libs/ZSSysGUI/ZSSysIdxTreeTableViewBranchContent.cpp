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
#include <QtGui/qevent.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qmenu.h>
#else
#include <QtWidgets/qmenu.h>
#endif

#include "ZSSysGUI/ZSSysIdxTreeTableViewBranchContent.h"
#include "ZSSysGUI/ZSSysIdxTreeModelBranchContent.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntries.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Trace;


/*******************************************************************************
class CTableViewIdxTreeBranchContent : public QTableView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTableViewIdxTreeBranchContent::CTableViewIdxTreeBranchContent(
    CModelIdxTree* i_pModelIdxTree,
    QWidget*       i_pWdgtParent,
    int            i_iTrcDetailLevel ) :
//------------------------------------------------------------------------------
    QTableView(i_pWdgtParent),
    m_pModel(nullptr),
    m_pBranch(nullptr),
    m_sortOrder(EIdxTreeSortOrder::Config),
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
    m_iTrcDetailLevel(i_iTrcDetailLevel)
{
    setObjectName( QString(i_pModelIdxTree == nullptr ? "IdxTree" : i_pModelIdxTree->objectName()) );

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "ModelIdxTree: " + QString(i_pModelIdxTree == nullptr ? "nullptr" : i_pModelIdxTree->objectName());
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

    m_pModel = new CModelIdxTreeBranchContent(i_pModelIdxTree->idxTree(), nullptr);

    setModel(m_pModel);

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

} // ctor

//------------------------------------------------------------------------------
CTableViewIdxTreeBranchContent::~CTableViewIdxTreeBranchContent()
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

    try
    {
        delete m_pModel;
    }
    catch(...)
    {
    }

    m_pModel = nullptr;
    m_pBranch = nullptr;
    m_sortOrder = static_cast<EIdxTreeSortOrder>(0);
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
    m_iTrcDetailLevel = 0;

} // dtor

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::setBranch( CBranchIdxTreeEntry* i_pBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setBranch",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pBranch != i_pBranch )
    {
        m_pBranch = i_pBranch;

        m_pModel->setBranch(m_pBranch);

    } // if( m_pBranch != i_pBranch )

} // setBranch

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::setSortOrder( EIdxTreeSortOrder i_sortOrder )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "SortOrder: " + idxTreeSortOrder2Str(i_sortOrder);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setSortOrder",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_sortOrder != i_sortOrder )
    {
        m_sortOrder = i_sortOrder;

        m_pModel->setSortOrder(m_sortOrder);

    } // if( m_pBranch != i_sortOrder )

} // setSortOrder

/*==============================================================================
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewIdxTreeBranchContent::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iEventDetails = m_iTrcDetailLevel >= ETraceDetailLevelInternalStates ? 1 : 0;
        strMthInArgs = "Event {" + qKeyEvent2Str(i_pEv, iEventDetails) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
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

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iEventDetails = m_iTrcDetailLevel >= ETraceDetailLevelInternalStates ? 1 : 0;
        strMthInArgs = "Event {" + qMouseEvent2Str(i_pEv, iEventDetails) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "mousePressEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;

    m_modelIdxSelectedOnMousePressEvent = indexAt(i_pEv->pos());

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelAbstractTreeEntry* pModelTreeEntry = static_cast<CModelAbstractTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

        if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
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

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iEventDetails = m_iTrcDetailLevel >= ETraceDetailLevelInternalStates ? 1 : 0;
        strMthInArgs = "Event {" + qMouseEvent2Str(i_pEv, iEventDetails) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "mouseReleaseEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;

    m_modelIdxSelectedOnMouseReleaseEvent = indexAt(i_pEv->pos());

    if( m_modelIdxSelectedOnMouseReleaseEvent.isValid() )
    {
        CModelAbstractTreeEntry* pModelTreeEntry = static_cast<CModelAbstractTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());

        if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
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
        /* strMethod          */ "onActionLeavePasteTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionLeavePasteTriggered
