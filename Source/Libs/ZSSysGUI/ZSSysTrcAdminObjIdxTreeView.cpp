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

#include <QtCore/qcoreapplication.h>
#include <QtGui/qevent.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qmenu.h>
#include <QtGui/qinputdialog.h>
#else
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qinputdialog.h>
#endif

#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeView.h"
#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeModel.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;


/*******************************************************************************
class CDelegateIdxTreeTrcAdminObjs : public QAbstractItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDelegateIdxTreeTrcAdminObjs::CDelegateIdxTreeTrcAdminObjs( QObject* i_pObjParent, int i_iTrcDetailLevel ) :
//------------------------------------------------------------------------------
    QItemDelegate(i_pObjParent),
    m_rectChkBoxTraceEnabled(0,0,10,10),
    m_iTrcDetailLevel(i_iTrcDetailLevel)
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Parent: " + QString(i_pObjParent == nullptr ? "nullptr" : i_pObjParent->objectName());
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

} // ctor

//------------------------------------------------------------------------------
CDelegateIdxTreeTrcAdminObjs::~CDelegateIdxTreeTrcAdminObjs()
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

    m_rectChkBoxTraceEnabled = QRect(0, 0, 0, 0);
    m_iTrcDetailLevel = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDelegateIdxTreeTrcAdminObjs::isCheckBoxEnabledHit(
    const QRect&       i_rectVisual,
    const QPoint&      i_ptPos,
    const QModelIndex& /*i_modelIdx*/ )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs = "RectVisual: " + qRect2Str(i_rectVisual);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "isCheckBoxEnabledHit",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIsHit = false;

    // If the check box is bigger than the visual rect ..
    if( i_rectVisual.width() <= m_rectChkBoxTraceEnabled.width()
     && i_rectVisual.height() <= m_rectChkBoxTraceEnabled.height() )
    {
        bIsHit = true;
    }
    // If the check box is smaller than the visual rect ..
    else
    {
        QRect rectCheckBox = m_rectChkBoxTraceEnabled;

        rectCheckBox.moveLeft(i_rectVisual.left());
        rectCheckBox.moveTop(i_rectVisual.top());

        if( i_rectVisual.width() > m_rectChkBoxTraceEnabled.width() )
        {
            rectCheckBox.moveLeft(i_rectVisual.left()+(i_rectVisual.width()-m_rectChkBoxTraceEnabled.width())/2);
        }
        if( i_rectVisual.height() > m_rectChkBoxTraceEnabled.height() )
        {
            rectCheckBox.moveTop(i_rectVisual.top()+(i_rectVisual.height()-m_rectChkBoxTraceEnabled.height())/2);
        }
        if( rectCheckBox.contains(i_ptPos) )
        {
            bIsHit = true;
        }
    }

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(bIsHit);
    }

    return bIsHit;

} // isCheckBoxEnabledHit

