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

#include <QtWidgets/qapplication.h>
#include <QtGui/qevent.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qmenu.h>
#include <QtGui/qinputdialog.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qinputdialog.h>
#endif

#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeView.h"
#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeModel.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSysGUI/ZSSysEditEnumValueDlg.h"
#include "ZSSysGUI/ZSSysEditStringValueDlg.h"
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


/*******************************************************************************
class CDelegateIdxTreeTrcAdminObjs : public QStyledItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDelegateIdxTreeTrcAdminObjs::CDelegateIdxTreeTrcAdminObjs( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QStyledItemDelegate(i_pObjParent),
    m_rectChkBoxTraceEnabled(0, 0, 10, 10),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( QString(i_pObjParent == nullptr ? "IdxTree" : i_pObjParent->objectName()) );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Parent: " + QString(i_pObjParent == nullptr ? "nullptr" : i_pObjParent->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

} // ctor

//------------------------------------------------------------------------------
CDelegateIdxTreeTrcAdminObjs::~CDelegateIdxTreeTrcAdminObjs()
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

    m_rectChkBoxTraceEnabled = QRect(0, 0, 0, 0);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDelegateIdxTreeTrcAdminObjs::isCheckBoxEnabledHit(
    const QRect&       i_rectVisual,
    const QPoint&      i_ptPos,
    const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "RectVisual: " + qRect2Str(i_rectVisual);
        strMthInArgs += ", Pos: " + qPoint2Str(i_ptPos);
        strMthInArgs += ", ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
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

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "paint",
        /* strMethodInArgs    */ strMthInArgs );

    bool bHandled = false;

    if( i_modelIdx.isValid() )
    {
        const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

        QStyleOptionViewItem styleOption  = i_styleOption;

        switch( i_modelIdx.column() )
        {
            case CModelIdxTreeTrcAdminObjs::EColumnEnabled:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                    bool bVal = i_modelIdx.model()->data(i_modelIdx, Qt::DisplayRole).toBool();
                    QStyleOptionButton styleOption;
                    QRect rctEditor = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &styleOption);
                    styleOption.rect = i_styleOption.rect;
                    styleOption.rect.setLeft(i_styleOption.rect.x() + i_styleOption.rect.width()/2 - rctEditor.width()/2);
                    if( bVal ) {
                        styleOption.state = QStyle::State_On|QStyle::State_Enabled;
                    }
                    else {
                        styleOption.state = QStyle::State_Off|QStyle::State_Enabled;
                    }
                    QApplication::style()->drawControl(QStyle::CE_CheckBox, &styleOption, i_pPainter);
                    bHandled = true;
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnMethodCallsDetailLevel:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnRuntimeInfoDetailLevel:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                }
                break;
            }
            default:
            {
                break;
            }
        } // switch( i_modelIdx.column() )
    } // if( i_modelIdx.isValid() )

    if( !bHandled )
    {
        QStyledItemDelegate::paint(i_pPainter, i_styleOption, i_modelIdx);
    }

} // paint

