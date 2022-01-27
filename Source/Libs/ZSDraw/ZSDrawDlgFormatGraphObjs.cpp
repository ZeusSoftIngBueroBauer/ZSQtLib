/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSDraw/ZSDrawDlgFormatGraphObjs.h"
#include "ZSDraw/ZSDrawWdgtFormatGraphObjsObjName.h"
#include "ZSDraw/ZSDrawWdgtFormatGraphObjsFillStyle.h"
#include "ZSDraw/ZSDrawWdgtFormatGraphObjsGeometry.h"
#include "ZSDraw/ZSDrawWdgtFormatGraphObjsLabels.h"
#include "ZSDraw/ZSDrawWdgtFormatGraphObjsLineStyle.h"
#include "ZSDraw/ZSDrawWdgtFormatGraphObjsText.h"
#include "ZSDraw/ZSDrawWdgtFormatGraphObjsTextStyle.h"
#include "ZSDraw/ZSDrawGraphObj.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;
using namespace ZS::Trace;


/*******************************************************************************
class CDlgFormatGraphObjs : public QDialog
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QString CDlgFormatGraphObjs::c_strWdgtObjName   = "Object Name";
const QString CDlgFormatGraphObjs::c_strWdgtLineStyle = "Line Style";
const QString CDlgFormatGraphObjs::c_strWdgtFillStyle = "Fill Style";
const QString CDlgFormatGraphObjs::c_strWdgtTextStyle = "Text Style";
const QString CDlgFormatGraphObjs::c_strWdgtGeometry  = "Geometry";
const QString CDlgFormatGraphObjs::c_strWdgtText      = "Text";
const QString CDlgFormatGraphObjs::c_strWdgtLabels    = "Labels";

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgFormatGraphObjs::CDlgFormatGraphObjs(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObj,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QDialog(i_pWdgtParent),
    m_pDrawingScene(i_pDrawingScene),
    m_pGraphObj(i_pGraphObj),
    m_pLyt(nullptr),
    m_pListWdgt(nullptr),
    m_pStackedWdgt(nullptr),
    // Format Widgets
    m_arpListWdgtItems(),
    m_arpWdgtsFormatGraphObjs(),
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
    if( m_pDrawingScene == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pDrawingScene == nullptr" );
    }

    setWindowTitle();

    setObjectName("DlgFormatGraphObjs");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    if( i_pGraphObj == nullptr )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pGraphObj == nullptr");
    }

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    QHBoxLayout* m_pLytSettings = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSettings);

    // List Widget (on the left) and Stacked Widget (on the right)
    //============================================================

    CDrawSettings drawSettings = m_pGraphObj->getDrawSettings();

    m_pListWdgt = new QListWidget();
    m_pListWdgt->setMaximumWidth(120);
    m_pLytSettings->addWidget(m_pListWdgt);

    m_pStackedWdgt = new QStackedWidget();
    m_pLytSettings->addWidget(m_pStackedWdgt,1);

    QIcon   iconObjName;
    QPixmap pxmObjName(":/ZS/Draw/Key16x16.bmp");
    pxmObjName.setMask(pxmObjName.createHeuristicMask());
    iconObjName.addPixmap(pxmObjName);

    QListWidgetItem* pListWdgtItemObjName = new QListWidgetItem( iconObjName, c_strWdgtObjName, m_pListWdgt );
    m_arpListWdgtItems.insert(c_strWdgtObjName,pListWdgtItemObjName);

    CWdgtFormatGraphObjsObjName* pWdgtObjName = new CWdgtFormatGraphObjsObjName(m_pDrawingScene,m_pGraphObj);
    m_arpWdgtsFormatGraphObjs.insert(c_strWdgtObjName,pWdgtObjName);
    m_pStackedWdgt->addWidget(pWdgtObjName);

    if( drawSettings.isPenUsed() )
    {
        QIcon   iconLine;
        QPixmap pxmLine(":/ZS/Draw/DrawToolPen16x16.bmp");
        pxmLine.setMask(pxmLine.createHeuristicMask());
        iconLine.addPixmap(pxmLine);

        QListWidgetItem* pListWdgtItemLineStyle = nullptr;

        if( m_pGraphObj != nullptr && m_pGraphObj->getType() == EGraphObjTypePoint )
        {
            pListWdgtItemLineStyle = new QListWidgetItem( iconLine, tr("Pen Style"), m_pListWdgt );
        }
        else
        {
            pListWdgtItemLineStyle = new QListWidgetItem( iconLine, c_strWdgtLineStyle, m_pListWdgt );
        }
        m_arpListWdgtItems.insert(c_strWdgtLineStyle,pListWdgtItemLineStyle);

        CWdgtFormatGraphObjsLineStyle* pWdgtLineStyle = new CWdgtFormatGraphObjsLineStyle(m_pDrawingScene,m_pGraphObj);
        m_arpWdgtsFormatGraphObjs.insert(c_strWdgtLineStyle,pWdgtLineStyle);
        m_pStackedWdgt->addWidget(pWdgtLineStyle);
    }

    if( drawSettings.isFillUsed() )
    {
        QIcon   iconFill;
        QPixmap pxmFill(":/ZS/Draw/DrawToolBrush16x16.bmp");
        pxmFill.setMask(pxmFill.createHeuristicMask());
        iconFill.addPixmap(pxmFill);

        QListWidgetItem* pListWdgtItemFillStyle = new QListWidgetItem( iconFill, c_strWdgtFillStyle, m_pListWdgt );
        m_arpListWdgtItems.insert(c_strWdgtFillStyle,pListWdgtItemFillStyle);

        CWdgtFormatGraphObjsFillStyle* pWdgtFillStyle = new CWdgtFormatGraphObjsFillStyle(m_pDrawingScene,m_pGraphObj);
        m_arpWdgtsFormatGraphObjs.insert(c_strWdgtFillStyle,pWdgtFillStyle);
        m_pStackedWdgt->addWidget(pWdgtFillStyle);
    }

    if( drawSettings.isTextUsed() )
    {
        QIcon   iconText;
        QPixmap pxmText(":/ZS/Draw/DrawToolText16x16.bmp");
        pxmText.setMask(pxmText.createHeuristicMask());
        iconText.addPixmap(pxmText);

        QListWidgetItem* pListWdgtItemTextStyle = new QListWidgetItem( iconText, c_strWdgtTextStyle, m_pListWdgt );
        m_arpListWdgtItems.insert(c_strWdgtTextStyle,pListWdgtItemTextStyle);

        CWdgtFormatGraphObjsTextStyle* pWdgtTextStyle = new CWdgtFormatGraphObjsTextStyle(m_pDrawingScene, m_pGraphObj);
        m_arpWdgtsFormatGraphObjs.insert(c_strWdgtTextStyle,pWdgtTextStyle);
        m_pStackedWdgt->addWidget(pWdgtTextStyle);
    }

    QIcon   iconGeometry;
    QPixmap pxmGeometry(":/ZS/Draw/DrawToolGeometry16x16.bmp");
    pxmGeometry.setMask(pxmGeometry.createHeuristicMask());
    iconGeometry.addPixmap(pxmGeometry);

    QListWidgetItem* pListWdgtItemGeometry = new QListWidgetItem( iconGeometry, c_strWdgtGeometry, m_pListWdgt );
    m_arpListWdgtItems.insert(c_strWdgtGeometry,pListWdgtItemGeometry);

    CWdgtFormatGraphObjsGeometry* pWdgtGeometry = new CWdgtFormatGraphObjsGeometry(m_pDrawingScene, m_pGraphObj);
    m_arpWdgtsFormatGraphObjs.insert(c_strWdgtGeometry,pWdgtGeometry);
    m_pStackedWdgt->addWidget(pWdgtGeometry);

    if( m_pGraphObj->getType() == EGraphObjTypeText )
    {
        QIcon   iconText;
        QPixmap pxmText(":/ZS/Draw/DrawText16x16.bmp");
        pxmText.setMask(pxmText.createHeuristicMask());
        iconText.addPixmap(pxmText);

        QListWidgetItem* pListWdgtItemText = new QListWidgetItem( iconText, c_strWdgtText, m_pListWdgt );
        m_arpListWdgtItems.insert(c_strWdgtText,pListWdgtItemText);

        CWdgtFormatGraphObjsText* pWdgtText = new CWdgtFormatGraphObjsText(m_pDrawingScene, m_pGraphObj);
        m_arpWdgtsFormatGraphObjs.insert(c_strWdgtText,pWdgtText);
        m_pStackedWdgt->addWidget(pWdgtText);
    }

    QIcon   iconLabel;
    QPixmap pxmLabel(":/ZS/Draw/DrawText16x16.bmp");
    pxmLabel.setMask(pxmLabel.createHeuristicMask());
    iconLabel.addPixmap(pxmLabel);

    QListWidgetItem* pListWdgtItemLabels = new QListWidgetItem(iconLabel, c_strWdgtLabels, m_pListWdgt);
    m_arpListWdgtItems.insert(c_strWdgtLabels, pListWdgtItemLabels);

    CWdgtFormatGraphObjsLabels* pWdgtLabels = new CWdgtFormatGraphObjsLabels(m_pDrawingScene, m_pGraphObj);
    m_arpWdgtsFormatGraphObjs.insert(c_strWdgtText, pWdgtLabels);
    m_pStackedWdgt->addWidget(pWdgtLabels);

    if( !connect(
        /* pObjSender   */ m_pListWdgt,
        /* szSignal     */ SIGNAL(currentRowChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onListWdgtCurrentRowChanged(int)) ) )
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
CDlgFormatGraphObjs::~CDlgFormatGraphObjs()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pDrawingScene = nullptr;
    m_pLyt = nullptr;
    m_pLytSettings = nullptr;
    m_pListWdgt = nullptr;
    m_pStackedWdgt = nullptr;
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
void CDlgFormatGraphObjs::addWidget(
    const QIcon&          i_icon,
    const QString&        i_strWdgtName,
    CWdgtFormatGraphObjs* i_pWdgt )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_strWdgtName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addWidget",
        /* strAddInfo   */ strAddTrcInfo );

    QListWidgetItem* pListWdgtItem = new QListWidgetItem(i_icon, i_strWdgtName, m_pListWdgt);
    m_arpListWdgtItems.insert(i_strWdgtName, pListWdgtItem);

    m_arpWdgtsFormatGraphObjs.insert(i_strWdgtName, i_pWdgt);
    m_pStackedWdgt->addWidget(i_pWdgt);

} // addWidget

