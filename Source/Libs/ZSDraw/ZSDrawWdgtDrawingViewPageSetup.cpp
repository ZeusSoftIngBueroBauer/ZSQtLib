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

#if QT_VERSION < 0x050000
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qspinbox.h>
#else
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qspinbox.h>
#endif

#include "ZSDraw/ZSDrawWdgtDrawingViewPageSetup.h"
#include "ZSDraw/ZSDrawingView.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
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
class CWdgtDrawingViewPageSetup : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDrawingViewPageSetup::CWdgtDrawingViewPageSetup(
    CDrawingView* i_pDrawingView,
    QWidget*      i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pDrawingView(i_pDrawingView),
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
    m_pLytSepLineMargins(nullptr),
    m_pLblSepLineMargins(nullptr),
    m_pSepLineMargins(nullptr),
    m_pLytMarginsLeft(nullptr),
    m_pLblMarginsLeft(nullptr),
    m_pEdtMarginsLeft(nullptr),
    m_pLytMarginsTop(nullptr),
    m_pLblMarginsTop(nullptr),
    m_pEdtMarginsTop(nullptr),
    m_pLytMarginsRight(nullptr),
    m_pLblMarginsRight(nullptr),
    m_pEdtMarginsRight(nullptr),
    m_pLytMarginsBottom(nullptr),
    m_pLblMarginsBottom(nullptr),
    m_pEdtMarginsBottom(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtPageSetupDrawingScene");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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

    double  fXResolution_dpi = i_pDrawingView->getXResolutionInDpmm()*25.4;
    double  fYResolution_dpi = i_pDrawingView->getYResolutionInDpmm()*25.4;

    QString strXDpi  = QString::number(fXResolution_dpi,'f',2);
    QString strYDpi  = QString::number(fYResolution_dpi,'f',2);
    QString strXDpmm = QString::number(i_pDrawingView->getXResolutionInDpmm(),'f',2);
    QString strYDpmm = QString::number(i_pDrawingView->getYResolutionInDpmm(),'f',2);
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

    // <Separator> Size
    //-----------------

    int cxEdtWidth = 100;

    QSize sizeDrawing = m_pDrawingView->getDrawingSizeInPixels();

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
    m_pLblSizeWidth = new QLabel("Width:");
    m_pLblSizeWidth->setFixedWidth(cxLblWidth);
    m_pLytSizeWidth->addWidget(m_pLblSizeWidth);
    m_pEdtSizeWidth = new QSpinBox();
    m_pEdtSizeWidth->setFixedWidth(cxEdtWidth);
    m_pEdtSizeWidth->setMinimum(1);
    m_pEdtSizeWidth->setMaximum(100000);
    m_pEdtSizeWidth->setSuffix(" px");
    m_pEdtSizeWidth->setValue(sizeDrawing.width());
    m_pLytSizeWidth->addWidget(m_pEdtSizeWidth);
    m_pLytSizeWidth->addStretch();

    if( !connect(
        /* pObjSender   */ m_pEdtSizeWidth,
        /* szSignal     */ SIGNAL(editingFinished()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtSizeWidthEditingFinished()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <LineEdit> Size - Height
    //-------------------------

    m_pLytSizeHeight = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSizeHeight);
    m_pLblSizeHeight = new QLabel("Height:");
    m_pLblSizeHeight->setFixedWidth(cxLblWidth);
    m_pLytSizeHeight->addWidget(m_pLblSizeHeight);
    m_pEdtSizeHeight = new QSpinBox();
    m_pEdtSizeHeight->setFixedWidth(cxEdtWidth);
    m_pEdtSizeHeight->setMinimum(1);
    m_pEdtSizeHeight->setMaximum(100000);
    m_pEdtSizeHeight->setSuffix(" px");
    m_pEdtSizeHeight->setValue(sizeDrawing.height());
    m_pLytSizeHeight->addWidget(m_pEdtSizeHeight);
    m_pLytSizeHeight->addStretch();

    if( !connect(
        /* pObjSender   */ m_pEdtSizeHeight,
        /* szSignal     */ SIGNAL(editingFinished()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtSizeHeightEditingFinished()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Separator> Margins
    //--------------------

    QMargins margins = m_pDrawingView->getViewportMargins();

    m_pLyt->addSpacing(4);

    m_pLytSepLineMargins = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSepLineMargins);

    m_pLblSepLineMargins = new QLabel("Margins:");
    m_pLytSepLineMargins->addWidget(m_pLblSepLineMargins);
    m_pSepLineMargins = new QFrame();
    m_pSepLineMargins->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    m_pLytSepLineMargins->addWidget(m_pSepLineMargins,1);

    m_pLyt->addSpacing(4);

    // <LineEdit> Margins - Left
    //--------------------------

    m_pLytMarginsLeft = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytMarginsLeft);
    m_pLblMarginsLeft = new QLabel("Left:");
    m_pLblMarginsLeft->setFixedWidth(cxLblWidth);
    m_pLytMarginsLeft->addWidget(m_pLblMarginsLeft);
    m_pEdtMarginsLeft = new QSpinBox();
    m_pEdtMarginsLeft->setFixedWidth(cxEdtWidth);
    m_pEdtMarginsLeft->setMinimum(0);
    m_pEdtMarginsLeft->setMaximum(100);
    m_pEdtMarginsLeft->setSuffix(" px");
    m_pEdtMarginsLeft->setValue(margins.left());
    m_pLytMarginsLeft->addWidget(m_pEdtMarginsLeft);
    m_pLytMarginsLeft->addStretch();

    if( !connect(
        /* pObjSender   */ m_pEdtMarginsLeft,
        /* szSignal     */ SIGNAL(editingFinished()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtMarginsLeftEditingFinished()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <LineEdit> Margins - Top
    //--------------------------

    m_pLytMarginsTop = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytMarginsTop);
    m_pLblMarginsTop = new QLabel("Top:");
    m_pLblMarginsTop->setFixedWidth(cxLblWidth);
    m_pLytMarginsTop->addWidget(m_pLblMarginsTop);
    m_pEdtMarginsTop = new QSpinBox();
    m_pEdtMarginsTop->setFixedWidth(cxEdtWidth);
    m_pEdtMarginsTop->setMinimum(0);
    m_pEdtMarginsTop->setMaximum(100);
    m_pEdtMarginsTop->setSuffix(" px");
    m_pEdtMarginsTop->setValue(margins.left());
    m_pLytMarginsTop->addWidget(m_pEdtMarginsTop);
    m_pLytMarginsTop->addStretch();

    if( !connect(
        /* pObjSender   */ m_pEdtMarginsTop,
        /* szSignal     */ SIGNAL(editingFinished()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtMarginsTopEditingFinished()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <LineEdit> Margins - Right
    //--------------------------

    m_pLytMarginsRight = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytMarginsRight);
    m_pLblMarginsRight = new QLabel("Right:");
    m_pLblMarginsRight->setFixedWidth(cxLblWidth);
    m_pLytMarginsRight->addWidget(m_pLblMarginsRight);
    m_pEdtMarginsRight = new QSpinBox();
    m_pEdtMarginsRight->setFixedWidth(cxEdtWidth);
    m_pEdtMarginsRight->setMinimum(0);
    m_pEdtMarginsRight->setMaximum(100);
    m_pEdtMarginsRight->setSuffix(" px");
    m_pEdtMarginsRight->setValue(margins.left());
    m_pLytMarginsRight->addWidget(m_pEdtMarginsRight);
    m_pLytMarginsRight->addStretch();

    if( !connect(
        /* pObjSender   */ m_pEdtMarginsRight,
        /* szSignal     */ SIGNAL(editingFinished()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtMarginsRightEditingFinished()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <LineEdit> Margins - Bottom
    //--------------------------

    m_pLytMarginsBottom = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytMarginsBottom);
    m_pLblMarginsBottom = new QLabel("Bottom:");
    m_pLblMarginsBottom->setFixedWidth(cxLblWidth);
    m_pLytMarginsBottom->addWidget(m_pLblMarginsBottom);
    m_pEdtMarginsBottom = new QSpinBox();
    m_pEdtMarginsBottom->setFixedWidth(cxEdtWidth);
    m_pEdtMarginsBottom->setMinimum(0);
    m_pEdtMarginsBottom->setMaximum(100);
    m_pEdtMarginsBottom->setSuffix(" px");
    m_pEdtMarginsBottom->setValue(margins.left());
    m_pLytMarginsBottom->addWidget(m_pEdtMarginsBottom);
    m_pLytMarginsBottom->addStretch();

    if( !connect(
        /* pObjSender   */ m_pEdtMarginsBottom,
        /* szSignal     */ SIGNAL(editingFinished()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtMarginsBottomEditingFinished()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Stretch> at bottom of widget
    //------------------------------

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtDrawingViewPageSetup::~CWdgtDrawingViewPageSetup()
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
    m_pLytSepLineMargins = nullptr;
    m_pLblSepLineMargins = nullptr;
    m_pSepLineMargins = nullptr;
    m_pLytMarginsLeft = nullptr;
    m_pLblMarginsLeft = nullptr;
    m_pEdtMarginsLeft = nullptr;
    m_pLytMarginsTop = nullptr;
    m_pLblMarginsTop = nullptr;
    m_pEdtMarginsTop = nullptr;
    m_pLytMarginsRight = nullptr;
    m_pLblMarginsRight = nullptr;
    m_pEdtMarginsRight = nullptr;
    m_pLytMarginsBottom = nullptr;
    m_pLblMarginsBottom = nullptr;
    m_pEdtMarginsBottom = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtDrawingViewPageSetup::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    int cxWidth = m_pEdtSizeWidth->value();
    int cyHeight = m_pEdtSizeHeight->value();

    QRectF rect = m_pDrawingView->sceneRect();

    if( rect.width() != cxWidth || rect.height() != cyHeight )
    {
        bHasChanges = true;
    }

    if( !bHasChanges )
    {
        int iMarginLeft = m_pEdtMarginsLeft->value();
        int iMarginTop = m_pEdtMarginsTop->value();
        int iMarginRight = m_pEdtMarginsRight->value();
        int iMarginBottom = m_pEdtMarginsBottom->value();

        QMargins margins = m_pDrawingView->getViewportMargins();

        if( margins.left() != iMarginLeft || margins.top() != iMarginTop
            || margins.right() != iMarginRight || margins.bottom() != iMarginBottom )
        {
            bHasChanges = true;
        }
    }

    return bHasChanges;

} // hasChanges

//------------------------------------------------------------------------------
void CWdgtDrawingViewPageSetup::acceptChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    int cxWidth = m_pEdtSizeWidth->value();
    int cyHeight = m_pEdtSizeHeight->value();

    QRectF rect = m_pDrawingView->sceneRect();

    rect.setWidth(cxWidth);
    rect.setHeight(cyHeight);

    // The scene rectangle of QGraphicsView defines the area of the scene
    // visualized by the view. This is the area of the scene that can be navigated
    // by the scroll bars of the QGraphicsView.
    // Here we don't want to change the visible areay within the scroll bars but
    // the scenes rectangle size.
    m_pDrawingView->getDrawingScene()->setSceneRect(rect);

    int iMarginLeft = m_pEdtMarginsLeft->value();
    int iMarginTop = m_pEdtMarginsTop->value();
    int iMarginRight = m_pEdtMarginsRight->value();
    int iMarginBottom = m_pEdtMarginsBottom->value();

    QMargins margins = m_pDrawingView->getViewportMargins();

    margins.setLeft(iMarginLeft);
    margins.setTop(iMarginTop);
    margins.setRight(iMarginRight);
    margins.setBottom(iMarginBottom);

    m_pDrawingView->setViewportMargins(margins);

} // acceptChanges

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewPageSetup::onEdtSizeWidthEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Height: " + m_pEdtSizeWidth->text();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtSizeWidthEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    emit settingsChanged();

} // onEdtSizeWidthEditingFinished

//------------------------------------------------------------------------------
void CWdgtDrawingViewPageSetup::onEdtSizeHeightEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Height: " + m_pEdtSizeHeight->text();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtSizeHeightEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    emit settingsChanged();

} // onEdtSizeHeightEditingFinished

//------------------------------------------------------------------------------
void CWdgtDrawingViewPageSetup::onEdtMarginsLeftEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Margins.Left: " + m_pEdtMarginsLeft->text();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtMarginsLeftEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    emit settingsChanged();

} // onEdtMarginsLeftEditingFinished

//------------------------------------------------------------------------------
void CWdgtDrawingViewPageSetup::onEdtMarginsTopEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Margins.Top: " + m_pEdtMarginsTop->text();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtMarginsTopEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    emit settingsChanged();

} // onEdtMarginsTopEditingFinished

//------------------------------------------------------------------------------
void CWdgtDrawingViewPageSetup::onEdtMarginsRightEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Margins.Right: " + m_pEdtMarginsRight->text();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtMarginsRightEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    emit settingsChanged();

} // onEdtMarginsRightEditingFinished

//------------------------------------------------------------------------------
void CWdgtDrawingViewPageSetup::onEdtMarginsBottomEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Margins.Bottom: " + m_pEdtMarginsBottom->text();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtMarginsBottomEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    emit settingsChanged();

} // onEdtMarginsBottomEditingFinished
