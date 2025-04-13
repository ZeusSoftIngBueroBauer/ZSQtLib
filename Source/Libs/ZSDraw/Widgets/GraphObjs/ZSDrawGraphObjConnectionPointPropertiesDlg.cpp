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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjConnectionPointPropertiesDlg.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#if QT_VERSION < 0x050000
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qscrollarea.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qscrollarea.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CDlgGraphObjConnectionPointProperties : public ZS::System::GUI::CDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgGraphObjConnectionPointProperties* CDlgGraphObjConnectionPointProperties::CreateInstance(
    const QString& i_strDlgTitle,
    CGraphObjConnectionPoint* i_pGraphObjConnectionPoint,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wFlags )
//------------------------------------------------------------------------------
{
    if (GetInstance(i_pGraphObjConnectionPoint) != nullptr )
    {
        QString strKey = buildPathStr("::", NameSpace(), "Widgets::GraphObjs::Connections", ClassName(), i_pGraphObjConnectionPoint->keyInTree());
        throw CException(__FILE__, __LINE__, EResultSingletonClassAlreadyInstantiated, strKey);
    }

    return new CDlgGraphObjConnectionPointProperties(
        /* strDlgTitle */ i_strDlgTitle,
        /* pGraphObj   */ i_pGraphObjConnectionPoint,
        /* pWdgtParent */ i_pWdgtParent,
        /* wFlags      */ i_wFlags );
}

//------------------------------------------------------------------------------
CDlgGraphObjConnectionPointProperties* CDlgGraphObjConnectionPointProperties::GetInstance( CGraphObjConnectionPoint* i_pGraphObjConnectionPoint )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgGraphObjConnectionPointProperties*>(
        CDialog::GetInstance(NameSpace() + "::Widgets::GraphObjs::Connections", ClassName(), i_pGraphObjConnectionPoint->keyInTree()));
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgGraphObjConnectionPointProperties::CDlgGraphObjConnectionPointProperties(
    const QString& i_strDlgTitle,
    CGraphObjConnectionPoint* i_pGraphObjConnectionPoint,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wFlags ) :
//------------------------------------------------------------------------------
    CDialog(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strNameSpace */ NameSpace() + "::Widgets::GraphObjs::Connections",
        /* strClassName */ ClassName(),
        /* strObjName   */ "theInst",
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags ),
    m_pGraphObjConnectionPoint(i_pGraphObjConnectionPoint),
    m_pLyt(nullptr),
    m_pScrollArea(nullptr),
    m_pWdgtConnectionPointSettings(nullptr),
    // Buttons
    m_pLytLineBtns(nullptr),
    m_pBtnOk(nullptr),
    m_pBtnApply(nullptr),
    m_pBtnReset(nullptr),
    m_pBtnCancel(nullptr)
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Title: " + i_strDlgTitle
            + ", View: " + QString(i_pGraphObjConnectionPoint == nullptr ? "null" : i_pGraphObjConnectionPoint->objectName());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    QObject::connect(
        m_pGraphObjConnectionPoint, &CGraphObj::destroyed,
        this, &CDlgGraphObjConnectionPointProperties::onGraphObjDestroyed);

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // Style Widgets
    //==============

    CDrawingScene* pDrawingScene = m_pGraphObjConnectionPoint->drawingScene();

    m_pScrollArea = new QScrollArea();
    m_pScrollArea->setWidgetResizable(true);
    m_pLyt->addWidget(m_pScrollArea, 1);

    m_pWdgtConnectionPointSettings = new CWdgtGraphObjConnectionPointProperties(pDrawingScene, "DlgGraphObjConnectionPointProperties", false);
    m_pWdgtConnectionPointSettings->setKeyInTree(m_pGraphObjConnectionPoint->keyInTree());
    m_pScrollArea->setWidget(m_pWdgtConnectionPointSettings);
    QObject::connect(
        m_pWdgtConnectionPointSettings, &CWdgtGraphObjConnectionPointProperties::contentChanged,
        this, &CDlgGraphObjConnectionPointProperties::onWdgtConnectionPointSettingsContentChanged);

    // Dialog buttons
    //================

    m_pLytLineBtns = new QHBoxLayout();
    m_pLyt->addSpacing(5);
    m_pLyt->addLayout(m_pLytLineBtns);

    m_pLytLineBtns->addStretch();

    m_pBtnOk = new QPushButton("Ok");
    m_pLytLineBtns->addWidget(m_pBtnOk);
    m_pBtnOk->setDefault(true);
    QObject::connect(
        m_pBtnOk, &QPushButton::clicked,
        this, &CDlgGraphObjConnectionPointProperties::onBtnOkClicked);

    m_pBtnApply = new QPushButton("Accept");
    m_pBtnApply->setEnabled(false);
    m_pLytLineBtns->addWidget(m_pBtnApply);
    QObject::connect(
        m_pBtnApply, &QPushButton::clicked,
        this, &CDlgGraphObjConnectionPointProperties::onBtnApplyClicked);

    m_pBtnReset = new QPushButton("Reset");
    m_pBtnReset->setEnabled(false);
    m_pLytLineBtns->addWidget(m_pBtnReset);
    QObject::connect(
        m_pBtnReset, &QPushButton::clicked,
        this, &CDlgGraphObjConnectionPointProperties::onBtnResetClicked);

    m_pBtnCancel = new QPushButton("Cancel");
    m_pLytLineBtns->addWidget(m_pBtnCancel);
    QObject::connect(
        m_pBtnCancel, &QPushButton::clicked,
        this, &CDlgGraphObjConnectionPointProperties::onBtnCancelClicked);

    // Geometry of dialog
    //-------------------

    setMinimumHeight(360);
    setMinimumWidth(460);

} // ctor