//------------------------------------------------------------------------------
void CDlgFormatGraphObjs::setCurrentWidget( const QString& i_strWdgtName )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_strWdgtName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCurrentWidget",
        /* strAddInfo   */ strAddTrcInfo );

    QListWidgetItem* pListWdgtItem;
    int              idxTmp;

    for( idxTmp = 0; idxTmp < m_pListWdgt->count(); idxTmp++ )
    {
        pListWdgtItem = m_pListWdgt->item(idxTmp);

        if( pListWdgtItem->text() == i_strWdgtName )
        {
            m_pListWdgt->setCurrentRow(idxTmp);
            break;
        }
    }

} // setCurrentWidget

/*==============================================================================
protected slots: // overridables of base class QDialog
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgFormatGraphObjs::closeEvent( QCloseEvent* i_pEv )
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
void CDlgFormatGraphObjs::showEvent( QShowEvent* i_pEv )
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

    if( m_pGraphObj != nullptr )
    {
    }

    if( m_pTmrBtnsStateRefresh != nullptr && !m_pTmrBtnsStateRefresh->isActive() )
    {
        m_pTmrBtnsStateRefresh->start(250);
    }

    setWindowTitle();

    QDialog::showEvent(i_pEv);

} // showEvent

/*==============================================================================
protected slots: // overridables of base class QDialog
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgFormatGraphObjs::accept()
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
void CDlgFormatGraphObjs::reject()
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
void CDlgFormatGraphObjs::onBtnAcceptClicked()
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

    QHashIterator<QString,CWdgtFormatGraphObjs*> it(m_arpWdgtsFormatGraphObjs);
    CWdgtFormatGraphObjs* pWdgtFormatGraphObjs;

    while( it.hasNext() )
    {
        it.next();
        pWdgtFormatGraphObjs = it.value();
        pWdgtFormatGraphObjs->applyChanges();
    }

} // onBtnAcceptClicked

//------------------------------------------------------------------------------
void CDlgFormatGraphObjs::onBtnResetClicked()
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

    QHashIterator<QString,CWdgtFormatGraphObjs*> it(m_arpWdgtsFormatGraphObjs);
    CWdgtFormatGraphObjs* pWdgtFormatGraphObjs;

    while( it.hasNext() )
    {
        it.next();
        pWdgtFormatGraphObjs = it.value();
        pWdgtFormatGraphObjs->resetChanges();
    }

} // onBtnResetClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgFormatGraphObjs::onTmrBtnsStateRefreshTimeout()
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    QHashIterator<QString,CWdgtFormatGraphObjs*> it(m_arpWdgtsFormatGraphObjs);
    CWdgtFormatGraphObjs* pWdgtFormatGraphObjs;

    while( it.hasNext() )
    {
        it.next();

        pWdgtFormatGraphObjs = it.value();

        bHasChanges = pWdgtFormatGraphObjs->hasChanges();

        if( bHasChanges )
        {
            break;
        }
    }

    if( bHasChanges )
    {
        m_pBtnAccept->setEnabled(true);
        m_pBtnReset->setEnabled(true);
    }
    else
    {
        if( m_pGraphObj != nullptr )
        {
            m_pBtnAccept->setEnabled(false);
        }
        else if( m_pDrawingScene->selectedItems().size() > 0 )
        {
            m_pBtnAccept->setEnabled(true);
        }
        else
        {
            m_pBtnAccept->setEnabled(false);
        }
        m_pBtnReset->setEnabled(false);

    } // if( !bHasChanges )

} // onTmrBtnsStateRefreshTimeout

/*==============================================================================
protected slots: // instance methods (List Widget)
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgFormatGraphObjs::onListWdgtCurrentRowChanged( int i_iRow )
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
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgFormatGraphObjs::hidePopups()
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

    QHashIterator<QString,CWdgtFormatGraphObjs*> it(m_arpWdgtsFormatGraphObjs);
    CWdgtFormatGraphObjs* pWdgtFormatGraphObjs;

    while( it.hasNext() )
    {
        it.next();
        pWdgtFormatGraphObjs = it.value();
        pWdgtFormatGraphObjs->hidePopups();
    }

} // hidePopups

//------------------------------------------------------------------------------
void CDlgFormatGraphObjs::setWindowTitle()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setWindowTitle",
        /* strAddInfo   */ strAddTrcInfo );

    // If one specific graphical object is selected ...
    if( m_pGraphObj != nullptr )
    {
        QDialog::setWindowTitle( QApplication::applicationName() + ": Formatting Graphical Object " + m_pGraphObj->name() );
    }
    else
    {
        // If no graphical object is selected ...
        if( m_pDrawingScene->selectedItems().size() == 0 )
        {
            // .. set default attribute for following draw commands.
            QDialog::setWindowTitle( QApplication::applicationName() + ": Set Draw Attributes" );
        }
        // If one specific graphical object is selected ...
        else if( m_pDrawingScene->selectedItems().size() == 1 )
        {
            // .. set drawing attributes for this object.
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(m_pDrawingScene->selectedItems()[0]);
            if( pGraphObj != nullptr )
            {
                QDialog::setWindowTitle( QApplication::applicationName() + ": Formatting Graphical Object " + pGraphObj->name() );
            }
            else
            {
                QDialog::setWindowTitle( QApplication::applicationName() + ": Set Draw Attributes" );
            }
        }
        // If several graphical objects are selected ...
        else // if( arpGraphicsItemsSelected.size() > 1 )
        {
            // .. set drawing attributes for those objects.
            QDialog::setWindowTitle( QApplication::applicationName() + ": Formatting Graphical Objects" );
        }
    }

} // setWindowTitle
