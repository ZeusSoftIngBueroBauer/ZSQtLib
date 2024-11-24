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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPolygonGeometryTableView.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPolygonGeometryModel.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qmenu.h>
#else
#include <QtWidgets/qmenu.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CTableViewGraphObjPolygonGeometry : public ZS::System::GUI::CTableView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTableViewGraphObjPolygonGeometry::CTableViewGraphObjPolygonGeometry(
    const QString& i_strObjName,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CTableView(i_strObjName, i_pWdgtParent),
    m_pModel(nullptr),
    m_pMenu(nullptr),
    m_pActionDelete(nullptr),
    m_pActionInsertBefore(nullptr),
    m_pActionInsertAfter(nullptr),
    m_modelIdxSelectedOnMousePressEvent(),
    m_modelIdxSelectedOnMouseReleaseEvent(),
    // Trace admin object for method tracing
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::GraphObjs::StandardShapes::Polygon", ClassName(), i_strObjName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pMenu = new QMenu("Point", this);

    m_pActionDelete = new QAction("Delete", this);
    m_pMenu->addAction(m_pActionDelete);
    QObject::connect(
        m_pActionDelete, &QAction::triggered,
        this, &CTableViewGraphObjPolygonGeometry::onActionDeleteTriggered );

    m_pActionInsertBefore = new QAction("Insert Before", this);
    m_pMenu->addAction(m_pActionInsertBefore);
    QObject::connect(
        m_pActionInsertBefore, &QAction::triggered,
        this, &CTableViewGraphObjPolygonGeometry::onActionInsertBeforeTriggered );

    m_pActionInsertAfter = new QAction("Insert After", this);
    m_pMenu->addAction(m_pActionInsertAfter);
    QObject::connect(
        m_pActionInsertAfter, &QAction::triggered,
        this, &CTableViewGraphObjPolygonGeometry::onActionInsertAfterTriggered );
}

//------------------------------------------------------------------------------
CTableViewGraphObjPolygonGeometry::~CTableViewGraphObjPolygonGeometry()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pModel = nullptr;
    m_pMenu = nullptr;
    m_pActionDelete = nullptr;
    m_pActionInsertBefore = nullptr;
    m_pActionInsertAfter = nullptr;
    // Trace admin object for method tracing
    m_pTrcAdminObj = nullptr;
}

/*==============================================================================
public: // overridables of base class QTableView
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewGraphObjPolygonGeometry::setModel(QAbstractItemModel* i_pModel)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pModel == nullptr ? "null" : i_pModel->objectName());
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setModel",
        /* strMethodInArgs    */ strMthInArgs );

    m_pModel = dynamic_cast<CModelGraphObjPolygonGeometry*>(i_pModel);
    if (m_pModel == nullptr) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    QTableView::setModel(i_pModel);
}

/*==============================================================================
protected: // overridables of base class QAbstractItemView
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewGraphObjPolygonGeometry::mousePressEvent(QMouseEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "mousePressEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;
    m_modelIdxSelectedOnMousePressEvent = indexAt(i_pEv->pos());
    if (m_modelIdxSelectedOnMousePressEvent.isValid()) {
        if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
            strMthInArgs = "ModelIdxPressed {" + qModelIndex2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
        }
        if (i_pEv->buttons() & Qt::RightButton) {
            m_pMenu->popup(QWidget::mapToGlobal(i_pEv->pos()));
            bEventHandled = true;
        }
    }
    if (!bEventHandled) {
        CTableView::mousePressEvent(i_pEv);
    }
}

//------------------------------------------------------------------------------
void CTableViewGraphObjPolygonGeometry::mouseReleaseEvent(QMouseEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "mouseReleaseEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;
    m_modelIdxSelectedOnMouseReleaseEvent = indexAt(i_pEv->pos());
    if (m_modelIdxSelectedOnMouseReleaseEvent.isValid()) {
        if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
            strMthInArgs = "ModelIdxReleased {" + qModelIndex2Str(m_modelIdxSelectedOnMouseReleaseEvent) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
        }
    }
    if (!bEventHandled) {
        CTableView::mouseReleaseEvent(i_pEv);
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewGraphObjPolygonGeometry::onActionDeleteTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionDeleteTriggered",
        /* strMethodInArgs    */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdxPressed {" + qModelIndex2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
        mthTracer.trace(strMthInArgs, ELogDetailLevel::None, ELogDetailLevel::None);
    }
}

//------------------------------------------------------------------------------
void CTableViewGraphObjPolygonGeometry::onActionInsertBeforeTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionInsertBeforeTriggered",
        /* strMethodInArgs    */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdxPressed {" + qModelIndex2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
        mthTracer.trace(strMthInArgs, ELogDetailLevel::None, ELogDetailLevel::None);
    }
}

//------------------------------------------------------------------------------
void CTableViewGraphObjPolygonGeometry::onActionInsertAfterTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionInsertAfterTriggered",
        /* strMethodInArgs    */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdxPressed {" + qModelIndex2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
        mthTracer.trace(strMthInArgs, ELogDetailLevel::None, ELogDetailLevel::None);
    }
}