//------------------------------------------------------------------------------
CDlgGraphObjConnectionPointProperties::~CDlgGraphObjConnectionPointProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pGraphObjConnectionPoint = nullptr;
    m_pLyt = nullptr;
    m_pScrollArea = nullptr;
    m_pWdgtConnectionPointSettings = nullptr;
    // Buttons
    m_pLytLineBtns = nullptr;
    m_pBtnOk = nullptr;
    m_pBtnApply = nullptr;
    m_pBtnReset = nullptr;
    m_pBtnCancel = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgGraphObjConnectionPointProperties::setCurrentWidget(CWdgtGraphObjConnectionPointProperties::EWidget i_widget)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CWdgtGraphObjConnectionPointProperties::widgetName(i_widget);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "setCurrentWidget",
        /* strAddInfo   */ strMthInArgs );

    m_pWdgtConnectionPointSettings->expand(i_widget, true);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgGraphObjConnectionPointProperties::onBtnOkClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "onBtnOkClicked",
        /* strAddInfo   */ "" );

    m_pWdgtConnectionPointSettings->acceptChanges();
    QDialog::accept();
}

//------------------------------------------------------------------------------
void CDlgGraphObjConnectionPointProperties::onBtnApplyClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "onBtnApplyClicked",
        /* strAddInfo   */ "" );

    m_pWdgtConnectionPointSettings->acceptChanges();

    // After accepting changes there should be no changes anymore.
    // Code just added for the sake of clarification.
    bool bHasChanges = m_pWdgtConnectionPointSettings->hasChanges();
    m_pBtnApply->setEnabled(bHasChanges);
    m_pBtnReset->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CDlgGraphObjConnectionPointProperties::onBtnResetClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "onBtnResetClicked",
        /* strAddInfo   */ "" );

    m_pWdgtConnectionPointSettings->rejectChanges();

    // After rejecting changes there should be no changes anymore.
    // Code just added for the sake of clarification.
    bool bHasChanges = m_pWdgtConnectionPointSettings->hasChanges();
    m_pBtnApply->setEnabled(bHasChanges);
    m_pBtnReset->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CDlgGraphObjConnectionPointProperties::onBtnCancelClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "onBtnCancelClicked",
        /* strAddInfo   */ "" );

    m_pWdgtConnectionPointSettings->rejectChanges();
    QDialog::reject();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgGraphObjConnectionPointProperties::onWdgtConnectionPointSettingsContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "onWdgtConnectionPointSettingsContentChanged",
        /* strAddInfo   */ "" );

    bool bHasChanges = m_pWdgtConnectionPointSettings->hasChanges();
    m_pBtnApply->setEnabled(bHasChanges);
    m_pBtnReset->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CDlgGraphObjConnectionPointProperties::onGraphObjDestroyed(QObject*)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "onGraphObjDestroyed",
        /* strAddInfo   */ "" );

    m_pGraphObjConnectionPoint = nullptr;

    QDialog::reject();
}
