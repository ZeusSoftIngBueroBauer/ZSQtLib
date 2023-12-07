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

#include "ZSPhysValGUI/ZSPhysValEditItemDelegate.h"
#include "ZSPhysValGUI/ZSPhysValEditWdgt.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
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
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CEditPhysValtemDelegate : public QStyledItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CEditPhysValtemDelegate::CEditPhysValtemDelegate(QAbstractItemModel* i_pModel, QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QStyledItemDelegate(i_pWdgtParent),
    m_pModel(i_pModel),
    m_iItemDataRoleMinVal(Qt::UserRole),
    m_iItemDataRoleMaxVal(Qt::UserRole + 1),
    m_pWdgtEditPhysVal(nullptr),
    m_modelIdxEditorCreated(),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjNoisyMethods(nullptr)
{
    setObjectName(i_pModel->objectName());

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName());
    m_pTrcAdminObjNoisyMethods = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::NoisyMethods");

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ "" );

} // ctor

//------------------------------------------------------------------------------
CEditPhysValtemDelegate::~CEditPhysValtemDelegate()
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
    if( m_pTrcAdminObjNoisyMethods != nullptr ) {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjNoisyMethods);
    }

    m_pModel = nullptr;
    m_iItemDataRoleMinVal = 0;
    m_iItemDataRoleMaxVal = 0;
    m_pWdgtEditPhysVal = nullptr;
    //m_modelIdxEditorCreated;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjNoisyMethods = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CEditPhysValtemDelegate::setItemDataRoleMinimumValue(int i_iRole)
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
        /* strMethod          */ "setItemDataRoleMinimumValue",
        /* strMethodInArgs    */ strMthInArgs );

    m_iItemDataRoleMinVal = i_iRole;
}

//------------------------------------------------------------------------------
int CEditPhysValtemDelegate::itemDataRoleMinimumValue() const
//------------------------------------------------------------------------------
{
    return m_iItemDataRoleMinVal;
}

//------------------------------------------------------------------------------
void CEditPhysValtemDelegate::setItemDataRoleMaximumValue(int i_iRole)
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
        /* strMethod          */ "setItemDataRoleMaximumValue",
        /* strMethodInArgs    */ strMthInArgs );

    m_iItemDataRoleMaxVal = i_iRole;
}

//------------------------------------------------------------------------------
int CEditPhysValtemDelegate::itemDataRoleMaximumValue() const
//------------------------------------------------------------------------------
{
    return m_iItemDataRoleMaxVal;
}

/*==============================================================================
public: // overridables of base class QStyledItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
QWidget* CEditPhysValtemDelegate::createEditor(
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

    m_pWdgtEditPhysVal = new CWdgtEditPhysVal(objectName(), i_pWdgtParent);
    m_pWdgtEditPhysVal->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_modelIdxEditorCreated = i_modelIdx;
    QObject::connect(
        m_pWdgtEditPhysVal, &CWdgtEditPhysVal::valueChanged,
        this, &CEditPhysValtemDelegate::onEditPhysValValueChanged);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(QString(m_pWdgtEditPhysVal == nullptr ? "null" : "CWdgtEditPhysVal"));
    }
    return m_pWdgtEditPhysVal;
}

//------------------------------------------------------------------------------
void CEditPhysValtemDelegate::setEditorData(
    QWidget* i_pWdgtEditor, const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + qModelIndex2Str(i_modelIdx) + "}" +
            ", " + i_modelIdx.model()->data(i_modelIdx, Qt::EditRole).value<CPhysVal>().toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "setEditorData",
        /* strMethodInArgs    */ strMthInArgs );

    CWdgtEditPhysVal* pWdgt = dynamic_cast<CWdgtEditPhysVal*>(i_pWdgtEditor);
    if (pWdgt != m_pWdgtEditPhysVal) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pWdgtEditor != m_pWdgtEditPhysVal");
    }
    QObject::disconnect(
        m_pWdgtEditPhysVal, &CWdgtEditPhysVal::valueChanged,
        this, &CEditPhysValtemDelegate::onEditPhysValValueChanged);
    CPhysVal physVal = i_modelIdx.model()->data(i_modelIdx, Qt::EditRole).value<CPhysVal>();
    m_pWdgtEditPhysVal->setUnit(physVal.unit());
    m_pWdgtEditPhysVal->setMinimum(i_modelIdx.model()->data(i_modelIdx, m_iItemDataRoleMinVal).toDouble());
    m_pWdgtEditPhysVal->setMaximum(i_modelIdx.model()->data(i_modelIdx, m_iItemDataRoleMaxVal).toDouble());
    m_pWdgtEditPhysVal->setResolution(physVal.getRes().getVal());
    m_pWdgtEditPhysVal->setValue(physVal.getVal());
    QObject::connect(
        m_pWdgtEditPhysVal, &CWdgtEditPhysVal::valueChanged,
        this, &CEditPhysValtemDelegate::onEditPhysValValueChanged);
}

//------------------------------------------------------------------------------
void CEditPhysValtemDelegate::setModelData(
    QWidget* i_pWdgtEditor, QAbstractItemModel* i_pModel, const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    CWdgtEditPhysVal* pWdgt = dynamic_cast<CWdgtEditPhysVal*>(i_pWdgtEditor);
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + qModelIndex2Str(i_modelIdx) + "}" +
         ", " + QVariant::fromValue(pWdgt->value()).toString();
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
    if (pWdgt != m_pWdgtEditPhysVal) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pWdgtEditor != m_pWdgtEditPhysVal");
    }
    i_pModel->setData(i_modelIdx, QVariant::fromValue(m_pWdgtEditPhysVal->value()), Qt::EditRole);
}

//------------------------------------------------------------------------------
QSize CEditPhysValtemDelegate::sizeHint(const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + qModelIndex2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "sizeHint",
        /* strMethodInArgs    */ strMthInArgs );
    QSize size = i_modelIdx.model()->data(i_modelIdx, Qt::SizeHintRole).toSize();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qSize2Str(size));
    }
    return size;
}

//------------------------------------------------------------------------------
void CEditPhysValtemDelegate::updateEditorGeometry(
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
void CEditPhysValtemDelegate::onCloseEditor(QWidget* i_pWdgtEditor, QAbstractItemDelegate::EndEditHint i_hint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qItemDelegateEndEditHint2Str(i_hint);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onCloseEditor",
        /* strMethodInArgs    */ strMthInArgs );

    m_pWdgtEditPhysVal = nullptr;
}

//------------------------------------------------------------------------------
void CEditPhysValtemDelegate::onEditPhysValValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "onEditPhysValValueChanged",
        /* strMethodInArgs    */ strMthInArgs );

    m_pModel->setData(m_modelIdxEditorCreated, QVariant::fromValue(i_physVal), Qt::EditRole);
}