//------------------------------------------------------------------------------
QWidget* CDelegateIdxTreeTrcAdminObjs::createEditor(
    QWidget* i_pWdgtParent,
    const QStyleOptionViewItem& i_styleOption,
    const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthReturn;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "createEditor",
        /* strMethodInArgs    */ strMthInArgs );

    QWidget* pWdgtEditor = nullptr;

    if( i_modelIdx.isValid() )
    {
        const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

        switch( i_modelIdx.column() )
        {
            case CModelIdxTreeTrcAdminObjs::EColumnEnabled:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                    // Dont't create an editor for the enabled check box. Otherwise the user
                    // would need to click twice on the cell in order to toggle the value.
                    // Changing the enabled value is realized by the TreeViewWidget's method
                    // "mouseReleaseEvent" if the enabled check box has been hit.
                    // pWdgtEditor = new QCheckBox(i_pWdgtParent);
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnMethodCallsDetailLevel:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                    QComboBox* pCmb = new QComboBox(i_pWdgtParent);
                    pCmb->setObjectName(pCModelTreeEntry->keyInTree() + ".MethodCalls");
                    pWdgtEditor = pCmb;
                    CEnumMethodTraceDetailLevel eDetailLevel;
                    for( eDetailLevel = 0; eDetailLevel < CEnumMethodTraceDetailLevel::count(); ++eDetailLevel )
                    {
                        pCmb->addItem(eDetailLevel.toString());
                    }
                    if( !QObject::connect(
                        /* pObjSender   */ pCmb,
                        /* szSignal     */ SIGNAL( activated(int) ),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT( onComboDetailLevelActivated(int) ) ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnRuntimeInfoDetailLevel:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                    QComboBox* pCmb = new QComboBox(i_pWdgtParent);
                    pCmb->setObjectName(pCModelTreeEntry->keyInTree() + ".RuntimeInfo");
                    pWdgtEditor = pCmb;
                    CEnumLogDetailLevel eDetailLevel;
                    for( eDetailLevel = 0; eDetailLevel < CEnumLogDetailLevel::count(); ++eDetailLevel )
                    {
                        pCmb->addItem(eDetailLevel.toString());
                    }
                    if( !QObject::connect(
                        /* pObjSender   */ pCmb,
                        /* szSignal     */ SIGNAL( activated(int) ),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT( onComboDetailLevelActivated(int) ) ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnDataFilter:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                    QLineEdit* pEdt = new QLineEdit(i_pWdgtParent);
                    pEdt->setObjectName(pCModelTreeEntry->keyInTree() + ".DataFilter");
                    pWdgtEditor = pEdt;
                    if( !QObject::connect(
                        /* pObjSender   */ pEdt,
                        /* szSignal     */ SIGNAL( editingFinished() ),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT( onEdtDataFilterEditingFinished() ) ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        } // switch( i_modelIdx.column() )
    } // if( i_modelIdx.isValid() )

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthReturn = pWdgtEditor == nullptr ? "nullptr" : pWdgtEditor->objectName();
        mthTracer.setMethodReturn(strMthReturn);
    }
    return pWdgtEditor;

} // createEditor

//------------------------------------------------------------------------------
void CDelegateIdxTreeTrcAdminObjs::setEditorData(
    QWidget* i_pWdgtEditor, const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "createEditor",
        /* strMethodInArgs    */ strMthInArgs );

    bool bHandled = false;

    if( i_modelIdx.isValid() )
    {
        const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

        switch( i_modelIdx.column() )
        {
            case CModelIdxTreeTrcAdminObjs::EColumnEnabled:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                    // No editor is used for the enabled check box. Otherwise the user
                    // would need to click twice on the cell in order to toggle the value.
                    // Changing the enabled value is realized by the TreeViewWidget's method
                    // "mouseReleaseEvent" if the enabled check box has been hit.
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnMethodCallsDetailLevel:
            case CModelIdxTreeTrcAdminObjs::EColumnRuntimeInfoDetailLevel:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                    QComboBox* pWdgtEditor = dynamic_cast<QComboBox*>(i_pWdgtEditor);
                    if( pWdgtEditor != nullptr )
                    {
                        QString strVal = i_modelIdx.data(Qt::EditRole).toString();
                        int idx = pWdgtEditor->findText(strVal);
                        if (idx >= 0) {
                            pWdgtEditor->setCurrentIndex(idx);
                        }
                        // Don't show popup immedetialy after setting the data.
                        // The popup will be closed immediately afterwards again by ?
                        // It is better to first select the field, show the combo box
                        // and click again to select a value. Double click to show
                        // the pulldown list is not so bad.
                        //pWdgtEditor->showPopup();
                        bHandled = true;
                    }
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnDataFilter:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                    QLineEdit* pWdgtEditor = dynamic_cast<QLineEdit*>(i_pWdgtEditor);
                    if( pWdgtEditor != nullptr )
                    {
                        QString strVal = i_modelIdx.data(Qt::EditRole).toString();
                        pWdgtEditor->setText(strVal);
                        bHandled = true;
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        } // switch( i_modelIdx.column() )
    } // if( i_modelIdx.isValid() )

    if( !bHandled )
    {
        QStyledItemDelegate::setEditorData(i_pWdgtEditor, i_modelIdx);
    }

} // setEditorData

//------------------------------------------------------------------------------
void CDelegateIdxTreeTrcAdminObjs::setModelData(
    QWidget* i_pWdgtEditor,
    QAbstractItemModel* i_pModel,
    const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setModelData",
        /* strMethodInArgs    */ strMthInArgs );

    bool bHandled = false;

    if( i_modelIdx.isValid() )
    {
        const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

        switch( i_modelIdx.column() )
        {
            case CModelIdxTreeTrcAdminObjs::EColumnEnabled:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                    // No editor is used for the enabled check box. Otherwise the user
                    // would need to click twice on the cell in order to toggle the value.
                    // Changing the enabled value is realized by the TreeViewWidget's method
                    // "mouseReleaseEvent" if the enabled check box has been hit.
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnMethodCallsDetailLevel:
            case CModelIdxTreeTrcAdminObjs::EColumnRuntimeInfoDetailLevel:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                    QComboBox* pWdgtEditor = dynamic_cast<QComboBox*>(i_pWdgtEditor);
                    if( pWdgtEditor != nullptr )
                    {
                        i_pModel->setData(i_modelIdx, pWdgtEditor->currentText(), Qt::EditRole);
                        bHandled = true;
                    }
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnDataFilter:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                    QLineEdit* pWdgtEditor = dynamic_cast<QLineEdit*>(i_pWdgtEditor);
                    if( pWdgtEditor != nullptr )
                    {
                        i_pModel->setData(i_modelIdx, pWdgtEditor->text(), Qt::EditRole);
                        bHandled = true;
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        } // switch( i_modelIdx.column() )
    } // if( i_modelIdx.isValid() )

    if( !bHandled )
    {
        QStyledItemDelegate::setModelData(i_pWdgtEditor, i_pModel, i_modelIdx);
    }

} // setModelData

//------------------------------------------------------------------------------
void CDelegateIdxTreeTrcAdminObjs::updateEditorGeometry(
    QWidget* i_pWdgtEditor,
    const QStyleOptionViewItem& i_styleOption,
    const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "updateEditorGeometry",
        /* strMethodInArgs    */ strMthInArgs );

    bool bHandled = false;

    if( i_modelIdx.isValid() )
    {
        const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

        switch( i_modelIdx.column() )
        {
            case CModelIdxTreeTrcAdminObjs::EColumnEnabled:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                    // No editor is used for the enabled check box. Otherwise the user
                    // would need to click twice on the cell in order to toggle the value.
                    // Changing the enabled value is realized by the TreeViewWidget's method
                    // "mouseReleaseEvent" if the enabled check box has been hit.
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnMethodCallsDetailLevel:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnRuntimeInfoDetailLevel:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnDataFilter:
            {
                if( pCModelTreeEntry->isLeave() )
                {
                }
                break;
            }
            default:
            {
                break;
            }
        } // switch( i_modelIdx.column() )
    } // if( i_modelIdx.isValid() )

    if( !bHandled )
    {
        QStyledItemDelegate::updateEditorGeometry(i_pWdgtEditor, i_styleOption, i_modelIdx);
    }

} // updateEditorGeometry

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDelegateIdxTreeTrcAdminObjs::onComboDetailLevelActivated( int i_idx )
//------------------------------------------------------------------------------
{
    QComboBox* pCmb = dynamic_cast<QComboBox*>(sender());

    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = pCmb == nullptr ? "null" : pCmb->objectName();
        strMthInArgs += ", " + QString(pCmb == nullptr ? QString::number(i_idx) : pCmb->currentText());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onComboDetailLevelActivated",
        /* strMethodInArgs    */ strMthInArgs );

    if( pCmb != nullptr )
    {
        emit commitData(pCmb);
        emit closeEditor(pCmb);
    }
}

//------------------------------------------------------------------------------
void CDelegateIdxTreeTrcAdminObjs::onEdtDataFilterEditingFinished()
//------------------------------------------------------------------------------
{
    QLineEdit* pEdt = dynamic_cast<QLineEdit*>(sender());

    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = pEdt == nullptr ? "null" : pEdt->objectName();
        strMthInArgs += ", " + QString(pEdt == nullptr ? "" : pEdt->text());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onEdtDataFilterEditingFinished",
        /* strMethodInArgs    */ strMthInArgs );
}


/*******************************************************************************
class CTreeViewIdxTreeTrcAdminObjs : public QTreeView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTreeViewIdxTreeTrcAdminObjs::CTreeViewIdxTreeTrcAdminObjs(
    CIdxTreeTrcAdminObjs* i_pIdxTree,
    QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QTreeView(i_pWdgtParent),
    m_pIdxTree(i_pIdxTree),
    m_pDelegate(nullptr),
    m_pModel(nullptr),
    m_pMenuNameSpaceContext(nullptr),
    m_pActionNameSpaceTitle(nullptr),
    m_pActionNameSpaceExpand(nullptr),
    m_pActionNameSpaceCollapse(nullptr),
    m_pActionNameSpaceEnableAdminObjs(nullptr),
    m_pActionNameSpaceDisableAdminObjs(nullptr),
    m_pActionNameSpaceSetAdminObjsMethodCallsDetailLevel(nullptr),
    m_pActionNameSpaceSetAdminObjsRuntimeInfoDetailLevel(nullptr),
    m_pActionNameSpaceSetAdminObjsTraceDataFilter(nullptr),
    m_modelIdxSelectedOnMousePressEvent(),
    m_modelIdxSelectedOnMouseReleaseEvent(),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( QString(i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName()) );

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

    m_pModel = new CModelIdxTreeTrcAdminObjs(m_pIdxTree, nullptr);

    setModel(m_pModel);

    m_pDelegate = new CDelegateIdxTreeTrcAdminObjs(this);

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAlternatingRowColors(true);
    setAllColumnsShowFocus(true);
    setEditTriggers(QAbstractItemView::AllEditTriggers);

    //hideColumn(CModelIdxTree::EColumnTreeEntryName);
    hideColumn(CModelIdxTree::EColumnTreeEntryNameDecorated);
    hideColumn(CModelIdxTree::EColumnTreeEntryTypeImageUrl);
    hideColumn(CModelIdxTree::EColumnTreeEntryTypeIcon);
    hideColumn(CModelIdxTree::EColumnTreeEntryType);
    hideColumn(CModelIdxTree::EColumnInternalId);
    hideColumn(CModelIdxTree::EColumnIdxInTree);
    hideColumn(CModelIdxTree::EColumnIdxInParentBranch);
    hideColumn(CModelIdxTree::EColumnKeyInTree);
    hideColumn(CModelIdxTree::EColumnKeyInParentBranch);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnRefCount);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnEnabled);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnMethodCallsDetailLevel);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnRuntimeInfoDetailLevel);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnDataFilter);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnNameSpace);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnClassName);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnObjName);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnObjThreadName);
    hideColumn(CModelIdxTreeTrcAdminObjs::EColumnObjAddress);

    setItemDelegate(m_pDelegate);

    // Context Popup Menu of Name Space Branches
    //------------------------------------------

    m_pMenuNameSpaceContext = new QMenu("Name Space",this);

    m_pActionNameSpaceTitle = new QAction("Name Space:",this);

    QPixmap pxmExpandAll(":/ZS/TreeView/TreeViewExpandAll.png");
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

    QPixmap pxmCollapseAll(":/ZS/TreeView/TreeViewCollapseAll.png");
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

    m_pActionNameSpaceEnableAdminObjs = new QAction("Recursively Enable Admin Objects",this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceEnableAdminObjs);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionNameSpaceEnableAdminObjs,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionNameSpaceEnableAdminObjsTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pActionNameSpaceDisableAdminObjs = new QAction("Recursively Disable Admin Objects",this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceDisableAdminObjs);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionNameSpaceDisableAdminObjs,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionNameSpaceDisableAdminObjsTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pMenuNameSpaceContext->addSeparator();

    m_pActionNameSpaceSetAdminObjsMethodCallsDetailLevel = new QAction("Recursively Set Method Calls Detail Level of Admin Objects",this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceSetAdminObjsMethodCallsDetailLevel);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionNameSpaceSetAdminObjsMethodCallsDetailLevel,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionNameSpaceSetAdminObjsMethodCallsDetailLevelTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pActionNameSpaceSetAdminObjsRuntimeInfoDetailLevel = new QAction("Recursively Set Runtime Info Detail Level of Admin Objects",this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceSetAdminObjsRuntimeInfoDetailLevel);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionNameSpaceSetAdminObjsRuntimeInfoDetailLevel,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionNameSpaceSetAdminObjsRuntimeInfoDetailLevelTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pActionNameSpaceSetAdminObjsTraceDataFilter = new QAction("Recursively Set Data Filter of Admin Objects",this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceSetAdminObjsTraceDataFilter);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionNameSpaceSetAdminObjsTraceDataFilter,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionNameSpaceSetAdminObjsTraceDataFilterTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CTreeViewIdxTreeTrcAdminObjs::~CTreeViewIdxTreeTrcAdminObjs()
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
    m_pDelegate = nullptr;
    m_pModel = nullptr;
    m_pMenuNameSpaceContext = nullptr;
    m_pActionNameSpaceTitle = nullptr;
    m_pActionNameSpaceExpand = nullptr;
    m_pActionNameSpaceCollapse = nullptr;
    m_pActionNameSpaceEnableAdminObjs = nullptr;
    m_pActionNameSpaceDisableAdminObjs = nullptr;
    m_pActionNameSpaceSetAdminObjsMethodCallsDetailLevel = nullptr;
    m_pActionNameSpaceSetAdminObjsRuntimeInfoDetailLevel = nullptr;
    m_pActionNameSpaceSetAdminObjsTraceDataFilter = nullptr;
    //m_modelIdxSelectedOnMousePressEvent;
    //m_modelIdxSelectedOnMouseReleaseEvent;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // overridable slots of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::expandAll()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "expandAll",
        /* strMethodInArgs    */ "" );

    QTreeView::expandAll();
    QModelIndex modelIdxRoot = model()->index(0, 0, QModelIndex());
    expandRecursive(modelIdxRoot);
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::collapseAll()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "collapseAll",
        /* strMethodInArgs    */ "" );

    QTreeView::collapseAll();
    QModelIndex modelIdxRoot = model()->index(0, 0, QModelIndex());
    collapseRecursive(modelIdxRoot);
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::expandRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "expandRecursive",
        /* strMethodInArgs    */ strMthInArgs );

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

                if( pModelTreeEntryChild->isRoot() || pModelTreeEntryChild->isBranch() )
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
void CTreeViewIdxTreeTrcAdminObjs::collapseRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "collapsRecursive",
        /* strMethodInArgs    */ strMthInArgs );

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

                if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
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
void CTreeViewIdxTreeTrcAdminObjs::expand( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::collapse( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onCollapsed( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onCollapsed",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_modelIdx.isValid() )
    {
        CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(model());

        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

        if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
        {
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            pModelIdxTree->setIsExpanded(pModelBranch, false);
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onExpanded( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

        if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
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
void CTreeViewIdxTreeTrcAdminObjs::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qKeyEvent2Str(i_pEv);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "keyPressEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;

    if( i_pEv->key() == Qt::Key_Return
     || i_pEv->key() == Qt::Key_Enter
     || i_pEv->key() == Qt::Key_Space
     || i_pEv->key() == Qt::Key_F5 )
    {
        QModelIndex modelIdxSelected = selectionModel()->currentIndex();

        if( modelIdxSelected.isValid() )
        {
            QAbstractItemModel* pModelAbstract = const_cast<QAbstractItemModel*>(modelIdxSelected.model());

            if( pModelAbstract != nullptr )
            {
                if( i_pEv->key() == Qt::Key_F5 )
                {
                    //pModelAbstract->beginReset();
                    //pModelAbstract->endReset();
                }
                else // if( i_pEv->key() != Qt::Key_F5 )
                {
                    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());
                    if( pModelTreeEntry->isLeave() )
                    {
                        if( modelIdxSelected.column() == CModelIdxTreeTrcAdminObjs::EColumnEnabled )
                        {
                            QVariant val = pModelAbstract->data(modelIdxSelected, Qt::DisplayRole);
                            if( val.canConvert(QVariant::Bool) )
                            {
                                pModelAbstract->setData(modelIdxSelected, !val.toBool(), Qt::EditRole);
                            }
                        }
                    }
                }
            } // if( pModelAbstract != nullptr )
        } // if( modelIdxSelected.isValid() )
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

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qMouseEvent2Str(i_pEv);
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

        if( pModelTreeEntry != nullptr )
        {
            if( i_pEv->buttons() & Qt::RightButton )
            {
                if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
                {
                    if( m_modelIdxSelectedOnMousePressEvent.column() == CModelIdxTreeTrcAdminObjs::EColumnTreeEntryName )
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

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qMouseEvent2Str(i_pEv);
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

        if( pModelTreeEntry != nullptr && pModelTreeEntry->isLeave() )
        {
            QAbstractItemModel* pModelAbstract = const_cast<QAbstractItemModel*>(m_modelIdxSelectedOnMouseReleaseEvent.model());
            CDelegateIdxTreeTrcAdminObjs* pDelegate = dynamic_cast<CDelegateIdxTreeTrcAdminObjs*>(itemDelegate());
            QRect rectVisual = visualRect(m_modelIdxSelectedOnMouseReleaseEvent);

            switch( m_modelIdxSelectedOnMouseReleaseEvent.column() )
            {
                case CModelIdxTreeTrcAdminObjs::EColumnEnabled:
                {
                    if( pDelegate != nullptr && pDelegate->isCheckBoxEnabledHit(rectVisual,i_pEv->pos(),m_modelIdxSelectedOnMouseReleaseEvent) )
                    {
                        QVariant val = pModelAbstract->data(m_modelIdxSelectedOnMouseReleaseEvent, Qt::DisplayRole);
                        if( val.canConvert(QVariant::Bool) )
                        {
                            pModelAbstract->setData(m_modelIdxSelectedOnMouseReleaseEvent, !val.toBool(), Qt::EditRole);
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
        } // if( pModelTreeEntry != nullptr && pModelTreeEntry->isLeave() )
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

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceCollapseTriggered( bool i_bChecked )
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
        /* strMethod          */ "onActionNameSpaceCollapseTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        collapseRecursive(m_modelIdxSelectedOnMousePressEvent);
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceEnableAdminObjsTriggered( bool i_bChecked )
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
        /* strMethod          */ "onActionNameSpaceEnableAdminObjsTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeTrcAdminObjs* pModelIdxTree = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(model());
        if( pModelIdxTree != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pModelIdxTree->idxTree();
            CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());
            if( pIdxTree != nullptr && pModelTreeEntry != nullptr )
            {
                CIdxTreeLocker idxTreeLocker(pIdxTree);
                CIdxTreeEntry* pBranch = pModelTreeEntry->getIdxTreeEntry();
                if( pBranch != nullptr )
                {
                    pIdxTree->setEnabled(pBranch, EEnabled::Yes);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceDisableAdminObjsTriggered( bool i_bChecked )
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
        /* strMethod          */ "onActionNameSpaceDisableAdminObjsTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeTrcAdminObjs* pModelIdxTree = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(model());
        if( pModelIdxTree != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pModelIdxTree->idxTree();
            CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());
            if( pIdxTree != nullptr && pModelTreeEntry != nullptr )
            {
                CIdxTreeLocker idxTreeLocker(pIdxTree);
                CIdxTreeEntry* pBranch = pModelTreeEntry->getIdxTreeEntry();
                if( pBranch != nullptr )
                {
                    pIdxTree->setEnabled(pBranch, EEnabled::No);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsMethodCallsDetailLevelTriggered( bool i_bChecked )
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
        /* strMethod          */ "onActionNameSpaceSetAdminObjsMethodCallsDetailLevelTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeTrcAdminObjs* pModelIdxTree = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(model());
        if( pModelIdxTree != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pModelIdxTree->idxTree();
            CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());
            if( pIdxTree != nullptr && pModelTreeEntry != nullptr )
            {
                CIdxTreeLocker idxTreeLocker(pIdxTree);
                CIdxTreeEntry* pBranch = pModelTreeEntry->getIdxTreeEntry();
                if( pBranch != nullptr )
                {
                    CDlgEditEnumValue* pDlg = CDlgEditEnumValue::CreateInstance(
                        /* strTitle    */ QCoreApplication::applicationName(),
                        /* strObjName  */ "MethodCallsDetailLevel",
                        /* pWdgtParent */ this );
                    pDlg->setValueName("MethodCalls");
                    pDlg->setComboItems(CEnumMethodTraceDetailLevel::s_arEnumEntries);
                    pDlg->setEnumerator(0);

                    if( pDlg->exec() == QDialog::Accepted )
                    {
                        try
                        {
                            int iDetailLevel = pDlg->getEnumerator();
                            CEnumMethodTraceDetailLevel eDetailLevel(iDetailLevel);
                            pIdxTree->setMethodCallsTraceDetailLevel(pBranch, eDetailLevel.enumerator());
                        }
                        catch(CException&)
                        {
                        }
                    }
                    CDlgEditEnumValue::DestroyInstance(pDlg);
                    pDlg = nullptr;
                }
            }
        }
    }
} // onActionNameSpaceSetAdminObjsMethodCallsDetailLevelTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsRuntimeInfoDetailLevelTriggered( bool i_bChecked )
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
        /* strMethod          */ "onActionNameSpaceSetAdminObjsRuntimeInfoDetailLevelTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeTrcAdminObjs* pModelIdxTree = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(model());
        if( pModelIdxTree != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pModelIdxTree->idxTree();
            CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());
            if( pIdxTree != nullptr && pModelTreeEntry != nullptr )
            {
                CIdxTreeLocker idxTreeLocker(pIdxTree);
                CIdxTreeEntry* pBranch = pModelTreeEntry->getIdxTreeEntry();
                if( pBranch != nullptr )
                {
                    CDlgEditEnumValue* pDlg = CDlgEditEnumValue::CreateInstance(
                        /* strTitle    */ QCoreApplication::applicationName(),
                        /* strObjName  */ "RuntimeInfoDetailLevel",
                        /* pWdgtParent */ this );
                    pDlg->setValueName("RuntimeInfo");
                    pDlg->setComboItems(CEnumLogDetailLevel::s_arEnumEntries);
                    pDlg->setEnumerator(0);
                    if( pDlg->exec() == QDialog::Accepted )
                    {
                        try
                        {
                            int iDetailLevel = pDlg->getEnumerator();
                            CEnumLogDetailLevel eDetailLevel(iDetailLevel);
                            pIdxTree->setRuntimeInfoTraceDetailLevel(pBranch, eDetailLevel.enumerator());
                        }
                        catch(CException&)
                        {
                        }
                    }
                    CDlgEditEnumValue::DestroyInstance(pDlg);
                    pDlg = nullptr;
                }
            }
        }
    }
} // onActionNameSpaceSetAdminObjsRuntimeInfoDetailLevelTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsTraceDataFilterTriggered( bool i_bChecked )
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
        /* strMethod          */ "onActionNameSpaceSetAdminObjsTraceDataFilterTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeTrcAdminObjs* pModelIdxTree = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(model());
        if( pModelIdxTree != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pModelIdxTree->idxTree();
            CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());
            if( pIdxTree != nullptr && pModelTreeEntry != nullptr )
            {
                CIdxTreeLocker idxTreeLocker(pIdxTree);
                CIdxTreeEntry* pBranch = pModelTreeEntry->getIdxTreeEntry();
                if( pBranch != nullptr )
                {
                    CDlgEditStringValue* pDlg = CDlgEditStringValue::CreateInstance(
                        /* strTitle    */ QCoreApplication::applicationName(),
                        /* strObjName  */ "TraceDataFilter",
                        /* pWdgtParent */ this );
                    pDlg->setValueName("DataFilter");
                    pDlg->setValue("");
                    if( pDlg->exec() == QDialog::Accepted )
                    {
                        try
                        {
                            QString strDataFilter = pDlg->getValue();
                            pIdxTree->setTraceDataFilter(pBranch, strDataFilter);
                        }
                        catch(CException&)
                        {
                        }
                    }
                    CDlgEditEnumValue::DestroyInstance(pDlg);
                    pDlg = nullptr;
                }
            }
        }
    }
} // onActionNameSpaceSetAdminObjsTraceDataFilterTriggered
