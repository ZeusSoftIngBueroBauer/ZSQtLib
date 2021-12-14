/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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
#include <QtCore/qtimer.h>
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

#include "ZSDraw/ZSDrawDlgPageSetup.h"
#include "ZSDraw/ZSDrawWdgtPageSetupDrawingPaper.h"
#include "ZSDraw/ZSDrawWdgtPageSetupDrawingScene.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawingView.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
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
using namespace ZS::Trace;


/*******************************************************************************
class CDlgPageSetup : public QDialog
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgPageSetup::CDlgPageSetup( CDrawingView* i_pDrawingView, QWidget* /*i_pWdgtParent*/ ) :
//------------------------------------------------------------------------------
    QDialog(i_pDrawingView),
    m_pDrawingView(i_pDrawingView),
    m_pageSetupOld(),
    m_pageSetupNew(),
    m_pLyt(nullptr),
    m_pListWdgt(nullptr),
    m_pStackedWdgt(nullptr),
    // Format Widgets
    //m_arpListWdgtItems[EWidgetCount]
    //m_arpWdgtsPageSetup[EWidgetCount]
    // Buttons
    m_pLytBtns(nullptr),
    m_pBtnOk(nullptr),
    m_pBtnAccept(nullptr),
    m_pBtnReset(nullptr),
    m_pBtnCancel(nullptr),
    m_pTmrBtnsStateRefresh(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    memset( m_arpListWdgtItems, 0x00, EWidgetCount*sizeof(m_arpListWdgtItems[0]) );
    memset( m_arpWdgtsPageSetup, 0x00, EWidgetCount*sizeof(m_arpWdgtsPageSetup[0]) );

    setWindowTitle( QApplication::applicationName() + ": Page Setup");

    setObjectName("DlgPageSetup");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Draw", "CDlgPageSetup", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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

    QListWidgetItem* pListWdgtItemPaper = new QListWidgetItem( iconPaper, tr("Paper"), m_pListWdgt );
    m_arpListWdgtItems[EWidgetDrawingPaper] = pListWdgtItemPaper;

    QListWidgetItem* pListWdgtItemScene = new QListWidgetItem( iconScene, tr("Scene"), m_pListWdgt );
    m_arpListWdgtItems[EWidgetDrawingScene] = pListWdgtItemScene;

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

    CWdgtPageSetupDrawingPaper* pWdgtDrawingPaper = new CWdgtPageSetupDrawingPaper(m_pDrawingView);

    m_arpWdgtsPageSetup[EWidgetDrawingPaper] = pWdgtDrawingPaper;
    pWdgtDrawingPaper->setSettings(m_pageSetupNew);
    m_pStackedWdgt->addWidget(pWdgtDrawingPaper);

    if( !connect(
        /* pObjSender   */ pWdgtDrawingPaper,
        /* szSignal     */ SIGNAL(pageSetupAttributeChanged(int,const QVariant&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onPageSetupAttributeChanged(int,const QVariant&)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Widget> Drawing Scene
    //-----------------------

    CWdgtPageSetupDrawingScene* pWdgtDrawingScene = new CWdgtPageSetupDrawingScene(m_pDrawingView);

    m_arpWdgtsPageSetup[EWidgetDrawingScene] = pWdgtDrawingScene;
    pWdgtDrawingScene->setSettings(m_pageSetupNew);
    m_pStackedWdgt->addWidget(pWdgtDrawingScene);

    if( !connect(
        /* pObjSender   */ pWdgtDrawingScene,
        /* szSignal     */ SIGNAL(pageSetupAttributeChanged(int,const QVariant&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onPageSetupAttributeChanged(int,const QVariant&)) ) )
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

    if( !connect(
        /* pObjSender   */ m_pBtnOk,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(accept()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    m_pBtnAccept = new QPushButton("Accept");
    m_pLytBtns->addWidget(m_pBtnAccept);

    if( !connect(
        /* pObjSender   */ m_pBtnAccept,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnAcceptClicked()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    m_pBtnReset = new QPushButton("Reset");
    m_pLytBtns->addWidget(m_pBtnReset);

    if( !connect(
        /* pObjSender   */ m_pBtnReset,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnResetClicked()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    m_pBtnCancel = new QPushButton("Cancel");
    m_pLytBtns->addWidget(m_pBtnCancel);

    if( !connect(
        /* pObjSender   */ m_pBtnCancel,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(reject()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    m_pTmrBtnsStateRefresh = new QTimer(this);

    if( !connect(
        /* pObjSender   */ m_pTmrBtnsStateRefresh,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTmrBtnsStateRefreshTimeout()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // Geometry of dialog
    //-------------------

    setMinimumHeight(360);
    setMinimumWidth(460);

} // ctor

//------------------------------------------------------------------------------
CDlgPageSetup::~CDlgPageSetup()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pDrawingView = nullptr;
    m_pLyt = nullptr;
    m_pLytSettings = nullptr;
    m_pListWdgt = nullptr;
    m_pStackedWdgt = nullptr;
    // Format Widgets
    memset( m_arpListWdgtItems, 0x00, EWidgetCount*sizeof(m_arpListWdgtItems[0]) );
    memset( m_arpWdgtsPageSetup, 0x00, EWidgetCount*sizeof(m_arpWdgtsPageSetup[0]) );
    // Buttons
    m_pLytBtns = nullptr;
    m_pBtnOk = nullptr;
    m_pBtnAccept = nullptr;
    m_pBtnReset = nullptr;
    m_pBtnCancel = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgPageSetup::setCurrentWidget( EWidget i_wdgt )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCurrentWidget",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_wdgt >= 0 && i_wdgt < EWidgetCount )
    {
        int idxWdgt = -1;

        for( int idxTmp = 0; idxTmp < m_pListWdgt->count(); idxTmp++ )
        {
            if( m_pListWdgt->item(idxTmp) == m_arpListWdgtItems[i_wdgt] )
            {
                idxWdgt = idxTmp;
                break;
            }
        }

        if( idxWdgt >= 0 && idxWdgt < m_pListWdgt->count() )
        {
            m_pListWdgt->setCurrentRow(idxWdgt);
        }
    }

} // setCurrentWidget

/*==============================================================================
protected slots: // overridables of base class QDialog
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgPageSetup::closeEvent( QCloseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "closeEvent",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups();

    if( m_pTmrBtnsStateRefresh != nullptr && m_pTmrBtnsStateRefresh->isActive() )
    {
        m_pTmrBtnsStateRefresh->stop();
    }

    QDialog::closeEvent(i_pEv);

} // closeEvent

//------------------------------------------------------------------------------
void CDlgPageSetup::showEvent( QShowEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "showEvent",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pTmrBtnsStateRefresh != nullptr && !m_pTmrBtnsStateRefresh->isActive() )
    {
        m_pTmrBtnsStateRefresh->start(250);
    }

    QDialog::showEvent(i_pEv);

} // showEvent

/*==============================================================================
protected slots: // overridables of base class QDialog
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgPageSetup::accept()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "accept",
        /* strAddInfo   */ strAddTrcInfo );

    onBtnAcceptClicked();

    QDialog::accept();

} // accept

//------------------------------------------------------------------------------
void CDlgPageSetup::reject()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "reject",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups();

    QDialog::reject();

} // reject

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgPageSetup::onBtnAcceptClicked()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnAcceptClicked",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups();

    if( m_pDrawingView != nullptr )
    {
        m_pDrawingView->setPageSetup(m_pageSetupNew);
    }
    m_pageSetupOld = m_pageSetupNew;

} // onBtnAcceptClicked

//------------------------------------------------------------------------------
void CDlgPageSetup::onBtnResetClicked()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnResetClicked",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups();

    m_pageSetupNew = m_pageSetupOld;

    CWdgtPageSetup* pWdgtPageSetup;
    int             idxWdgt;

    for( idxWdgt = 0; idxWdgt < EWidgetCount; idxWdgt++ )
    {
        pWdgtPageSetup = m_arpWdgtsPageSetup[idxWdgt];

        if( pWdgtPageSetup != nullptr )
        {
            pWdgtPageSetup->setSettings(m_pageSetupNew);
        }
    }

} // onBtnResetClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgPageSetup::onTmrBtnsStateRefreshTimeout()
//------------------------------------------------------------------------------
{
    if( m_pageSetupNew == m_pageSetupOld )
    {
        m_pBtnAccept->setEnabled(false);
        m_pBtnReset->setEnabled(false);
    }
    else
    {
        m_pBtnAccept->setEnabled(true);
        m_pBtnReset->setEnabled(true);
    }

} // onTmrBtnsStateRefreshTimeout

/*==============================================================================
protected slots: // instance methods (List Widget)
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgPageSetup::onListWdgtCurrentRowChanged( int i_iRow )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onListWdgtCurrentRowChanged",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups();

    m_pStackedWdgt->setCurrentIndex(i_iRow);

} // onListWdgtCurrentRowChanged

/*==============================================================================
protected slots: // Format Widgets
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgPageSetup::onPageSetupAttributeChanged( int i_iAttr, const QVariant& i_val )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onPageSetupAttributeChanged",
        /* strAddInfo   */ strAddTrcInfo );

    m_pageSetupNew.setAttribute( i_iAttr, i_val );

} // onPageSetupAttributeChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgPageSetup::hidePopups()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "hidePopups",
        /* strAddInfo   */ strAddTrcInfo );

    CWdgtPageSetup* pWdgtPageSetup;
    int             idxWdgt;

    for( idxWdgt = 0; idxWdgt < EWidgetCount; idxWdgt++ )
    {
        pWdgtPageSetup = m_arpWdgtsPageSetup[idxWdgt];

        if( pWdgtPageSetup != nullptr )
        {
            pWdgtPageSetup->hidePopups();
        }
    }

} // hidePopups

/*==============================================================================
private: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSize* CDlgPageSetup::getDrawingScenePhysSize()
//------------------------------------------------------------------------------
{
    CPhysSize* pPhysSize = nullptr;

    //if( m_pCmbDrawingPhysSize->currentText() == Geometry::GraphDevice().getSIUnit()->getName() )
    //{
    //    pPhysSize = &Geometry::GraphDevice();
    //}
    //else if( m_pCmbDrawingPhysSize->currentText() == Geometry::Length().getSIUnit()->getName() )
    //{
        pPhysSize = Geometry::Length();
    //}
    return pPhysSize;

} // getDrawingScenePhysSize

//------------------------------------------------------------------------------
CPhysUnit* CDlgPageSetup::getDrawingScenePhysUnit()
//------------------------------------------------------------------------------
{
    CPhysSize* pPhysSize = getDrawingScenePhysSize();
    CPhysUnit* pPhysUnit = nullptr;

    if( pPhysSize == Geometry::GraphDevice() )
    {
        pPhysUnit = Geometry::GraphDevice()->Pixel();
    }
    else if( pPhysSize == Geometry::Length() )
    {
        pPhysUnit = Geometry::Length()->MilliMeter();
    }
    return pPhysUnit;

} // getDrawingScenePhysUnit

//------------------------------------------------------------------------------
void CDlgPageSetup::updateDrawingSize()
//------------------------------------------------------------------------------
{
    //double fPaperWidth_mm         = m_pEdtPaperWidth->text().toDouble();
    //double fPaperHeight_mm        = m_pEdtPaperHeight->text().toDouble();
    //double fPaperMarginLeft_mm    = m_pEdtPaperMarginLeft->text().toDouble();
    //double fPaperMarginRight_mm   = m_pEdtPaperMarginRight->text().toDouble();
    //double fPaperMarginTop_mm     = m_pEdtPaperMarginTop->text().toDouble();
    //double fPaperMarginBottom_mm  = m_pEdtPaperMarginBottom->text().toDouble();
    //double fDrawingWidth_mm       = fPaperWidth_mm - fPaperMarginLeft_mm - fPaperMarginRight_mm;
    //double fDrawingHeight_mm      = fPaperHeight_mm - fPaperMarginTop_mm - fPaperMarginBottom_mm;
    //double fDrawingXScale         = m_pEdtDrawingXScale->text().toDouble();
    //double fDrawingYScale         = m_pEdtDrawingYScale->text().toDouble();

    //CPhysVal physValDrawingWidth  = CPhysVal( fDrawingWidth_mm / fDrawingXScale, Geometry::Length().MilliMeter() );
    //CPhysVal physValDrawingHeight = CPhysVal( fDrawingHeight_mm / fDrawingYScale, Geometry::Length().MilliMeter() );

    //if( *getDrawingScenePhysSize() == Geometry::GraphDevice() )
    //{
    //    //double fDrawingWidth_pix  = m_pDrawingView->getDrawingScene()->getXInPixels(physValDrawingWidth);
    //    //double fDrawingHeight_pix = m_pDrawingView->getDrawingScene()->getYInPixels(physValDrawingHeight);

    //    //physValDrawingWidth  = CPhysVal( fDrawingWidth_pix, Geometry::GraphDevice().Pixel() );
    //    //physValDrawingHeight = CPhysVal( fDrawingHeight_pix, Geometry::GraphDevice().Pixel() );
    //}

    //QString strUnit, strLbl;

    //strUnit = physValDrawingWidth.getPhysUnit()->getSymbol();
    //strLbl  = "Width [" + strUnit + "]";
    //m_pLblDrawingSizeWidth->setText(strLbl);
    //m_pEdtDrawingSizeWidth->setText(QString::number(physValDrawingWidth.getVal()));

    //strUnit = physValDrawingHeight.getPhysUnit()->getSymbol();
    //strLbl  = "Height [" + strUnit + "]";
    //m_pLblDrawingSizeHeight->setText(strLbl);
    //m_pEdtDrawingSizeHeight->setText(QString::number(physValDrawingHeight.getVal()));

} // updateDrawingSize