/*==============================================================================
public: // overridables of base class QItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
void CDelegateIdxTreeTrcAdminObjs::paint(
    QPainter*                   i_pPainter,
    const QStyleOptionViewItem& i_styleOption,
    const QModelIndex&          i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "paint",
        /* strMethodInArgs    */ strMthInArgs );

    bool bPainted = false;

    const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    CModelIdxTreeEntry*       pVModelTreeEntry = const_cast<CModelIdxTreeEntry*>(pCModelTreeEntry);

    CTrcAdminObj*        pTrcAdminObj = nullptr;
    QStyleOptionViewItem styleOption  = i_styleOption;

    if( pVModelTreeEntry != nullptr )
    {
        pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pVModelTreeEntry->treeEntry());
    }

    switch( i_modelIdx.column() )
    {
        case CModelIdxTreeTrcAdminObjs::EColumnTreeEntryName:
        {
            if( pTrcAdminObj != nullptr )
            {
                QRect   rctDecoration(styleOption.rect.left(),styleOption.rect.top(),22,styleOption.rect.height());
                QRect   rctText(rctDecoration.right()+1,styleOption.rect.top(),styleOption.rect.width()-rctDecoration.width(),styleOption.rect.height());
                QPixmap pxm;
                QString strNodeName;

                if( pVModelTreeEntry->isSelected() )
                {
                    pxm = CModelIdxTreeTrcAdminObjs::GetIcon(pVModelTreeEntry->entryType()).pixmap(rctDecoration.size());
                }
                else
                {
                    pxm = CModelIdxTreeTrcAdminObjs::GetIcon(pVModelTreeEntry->entryType()).pixmap(rctDecoration.size());
                }
                strNodeName = pTrcAdminObj->name();

                QItemDelegate::drawDecoration(i_pPainter,styleOption,rctDecoration,pxm);
                QItemDelegate::drawDisplay(i_pPainter,styleOption,rctText,strNodeName);

                if( styleOption.state & QStyle::State_HasFocus )
                {
                    QItemDelegate::drawFocus(i_pPainter,styleOption,rctText);
                }
                bPainted = true;
            }
            break;
        } // case CModelIdxTreeTrcAdminObjs::EColumnNodeName

        case CModelIdxTreeTrcAdminObjs::EColumnEnabled:
        {
            if( pTrcAdminObj != nullptr )
            {
                QRect          rectCheckBox = m_rectChkBoxTraceEnabled;
                Qt::CheckState checkState   = Qt::Unchecked;

                rectCheckBox.moveLeft(styleOption.rect.left());
                rectCheckBox.moveTop(styleOption.rect.top());

                //if( pVModelTreeEntry->getStyleState() & QStyle::State_Selected )
                //{
                //    styleOption.state |= QStyle::State_Selected;
                //}
                if( styleOption.rect.width() > m_rectChkBoxTraceEnabled.width() )
                {
                    rectCheckBox.moveLeft(styleOption.rect.left()+(styleOption.rect.width()-m_rectChkBoxTraceEnabled.width())/2);
                }
                if( styleOption.rect.height() > m_rectChkBoxTraceEnabled.height() )
                {
                    rectCheckBox.moveTop(styleOption.rect.top()+(styleOption.rect.height()-m_rectChkBoxTraceEnabled.height())/2);
                }
                if( pTrcAdminObj->getEnabled() == EEnabled::Yes )
                {
                    checkState = Qt::Checked;
                }
                else
                {
                    checkState = Qt::Unchecked;
                }

                QItemDelegate::drawBackground(i_pPainter,styleOption,i_modelIdx);
                QItemDelegate::drawCheck(i_pPainter,styleOption,rectCheckBox,checkState);

                if( styleOption.state & QStyle::State_HasFocus )
                {
                    QItemDelegate::drawFocus(i_pPainter,styleOption,styleOption.rect);
                }

                //QBrush             brush;
                //QPaintDevice*      pPaintDevice = nullptr;
                //QWidget*           pWdgt = nullptr;
                //QStyleOptionButton styleOptionFocusRect;
                //QStyleOptionButton styleOptionChkBox;

                //if( styleOption.state & QStyle::State_Selected )
                //{
                //    i_pPainter->save();
                //    brush = styleOption.palette.brush(QPalette::Normal,QPalette::Highlight);
                //    brush.setStyle(Qt::SolidPattern);
                //    i_pPainter->setBrush(brush);
                //    i_pPainter->setPen(Qt::NoPen);
                //    i_pPainter->drawRect(styleOption.rect);
                //    i_pPainter->restore();
                //}

                //pPaintDevice = i_pPainter->device();
                //if( pPaintDevice != nullptr )
                //{
                //    pWdgt = dynamic_cast<QWidget*>(pPaintDevice);
                //}
                //if( pWdgt != nullptr )
                //{
                //    styleOptionChkBox.initFrom(pWdgt);
                //    styleOptionChkBox.rect = rectCheckBox;
                //    styleOptionChkBox.state = QStyle::State_Enabled;
                //    if( checkState == Qt::Checked )
                //    {
                //        styleOptionChkBox.state |= QStyle::State_On;
                //    }
                //    else
                //    {
                //        styleOptionChkBox.state |= QStyle::State_Off;
                //    }
                //    pWdgt->style()->drawControl( QStyle::CE_CheckBox, &styleOptionChkBox, i_pPainter, pWdgt );
                //}

                bPainted = true;
            }
            break;
        } // case CModelIdxTreeTrcAdminObjs::EColumnEnabled

        default:
        {
            break;
        }
    } // switch( i_modelIdx.column() )

    if( !bPainted )
    {
        QItemDelegate::paint(i_pPainter,styleOption,i_modelIdx);
    }

} // paint


