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

#if QT_VERSION < 0x050000
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#else
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#endif

#include "ZSDraw/ZSDrawWdgtDrawingScenePageSetup.h"
#include "ZSDraw/ZSDrawingView.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CWdgtPageSetupDrawingScene : public CWdgtPageSetup
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtPageSetupDrawingScene::CWdgtPageSetupDrawingScene(
    CDrawingView* i_pDrawingView,
    QWidget*      i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtPageSetup(i_pDrawingView,i_pWdgtParent),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    m_pLytResolutionDpi(nullptr),
    m_pLblResolutionDpi(nullptr),
    m_pLblResolutionDpiVal(nullptr),
    m_pLblResolutionDpiUnit(nullptr),
    m_pLytResolutionDpmm(nullptr),
    m_pLblResolutionDpmm(nullptr),
    m_pLblResolutionDpmmVal(nullptr),
    m_pLblResolutionDpmmUnit(nullptr),
    m_pLytSepLineSize(nullptr),
    m_pLblSepLineSize(nullptr),
    m_pSepLineSize(nullptr),
    m_pLytSizeWidth(nullptr),
    m_pLblSizeWidth(nullptr),
    m_pEdtSizeWidth(nullptr),
    m_pLytSizeHeight(nullptr),
    m_pLblSizeHeight(nullptr),
    m_pEdtSizeHeight(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtPageSetupDrawingScene");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Draw", "CWdgtPageSetupDrawingScene", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    int cxLblWidth = 60;
    //int cxEdtWidth = 80;

    QFont fntHeadLine;

    m_pLblHeadLine = new QLabel("Drawing Scene");
    fntHeadLine = m_pLblHeadLine->font();
    fntHeadLine.setPointSize(fntHeadLine.pointSize()+4);
    m_pLblHeadLine->setFont(fntHeadLine);
    m_pLyt->addWidget(m_pLblHeadLine);
    m_pLyt->addSpacing(4);

    // <Section> Resolution
    //---------------------

    double fXResolution_dpmm = Units.GraphDevice.dpmm(EDirection::Horizontal);
    double fYResolution_dpmm = Units.GraphDevice.dpmm(EDirection::Vertical);
    double fXResolution_dpi = fXResolution_dpmm*25.4;
    double fYResolution_dpi = fYResolution_dpmm*25.4;

    QString strXDpi  = QString::number(fXResolution_dpi,'f',2);
    QString strYDpi  = QString::number(fYResolution_dpi,'f',2);
    QString strXDpmm = QString::number(fXResolution_dpmm,'f',2);
    QString strYDpmm = QString::number(fYResolution_dpmm,'f',2);
    QString strResolution;

    // <Label> Resolution - dots per inch
    //-----------------------------------

    strResolution = strXDpi + " * " + strYDpi;
    m_pLytResolutionDpi = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytResolutionDpi);
    m_pLblResolutionDpi     = new QLabel("Resolution");
    m_pLblResolutionDpiVal  = new QLabel(strResolution);
    m_pLblResolutionDpiUnit = new QLabel("dots per inch");
    m_pLytResolutionDpi->addWidget(m_pLblResolutionDpi);
    m_pLytResolutionDpi->addWidget(m_pLblResolutionDpiVal);
    m_pLytResolutionDpi->addWidget(m_pLblResolutionDpiUnit);
    m_pLytResolutionDpi->addStretch();

    // <Label> Resolution - dots per millimeter
    //-----------------------------------------

    strResolution = strXDpmm + " * " + strYDpmm;
    m_pLytResolutionDpmm = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytResolutionDpmm);
    m_pLblResolutionDpmm     = new QLabel("Resolution");
    m_pLblResolutionDpmmVal  = new QLabel(strResolution);
    m_pLblResolutionDpmmUnit = new QLabel("dots per milli meter");
    m_pLytResolutionDpmm->addWidget(m_pLblResolutionDpmm);
    m_pLytResolutionDpmm->addWidget(m_pLblResolutionDpmmVal);
    m_pLytResolutionDpmm->addWidget(m_pLblResolutionDpmmUnit);
    m_pLytResolutionDpmm->addStretch();

    // <Separator> Resolution
    //-----------------------

    m_pLyt->addSpacing(4);

    m_pLytSepLineSize = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSepLineSize);

    m_pLblSepLineSize = new QLabel("Size:");
    m_pLytSepLineSize->addWidget(m_pLblSepLineSize);
    m_pSepLineSize = new QFrame();
    m_pSepLineSize->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    m_pLytSepLineSize->addWidget(m_pSepLineSize,1);

    m_pLyt->addSpacing(4);

    // <LineEdit> Size - Width
    //------------------------

    m_pLytSizeWidth = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSizeWidth);
    m_pLblSizeWidth = new QLabel("Width/px:");
    m_pLblSizeWidth->setFixedWidth(cxLblWidth);
    m_pLytSizeWidth->addWidget(m_pLblSizeWidth);
    m_pEdtSizeWidth = new QLineEdit();
    m_pLytSizeWidth->addWidget(m_pEdtSizeWidth);
    m_pLytSizeWidth->addStretch();

    if( !connect(
        /* pObjSender   */ m_pEdtSizeWidth,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtSizeWidthTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <LineEdit> Size - Height
    //-------------------------

    m_pLytSizeHeight = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSizeHeight);
    m_pLblSizeHeight = new QLabel("Height/px:");
    m_pLblSizeHeight->setFixedWidth(cxLblWidth);
    m_pLytSizeHeight->addWidget(m_pLblSizeHeight);
    m_pEdtSizeHeight = new QLineEdit();
    m_pLytSizeHeight->addWidget(m_pEdtSizeHeight);
    m_pLytSizeHeight->addStretch();

    if( !connect(
        /* pObjSender   */ m_pEdtSizeHeight,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtSizeHeightTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Stretch> at bottom of widget
    //------------------------------

    m_pLyt->addStretch();

    // Set settings at GUI controls
    //-----------------------------

    setSettings(m_pageSetup);

} // ctor

//------------------------------------------------------------------------------
CWdgtPageSetupDrawingScene::~CWdgtPageSetupDrawingScene()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pLyt = nullptr;
    m_pLblHeadLine = nullptr;
    m_pLytResolutionDpi = nullptr;
    m_pLblResolutionDpi = nullptr;
    m_pLblResolutionDpiVal = nullptr;
    m_pLblResolutionDpiUnit = nullptr;
    m_pLytResolutionDpmm = nullptr;
    m_pLblResolutionDpmm = nullptr;
    m_pLblResolutionDpmmVal = nullptr;
    m_pLblResolutionDpmmUnit = nullptr;
    m_pLytSepLineSize = nullptr;
    m_pLblSepLineSize = nullptr;
    m_pSepLineSize = nullptr;
    m_pLytSizeWidth = nullptr;
    m_pLblSizeWidth = nullptr;
    m_pEdtSizeWidth = nullptr;
    m_pLytSizeHeight = nullptr;
    m_pLblSizeHeight = nullptr;
    m_pEdtSizeHeight = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPageSetupDrawingScene::setSettings( const CPageSetup& i_pageSetup )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setSettings",
        /* strAddInfo   */ strAddTrcInfo );

    m_pageSetup = i_pageSetup;

    // <EditField> Size - Width
    //-------------------------

    m_pEdtSizeWidth->setText(
        QString::number(m_pageSetup.drawingWidth().getVal(Units.GraphDevice.px)) );

    // <EditField> Size - Height
    //-------------------------

    m_pEdtSizeHeight->setText(
        QString::number(m_pageSetup.drawingHeight().getVal(Units.GraphDevice.px)) );

    //emit pageSetupAttributeChanged(
    //    EPageSetupAttributeDrawingWidthInPixels,
    //    static_cast<int>(m_pageSetup.drawingWidth().getVal(Units.GraphDevice.px)) );
    //emit pageSetupAttributeChanged(
    //    EPageSetupAttributeDrawingHeightInPixels,
    //    static_cast<int>(m_pageSetup.drawingHeight().getVal(Units.GraphDevice.px)) );

} // setSettings

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPageSetupDrawingScene::onEdtSizeWidthTextChanged( const QString& i_strText )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtSizeWidthTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups();

    m_pageSetup.setDrawingWidth(i_strText.toInt());

    //emit pageSetupAttributeChanged(
    //    EPageSetupAttributeDrawingWidthInPixels,
    //    static_cast<int>(m_pageSetup.drawingWidth().getVal(Units.GraphDevice.px)) );

} // onEdtSizeWidthTextChanged

//------------------------------------------------------------------------------
void CWdgtPageSetupDrawingScene::onEdtSizeHeightTextChanged( const QString& i_strText )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtSizeHeightTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups();

    m_pageSetup.setDrawingHeight(i_strText.toInt());

    //emit pageSetupAttributeChanged(
    //    EPageSetupAttributeDrawingHeightInPixels,
    //    static_cast<int>(m_pageSetup.drawingHeight().getVal(Units.GraphDevice.px)) );

} // onEdtSizeHeightTextChanged
