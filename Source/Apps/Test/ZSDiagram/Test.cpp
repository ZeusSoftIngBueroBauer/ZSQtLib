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

#include "Test.h"
#include "App.h"
#include "WidgetCentral.h"
#include "WdgtTestOutput.h"
#include "Units/Units.h"

#include "ZSDiagram/ZSDiagramProcWdgt.h"
#include "ZSDiagram/ZSDiagTrace.h"
#include "ZSDiagram/ZSDiagObjAxisLabel.h"
#include "ZSDiagram/ZSDiagObjCurve.h"
#include "ZSDiagram/ZSDiagObjGrid.h"
#include "ZSDiagram/ZSDiagObjLabel.h"
#include "ZSDiagram/ZSDiagObjMarker.h"
#include "ZSDiagram/ZSDiagObjRect.h"
#include "ZSDiagram/ZSDiagramFrameStyles.h"
#include "ZSDiagram/ZSDiagramImageStyles.h"
#include "ZSDiagram/ZSDiagramLineStyles.h"
#include "ZSDiagram/ZSDiagramToolTipStyles.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysMath.h"

#include <QtCore/qtimer.h>

#if QT_VERSION < 0x050000
#include <QtGui/qframe.h>
#include <QtGui/qlayout.h>
#else
#include <QtWidgets/qframe.h>
#include <QtWidgets/qlayout.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::Diagram;


/*******************************************************************************
class CTest
*******************************************************************************/

// ColorCodes
//------------

