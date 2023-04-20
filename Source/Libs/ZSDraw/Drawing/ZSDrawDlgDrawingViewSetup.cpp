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

#include "ZSDraw/Drawing/ZSDrawDlgDrawingViewSetup.h"
#include "ZSDraw/Drawing/ZSDrawingViewPropertiesWdgt.h"
//#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ZSDrawingView.h"
#include "ZSSys/ZSSysAux.h"
//#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
//#include "ZSSys/ZSSysTrcServer.h"

//#include <QtCore/qglobal.h>
#include <QtCore/qsettings.h>
//#include <QtGui/qbitmap.h>
//#include <QtGui/qicon.h>

#if QT_VERSION < 0x050000
//#include <QtGui/qapplication.h>
#include <QtGui/qlayout.h>
//#include <QtGui/qlistwidget.h>
#include <QtGui/qpushbutton.h>
//#include <QtGui/qstackedwidget.h>
#else
//#include <QtWidgets/qapplication.h>
#include <QtWidgets/qlayout.h>
//#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qpushbutton.h>
//#include <QtWidgets/qstackedwidget.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;
//using namespace ZS::PhysVal;


/*******************************************************************************
class CDlgDrawingViewSetup : public CDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgDrawingViewSetup* CDlgDrawingViewSetup::CreateInstance(
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

    return new CDlgDrawingViewSetup(
        /* strDlgTitle  */ i_strDlgTitle,
        /* pDrawingView */ i_pDrawingView,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags );

} // CreateInstance

//------------------------------------------------------------------------------
CDlgDrawingViewSetup* CDlgDrawingViewSetup::GetInstance( CDrawingView* i_pDrawingView )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgDrawingViewSetup*>(
        CDialog::GetInstance(NameSpace(), ClassName(), i_pDrawingView->objectName()));
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgDrawingViewSetup::CDlgDrawingViewSetup(
    const QString&  i_strDlgTitle,
    CDrawingView*   i_pDrawingView,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags ) :