/*******************************************************************************
class CTreeViewIdxTreeTrcAdminObjs : public QTreeView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTreeViewIdxTreeTrcAdminObjs::CTreeViewIdxTreeTrcAdminObjs(
    CModelIdxTreeTrcAdminObjs* i_pModel,
    QWidget*                   i_pWdgtParent,
    int                        i_iTrcDetailLevel ) :
//------------------------------------------------------------------------------
    QTreeView(i_pWdgtParent),
    m_pDelegate(nullptr),
    m_pMenuNameSpaceContext(nullptr),
    m_pActionNameSpaceTitle(nullptr),
    m_pActionNameSpaceExpand(nullptr),
    m_pActionNameSpaceCollapse(nullptr),
    m_pActionNameSpaceEnableAdmObjects(nullptr),
    m_pActionNameSpaceDisableAdmObjects(nullptr),
    m_pActionNameSpaceSetDetailLevelAdmObjects(nullptr),
    m_modelIdxSelectedOnMousePressEvent(),
    m_modelIdxSelectedOnMouseReleaseEvent(),
    m_iTrcDetailLevel(i_iTrcDetailLevel)
{
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

    setModel(i_pModel);

    m_pDelegate = new CDelegateIdxTreeTrcAdminObjs(this);

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAlternatingRowColors(true);
    setAllColumnsShowFocus(true);

    //hideColumn(CModelIdxTree::EColumnTreeEntryName);
    hideColumn(CModelIdxTree::EColumnInternalId);
    hideColumn(CModelIdxTree::EColumnTreeEntryType);
    //hideColumn(CModelIdxTree::EColumnIdxInTree);
    hideColumn(CModelIdxTree::EColumnIdxInParentBranch);
    hideColumn(CModelIdxTree::EColumnKeyInTree);
    hideColumn(CModelIdxTree::EColumnKeyInParentBranch);
    hideColumn(CModelIdxTreeTrcAdminObjs::EColumnObjAddress);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnRefCount);
    hideColumn(CModelIdxTreeTrcAdminObjs::EColumnNameSpace);
    hideColumn(CModelIdxTreeTrcAdminObjs::EColumnClassName);
    hideColumn(CModelIdxTreeTrcAdminObjs::EColumnObjName);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnObjThreadName);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnEnabled);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnDetailLevel);

    setItemDelegate(m_pDelegate);

    // Context Popup Menu of Name Space Branches
    //------------------------------------------

    m_pMenuNameSpaceContext = new QMenu("Name Space",this);

    m_pActionNameSpaceTitle = new QAction("Name Space:",this);

    QPixmap pxmExpandAll(":/ZS/TreeView/TreeViewExpandAll.bmp");
    pxmExpandAll.setMask(pxmExpandAll.createHeuristicMask());
    //pxmExpandAll = pxmExpandAll.scaled(16,16);
    m_pActionNameSpaceExpand = new QAction(pxmExpandAll, "Expand", this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceExpand);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionNameSpaceExpand,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionNameSpaceExpandTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    QPixmap pxmCollapseAll(":/ZS/TreeView/TreeViewCollapseAll.bmp");
    pxmCollapseAll.setMask(pxmCollapseAll.createHeuristicMask());

    m_pActionNameSpaceCollapse = new QAction(pxmCollapseAll, "Collapse", this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceCollapse);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionNameSpaceCollapse,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionNameSpaceCollapseTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pMenuNameSpaceContext->addSeparator();

    QFont fntActionTitle = m_pActionNameSpaceTitle->font();
    fntActionTitle.setBold(true);
    m_pActionNameSpaceTitle->setFont(fntActionTitle);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceTitle);

    m_pActionNameSpaceEnableAdmObjects = new QAction("Recursively Enable Admin Objects",this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceEnableAdmObjects);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionNameSpaceEnableAdmObjects,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionNameSpaceEnableAdmObjectsTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pActionNameSpaceDisableAdmObjects = new QAction("Recursively Disable Admin Objects",this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceDisableAdmObjects);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionNameSpaceDisableAdmObjects,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionNameSpaceDisableAdmObjectsTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pMenuNameSpaceContext->addSeparator();

    m_pActionNameSpaceSetDetailLevelAdmObjects = new QAction("Recursively Set Detail Level of Admin Objects",this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceSetDetailLevelAdmObjects);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionNameSpaceSetDetailLevelAdmObjects,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionNameSpaceSetDetailLevelAdmObjectsTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CTreeViewIdxTreeTrcAdminObjs::~CTreeViewIdxTreeTrcAdminObjs()
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

    m_pDelegate = nullptr;
    m_pMenuNameSpaceContext = nullptr;
    m_pActionNameSpaceTitle = nullptr;
    m_pActionNameSpaceExpand = nullptr;
    m_pActionNameSpaceCollapse = nullptr;
    m_pActionNameSpaceEnableAdmObjects = nullptr;
    m_pActionNameSpaceDisableAdmObjects = nullptr;
    m_pActionNameSpaceSetDetailLevelAdmObjects = nullptr;
    //m_modelIdxSelectedOnMousePressEvent;
    //m_modelIdxSelectedOnMouseReleaseEvent;
    m_iTrcDetailLevel = 0;

} // dtor

/*==============================================================================
public: // overridable slots of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::expandAll()
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
        /* strMethod          */ "expandAll",
        /* strMethodInArgs    */ strMthInArgs );

    QTreeView::expandAll();

    QModelIndex modelIdxRoot = model()->index(0, 0, QModelIndex());

    expandRecursive(modelIdxRoot);

} // expandAll

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::collapseAll()
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
        /* strMethod          */ "collapseAll",
        /* strMethodInArgs    */ strMthInArgs );

    QTreeView::collapseAll();

    QModelIndex modelIdxRoot = model()->index(0, 0, QModelIndex());

    collapseRecursive(modelIdxRoot);

} // collapseAll

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::expandRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "expandRecursive",
        /* strMethodInArgs    */ strMthInArgs );

    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

    if( pModelTreeEntry != nullptr )
    {
        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
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

        } // if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
    } // if( pModelTreeEntry != nullptr )

} // expandRecursive

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::collapseRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "collapsRecursive",
        /* strMethodInArgs    */ strMthInArgs );

    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

    if( pModelTreeEntry != nullptr )
    {
        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
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

        } // if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
    } // if( pModelTreeEntry != nullptr )

} // collapseRecursive

