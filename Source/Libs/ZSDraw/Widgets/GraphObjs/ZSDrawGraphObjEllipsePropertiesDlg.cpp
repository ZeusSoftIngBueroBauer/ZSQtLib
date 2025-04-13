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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjEllipsePropertiesDlg.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjEllipse.h"
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
class CDlgGraphObjEllipseProperties : public ZS::System::GUI::CDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgGraphObjEllipseProperties* CDlgGraphObjEllipseProperties::CreateInstance(
    const QString& i_strDlgTitle,
    CGraphObjEllipse* i_pGraphObjEllipse,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wFlags )
//------------------------------------------------------------------------------
{
    if (GetInstance(i_pGraphObjEllipse) != nullptr )
    {
        QString strKey = buildPathStr("::", NameSpace(), "Widgets::GraphObjs::StandardShapes", ClassName(), i_pGraphObjEllipse->keyInTree());
        throw CException(__FILE__, __LINE__, EResultSingletonClassAlreadyInstantiated, strKey);
    }

    return new CDlgGraphObjEllipseProperties(
        /* strDlgTitle   */ i_strDlgTitle,
        /* pGraphObjEllipse */ i_pGraphObjEllipse,
        /* pWdgtParent   */ i_pWdgtParent,
        /* wFlags        */ i_wFlags );
}

//------------------------------------------------------------------------------
CDlgGraphObjEllipseProperties* CDlgGraphObjEllipseProperties::GetInstance( CGraphObjEllipse* i_pGraphObjEllipse )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgGraphObjEllipseProperties*>(
        CDialog::GetInstance(NameSpace() + "::Widgets::GraphObjs::StandardShapes", ClassName(), i_pGraphObjEllipse->keyInTree()));
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgGraphObjEllipseProperties::CDlgGraphObjEllipseProperties(
    const QString& i_strDlgTitle,
    CGraphObjEllipse* i_pGraphObjEllipse,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wFlags ) :
//------------------------------------------------------------------------------
    CDialog(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strNameSpace */ NameSpace() + "::Widgets::GraphObjs::StandardShapes",
        /* strClassName */ ClassName(),
        /* strObjName   */ "theInst",
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags ),
    m_pGraphObjEllipse(i_pGraphObjEllipse),
    m_pLyt(nullptr),
    m_pScrollArea(nullptr),
    m_pWdgtEllipseSettings(nullptr),
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
            + ", View: " + QString(i_pGraphObjEllipse == nullptr ? "null" : i_pGraphObjEllipse->objectName());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    QObject::connect(
        m_pGraphObjEllipse, &CGraphObj::destroyed,
        this, &CDlgGraphObjEllipseProperties::onGraphObjDestroyed);

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // Style Widgets
    //==============

    CDrawingScene* pDrawingScene = m_pGraphObjEllipse->drawingScene();

    m_pScrollArea = new QScrollArea();
    m_pScrollArea->setWidgetResizable(true);
    m_pLyt->addWidget(m_pScrollArea, 1);

    m_pWdgtEllipseSettings = new CWdgtGraphObjEllipseProperties(pDrawingScene, "DlgGraphObjEllipseProperties", false);
    m_pWdgtEllipseSettings->setKeyInTree(m_pGraphObjEllipse->keyInTree());
    m_pScrollArea->setWidget(m_pWdgtEllipseSettings);
    QObject::connect(
        m_pWdgtEllipseSettings, &CWdgtGraphObjEllipseProperties::contentChanged,
        this, &CDlgGraphObjEllipseProperties::onWdgtEllipseSettingsContentChanged);

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
        this, &CDlgGraphObjEllipseProperties::onBtnOkClicked);

    m_pBtnApply = new QPushButton("Accept");
    m_pBtnApply->setEnabled(false);
    m_pLytLineBtns->addWidget(m_pBtnApply);
    QObject::connect(
        m_pBtnApply, &QPushButton::clicked,
        this, &CDlgGraphObjEllipseProperties::onBtnApplyClicked);

    m_pBtnReset = new QPushButton("Reset");
    m_pBtnReset->setEnabled(false);
    m_pLytLineBtns->addWidget(m_pBtnReset);
    QObject::connect(
        m_pBtnReset, &QPushButton::clicked,
        this, &CDlgGraphObjEllipseProperties::onBtnResetClicked);

    m_pBtnCancel = new QPushButton("Cancel");
    m_pLytLineBtns->addWidget(m_pBtnCancel);
    QObject::connect(
        m_pBtnCancel, &QPushButton::clicked,
        this, &CDlgGraphObjEllipseProperties::onBtnCancelClicked);

    // Geometry of dialog
    //-------------------

    setMinimumHeight(360);
    setMinimumWidth(460);

} // ctor

//------------------------------------------------------------------------------
CDlgGraphObjEllipseProperties::~CDlgGraphObjEllipseProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pGraphObjEllipse = nullptr;
    m_pLyt = nullptr;
    m_pScrollArea = nullptr;
    m_pWdgtEllipseSettings = nullptr;
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
void CDlgGraphObjEllipseProperties::setCurrentWidget(CWdgtGraphObjEllipseProperties::EWidget i_widget)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CWdgtGraphObjEllipseProperties::widgetName(i_widget);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "setCurrentWidget",
        /* strAddInfo   */ strMthInArgs );

    m_pWdgtEllipseSettings->expand(i_widget, true);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgGraphObjEllipseProperties::onBtnOkClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "onBtnOkClicked",
        /* strAddInfo   */ "" );

    m_pWdgtEllipseSettings->acceptChanges();
    QDialog::accept();
}

//------------------------------------------------------------------------------
void CDlgGraphObjEllipseProperties::onBtnApplyClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "onBtnApplyClicked",
        /* strAddInfo   */ "" );

    m_pWdgtEllipseSettings->acceptChanges();

    // After accepting changes there should be no changes anymore.
    // Code just added for the sake of clarification.
    bool bHasChanges = m_pWdgtEllipseSettings->hasChanges();
    m_pBtnApply->setEnabled(bHasChanges);
    m_pBtnReset->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CDlgGraphObjEllipseProperties::onBtnResetClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "onBtnResetClicked",
        /* strAddInfo   */ "" );

    m_pWdgtEllipseSettings->rejectChanges();

    // After rejecting changes there should be no changes anymore.
    // Code just added for the sake of clarification.
    bool bHasChanges = m_pWdgtEllipseSettings->hasChanges();
    m_pBtnApply->setEnabled(bHasChanges);
    m_pBtnReset->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CDlgGraphObjEllipseProperties::onBtnCancelClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "onBtnCancelClicked",
        /* strAddInfo   */ "" );

    m_pWdgtEllipseSettings->rejectChanges();
    QDialog::reject();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgGraphObjEllipseProperties::onWdgtEllipseSettingsContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "onWdgtEllipseSettingsContentChanged",
        /* strAddInfo   */ "" );

    bool bHasChanges = m_pWdgtEllipseSettings->hasChanges();
    m_pBtnApply->setEnabled(bHasChanges);
    m_pBtnReset->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CDlgGraphObjEllipseProperties::onGraphObjDestroyed(QObject*)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "onGraphObjDestroyed",
        /* strAddInfo   */ "" );

    m_pGraphObjEllipse = nullptr;

    QDialog::reject();
}
