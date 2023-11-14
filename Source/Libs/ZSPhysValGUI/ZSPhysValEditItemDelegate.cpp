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
    m_modelIdxEditorCreated(),
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
CEditPhysValtemDelegate::~CEditPhysValtemDelegate()
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
    //m_modelIdxEditorCreated;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjNoisyMethods = nullptr;

} // dtor

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
        /* strMethod          */ "createEditor",
        /* strMethodInArgs    */ strMthInArgs );

    CWdgtEditPhysVal* pWdgt = new CWdgtEditPhysVal(objectName(), i_pWdgtParent);
    m_modelIdxEditorCreated = i_modelIdx;
    QObject::connect(
        pWdgt, &CWdgtEditPhysVal::valueChanged,
        this, &CEditPhysValtemDelegate::onEditPhysValValueChanged);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(QString(pWdgt == nullptr ? "null" : "CWdgtEditPhysVal"));
    }
    return pWdgt;
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
        /* strMethod          */ "setEditorData",
        /* strMethodInArgs    */ strMthInArgs );

    CWdgtEditPhysVal* pWdgt = dynamic_cast<CWdgtEditPhysVal*>(i_pWdgtEditor);
    QObject::disconnect(
        pWdgt, &CWdgtEditPhysVal::valueChanged,
        this, &CEditPhysValtemDelegate::onEditPhysValValueChanged);
    CPhysVal physVal = i_modelIdx.model()->data(i_modelIdx, Qt::EditRole).value<CPhysVal>();
    pWdgt->setUnit(physVal.unit());
    pWdgt->setResolution(physVal.getRes().getVal());
    pWdgt->setValue(physVal.getVal());
    QObject::connect(
        pWdgt, &CWdgtEditPhysVal::valueChanged,
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
        /* strMethod          */ "setModelData",
        /* strMethodInArgs    */ strMthInArgs );
    if (i_pModel != m_pModel) {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }
    i_pModel->setData(i_modelIdx, QVariant::fromValue(pWdgt->value()), Qt::EditRole);
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
        /* strMethod          */ "updateEditorGeometry",
        /* strMethodInArgs    */ strMthInArgs );

    i_pWdgtEditor->setGeometry(i_option.rect);
}

/*==============================================================================
protected slots:
==============================================================================*/

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
        /* strMethod          */ "onEditPhysValValueChanged",
        /* strMethodInArgs    */ strMthInArgs );

    m_pModel->setData(m_modelIdxEditorCreated, QVariant::fromValue(i_physVal), Qt::EditRole);
}
