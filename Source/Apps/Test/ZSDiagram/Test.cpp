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

#include <QtCore/qfile.h>
#include <QtCore/qtimer.h>
#include <QtNetwork/qhostinfo.h>

#if QT_VERSION < 0x050000
#include <QtGui/qframe.h>
#include <QtGui/qlayout.h>
#else
#include <QtWidgets/qframe.h>
#include <QtWidgets/qlayout.h>
#endif

#include "Test.h"
#include "App.h"
#include "WidgetCentral.h"
#include "WdgtTestOutput.h"

#include "ZSDiagram/ZSDiagram.h"
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
#include "ZSPhysSizes/Electricity/ZSPhysSizes.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSPhysSizes/Kinematics/ZSPhysSizes.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::Electricity;
using namespace ZS::PhysVal::Geometry;
using namespace ZS::PhysVal::Kinematics;
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
CTest::CTest( const QString& i_strTestStepsFileName ) :
//------------------------------------------------------------------------------
    ZS::Test::CTest("ZS::Diagram", i_strTestStepsFileName),
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

    #if 0

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
        /* strName         */ "Step " + QString::number(++idxStep) + " Set Values",
        /* strOperation    */ "CDiagTrace::setValues()",
        /* pTSGrpParent    */ pGrpSigGen,
        /* szDoTestStepFct */ SLOT(doTestStepSigGenSetValues(ZS::Test::CTestStep*)) );

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

    #endif // #if 0

    // Test Group: Spectral Barriers
    //--------------------------------------

    m_mapSBarCols["TE1"] = Qt::red;
    m_mapSBarCols["TE2"] = Qt::green;
    m_mapSBarCols["TE3"] = Qt::yellow;
    m_mapSBarCols["TE4"] = Qt::cyan;

    m_mapSBarLabelsPosOffsY["TE1"] = 30;
    m_mapSBarLabelsPosOffsY["TE2"] = 50;
    m_mapSBarLabelsPosOffsY["TE3"] = 70;
    m_mapSBarLabelsPosOffsY["TE4"] = 90;

    ZS::Test::CTestStepGroup* pGrpSBar = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Spectral Barriers",
        /* pTSGrpParent    */ nullptr );

    // Test Group: Spectral Barriers - Group 1
    //----------------------------------------

    /*
    Isolation Table:
    ----------------

         |  Clm
     Row |   1    |   2    |   3    |   4    |
    =====+========+========+========+========+
      1  |*  0.0 *|  25.0  |  25.0  |  25.0  |
    -----+--------+--------+--------+--------+
      2  | =(1,2) |*  0.0 *|  25.0  |  25.0  |
    -----+--------+--------+--------+--------+
      3  | =(1,3) | =(2,3) |*  0.0 *|  25.0  |
    -----+--------+--------+--------+--------+
      4  | =(1,4) | =(2,4) | =(3,4) |*  0.0 *|
    =====+========+========+========+========+
    */

    ZS::Test::CTestStepGroup* pGrpSBar1 = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Spectral Barriers - 1",
        /* pTSGrpParent    */ pGrpSBar );

    ZS::Test::CTestStepGroup* pGrpSBar11 = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Spectral Barriers - 1.1",
        /* pTSGrpParent    */ pGrpSBar1 );

    pGrpSBar11->setToolTip("Request 20 dB dynamic, parallel execution possible");

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Create Diagram",
        /* strOperation    */ "new CDiagram",
        /* pTSGrpParent    */ pGrpSBar11,
        /* szDoTestStepFct */ SLOT(doTestStepSBarCreateDiagram(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:ITAB:DEF 4, 25.0",
        /* strOperation    */ "ITAB:DEF 4, 25.0",
        /* pTSGrpParent    */ pGrpSBar11,
        /* szDoTestStepFct */ SLOT(doTestStepSBarITabDef(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE1, 900.0e6, 200.0e3, 33.0, 20.0, 1",
        /* strOperation    */ "TE1:LOCK 900.0e6, 200.0e3, 33.0, 20.0, 1",
        /* pTSGrpParent    */ pGrpSBar11,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE2, 900.0e6, 200.0e3, 33.0, 20.0, 2",
        /* strOperation    */ "TE2:LOCK 900.0e6, 200.0e3, 33.0, 20.0, 2",
        /* pTSGrpParent    */ pGrpSBar11,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE1 with TE2",
        /* strOperation    */ "TE1 -> TE2",
        /* pTSGrpParent    */ pGrpSBar11,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE2 with TE1",
        /* strOperation    */ "TE2 -> TE1",
        /* pTSGrpParent    */ pGrpSBar11,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Remove Interference Checks",
        /* strOperation    */ "",
        /* pTSGrpParent    */ pGrpSBar11,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:UNLock TE1",
        /* strOperation    */ "TE1:UNLock",
        /* pTSGrpParent    */ pGrpSBar11,
        /* szDoTestStepFct */ SLOT(doTestStepSBarUnlock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:UNLock TE2",
        /* strOperation    */ "TE2:UNLock",
        /* pTSGrpParent    */ pGrpSBar11,
        /* szDoTestStepFct */ SLOT(doTestStepSBarUnlock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Destroy Diagram",
        /* strOperation    */ "delete CDiagram",
        /* pTSGrpParent    */ pGrpSBar11,
        /* szDoTestStepFct */ SLOT(doTestStepSBarDestroyDiagram(ZS::Test::CTestStep*)) );

    ZS::Test::CTestStepGroup* pGrpSBar12 = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Spectral Barriers - 1.2",
        /* pTSGrpParent    */ pGrpSBar1 );

    pGrpSBar11->setToolTip("Request 30 dB dynamic for second TE, 2nd TE should block");

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Create Diagram",
        /* strOperation    */ "new CDiagram",
        /* pTSGrpParent    */ pGrpSBar12,
        /* szDoTestStepFct */ SLOT(doTestStepSBarCreateDiagram(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:ITAB:DEF 4, 25.0",
        /* strOperation    */ "ITAB:DEF 4, 25.0",
        /* pTSGrpParent    */ pGrpSBar12,
        /* szDoTestStepFct */ SLOT(doTestStepSBarITabDef(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE1, 900.0e6, 200.0e3, 33.0, 20.0, 1",
        /* strOperation    */ "TE1:LOCK 900.0e6, 200.0e3, 33.0, 20.0, 1",
        /* pTSGrpParent    */ pGrpSBar12,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE2, 900.0e6, 200.0e3, 33.0, 30.0, 2",
        /* strOperation    */ "TE2:LOCK 900.0e6, 200.0e3, 33.0, 30.0, 2",
        /* pTSGrpParent    */ pGrpSBar12,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE1 with TE2",
        /* strOperation    */ "TE1 -> TE2",
        /* pTSGrpParent    */ pGrpSBar12,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE2 with TE1",
        /* strOperation    */ "TE2 -> TE1",
        /* pTSGrpParent    */ pGrpSBar12,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Remove Interference Checks",
        /* strOperation    */ "",
        /* pTSGrpParent    */ pGrpSBar12,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:UNLock TE1",
        /* strOperation    */ "TE1:UNLock",
        /* pTSGrpParent    */ pGrpSBar12,
        /* szDoTestStepFct */ SLOT(doTestStepSBarUnlock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:UNLock TE2",
        /* strOperation    */ "TE2:UNLock",
        /* pTSGrpParent    */ pGrpSBar12,
        /* szDoTestStepFct */ SLOT(doTestStepSBarUnlock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Destroy Diagram",
        /* strOperation    */ "delete CDiagram",
        /* pTSGrpParent    */ pGrpSBar12,
        /* szDoTestStepFct */ SLOT(doTestStepSBarDestroyDiagram(ZS::Test::CTestStep*)) );

    ZS::Test::CTestStepGroup* pGrpSBar13 = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Spectral Barriers - 1.3",
        /* pTSGrpParent    */ pGrpSBar1 );

    pGrpSBar11->setToolTip("Request 30 dB dynamic for first TE, 2nd TE should block");

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Create Diagram",
        /* strOperation    */ "new CDiagram",
        /* pTSGrpParent    */ pGrpSBar13,
        /* szDoTestStepFct */ SLOT(doTestStepSBarCreateDiagram(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:ITAB:DEF 4, 25.0",
        /* strOperation    */ "ITAB:DEF 4, 25.0",
        /* pTSGrpParent    */ pGrpSBar13,
        /* szDoTestStepFct */ SLOT(doTestStepSBarITabDef(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE1, 900.0e6, 200.0e3, 33.0, 30.0, 1",
        /* strOperation    */ "TE1:LOCK 900.0e6, 200.0e3, 33.0, 30.0, 1",
        /* pTSGrpParent    */ pGrpSBar13,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE2, 900.0e6, 200.0e3, 33.0, 20.0, 2",
        /* strOperation    */ "TE2:LOCK 900.0e6, 200.0e3, 33.0, 20.0, 2",
        /* pTSGrpParent    */ pGrpSBar13,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE1 with TE2",
        /* strOperation    */ "TE1 -> TE2",
        /* pTSGrpParent    */ pGrpSBar13,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE2 with TE1",
        /* strOperation    */ "TE2 -> TE1",
        /* pTSGrpParent    */ pGrpSBar13,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Remove Interference Checks",
        /* strOperation    */ "",
        /* pTSGrpParent    */ pGrpSBar13,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:UNLock TE1",
        /* strOperation    */ "TE1:UNLock",
        /* pTSGrpParent    */ pGrpSBar13,
        /* szDoTestStepFct */ SLOT(doTestStepSBarUnlock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:UNLock TE2",
        /* strOperation    */ "TE2:UNLock",
        /* pTSGrpParent    */ pGrpSBar13,
        /* szDoTestStepFct */ SLOT(doTestStepSBarUnlock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Destroy Diagram",
        /* strOperation    */ "delete CDiagram",
        /* pTSGrpParent    */ pGrpSBar13,
        /* szDoTestStepFct */ SLOT(doTestStepSBarDestroyDiagram(ZS::Test::CTestStep*)) );

    ZS::Test::CTestStepGroup* pGrpSBar14 = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Spectral Barriers - 1.4",
        /* pTSGrpParent    */ pGrpSBar1 );

    pGrpSBar11->setToolTip("Request low level for 2nd TE, 2nd TE should block");

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Create Diagram",
        /* strOperation    */ "new CDiagram",
        /* pTSGrpParent    */ pGrpSBar14,
        /* szDoTestStepFct */ SLOT(doTestStepSBarCreateDiagram(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:ITAB:DEF 4, 25.0",
        /* strOperation    */ "ITAB:DEF 4, 25.0",
        /* pTSGrpParent    */ pGrpSBar14,
        /* szDoTestStepFct */ SLOT(doTestStepSBarITabDef(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE1, 900.0e6, 200.0e3, 33.0, 20.0, 1",
        /* strOperation    */ "TE1:LOCK 900.0e6, 200.0e3, 33.0, 20.0, 1",
        /* pTSGrpParent    */ pGrpSBar14,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE2, 900.0e6, 200.0e3, -20.0, 20.0, 2",
        /* strOperation    */ "TE2:LOCK 900.0e6, 200.0e3, -20.0, 20.0, 2",
        /* pTSGrpParent    */ pGrpSBar14,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE1 with TE2",
        /* strOperation    */ "TE1 -> TE2",
        /* pTSGrpParent    */ pGrpSBar14,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE2 with TE1",
        /* strOperation    */ "TE2 -> TE1",
        /* pTSGrpParent    */ pGrpSBar14,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Remove Interference Checks",
        /* strOperation    */ "",
        /* pTSGrpParent    */ pGrpSBar14,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:UNLock TE1",
        /* strOperation    */ "TE1:UNLock",
        /* pTSGrpParent    */ pGrpSBar14,
        /* szDoTestStepFct */ SLOT(doTestStepSBarUnlock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:UNLock TE2",
        /* strOperation    */ "TE2:UNLock",
        /* pTSGrpParent    */ pGrpSBar14,
        /* szDoTestStepFct */ SLOT(doTestStepSBarUnlock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Destroy Diagram",
        /* strOperation    */ "delete CDiagram",
        /* pTSGrpParent    */ pGrpSBar14,
        /* szDoTestStepFct */ SLOT(doTestStepSBarDestroyDiagram(ZS::Test::CTestStep*)) );

    ZS::Test::CTestStepGroup* pGrpSBar15 = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Spectral Barriers - 1.5",
        /* pTSGrpParent    */ pGrpSBar1 );

    pGrpSBar11->setToolTip("Request low level for 1st TE, 2nd TE should block to avoid override");

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Create Diagram",
        /* strOperation    */ "new CDiagram",
        /* pTSGrpParent    */ pGrpSBar15,
        /* szDoTestStepFct */ SLOT(doTestStepSBarCreateDiagram(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:ITAB:DEF 4, 25.0",
        /* strOperation    */ "ITAB:DEF 4, 25.0",
        /* pTSGrpParent    */ pGrpSBar15,
        /* szDoTestStepFct */ SLOT(doTestStepSBarITabDef(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE1, 900.0e6, 200.0e3, -20.0, 20.0, 1",
        /* strOperation    */ "TE1:LOCK 900.0e6, 200.0e3, -20.0, 20.0, 1",
        /* pTSGrpParent    */ pGrpSBar15,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE2, 900.0e6, 200.0e3, 33.0, 20.0, 2",
        /* strOperation    */ "TE2:LOCK 900.0e6, 200.0e3, 33.0, 20.0, 2",
        /* pTSGrpParent    */ pGrpSBar15,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE1 with TE2",
        /* strOperation    */ "TE1 -> TE2",
        /* pTSGrpParent    */ pGrpSBar15,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE2 with TE1",
        /* strOperation    */ "TE2 -> TE1",
        /* pTSGrpParent    */ pGrpSBar15,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Remove Interference Checks",
        /* strOperation    */ "",
        /* pTSGrpParent    */ pGrpSBar15,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:UNLock TE1",
        /* strOperation    */ "TE1:UNLock",
        /* pTSGrpParent    */ pGrpSBar15,
        /* szDoTestStepFct */ SLOT(doTestStepSBarUnlock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:UNLock TE2",
        /* strOperation    */ "TE2:UNLock",
        /* pTSGrpParent    */ pGrpSBar15,
        /* szDoTestStepFct */ SLOT(doTestStepSBarUnlock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Destroy Diagram",
        /* strOperation    */ "delete CDiagram",
        /* pTSGrpParent    */ pGrpSBar15,
        /* szDoTestStepFct */ SLOT(doTestStepSBarDestroyDiagram(ZS::Test::CTestStep*)) );

    // Test Group: Spectral Barriers - Group 2
    //----------------------------------------

    #if 0

    /*
    Isolation Table:
    ----------------

         |  Clm
     Row |   1    |   2    |   3    |   4    |
    =====+========+========+========+========+
      1  |*  0.0 *|  12.0  |  13.0  |  14.0  |
    -----+--------+--------+--------+--------+
      2  | =(1,2) |*  0.0 *|  23.0  |  24.0  |
    -----+--------+--------+--------+--------+
      3  | =(1,3) | =(2,3) |*  0.0 *|  34.0  |
    -----+--------+--------+--------+--------+
      4  | =(1,4) | =(2,4) | =(3,4) |*  0.0 *|
    =====+========+========+========+========+
    */

    ZS::Test::CTestStepGroup* pGrpSBar2 = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Spectral Barriers - 2",
        /* pTSGrpParent    */ pGrpSBar );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Create Diagram",
        /* strOperation    */ "new CDiagram",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarCreateDiagram(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:ITAB:DEF 4, 25.0",
        /* strOperation    */ "ITAB:DEF 4, xy",
        /* pTSGrpParent    */ pGrpSBar11,
        /* szDoTestStepFct */ SLOT(doTestStepSBarITabDef(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE1, 899900000.000000, 210.0e3, 33.0, 24.028359, 1",
        /* strOperation    */ "TE1:LOCK 899900000.000000, 210.0e3, 33.0, 24.028359, 1",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE2, 900000000.000000, 210.0e3, 33.0, 28.880704, 2",
        /* strOperation    */ "TE2:LOCK 900000000.000000, 210.0e3, 33.0, 28.880704, 2",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE1 with TE2",
        /* strOperation    */ "TE1 -> TE2",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE2 with TE1",
        /* strOperation    */ "TE2 -> TE1",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Remove Interference Checks",
        /* strOperation    */ "",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE3, 900000000.000000, 210.0e3, 33.0, 11.750525, 3",
        /* strOperation    */ "TE3:LOCK 900000000.000000, 210.0e3, 33.0, 11.750525, 3",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE1 with TE3",
        /* strOperation    */ "TE1 -> TE3",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE3 with TE1",
        /* strOperation    */ "TE3 -> TE1",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Remove Interference Checks",
        /* strOperation    */ "",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE4, 899700000.000000, 210.0e3, 33.0, 10.196126, 4",
        /* strOperation    */ "TE4:LOCK 899700000.000000, 210.0e3, 33.0, 10.196126, 4",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE1 with TE4",
        /* strOperation    */ "TE1 -> TE4",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE4 with TE1",
        /* strOperation    */ "TE4 -> TE1",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Remove Interference Checks",
        /* strOperation    */ "",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:UNLock TE1",
        /* strOperation    */ "TE1:UNLock",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarUnlock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE2",
        /* strOperation    */ "TE2:LOCK",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE2 with TE3",
        /* strOperation    */ "TE2 -> TE3",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE3 with TE2",
        /* strOperation    */ "TE3 -> TE2",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE2 with TE4",
        /* strOperation    */ "TE2 -> TE4",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE4 with TE2",
        /* strOperation    */ "TE4 -> TE2",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Remove Interference Checks",
        /* strOperation    */ "",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE4",
        /* strOperation    */ "TE4:LOCK",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE1, 900000000.000000, 210.0e3, 33.0, 16.425658, 1",
        /* strOperation    */ "TE1:LOCK 900000000.000000, 210.0e3, 33.0, 16.425658, 1",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE2 with TE1",
        /* strOperation    */ "TE2 -> TE1",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE1 with TE2",
        /* strOperation    */ "TE1 -> TE2",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE4 with TE1",
        /* strOperation    */ "TE4 -> TE1",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE1 with TE4",
        /* strOperation    */ "TE1 -> TE4",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Remove Interference Checks",
        /* strOperation    */ "",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:UNLock TE2",
        /* strOperation    */ "TE2:UNLock",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarUnlock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE3 with TE4",
        /* strOperation    */ "TE3 -> TE4",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE4 with TE3",
        /* strOperation    */ "TE4 -> TE3",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE1 with TE4",
        /* strOperation    */ "TE1 -> TE4",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE4 with TE1",
        /* strOperation    */ "TE4 -> TE1",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Remove Interference Checks",
        /* strOperation    */ "",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE3",
        /* strOperation    */ "TE3:LOCK",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": SBAR:LOCK TE2, 899900000.000000, 210.0e3, 33.0, 27.767715, 2",
        /* strOperation    */ "TE2:LOCK 899900000.000000, 210.0e3, 33.0, 27.767715, 2",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarLock(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE3 with TE2",
        /* strOperation    */ "TE3 -> TE2",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE2 with TE3",
        /* strOperation    */ "TE2 -> TE3",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE4 with TE2",
        /* strOperation    */ "TE4 -> TE2",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Interference check in path TE2 with TE4",
        /* strOperation    */ "TE2 -> TE4",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Remove Interference Checks",
        /* strOperation    */ "",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarInterferenceCheck(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Destroy Diagram",
        /* strOperation    */ "delete CDiagram",
        /* pTSGrpParent    */ pGrpSBar2,
        /* szDoTestStepFct */ SLOT(doTestStepSBarDestroyDiagram(ZS::Test::CTestStep*)) );

    #endif // #if 0

    // Recall test admin object settings
    //----------------------------------

    m_pAdminObjPool->read_(i_strTestStepsFileName);

} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pTmrTestStepTimeout;
    }
    catch(...)
    {
    }

    m_pAdminObjPool->save_();

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
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    m_pFrameDiagram = new QFrame();
    m_pFrameDiagram->setFrameShape(QFrame::Panel);
    m_pFrameDiagram->setFrameShadow(QFrame::Raised);
    QVBoxLayout* pLytFrameDiagram = new QVBoxLayout();
    //pLytFrameDiagram->setContentsMargins(0,0,0,0); // left,tip,right,bottom
    m_pFrameDiagram->setLayout(pLytFrameDiagram);
    CWidgetCentral::GetInstance()->getTestOutputWidget()->layout()->addWidget(m_pFrameDiagram);

    m_pWdgtDiagram = new CWdgtDiagram(
        /* pWdgtParent */ m_pFrameDiagram,
        /* strObjName  */ "Diagram" );
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

    // Actual Values Values
    //---------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSigGenCreateDiagram

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenDestroyDiagram( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    delete m_pWdgtDiagram;
    m_pWdgtDiagram = nullptr;

    delete m_pFrameDiagram;
    m_pFrameDiagram = nullptr;

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSigGenDestroyDiagram

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddScales( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    if( m_pWdgtDiagram != nullptr )
    {
        ZS::Diagram::SScale scaleX(
            /* fMin  */ 899.0e6,
            /* fMax  */ 901.0e6,
            /* pUnit */ Frequency()->Hertz() );

        m_pDiagScaleX = new CDiagScale(
            /* strObjName */ "DiagScaleX",
            /* scaleDir   */ EScaleDirX,
            /* scale      */ scaleX );
        m_pDiagScaleX->setDivLineDistMinPix(EDivLineLayerMain, 20);

        m_pWdgtDiagram->addDiagScale(m_pDiagScaleX);

        ZS::Diagram::SScale scaleY(
            /* fMin      */ 0.0,
            /* fMax      */ 40.0,
            /* pPhysUnit */ Power()->dBm() );

        m_pDiagScaleY = new CDiagScale(
            /* strObjName */ "DiagScaleY",
            /* scaleDir   */ EScaleDirY,
            /* scale      */ scaleY );
        m_pDiagScaleY->setDivLineDistMinPix(EDivLineLayerMain, 20);

        m_pWdgtDiagram->addDiagScale(m_pDiagScaleY);

    } // if( m_pWdgtDiagram != nullptr )

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSigGenAddScales

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveScales( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

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

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSigGenRemoveScales

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddTraces( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

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

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSigGenAddTraces

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveTraces( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

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

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSigGenRemoveTraces

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddGrid( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

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

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSigGenAddGrid

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveGrid( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

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

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSigGenRemoveGrid

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddAxisLabels( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

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

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSigGenAddAxisLabels

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveAxisLabels( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

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

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSigGenRemoveAxisLabels

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddCurves( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

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

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSigGenAddCurves

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveCurves( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

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

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSigGenRemoveCurves

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenAddMarkers( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

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

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSigGenAddMarkers

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenRemoveMarkers( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

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

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSigGenRemoveMarkers

//------------------------------------------------------------------------------
void CTest::doTestStepSigGenSetValues( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    //m_pTimerSigGen = new QTimer(this);
    //m_pTimerSigGen->start(10);

    //if( !connect(
    //    /* pObjSender   */ m_pTimerSigGen,
    //    /* szSignal     */ SIGNAL(timeout()),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onTimerSigGenTimeout()) ) )
    //{
    //    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    //}

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSigGenSetValues

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
        double fx, fy;
        double fRad;
        int    idxVal;

        s_fRadDrift += c_fRadDrift;

        if( s_fRadDrift > 2.0*Math::c_fPI )
        {
            s_fRadDrift = 0.0;
        }

        for( idxVal = 0; idxVal < c_iValCount; idxVal++ )
        {
            fx = scaleX.m_fMin + idxVal*fxStep;
            s_arfXValuesTraces01[idxVal] = fx;

            fRad = s_fRadDrift + 2.0*Math::c_fPI*(fx - scaleX.m_fMin) / fT;
            fy = scaleY.m_fMin + fyRange/2.0 + sin(fRad)*fyRange/5.0;
            s_arfYValuesTrace0[idxVal] = fy - 2.0 * fyRange + (static_cast<double>(rand()) / (static_cast<double>(RAND_MAX) + 1.0) - 0.5) * 0.02 * fyRange;
            s_arfYValuesTrace1[idxVal] = fy + 2.0 * fyRange + (static_cast<double>(rand()) / (static_cast<double>(RAND_MAX) + 1.0) - 0.5) * 0.1 * fyRange;
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
public slots: // test step methods (SBarrier interference checks)
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepSBarCreateDiagram( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    m_pFrameDiagram = new QFrame();
    m_pFrameDiagram->setFrameShape(QFrame::Panel);
    m_pFrameDiagram->setFrameShadow(QFrame::Raised);
    QVBoxLayout* pLytFrameDiagram = new QVBoxLayout();
    //pLytFrameDiagram->setContentsMargins(0,0,0,0); // left,tip,right,bottom
    m_pFrameDiagram->setLayout(pLytFrameDiagram);
    CWidgetCentral::GetInstance()->getTestOutputWidget()->layout()->addWidget(m_pFrameDiagram);

    m_pWdgtDiagram = new CWdgtDiagram(
        /* pWdgtParent */ m_pFrameDiagram,
        /* strObjName  */ "Diagram" );
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

    ZS::Diagram::SScale scaleX(
        /* fMin  */ 899.0e6,
        /* fMax  */ 901.0e6,
        /* pUnit */ Frequency()->Hertz() );

    m_pDiagScaleX = new CDiagScale(
        /* strObjName */ "DiagScaleX",
        /* scaleDir   */ EScaleDirX,
        /* scale      */ scaleX );
    m_pDiagScaleX->setDivLineDistMinPix(EDivLineLayerMain, 20);

    m_pWdgtDiagram->addDiagScale(m_pDiagScaleX);

    ZS::Diagram::SScale scaleY(
        /* fMin      */ -50.0,
        /* fMax      */ 50.0,
        /* pPhysUnit */ Power()->dBm() );

    m_pDiagScaleY = new CDiagScale(
        /* strObjName */ "DiagScaleY",
        /* scaleDir   */ EScaleDirY,
        /* scale      */ scaleY );
    m_pDiagScaleY->setDivLineDistMinPix(EDivLineLayerMain, 20);

    m_pWdgtDiagram->addDiagScale(m_pDiagScaleY);

    m_pDiagObjGrid = new CDiagObjGrid(
        /* strObjName  */ "DiagObjGrid",
        /* pDiagScaleX */ m_pDiagScaleX,
        /* pDiagScaleY */ m_pDiagScaleY );
    m_pWdgtDiagram->addDiagObj(m_pDiagObjGrid);

    m_pDiagObjAxisLabelX = new CDiagObjAxisLabel(
        /* strObjName  */ "DiagObjAxisLabelX",
        /* pDiagScaleX */ m_pDiagScaleX,
        /* layoutPos   */ ELayoutPosBottom );
    m_pWdgtDiagram->addDiagObj(m_pDiagObjAxisLabelX);

    m_pDiagObjAxisLabelY = new CDiagObjAxisLabel(
        /* strObjName  */ "DiagObjAxisLabelY",
        /* pDiagScaleX */ m_pDiagScaleY,
        /* layoutPos   */ ELayoutPosLeft );
    m_pWdgtDiagram->addDiagObj(m_pDiagObjAxisLabelY);

    m_pWdgtDiagram->update();

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSBarCreateDiagram

//------------------------------------------------------------------------------
void CTest::doTestStepSBarDestroyDiagram( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    m_mapiPathsSBars.clear();
    m_mappDiagTracesSBars.clear();
    m_mappDiagObjRectsSBars.clear();
    m_mappDiagObjLabelsSBars.clear();
    m_mappDiagTracesSBarsIfCheck.clear();
    m_mappDiagObjRectsSBarsIfCheck.clear();
    m_strlstSBarsLocked.clear();

    delete m_pWdgtDiagram;
    m_pWdgtDiagram = nullptr;

    delete m_pFrameDiagram;
    m_pFrameDiagram = nullptr;

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSBarDestroyDiagram

//------------------------------------------------------------------------------
void CTest::doTestStepSBarITabDef( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    // Example for strOperation:
    // "ITAB:DEF 4, 25.0"
    // "ITAB:DEF 4, xy"

    QString strOperation = i_pTestStep->getOperation();

    strOperation.remove("ITAB:DEF");

    QStringList strlst = strOperation.split(",");

    QString strPaths;
    int     iPaths = 0;
    QString strDefVal;
    double  fDefVal = 0.0;
    bool    bOk;
    int     iValTmp;
    double  fValTmp;

    if( strlst.size() > 0 ) strPaths = strlst[0];
    if( strlst.size() > 1 ) strDefVal = strlst[1];

    if( !strPaths.isEmpty() )
    {
        iValTmp = strPaths.toInt(&bOk);
        if( bOk ) iPaths = iValTmp;
    }
    if( !strDefVal.isEmpty() )
    {
        fValTmp = strDefVal.toDouble(&bOk);
        if( bOk ) fDefVal = fValTmp;
    }

    m_ararfIsoTab_dB.clear();

    if( iPaths > 0 )
    {
        m_ararfIsoTab_dB.resize(iPaths);

        for( int idxRow = 0; idxRow < m_ararfIsoTab_dB.size(); ++idxRow )
        {
            m_ararfIsoTab_dB[idxRow].resize(iPaths);
        }

        for( int idxRow = 0; idxRow < iPaths; ++idxRow )
        {
            for( int idxClm = 0; idxClm < iPaths; ++idxClm )
            {
                if( idxClm == idxRow )
                {
                    m_ararfIsoTab_dB[idxRow][idxClm] = 0.0;
                }
                else if( idxClm > idxRow )
                {
                    if( strDefVal == "xy" )
                    {
                        m_ararfIsoTab_dB[idxRow][idxClm] = (idxRow+1)*10.0 + idxClm+1;
                    }
                    else
                    {
                        m_ararfIsoTab_dB[idxRow][idxClm] = fDefVal;
                    }
                }
                else // if( idxClm < idxRow )
                {
                    m_ararfIsoTab_dB[idxRow][idxClm] = m_ararfIsoTab_dB[idxClm][idxRow];
                }
            }
        }
    }

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSBarITabDef

//------------------------------------------------------------------------------
void CTest::doTestStepSBarLock( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    // Example for strOperation:
    // "TE1:LOCK 899900000.000000, 210.0e3, 33.0, 24.028359, 1"

    QString strOperation = i_pTestStep->getOperation();

    QStringList strlst;

    strlst = strOperation.split(":LOCK");

    QString strSBar;
    QString strTraceName;
    QString strValues;

    if( strlst.size() > 0 ) strSBar = strlst[0];
    if( strlst.size() > 1 ) strValues = strlst[1];

    if( !strSBar.isEmpty() )
    {
        strTraceName = "DiagTraceSBar" + strSBar;
    }

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

    if( m_pWdgtDiagram != nullptr )
    {
        QColor colSBar = m_mapSBarCols.value(strSBar, c_colRed);

        CDiagTrace* pDiagTrace = m_pWdgtDiagram->getDiagTrace(strTraceName);

        if( pDiagTrace == nullptr )
        {
            pDiagTrace = new CDiagTrace(
                /* strObjName  */ strTraceName,
                /* pDiagScaleX */ m_pDiagScaleX,
                /* pDiagScaleY */ m_pDiagScaleY );
            m_pWdgtDiagram->addDiagTrace(pDiagTrace);
            m_mappDiagTracesSBars[strSBar] = pDiagTrace;

            CDiagObjRect* pDiagObjRect = new CDiagObjRect(
                /* strObjName */ "DiagObjRect" + strSBar,
                /* pDiagTrace */ m_mappDiagTracesSBars[strSBar] );
            m_pWdgtDiagram->addDiagObj(pDiagObjRect);
            m_mappDiagObjRectsSBars[strSBar] = pDiagObjRect;

            int iLabelPosOffsY = m_mapSBarLabelsPosOffsY.value(strSBar, 10);

            CDiagObjLabel* pDiagObjLabel = new CDiagObjLabel(
                /* strObjName */ "DiagObjLabel" + strSBar,
                /* strText    */ strSBar,
                /* layoutPos  */ ELayoutPosLeft );
            pDiagObjLabel->setColBg(Qt::white);
            pDiagObjLabel->setColText(colSBar);
            pDiagObjLabel->setLayoutPosAlignmentFlags(Qt::AlignTop);
            pDiagObjLabel->setLayoutPosOffsY(iLabelPosOffsY);
            m_pWdgtDiagram->addDiagObj(pDiagObjLabel);
            m_mappDiagObjLabelsSBars[strSBar] = pDiagObjLabel;

        } // if( pDiagTrace == nullptr )

        QVector<double> arfXValues(2);
        QVector<double> arfYValues(2);

        double fSBarCenterFreq_Hz = 0.0;
        double fSBarBandwidth_Hz = 0.0;
        double fSBarMaxPeakPower_dBm = 0.0;
        double fSBarDynRange_dB = 0.0;
        int    iPath = -1;

        strValues.remove("(");
        strValues.remove(")");

        strlst = strValues.split(",");

        if( strlst.size() == 0 )
        {
            arfXValues = pDiagTrace->getValues(EScaleDirX);
            arfYValues = pDiagTrace->getValues(EScaleDirY);

            iPath = m_mapiPathsSBars.value(strSBar, -1);
        }
        else if( strlst.size() == 5 )
        {
            bool bOk = false;

            fSBarCenterFreq_Hz = strlst[0].toDouble(&bOk);
            if( bOk ) fSBarBandwidth_Hz = strlst[1].toDouble(&bOk);
            if( bOk ) fSBarMaxPeakPower_dBm = strlst[2].toDouble(&bOk);
            if( bOk ) fSBarDynRange_dB = strlst[3].toDouble(&bOk);
            if( bOk ) iPath = strlst[4].toInt(&bOk);

            if( bOk )
            {
                arfXValues[0] = fSBarCenterFreq_Hz - 0.5 * fSBarBandwidth_Hz;   // min
                arfXValues[1] = fSBarCenterFreq_Hz + 0.5 * fSBarBandwidth_Hz;   // max

                arfYValues[0] = fSBarMaxPeakPower_dBm - fSBarDynRange_dB;       // min
                arfYValues[1] = fSBarMaxPeakPower_dBm;                          // max

                pDiagTrace->setValues(EScaleDirX, arfXValues);
                pDiagTrace->setValues(EScaleDirY, arfYValues);

                m_mapiPathsSBars[strSBar] = iPath;
            }
        }

        bool bNewLockInterferesWithAlreadyLockedSBars = false;

        if( m_strlstSBarsLocked.size() > 0 )
        {
            // Check if already locked SBars interferes with new lock.
            for( auto& strSBarLocked : m_strlstSBarsLocked )
            {
                CDiagTrace* pDiagTrcSrc = m_pWdgtDiagram->getDiagTrace("DiagTraceSBar" + strSBarLocked);
                CDiagTrace* pDiagTrcTrg = m_pWdgtDiagram->getDiagTrace("DiagTraceSBar" + strSBar);

                QVector<double> arfXValuesSrc = pDiagTrcSrc->getValues(EScaleDirX);
                QVector<double> arfYValuesSrc = pDiagTrcSrc->getValues(EScaleDirY);

                QVector<double> arfXValuesTrg = pDiagTrcTrg->getValues(EScaleDirX);;
                QVector<double> arfYValuesTrg = pDiagTrcTrg->getValues(EScaleDirY);;

                int iPathSrc = m_mapiPathsSBars.value(strSBarLocked, -1);
                int iPathTrg = m_mapiPathsSBars.value(strSBar, -1);

                if( iPathSrc >= 1 && iPathSrc <= m_ararfIsoTab_dB.size() )
                {
                    if( iPathTrg >= 1 && iPathTrg <= m_ararfIsoTab_dB[iPathSrc-1].size() )
                    {
                        for( int idxVal = 0; idxVal < arfYValues.size(); ++idxVal )
                        {
                            arfYValuesTrg[idxVal] -= m_ararfIsoTab_dB[iPathSrc-1][iPathTrg-1];
                        }
                    }
                }

                double fSBarFreqMinSrc_Hz     = arfXValuesSrc[0];
                double fSBarFreqRangeSrc_Hz   = arfXValuesSrc[1]- arfXValuesSrc[0];
                double fSBarPowerMinSrc_dBm   = arfYValuesSrc[0];
                double fSBarPowerRangeSrc_dBm = arfYValuesSrc[1]- arfYValuesSrc[0];

                QRectF rectSrc(fSBarFreqMinSrc_Hz, fSBarPowerMinSrc_dBm, fSBarFreqRangeSrc_Hz, fSBarPowerRangeSrc_dBm);

                double fSBarFreqMinTrg_Hz     = arfXValuesTrg[0];
                double fSBarFreqRangeTrg_Hz   = arfXValuesTrg[1]- arfXValuesTrg[0];
                double fSBarPowerMinTrg_dBm   = arfYValuesTrg[0];
                double fSBarPowerRangeTrg_dBm = arfYValuesTrg[1]- arfYValuesTrg[0];

                QRectF rectTrg(fSBarFreqMinTrg_Hz, fSBarPowerMinTrg_dBm, fSBarFreqRangeTrg_Hz, fSBarPowerRangeTrg_dBm);

                if( rectSrc.intersects(rectTrg) )
                {
                    bNewLockInterferesWithAlreadyLockedSBars = true;
                    break;
                }
            } // for( auto& strSBarLocked : m_strlstSBarsLocked )

            // Check if new lock interferes with already locked SBars.
            for( auto& strSBarLocked : m_strlstSBarsLocked )
            {
                CDiagTrace* pDiagTrcSrc = m_pWdgtDiagram->getDiagTrace("DiagTraceSBar" + strSBar);
                CDiagTrace* pDiagTrcTrg = m_pWdgtDiagram->getDiagTrace("DiagTraceSBar" + strSBarLocked);

                QVector<double> arfXValuesSrc = pDiagTrcSrc->getValues(EScaleDirX);
                QVector<double> arfYValuesSrc = pDiagTrcSrc->getValues(EScaleDirY);

                QVector<double> arfXValuesTrg = pDiagTrcTrg->getValues(EScaleDirX);;
                QVector<double> arfYValuesTrg = pDiagTrcTrg->getValues(EScaleDirY);;

                int iPathSrc = m_mapiPathsSBars.value(strSBar, -1);
                int iPathTrg = m_mapiPathsSBars.value(strSBarLocked, -1);

                if( iPathSrc >= 1 && iPathSrc <= m_ararfIsoTab_dB.size() )
                {
                    if( iPathTrg >= 1 && iPathTrg <= m_ararfIsoTab_dB[iPathSrc-1].size() )
                    {
                        for( int idxVal = 0; idxVal < arfYValues.size(); ++idxVal )
                        {
                            arfYValuesTrg[idxVal] -= m_ararfIsoTab_dB[iPathSrc-1][iPathTrg-1];
                        }
                    }
                }

                double fSBarFreqMinSrc_Hz     = arfXValuesSrc[0];
                double fSBarFreqRangeSrc_Hz   = arfXValuesSrc[1]- arfXValuesSrc[0];
                double fSBarPowerMinSrc_dBm   = arfYValuesSrc[0];
                double fSBarPowerRangeSrc_dBm = arfYValuesSrc[1]- arfYValuesSrc[0];

                QRectF rectSrc(fSBarFreqMinSrc_Hz, fSBarPowerMinSrc_dBm, fSBarFreqRangeSrc_Hz, fSBarPowerRangeSrc_dBm);

                double fSBarFreqMinTrg_Hz     = arfXValuesTrg[0];
                double fSBarFreqRangeTrg_Hz   = arfXValuesTrg[1]- arfXValuesTrg[0];
                double fSBarPowerMinTrg_dBm   = arfYValuesTrg[0];
                double fSBarPowerRangeTrg_dBm = arfYValuesTrg[1]- arfYValuesTrg[0];

                QRectF rectTrg(fSBarFreqMinTrg_Hz, fSBarPowerMinTrg_dBm, fSBarFreqRangeTrg_Hz, fSBarPowerRangeTrg_dBm);

                if( rectSrc.intersects(rectTrg) )
                {
                    bNewLockInterferesWithAlreadyLockedSBars = true;
                    break;
                }
            } // for( auto& strSBarLocked : m_strlstSBarsLocked )
        } // if( m_strlstSBarsLocked.size() > 0 )

        if( !bNewLockInterferesWithAlreadyLockedSBars )
        {
            if( !m_strlstSBarsLocked.contains(strSBar) )
            {
                m_strlstSBarsLocked.append(strSBar);
            }
        }

        QString strSBarTmp;
        QPen    pen;
        QBrush  brush;

        for( auto& pDiagObjRectTmp : m_mappDiagObjRectsSBars )
        {
            strSBarTmp = pDiagObjRectTmp->getObjName(); strSBarTmp.remove("DiagObjRect");

            colSBar = m_mapSBarCols.value(strSBarTmp, c_colRed);

            pen.setColor(colSBar);
            pen.setStyle(Qt::SolidLine);

            pDiagObjRectTmp->setPen(pen);

            if( m_strlstSBarsLocked.contains(strSBarTmp) )
            {
                colSBar.setAlpha(207);
            }
            else
            {
                colSBar.setAlpha(95);
            }
            brush.setColor(colSBar);
            brush.setStyle(Qt::SolidPattern);

            pDiagObjRectTmp->setBrush(brush);
        }

        for( auto& pDiagObjLabel : m_mappDiagObjLabelsSBars )
        {
            strSBarTmp = pDiagObjLabel->getObjName(); strSBarTmp.remove("DiagObjLabel");

            if( m_strlstSBarsLocked.contains(strSBarTmp) )
            {
                pDiagObjLabel->setText(strSBarTmp + " (L)");
            }
            else
            {
                pDiagObjLabel->setText(strSBarTmp);
            }
        }

        for( auto& pDiagObjRectTmp : m_mappDiagObjRectsSBars )
        {
            strSBarTmp = pDiagObjRectTmp->getObjName(); strSBarTmp.remove("DiagObjRect");

            if( m_strlstSBarsLocked.contains(strSBarTmp) )
            {
                m_pWdgtDiagram->moveDiagObjInPaintList(pDiagObjRectTmp, EDiagObjMoveModeToTop);
            }
        }

        m_pWdgtDiagram->update();

    } // if( m_pWdgtDiagram != nullptr )

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSBarLock

//------------------------------------------------------------------------------
void CTest::doTestStepSBarUnlock( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    // Example for strOperation:
    // "TE1:UNLock"

    QString strOperation = i_pTestStep->getOperation();

    QStringList strlst;

    strlst = strOperation.split(":UNLock");

    QString strSBar;
    QString strTraceName;

    if( strlst.size() > 0 ) strSBar = strlst[0];

    if( !strSBar.isEmpty()  )
    {
        strTraceName = "DiagTraceSBar" + strSBar;
    }

    if( m_strlstSBarsLocked.contains(strSBar) )
    {
        m_strlstSBarsLocked.removeOne(strSBar);
    }
    if( m_mapiPathsSBars.contains(strSBar) )
    {
        m_mapiPathsSBars.remove(strSBar);
    }

    if( m_pWdgtDiagram != nullptr )
    {
        CDiagTrace* pDiagTrace = m_pWdgtDiagram->getDiagTrace(strTraceName);

        if( pDiagTrace != nullptr )
        {
            CDiagObjLabel* pDiagObjLabel = m_mappDiagObjLabelsSBars.value(strSBar, nullptr);

            if( pDiagObjLabel != nullptr )
            {
                m_pWdgtDiagram->removeDiagObj(pDiagObjLabel);
                delete pDiagObjLabel;
                pDiagObjLabel = nullptr;
                m_mappDiagObjLabelsSBars.remove(strSBar);
            }

            CDiagObjRect* pDiagObjRect = m_mappDiagObjRectsSBars.value(strSBar, nullptr);

            if( pDiagObjRect != nullptr )
            {
                m_pWdgtDiagram->removeDiagObj(pDiagObjRect);
                delete pDiagObjRect;
                pDiagObjRect = nullptr;
                m_mappDiagObjRectsSBars.remove(strSBar);
            }

            pDiagObjRect = m_mappDiagObjRectsSBarsIfCheck.value(strSBar, nullptr);

            if( pDiagObjRect != nullptr )
            {
                m_pWdgtDiagram->removeDiagObj(pDiagObjRect);
                delete pDiagObjRect;
                pDiagObjRect = nullptr;
                m_mappDiagObjRectsSBarsIfCheck.remove(strSBar);
            }

            m_pWdgtDiagram->removeDiagTrace(pDiagTrace);
            delete pDiagTrace;
            pDiagTrace = nullptr;
            m_mappDiagTracesSBars.remove(strSBar);

            pDiagTrace = m_mappDiagTracesSBarsIfCheck.value(strSBar, nullptr);

            if( pDiagTrace != nullptr )
            {
                m_pWdgtDiagram->removeDiagTrace(pDiagTrace);
                delete pDiagTrace;
                pDiagTrace = nullptr;
                m_mappDiagTracesSBarsIfCheck.remove(strSBar);
            }

            QString strSBarTmp;
            QColor  colSBar;
            QPen    pen;
            QBrush  brush;

            for( auto& pDiagObjRectTmp : m_mappDiagObjRectsSBars )
            {
                strSBarTmp = pDiagObjRectTmp->getObjName(); strSBarTmp.remove("DiagObjRect");

                colSBar = m_mapSBarCols.value(strSBarTmp, c_colRed);

                pen.setColor(colSBar);
                pen.setStyle(Qt::SolidLine);

                pDiagObjRectTmp->setPen(pen);

                if( m_strlstSBarsLocked.contains(strSBarTmp) )
                {
                    colSBar.setAlpha(207);
                }
                else
                {
                    colSBar.setAlpha(95);
                }
                brush.setColor(colSBar);
                brush.setStyle(Qt::SolidPattern);

                pDiagObjRectTmp->setBrush(brush);
            }

            for( auto& pDiagObjLabel : m_mappDiagObjLabelsSBars )
            {
                strSBarTmp = pDiagObjLabel->getObjName(); strSBarTmp.remove("DiagObjLabel");

                if( m_strlstSBarsLocked.contains(strSBarTmp) )
                {
                    pDiagObjLabel->setText(strSBarTmp + " (L)");
                }
                else
                {
                    pDiagObjLabel->setText(strSBarTmp);
                }
            }

            for( auto& pDiagObjRectTmp : m_mappDiagObjRectsSBars )
            {
                strSBarTmp = pDiagObjRectTmp->getObjName(); strSBarTmp.remove("DiagObjRect");

                if( m_strlstSBarsLocked.contains(strSBarTmp) )
                {
                    m_pWdgtDiagram->moveDiagObjInPaintList(pDiagObjRectTmp, EDiagObjMoveModeToTop);
                }
            }
        } // if( pDiagTrace != nullptr )

        m_pWdgtDiagram->update();

    } // if( m_pWdgtDiagram != nullptr )

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSBarUnlock

//------------------------------------------------------------------------------
void CTest::doTestStepSBarInterferenceCheck( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    // Example for strOperation:
    // "TE1 -> TE2"

    QString strOperation = i_pTestStep->getOperation();
    QString strSBarSrc;
    QString strSBarTrg;

    strOperation.remove(" ");

    QStringList strlst = strOperation.split("->");

    if( strlst.size() == 2 )
    {
        strSBarSrc = strlst[0];
        strSBarTrg = strlst[1];
    }

    if( m_pWdgtDiagram != nullptr )
    {
        QString strSBarTmp;
        QColor  col;
        QPen    pen;
        QBrush  brush;

        for( auto& pDiagObjRectTmp : m_mappDiagObjRectsSBarsIfCheck )
        {
            m_pWdgtDiagram->removeDiagObj(pDiagObjRectTmp);
            delete pDiagObjRectTmp;
            pDiagObjRectTmp = nullptr;
        }
        m_mappDiagObjRectsSBarsIfCheck.clear();

        for( auto& pDiagTraceTmp : m_mappDiagTracesSBarsIfCheck )
        {
            m_pWdgtDiagram->removeDiagTrace(pDiagTraceTmp);
            delete pDiagTraceTmp;
            pDiagTraceTmp = nullptr;
        }
        m_mappDiagTracesSBarsIfCheck.clear();

        for( auto& pDiagObjRectTmp : m_mappDiagObjRectsSBars )
        {
            strSBarTmp = pDiagObjRectTmp->getObjName(); strSBarTmp.remove("DiagObjRect");

            col = m_mapSBarCols.value(strSBarTmp, c_colRed);

            pen.setColor(col);
            pen.setStyle(Qt::SolidLine);

            pDiagObjRectTmp->setPen(pen);

            if( m_strlstSBarsLocked.contains(strSBarTmp) )
            {
                col.setAlpha(207);
            }
            else
            {
                col.setAlpha(95);
            }
            brush.setColor(col);
            brush.setStyle(Qt::SolidPattern);

            pDiagObjRectTmp->setBrush(brush);
            pDiagObjRectTmp->show();
        }

        if( !strOperation.isEmpty() )
        {
            QString strTraceNameSrc   = "DiagTraceSBar" + strSBarSrc;
            QString strTraceNameTrg   = "DiagTraceSBar" + strSBarTrg;
            QString strDiagObjNameSrc = "DiagObjRect" + strSBarSrc;
            QString strDiagObjNameTrg = "DiagObjRect" + strSBarTrg;

            CDiagTrace*   pDiagTraceSrc   = m_pWdgtDiagram->getDiagTrace(strTraceNameSrc);
            CDiagObjRect* pDiagObjRectSrc = dynamic_cast<CDiagObjRect*>(m_pWdgtDiagram->getDiagObj(strDiagObjNameSrc));

            CDiagTrace*   pDiagTraceTrg   = m_pWdgtDiagram->getDiagTrace(strTraceNameTrg);
            CDiagObjRect* pDiagObjRectTrg = dynamic_cast<CDiagObjRect*>(m_pWdgtDiagram->getDiagObj(strDiagObjNameTrg));

            if( pDiagTraceSrc != nullptr && pDiagObjRectSrc != nullptr && pDiagTraceTrg != nullptr && pDiagObjRectTrg != nullptr )
            {
                for( auto& pDiagObjRectTmp : m_mappDiagObjRectsSBars )
                {
                    if( pDiagObjRectTmp != pDiagObjRectSrc && pDiagObjRectTmp != pDiagObjRectTrg )
                    {
                        pDiagObjRectTmp->hide();
                    }
                }

                pen = pDiagObjRectTrg->getPen();

                pen.setStyle(Qt::DotLine);

                pDiagObjRectTrg->setPen(pen);

                col = pen.color();
                col.setAlpha(63);
                brush.setColor(col);
                brush.setStyle(Qt::SolidPattern);

                pDiagObjRectTrg->setBrush(brush);

                CDiagTrace* pDiagTraceIfCheck = m_mappDiagTracesSBarsIfCheck.value(strSBarTrg, nullptr);

                if( pDiagTraceIfCheck == nullptr )
                {
                    pDiagTraceIfCheck = new CDiagTrace(
                        /* strObjName  */ strTraceNameTrg + "IfCheck",
                        /* pDiagScaleX */ m_pDiagScaleX,
                        /* pDiagScaleY */ m_pDiagScaleY );
                    m_pWdgtDiagram->addDiagTrace(pDiagTraceIfCheck);
                    m_mappDiagTracesSBarsIfCheck[strSBarTrg] = pDiagTraceIfCheck;
                }

                CDiagObjRect* pDiagObjRectIfCheck = m_mappDiagObjRectsSBarsIfCheck.value(strSBarTrg, nullptr);

                if( pDiagObjRectIfCheck == nullptr )
                {
                    pDiagObjRectIfCheck = new CDiagObjRect(
                        /* strObjName */ "DiagObjRect" + strSBarTrg + "IfCheck",
                        /* pDiagTrace */ pDiagTraceIfCheck );
                    m_pWdgtDiagram->addDiagObj(pDiagObjRectIfCheck);
                    m_mappDiagObjRectsSBarsIfCheck[strSBarTrg] = pDiagObjRectIfCheck;

                    col = pDiagObjRectTrg->getPen().color();

                    col.setAlpha(127);
                    brush.setColor(col);
                    brush.setStyle(Qt::SolidPattern);
                    pDiagObjRectIfCheck->setPen(col);
                    pDiagObjRectIfCheck->setBrush(brush);
                }

                QVector<double> arfXValues = pDiagTraceTrg->getValues(EScaleDirX);
                QVector<double> arfYValues = pDiagTraceTrg->getValues(EScaleDirY);

                int iPathSrc = m_mapiPathsSBars.value(strSBarSrc, -1);
                int iPathTrg = m_mapiPathsSBars.value(strSBarTrg, -1);

                if( iPathSrc >= 1 && iPathSrc <= m_ararfIsoTab_dB.size() )
                {
                    if( iPathTrg >= 1 && iPathTrg <= m_ararfIsoTab_dB[iPathSrc-1].size() )
                    {
                        for( int idxVal = 0; idxVal < arfYValues.size(); ++idxVal )
                        {
                            arfYValues[idxVal] -= m_ararfIsoTab_dB[iPathSrc-1][iPathTrg-1];
                        }
                    }
                }

                pDiagTraceIfCheck->setValues(EScaleDirX, arfXValues);
                pDiagTraceIfCheck->setValues(EScaleDirY, arfYValues);

            } // if( pDiagTraceSrc != nullptr && pDiagObjRectSrc != nullptr && pDiagTraceTrg != nullptr && pDiagObjRectTrg != nullptr )
        } // if( !strOperation.isEmpty() )

        for( auto& pDiagObjRectTmp : m_mappDiagObjRectsSBars )
        {
            strSBarTmp = pDiagObjRectTmp->getObjName(); strSBarTmp.remove("DiagObjRect");

            if( m_strlstSBarsLocked.contains(strSBarTmp) )
            {
                m_pWdgtDiagram->moveDiagObjInPaintList(pDiagObjRectTmp, EDiagObjMoveModeToTop);
            }
        }

        m_pWdgtDiagram->update();

    } // if( m_pWdgtDiagram != nullptr )

    // Actual Values Values
    //---------------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepSBarInterferenceCheck

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
        QString     strActualValue;
        QStringList strlstActualValues;

        strActualValue = "Test step not finished in time";
        strlstActualValues.append(strActualValue);

        pTestStep->setActualValues(strlstActualValues);

    } // if( pTestStep != nullptr )

} // onTimerTestStepTimeout()
