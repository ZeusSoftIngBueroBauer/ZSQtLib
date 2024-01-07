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

#include "ZSSysGUI/ZSSysComboBoxItemDelegate.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtWidgets/qapplication.h>
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcombobox.h>
#else
#include <QtWidgets/qcombobox.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CComboBoxItemDelegate : public QStyledItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CComboBoxItemDelegate::CComboBoxItemDelegate(QAbstractItemModel* i_pModel, QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QStyledItemDelegate(i_pWdgtParent),
    m_pModel(i_pModel),
    m_iItemDataRole(Qt::AccessibleTextRole),
    m_modelIdxEditorCreated(),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjPaint(nullptr)
{
    setObjectName(i_pModel->objectName());

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName());
    m_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Paint");

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ "" );

} // ctor

//------------------------------------------------------------------------------
CComboBoxItemDelegate::~CComboBoxItemDelegate()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    if( m_pTrcAdminObj != nullptr ) {
        mthTracer.onAdminObjAboutToBeReleased();
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }
    if( m_pTrcAdminObjPaint != nullptr ) {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjPaint);
    }

    m_pModel = nullptr;
    m_iItemDataRole = 0;
    //m_modelIdxEditorCreated;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjPaint = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CComboBoxItemDelegate::setItemDataRole(int i_iRole)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qItemDataRole2Str(i_iRole);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "setItemDataRole",
        /* strMethodInArgs    */ strMthInArgs );

    m_iItemDataRole = i_iRole;
}

//------------------------------------------------------------------------------
int CComboBoxItemDelegate::itemDataRole() const
//------------------------------------------------------------------------------
{
    return m_iItemDataRole;
}

/*==============================================================================
public: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CComboBoxItemDelegate::eventFilter(QObject* i_pObjSrc, QEvent* i_pEv)
//------------------------------------------------------------------------------
{
    if (i_pEv->type() == QEvent::FocusIn) {
        QComboBox* pCmb = dynamic_cast<QComboBox*>(i_pObjSrc);
        if (pCmb != nullptr) {
            pCmb->showPopup();
            // Important to handle it only the first time,
            // otherwise will result in focus glitches
            pCmb->removeEventFilter(this);
        }
    }
    return QStyledItemDelegate::eventFilter(i_pObjSrc, i_pEv);
}

/*==============================================================================
public: // overridables of base class QStyledItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
void CComboBoxItemDelegate::paint(
    QPainter* i_pPainter, const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjPaint, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + qModelIndex2Str(i_modelIdx) + "}" +
            ", Data: " + i_modelIdx.data(Qt::DisplayRole).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjPaint,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "paint",
        /* strMethodInArgs    */ strMthInArgs );

    //QStyleOptionViewItem option = i_option;
    //option.text = "Hello World"; //i_modelIdx.model()->data(i_modelIdx, Qt::DisplayRole).toString();
    //QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &option, i_pPainter);
    QStyledItemDelegate::paint(i_pPainter, i_option, i_modelIdx);
}

//------------------------------------------------------------------------------
QWidget* CComboBoxItemDelegate::createEditor(
    QWidget* i_pWdgtParent, const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + qModelIndex2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "createEditor",
        /* strMethodInArgs    */ strMthInArgs );

    QComboBox* pCmb = new QComboBox(i_pWdgtParent);
    pCmb->installEventFilter(const_cast<CComboBoxItemDelegate*>(this));
    m_modelIdxEditorCreated = i_modelIdx;
    QObject::connect(
        pCmb, &QComboBox::currentTextChanged,
        this, &CComboBoxItemDelegate::onCmbCurrentTextChanged);
    QList<SComboBoxItem> arItems = i_modelIdx.data(m_iItemDataRole).value<QList<SComboBoxItem>>();
    for(const SComboBoxItem& item : arItems) {
        if (!item.m_icon.isNull()) {
            pCmb->addItem(item.m_icon, item.m_strText, item.m_userData);
        }
        else {
            pCmb->addItem(item.m_strText, item.m_userData);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(QString(pCmb == nullptr ? "null" : "Cmb"));
    }
    return pCmb;
}

//------------------------------------------------------------------------------
void CComboBoxItemDelegate::setEditorData(
    QWidget* i_pWdgtEditor, const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + qModelIndex2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "setEditorData",
        /* strMethodInArgs    */ strMthInArgs );

    QComboBox* pCmb = dynamic_cast<QComboBox*>(i_pWdgtEditor);
    QString strText = i_modelIdx.model()->data(i_modelIdx, Qt::EditRole).toString();
    pCmb->setCurrentText(strText);
}

//------------------------------------------------------------------------------
void CComboBoxItemDelegate::setModelData(
    QWidget* i_pWdgtEditor, QAbstractItemModel* i_pModel, const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + qModelIndex2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "setModelData",
        /* strMethodInArgs    */ strMthInArgs );
    if (i_pModel != m_pModel) {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }
    QComboBox* pCmb = dynamic_cast<QComboBox*>(i_pWdgtEditor);
    i_pModel->setData(i_modelIdx, pCmb->currentText(), Qt::EditRole);
}

//------------------------------------------------------------------------------
void CComboBoxItemDelegate::updateEditorGeometry(
    QWidget* i_pWdgtEditor, const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Rect {" + qRect2Str(i_option.rect) + ", ModelIdx {" + qModelIndex2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "updateEditorGeometry",
        /* strMethodInArgs    */ strMthInArgs );

    i_pWdgtEditor->setGeometry(i_option.rect);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CComboBoxItemDelegate::onCmbCurrentTextChanged(const QString& i_strText)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strText;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "onCmbCurrentTextChanged",
        /* strMethodInArgs    */ strMthInArgs );

    m_pModel->setData(m_modelIdxEditorCreated, i_strText, Qt::EditRole);
}
