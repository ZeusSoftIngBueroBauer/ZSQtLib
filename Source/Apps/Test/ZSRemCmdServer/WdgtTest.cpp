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

#include <QtCore/qtimer.h>
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcombobox.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qformlayout.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "WdgtTest.h"
#include "App.h"
#include "Database.h"
#include "MsgTest.h"
#include "TestModule1.h"
#include "TestModule2.h"

#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
//#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::RemCmdServer;


/*******************************************************************************
class CWdgtTest : public QWidget
*******************************************************************************/

/*==============================================================================
private: // class members
==============================================================================*/

CWdgtTest* CWdgtTest::s_pThis = nullptr;

static const QString c_strSendMessage            = "Send Message";
static const QString c_strCreateModule1          = "Create Module 1";
static const QString c_strDeleteModule1          = "Delete Module 1";
static const QString c_strWaitingForConfirmation = "Waiting for Confirmation...";
static const QString c_strCreateModule2          = "Create Module 2";
static const QString c_strDeleteModule2          = "Delete Module 2";

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtTest* CWdgtTest::GetInstance()
//------------------------------------------------------------------------------
{
    return s_pThis;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtTest::CWdgtTest( QWidget* i_pWdgtParent, Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    // All test modules
    m_pTimerWdgtUpdate(nullptr),
    // Test modules
    m_pBtnTestModule1CreateDelete(nullptr),
    m_pBtnTestModule1CreateDeleteModule2(nullptr),
    m_pBtnTestModule1SendMsgToModule2(nullptr),
    // Test module 1
    m_rectTestModule1(),
    m_rectTitleTestModule1(),
    m_rectBtnTestModule1CreateDelete(),
    m_rectBtnTestModule1CreateDeleteModule2(),
    m_rectBtnTestModule1SendMsgToModule2(),
    // Test module 2
    m_rectTestModule2ThreadCtor(),
    m_rectTitleTestModule2ThreadCtorLine1(),
    m_rectTitleTestModule2ThreadCtorLine2(),
    m_rectTestModule2ThreadCtx(),
    m_rectTitleTestModule2ThreadCtxLine1(),
    m_rectTitleTestModule2ThreadCtxLine2(),
    m_rectTestModule2(),
    m_rectTitleTestModule2(),
    // Database
    m_pDb(nullptr),
    // Signal generator
    m_pWdgtSigGen(nullptr),
    m_pLytWdgtSigGen(nullptr),
    m_pCmbSigGenWaveform(nullptr),
    m_pEdtSigGenYMin(nullptr),
    m_pEdtSigGenYMax(nullptr),
    m_pEdtSigGenFreq_Hz(nullptr),
    // Signal analyzer
    m_pWdgtSigAna(nullptr),
    m_pLytWdgtSigAna(nullptr),
    m_pEdtSigAnaSamplingRate_Hz(nullptr),
    m_pEdtSigAnaYVal(nullptr),
    // Tracing
    m_pTrcAdminObj(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::RemCmdServer", "CWdgtTest", "theWidget");

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "ctor",
        /* szAddInfo    */ "" );

    int iTestModuleCount    = 4;
    int iMarginTop          = 100;
    int iMarginBottomMin    = 100;
    int iMarginLeft         = 40;
    int iMarginRightMin     = 40;
    int cxSpace             = 100;
    int cxModuleWidth       = 150;
    int cyModuleHeight      = 120;
    int cxMinimumWidth      = iMarginLeft + iMarginRightMin + iTestModuleCount*cxModuleWidth + (iTestModuleCount-1)*cxSpace;
    int cyMinimumHeight     = iMarginTop + iMarginBottomMin + cyModuleHeight;
    int iModuleMarginTop    = 4;
    int iModuleMarginBottom = 4;
    int iModuleMarginLeft   = 4;
    int iModuleMarginRight  = 4;
    int cyModuleTitleHeight = 20;
    int cyModuleLineSpace   = 4;
    int cxBtnWidth          = cxModuleWidth - iModuleMarginLeft - iModuleMarginRight;
    int cyBtnHeight         = 24;
    int idxCmb;

    setObjectName("WdgtTest");

    // All test modules
    //-----------------

    m_pTimerWdgtUpdate = new QTimer(this);

    if( !connect(
        /* pObjSender   */ m_pTimerWdgtUpdate,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimerWdgtUpdateTimeout()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    m_pTimerWdgtUpdate->start(1000);

    // Test module 1
    //--------------

    m_rectTestModule1.setLeft(iMarginLeft);
    m_rectTestModule1.setTop(iMarginTop);
    m_rectTestModule1.setWidth(cxModuleWidth);
    m_rectTestModule1.setHeight(cyModuleHeight);

    // Title
    m_rectTitleTestModule1.setLeft(m_rectTestModule1.left());
    m_rectTitleTestModule1.setTop( m_rectTestModule1.top() + iModuleMarginTop );
    m_rectTitleTestModule1.setWidth(m_rectTestModule1.width());
    m_rectTitleTestModule1.setHeight(cyModuleTitleHeight);

    // <PushButton> Create/Delete Module 1
    m_rectBtnTestModule1CreateDelete.setLeft( m_rectTestModule1.left() + iModuleMarginLeft );
    m_rectBtnTestModule1CreateDelete.setTop( m_rectTitleTestModule1.bottom() + cyModuleLineSpace );
    m_rectBtnTestModule1CreateDelete.setWidth(cxBtnWidth);
    m_rectBtnTestModule1CreateDelete.setHeight(cyBtnHeight);
    m_pBtnTestModule1CreateDelete = new QPushButton("Create Module 1",this);
    m_pBtnTestModule1CreateDelete->setGeometry(m_rectBtnTestModule1CreateDelete);

    if( !connect(
        /* pObjSender   */ m_pBtnTestModule1CreateDelete,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnTestModule1CreateDeleteClicked()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <PushButton> Create/Delete Module 3
    m_rectBtnTestModule1CreateDeleteModule2.setLeft( m_rectTestModule1.left() + iModuleMarginLeft );
    m_rectBtnTestModule1CreateDeleteModule2.setTop( m_rectBtnTestModule1CreateDelete.bottom() + cyModuleLineSpace );
    m_rectBtnTestModule1CreateDeleteModule2.setWidth(cxBtnWidth);
    m_rectBtnTestModule1CreateDeleteModule2.setHeight(cyBtnHeight);
    m_pBtnTestModule1CreateDeleteModule2 = new QPushButton("Create Module 2",this);
    m_pBtnTestModule1CreateDeleteModule2->setGeometry(m_rectBtnTestModule1CreateDeleteModule2);
    m_pBtnTestModule1CreateDeleteModule2->hide();

    if( !connect(
        /* pObjSender   */ m_pBtnTestModule1CreateDeleteModule2,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnTestModule1CreateDeleteModule2Clicked()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <PushButton> Send Message To Test Module 2
    m_rectBtnTestModule1SendMsgToModule2.setLeft( m_rectTestModule1.left() + iModuleMarginLeft );
    m_rectBtnTestModule1SendMsgToModule2.setTop( m_rectBtnTestModule1CreateDeleteModule2.bottom() + cyModuleLineSpace );
    m_rectBtnTestModule1SendMsgToModule2.setWidth(cxBtnWidth);
    m_rectBtnTestModule1SendMsgToModule2.setHeight(cyBtnHeight);
    m_pBtnTestModule1SendMsgToModule2 = new QPushButton("Send Message",this);
    m_pBtnTestModule1SendMsgToModule2->setGeometry(m_rectBtnTestModule1SendMsgToModule2);
    m_pBtnTestModule1SendMsgToModule2->hide();

    if( !connect(
        /* pObjSender   */ m_pBtnTestModule1SendMsgToModule2,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnTestModule1SendMsgToModule2Clicked()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Test module 2
    //--------------

    m_rectTestModule2.setLeft( m_rectTestModule1.right() + cxSpace );
    m_rectTestModule2.setTop( m_rectTestModule1.top() );
    m_rectTestModule2.setWidth(cxModuleWidth);
    m_rectTestModule2.setHeight(cyModuleHeight);

    // Title
    m_rectTitleTestModule2.setLeft(m_rectTestModule2.left());
    m_rectTitleTestModule2.setTop( m_rectTestModule2.top() + iModuleMarginTop );
    m_rectTitleTestModule2.setWidth(m_rectTestModule2.width());
    m_rectTitleTestModule2.setHeight(cyModuleTitleHeight);

    // Test module 2 (thread created by module 1)
    //-------------------------------------------

    // Thread instance
    m_rectTestModule2ThreadCtor.setLeft( m_rectTestModule1.right() - cxModuleWidth/2 );
    m_rectTestModule2ThreadCtor.setTop( m_rectTestModule1.top() - 2*cyModuleTitleHeight - iModuleMarginTop - iModuleMarginBottom );
    m_rectTestModule2ThreadCtor.setWidth( cxModuleWidth/2 );
    m_rectTestModule2ThreadCtor.setHeight( 2*cyModuleTitleHeight + iModuleMarginTop + iModuleMarginBottom );

    // Title (2 lines)
    m_rectTitleTestModule2ThreadCtorLine1.setLeft(m_rectTestModule2ThreadCtor.left());
    m_rectTitleTestModule2ThreadCtorLine1.setTop( m_rectTestModule2ThreadCtor.top() + iModuleMarginTop );
    m_rectTitleTestModule2ThreadCtorLine1.setWidth(m_rectTestModule2ThreadCtor.width());
    m_rectTitleTestModule2ThreadCtorLine1.setHeight(cyModuleTitleHeight);
    m_rectTitleTestModule2ThreadCtorLine2.setLeft(m_rectTestModule2ThreadCtor.left());
    m_rectTitleTestModule2ThreadCtorLine2.setTop( m_rectTitleTestModule2ThreadCtorLine1.bottom() + cyModuleLineSpace );
    m_rectTitleTestModule2ThreadCtorLine2.setWidth(m_rectTestModule2ThreadCtor.width());
    m_rectTitleTestModule2ThreadCtorLine2.setHeight(cyModuleTitleHeight);

    // Thread context
    m_rectTestModule2ThreadCtx.setLeft( m_rectTestModule2.left() - iModuleMarginLeft );
    m_rectTestModule2ThreadCtx.setTop( m_rectTestModule2.top() - 2*cyModuleTitleHeight - iModuleMarginTop - iModuleMarginBottom );
    m_rectTestModule2ThreadCtx.setWidth( cxModuleWidth + iModuleMarginLeft + iModuleMarginRight );
    m_rectTestModule2ThreadCtx.setHeight( cyModuleHeight + 2*cyModuleTitleHeight + iModuleMarginTop + 2*iModuleMarginBottom );

    // Title (2 lines)
    m_rectTitleTestModule2ThreadCtxLine1.setLeft(m_rectTestModule2ThreadCtx.left());
    m_rectTitleTestModule2ThreadCtxLine1.setTop( m_rectTestModule2ThreadCtx.top() + iModuleMarginTop );
    m_rectTitleTestModule2ThreadCtxLine1.setWidth(m_rectTestModule2ThreadCtx.width());
    m_rectTitleTestModule2ThreadCtxLine1.setHeight(cyModuleTitleHeight);
    m_rectTitleTestModule2ThreadCtxLine2.setLeft(m_rectTestModule2ThreadCtx.left());
    m_rectTitleTestModule2ThreadCtxLine2.setTop( m_rectTitleTestModule2ThreadCtxLine1.bottom() + cyModuleLineSpace );
    m_rectTitleTestModule2ThreadCtxLine2.setWidth(m_rectTestModule2ThreadCtx.width());
    m_rectTitleTestModule2ThreadCtxLine2.setHeight(cyModuleTitleHeight);

    // Signal generator
    //-----------------

    int cxLblWidth     = 120;
    int cxEdtWidth     = 100;
    int cySigGenHeight = 132;

    QPoint ptPosWdgtSigGen( m_rectTestModule2.right() + cxSpace, m_rectTestModule2ThreadCtor.top() - 20 );

    m_pWdgtSigGen = new QGroupBox( "Signal Generator", this );
    m_pWdgtSigGen->move(ptPosWdgtSigGen);

    m_pLytWdgtSigGen = new QFormLayout();
    m_pWdgtSigGen->setLayout(m_pLytWdgtSigGen);

    // <ComboBox> Waveform
    //--------------------

    m_pCmbSigGenWaveform = new QComboBox();
    m_pCmbSigGenWaveform->setFixedWidth(cxEdtWidth);
    m_pCmbSigGenWaveform->setEnabled(false);
    m_pLytWdgtSigGen->addRow( "Waveform:", m_pCmbSigGenWaveform );

    for( idxCmb = 0; idxCmb < ESigGenWaveformCount; idxCmb++ )
    {
        m_pCmbSigGenWaveform->addItem( sigGenWaveform2Str(idxCmb), idxCmb );
    }

    QWidget* pLblSigGenWaveform = m_pLytWdgtSigGen->labelForField(m_pCmbSigGenWaveform);
    pLblSigGenWaveform->setMinimumWidth(cxLblWidth);

    if( !QObject::connect(
        /* pObjSender     */ m_pCmbSigGenWaveform,
        /* szSignal       */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver   */ this,
        /* szSlot         */ SLOT(onCmbSigGenWaveformCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <LineEdit> Y Min
    //--------------------

    m_pEdtSigGenYMin = new QDoubleSpinBox();
    m_pEdtSigGenYMin->setFixedWidth(cxEdtWidth);
    m_pEdtSigGenYMin->setEnabled(false);
    m_pEdtSigGenYMin->setDecimals(1);
    m_pEdtSigGenYMin->setSingleStep(0.1);
    m_pEdtSigGenYMin->setRange(-5.0,-0.1);
    m_pLytWdgtSigGen->addRow( "Y Min:", m_pEdtSigGenYMin );

    if( !QObject::connect(
        /* pObjSender     */ m_pEdtSigGenYMin,
        /* szSignal       */ SIGNAL(valueChanged(double)),
        /* pObjReceiver   */ this,
        /* szSlot         */ SLOT(onEdtSigGenYMinValueChanged(double)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <LineEdit> Y Max
    //--------------------

    m_pEdtSigGenYMax = new QDoubleSpinBox();
    m_pEdtSigGenYMax->setFixedWidth(cxEdtWidth);
    m_pEdtSigGenYMax->setEnabled(false);
    m_pEdtSigGenYMax->setDecimals(1);
    m_pEdtSigGenYMax->setSingleStep(0.1);
    m_pEdtSigGenYMax->setRange(0.1,5.0);
    m_pLytWdgtSigGen->addRow( "Y Max:", m_pEdtSigGenYMax );

    if( !QObject::connect(
        /* pObjSender     */ m_pEdtSigGenYMax,
        /* szSignal       */ SIGNAL(valueChanged(double)),
        /* pObjReceiver   */ this,
        /* szSlot         */ SLOT(onEdtSigGenYMaxValueChanged(double)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <LineEdit> Frequency / Hz
    //--------------------------

    m_pEdtSigGenFreq_Hz = new QDoubleSpinBox();
    m_pEdtSigGenFreq_Hz->setFixedWidth(cxEdtWidth);
    m_pEdtSigGenFreq_Hz->setEnabled(false);
    m_pEdtSigGenFreq_Hz->setDecimals(3);
    m_pEdtSigGenFreq_Hz->setSingleStep(0.001);
    m_pEdtSigGenFreq_Hz->setRange(0.1,5.0);
    m_pLytWdgtSigGen->addRow( "Frequency/Hz:", m_pEdtSigGenFreq_Hz );

    if( !QObject::connect(
        /* pObjSender     */ m_pEdtSigGenFreq_Hz,
        /* szSignal       */ SIGNAL(valueChanged(double)),
        /* pObjReceiver   */ this,
        /* szSlot         */ SLOT(onEdtSigGenFreqHzValueChanged(double)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Signal analyzer
    //-----------------

    QPoint ptPosWdgtSigAna( m_rectTestModule2.right() + cxSpace, ptPosWdgtSigGen.y() + cySigGenHeight + 20 );

    m_pWdgtSigAna = new QGroupBox( "Signal Analyzer", this );
    m_pWdgtSigAna->move(ptPosWdgtSigAna);

    m_pLytWdgtSigAna = new QFormLayout();
    m_pWdgtSigAna->setLayout(m_pLytWdgtSigAna);

    // <LineEdit> Sampling Rate
    //-------------------------

    m_pEdtSigAnaSamplingRate_Hz = new QDoubleSpinBox();
    m_pEdtSigAnaSamplingRate_Hz->setFixedWidth(cxEdtWidth);
    m_pEdtSigAnaSamplingRate_Hz->setEnabled(false);
    m_pEdtSigAnaSamplingRate_Hz->setDecimals(3);
    m_pEdtSigAnaSamplingRate_Hz->setSingleStep(0.001);
    m_pEdtSigAnaSamplingRate_Hz->setRange(5.0,10.0);
    m_pLytWdgtSigAna->addRow( "Sampling Rate/Hz:", m_pEdtSigAnaSamplingRate_Hz );

    QWidget* pLblSigAnaSamplingRate_Hz = m_pLytWdgtSigAna->labelForField(m_pEdtSigAnaSamplingRate_Hz);
    pLblSigAnaSamplingRate_Hz->setMinimumWidth(cxLblWidth);

    if( !QObject::connect(
        /* pObjSender     */ m_pEdtSigAnaSamplingRate_Hz,
        /* szSignal       */ SIGNAL(valueChanged(double)),
        /* pObjReceiver   */ this,
        /* szSlot         */ SLOT(onEdtSigAnaSamplingRateHzValueChanged(double)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <LineEdit> Y Value
    //--------------------------

    QFont fntMeasVal = font();

    fntMeasVal.setBold(true);
    fntMeasVal.setPointSize( fntMeasVal.pointSize() + 8 );

    m_pEdtSigAnaYVal = new QLineEdit("---");
    m_pEdtSigAnaYVal->setFixedWidth(cxEdtWidth);
    m_pEdtSigAnaYVal->setEnabled(false);
    m_pEdtSigAnaYVal->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    m_pLytWdgtSigAna->addRow( "Y Value:", m_pEdtSigAnaYVal );

    m_pEdtSigAnaYVal->setFont(fntMeasVal);

    // Complete test widget
    //---------------------

    setMinimumWidth(cxMinimumWidth);
    setMinimumHeight(cyMinimumHeight);

} // ctor

//------------------------------------------------------------------------------
CWdgtTest::~CWdgtTest()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "dtor",
        /* szAddInfo    */ "" );

    if( m_pDb != nullptr )
    {
        m_pDb->unregisterSigGenClient(this);
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    s_pThis = nullptr;

    // All test modules
    m_pTimerWdgtUpdate = nullptr;
    // Test modules
    m_pBtnTestModule1CreateDelete = nullptr;
    m_pBtnTestModule1CreateDeleteModule2 = nullptr;
    m_pBtnTestModule1SendMsgToModule2 = nullptr;
    // Database
    m_pDb = nullptr;
    // Signal generator
    m_pWdgtSigGen = nullptr;
    m_pLytWdgtSigGen = nullptr;
    m_pCmbSigGenWaveform = nullptr;
    m_pEdtSigGenYMin = nullptr;
    m_pEdtSigGenYMax = nullptr;
    m_pEdtSigGenFreq_Hz = nullptr;
    // Signal analyzer
    m_pWdgtSigGen = nullptr;
    m_pLytWdgtSigGen = nullptr;
    m_pEdtSigAnaSamplingRate_Hz = nullptr;
    m_pEdtSigAnaYVal = nullptr;
    // Tracing
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTest::setDb( CDatabase* i_pDb )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "setDb",
        /* szAddInfo    */ "" );

    m_pDb = i_pDb;

    m_pDb->registerSigGenClient(this);
    m_pDb->registerSigAnaClient(this);

    CMsgReqDbParGetVal* pMsgReq;

    pMsgReq = new CMsgReqDbParGetVal(
        /* pObjSender   */ this,
        /* pObjReceiver */ m_pDb,
        /* strDbPar     */ c_strDbParSigGenWaveform,
        /* iReqId       */ -1 );
    POST_OR_DELETE_MESSAGE(pMsgReq);
    pMsgReq = nullptr;

    pMsgReq = new CMsgReqDbParGetVal(
        /* pObjSender   */ this,
        /* pObjReceiver */ m_pDb,
        /* strDbPar     */ c_strDbParSigGenYMin,
        /* iReqId       */ -1 );
    POST_OR_DELETE_MESSAGE(pMsgReq);
    pMsgReq = nullptr;

    pMsgReq = new CMsgReqDbParGetVal(
        /* pObjSender   */ this,
        /* pObjReceiver */ m_pDb,
        /* strDbPar     */ c_strDbParSigGenYMax,
        /* iReqId       */ -1 );
    POST_OR_DELETE_MESSAGE(pMsgReq);
    pMsgReq = nullptr;

    pMsgReq = new CMsgReqDbParGetVal(
        /* pObjSender   */ this,
        /* pObjReceiver */ m_pDb,
        /* strDbPar     */ c_strDbParSigGenFreq,
        /* iReqId       */ -1 );
    POST_OR_DELETE_MESSAGE(pMsgReq);
    pMsgReq = nullptr;

    pMsgReq = new CMsgReqDbParGetVal(
        /* pObjSender   */ this,
        /* pObjReceiver */ m_pDb,
        /* strDbPar     */ c_strDbParSigAnaSamplingRate,
        /* iReqId       */ -1 );
    POST_OR_DELETE_MESSAGE(pMsgReq);
    pMsgReq = nullptr;

    // Use "direct connection" to avoid that the "onDestroyed" slot is called after
    // the object has already been completely destroyed.
    if( !QObject::connect(
        /* pObjSender     */ m_pDb,
        /* szSignal       */ SIGNAL(destroyed(QObject*)),
        /* pObjReceiver   */ this,
        /* szSlot         */ SLOT(onDbDestroyed(QObject*)),
        /* connectionType */ Qt::DirectConnection ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // setDb

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTest::onDbDestroyed( QObject* /*i_pDb*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onDbDestroyed",
        /* szAddInfo    */ "" );

    m_pDb = nullptr;

} // onDbDestroyed

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtTest::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    switch( static_cast<int>(i_pEv->type()) )
    {
        case EMsgTypeConTest:
        {
            m_pBtnTestModule1CreateDelete->setEnabled(true);
            m_pBtnTestModule1CreateDeleteModule2->setEnabled(true);
            m_pBtnTestModule1SendMsgToModule2->setEnabled(true);
            m_pBtnTestModule1SendMsgToModule2->setText(c_strSendMessage);

            CMsgConTest* pMsgCon = dynamic_cast<CMsgConTest*>(i_pEv);
            if( pMsgCon == nullptr )
            {
                throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
            }
            bEventHandled = true;
            break;
        } // case EMsgTypeConTest

        case EMsgTypeConDbParSetVal:
        {
            CMsgConDbParSetVal* pMsgCon = dynamic_cast<CMsgConDbParSetVal*>(i_pEv);

            if( pMsgCon != nullptr )
            {
                if( pMsgCon->getDbPar() == c_strDbParSigGenWaveform )
                {
                    m_pCmbSigGenWaveform->setEnabled(true);
                }
                else if( pMsgCon->getDbPar() == c_strDbParSigGenYMin )
                {
                    m_pEdtSigGenYMin->setEnabled(true);
                }
                else if( pMsgCon->getDbPar() == c_strDbParSigGenYMax )
                {
                    m_pEdtSigGenYMax->setEnabled(true);
                }
                else if( pMsgCon->getDbPar() == c_strDbParSigGenFreq )
                {
                    //m_pEdtSigGenFreq_Hz->setEnabled(true);
                }
                else if( pMsgCon->getDbPar() == c_strDbParSigAnaSamplingRate )
                {
                    //m_pEdtSigAnaSamplingRate_Hz->setEnabled(true);
                }
            }
            bEventHandled = true;
            break;
        } // case EMsgTypeConDbParSetVal

        case EMsgTypeConDbParGetVal:
        {
            CMsgConDbParGetVal* pMsgCon = dynamic_cast<CMsgConDbParGetVal*>(i_pEv);

            if( pMsgCon != nullptr )
            {
                if( pMsgCon->getDbPar() == c_strDbParSigGenWaveform )
                {
                    m_pCmbSigGenWaveform->setCurrentIndex( m_pCmbSigGenWaveform->findData(pMsgCon->getVal().toInt()) );
                    m_pCmbSigGenWaveform->setEnabled(true);
                }
                else if( pMsgCon->getDbPar() == c_strDbParSigGenYMin )
                {
                    m_pEdtSigGenYMin->setValue( pMsgCon->getVal().toDouble() );
                    m_pEdtSigGenYMin->setEnabled(true);
                }
                else if( pMsgCon->getDbPar() == c_strDbParSigGenYMax )
                {
                    m_pEdtSigGenYMax->setValue( pMsgCon->getVal().toDouble() );
                    m_pEdtSigGenYMax->setEnabled(true);
                }
                else if( pMsgCon->getDbPar() == c_strDbParSigGenFreq )
                {
                    m_pEdtSigGenFreq_Hz->setValue( pMsgCon->getVal().toDouble() );
                    //m_pEdtSigGenFreq_Hz->setEnabled(true);
                }
                else if( pMsgCon->getDbPar() == c_strDbParSigAnaSamplingRate )
                {
                    m_pEdtSigAnaSamplingRate_Hz->setValue( pMsgCon->getVal().toDouble() );
                    //m_pEdtSigAnaSamplingRate_Hz->setEnabled(true);
                }
                else if( pMsgCon->getDbPar() == c_strDbParSigAnaYVal ) // read only
                {
                    m_pEdtSigAnaYVal->setText( QString::number(pMsgCon->getVal().toDouble()) );
                }
            }
            bEventHandled = true;
            break;
        } // case EMsgTypeConDbParGetVal

        case EMsgTypeIndDbParValChanged:
        {
            CMsgIndDbParValChanged* pMsgInd = dynamic_cast<CMsgIndDbParValChanged*>(i_pEv);

            if( pMsgInd != nullptr )
            {
                if( pMsgInd->getDbPar() == c_strDbParSigGenWaveform )
                {
                    m_pCmbSigGenWaveform->setCurrentIndex( m_pCmbSigGenWaveform->findData(pMsgInd->getVal().toInt()) );
                }
                else if( pMsgInd->getDbPar() == c_strDbParSigGenYMin )
                {
                    m_pEdtSigGenYMin->setValue( pMsgInd->getVal().toDouble() );
                }
                else if( pMsgInd->getDbPar() == c_strDbParSigGenYMax )
                {
                    m_pEdtSigGenYMax->setValue( pMsgInd->getVal().toDouble() );
                }
                else if( pMsgInd->getDbPar() == c_strDbParSigGenFreq )
                {
                    m_pEdtSigGenFreq_Hz->setValue( pMsgInd->getVal().toDouble() );
                }
                else if( pMsgInd->getDbPar() == c_strDbParSigAnaSamplingRate )
                {
                    m_pEdtSigAnaSamplingRate_Hz->setValue( pMsgInd->getVal().toDouble() );
                }
                else if( pMsgInd->getDbPar() == c_strDbParSigAnaYVal ) // read only
                {
                    m_pEdtSigAnaYVal->setText( QString::number(pMsgInd->getVal().toDouble(),'f',3) );
                }
            }
            bEventHandled = true;
            break;
        } // case EMsgTypeIndDbParValChanged

        default:
        {
            break;
        }

    } // switch( i_pMsg->type() )

    if( !bEventHandled )
    {
        bEventHandled = QWidget::event(i_pEv);
    }
    return bEventHandled;

} // event

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTest::paintEvent( QPaintEvent* /*i_pEv*/ )
//------------------------------------------------------------------------------
{
    QPainter painter(this);
    QPen     pen;

    // Test module 1
    //---------------

    CTestModule1* pTestModule1 = CApplication::GetInstance()->getTestModule1();

    if( pTestModule1 == nullptr )
    {
        m_pBtnTestModule1CreateDelete->setText(c_strCreateModule1);
        m_pBtnTestModule1CreateDeleteModule2->hide();

        pen.setColor(Qt::gray);
        pen.setStyle(Qt::DotLine);
    }
    else
    {
        m_pBtnTestModule1CreateDelete->setText(c_strDeleteModule1);
        m_pBtnTestModule1CreateDeleteModule2->show();

        pen.setColor(Qt::black);
        pen.setStyle(Qt::SolidLine);
    }

    painter.setPen(pen);
    painter.drawRect(m_rectTestModule1);
    painter.drawText( m_rectTitleTestModule1, Qt::AlignHCenter|Qt::AlignVCenter, "Module 1" );

    // Test module 2
    //---------------

    CTestModule2Thread* pTestModule2Thread = CApplication::GetInstance()->getTestModule2Thread();
    CTestModule2*       pTestModule2       = CApplication::GetInstance()->getTestModule2();

    if( pTestModule2 == nullptr )
    {
        pen.setColor(Qt::gray);
        pen.setStyle(Qt::DotLine);
    }
    else
    {
        pen.setColor(Qt::black);
        pen.setStyle(Qt::SolidLine);
    }

    painter.setPen(pen);
    painter.drawRect(m_rectTestModule2ThreadCtor);
    painter.drawText(m_rectTitleTestModule2ThreadCtorLine1,Qt::AlignHCenter|Qt::AlignVCenter,"Module 2");
    painter.drawText(m_rectTitleTestModule2ThreadCtorLine2,Qt::AlignHCenter|Qt::AlignVCenter,"ctor");
    painter.drawRect(m_rectTestModule2ThreadCtx);
    painter.drawText(m_rectTitleTestModule2ThreadCtxLine1,Qt::AlignHCenter|Qt::AlignVCenter,"Module 2");
    painter.drawText(m_rectTitleTestModule2ThreadCtxLine2,Qt::AlignHCenter|Qt::AlignVCenter,"run");

    if( pTestModule2Thread == nullptr )
    {
        m_pBtnTestModule1CreateDeleteModule2->setText(c_strCreateModule2);
        m_pBtnTestModule1SendMsgToModule2->hide();

        pen.setColor(Qt::gray);
        pen.setStyle(Qt::DotLine);
    }
    else
    {
        m_pBtnTestModule1CreateDeleteModule2->setText(c_strDeleteModule2);
        m_pBtnTestModule1SendMsgToModule2->show();

        pen.setColor(Qt::black);
        pen.setStyle(Qt::SolidLine);
    }

    painter.setPen(pen);
    painter.drawRect(m_rectTestModule2);
    painter.drawText(m_rectTitleTestModule2,Qt::AlignHCenter|Qt::AlignVCenter,"Module 2");

} // paintEvent

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTest::onTimerWdgtUpdateTimeout()
//------------------------------------------------------------------------------
{
    update();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTest::onBtnTestModule1CreateDeleteClicked()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onBtnTestModule1CreateDeleteClicked",
        /* szAddInfo    */ "" );

    if( m_pBtnTestModule1CreateDelete->text() == c_strCreateModule1 )
    {
        CApplication::GetInstance()->createModule1();
    }
    else if( m_pBtnTestModule1CreateDelete->text() == c_strDeleteModule1 )
    {
        CApplication::GetInstance()->deleteModule1();
    }

    // Trigger paint event
    update();

} // onBtnTestModule1CreateDeleteClicked

//------------------------------------------------------------------------------
void CWdgtTest::onBtnTestModule1CreateDeleteModule2Clicked()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onBtnTestModule1CreateDeleteModule2Clicked",
        /* szAddInfo    */ "" );

    if( m_pBtnTestModule1CreateDeleteModule2->text() == c_strCreateModule2 )
    {
        CApplication::GetInstance()->createModule2();
    }
    else if( m_pBtnTestModule1CreateDeleteModule2->text() == c_strDeleteModule2 )
    {
        CApplication::GetInstance()->deleteModule2();
    }

    update();

} // onBtnTestModule1CreateDeleteModule2Clicked

//------------------------------------------------------------------------------
void CWdgtTest::onBtnTestModule1SendMsgToModule2Clicked()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onBtnTestModule1SendMsgToModule2Clicked",
        /* szAddInfo    */ "" );

    CTestModule2* pTestModule2 = CApplication::GetInstance()->getTestModule2();

    if( pTestModule2 != nullptr )
    {
        CMsgReqTest* pMsgReq = new CMsgReqTest(
            /* pObjSender       */ this,
            /* pObjReceiver     */ pTestModule2,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ -1 );
        POST_OR_DELETE_MESSAGE(pMsgReq);
        pMsgReq = nullptr;

        m_pBtnTestModule1SendMsgToModule2->setText(c_strWaitingForConfirmation);
    }

} // onBtnTestModule1SendMsgToModule2Clicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTest::onCmbSigGenWaveformCurrentIndexChanged( int /*i_idx*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onCmbSigGenWaveformCurrentIndexChanged",
        /* szAddInfo    */ "" );

} // onCmbSigGenWaveformCurrentIndexChanged

//------------------------------------------------------------------------------
void CWdgtTest::onEdtSigGenYMinValueChanged( double i_fVal )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onEdtSigGenYMinValueChanged",
        /* szAddInfo    */ "" );

    if( m_pDb != nullptr )
    {
        CMsgReqDbParSetVal* pMsgReq = new CMsgReqDbParSetVal(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pDb,
            /* strDbPar         */ c_strDbParSigGenYMin,
            /* val              */ i_fVal,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ -1 );
        POST_OR_DELETE_MESSAGE(pMsgReq);
        pMsgReq = nullptr;

        m_pEdtSigGenYMin->setEnabled(false);
    }

} // onEdtSigGenYMinValueChanged

//------------------------------------------------------------------------------
void CWdgtTest::onEdtSigGenYMaxValueChanged( double i_fVal )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onEdtSigGenYMaxValueChanged",
        /* szAddInfo    */ "" );

    if( m_pDb != nullptr )
    {
        CMsgReqDbParSetVal* pMsgReq = new CMsgReqDbParSetVal(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pDb,
            /* strDbPar         */ c_strDbParSigGenYMax,
            /* val              */ i_fVal,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ -1 );
        POST_OR_DELETE_MESSAGE(pMsgReq);
        pMsgReq = nullptr;

        m_pEdtSigGenYMax->setEnabled(false);
    }

} // onEdtSigGenYMaxValueChanged

//------------------------------------------------------------------------------
void CWdgtTest::onEdtSigGenFreqHzValueChanged( double /*i_fVal*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onEdtSigGenFreqHzValueChanged",
        /* szAddInfo    */ "" );

} // onEdtSigGenFreqHzValueChanged

//------------------------------------------------------------------------------
void CWdgtTest::onEdtSigAnaSamplingRateHzValueChanged( double /*i_fVal*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onEdtSigAnaSamplingRateHzValueChanged",
        /* szAddInfo    */ "" );

} // onEdtSigAnaSamplingRateHzValueChanged