/*==============================================================================
public: // slots (hiding not overridable slots with same name in QTreeView)
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::expand( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "expand",
        /* strMethodInArgs    */ strMthInArgs );

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
void CTreeViewIdxTreeTrcAdminObjs::collapse( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "collapse",
        /* strMethodInArgs    */ strMthInArgs );

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
void CTreeViewIdxTreeTrcAdminObjs::onCollapsed( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onCollapsed",
        /* strMethodInArgs    */ strMthInArgs );

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
void CTreeViewIdxTreeTrcAdminObjs::onExpanded( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onExpanded",
        /* strMethodInArgs    */ strMthInArgs );

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

/*==============================================================================
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = qKeyEvent2Str(i_pEv);
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

    if( i_pEv->key() == Qt::Key_Return
     || i_pEv->key() == Qt::Key_Enter
     || i_pEv->key() == Qt::Key_Space
     || i_pEv->key() == Qt::Key_F5 )
    {
        QModelIndex                modelIdxSelected = selectionModel()->currentIndex();
        CModelIdxTreeEntry*        pModelTreeEntry = nullptr;
        CTrcAdminObj*              pTrcAdminObj = nullptr;
        QAbstractItemModel*        pModelAbstract = nullptr;
        CModelIdxTreeTrcAdminObjs* pTrcAdmObjIdxTreeModel = nullptr;

        pModelAbstract = const_cast<QAbstractItemModel*>(modelIdxSelected.model());

        if( pModelAbstract != nullptr )
        {
            pTrcAdmObjIdxTreeModel = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(pModelAbstract);
        }

        if( i_pEv->key() == Qt::Key_F5 )
        {
            if( pTrcAdmObjIdxTreeModel != nullptr )
            {
                //pTrcAdmObjIdxTreeModel->beginReset();
                //pTrcAdmObjIdxTreeModel->endReset();
            }
        }
        else // if( i_pEv->key() != Qt::Key_F5 )
        {
            if( modelIdxSelected.isValid() )
            {
                pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdxSelected.internalPointer());

                if( pModelTreeEntry != nullptr )
                {
                    pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pModelTreeEntry->treeEntry());
                }
            }

            if( pTrcAdminObj != nullptr )
            {
                if( modelIdxSelected.column() >= CModelIdxTree::EColumnCount )
                {
                    switch( modelIdxSelected.column() )
                    {
                        case CModelIdxTreeTrcAdminObjs::EColumnEnabled:
                        {
                            if( pTrcAdminObj->getEnabled() == EEnabled::Yes )
                            {
                                pTrcAdminObj->setEnabled(EEnabled::No);
                            }
                            else
                            {
                                pTrcAdminObj->setEnabled(EEnabled::Yes);
                            }
                            bEventHandled = true;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    } // switch( modelIdxSelected.column() )
                } // if( modelIdxSelected.column() >= CModelIdxTree::EColumnCount )
            } // if( pTrcAdminObj != nullptr )
        } // if( i_pEv->key() != Qt::Key_F5 )
    } // if( i_pEv->key() == Qt::Key_Return ...

    if( !bEventHandled )
    {
        QTreeView::keyPressEvent(i_pEv);
    }

} // keyPressEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = qMouseEvent2Str(i_pEv);
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

    //QModelIndex modelIdxSelected = selectionModel()->currentIndex();

    m_modelIdxSelectedOnMousePressEvent = indexAt(i_pEv->pos());

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

        if( pModelTreeEntry != nullptr )
        {
            if( i_pEv->buttons() & Qt::RightButton )
            {
                if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
                {
                    CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;

                    if( pModelBranch != nullptr )
                    {
                        switch( m_modelIdxSelectedOnMousePressEvent.column() )
                        {
                            case CModelIdxTreeTrcAdminObjs::EColumnTreeEntryName:
                            {
                                if( m_pMenuNameSpaceContext != nullptr )
                                {
                                    m_pActionNameSpaceTitle->setText( "Name Space: " + pModelBranch->name() );
                                    m_pMenuNameSpaceContext->popup(QWidget::mapToGlobal(i_pEv->pos()));
                                }
                                bEventHandled = true;
                                break;
                            }
                            case CModelIdxTreeTrcAdminObjs::EColumnRefCount:
                            case CModelIdxTreeTrcAdminObjs::EColumnNameSpace:
                            case CModelIdxTreeTrcAdminObjs::EColumnClassName:
                            case CModelIdxTreeTrcAdminObjs::EColumnObjName:
                            case CModelIdxTreeTrcAdminObjs::EColumnEnabled:
                            default:
                            {
                                break;
                            }
                        }
                    } // if( pTrcAdminObj != nullptr )
                }
            } // if( i_pEv->buttons() & Qt::RightButton )
        } // if( pModelTreeEntry != nullptr )
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

    if( !bEventHandled )
    {
        QTreeView::mousePressEvent(i_pEv);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = qMouseEvent2Str(i_pEv);
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

    //QModelIndex modelIdxSelected = selectionModel()->currentIndex();

    m_modelIdxSelectedOnMouseReleaseEvent = indexAt(i_pEv->pos());

    if( m_modelIdxSelectedOnMouseReleaseEvent.isValid() )
    {
        CDelegateIdxTreeTrcAdminObjs* pDelegate = dynamic_cast<CDelegateIdxTreeTrcAdminObjs*>(itemDelegate());
        CModelIdxTreeEntry*           pModelTreeEntry = nullptr;
        CTrcAdminObj*                 pTrcAdminObj = nullptr;

        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());

        if( pModelTreeEntry != nullptr )
        {
            pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pModelTreeEntry->treeEntry());
        }

        if( pTrcAdminObj != nullptr )
        {
            if( m_modelIdxSelectedOnMouseReleaseEvent.column() >= CModelIdxTree::EColumnCount )
            {
                switch( m_modelIdxSelectedOnMouseReleaseEvent.column() )
                {
                    case CModelIdxTreeTrcAdminObjs::EColumnEnabled:
                    {
                        QRect rectVisual = visualRect(m_modelIdxSelectedOnMouseReleaseEvent);

                        if( pDelegate != nullptr && pDelegate->isCheckBoxEnabledHit(rectVisual,i_pEv->pos(),m_modelIdxSelectedOnMouseReleaseEvent) )
                        {
                            if( pTrcAdminObj->getEnabled() == EEnabled::Yes )
                            {
                                pTrcAdminObj->setEnabled(EEnabled::No);
                            }
                            else
                            {
                                pTrcAdminObj->setEnabled(EEnabled::Yes);
                            }
                            bEventHandled = true;
                        }
                        break;
                    }
                    default:
                    {
                        break;
                    }
                } // switch( modelIdxSelected.column() )
            } // if( m_modelIdxSelectedOnMouseReleaseEvent.column() >= CModelIdxTree::EColumnCount )
        } // if( pTrcAdminObj != nullptr )
    } // if( m_modelIdxSelectedOnMouseReleaseEvent.isValid() )

    if( !bEventHandled )
    {
        QTreeView::mouseReleaseEvent(i_pEv);
    }

} // mouseReleaseEvent

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceExpandTriggered( bool i_bChecked )
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
        /* strMethod          */ "onActionNameSpaceExpandTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        expandRecursive(m_modelIdxSelectedOnMousePressEvent);

        for( int idxClm = 0; idxClm < CModelIdxTree::EColumnCount; idxClm++ )
        {
            resizeColumnToContents(idxClm);
        }
    }
} // onActionNameSpaceExpandTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceCollapseTriggered( bool i_bChecked )
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
        /* strMethod          */ "onActionNameSpaceCollapseTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        collapseRecursive(m_modelIdxSelectedOnMousePressEvent);
    }
} // onActionNameSpaceCollapseTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceEnableAdmObjectsTriggered( bool i_bChecked )
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
        /* strMethod          */ "onActionNameSpaceEnableAdmObjectsTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeTrcAdminObjs* pModelIdxTree = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(model());

        if( pModelIdxTree != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pModelIdxTree->idxTree();

            CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());

            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;

            if( pIdxTree != nullptr && pModelBranch != nullptr )
            {
                CIdxTreeEntry* pBranch = pModelBranch->treeEntry();

                pIdxTree->setEnabled(pBranch, EEnabled::Yes);
            }
        }
    }

} // onActionNameSpaceEnableAdmObjectsTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceDisableAdmObjectsTriggered( bool i_bChecked )
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
        /* strMethod          */ "onActionNameSpaceDisableAdmObjectsTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeTrcAdminObjs* pModelIdxTree = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(model());

        if( pModelIdxTree != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pModelIdxTree->idxTree();

            CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());

            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;

            if( pIdxTree != nullptr && pModelBranch != nullptr )
            {
                CIdxTreeEntry* pBranch = pModelBranch->treeEntry();

                pIdxTree->setEnabled(pBranch, EEnabled::No);
            }
        }
    }

} // onActionNameSpaceDisableAdmObjectsTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetDetailLevelAdmObjectsTriggered( bool i_bChecked )
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
        /* strMethod          */ "onActionNameSpaceSetDetailLevelAdmObjectsTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeTrcAdminObjs* pModelIdxTree = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(model());

        if( pModelIdxTree != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pModelIdxTree->idxTree();

            CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());

            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;

            if( pIdxTree != nullptr && pModelBranch != nullptr )
            {
                CIdxTreeEntry* pBranch = pModelBranch->treeEntry();

                bool bOk;

                int iDetailLevel = QInputDialog::getInt(
                    /* pWdgtParent */ this,
                    /* strTitle    */ QCoreApplication::applicationName(),
                    /* strLabel    */ 0,
                    /* iValue      */ 0,
                    /* iMinValue   */ 0,
                    /* iMaxValue   */ 10,
                    /* iStep       */ 1,
                    /* pbOk        */ &bOk );

                if( bOk )
                {
                    pIdxTree->setTraceDetailLevel(pBranch, iDetailLevel);
                }
            }
        }
    }

} // onActionNameSpaceSetDetailLevelAdmObjectsTriggered
