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

#include "ZSSysGUI/ZSSysLoggerIdxTreeView.h"
#include "ZSSysGUI/ZSSysLoggerIdxTreeModel.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSysGUI/ZSSysEditEnumValueDlg.h"
#include "ZSSysGUI/ZSSysEditStringValueDlg.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysLogger.h"
#include "ZSSys/ZSSysLoggerIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CDelegateIdxTreeLoggers : public QStyledItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDelegateIdxTreeLoggers::CDelegateIdxTreeLoggers(QObject* i_pObjParent) :
//------------------------------------------------------------------------------
    QStyledItemDelegate(i_pObjParent),
    m_rectChkBoxEnabled(0, 0, 10, 10),
    m_rectChkBoxAddThreadName(0, 0, 10, 10),
    m_rectChkBoxAddDateTime(0, 0, 10, 10),
    m_rectChkBoxAddSystemTime(0, 0, 10, 10)
{
}

//------------------------------------------------------------------------------
CDelegateIdxTreeLoggers::~CDelegateIdxTreeLoggers()
//------------------------------------------------------------------------------
{
    m_rectChkBoxEnabled = QRect(0, 0, 0, 0);
    m_rectChkBoxAddThreadName = QRect(0, 0, 0, 0);
    m_rectChkBoxAddDateTime = QRect(0, 0, 0, 0);
    m_rectChkBoxAddSystemTime = QRect(0, 0, 0, 0);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDelegateIdxTreeLoggers::isCheckBoxEnabledHit(
    const QRect&       i_rectVisual,
    const QPoint&      i_ptPos,
    const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    // If the check box is bigger than the visual rect ..
    if( i_rectVisual.width() <= m_rectChkBoxEnabled.width()
     && i_rectVisual.height() <= m_rectChkBoxEnabled.height() )
    {
        bIsHit = true;
    }
    // If the check box is smaller than the visual rect ..
    else
    {
        QRect rectCheckBox = m_rectChkBoxEnabled;

        rectCheckBox.moveLeft(i_rectVisual.left());
        rectCheckBox.moveTop(i_rectVisual.top());

        if( i_rectVisual.width() > m_rectChkBoxEnabled.width() )
        {
            rectCheckBox.moveLeft(i_rectVisual.left()+(i_rectVisual.width()-m_rectChkBoxEnabled.width())/2);
        }
        if( i_rectVisual.height() > m_rectChkBoxEnabled.height() )
        {
            rectCheckBox.moveTop(i_rectVisual.top()+(i_rectVisual.height()-m_rectChkBoxEnabled.height())/2);
        }
        if( rectCheckBox.contains(i_ptPos) )
        {
            bIsHit = true;
        }
    }
    return bIsHit;
}

//------------------------------------------------------------------------------
bool CDelegateIdxTreeLoggers::isCheckBoxAddThreadNameHit(
    const QRect&       i_rectVisual,
    const QPoint&      i_ptPos,
    const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    // If the check box is bigger than the visual rect ..
    if( i_rectVisual.width() <= m_rectChkBoxAddThreadName.width()
     && i_rectVisual.height() <= m_rectChkBoxAddThreadName.height() )
    {
        bIsHit = true;
    }
    // If the check box is smaller than the visual rect ..
    else
    {
        QRect rectCheckBox = m_rectChkBoxAddThreadName;

        rectCheckBox.moveLeft(i_rectVisual.left());
        rectCheckBox.moveTop(i_rectVisual.top());

        if( i_rectVisual.width() > m_rectChkBoxAddThreadName.width() )
        {
            rectCheckBox.moveLeft(i_rectVisual.left()+(i_rectVisual.width()-m_rectChkBoxAddThreadName.width())/2);
        }
        if( i_rectVisual.height() > m_rectChkBoxAddThreadName.height() )
        {
            rectCheckBox.moveTop(i_rectVisual.top()+(i_rectVisual.height()-m_rectChkBoxAddThreadName.height())/2);
        }
        if( rectCheckBox.contains(i_ptPos) )
        {
            bIsHit = true;
        }
    }
    return bIsHit;
}

//------------------------------------------------------------------------------
bool CDelegateIdxTreeLoggers::isCheckBoxAddDateTimeHit(
    const QRect&       i_rectVisual,
    const QPoint&      i_ptPos,
    const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    // If the check box is bigger than the visual rect ..
    if( i_rectVisual.width() <= m_rectChkBoxAddDateTime.width()
     && i_rectVisual.height() <= m_rectChkBoxAddDateTime.height() )
    {
        bIsHit = true;
    }
    // If the check box is smaller than the visual rect ..
    else
    {
        QRect rectCheckBox = m_rectChkBoxAddDateTime;

        rectCheckBox.moveLeft(i_rectVisual.left());
        rectCheckBox.moveTop(i_rectVisual.top());

        if( i_rectVisual.width() > m_rectChkBoxAddDateTime.width() )
        {
            rectCheckBox.moveLeft(i_rectVisual.left()+(i_rectVisual.width()-m_rectChkBoxAddDateTime.width())/2);
        }
        if( i_rectVisual.height() > m_rectChkBoxAddDateTime.height() )
        {
            rectCheckBox.moveTop(i_rectVisual.top()+(i_rectVisual.height()-m_rectChkBoxAddDateTime.height())/2);
        }
        if( rectCheckBox.contains(i_ptPos) )
        {
            bIsHit = true;
        }
    }
    return bIsHit;
}

//------------------------------------------------------------------------------
bool CDelegateIdxTreeLoggers::isCheckBoxAddSystemTimeHit(
    const QRect&       i_rectVisual,
    const QPoint&      i_ptPos,
    const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    // If the check box is bigger than the visual rect ..
    if( i_rectVisual.width() <= m_rectChkBoxAddSystemTime.width()
     && i_rectVisual.height() <= m_rectChkBoxAddSystemTime.height() )
    {
        bIsHit = true;
    }
    // If the check box is smaller than the visual rect ..
    else
    {
        QRect rectCheckBox = m_rectChkBoxAddSystemTime;

        rectCheckBox.moveLeft(i_rectVisual.left());
        rectCheckBox.moveTop(i_rectVisual.top());

        if( i_rectVisual.width() > m_rectChkBoxAddSystemTime.width() )
        {
            rectCheckBox.moveLeft(i_rectVisual.left()+(i_rectVisual.width()-m_rectChkBoxAddSystemTime.width())/2);
        }
        if( i_rectVisual.height() > m_rectChkBoxAddSystemTime.height() )
        {
            rectCheckBox.moveTop(i_rectVisual.top()+(i_rectVisual.height()-m_rectChkBoxAddSystemTime.height())/2);
        }
        if( rectCheckBox.contains(i_ptPos) )
        {
            bIsHit = true;
        }
    }
    return bIsHit;
}

/*==============================================================================
public: // overridables of base class QItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
void CDelegateIdxTreeLoggers::paint(
    QPainter*                   i_pPainter,
    const QStyleOptionViewItem& i_styleOption,
    const QModelIndex&          i_modelIdx ) const
//------------------------------------------------------------------------------
{
    bool bHandled = false;

    if( i_modelIdx.isValid() )
    {
        const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
        CModelIdxTreeEntry*       pVModelTreeEntry = const_cast<CModelIdxTreeEntry*>(pCModelTreeEntry);

        CLogger*        pLogger = nullptr;
        QStyleOptionViewItem styleOption  = i_styleOption;

        if( pVModelTreeEntry != nullptr )
        {
            pLogger = dynamic_cast<CLogger*>(pVModelTreeEntry->treeEntry());
        }

        switch( i_modelIdx.column() )
        {
            case CModelIdxTreeLoggers::EColumnEnabled:
            case CModelIdxTreeLoggers::EColumnAddThreadName:
            case CModelIdxTreeLoggers::EColumnAddDateTime:
            case CModelIdxTreeLoggers::EColumnAddSystemTime:
            {
                if( pLogger != nullptr )
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
QWidget* CDelegateIdxTreeLoggers::createEditor(
    QWidget* i_pWdgtParent,
    const QStyleOptionViewItem& i_styleOption,
    const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QWidget* pWdgtEditor = nullptr;

    if( i_modelIdx.isValid() )
    {
        const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
        CModelIdxTreeEntry*       pVModelTreeEntry = const_cast<CModelIdxTreeEntry*>(pCModelTreeEntry);

        CLogger* pLogger = nullptr;

        if( pVModelTreeEntry != nullptr )
        {
            pLogger = dynamic_cast<CLogger*>(pVModelTreeEntry->treeEntry());
        }

        switch( i_modelIdx.column() )
        {
            case CModelIdxTreeLoggers::EColumnEnabled:
            case CModelIdxTreeLoggers::EColumnAddThreadName:
            case CModelIdxTreeLoggers::EColumnAddDateTime:
            case CModelIdxTreeLoggers::EColumnAddSystemTime:
            {
                if( pLogger != nullptr )
                {
                    // Dont't create an editor for the enabled check box. Otherwise the user
                    // would need to click twice on the cell in order to toggle the value.
                    // Changing the enabled value is realized by the TreeViewWidget's method
                    // "mouseReleaseEvent" if the enabled check box has been hit.
                    // pWdgtEditor = new QCheckBox(i_pWdgtParent);
                }
                break;
            }
            case CModelIdxTreeLoggers::EColumnLogLevel:
            {
                if( pLogger != nullptr )
                {
                    QComboBox* pCmb = new QComboBox(i_pWdgtParent);
                    pCmb->setObjectName(pLogger->keyInTree() + ".LogLevel");
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
            case CModelIdxTreeLoggers::EColumnDataFilter:
            {
                if( pLogger != nullptr )
                {
                    QLineEdit* pEdt = new QLineEdit(i_pWdgtParent);
                    pEdt->setObjectName(pLogger->keyInTree() + ".DataFilter");
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

    return pWdgtEditor;
}

//------------------------------------------------------------------------------
void CDelegateIdxTreeLoggers::setEditorData(
    QWidget* i_pWdgtEditor, const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    bool bHandled = false;

    if( i_modelIdx.isValid() )
    {
        const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
        CModelIdxTreeEntry*       pVModelTreeEntry = const_cast<CModelIdxTreeEntry*>(pCModelTreeEntry);

        CLogger* pLogger = nullptr;

        if( pVModelTreeEntry != nullptr )
        {
            pLogger = dynamic_cast<CLogger*>(pVModelTreeEntry->treeEntry());
        }

        switch( i_modelIdx.column() )
        {
            case CModelIdxTreeLoggers::EColumnEnabled:
            case CModelIdxTreeLoggers::EColumnAddThreadName:
            case CModelIdxTreeLoggers::EColumnAddDateTime:
            case CModelIdxTreeLoggers::EColumnAddSystemTime:
            {
                if( pLogger != nullptr )
                {
                    // No editor is used for the enabled check box. Otherwise the user
                    // would need to click twice on the cell in order to toggle the value.
                    // Changing the enabled value is realized by the TreeViewWidget's method
                    // "mouseReleaseEvent" if the enabled check box has been hit.
                }
                break;
            }
            case CModelIdxTreeLoggers::EColumnLogLevel:
            {
                if( pLogger != nullptr )
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
            case CModelIdxTreeLoggers::EColumnDataFilter:
            {
                if( pLogger != nullptr )
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
void CDelegateIdxTreeLoggers::setModelData(
    QWidget* i_pWdgtEditor,
    QAbstractItemModel* i_pModel,
    const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    bool bHandled = false;

    if( i_modelIdx.isValid() )
    {
        const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
        CModelIdxTreeEntry*       pVModelTreeEntry = const_cast<CModelIdxTreeEntry*>(pCModelTreeEntry);

        CLogger* pLogger = nullptr;

        if( pVModelTreeEntry != nullptr )
        {
            pLogger = dynamic_cast<CLogger*>(pVModelTreeEntry->treeEntry());
        }

        switch( i_modelIdx.column() )
        {
            case CModelIdxTreeLoggers::EColumnEnabled:
            case CModelIdxTreeLoggers::EColumnAddThreadName:
            case CModelIdxTreeLoggers::EColumnAddDateTime:
            case CModelIdxTreeLoggers::EColumnAddSystemTime:
            {
                if( pLogger != nullptr )
                {
                    // No editor is used for the enabled check box. Otherwise the user
                    // would need to click twice on the cell in order to toggle the value.
                    // Changing the enabled value is realized by the TreeViewWidget's method
                    // "mouseReleaseEvent" if the enabled check box has been hit.
                }
                break;
            }
            case CModelIdxTreeLoggers::EColumnLogLevel:
            {
                if( pLogger != nullptr )
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
            case CModelIdxTreeLoggers::EColumnDataFilter:
            {
                if( pLogger != nullptr )
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
void CDelegateIdxTreeLoggers::updateEditorGeometry(
    QWidget* i_pWdgtEditor,
    const QStyleOptionViewItem& i_styleOption,
    const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    bool bHandled = false;

    if( i_modelIdx.isValid() )
    {
        const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
        CModelIdxTreeEntry*       pVModelTreeEntry = const_cast<CModelIdxTreeEntry*>(pCModelTreeEntry);

        CLogger* pLogger = nullptr;

        if( pVModelTreeEntry != nullptr )
        {
            pLogger = dynamic_cast<CLogger*>(pVModelTreeEntry->treeEntry());
        }

        switch( i_modelIdx.column() )
        {
            case CModelIdxTreeLoggers::EColumnEnabled:
            case CModelIdxTreeLoggers::EColumnAddThreadName:
            case CModelIdxTreeLoggers::EColumnAddDateTime:
            case CModelIdxTreeLoggers::EColumnAddSystemTime:
            {
                if( pLogger != nullptr )
                {
                    // No editor is used for the enabled check box. Otherwise the user
                    // would need to click twice on the cell in order to toggle the value.
                    // Changing the enabled value is realized by the TreeViewWidget's method
                    // "mouseReleaseEvent" if the enabled check box has been hit.
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
void CDelegateIdxTreeLoggers::onComboDetailLevelActivated( int i_idx )
//------------------------------------------------------------------------------
{
    QComboBox* pCmb = dynamic_cast<QComboBox*>(sender());

    if( pCmb != nullptr )
    {
        emit commitData(pCmb);
        emit closeEditor(pCmb);
    }
}

//------------------------------------------------------------------------------
void CDelegateIdxTreeLoggers::onEdtDataFilterEditingFinished()
//------------------------------------------------------------------------------
{
}


/*******************************************************************************
class CTreeViewIdxTreeLoggers : public QTreeView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTreeViewIdxTreeLoggers::CTreeViewIdxTreeLoggers(
    CModelIdxTreeLoggers* i_pModel,
    QWidget*              i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QTreeView(i_pWdgtParent),
    m_pDelegate(nullptr),
    m_pMenuNameSpaceContext(nullptr),
    m_pActionNameSpaceTitle(nullptr),
    m_pActionNameSpaceExpand(nullptr),
    m_pActionNameSpaceCollapse(nullptr),
    m_pActionNameSpaceEnableLoggers(nullptr),
    m_pActionNameSpaceDisableLoggers(nullptr),
    m_pActionNameSpaceSetLoggersDetailLevel(nullptr),
    m_pActionNameSpaceSetLoggersDataFilter(nullptr),
    m_modelIdxSelectedOnMousePressEvent(),
    m_modelIdxSelectedOnMouseReleaseEvent()
{
    setModel(i_pModel);

    m_pDelegate = new CDelegateIdxTreeLoggers(this);

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAlternatingRowColors(true);
    setAllColumnsShowFocus(true);
    setEditTriggers(QAbstractItemView::AllEditTriggers);

    //hideColumn(CModelIdxTree::EColumnTreeEntryName);
    hideColumn(CModelIdxTree::EColumnInternalId);
    hideColumn(CModelIdxTree::EColumnTreeEntryType);
    //hideColumn(CModelIdxTree::EColumnIdxInTree);
    hideColumn(CModelIdxTree::EColumnIdxInParentBranch);
    hideColumn(CModelIdxTree::EColumnKeyInTree);
    hideColumn(CModelIdxTree::EColumnKeyInParentBranch);
    //hideColumn(CModelIdxTreeLoggers::EColumnEnabled);
    //hideColumn(CModelIdxTreeLoggers::EColumnDetailLevel);
    //hideColumn(CModelIdxTreeLoggers::EColumnDataFilter);
    //hideColumn(CModelIdxTreeLoggers::EColumnNameSpace);
    //hideColumn(CModelIdxTreeLoggers::EColumnClassName);
    //hideColumn(CModelIdxTreeLoggers::EColumnObjName);
    hideColumn(CModelIdxTreeLoggers::EColumnObjAddress);

    setItemDelegate(m_pDelegate);

    // Context Popup Menu of Name Space Branches
    //------------------------------------------

    m_pMenuNameSpaceContext = new QMenu("Name Space", this);

    m_pActionNameSpaceTitle = new QAction("Name Space:", this);

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

    m_pActionNameSpaceEnableLoggers = new QAction("Recursively Enable Loggers",this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceEnableLoggers);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionNameSpaceEnableLoggers,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionNameSpaceEnableLoggersTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pActionNameSpaceDisableLoggers = new QAction("Recursively Disable Loggers",this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceDisableLoggers);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionNameSpaceDisableLoggers,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionNameSpaceDisableLoggersTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pMenuNameSpaceContext->addSeparator();

    m_pActionNameSpaceSetLoggersDetailLevel = new QAction("Recursively Set Detail Level of Loggers",this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceSetLoggersDetailLevel);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionNameSpaceSetLoggersDetailLevel,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionNameSpaceSetLoggersDetailLevelTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pActionNameSpaceSetLoggersDataFilter = new QAction("Recursively Set Data Filter of Loggers",this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceSetLoggersDataFilter);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionNameSpaceSetLoggersDataFilter,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionNameSpaceSetLoggersDataFilterTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CTreeViewIdxTreeLoggers::~CTreeViewIdxTreeLoggers()
//------------------------------------------------------------------------------
{
    m_pDelegate = nullptr;
    m_pMenuNameSpaceContext = nullptr;
    m_pActionNameSpaceTitle = nullptr;
    m_pActionNameSpaceExpand = nullptr;
    m_pActionNameSpaceCollapse = nullptr;
    m_pActionNameSpaceEnableLoggers = nullptr;
    m_pActionNameSpaceDisableLoggers = nullptr;
    m_pActionNameSpaceSetLoggersDetailLevel = nullptr;
    m_pActionNameSpaceSetLoggersDataFilter = nullptr;
    //m_modelIdxSelectedOnMousePressEvent;
    //m_modelIdxSelectedOnMouseReleaseEvent;

} // dtor

/*==============================================================================
public: // overridable slots of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeLoggers::expandAll()
//------------------------------------------------------------------------------
{
    QTreeView::expandAll();
    QModelIndex modelIdxRoot = model()->index(0, 0, QModelIndex());
    expandRecursive(modelIdxRoot);
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeLoggers::collapseAll()
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
void CTreeViewIdxTreeLoggers::expandRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
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
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeLoggers::collapseRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
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
}

/*==============================================================================
public: // slots (hiding not overridable slots with same name in QTreeView)
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeLoggers::expand( const QModelIndex& i_modelIdx )
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
void CTreeViewIdxTreeLoggers::collapse( const QModelIndex& i_modelIdx )
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
void CTreeViewIdxTreeLoggers::onCollapsed( const QModelIndex& i_modelIdx )
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
    } // if( i_modelIdx.isValid() )
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeLoggers::onExpanded( const QModelIndex& i_modelIdx )
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
    } // if( i_modelIdx.isValid() )
}

/*==============================================================================
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeLoggers::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( i_pEv->key() == Qt::Key_Return
     || i_pEv->key() == Qt::Key_Enter
     || i_pEv->key() == Qt::Key_Space
     || i_pEv->key() == Qt::Key_F5 )
    {
        QModelIndex                modelIdxSelected = selectionModel()->currentIndex();
        CModelIdxTreeEntry*        pModelTreeEntry = nullptr;
        CLogger*              pLogger = nullptr;
        QAbstractItemModel*        pModelAbstract = nullptr;
        CModelIdxTreeLoggers* pTrcAdmObjIdxTreeModel = nullptr;

        pModelAbstract = const_cast<QAbstractItemModel*>(modelIdxSelected.model());

        if( pModelAbstract != nullptr )
        {
            pTrcAdmObjIdxTreeModel = dynamic_cast<CModelIdxTreeLoggers*>(pModelAbstract);
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
                    pLogger = dynamic_cast<CLogger*>(pModelTreeEntry->treeEntry());
                }
            }

            if( pLogger != nullptr )
            {
                if( modelIdxSelected.column() >= CModelIdxTree::EColumnCount )
                {
                    switch( modelIdxSelected.column() )
                    {
                        case CModelIdxTreeLoggers::EColumnEnabled:
                        {
                            if( pLogger->getEnabled() == EEnabled::Yes )
                            {
                                pLogger->setEnabled(EEnabled::No);
                            }
                            else
                            {
                                pLogger->setEnabled(EEnabled::Yes);
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
            } // if( pLogger != nullptr )
        } // if( i_pEv->key() != Qt::Key_F5 )
    } // if( i_pEv->key() == Qt::Key_Return ...

    if( !bEventHandled )
    {
        QTreeView::keyPressEvent(i_pEv);
    }

} // keyPressEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTreeLoggers::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
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
                            case CModelIdxTreeLoggers::EColumnTreeEntryName:
                            {
                                if( m_pMenuNameSpaceContext != nullptr )
                                {
                                    m_pActionNameSpaceTitle->setText( "Name Space: " + pModelBranch->name() );
                                    m_pMenuNameSpaceContext->popup(QWidget::mapToGlobal(i_pEv->pos()));
                                }
                                bEventHandled = true;
                                break;
                            }
                            case CModelIdxTreeLoggers::EColumnNameSpace:
                            case CModelIdxTreeLoggers::EColumnClassName:
                            case CModelIdxTreeLoggers::EColumnObjName:
                            case CModelIdxTreeLoggers::EColumnEnabled:
                            default:
                            {
                                break;
                            }
                        }
                    } // if( pLogger != nullptr )
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
void CTreeViewIdxTreeLoggers::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    //QModelIndex modelIdxSelected = selectionModel()->currentIndex();

    m_modelIdxSelectedOnMouseReleaseEvent = indexAt(i_pEv->pos());

    if( m_modelIdxSelectedOnMouseReleaseEvent.isValid() )
    {
        CDelegateIdxTreeLoggers* pDelegate = dynamic_cast<CDelegateIdxTreeLoggers*>(itemDelegate());
        CModelIdxTreeEntry*           pModelTreeEntry = nullptr;
        CLogger*                 pLogger = nullptr;

        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());

        if( pModelTreeEntry != nullptr )
        {
            pLogger = dynamic_cast<CLogger*>(pModelTreeEntry->treeEntry());
        }

        if( pLogger != nullptr )
        {
            if( m_modelIdxSelectedOnMouseReleaseEvent.column() >= CModelIdxTree::EColumnCount )
            {
                switch( m_modelIdxSelectedOnMouseReleaseEvent.column() )
                {
                    case CModelIdxTreeLoggers::EColumnEnabled:
                    {
                        QRect rectVisual = visualRect(m_modelIdxSelectedOnMouseReleaseEvent);

                        if( pDelegate != nullptr && pDelegate->isCheckBoxEnabledHit(rectVisual,i_pEv->pos(),m_modelIdxSelectedOnMouseReleaseEvent) )
                        {
                            if( pLogger->getEnabled() == EEnabled::Yes )
                            {
                                pLogger->setEnabled(EEnabled::No);
                            }
                            else
                            {
                                pLogger->setEnabled(EEnabled::Yes);
                            }
                            bEventHandled = true;
                        }
                        break;
                    }
                    case CModelIdxTreeLoggers::EColumnAddThreadName:
                    {
                        QRect rectVisual = visualRect(m_modelIdxSelectedOnMouseReleaseEvent);

                        if( pDelegate != nullptr && pDelegate->isCheckBoxAddThreadNameHit(rectVisual,i_pEv->pos(),m_modelIdxSelectedOnMouseReleaseEvent) )
                        {
                            if( pLogger->addThreadName() )
                            {
                                pLogger->setAddThreadName(false);
                            }
                            else
                            {
                                pLogger->setAddThreadName(true);
                            }
                            bEventHandled = true;
                        }
                        break;
                    }
                    case CModelIdxTreeLoggers::EColumnAddDateTime:
                    {
                        QRect rectVisual = visualRect(m_modelIdxSelectedOnMouseReleaseEvent);

                        if( pDelegate != nullptr && pDelegate->isCheckBoxAddDateTimeHit(rectVisual,i_pEv->pos(),m_modelIdxSelectedOnMouseReleaseEvent) )
                        {
                            if( pLogger->addDateTime() )
                            {
                                pLogger->setAddDateTime(false);
                            }
                            else
                            {
                                pLogger->setAddDateTime(true);
                            }
                            bEventHandled = true;
                        }
                        break;
                    }
                    case CModelIdxTreeLoggers::EColumnAddSystemTime:
                    {
                        QRect rectVisual = visualRect(m_modelIdxSelectedOnMouseReleaseEvent);

                        if( pDelegate != nullptr && pDelegate->isCheckBoxAddSystemTimeHit(rectVisual,i_pEv->pos(),m_modelIdxSelectedOnMouseReleaseEvent) )
                        {
                            if( pLogger->addSystemTime() )
                            {
                                pLogger->setAddSystemTime(false);
                            }
                            else
                            {
                                pLogger->setAddSystemTime(true);
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
        } // if( pLogger != nullptr )
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
void CTreeViewIdxTreeLoggers::onActionNameSpaceExpandTriggered( bool i_bChecked )
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
void CTreeViewIdxTreeLoggers::onActionNameSpaceCollapseTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        collapseRecursive(m_modelIdxSelectedOnMousePressEvent);
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeLoggers::onActionNameSpaceEnableLoggersTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeLoggers* pModelIdxTree = dynamic_cast<CModelIdxTreeLoggers*>(model());

        if( pModelIdxTree != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pModelIdxTree->idxTree();

            CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());

            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;

            if( pIdxTree != nullptr && pModelBranch != nullptr )
            {
                CIdxTreeEntry* pBranch = pModelBranch->treeEntry();

                pIdxTree->setEnabled(pBranch, EEnabled::Yes);
            }
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeLoggers::onActionNameSpaceDisableLoggersTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeLoggers* pModelIdxTree = dynamic_cast<CModelIdxTreeLoggers*>(model());

        if( pModelIdxTree != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pModelIdxTree->idxTree();

            CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());

            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;

            if( pIdxTree != nullptr && pModelBranch != nullptr )
            {
                CIdxTreeEntry* pBranch = pModelBranch->treeEntry();

                pIdxTree->setEnabled(pBranch, EEnabled::No);
            }
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeLoggers::onActionNameSpaceSetLoggersDetailLevelTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeLoggers* pModelIdxTree = dynamic_cast<CModelIdxTreeLoggers*>(model());

        if( pModelIdxTree != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pModelIdxTree->idxTree();

            CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());

            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;

            if( pIdxTree != nullptr && pModelBranch != nullptr )
            {
                CIdxTreeEntry* pBranch = pModelBranch->treeEntry();

                CDlgEditEnumValue* pDlg = CDlgEditEnumValue::CreateInstance(
                    /* strTitle    */ QCoreApplication::applicationName(),
                    /* strObjName  */ "LogLevel",
                    /* pWdgtParent */ this );
                pDlg->setValueName("LogLevel");
                pDlg->setComboItems(CEnumLogDetailLevel::s_arEnumEntries);
                pDlg->setEnumerator(0);

                if( pDlg->exec() == QDialog::Accepted )
                {
                    try
                    {
                        int iDetailLevel = pDlg->getEnumerator();
                        CEnumLogDetailLevel eDetailLevel(iDetailLevel);
                        pIdxTree->setDetailLevel(pBranch, eDetailLevel.enumerator());
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
} // onActionNameSpaceSetLoggersDetailLevelTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeLoggers::onActionNameSpaceSetLoggersDataFilterTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeLoggers* pModelIdxTree = dynamic_cast<CModelIdxTreeLoggers*>(model());

        if( pModelIdxTree != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pModelIdxTree->idxTree();

            CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());

            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;

            if( pIdxTree != nullptr && pModelBranch != nullptr )
            {
                CIdxTreeEntry* pBranch = pModelBranch->treeEntry();

                CDlgEditStringValue* pDlg = CDlgEditStringValue::CreateInstance(
                    /* strTitle    */ QCoreApplication::applicationName(),
                    /* strObjName  */ "DataFilter",
                    /* pWdgtParent */ this );
                pDlg->setValueName("DataFilter");
                pDlg->setValue("");

                if( pDlg->exec() == QDialog::Accepted )
                {
                    try
                    {
                        QString strDataFilter = pDlg->getValue();
                        pIdxTree->setDataFilter(pBranch, strDataFilter);
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
