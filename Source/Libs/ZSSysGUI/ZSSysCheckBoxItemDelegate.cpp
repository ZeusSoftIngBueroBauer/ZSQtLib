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
    m_pTrcAdminObjNoisyMethods(nullptr)
{
    setObjectName(i_pModel->objectName());

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());
    m_pTrcAdminObjNoisyMethods = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::NoisyMethods", objectName());

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    if( m_pTrcAdminObj != nullptr ) {
        mthTracer.onAdminObjAboutToBeReleased();
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }
    if( m_pTrcAdminObjNoisyMethods != nullptr ) {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjNoisyMethods);
    }

    m_pModel = nullptr;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjNoisyMethods = nullptr;

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
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + qModelIndex2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "paint",
        /* strMethodInArgs    */ strMthInArgs );

    QStyleOptionButton cbOpt;

    // Centered as default. If this should be changed a call to i_modelIdx.data
    // with Qt::TextAlignmentRole should be added and the model must provide the
    // desired alignment flags.
    cbOpt.rect = QRect(
        /* x      */ i_option.rect.center().x() - cbOpt.rect.height() / 2 - 2,
        /* y      */ i_option.rect.center().y() - cbOpt.rect.height() / 2 + 2,
        /* width  */ cbOpt.rect.height() / 2 - 4,
        /* height */ cbOpt.rect.height() / 2 - 4);

    bool isChecked = i_modelIdx.data(Qt::DisplayRole).toBool();
    if (isChecked) {
        cbOpt.state |= QStyle::State_On;
    }
    else {
        cbOpt.state |= QStyle::State_Off;
    }
    QApplication::style()->drawControl(QStyle::CE_CheckBox, &cbOpt, i_pPainter);
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
    else {
        bDataSet = QStyledItemDelegate::editorEvent(i_pEv, i_pModel, i_option, i_modelIdx);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bDataSet);
    }
    return bDataSet;
}
