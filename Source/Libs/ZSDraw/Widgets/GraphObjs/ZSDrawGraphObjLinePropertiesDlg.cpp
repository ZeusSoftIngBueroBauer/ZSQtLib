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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLinePropertiesDlg.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
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
class CDlgGraphObjLineProperties : public ZS::System::GUI::CDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgGraphObjLineProperties* CDlgGraphObjLineProperties::CreateInstance(
    const QString&  i_strDlgTitle,
    CGraphObjLine*  i_pGraphObjLine,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags )
//------------------------------------------------------------------------------
{
    if (GetInstance(i_pGraphObjLine) != nullptr )
    {
        QString strKey = buildPathStr("::", NameSpace(), "Widgets", "GraphObjs", ClassName(), i_pGraphObjLine->keyInTree());
        throw CException(__FILE__, __LINE__, EResultSingletonClassAlreadyInstantiated, strKey);
    }

    return new CDlgGraphObjLineProperties(
        /* strDlgTitle   */ i_strDlgTitle,
        /* pGraphObjLine */ i_pGraphObjLine,
        /* pWdgtParent   */ i_pWdgtParent,
        /* wFlags        */ i_wFlags );
}

//------------------------------------------------------------------------------
CDlgGraphObjLineProperties* CDlgGraphObjLineProperties::GetInstance( CGraphObjLine* i_pGraphObjLine )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgGraphObjLineProperties*>(
        CDialog::GetInstance(NameSpace() + "::Widgets::GraphObjs", ClassName(), i_pGraphObjLine->keyInTree()));
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgGraphObjLineProperties::CDlgGraphObjLineProperties(
    const QString&  i_strDlgTitle,
    CGraphObjLine*  i_pGraphObjLine,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags ) :
//------------------------------------------------------------------------------
    CDialog(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strNameSpace */ NameSpace() + "::Widgets::GraphObjs",
        /* strClassName */ ClassName(),
        /* strObjName   */ i_pGraphObjLine->keyInTree(),
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags ),
    m_pGraphObjLine(i_pGraphObjLine),
    m_pLyt(nullptr),
    m_pScrollArea(nullptr),
    m_pWdgtLineSettings(nullptr),
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
            + ", View: " + QString(i_pGraphObjLine == nullptr ? "null" : i_pGraphObjLine->objectName());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    QObject::connect(
        m_pGraphObjLine, &CGraphObj::destroyed,
        this, &CDlgGraphObjLineProperties::onGraphObjDestroyed);

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // Style Widgets
    //==============

    CDrawingScene* pDrawingScene = m_pGraphObjLine->getDrawingScene();

    m_pScrollArea = new QScrollArea();
    m_pScrollArea->setWidgetResizable(true);
    m_pLyt->addWidget(m_pScrollArea, 1);

    m_pWdgtLineSettings = new CWdgtGraphObjLineProperties(pDrawingScene, objectName(), false);
    m_pWdgtLineSettings->setKeyInTree(m_pGraphObjLine->keyInTree());
    m_pScrollArea->setWidget(m_pWdgtLineSettings);
    QObject::connect(
        m_pWdgtLineSettings, &CWdgtGraphObjLineProperties::contentChanged,
        this, &CDlgGraphObjLineProperties::onWdgtLineSettingsContentChanged);

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
        this, &CDlgGraphObjLineProperties::onBtnOkClicked);

    m_pBtnApply = new QPushButton("Accept");
    m_pBtnApply->setEnabled(false);
    m_pLytLineBtns->addWidget(m_pBtnApply);
    QObject::connect(
        m_pBtnApply, &QPushButton::clicked,
        this, &CDlgGraphObjLineProperties::onBtnApplyClicked);

    m_pBtnReset = new QPushButton("Reset");
    m_pBtnReset->setEnabled(false);
    m_pLytLineBtns->addWidget(m_pBtnReset);
    QObject::connect(
        m_pBtnReset, &QPushButton::clicked,
        this, &CDlgGraphObjLineProperties::onBtnResetClicked);

    m_pBtnCancel = new QPushButton("Cancel");
    m_pLytLineBtns->addWidget(m_pBtnCancel);
    QObject::connect(
        m_pBtnCancel, &QPushButton::clicked,
        this, &CDlgGraphObjLineProperties::onBtnCancelClicked);

    // Geometry of dialog
    //-------------------

    setMinimumHeight(360);
    setMinimumWidth(460);

} // ctor

//------------------------------------------------------------------------------
CDlgGraphObjLineProperties::~CDlgGraphObjLineProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pGraphObjLine = nullptr;
    m_pLyt = nullptr;
    m_pScrollArea = nullptr;
    m_pWdgtLineSettings = nullptr;
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
void CDlgGraphObjLineProperties::setCurrentWidget(CWdgtGraphObjLineProperties::EWidget i_widget)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CWdgtGraphObjLineProperties::widgetName(i_widget);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCurrentWidget",
        /* strAddInfo   */ strMthInArgs );

    m_pWdgtLineSettings->expand(i_widget, true);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgGraphObjLineProperties::onBtnOkClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnOkClicked",
        /* strAddInfo   */ "" );

    m_pWdgtLineSettings->acceptChanges();
    QDialog::accept();
}

//------------------------------------------------------------------------------
void CDlgGraphObjLineProperties::onBtnApplyClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnApplyClicked",
        /* strAddInfo   */ "" );

    m_pWdgtLineSettings->acceptChanges();

    // After accepting changes there should be no changes anymore.
    // Code just added for the sake of clarification.
    bool bHasChanges = m_pWdgtLineSettings->hasChanges();
    m_pBtnApply->setEnabled(bHasChanges);
    m_pBtnReset->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CDlgGraphObjLineProperties::onBtnResetClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnResetClicked",
        /* strAddInfo   */ "" );

    m_pWdgtLineSettings->rejectChanges();

    // After rejecting changes there should be no changes anymore.
    // Code just added for the sake of clarification.
    bool bHasChanges = m_pWdgtLineSettings->hasChanges();
    m_pBtnApply->setEnabled(bHasChanges);
    m_pBtnReset->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CDlgGraphObjLineProperties::onBtnCancelClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnCancelClicked",
        /* strAddInfo   */ "" );

    m_pWdgtLineSettings->rejectChanges();
    QDialog::reject();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgGraphObjLineProperties::onWdgtLineSettingsContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtLineSettingsContentChanged",
        /* strAddInfo   */ "" );

    bool bHasChanges = m_pWdgtLineSettings->hasChanges();
    m_pBtnApply->setEnabled(bHasChanges);
    m_pBtnReset->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CDlgGraphObjLineProperties::onGraphObjDestroyed(QObject*)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjDestroyed",
        /* strAddInfo   */ "" );

    m_pGraphObjLine = nullptr;

    QDialog::reject();
}