static const QColor c_colRed("#FF0000");
static const QColor c_colWhite("#FFFFFF");
static const QColor c_colTurquoise("#00FFFF");
static const QColor c_colLightGrey("#C0C0C0");
static const QColor c_colLightBlue("#0000FF");
static const QColor c_colDarkGrey("#808080");
static const QColor c_colDarkBlue("#0000A0");
static const QColor c_colBlack("#000000");
static const QColor c_colLightPurple("#FF0080");
static const QColor c_colOrange("#FF8040");
static const QColor c_colDarkPurple("#800080");
static const QColor c_colBrown("#804000");
static const QColor c_colYellow("#FFFF00");
static const QColor c_colBurgundy("#800000");
static const QColor c_colPastelGreen("#00FF00");
static const QColor c_colForestGreen("#808000");
static const QColor c_colPink("#FF00FF");
static const QColor c_colGrassGreen("#408080");

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest() :
//------------------------------------------------------------------------------
    ZS::Test::CTest(NameSpace(), "theInst"),
    m_pTmrTestStepTimeout(nullptr),
    m_pTimerSigGen(nullptr),
    m_pWdgtDiagram(nullptr),
    m_scaleX(899.0e6, 901.0e6, Frequency.Hertz, CPhysValRes(0.1, Frequency.Hertz)),
    m_scaleY(0.0, 40.0, Power.dBm, CPhysValRes(0.1, Power.dBm)),
    m_pDiagScaleX(nullptr),
    m_pDiagScaleY(nullptr),
    m_pDiagObjGrid(nullptr),
    m_pDiagObjAxisLabelX(nullptr),
    m_pDiagObjAxisLabelY(nullptr),
    m_pDiagTraceSigGen1(nullptr),
    m_pDiagTraceSigGen2(nullptr),
    m_pDiagObjCurveSigGen1(nullptr),
    m_pDiagObjCurveSigGen2(nullptr),
    m_pDiagObjMarkerSigGen1(nullptr),
    m_pDiagObjMarkerSigGen2(nullptr),
    m_mapSBarCols(),
    m_mapSBarLabelsPosOffsY(),
    m_ararfIsoTab_dB(),
    m_mapiPathsSBars(),
    m_mappDiagTracesSBars(),
    m_mappDiagObjRectsSBars(),
    m_mappDiagObjLabelsSBars(),
    m_mappDiagTracesSBarsIfCheck(),
    m_mappDiagObjRectsSBarsIfCheck(),
    m_strlstSBarsLocked()
{
    m_pTmrTestStepTimeout = new QTimer();
    m_pTmrTestStepTimeout->setSingleShot(true);

    if( !QObject::connect(
        /* pObjSender   */ m_pTmrTestStepTimeout,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimerTestStepTimeout()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Test Steps
    //-----------

    //ZS::Test::CTestStep* pTestStep = nullptr;

    int idxSubGroup = 0;

    int idxStep = 0;

    // Test Group: Signal Generator
    //------------------------------

    ZS::Test::CTestStepGroup* pGrpSigGen = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Signal Generator",
        /* pTSGrpParent    */ nullptr );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Create Diagram",
        /* strOperation    */ "new CDiagram",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenCreateDiagram(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Scales",
        /* strOperation    */ "CDiagram::addDiagScale()",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenAddScales(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Traces",
        /* strOperation    */ "CDiagram::doTestStepAddSigGenTraces()",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenAddTraces(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Grid",
        /* strOperation    */ "CDiagram::addDiagObj(CDiagObjGrid)",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenAddGrid(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Axis Labels",
        /* strOperation    */ "CDiagram::addDiagObj(CDiagObjAxisLabel)",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenAddAxisLabels(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Curves",
        /* strOperation    */ "CDiagram::addDiagObj(CDiagObjCurve)",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenAddCurves(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Markers",
        /* strOperation    */ "CDiagram::addDiagObj(CDiagObjMarker)",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenAddMarkers(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Move Markers",
        /* strOperation    */ "CDiagObjMarker::setVal()",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenMoveMarkers(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Single Shot Signal Generator",
        /* strOperation    */ "CDiagTrace::setValues()",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenSingleShot(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Start Signal Generator",
        /* strOperation    */ "CDiagTrace::setValues()",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenStart(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Stop Signal Generator",
        /* strOperation    */ "TimerSigGen.stop()",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenStop(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Remove Markers",
        /* strOperation    */ "CDiagram::removeDiagObj(CDiagObjMarker)",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenRemoveMarkers(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Remove Curves",
        /* strOperation    */ "CDiagram::removeDiagObj(CDiagObjCurve)",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenRemoveCurves(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Remove Axis Labels",
        /* strOperation    */ "CDiagram::removeDiagObj(CDiagObjAxisLabel)",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenRemoveAxisLabels(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Remove Grid",
        /* strOperation    */ "CDiagram::removeDiagObj(CDiagObjGrid)",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenRemoveGrid(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Remove Traces",
        /* strOperation    */ "CDiagram::removeDiagTrace()",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenRemoveTraces(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Remove Scales",
        /* strOperation    */ "CDiagram::removeDiagScale()",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenRemoveScales(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Destroy Diagram",
        /* strOperation    */ "delete CDiagram",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenDestroyDiagram(ZS::Test::CTestStep*)) );

    // Recall test step settings
    //--------------------------

    recallTestSteps();

} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo = saveTestSteps();

    if(errResultInfo.isErrorResult())
    {
        if(CErrLog::GetInstance() != nullptr)
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    try
    {
        delete m_pTmrTestStepTimeout;
    }
    catch(...)
    {
    }

    m_pTmrTestStepTimeout = nullptr;
    m_pTimerSigGen = nullptr;
    m_pWdgtDiagram = nullptr;
    //m_scaleX;
    //m_scaleY;
    m_pDiagScaleX = nullptr;
    m_pDiagScaleY = nullptr;
    m_pDiagObjGrid = nullptr;
    m_pDiagObjAxisLabelX = nullptr;
    m_pDiagObjAxisLabelY = nullptr;
    m_pDiagTraceSigGen1 = nullptr;
    m_pDiagTraceSigGen2 = nullptr;
    m_pDiagObjCurveSigGen1 = nullptr;
    m_pDiagObjCurveSigGen2 = nullptr;
    m_pDiagObjMarkerSigGen1 = nullptr;
    m_pDiagObjMarkerSigGen2 = nullptr;
    m_mapSBarCols.clear();
    m_mapSBarLabelsPosOffsY.clear();
    m_ararfIsoTab_dB.clear();
    m_mapiPathsSBars.clear();
    m_mappDiagTracesSBars.clear();
    m_mappDiagObjRectsSBars.clear();
    m_mappDiagObjLabelsSBars.clear();
    m_mappDiagTracesSBarsIfCheck.clear();
    m_mappDiagObjRectsSBarsIfCheck.clear();
    m_strlstSBarsLocked.clear();

} // dtor

/*==============================================================================
public slots: // test step methods (Signal Generators)
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenCreateDiagram( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("Diagram added to test output widget");
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    m_pWdgtDiagram = new CWdgtDiagram("Diagram");
    m_pWdgtDiagram->setMouseTracking(true);
    m_pWdgtDiagram->enableZooming();
    //m_pWdgtDiagram->setMarginTop(0);
    //m_pWdgtDiagram->setMarginBottom(0);
    //m_pWdgtDiagram->setMarginLeft(0);
    //m_pWdgtDiagram->setMarginRight(0);
    m_pWdgtDiagram->setMinimumHeightPartTop(10);
    m_pWdgtDiagram->setMinimumHeightPartBottom(10);
    m_pWdgtDiagram->setMinimumWidthPartLeft(10);
    m_pWdgtDiagram->setMinimumWidthPartRight(20);

    SFrameStyle3DSunken* pFrameStyleDiagram = new SFrameStyle3DSunken;
    m_pWdgtDiagram->setFrameStyle(pFrameStyleDiagram);
    pFrameStyleDiagram = nullptr;

    //SFrameStyle* pFrameStyleDiagramPartCenter = new SFrameStyle(1,Qt::red);
    //m_pWdgtDiagram->setFrameStylePartCenter(pFrameStyleDiagramPartCenter);
    //pFrameStyleDiagramPartCenter = nullptr;

    m_pWdgtDiagram->setFixedWidth(1020);
    m_pWdgtDiagram->setFixedHeight(330);

    CWidgetCentral::GetInstance()->getTestOutputWidget()->setDiagram(m_pWdgtDiagram);

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        QString strResultValue = "Diagram not added to test output widget";
        QWidget* pChildWidget = CWidgetCentral::GetInstance()->getTestOutputWidget()->childAt(QPoint(50, 50));
        if (pChildWidget != nullptr) {
            QFrame* pChildFrame = dynamic_cast<QFrame*>(pChildWidget);
            if (pChildFrame != nullptr) {
                QLayout* pLyt = pChildFrame->layout();
                if (pLyt != nullptr) {
                    int idxDiagram = pLyt->indexOf(m_pWdgtDiagram);
                    if (idxDiagram >= 0) {
                        strResultValue = "Diagram added to test output widget";
                    }
                }
            }
        }
        strlstResultValues.append(strResultValue);
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenCreateDiagram

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenDestroyDiagram( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("Diagram removed from test output widget");
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    delete m_pWdgtDiagram;
    m_pWdgtDiagram = nullptr;

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        QString strResultValue = "Diagram not removed from test output widget";
        QWidget* pChildWidget = CWidgetCentral::GetInstance()->getTestOutputWidget()->childAt(QPoint(50, 50));
        if (pChildWidget != nullptr) {
            QFrame* pChildFrame = dynamic_cast<QFrame*>(pChildWidget);
            if (pChildFrame != nullptr) {
                QLayout* pLyt = pChildFrame->layout();
                if (pLyt != nullptr) {
                    int idxDiagram = pLyt->indexOf(m_pWdgtDiagram);
                    if (idxDiagram < 0) {
                        strResultValue = "Diagram removed from test output widget";
                    }
                }
            }
        }
        strlstResultValues.append(strResultValue);
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenDestroyDiagram

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddScales( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("X-Scale added to diagram");
    strlstExpectedValues.append("Y-Scale added to diagram");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pWdgtDiagram != nullptr )
    {
        m_pDiagScaleX = new CDiagScale("X", EScaleAxis::X, m_scaleX);
        m_pDiagScaleX->setDivLineDistMinPix(EDivLineLayer::Main, 20);
        m_pWdgtDiagram->addDiagScale(m_pDiagScaleX);

        m_pDiagScaleY = new CDiagScale("Y", EScaleAxis::Y, m_scaleY);
        m_pDiagScaleY->setDivLineDistMinPix(EDivLineLayer::Main, 20);
        m_pWdgtDiagram->addDiagScale(m_pDiagScaleY);
    }

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        QString strResultValueXScale = "X-Scale not added to diagram";
        QString strResultValueYScale = "Y-Scale not added to diagram";
        if (m_pWdgtDiagram != nullptr) {
            CDiagScale* pDiagScaleX = m_pWdgtDiagram->findDiagScale("X");
            if (pDiagScaleX != nullptr) {
                strResultValueXScale = "X-Scale added to diagram";
            }
            CDiagScale* pDiagScaleY = m_pWdgtDiagram->findDiagScale("Y");
            if (pDiagScaleY != nullptr) {
                strResultValueYScale = "Y-Scale added to diagram";
            }
        }
        strlstResultValues.append(strResultValueXScale);
        strlstResultValues.append(strResultValueYScale);
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenAddScales

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveScales( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("X-Scale removed from diagram");
    strlstExpectedValues.append("Y-Scale removed from diagram");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pDiagScaleX != nullptr )
    {
        m_pWdgtDiagram->removeDiagScale(m_pDiagScaleX);
        delete m_pDiagScaleX;
        m_pDiagScaleX = nullptr;
    }

    if( m_pDiagScaleY != nullptr )
    {
        m_pWdgtDiagram->removeDiagScale(m_pDiagScaleY);
        delete m_pDiagScaleY;
        m_pDiagScaleY = nullptr;
    }

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        QString strResultValueXScale = "X-Scale not removed from diagram";
        QString strResultValueYScale = "Y-Scale not removed from diagram";
        if (m_pWdgtDiagram != nullptr) {
            CDiagScale* pDiagScaleX = m_pWdgtDiagram->findDiagScale("X");
            if (pDiagScaleX == nullptr) {
                strResultValueXScale = "X-Scale removed from diagram";
            }
            CDiagScale* pDiagScaleY = m_pWdgtDiagram->findDiagScale("Y");
            if (pDiagScaleY == nullptr) {
                strResultValueYScale = "Y-Scale removed from diagram";
            }
        }
        strlstResultValues.append(strResultValueXScale);
        strlstResultValues.append(strResultValueYScale);
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenRemoveScales

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddTraces( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("SigGen1-Trace added to diagram");
    strlstExpectedValues.append("SigGen2-Trace added to diagram");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pWdgtDiagram != nullptr && m_pDiagScaleX != nullptr && m_pDiagScaleY != nullptr )
    {
        m_pDiagTraceSigGen1 = new CDiagTrace(
            /* strObjName  */ "SigGen1",
            /* pDiagScaleX */ m_pDiagScaleX,
            /* pDiagScaleY */ m_pDiagScaleY,
            /* physValResX */ m_pDiagScaleX->getScaleRes(),
            /* physValResY */ m_pDiagScaleY->getScaleRes());
        m_pWdgtDiagram->addDiagTrace(m_pDiagTraceSigGen1);

        m_pDiagTraceSigGen2 = new CDiagTrace(
            /* strObjName  */ "SigGen2",
            /* pDiagScaleX */ m_pDiagScaleX,
            /* pDiagScaleY */ m_pDiagScaleY,
            /* physValResX */ m_pDiagScaleX->getScaleRes(),
            /* physValResY */ m_pDiagScaleY->getScaleRes());
        m_pWdgtDiagram->addDiagTrace(m_pDiagTraceSigGen2);

    } // if( m_pWdgtDiagram != nullptr && m_pDiagScaleX != nullptr && m_pDiagScaleY != nullptr )

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        QString strResultValueSigGen1 = "SigGen1-Trace not added to diagram";
        QString strResultValueSigGen2 = "SigGen2-Trace not added to diagram";
        if (m_pWdgtDiagram != nullptr) {
            CDiagTrace* pDiagTraceSigGen1 = m_pWdgtDiagram->findDiagTrace("SigGen1");
            if (pDiagTraceSigGen1 != nullptr) {
                strResultValueSigGen1 = "SigGen1-Trace added to diagram";
            }
            CDiagTrace* pDiagTraceSigGen2 = m_pWdgtDiagram->findDiagTrace("SigGen2");
            if (pDiagTraceSigGen2 != nullptr) {
                strResultValueSigGen2 = "SigGen2-Trace added to diagram";
            }
        }
        strlstResultValues.append(strResultValueSigGen1);
        strlstResultValues.append(strResultValueSigGen2);
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenAddTraces

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveTraces( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("SigGen1-Trace removed from diagram");
    strlstExpectedValues.append("SigGen2-Trace removed from diagram");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pDiagTraceSigGen2 != nullptr )
    {
        m_pWdgtDiagram->removeDiagTrace(m_pDiagTraceSigGen2);
        delete m_pDiagTraceSigGen2;
        m_pDiagTraceSigGen2 = nullptr;
    }

    if( m_pDiagTraceSigGen1 != nullptr )
    {
        m_pWdgtDiagram->removeDiagTrace(m_pDiagTraceSigGen1);
        delete m_pDiagTraceSigGen1;
        m_pDiagTraceSigGen1 = nullptr;
    }

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        QString strResultValueSigGen1 = "SigGen1-Trace not removed from diagram";
        QString strResultValueSigGen2 = "SigGen2-Trace not removed from diagram";
        if (m_pWdgtDiagram != nullptr) {
            CDiagTrace* pDiagTraceSigGen1 = m_pWdgtDiagram->findDiagTrace("SigGen1");
            if (pDiagTraceSigGen1 == nullptr) {
                strResultValueSigGen1 = "SigGen1-Trace removed from diagram";
            }
            CDiagTrace* pDiagTraceSigGen2 = m_pWdgtDiagram->findDiagTrace("SigGen2");
            if (pDiagTraceSigGen2 == nullptr) {
                strResultValueSigGen2 = "SigGen2-Trace removed from diagram";
            }
        }
        strlstResultValues.append(strResultValueSigGen1);
        strlstResultValues.append(strResultValueSigGen2);
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenRemoveTraces

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddGrid( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("Grid added to diagram");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pWdgtDiagram != nullptr && m_pDiagScaleX != nullptr && m_pDiagScaleY != nullptr )
    {
        m_pDiagObjGrid = new CDiagObjGrid(
            /* strObjName  */ "Grid",
            /* pDiagScaleX */ m_pDiagScaleX,
            /* pDiagScaleY */ m_pDiagScaleY );
        m_pWdgtDiagram->addDiagObj(m_pDiagObjGrid);

        m_pWdgtDiagram->update();

    } // if( m_pWdgtDiagram != nullptr && m_pDiagScaleX != nullptr && m_pDiagScaleY != nullptr )

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        QString strResultValue = "Grid not added to diagram";
        if (m_pWdgtDiagram != nullptr) {
            CDiagObjGrid* pDiagObjGrid = dynamic_cast<CDiagObjGrid*>(m_pWdgtDiagram->findDiagObj(CDiagObjGrid::ClassName(), "Grid"));
            if (pDiagObjGrid != nullptr) {
                strResultValue = "Grid added to diagram";
            }
        }
        strlstResultValues.append(strResultValue);
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenAddGrid

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveGrid( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("Grid removed from diagram");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pDiagObjGrid != nullptr )
    {
        m_pWdgtDiagram->removeDiagObj(m_pDiagObjGrid);
        delete m_pDiagObjGrid;
        m_pDiagObjGrid = nullptr;
    }

    if( m_pWdgtDiagram != nullptr )
    {
        m_pWdgtDiagram->update();
    }

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        QString strResultValue = "Grid not removed from to diagram";
        if (m_pWdgtDiagram != nullptr) {
            CDiagObj* pDiagObjGrid = m_pWdgtDiagram->findDiagObj(CDiagObjGrid::ClassName(), "Grid");
            if (pDiagObjGrid == nullptr) {
                strResultValue = "Grid removed from diagram";
            }
        }
        strlstResultValues.append(strResultValue);
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenRemoveGrid

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddAxisLabels( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("X-AxisLabel added to diagram");
    strlstExpectedValues.append("Y-AxisLabel added to diagram");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pWdgtDiagram != nullptr && m_pDiagScaleX != nullptr )
    {
        m_pDiagObjAxisLabelX = new CDiagObjAxisLabel(
            /* strObjName  */ "X-AxisLabel",
            /* pDiagScaleX */ m_pDiagScaleX,
            /* layoutPos   */ ELayoutPos::Bottom );
        m_pDiagObjAxisLabelX->setSpacingDiagPartCenter2DivLineLabels(10);
        m_pDiagObjAxisLabelX->setSpacingDivLineLabels2AxisLabel(10);
        m_pDiagObjAxisLabelX->setDivLinesColor(Qt::red);
        m_pDiagObjAxisLabelX->setDivLinesPenStyle(Qt::SolidLine);
        m_pDiagObjAxisLabelX->setAxisLabel("Frequency");
        m_pDiagObjAxisLabelX->showUnitAtDivLines();
        m_pDiagObjAxisLabelX->showUnitAtAxisLabel();
        m_pWdgtDiagram->addDiagObj(m_pDiagObjAxisLabelX);
    }

    if( m_pWdgtDiagram != nullptr && m_pDiagScaleY != nullptr )
    {
        m_pDiagObjAxisLabelY = new CDiagObjAxisLabel(
            /* strObjName  */ "Y-AxisLabel",
            /* pDiagScaleX */ m_pDiagScaleY,
            /* layoutPos   */ ELayoutPos::Left );
        m_pDiagObjAxisLabelY->setSpacingDiagPartCenter2DivLineLabels(10);
        m_pDiagObjAxisLabelY->setSpacingDivLineLabels2AxisLabel(10);
        m_pDiagObjAxisLabelY->setDivLinesColor(Qt::red);
        m_pDiagObjAxisLabelY->setDivLinesPenStyle(Qt::SolidLine);
        m_pDiagObjAxisLabelY->setAxisLabel("Power");
        m_pDiagObjAxisLabelY->showUnitAtDivLines();
        m_pDiagObjAxisLabelY->showUnitAtAxisLabel();
        m_pWdgtDiagram->addDiagObj(m_pDiagObjAxisLabelY);
    }

    if( m_pWdgtDiagram != nullptr )
    {
        m_pWdgtDiagram->update();
    }

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        QString strResultValueXAxisLabel = "X-AxisLabel not added to diagram";
        QString strResultValueYAxisLabel = "Y-AxisLabel not added to diagram";
        if (m_pWdgtDiagram != nullptr) {
            CDiagObjAxisLabel* pDiagObjAxisLabelX = dynamic_cast<CDiagObjAxisLabel*>(m_pWdgtDiagram->findDiagObj(CDiagObjAxisLabel::ClassName(), "X-AxisLabel"));
            if (pDiagObjAxisLabelX != nullptr) {
                strResultValueXAxisLabel = "X-AxisLabel added to diagram";
            }
            CDiagObjAxisLabel* pDiagObjAxisLabelY = dynamic_cast<CDiagObjAxisLabel*>(m_pWdgtDiagram->findDiagObj(CDiagObjAxisLabel::ClassName(), "Y-AxisLabel"));
            if (pDiagObjAxisLabelY != nullptr) {
                strResultValueYAxisLabel = "Y-AxisLabel added to diagram";
            }
        }
        strlstResultValues.append(strResultValueXAxisLabel);
        strlstResultValues.append(strResultValueYAxisLabel);
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenAddAxisLabels

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveAxisLabels( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("X-AxisLabel removed from diagram");
    strlstExpectedValues.append("Y-AxisLabel removed from diagram");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pDiagObjAxisLabelY != nullptr )
    {
        m_pWdgtDiagram->removeDiagObj(m_pDiagObjAxisLabelY);
        delete m_pDiagObjAxisLabelY;
        m_pDiagObjAxisLabelY = nullptr;
    }

    if( m_pDiagObjAxisLabelX != nullptr )
    {
        m_pWdgtDiagram->removeDiagObj(m_pDiagObjAxisLabelX);
        delete m_pDiagObjAxisLabelX;
        m_pDiagObjAxisLabelX = nullptr;
    }

    if( m_pWdgtDiagram != nullptr )
    {
        m_pWdgtDiagram->update();
    }

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        QString strResultValueXAxisLabel = "X-AxisLabel not removed from diagram";
        QString strResultValueYAxisLabel = "Y-AxisLabel not removed from diagram";
        if (m_pWdgtDiagram != nullptr) {
            CDiagObj* pDiagObjAxisLabelX = m_pWdgtDiagram->findDiagObj(CDiagObjAxisLabel::ClassName(), "X-AxisLabel");
            if (pDiagObjAxisLabelX == nullptr) {
                strResultValueXAxisLabel = "X-AxisLabel removed from diagram";
            }
            CDiagObj* pDiagObjAxisLabelY = m_pWdgtDiagram->findDiagObj(CDiagObjAxisLabel::ClassName(), "Y-AxisLabel");
            if (pDiagObjAxisLabelY == nullptr) {
                strResultValueYAxisLabel = "Y-AxisLabel removed from diagram";
            }
        }
        strlstResultValues.append(strResultValueXAxisLabel);
        strlstResultValues.append(strResultValueYAxisLabel);
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenRemoveAxisLabels

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddCurves( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("Curve-SigGen1 added to diagram");
    strlstExpectedValues.append("Curve-SigGen2 added to diagram");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    /*
    c_colRed("#FF0000");
    c_colWhite("#FFFFFF");
    c_colTurquoise("#00FFFF");
    c_colLightGrey("#C0C0C0");
    c_colLightBlue("#0000FF");
    c_colDarkGrey("#808080");
    c_colDarkBlue("#0000A0");
    c_colBlack("#000000");
    c_colLightPurple("#FF0080");
    c_colOrange("#FF8040");
    c_colDarkPurple("#800080");
    c_colBrown("#804000");
    c_colYellow("#FFFF00");
    c_colBurgundy("#800000");
    c_colPastelGreen("#00FF00");
    c_colForestGreen("#808000");
    c_colPink("#FF00FF");
    c_colGrassGreen("#408080");
    */

    if( m_pWdgtDiagram != nullptr && m_pDiagTraceSigGen1 != nullptr )
    {
        m_pDiagObjCurveSigGen1 = new CDiagObjCurve(
            /* strObjName */ "Curve-SigGen1",
            /* pDiagTrace */ m_pDiagTraceSigGen1 );
        m_pDiagObjCurveSigGen1->setCol(c_colYellow);
        m_pWdgtDiagram->addDiagObj(m_pDiagObjCurveSigGen1);
    }

    if( m_pWdgtDiagram != nullptr && m_pDiagTraceSigGen2 != nullptr )
    {
        m_pDiagObjCurveSigGen2 = new CDiagObjCurve(
            /* strObjName */ "Curve-SigGen2",
            /* pDiagTrace */ m_pDiagTraceSigGen2 );
        m_pDiagObjCurveSigGen2->setCol(c_colPastelGreen);
        m_pWdgtDiagram->addDiagObj(m_pDiagObjCurveSigGen2);
    }

    if( m_pWdgtDiagram != nullptr )
    {
        m_pWdgtDiagram->update();
    }

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        QString strResultValueSigGen1 = "Curve-SigGen1 not added to diagram";
        QString strResultValueSigGen2 = "Curve-SigGen2 not added to diagram";
        if (m_pWdgtDiagram != nullptr) {
            CDiagObjCurve* pDiagObjCurveSigGen1 = dynamic_cast<CDiagObjCurve*>(m_pWdgtDiagram->findDiagObj(CDiagObjCurve::ClassName(), "Curve-SigGen1"));
            if (pDiagObjCurveSigGen1 != nullptr) {
                strResultValueSigGen1 = "Curve-SigGen1 added to diagram";
            }
            CDiagObjCurve* pDiagObjCurveSigGen2 = dynamic_cast<CDiagObjCurve*>(m_pWdgtDiagram->findDiagObj(CDiagObjCurve::ClassName(), "Curve-SigGen2"));
            if (pDiagObjCurveSigGen2 != nullptr) {
                strResultValueSigGen2 = "Curve-SigGen2 added to diagram";
            }
        }
        strlstResultValues.append(strResultValueSigGen1);
        strlstResultValues.append(strResultValueSigGen2);
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenAddCurves

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveCurves( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("Curve-SigGen1 removed from diagram");
    strlstExpectedValues.append("Curve-SigGen2 removed from diagram");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pDiagObjCurveSigGen2 != nullptr )
    {
        m_pWdgtDiagram->removeDiagObj(m_pDiagObjCurveSigGen2);
        delete m_pDiagObjCurveSigGen2;
        m_pDiagObjCurveSigGen2 = nullptr;
    }

    if( m_pDiagObjCurveSigGen1 != nullptr )
    {
        m_pWdgtDiagram->removeDiagObj(m_pDiagObjCurveSigGen1);
        delete m_pDiagObjCurveSigGen1;
        m_pDiagObjCurveSigGen1 = nullptr;
    }

    if( m_pWdgtDiagram != nullptr )
    {
        m_pWdgtDiagram->update();
    }

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        QString strResultValueSigGen1 = "Curve-SigGen1 not removed from diagram";
        QString strResultValueSigGen2 = "Curve-SigGen2 not removed from diagram";
        if (m_pWdgtDiagram != nullptr) {
            CDiagObj* pDiagObjCurveSigGen1 = m_pWdgtDiagram->findDiagObj(CDiagObjCurve::ClassName(), "Curve-SigGen1");
            if (pDiagObjCurveSigGen1 == nullptr) {
                strResultValueSigGen1 = "Curve-SigGen1 removed from diagram";
            }
            CDiagObj* pDiagObjCurveSigGen2 = m_pWdgtDiagram->findDiagObj(CDiagObjCurve::ClassName(), "Curve-SigGen2");
            if (pDiagObjCurveSigGen2 == nullptr) {
                strResultValueSigGen2 = "Curve-SigGen2 removed from diagram";
            }
        }
        strlstResultValues.append(strResultValueSigGen1);
        strlstResultValues.append(strResultValueSigGen2);
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenRemoveCurves

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddMarkers( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("Marker-SigGen1 added to diagram");
    strlstExpectedValues.append("Marker-SigGen2 added to diagram");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pDiagTraceSigGen1 != nullptr )
    {
        QColor col(255, 0, 0, 127);

        if( m_pDiagObjCurveSigGen1 != nullptr )
        {
            col = m_pDiagObjCurveSigGen1->getCol();
        }

        m_pDiagObjMarkerSigGen1 = new CDiagObjMarker(
            /* strObjName */ "Marker-SigGen1",
            /* pDiagTrace */ m_pDiagTraceSigGen1 );

        SLineStyle* pLineStyle = new SLineStyle(col, Qt::DotLine, 1);
        m_pDiagObjMarkerSigGen1->setLineStyleVer(EDiagObjStateCount, pLineStyle);
        pLineStyle = nullptr; //lint !e423
        m_pDiagObjMarkerSigGen1->showElement(EDiagObjStateCount, CDiagObjMarker::EElementLineVer);

        CToolTipStyle* pToolTipStyle = new CToolTipStyle(
            /* colFg         */ Qt::black,
            /* colBg         */ col,
            /* fnt           */ QFont(),
            /* cxOffs        */ 8,
            /* cyOffs        */ 8,
            /* pFrameStyle   */ nullptr,
            /* iMarginTop    */ 0,
            /* iMarginBottom */ 0,
            /* iMarginLeft   */ 0,
            /* iMarginRight  */ 0,
            /* cxArrowWidth  */ 6 );
        m_pDiagObjMarkerSigGen1->setToolTipStyle(EDiagObjStateCount, pToolTipStyle);
        pToolTipStyle = nullptr;
        m_pDiagObjMarkerSigGen1->showElement(EDiagObjStateCount, CDiagObjMarker::EElementToolTip);

        CImageStyleHCursorFocused* pImageStyleHCursorFocused = new CImageStyleHCursorFocused();
        m_pDiagObjMarkerSigGen1->setImageStyleCursor(EDiagObjStateFocused, pImageStyleHCursorFocused);
        pImageStyleHCursorFocused = nullptr; //lint !e423
        m_pDiagObjMarkerSigGen1->showElement(EDiagObjStateFocused, CDiagObjMarker::EElementCursor);

        CImageStyleHCursorEditing* pImageStyleHCursorEditing = new CImageStyleHCursorEditing();
        m_pDiagObjMarkerSigGen1->setImageStyleCursor(EDiagObjStateEditing, pImageStyleHCursorEditing);
        pImageStyleHCursorEditing = nullptr; //lint !e423
        m_pDiagObjMarkerSigGen1->showElement(EDiagObjStateEditing, CDiagObjMarker::EElementCursor);

        m_pWdgtDiagram->addDiagObj(m_pDiagObjMarkerSigGen1);

    } // if( m_pDiagTraceSigGen1 != nullptr )

    if( m_pDiagTraceSigGen2 != nullptr )
    {
        QColor col(255, 0, 0, 127);

        if( m_pDiagObjCurveSigGen2 != nullptr )
        {
            col = m_pDiagObjCurveSigGen2->getCol();
        }

        m_pDiagObjMarkerSigGen2 = new CDiagObjMarker(
            /* strObjName */ "Marker-SigGen2",
            /* pDiagTrace */ m_pDiagTraceSigGen2 );

        SLineStyle* pLineStyle = new SLineStyle(col, Qt::DotLine, 1);
        m_pDiagObjMarkerSigGen2->setLineStyleVer(EDiagObjStateCount, pLineStyle);
        pLineStyle = nullptr;
        m_pDiagObjMarkerSigGen2->showElement(EDiagObjStateCount, CDiagObjMarker::EElementLineVer);

        CToolTipStyle* pToolTipStyle = new CToolTipStyle(
            /* colFg         */ Qt::black,
            /* colBg         */ col,
            /* fnt           */ QFont(),
            /* cxOffs        */ 8,
            /* cyOffs        */ 8,
            /* pFrameStyle   */ nullptr,
            /* iMarginTop    */ 0,
            /* iMarginBottom */ 0,
            /* iMarginLeft   */ 0,
            /* iMarginRight  */ 0,
            /* cxArrowWidth  */ 6 );
        m_pDiagObjMarkerSigGen2->setToolTipStyle(EDiagObjStateCount, pToolTipStyle);
        pToolTipStyle = nullptr;
        m_pDiagObjMarkerSigGen2->showElement(EDiagObjStateCount, CDiagObjMarker::EElementToolTip);

        CImageStyleHCursorFocused* pImageStyleHCursorFocused = new CImageStyleHCursorFocused();
        m_pDiagObjMarkerSigGen2->setImageStyleCursor(EDiagObjStateFocused, pImageStyleHCursorFocused);
        pImageStyleHCursorFocused = nullptr; //lint !e423
        m_pDiagObjMarkerSigGen2->showElement(EDiagObjStateFocused, CDiagObjMarker::EElementCursor);

        CImageStyleHCursorEditing* pImageStyleHCursorEditing = new CImageStyleHCursorEditing();
        m_pDiagObjMarkerSigGen2->setImageStyleCursor(EDiagObjStateEditing, pImageStyleHCursorEditing);
        pImageStyleHCursorEditing = nullptr; //lint !e423
        m_pDiagObjMarkerSigGen2->showElement(EDiagObjStateEditing, CDiagObjMarker::EElementCursor);

        m_pWdgtDiagram->addDiagObj(m_pDiagObjMarkerSigGen2);

    } // if( m_pDiagTraceSigGen2 != nullptr )

    if( m_pWdgtDiagram != nullptr )
    {
        m_pWdgtDiagram->update();
    }

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        QString strResultValueSigGen1 = "Marker-SigGen1 not added to diagram";
        QString strResultValueSigGen2 = "Marker-SigGen2 not added to diagram";
        if (m_pWdgtDiagram != nullptr) {
            CDiagObjMarker* pDiagObjMarkerSigGen1 = dynamic_cast<CDiagObjMarker*>(m_pWdgtDiagram->findDiagObj(CDiagObjMarker::ClassName(), "Marker-SigGen1"));
            if (pDiagObjMarkerSigGen1 != nullptr) {
                strResultValueSigGen1 = "Marker-SigGen1 added to diagram";
            }
            CDiagObjMarker* pDiagObjMarkerSigGen2 = dynamic_cast<CDiagObjMarker*>(m_pWdgtDiagram->findDiagObj(CDiagObjMarker::ClassName(), "Marker-SigGen2"));
            if (pDiagObjMarkerSigGen2 != nullptr) {
                strResultValueSigGen2 = "Marker-SigGen2 added to diagram";
            }
        }
        strlstResultValues.append(strResultValueSigGen1);
        strlstResultValues.append(strResultValueSigGen2);
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenAddMarkers

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenMoveMarkers( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    CPhysVal physValXMarkerSigGen1;
    CPhysVal physValXMarkerSigGen2;

    if (m_pDiagScaleX != nullptr)
    {
        physValXMarkerSigGen1 = m_pDiagScaleX->getScale().minVal() + m_pDiagScaleX->getScale().rangeVal() / 4.0;
        physValXMarkerSigGen2 = m_pDiagScaleX->getScale().minVal() + m_pDiagScaleX->getScale().rangeVal() * (3.0/4.0);
    }

    strlstExpectedValues.append(physValXMarkerSigGen1.toString());
    strlstExpectedValues.append(physValXMarkerSigGen2.toString());

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if (m_pDiagObjMarkerSigGen1 != nullptr)
    {
        m_pDiagObjMarkerSigGen1->setVal(EScaleAxis::X, physValXMarkerSigGen1);
    }
    if (m_pDiagObjMarkerSigGen2 != nullptr)
    {
        m_pDiagObjMarkerSigGen2->setVal(EScaleAxis::X, physValXMarkerSigGen2);
    }
    if (m_pWdgtDiagram != nullptr)
    {
        m_pWdgtDiagram->update();
    }

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        physValXMarkerSigGen1 = CPhysVal();
        physValXMarkerSigGen2 = CPhysVal();

        if (m_pDiagObjMarkerSigGen1 != nullptr)
        {
            physValXMarkerSigGen1 = m_pDiagObjMarkerSigGen1->getVal(EScaleAxis::X);
        }
        if (m_pDiagObjMarkerSigGen2 != nullptr)
        {
            physValXMarkerSigGen2 = m_pDiagObjMarkerSigGen2->getVal(EScaleAxis::X);
        }
        strlstResultValues.append(physValXMarkerSigGen1.toString());
        strlstResultValues.append(physValXMarkerSigGen2.toString());
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenMoveMarkers

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveMarkers( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("Marker-SigGen1 removed from diagram");
    strlstExpectedValues.append("Marker-SigGen2 removed from diagram");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pDiagObjMarkerSigGen2 != nullptr )
    {
        m_pWdgtDiagram->removeDiagObj(m_pDiagObjMarkerSigGen2);
        delete m_pDiagObjMarkerSigGen2;
        m_pDiagObjMarkerSigGen2 = nullptr;
    }

    if( m_pDiagObjMarkerSigGen1 != nullptr )
    {
        m_pWdgtDiagram->removeDiagObj(m_pDiagObjMarkerSigGen1);
        delete m_pDiagObjMarkerSigGen1;
        m_pDiagObjMarkerSigGen1 = nullptr;
    }

    if( m_pWdgtDiagram != nullptr )
    {
        m_pWdgtDiagram->update();
    }

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        QString strResultValueSigGen1 = "Marker-SigGen1 not removed from diagram";
        QString strResultValueSigGen2 = "Marker-SigGen2 not removed from diagram";
        if (m_pWdgtDiagram != nullptr) {
            CDiagObj* pDiagObjMarkerSigGen1 = m_pWdgtDiagram->findDiagObj(CDiagObjMarker::ClassName(), "Marker-SigGen1");
            if (pDiagObjMarkerSigGen1 == nullptr) {
                strResultValueSigGen1 = "Marker-SigGen1 removed from diagram";
            }
            CDiagObj* pDiagObjMarkerSigGen2 = m_pWdgtDiagram->findDiagObj(CDiagObjMarker::ClassName(), "Marker-SigGen2");
            if (pDiagObjMarkerSigGen2 == nullptr) {
                strResultValueSigGen2 = "Marker-SigGen2 removed from diagram";
            }
        }
        strlstResultValues.append(strResultValueSigGen1);
        strlstResultValues.append(strResultValueSigGen2);
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenRemoveMarkers

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenSingleShot( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("Test Step not completely implemented");
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    onTimerSigGenTimeout();

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues.append("Test Step not completely implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenSingleShot

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenStart( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("Test Step not completely implemented");
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    m_pTimerSigGen = new QTimer(this);
    m_pTimerSigGen->start(10);

    if( !connect(
        /* pObjSender   */ m_pTimerSigGen,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimerSigGenTimeout()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues.append("Test Step not completely implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenStart

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenStop( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append("Test Step not completely implemented");
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pTimerSigGen != nullptr )
    {
        m_pTimerSigGen->stop();
    }

    try
    {
        delete m_pTimerSigGen;
    }
    catch(...)
    {
    }
    m_pTimerSigGen = nullptr;

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues.append("Test Step not completely implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenStop

/*==============================================================================
private slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::onTimerSigGenTimeout()
//------------------------------------------------------------------------------
{
    #if 1

    const  double c_fRadDrift = Math::c_fPI/100.0;
    static double s_fRadDrift = 0.0;
    const  int    c_iValCount = 1001;

    static QVector<double> s_arfXValuesTraces01(c_iValCount);
    static QVector<double> s_arfYValuesTrace0(c_iValCount);
    static QVector<double> s_arfYValuesTrace1(c_iValCount);

    if( m_pDiagScaleX != nullptr && m_pDiagScaleY != nullptr )
    {
        double fxRange = m_scaleX.rangeVal().getVal();
        double fyRange = fabs(m_scaleY.rangeVal().getVal());
        double fT = 0.5 * fxRange;
        double fxStep = fxRange / (c_iValCount-1);

        s_fRadDrift += c_fRadDrift;

        if( s_fRadDrift > 2.0*Math::c_fPI )
        {
            s_fRadDrift = 0.0;
        }

        for( int idxVal = 0; idxVal < c_iValCount; idxVal++ )
        {
            double fx = m_scaleX.minVal().getVal() + idxVal*fxStep;
            s_arfXValuesTraces01[idxVal] = fx;

            double fRad = s_fRadDrift + 2.0*Math::c_fPI*(fx - m_scaleX.minVal().getVal()) / fT;
            double fy = m_scaleY.minVal().getVal() + fyRange/2.0 + sin(fRad)*fyRange/5.0;

            // Simulate sinus with a little bit signal noise.
            // Random factor in the range of [-0.5 .. 0.5].
            double fRandFacTrace0 = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX) + 1.0) - 0.5;
            double fRandFacTrace1 = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX) + 1.0) - 0.5;

            // Signal1 noise 1% of yRange.
            s_arfYValuesTrace0[idxVal] = fy + (fRandFacTrace0 / 100.0) * fyRange;

            // Signal2 noise 10% of yRange.
            s_arfYValuesTrace1[idxVal] = fy + (fRandFacTrace1 / 10.0) * fyRange;
        }

        if( m_pDiagTraceSigGen1 != nullptr )
        {
            m_pDiagTraceSigGen1->setValues(EScaleAxis::X, s_arfXValuesTraces01, &m_scaleX.unit());
            m_pDiagTraceSigGen1->setValues(EScaleAxis::Y, s_arfYValuesTrace0, &m_scaleY.unit());
        }

        if( m_pDiagTraceSigGen2 != nullptr )
        {
            m_pDiagTraceSigGen2->setValues(EScaleAxis::X, s_arfXValuesTraces01, &m_scaleX.unit());
            m_pDiagTraceSigGen2->setValues(EScaleAxis::Y, s_arfYValuesTrace1, &m_scaleY.unit());
        }
    } // if( m_pDiagScaleX != nullptr && m_pDiagScaleY != nullptr )

    #else

    const int c_iValCount = 2;

    static QVector<double> s_arfXValuesTraces01(c_iValCount);
    static QVector<double> s_arfYValuesTrace0(c_iValCount);

    if( m_pDiagScaleX != nullptr && m_pDiagScaleY != nullptr )
    {
        double fxRange = m_scaleX.rangeVal().getVal();
        double fyRange = fabs(m_scaleY.rangeVal().getVal());

        double fx;
        double fy;

        fx = m_scaleX.minVal().getVal() + fxRange/5.0;
        s_arfXValuesTraces01[0] = fx;
        fx = m_scaleX.minVal().getVal() + 4.0*fxRange/5.0;
        s_arfXValuesTraces01[1] = fx;

        fy = m_scaleY.minVal().getVal() + fyRange/2.0;
        s_arfYValuesTrace0[0] = fy;
        s_arfYValuesTrace0[1] = fy;

        if( m_pDiagTraceSigGen1 != nullptr )
        {
            m_pDiagTraceSigGen1->setValues(EScaleAxis::X, s_arfXValuesTraces01, &m_scaleX.unit());
            m_pDiagTraceSigGen1->setValues(EScaleAxis::Y, s_arfYValuesTrace0, &m_scaleY.unit());
        }
    }
    #endif

    if( m_pWdgtDiagram != nullptr )
    {
        m_pWdgtDiagram->update();
    }

} // onTimerSigGenTimeout()

/*==============================================================================
private: // slots
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::onTimerTestStepTimeout()
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStep* pTestStep = getCurrentTestStep();

    if( pTestStep != nullptr )
    {
        QString     strResultValue;
        QStringList strlstResultValues;

        strResultValue = "Test step not finished in time";
        strlstResultValues.append(strResultValue);

        pTestStep->setResultValues(strlstResultValues);
    }
}
