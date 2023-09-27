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

#include "ZSDraw/Widgets/Drawing/ZSDrawingViewPropertiesDlg.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingViewPropertiesWdgt.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingView.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include <QtCore/qsettings.h>

#if QT_VERSION < 0x050000
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;
//using namespace ZS::PhysVal;


/*******************************************************************************
class CDlgDrawingViewProperties : public CDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgDrawingViewProperties* CDlgDrawingViewProperties::CreateInstance(
    const QString&  i_strDlgTitle,
    CDrawingView*   i_pDrawingView,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags )
//------------------------------------------------------------------------------
{
    if( CDialog::GetInstance(NameSpace(), ClassName(), i_pDrawingView->objectName()) != nullptr )
    {
        QString strKey = buildPathStr("::", NameSpace(), ClassName(), i_pDrawingView->objectName());
        throw CException(__FILE__, __LINE__, EResultSingletonClassAlreadyInstantiated, strKey);
    }

    return new CDlgDrawingViewProperties(
        /* strDlgTitle  */ i_strDlgTitle,
        /* pDrawingView */ i_pDrawingView,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags );
}

//------------------------------------------------------------------------------
CDlgDrawingViewProperties* CDlgDrawingViewProperties::GetInstance( CDrawingView* i_pDrawingView )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgDrawingViewProperties*>(
        CDialog::GetInstance(NameSpace(), ClassName(), i_pDrawingView->objectName()));
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgDrawingViewProperties::CDlgDrawingViewProperties(
    const QString&  i_strDlgTitle,
    CDrawingView*   i_pDrawingView,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags ) :
//------------------------------------------------------------------------------
    CDialog(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strNameSpace */ NameSpace() + "::Widgets::Drawing",
        /* strClassName */ ClassName(),
        /* strObjName   */ i_pDrawingView->objectName(),
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags ),
    m_pDrawingView(i_pDrawingView),
    m_pLyt(nullptr),
    m_pWdgtDrawingViewProperties(nullptr),
    m_pLytLineBtns(nullptr),
    m_pBtnOk(nullptr),
    m_pBtnApply(nullptr),
    m_pBtnReset(nullptr),
    m_pBtnCancel(nullptr)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    QHBoxLayout* m_pLytSettings = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSettings);

    setMinimumHeight(480);
    setMinimumWidth(560);

    // <Widget> Drawing Scene
    //-----------------------

    m_pWdgtDrawingViewProperties = new CWdgtDrawingViewProperties(m_pDrawingView, "DialogPageSetup", false);
    m_pLyt->addWidget(m_pWdgtDrawingViewProperties);

    QObject::connect(
        m_pWdgtDrawingViewProperties, &CWdgtDrawingViewProperties::contentChanged,
        this, &CDlgDrawingViewProperties::onWdgtDrawingViewPropertiesContentChanged);

    // Dialog buttons
    //---------------

    m_pLytLineBtns = new QHBoxLayout();
    m_pLyt->addSpacing(5);
    m_pLyt->addLayout(m_pLytLineBtns);

    m_pBtnOk = new QPushButton("Ok");
    m_pLytLineBtns->addWidget(m_pBtnOk);
    m_pBtnOk->setDefault(true);
    QObject::connect(
        m_pBtnOk, &QPushButton::clicked,
        this, &CDlgDrawingViewProperties::onBtnOkClicked);

    m_pBtnApply = new QPushButton("Apply");
    m_pBtnApply->setEnabled(false);
    m_pLytLineBtns->addWidget(m_pBtnApply);
    QObject::connect(
        m_pBtnApply, &QPushButton::clicked,
        this, &CDlgDrawingViewProperties::onBtnApplyClicked);

    m_pBtnReset = new QPushButton("Reset");
    m_pBtnReset->setEnabled(false);
    m_pLytLineBtns->addWidget(m_pBtnReset);
    QObject::connect(
        m_pBtnReset, &QPushButton::clicked,
        this, &CDlgDrawingViewProperties::onBtnResetClicked);

    m_pBtnCancel = new QPushButton("Cancel");
    m_pLytLineBtns->addWidget(m_pBtnCancel);
    QObject::connect(
        m_pBtnCancel, &QPushButton::clicked,
        this, &CDlgDrawingViewProperties::onBtnCancelClicked);

    m_pLytLineBtns->addStretch();

    // Restore Geometry
    //-----------------

    QSettings settings;
    restoreGeometry( settings.value(objectName()+"/Geometry").toByteArray() );

} // ctor

//------------------------------------------------------------------------------
CDlgDrawingViewProperties::~CDlgDrawingViewProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    QSettings settings;
    settings.setValue( objectName()+"/Geometry", saveGeometry() );

    m_pDrawingView = nullptr;
    m_pLyt = nullptr;
    m_pWdgtDrawingViewProperties = nullptr;
    m_pLytLineBtns = nullptr;
    m_pBtnOk = nullptr;
    m_pBtnApply = nullptr;
    m_pBtnReset = nullptr;
    m_pBtnCancel = nullptr;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDrawingViewProperties::onBtnOkClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnOkClicked",
        /* strAddInfo   */ "" );

    m_pWdgtDrawingViewProperties->acceptChanges();

    QDialog::accept();
}

//------------------------------------------------------------------------------
void CDlgDrawingViewProperties::onBtnApplyClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnApplyClicked",
        /* strAddInfo   */ "" );

    m_pWdgtDrawingViewProperties->acceptChanges();

    if( m_pWdgtDrawingViewProperties->hasChanges() )
    {
        m_pBtnApply->setEnabled(true);
        m_pBtnReset->setEnabled(true);
    }
    else
    {
        m_pBtnApply->setEnabled(false);
        m_pBtnReset->setEnabled(false);
    }
}

//------------------------------------------------------------------------------
void CDlgDrawingViewProperties::onBtnResetClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnResetClicked",
        /* strAddInfo   */ "" );

    m_pWdgtDrawingViewProperties->rejectChanges();

    if( m_pWdgtDrawingViewProperties->hasChanges() )
    {
        m_pBtnApply->setEnabled(true);
        m_pBtnReset->setEnabled(true);
    }
    else
    {
        m_pBtnApply->setEnabled(false);
        m_pBtnReset->setEnabled(false);
    }
}

//------------------------------------------------------------------------------
void CDlgDrawingViewProperties::onBtnCancelClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnCancelClicked",
        /* strAddInfo   */ "" );

    m_pWdgtDrawingViewProperties->rejectChanges();

    QDialog::reject();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDrawingViewProperties::onWdgtDrawingViewPropertiesContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtDrawingViewPropertiesDrawingSizeChanged",
        /* strAddInfo   */ "" );

    if( m_pWdgtDrawingViewProperties->hasChanges() )
    {
        m_pBtnApply->setEnabled(true);
        m_pBtnReset->setEnabled(true);
    }
    else
    {
        m_pBtnApply->setEnabled(false);
        m_pBtnReset->setEnabled(false);
    }
}
