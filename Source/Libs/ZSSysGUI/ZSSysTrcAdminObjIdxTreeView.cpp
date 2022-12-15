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
CDelegateIdxTreeTrcAdminObjs::CDelegateIdxTreeTrcAdminObjs(
    QObject* i_pObjParent,
    EMethodTraceDetailLevel i_eTrcDetailLevel,
    EMethodTraceDetailLevel i_eTrcDetailLevelNoisyMethods ) :
//------------------------------------------------------------------------------
    QStyledItemDelegate(i_pObjParent),
    m_rectChkBoxTraceEnabled(0, 0, 10, 10),
    m_eTrcDetailLevel(i_eTrcDetailLevel),
    m_eTrcDetailLevelNoisyMethods(i_eTrcDetailLevelNoisyMethods)
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Parent: " + QString(i_pObjParent == nullptr ? "nullptr" : i_pObjParent->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ strMthInArgs );

    m_rectChkBoxTraceEnabled = QRect(0, 0, 0, 0);
    m_eTrcDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_eTrcDetailLevelNoisyMethods = static_cast<EMethodTraceDetailLevel>(0);

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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "RectVisual: " + qRect2Str(i_rectVisual);
        strMthInArgs += ", Pos: " + qPoint2Str(i_ptPos);
        strMthInArgs += ", ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose )
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

    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevelNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "paint",
        /* strMethodInArgs    */ strMthInArgs );

    bool bHandled = false;

    if( i_modelIdx.isValid() )
    {
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
            case CModelIdxTreeTrcAdminObjs::EColumnEnabled:
            {
                if( pTrcAdminObj != nullptr )
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
                if( pTrcAdminObj != nullptr )
                {
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnRuntimeInfoDetailLevel:
            {
                if( pTrcAdminObj != nullptr )
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "createEditor",
        /* strMethodInArgs    */ strMthInArgs );

    QWidget* pWdgtEditor = nullptr;

    if( i_modelIdx.isValid() )
    {
        const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
        CModelIdxTreeEntry*       pVModelTreeEntry = const_cast<CModelIdxTreeEntry*>(pCModelTreeEntry);

        CTrcAdminObj* pTrcAdminObj = nullptr;

        if( pVModelTreeEntry != nullptr )
        {
            pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pVModelTreeEntry->treeEntry());
        }

        switch( i_modelIdx.column() )
        {
            case CModelIdxTreeTrcAdminObjs::EColumnEnabled:
            {
                if( pTrcAdminObj != nullptr )
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
                if( pTrcAdminObj != nullptr )
                {
                    QComboBox* pCmb = new QComboBox(i_pWdgtParent);
                    pCmb->setObjectName(pTrcAdminObj->keyInTree() + ".MethodCalls");
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
                if( pTrcAdminObj != nullptr )
                {
                    QComboBox* pCmb = new QComboBox(i_pWdgtParent);
                    pCmb->setObjectName(pTrcAdminObj->keyInTree() + ".RuntimeInfo");
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
                if( pTrcAdminObj != nullptr )
                {
                    QLineEdit* pEdt = new QLineEdit(i_pWdgtParent);
                    pEdt->setObjectName(pTrcAdminObj->keyInTree() + ".DataFilter");
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "createEditor",
        /* strMethodInArgs    */ strMthInArgs );

    bool bHandled = false;

    if( i_modelIdx.isValid() )
    {
        const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
        CModelIdxTreeEntry*       pVModelTreeEntry = const_cast<CModelIdxTreeEntry*>(pCModelTreeEntry);

        CTrcAdminObj* pTrcAdminObj = nullptr;

        if( pVModelTreeEntry != nullptr )
        {
            pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pVModelTreeEntry->treeEntry());
        }

        switch( i_modelIdx.column() )
        {
            case CModelIdxTreeTrcAdminObjs::EColumnEnabled:
            {
                if( pTrcAdminObj != nullptr )
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
                if( pTrcAdminObj != nullptr )
                {
                    QComboBox* pWdgtEditor = dynamic_cast<QComboBox*>(i_pWdgtEditor);
                    if( pWdgtEditor != nullptr )
                    {
                        QString strVal = i_modelIdx.data(Qt::EditRole).toString();
                        int idx = pWdgtEditor->findText(strVal);
                        if (idx >= 0) {
                            pWdgtEditor->setCurrentIndex(idx);
                        }
                        pWdgtEditor->showPopup();
                        bHandled = true;
                    }
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnDataFilter:
            {
                if( pTrcAdminObj != nullptr )
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setModelData",
        /* strMethodInArgs    */ strMthInArgs );

    bool bHandled = false;

    if( i_modelIdx.isValid() )
    {
        const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
        CModelIdxTreeEntry*       pVModelTreeEntry = const_cast<CModelIdxTreeEntry*>(pCModelTreeEntry);

        CTrcAdminObj* pTrcAdminObj = nullptr;

        if( pVModelTreeEntry != nullptr )
        {
            pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pVModelTreeEntry->treeEntry());
        }

        switch( i_modelIdx.column() )
        {
            case CModelIdxTreeTrcAdminObjs::EColumnEnabled:
            {
                if( pTrcAdminObj != nullptr )
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
                if( pTrcAdminObj != nullptr )
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
                if( pTrcAdminObj != nullptr )
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "updateEditorGeometry",
        /* strMethodInArgs    */ strMthInArgs );

    bool bHandled = false;

    if( i_modelIdx.isValid() )
    {
        const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
        CModelIdxTreeEntry*       pVModelTreeEntry = const_cast<CModelIdxTreeEntry*>(pCModelTreeEntry);

        CTrcAdminObj* pTrcAdminObj = nullptr;

        if( pVModelTreeEntry != nullptr )
        {
            pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pVModelTreeEntry->treeEntry());
        }

        switch( i_modelIdx.column() )
        {
            case CModelIdxTreeTrcAdminObjs::EColumnEnabled:
            {
                if( pTrcAdminObj != nullptr )
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
                if( pTrcAdminObj != nullptr )
                {
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnRuntimeInfoDetailLevel:
            {
                if( pTrcAdminObj != nullptr )
                {
                }
                break;
            }
            case CModelIdxTreeTrcAdminObjs::EColumnDataFilter:
            {
                if( pTrcAdminObj != nullptr )
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = pCmb == nullptr ? "null" : pCmb->objectName();
        strMthInArgs += ", " + QString(pCmb == nullptr ? QString::number(i_idx) : pCmb->currentText());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = pEdt == nullptr ? "null" : pEdt->objectName();
        strMthInArgs += ", " + QString(pEdt == nullptr ? "" : pEdt->text());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
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
    CModelIdxTreeTrcAdminObjs* i_pModel,
    QWidget* i_pWdgtParent,
    EMethodTraceDetailLevel i_eTrcDetailLevel,
    EMethodTraceDetailLevel i_eTrcDetailLevelNoisyMethods ) :
//------------------------------------------------------------------------------
    QTreeView(i_pWdgtParent),
    m_pDelegate(nullptr),
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
    m_eTrcDetailLevel(i_eTrcDetailLevel),
    m_eTrcDetailLevelNoisyMethods(i_eTrcDetailLevelNoisyMethods)
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Model: " + QString(i_pModel == nullptr ? "nullptr" : i_pModel->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    setModel(i_pModel);

    m_pDelegate = new CDelegateIdxTreeTrcAdminObjs(this, m_eTrcDetailLevel, m_eTrcDetailLevelNoisyMethods);

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
    //hideColumn(CModelIdxTree::EColumnIdxInTree);
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
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
    m_pActionNameSpaceEnableAdminObjs = nullptr;
    m_pActionNameSpaceDisableAdminObjs = nullptr;
    m_pActionNameSpaceSetAdminObjsMethodCallsDetailLevel = nullptr;
    m_pActionNameSpaceSetAdminObjsRuntimeInfoDetailLevel = nullptr;
    m_pActionNameSpaceSetAdminObjsTraceDataFilter = nullptr;
    //m_modelIdxSelectedOnMousePressEvent;
    //m_modelIdxSelectedOnMouseReleaseEvent;
    m_eTrcDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_eTrcDetailLevelNoisyMethods = static_cast<EMethodTraceDetailLevel>(0);

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
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = qKeyEvent2Str(i_pEv);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = qMouseEvent2Str(i_pEv);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = qMouseEvent2Str(i_pEv);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceEnableAdminObjsTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onActionNameSpaceEnableAdminObjsTriggered",
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
} // onActionNameSpaceEnableAdminObjsTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceDisableAdminObjsTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onActionNameSpaceDisableAdminObjsTriggered",
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
} // onActionNameSpaceDisableAdminObjsTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsMethodCallsDetailLevelTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onActionNameSpaceSetAdminObjsMethodCallsDetailLevelTriggered",
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
} // onActionNameSpaceSetAdminObjsMethodCallsDetailLevelTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsRuntimeInfoDetailLevelTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onActionNameSpaceSetAdminObjsRuntimeInfoDetailLevelTriggered",
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
} // onActionNameSpaceSetAdminObjsRuntimeInfoDetailLevelTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsTraceDataFilterTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onActionNameSpaceSetAdminObjsTraceDataFilterTriggered",
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
} // onActionNameSpaceSetAdminObjsTraceDataFilterTriggered
