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

#include <QtCore/qglobal.h>
#include <QtCore/qsettings.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qicon.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlistwidget.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qstackedwidget.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qstackedwidget.h>
#endif

#include "ZSDraw/DrawingPageSetup/ZSDrawDlgDrawingViewSetup.h"
#include "ZSDraw/Drawing/ZSDrawingViewPropertiesWdgt.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ZSDrawingView.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDlgDrawingViewSetup : public QDialog
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgDrawingViewSetup::CDlgDrawingViewSetup( CDrawingView* i_pDrawingView, QWidget* /*i_pWdgtParent*/ ) :
//------------------------------------------------------------------------------
    QDialog(i_pDrawingView),
    m_pDrawingView(i_pDrawingView),
    m_pLyt(nullptr),
    m_pListWdgt(nullptr),
    m_pStackedWdgt(nullptr),
    // Format Widgets
    //m_pWdgtDrawingPaperPageSetup(nullptr),
    m_pWdgtDrawingViewProperties(nullptr),
    // Buttons
    m_pLytBtns(nullptr),
    m_pBtnOk(nullptr),
    m_pBtnAccept(nullptr),
    m_pBtnCancel(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setWindowTitle( QApplication::applicationName() + ": Page Setup");

    setObjectName("DlgPageSetup");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

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

    QFont fntHeadLines;
    QIcon iconPaper;
    QIcon iconScene;

    QPixmap pxmPaper(":/ZS/Draw/FilePageSetup16x16.bmp");
    pxmPaper.setMask(pxmPaper.createHeuristicMask());
    iconPaper.addPixmap(pxmPaper);

    QPixmap pxmScene(":/ZS/Draw/FilePageSetup16x16.bmp");
    pxmScene.setMask(pxmScene.createHeuristicMask());
    iconScene.addPixmap(pxmScene);

    m_pListWdgt = new QListWidget();
    m_pListWdgt->setMaximumWidth(120);
    m_pLytSettings->addWidget(m_pListWdgt);

    new QListWidgetItem(iconPaper, tr("Paper"), m_pListWdgt);

    new QListWidgetItem(iconScene, tr("View"), m_pListWdgt);

    if( !connect(
        /* pObjSender   */ m_pListWdgt,
        /* szSignal     */ SIGNAL(currentRowChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onListWdgtCurrentRowChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // Widget Stack
    //=============

    m_pStackedWdgt = new QStackedWidget();
    m_pLytSettings->addWidget(m_pStackedWdgt,1);

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

    m_pWdgtDrawingViewProperties = new CWdgtDrawingViewProperties(m_pDrawingView);
    m_pStackedWdgt->addWidget(m_pWdgtDrawingViewProperties);

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtDrawingViewProperties,
        /* szSignal     */ SIGNAL(settingsChanged()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onWdgtDrawingViewSettingsChanged()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // Dialog buttons
    //================

    m_pLytBtns = new QHBoxLayout();
    m_pLyt->addSpacing(5);
    m_pLyt->addLayout(m_pLytBtns);

    m_pLytBtns->addStretch();

    m_pBtnOk = new QPushButton("Ok");
    m_pLytBtns->addWidget(m_pBtnOk);
    m_pBtnOk->setDefault(true);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnOk,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(accept()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    m_pBtnAccept = new QPushButton("Accept");
    m_pBtnAccept->setEnabled(false);
    m_pLytBtns->addWidget(m_pBtnAccept);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnAccept,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnAcceptClicked()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    m_pBtnCancel = new QPushButton("Cancel");
    m_pLytBtns->addWidget(m_pBtnCancel);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnCancel,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(reject()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

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

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pDrawingView = nullptr;
    m_pLyt = nullptr;
    m_pLytSettings = nullptr;
    m_pListWdgt = nullptr;
    m_pStackedWdgt = nullptr;
    // Format Widgets
    //m_pWdgtDrawingPaperPageSetup = nullptr;
    m_pWdgtDrawingViewProperties = nullptr;
    // Buttons
    m_pLytBtns = nullptr;
    m_pBtnOk = nullptr;
    m_pBtnAccept = nullptr;
    m_pBtnCancel = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDrawingViewSetup::setCurrentWidget( EWidget i_wdgt )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCurrentWidget",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_wdgt >= 0 && i_wdgt < EWidgetCount )
    {
        m_pListWdgt->setCurrentRow(i_wdgt);
    }

} // setCurrentWidget

/*==============================================================================
protected slots: // overridables of base class QDialog
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDrawingViewSetup::closeEvent( QCloseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "closeEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QDialog::closeEvent(i_pEv);

} // closeEvent

//------------------------------------------------------------------------------
void CDlgDrawingViewSetup::showEvent( QShowEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "showEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QDialog::showEvent(i_pEv);

} // showEvent

/*==============================================================================
protected slots: // overridables of base class QDialog
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDrawingViewSetup::accept()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "accept",
        /* strAddInfo   */ strAddTrcInfo );

    onBtnAcceptClicked();

    QDialog::accept();

} // accept

//------------------------------------------------------------------------------
void CDlgDrawingViewSetup::reject()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "reject",
        /* strAddInfo   */ strAddTrcInfo );

    QDialog::reject();

} // reject

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDrawingViewSetup::onBtnAcceptClicked()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnAcceptClicked",
        /* strAddInfo   */ strAddTrcInfo );

    //m_pWdgtDrawingPaperPageSetup->acceptChanges();
    //m_pWdgtDrawingViewProperties->acceptChanges();

    //if( /*m_pWdgtDrawingPaperPageSetup->hasChanges() ||*/ m_pWdgtDrawingViewProperties->hasChanges() )
    //{
    //    m_pBtnAccept->setEnabled(true);
    //}
    //else
    //{
    //    m_pBtnAccept->setEnabled(false);
    //}

} // onBtnAcceptClicked

/*==============================================================================
protected slots: // instance methods (List Widget)
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDrawingViewSetup::onListWdgtCurrentRowChanged( int i_iRow )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onListWdgtCurrentRowChanged",
        /* strAddInfo   */ strAddTrcInfo );

    m_pStackedWdgt->setCurrentIndex(i_iRow);

} // onListWdgtCurrentRowChanged

/*==============================================================================
protected slots: // Format Widgets
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDrawingViewSetup::onWdgtDrawingPaperSettingsChanged()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtDrawingPaperSettingsChanged",
        /* strAddInfo   */ strAddTrcInfo );

    //if( /*m_pWdgtDrawingPaperPageSetup->hasChanges() ||*/ m_pWdgtDrawingViewProperties->hasChanges() )
    //{
    //    m_pBtnAccept->setEnabled(true);
    //}
    //else
    //{
    //    m_pBtnAccept->setEnabled(false);
    //}

} // onWdgtDrawingPaperSettingsChanged

//------------------------------------------------------------------------------
void CDlgDrawingViewSetup::onWdgtDrawingViewSettingsChanged()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtDrawingViewSettingsChanged",
        /* strAddInfo   */ strAddTrcInfo );

    //if( /*m_pWdgtDrawingPaperPageSetup->hasChanges() ||*/ m_pWdgtDrawingViewProperties->hasChanges() )
    //{
    //    m_pBtnAccept->setEnabled(true);
    //}
    //else
    //{
    //    m_pBtnAccept->setEnabled(false);
    //}

} // onWdgtDrawingViewSettingsChanged
