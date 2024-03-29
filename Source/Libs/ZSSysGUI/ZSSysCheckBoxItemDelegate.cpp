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

#include "ZSSysGUI/ZSSysCheckBoxItemDelegate.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtWidgets/qapplication.h>
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CheckBoxItemDelegate : public QStyledItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CCheckBoxItemDelegate::CCheckBoxItemDelegate(QAbstractItemModel* i_pModel, QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QStyledItemDelegate(i_pWdgtParent),
    m_pModel(i_pModel),
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
CCheckBoxItemDelegate::~CCheckBoxItemDelegate()
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
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjPaint = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class QStyledItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
void CCheckBoxItemDelegate::paint(
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

    QVariant varData = i_modelIdx.data(Qt::DisplayRole);
    if (varData.isValid()) {
        QStyleOptionButton cbOpt;
        cbOpt.state = i_option.state;
        bool isChecked = i_modelIdx.data(Qt::DisplayRole).toBool();
        if (isChecked) {
            cbOpt.state |= QStyle::State_On;
        }
        else {
            cbOpt.state |= QStyle::State_Off;
        }

        if (cbOpt.state & QStyle::State_Selected) {
            cbOpt.rect = QRect(
                /* x      */ i_option.rect.center().x() - i_option.rect.height() / 2 + 2,
                /* y      */ i_option.rect.center().y() - i_option.rect.height() / 2 + 3,
                /* width  */ i_option.rect.height() - 5,
                /* height */ i_option.rect.height() - 5);
            i_pPainter->fillRect(cbOpt.rect, cbOpt.palette.color(QPalette::Highlight));
        }

        // Centered as default. If this should be changed a call to i_modelIdx.data
        // with Qt::TextAlignmentRole should be added and the model must provide the
        // desired alignment flags.
        cbOpt.rect = QRect(
            /* x      */ i_option.rect.center().x() - i_option.rect.height() / 2 + 4,
            /* y      */ i_option.rect.center().y() - i_option.rect.height() / 2 + 3,
            /* width  */ i_option.rect.height() - 5,
            /* height */ i_option.rect.height() - 5);
        QApplication::style()->drawControl(QStyle::CE_CheckBox, &cbOpt, i_pPainter);
    }
    else {
        QStyledItemDelegate::paint(i_pPainter, i_option, i_modelIdx);
    }
}

/*==============================================================================
protected: // overridables of base class QStyledItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
bool CCheckBoxItemDelegate::editorEvent(
    QEvent* i_pEv, QAbstractItemModel* i_pModel,
    const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Event {" + qEvent2Str(i_pEv) + "}, ModelIdx {" + qModelIndex2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "editorEvent",
        /* strMethodInArgs    */ strMthInArgs );

    if (i_pModel != m_pModel) {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }
    bool bDataSet = false;
    if (i_pEv->type() == QEvent::MouseButtonRelease) {
        bool bVal = i_modelIdx.data(Qt::DisplayRole).toBool();
        // invert checkbox state
        bDataSet = i_pModel->setData(i_modelIdx, !bVal, Qt::EditRole);
    }
    else if (i_pEv->type() == QEvent::KeyPress) {
        QKeyEvent* pKeyEvent = dynamic_cast<QKeyEvent*>(i_pEv);
        if (pKeyEvent != nullptr) {
            if ((pKeyEvent->key() == Qt::Key_Return) || (pKeyEvent->key() == Qt::Key_Enter)) {
                bool bVal = i_modelIdx.data(Qt::DisplayRole).toBool();
                // invert checkbox state
                bDataSet = i_pModel->setData(i_modelIdx, !bVal, Qt::EditRole);
            }
        }
    }
    else {
        bDataSet = QStyledItemDelegate::editorEvent(i_pEv, i_pModel, i_option, i_modelIdx);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bDataSet);
    }
    return bDataSet;
}
