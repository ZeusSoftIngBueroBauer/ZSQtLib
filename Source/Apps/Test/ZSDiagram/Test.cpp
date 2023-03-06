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
#include "ZSDiagram/ZSDiagScale.h"
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
    ZS::Test::CTest("ZSDiagram"),
    m_pTmrTestStepTimeout(nullptr),
    m_pTimerSigGen(nullptr),
    m_pFrameDiagram(nullptr),
    m_pWdgtDiagram(nullptr),
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
    m_pFrameDiagram = nullptr;
    m_pWdgtDiagram = nullptr;
    m_pTimerSigGen = nullptr;
    m_pFrameDiagram = nullptr;
    m_pWdgtDiagram = nullptr;
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

    strlstExpectedValues.append("Diagram Window Visible");
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    m_pFrameDiagram = new QFrame();
    m_pFrameDiagram->setFrameShape(QFrame::Panel);
    m_pFrameDiagram->setFrameShadow(QFrame::Raised);
    QVBoxLayout* pLytFrameDiagram = new QVBoxLayout();
    //pLytFrameDiagram->setContentsMargins(0,0,0,0); // left,tip,right,bottom
    m_pFrameDiagram->setLayout(pLytFrameDiagram);
    CWidgetCentral::GetInstance()->getTestOutputWidget()->layout()->addWidget(m_pFrameDiagram);

    m_pWdgtDiagram = new CWdgtDiagram("Diagram", m_pFrameDiagram);
    m_pWdgtDiagram->setMouseTracking(true);
    m_pWdgtDiagram->enableZooming();
    //m_pWdgtDiagram->setMarginTop(0);
    //m_pWdgtDiagram->setMarginBottom(0);
    //m_pWdgtDiagram->setMarginLeft(0);
    //m_pWdgtDiagram->setMarginRight(0);
    m_pWdgtDiagram->setMinimumHeightPartTop(0);
    m_pWdgtDiagram->setMinimumHeightPartBottom(0);
    m_pWdgtDiagram->setMinimumWidthPartLeft(0);
    m_pWdgtDiagram->setMinimumWidthPartRight(0);

    SFrameStyle3DSunken* pFrameStyleDiagram = new SFrameStyle3DSunken;
    m_pWdgtDiagram->setFrameStyle(pFrameStyleDiagram);
    pFrameStyleDiagram = nullptr;

    //SFrameStyle* pFrameStyleDiagramPartCenter = new SFrameStyle(1,Qt::red);
    //m_pWdgtDiagram->setFrameStylePartCenter(pFrameStyleDiagramPartCenter);
    //pFrameStyleDiagramPartCenter = nullptr;

    m_pWdgtDiagram->setMinimumWidth(400);
    m_pWdgtDiagram->setMinimumHeight(200);

    m_pFrameDiagram->layout()->addWidget(m_pWdgtDiagram);

    // Actual Result Values
    //---------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues.append("Test Step not completely implemented");
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

    strlstExpectedValues.append("Diagram Window Destroyed");
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    delete m_pWdgtDiagram;
    m_pWdgtDiagram = nullptr;

    delete m_pFrameDiagram;
    m_pFrameDiagram = nullptr;

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues.append("Test Step not completely implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenDestroyDiagram

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddScales( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pWdgtDiagram != nullptr )
    {
        ZS::Diagram::SScale scaleX(
            /* fMin  */ 899.0e6,
            /* fMax  */ 901.0e6,
            /* pUnit */ Frequency.Hertz );

        m_pDiagScaleX = new CDiagScale(
            /* strObjName */ "DiagScaleX",
            /* scaleDir   */ EScaleDirX,
            /* scale      */ scaleX );
        m_pDiagScaleX->setDivLineDistMinPix(EDivLineLayerMain, 20);

        m_pWdgtDiagram->addDiagScale(m_pDiagScaleX);

        ZS::Diagram::SScale scaleY(
            /* fMin      */ 0.0,
            /* fMax      */ 40.0,
            /* pPhysUnit */ Power.dBm );

        m_pDiagScaleY = new CDiagScale(
            /* strObjName */ "DiagScaleY",
            /* scaleDir   */ EScaleDirY,
            /* scale      */ scaleY );
        m_pDiagScaleY->setDivLineDistMinPix(EDivLineLayerMain, 20);

        m_pWdgtDiagram->addDiagScale(m_pDiagScaleY);

    } // if( m_pWdgtDiagram != nullptr )

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues.append("Test Step not completely implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenAddScales

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveScales( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pDiagScaleX != nullptr )
    {
        m_pWdgtDiagram->removeDiagScale(m_pDiagScaleX);
        delete m_pDiagScaleX;
        m_pDiagScaleX = nullptr;
    }

    if( m_pDiagScaleX != nullptr )
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
        strlstResultValues.append("Test Step not completely implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenRemoveScales

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddTraces( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pWdgtDiagram != nullptr && m_pDiagScaleX != nullptr && m_pDiagScaleY != nullptr )
    {
        m_pDiagTraceSigGen1 = new CDiagTrace(
            /* strObjName  */ "DiagTraceSigGen1",
            /* pDiagScaleX */ m_pDiagScaleX,
            /* pDiagScaleY */ m_pDiagScaleY );
        m_pWdgtDiagram->addDiagTrace(m_pDiagTraceSigGen1);

        m_pDiagTraceSigGen2 = new CDiagTrace(
            /* strObjName  */ "DiagTraceSigGen2",
            /* pDiagScaleX */ m_pDiagScaleX,
            /* pDiagScaleY */ m_pDiagScaleY );
        m_pWdgtDiagram->addDiagTrace(m_pDiagTraceSigGen2);

    } // if( m_pWdgtDiagram != nullptr && m_pDiagScaleX != nullptr && m_pDiagScaleY != nullptr )

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues.append("Test Step not completely implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenAddTraces

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveTraces( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

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
        strlstResultValues.append("Test Step not completely implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenRemoveTraces

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddGrid( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pWdgtDiagram != nullptr && m_pDiagScaleX != nullptr && m_pDiagScaleY != nullptr )
    {
        m_pDiagObjGrid = new CDiagObjGrid(
            /* strObjName  */ "DiagObjGrid",
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
        strlstResultValues.append("Test Step not completely implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenAddGrid

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveGrid( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

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
        strlstResultValues.append("Test Step not completely implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenRemoveGrid

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddAxisLabels( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pWdgtDiagram != nullptr && m_pDiagScaleX != nullptr )
    {
        m_pDiagObjAxisLabelX = new CDiagObjAxisLabel(
            /* strObjName  */ "DiagObjAxisLabelX",
            /* pDiagScaleX */ m_pDiagScaleX,
            /* layoutPos   */ ELayoutPosBottom );
        m_pWdgtDiagram->addDiagObj(m_pDiagObjAxisLabelX);
    }

    if( m_pWdgtDiagram != nullptr && m_pDiagScaleY != nullptr )
    {
        m_pDiagObjAxisLabelY = new CDiagObjAxisLabel(
            /* strObjName  */ "DiagObjAxisLabelY",
            /* pDiagScaleX */ m_pDiagScaleY,
            /* layoutPos   */ ELayoutPosLeft );
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
        strlstResultValues.append("Test Step not completely implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenAddAxisLabels

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveAxisLabels( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

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
        strlstResultValues.append("Test Step not completely implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenRemoveAxisLabels

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddCurves( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

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
            /* strObjName */ "DiagObjCurve0",
            /* pDiagTrace */ m_pDiagTraceSigGen1 );
        m_pDiagObjCurveSigGen1->setCol(c_colYellow);
        m_pWdgtDiagram->addDiagObj(m_pDiagObjCurveSigGen1);
    }

    if( m_pWdgtDiagram != nullptr && m_pDiagTraceSigGen2 != nullptr )
    {
        m_pDiagObjCurveSigGen2 = new CDiagObjCurve(
            /* strObjName */ "DiagObjCurve1",
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
        strlstResultValues.append("Test Step not completely implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenAddCurves

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveCurves( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

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
        strlstResultValues.append("Test Step not completely implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenRemoveCurves

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddMarkers( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

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
            /* strObjName */ "DiagObjMarker0",
            /* pDiagTrace */ m_pDiagTraceSigGen1 );

        SLineStyle* pLineStyle = new SLineStyle(col, Qt::DotLine, 1);
        m_pDiagObjMarkerSigGen1->setLineStyleVer(EDiagObjStateCount, pLineStyle);
        pLineStyle = nullptr; //lint !e423
        m_pDiagObjMarkerSigGen1->showElement(EDiagObjStateCount, CDiagObjMarker::EElementLineVer);

        CToolTipStyle* pToolTipStyle = new CToolTipStyle(
            /* colFg         */ Qt::white,
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
            /* strObjName */ "DiagObjMarker1",
            /* pDiagTrace */ m_pDiagTraceSigGen2 );

        SLineStyle* pLineStyle = new SLineStyle(col, Qt::DotLine, 1);
        m_pDiagObjMarkerSigGen2->setLineStyleVer(EDiagObjStateCount, pLineStyle);
        pLineStyle = nullptr;
        m_pDiagObjMarkerSigGen2->showElement(EDiagObjStateCount, CDiagObjMarker::EElementLineVer);

        CToolTipStyle* pToolTipStyle = new CToolTipStyle(
            /* colFg         */ Qt::white,
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
        strlstResultValues.append("Test Step not completely implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenAddMarkers

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveMarkers( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

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
        strlstResultValues.append("Test Step not implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenRemoveMarkers

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenSingleShot( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    onTimerSigGenTimeout();

    // Actual Result Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues.append("Test Step not implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenSingleShot

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenStart( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

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
        strlstResultValues.append("Test Step not implemented");
    }
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepSigGenStart

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenStop( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

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
        strlstResultValues.append("Test Step not implemented");
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
    const  double c_fRadDrift = Math::c_fPI/100.0;
    static double s_fRadDrift = 0.0;
    const  int    c_iValCount = 1001;

    static QVector<double> s_arfXValuesTraces01(c_iValCount);
    static QVector<double> s_arfYValuesTrace0(c_iValCount);
    static QVector<double> s_arfYValuesTrace1(c_iValCount);

    if( m_pDiagScaleX != nullptr && m_pDiagScaleY != nullptr )
    {
        ZS::Diagram::SScale scaleX = m_pDiagScaleX->getScale();
        ZS::Diagram::SScale scaleY = m_pDiagScaleY->getScale();

        double fxRange = scaleX.m_fMax - scaleX.m_fMin;
        double fyRange = fabs(scaleY.m_fMax - scaleY.m_fMin);
        double fT = 0.5 * fxRange;
        double fxStep = fxRange / (c_iValCount-1);

        s_fRadDrift += c_fRadDrift;

        if( s_fRadDrift > 2.0*Math::c_fPI )
        {
            s_fRadDrift = 0.0;
        }

        for( int idxVal = 0; idxVal < c_iValCount; idxVal++ )
        {
            double fx = scaleX.m_fMin + idxVal*fxStep;
            s_arfXValuesTraces01[idxVal] = fx;

            double fRad = s_fRadDrift + 2.0*Math::c_fPI*(fx - scaleX.m_fMin) / fT;
            double fy = scaleY.m_fMin + fyRange/2.0 + sin(fRad)*fyRange/5.0;

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
            m_pDiagTraceSigGen1->setValues(EScaleDirX, s_arfXValuesTraces01);
            m_pDiagTraceSigGen1->setValues(EScaleDirY, s_arfYValuesTrace0);
        }

        if( m_pDiagTraceSigGen2 != nullptr )
        {
            m_pDiagTraceSigGen2->setValues(EScaleDirX, s_arfXValuesTraces01);
            m_pDiagTraceSigGen2->setValues(EScaleDirY, s_arfYValuesTrace1);
        }
    } // if( m_pDiagScaleX != nullptr && m_pDiagScaleY != nullptr )

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

    } // if( pTestStep != nullptr )

} // onTimerTestStepTimeout()