//------------------------------------------------------------------------------
    CDialog(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strNameSpace */ NameSpace(),
        /* strClassName */ ClassName(),
        /* strObjName   */ i_pDrawingView->objectName(),
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags ),
    m_pDrawingView(i_pDrawingView),
    m_pLyt(nullptr),
    //m_pLytSettings(nullptr),
    //m_pListWdgt(nullptr),
    //m_pStackedWdgt(nullptr),
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

    //int cxLblWidth = 60;
    //int cxEdtWidth = 80;

    // List Widget
    //============

    //QFont fntHeadLines;
    //QIcon iconPaper;
    //QIcon iconScene;

    //QPixmap pxmPaper(":/ZS/Draw/FilePageSetup16x16.bmp");
    //pxmPaper.setMask(pxmPaper.createHeuristicMask());
    //iconPaper.addPixmap(pxmPaper);

    //QPixmap pxmScene(":/ZS/Draw/FilePageSetup16x16.bmp");
    //pxmScene.setMask(pxmScene.createHeuristicMask());
    //iconScene.addPixmap(pxmScene);

    //m_pListWdgt = new QListWidget();
    //m_pListWdgt->setMaximumWidth(120);
    //m_pLytSettings->addWidget(m_pListWdgt);

    //new QListWidgetItem(iconPaper, tr("Paper"), m_pListWdgt);

    //new QListWidgetItem(iconScene, tr("View"), m_pListWdgt);

    //if( !connect(
    //    /* pObjSender   */ m_pListWdgt,
    //    /* szSignal     */ SIGNAL(currentRowChanged(int)),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onListWdgtCurrentRowChanged(int)) ) )
    //{
    //    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    //}

    // Widget Stack
    //=============

    //m_pStackedWdgt = new QStackedWidget();
    //m_pLytSettings->addWidget(m_pStackedWdgt,1);

    // Page Setup Widgets
    //===================

    // <Widget> Drawing Paper
    //-----------------------

    //m_pWdgtDrawingPaperPageSetup = new CWdgtDrawingPaperPageSetup(m_pDrawingView);
    //m_pStackedWdgt->addWidget(m_pWdgtDrawingPaperPageSetup);

    //if( !connect(
    //    /* pObjSender   */ m_pWdgtDrawingPaperPageSetup,
    //    /* szSignal     */ SIGNAL(settingsChanged()),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onWdgtDrawingPaperSettingsChanged()) ) )
    //{
    //    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    //}

    // <Widget> Drawing Scene
    //-----------------------

    m_pWdgtDrawingViewProperties = new CWdgtDrawingViewProperties(m_pDrawingView, EMode::Edit);
    m_pLyt->addWidget(m_pWdgtDrawingViewProperties);

    QObject::connect(
        m_pWdgtDrawingViewProperties, &CWdgtDrawingViewProperties::drawingSizeChanged,
        this, &CDlgDrawingViewSetup::onWdgtDrawingViewPropertiesDrawingSizeChanged);
    QObject::connect(
        m_pWdgtDrawingViewProperties, &CWdgtDrawingViewProperties::gridSettingsChanged,
        this, &CDlgDrawingViewSetup::onWdgtDrawingViewPropertiesGridSettingsChanged);

    // Dialog buttons
    //================

    m_pLytLineBtns = new QHBoxLayout();
    m_pLyt->addSpacing(5);
    m_pLyt->addLayout(m_pLytLineBtns);

    m_pBtnOk = new QPushButton("Ok");
    m_pLytLineBtns->addWidget(m_pBtnOk);
    m_pBtnOk->setDefault(true);
    QObject::connect(
        m_pBtnOk, &QPushButton::clicked,
        this, &CDlgDrawingViewSetup::onBtnOkClicked);

    m_pBtnApply = new QPushButton("Apply");
    m_pBtnApply->setEnabled(false);
    m_pLytLineBtns->addWidget(m_pBtnApply);
    QObject::connect(
        m_pBtnApply, &QPushButton::clicked,
        this, &CDlgDrawingViewSetup::onBtnApplyClicked);

    m_pBtnReset = new QPushButton("Reset");
    m_pBtnReset->setEnabled(false);
    m_pLytLineBtns->addWidget(m_pBtnReset);
    QObject::connect(
        m_pBtnReset, &QPushButton::clicked,
        this, &CDlgDrawingViewSetup::onBtnResetClicked);

    m_pBtnCancel = new QPushButton("Cancel");
    m_pLytLineBtns->addWidget(m_pBtnCancel);
    QObject::connect(
        m_pBtnCancel, &QPushButton::clicked,
        this, &CDlgDrawingViewSetup::onBtnCancelClicked);

    m_pLytLineBtns->addStretch();

    // Geometry of dialog
    //===================

    setMinimumHeight(360);
    setMinimumWidth(460);

    // Restore Geometry
    //-----------------

    QSettings settings;
    restoreGeometry( settings.value(objectName()+"/Geometry").toByteArray() );

} // ctor

//------------------------------------------------------------------------------
CDlgDrawingViewSetup::~CDlgDrawingViewSetup()
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
    //m_pLytSettings = nullptr;
    //m_pListWdgt = nullptr;
    //m_pStackedWdgt = nullptr;
    m_pWdgtDrawingViewProperties = nullptr;
    m_pLytLineBtns = nullptr;
    m_pBtnOk = nullptr;
    m_pBtnApply = nullptr;
    m_pBtnReset = nullptr;
    m_pBtnCancel = nullptr;

} // dtor

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDrawingViewSetup::onBtnOkClicked(bool /*i_bChecked*/)
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
void CDlgDrawingViewSetup::onBtnApplyClicked(bool /*i_bChecked*/)
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
void CDlgDrawingViewSetup::onBtnResetClicked(bool /*i_bChecked*/)
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
void CDlgDrawingViewSetup::onBtnCancelClicked(bool /*i_bChecked*/)
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
void CDlgDrawingViewSetup::onWdgtDrawingViewPropertiesDrawingSizeChanged(
    const CDrawingSize& i_size)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_size.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtDrawingViewPropertiesDrawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );

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
void CDlgDrawingViewSetup::onWdgtDrawingViewPropertiesGridSettingsChanged(
    const CDrawGridSettings& i_settings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_settings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtDrawingViewPropertiesGridSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

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
