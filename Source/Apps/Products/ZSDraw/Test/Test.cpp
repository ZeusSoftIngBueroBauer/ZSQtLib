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

*******************************************************************************/

#include "Test/Test.h"
#include "Test/TestConfig.h"
#include "MainWindow.h"
#include "WidgetCentral.h"

#include "Electricity/ZSDrawGraphObjElectricityResistor.h"
#include "Electricity/ZSDrawGraphObjElectricityCapacitor.h"
#include "Electricity/ZSDrawGraphObjElectricityInductor.h"
#include "Electricity/ZSDrawGraphObjElectricitySwitch.h"
#include "Electricity/ZSDrawGraphObjElectricityTransistor.h"
#include "QtWidgets/ZSDrawGraphObjWdgtCheckBox.h"
#include "QtWidgets/ZSDrawGraphObjWdgtComboBox.h"
#include "QtWidgets/ZSDrawGraphObjWdgtGroupBox.h"
#include "QtWidgets/ZSDrawGraphObjWdgtLabel.h"
#include "QtWidgets/ZSDrawGraphObjWdgtLineEdit.h"
#include "QtWidgets/ZSDrawGraphObjWdgtPushButton.h"

#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingView.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjEllipse.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjImage.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPoint.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPolygon.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPolyline.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjRect.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjText.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtCore/qdir.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qfile.h>
#include <QtCore/qstandardpaths.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qtimer.h>

#include <QtGui/qevent.h>

#if QT_VERSION < 0x050000
#include <QtGui/qaction.h>
#else
#include <QtWidgets/qaction.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Draw::Electricity;
using namespace ZS::Draw::QtWidgets;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Products::Draw;
using namespace ZS::Apps::Test::Draw;


/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

QHash<QString, int> CTest::s_hshGraphObjsInstCounts = {
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypePoint), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypeLine), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypeRect), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypeEllipse), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypePolygon), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypePolyline), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypeText), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypeImage), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypeGroup), 0},
    { CObjFactory::c_strGroupNameConnections + "::" + graphObjType2Str(EGraphObjTypeConnectionLine), 0},
    { CObjFactory::c_strGroupNameConnections + "::" + graphObjType2Str(EGraphObjTypeConnectionPoint), 0}
};

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest() :
//------------------------------------------------------------------------------
    ZS::Test::CTest(NameSpace(), "theInst"),
    m_physValAngleSmallPlusSign(0.0, Units.Angle.Degree, 0.1),
    m_physValAngleBigPlusSign(0.0, Units.Angle.Degree, 0.1),
    m_physValAngleCheckmark(0.0, Units.Angle.Degree, 0.1),
    m_physValAngleSmallRect(0.0, Units.Angle.Degree, 0.1),
    m_physValAngleTopGroup(0.0, Units.Angle.Degree, 0.1)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    SErrResultInfo errResultInfo = saveTestSteps();
    if (errResultInfo.isErrorResult()) {
        if(CErrLog::GetInstance() != nullptr) {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    delete m_pPhysValRectSmallPlusSign;
    delete m_pPhysValLineSmallPlusSignVerticalLine;
    delete m_pPhysValLineSmallPlusSignHorizontalLine;

    delete m_pPhysValRectBigPlusSign;
    delete m_pPhysValLineBigPlusSignVerticalLine;
    delete m_pPhysValLineBigPlusSignHorizontalLine;

    delete m_pPhysValRectCheckmark;
    delete m_pPhysValLineCheckmarkLeftLine;
    delete m_pPhysValLineCheckmarkRightLine;

    delete m_pPhysValRectSmallRect;
    delete m_pPhysValLineSmallRectTopLine;
    delete m_pPhysValLineSmallRectRightLine;
    delete m_pPhysValLineSmallRectBottomLine;
    delete m_pPhysValLineSmallRectLeftLine;

    delete m_pPhysValRectTopGroup;

    m_pMainWindow = nullptr;
    m_pDrawingView = nullptr;
    m_pDrawingScene = nullptr;
    m_pPhysValRectSmallPlusSign = nullptr;
    m_pPhysValLineSmallPlusSignVerticalLine = nullptr;
    m_pPhysValLineSmallPlusSignHorizontalLine = nullptr;
    m_pPhysValRectBigPlusSign = nullptr;
    m_pPhysValLineBigPlusSignVerticalLine = nullptr;
    m_pPhysValLineBigPlusSignHorizontalLine = nullptr;
    m_pPhysValRectCheckmark = nullptr;
    m_pPhysValLineCheckmarkLeftLine = nullptr;
    m_pPhysValLineCheckmarkRightLine = nullptr;
    m_pPhysValRectSmallRect = nullptr;
    m_pPhysValLineSmallRectTopLine = nullptr;
    m_pPhysValLineSmallRectRightLine = nullptr;
    m_pPhysValLineSmallRectBottomLine = nullptr;
    m_pPhysValLineSmallRectLeftLine = nullptr;
    m_pPhysValRectTopGroup = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::setMainWindow( CMainWindow* i_pMainWindow )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pMainWindow == nullptr ? "null" : i_pMainWindow->objectName());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMainWindow",
        /* strAddInfo   */ strMthInArgs );

    m_pMainWindow = i_pMainWindow;
    m_pDrawingView = CWidgetCentral::GetInstance()->drawingView();
    m_pDrawingScene = CWidgetCentral::GetInstance()->drawingScene();

    m_pPhysValRectSmallPlusSign = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValLineSmallPlusSignVerticalLine = new CPhysValLine(*m_pDrawingScene);
    m_pPhysValLineSmallPlusSignHorizontalLine = new CPhysValLine(*m_pDrawingScene);

    m_pPhysValRectBigPlusSign = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValLineBigPlusSignVerticalLine = new CPhysValLine(*m_pDrawingScene);
    m_pPhysValLineBigPlusSignHorizontalLine = new CPhysValLine(*m_pDrawingScene);

    m_pPhysValRectCheckmark = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValLineCheckmarkLeftLine = new CPhysValLine(*m_pDrawingScene);
    m_pPhysValLineCheckmarkRightLine = new CPhysValLine(*m_pDrawingScene);

    m_pPhysValRectSmallRect = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValLineSmallRectTopLine = new CPhysValLine(*m_pDrawingScene);
    m_pPhysValLineSmallRectRightLine = new CPhysValLine(*m_pDrawingScene);
    m_pPhysValLineSmallRectBottomLine = new CPhysValLine(*m_pDrawingScene);
    m_pPhysValLineSmallRectLeftLine = new CPhysValLine(*m_pDrawingScene);

    m_pPhysValRectTopGroup = new CPhysValRect(*m_pDrawingScene);

    CDrawGridSettings gridSettings;
    gridSettings.setLinesVisible(true);
    gridSettings.setLinesDistMin(20);
    gridSettings.setLabelsVisible(true);
    gridSettings.setLabelsFont(QFont("Terminal"));

    createTestGroupDrawingSize(nullptr);

    // Pixels Drawings
    //----------------

    CDrawingSize drawingSize;
    drawingSize.setDimensionUnit(EScaleDimensionUnit::Pixels);
    drawingSize.setScreenResolutionInPxPerMM(1.0);
    drawingSize.setImageSize(CPhysVal(100, Units.Length.px), CPhysVal(100, Units.Length.px));
    // Must be set before creating test groups and test steps as the methods access the drawing scenes
    // drawing size to name test groups and steps and to parametrize the test steps.
    m_pDrawingScene->setDrawingSize(drawingSize);

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Pixels Drawing",
        /* pTSGrpParent */ nullptr );

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing100x100px = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ImageSize(" + qSize2Str(drawingSize.imageSizeInPixels()) + " " + drawingSize.unit().symbol(),
        /* pTSGrpParent */ pGrpPixelsDrawing );

    // For 100x100 pixel drawing we want to show that the right and bottom lines don't become visible.
    // But for this the painter must not use Antialiasing.
    const QMap<EGraphObjType, QPainter::RenderHints> mapGraphObjTypeRenderHints({
        {EGraphObjTypeLine, QPainter::RenderHints()}
    });
    createTestGroupPrepareScene(pGrpPixelsDrawing100x100px, drawingSize, gridSettings, mapGraphObjTypeRenderHints);

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing100x100pxObjectCoordinates = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Object Coordinates",
        /* pTSGrpParent */ pGrpPixelsDrawing100x100px );
    createTestGroupObjectCoordinatesAddLines(pGrpPixelsDrawing100x100pxObjectCoordinates);

    drawingSize.setImageSize(CPhysVal(101, Units.Length.px), CPhysVal(101, Units.Length.px));
    // Must be set before creating test groups and test steps as the methods access the drawing scenes
    // drawing size to name test groups and steps and to parametrize the test steps.
    m_pDrawingScene->setDrawingSize(drawingSize);

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing101x101px = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ImageSize(" + qSize2Str(drawingSize.imageSizeInPixels()) + " " + drawingSize.unit().symbol(),
        /* pTSGrpParent */ pGrpPixelsDrawing );

    createTestGroupPrepareScene(pGrpPixelsDrawing101x101px, drawingSize, gridSettings, mapGraphObjTypeRenderHints);

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing101x101pxObjectCoordinates = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Object Coordinates",
        /* pTSGrpParent */ pGrpPixelsDrawing101x101px );
    // For 101x101 pixel drawing we want to show that the right and bottom lines becomes even if no aliasing is set.
    createTestGroupObjectCoordinatesAddLines(pGrpPixelsDrawing101x101pxObjectCoordinates);

    drawingSize.setImageSize(CPhysVal(800, Units.Length.px, 1.0), CPhysVal(600, Units.Length.px, 1.0));
    // Must be set before creating test groups and test steps as the methods access the drawing scenes
    // drawing size to name test groups and steps and to parametrize the test steps.
    m_pDrawingScene->setDrawingSize(drawingSize);

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing800x600px = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ImageSize(" + qSize2Str(drawingSize.imageSizeInPixels()) + " " + drawingSize.unit().symbol(),
        /* pTSGrpParent */ pGrpPixelsDrawing );

    createTestGroupPrepareScene(pGrpPixelsDrawing800x600px, drawingSize, gridSettings);

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing800x600pxObjectCoordinates = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Object Coordinates",
        /* pTSGrpParent */ pGrpPixelsDrawing800x600px );
    createTestGroupObjectCoordinatesTransformPhysValShapes(pGrpPixelsDrawing800x600pxObjectCoordinates);

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing800x600pxAddObjects = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Add Objects",
        /* pTSGrpParent */ pGrpPixelsDrawing800x600px );
    createTestGroupAddObjects(pGrpPixelsDrawing800x600pxAddObjects);

    //ZS::Test::CTestStepGroup* pGrpPixelsDrawing800x600pxDrawObjects = new ZS::Test::CTestStepGroup(
    //    /* pTest        */ this,
    //    /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Objects",
    //    /* pTSGrpParent */ pGrpPixelsDrawing800x600px );
    //createTestGroupDrawObjects(pGrpPixelsDrawing800x600pxDrawObjects);

    // Metrics Drawings
    //-----------------

    drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
    drawingSize.setMetricUnit(Units.Length.mm);
    drawingSize.setMetricImageCoorsDecimals(1);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawing = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Metrics Drawing",
        /* pTSGrpParent */ nullptr );

    // YScaleTopDown
    //--------------

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Y-Scale TopDown",
        /* pTSGrpParent */ pGrpMetricsDrawing );

    drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::TopDown);
    drawingSize.setImageSize(CPhysVal(100, Units.Length.mm), CPhysVal(100, Units.Length.mm));
    drawingSize.setScreenResolutionInPxPerMM(3.5);
    // Must be set before creating test groups and test steps as the methods access the drawing scenes
    // drawing size to name test groups and steps and to parametrize the test steps.
    m_pDrawingScene->setDrawingSize(drawingSize);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown100x100mm = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ImageSize(" + drawingSize.metricImageWidth().toString() + ", " + drawingSize.metricImageHeight().toString() + ")",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleTopDown );

    createTestGroupPrepareScene(pGrpMetricsDrawingYScaleTopDown100x100mm, drawingSize, gridSettings);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown100x100mmObjectCoordinates = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Object Coordinates",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleTopDown100x100mm );
    createTestGroupObjectCoordinatesMetricsDrawingConversionFunctions(pGrpMetricsDrawingYScaleTopDown100x100mmObjectCoordinates);

    drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::TopDown);
    drawingSize.setImageSize(CPhysVal(800, Units.Length.mm), CPhysVal(600, Units.Length.mm));
    drawingSize.setScreenResolutionInPxPerMM(1.0);
    // Must be set before creating test groups and test steps as the methods access the drawing scenes
    // drawing size to name test groups and steps and to parametrize the test steps.
    m_pDrawingScene->setDrawingSize(drawingSize);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown800x600mm = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ImageSize(" + drawingSize.metricImageWidth().toString() + ", " + drawingSize.metricImageHeight().toString() + ")",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleTopDown );

    createTestGroupPrepareScene(pGrpMetricsDrawingYScaleTopDown800x600mm, drawingSize, gridSettings);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown800x600mmObjectCoordinates = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Object Coordinates",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleTopDown800x600mm );
    createTestGroupObjectCoordinatesTransformPhysValShapes(pGrpMetricsDrawingYScaleTopDown800x600mmObjectCoordinates);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown800x600mmAddObjects = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Add Objects",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleTopDown800x600mm );
    createTestGroupAddObjects(pGrpMetricsDrawingYScaleTopDown800x600mmAddObjects);

    //ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown800x600mmDrawObjects = new ZS::Test::CTestStepGroup(
    //    /* pTest        */ this,
    //    /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Objects",
    //    /* pTSGrpParent */ pGrpMetricsDrawingYScaleTopDown800x600mm );
    //createTestGroupDrawObjects(pGrpMetricsDrawingYScaleTopDown800x600mmDrawObjects);

    // YScaleBottomUp
    //---------------

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Y-Scale BottomUp",
        /* pTSGrpParent */ pGrpMetricsDrawing );

    drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::BottomUp);
    drawingSize.setImageSize(CPhysVal(100, Units.Length.mm), CPhysVal(100, Units.Length.mm));
    drawingSize.setScreenResolutionInPxPerMM(3.5);
    // Must be set before creating test groups and test steps as the methods access the drawing scenes
    // drawing size to name test groups and steps and to parametrize the test steps.
    m_pDrawingScene->setDrawingSize(drawingSize);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp100x100mm = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ImageSize(" + drawingSize.metricImageWidth().toString() + ", " + drawingSize.metricImageHeight().toString() + ")",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleBottomUp );

    createTestGroupPrepareScene(pGrpMetricsDrawingYScaleBottomUp100x100mm, drawingSize, gridSettings);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp100x100mmObjectCoordinates = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Object Coordinates",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleBottomUp100x100mm );
    createTestGroupObjectCoordinatesMetricsDrawingConversionFunctions(pGrpMetricsDrawingYScaleBottomUp100x100mmObjectCoordinates);

    drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::BottomUp);
    drawingSize.setImageSize(CPhysVal(800, Units.Length.mm), CPhysVal(600, Units.Length.mm));
    drawingSize.setScreenResolutionInPxPerMM(1.0);
    // Must be set before creating test groups and test steps as the methods access the drawing scenes
    // drawing size to name test groups and steps and to parametrize the test steps.
    m_pDrawingScene->setDrawingSize(drawingSize);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp800x600mm = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ImageSize(" + drawingSize.metricImageWidth().toString() + ", " + drawingSize.metricImageHeight().toString() + ")",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleBottomUp );

    createTestGroupPrepareScene(pGrpMetricsDrawingYScaleBottomUp800x600mm, drawingSize, gridSettings);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp800x600mmObjectCoordinates = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Object Coordinates",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleBottomUp800x600mm );
    createTestGroupObjectCoordinatesTransformPhysValShapes(pGrpMetricsDrawingYScaleBottomUp800x600mmObjectCoordinates);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp800x600mmAddObjects = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Add Objects",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleBottomUp800x600mm );
    createTestGroupAddObjects(pGrpMetricsDrawingYScaleBottomUp800x600mmAddObjects);

    //ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp800x600mmDrawObjects = new ZS::Test::CTestStepGroup(
    //    /* pTest        */ this,
    //    /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Objects",
    //    /* pTSGrpParent */ pGrpMetricsDrawingYScaleBottomUp800x600mm );
    //createTestGroupDrawObjects(pGrpMetricsDrawingYScaleBottomUp800x600mmDrawObjects);

    // Recall test step settings
    //--------------------------

    recallTestSteps();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawingSize(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawingSize",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpDrawingSize = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Drawing Size",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingSize",
        /* strOperation    */ "DrawingSize",
        /* pGrpParent      */ pGrpDrawingSize,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("setScreenResolutionInPxPerMM", 3.5);

    QStringList strlstExpectedValues;
    strlstExpectedValues.append("IsValid: false, DimensionUnit: Pixels, YScale: TopDown, "
                                "ScreenResolution: 3.5 px/mm, Size/mm {0.00, 0.00}, Size/px {0, 0}, "
                                "Scale: 1:1, PaperSize: ?, Orientation: ?");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingSize",
        /* strOperation    */ "DrawingSize",
        /* pGrpParent      */ pGrpDrawingSize,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("1: setScreenResolutionInPxPerMM", 3.5);
    pTestStep->setConfigValue("2: setDimensionUnit", CEnumScaleDimensionUnit(EScaleDimensionUnit::Pixels).toString());
    pTestStep->setConfigValue("3: setImageSize", "700, 350, 1.0, px");

    strlstExpectedValues.clear();
    strlstExpectedValues.append("IsValid: true, DimensionUnit: Pixels, YScale: TopDown, "
                                "ScreenResolution: 3.5 px/mm, Size/mm {200.00, 100.00}, Size/px {700, 350}, "
                                "Scale: 1:1, PaperSize: ?, Orientation: ?");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingSize",
        /* strOperation    */ "DrawingSize",
        /* pGrpParent      */ pGrpDrawingSize,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("1: setScreenResolutionInPxPerMM", 3.5);
    pTestStep->setConfigValue("2: setDimensionUnit", CEnumScaleDimensionUnit(EScaleDimensionUnit::Metric).toString());
    pTestStep->setConfigValue("3: setMetricImageCoorsDecimals", 2);
    pTestStep->setConfigValue("4: setMetricUnit", "mm");
    pTestStep->setConfigValue("5: setScaleFactor", "1:1");
    pTestStep->setConfigValue("6: setYScaleAxisOrientation", CEnumYScaleAxisOrientation(EYScaleAxisOrientation::TopDown).toString());
    pTestStep->setConfigValue("7: setImageSize", "100, 100, 1.0, mm");

    strlstExpectedValues.clear();
    strlstExpectedValues.append("IsValid: true, DimensionUnit: Metric, YScale: TopDown, "
                                "ScreenResolution: 3.5 px/mm, Size/mm {100.00, 100.00}, Size/px {351, 351}, "
                                "Scale: 1:1, PaperSize: ?, Orientation: ?");
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupPrepareScene(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent,
    const CDrawingSize& i_drawingSize,
    const CDrawGridSettings& i_gridSettings,
    const QMap<EGraphObjType, QPainter::RenderHints> i_mapGraphObjTypeRenderHints)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", DrawingSize {" + i_drawingSize.toString() + "}",
                       ", GridSettings {" + i_gridSettings.toString() + "}" +
                       ", RenderHints [" + QString::number(i_mapGraphObjTypeRenderHints.size()) + "]";
        if (i_mapGraphObjTypeRenderHints.size() > 0) {
            strMthInArgs += "(";
            for (const EGraphObjType graphObjType : i_mapGraphObjTypeRenderHints.keys()) {
                if (!strMthInArgs.endsWith("(")) strMthInArgs += ", ";
                strMthInArgs += "{" + graphObjType2Str(graphObjType) + ": " + GUI::qPainterRenderHints2Str(i_mapGraphObjTypeRenderHints[graphObjType]) + "}";
            }
            strMthInArgs += ")";
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupPrepareScene",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStep* pTestStep = nullptr;

    ZS::Test::CTestStepGroup* pGrpPrepareScene = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Prepare Scene",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    QSize sizeMainWindow(1800, 840);
    QString strMainWindowInfo = "{" + qSize2Str(sizeMainWindow) + " px}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " MainWindow.SetGeometry(" + strMainWindowInfo + ")",
        /* strOperation    */ "MainWindow.setGeometry(" + strMainWindowInfo + ")",
        /* pGrpParent      */ pGrpPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepMainWindowSetGeometry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", sizeMainWindow);
    pTestStep->setExpectedValue(qSize2Str(sizeMainWindow));

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    //initInstCounts();
    //initObjectCoors();

    QString strDrawingSizeInfo = "{" + qSize2Str(i_drawingSize.imageSizeInPixels()) + "} px";
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
        CPhysValSize physValSize(*m_pDrawingScene, i_drawingSize.metricImageWidth(), i_drawingSize.metricImageHeight());
        strDrawingSizeInfo += " " + i_drawingSize.dimensionUnit().toString()
            + " " + i_drawingSize.yScaleAxisOrientation().toString()
            + " {" + physValSize.toString() + "} " + physValSize.unit().symbol();
    }
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " SetDrawingSize(" + strDrawingSizeInfo + ")",
        /* strOperation    */ "DrawingScene.setDrawingSize(" + strDrawingSizeInfo + ")",
        /* pGrpParent      */ pGrpPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawingSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue(
        XmlStreamParser::c_strXmlElemNameDimensionUnit, i_drawingSize.dimensionUnit().toString());
    pTestStep->setConfigValue(
        XmlStreamParser::c_strXmlElemNameScreenResolutionPxPerMilliMeter, i_drawingSize.screenResolutionInPxPerMM());
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        pTestStep->setConfigValue(
            XmlStreamParser::c_strXmlElemNameWidth, i_drawingSize.imageWidthInPixels());
        pTestStep->setConfigValue(
            XmlStreamParser::c_strXmlElemNameHeight, i_drawingSize.imageHeightInPixels());
    }
    else {
        pTestStep->setConfigValue(
            XmlStreamParser::c_strXmlElemNameUnit, i_drawingSize.metricUnit().symbol());
        pTestStep->setConfigValue(
            XmlStreamParser::c_strXmlElemNameMetricImageCoorsDecimals, i_drawingSize.metricImageCoorsDecimals());
        pTestStep->setConfigValue(
            XmlStreamParser::c_strXmlElemNameWidth, i_drawingSize.metricImageWidth().getVal());
        pTestStep->setConfigValue(
            XmlStreamParser::c_strXmlElemNameHeight, i_drawingSize.metricImageHeight().getVal());
        pTestStep->setConfigValue(
            XmlStreamParser::c_strXmlElemNameYScaleAxisOrientation, i_drawingSize.yScaleAxisOrientation().toString());
    }
    pTestStep->setExpectedValue(i_drawingSize.toString());

    QString strGridSettingsInfo = "Lines {" + QString(i_gridSettings.areLinesVisible() ? "Visible" : "Invisible") +
        ", MinDist: " + QString::number(i_gridSettings.linesDistMin()) + " px}" +
        ", Labels {" + QString(i_gridSettings.areLabelsVisible() ? "Visible" : "Invisible") + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " SetGridSettings(" + strGridSettingsInfo + ")",
        /* strOperation    */ "DrawingScene.setGridSettings(" + strGridSettingsInfo + ")",
        /* pGrpParent      */ pGrpPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepSetGridSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue(
        XmlStreamParser::c_strXmlElemNameLinesVisible, i_gridSettings.areLinesVisible());
    pTestStep->setConfigValue(
        XmlStreamParser::c_strXmlElemNameLinesDistMin, i_gridSettings.linesDistMin());
    pTestStep->setConfigValue(
        XmlStreamParser::c_strXmlElemNameLabelsVisible, i_gridSettings.areLabelsVisible());
    pTestStep->setConfigValue(
        XmlStreamParser::c_strXmlElemNameFont, i_gridSettings.labelsFont().family());
    pTestStep->setExpectedValue(i_gridSettings.toString());

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " SetPainterRenderHints",
        /* strOperation    */ "GraphObjTypes.setPainterRenderHints",
        /* pGrpParent      */ pGrpPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepSetPainterRenderHints(ZS::Test::CTestStep*)) );
    for (const EGraphObjType graphObjType : i_mapGraphObjTypeRenderHints.keys()) {
        pTestStep->addDataRow({
            {"GraphObjType", graphObjType2Str(graphObjType)},
            {"PainterRenderHints", static_cast<uint>(i_mapGraphObjTypeRenderHints[graphObjType])}
        });
    }
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();
}

//------------------------------------------------------------------------------
void CTest::createTestStepSaveLoadFile(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestStepSaveLoadFile",
        /* strAddInfo   */ strMthInArgs );

    QString strAbsDirPath =
        QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator() + "Test";
    QString strFileName;
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strFileName = QString::number(drawingSize.imageWidthInPixels()) + "x" +
                      QString::number(drawingSize.imageHeightInPixels()) + "px.xml";
    }
    else {
        strFileName = QString::number(static_cast<int>(drawingSize.metricImageWidth().getVal())) + "x" +
                      QString::number(static_cast<int>(drawingSize.metricImageHeight().getVal())) +
                      drawingSize.metricUnit().symbol() + "-" + drawingSize.yScaleAxisOrientation().toString() + ".xml";
    }

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " SaveLoadFile",
        /* strOperation    */ "DrawingScene.save.load(" + strAbsDirPath + "/" + strFileName + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepSaveLoadFile(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("AbsDirPath", strAbsDirPath);
    pTestStep->setConfigValue("FileName", strFileName);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepDrawingSize( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepDrawingSize",
        /* strAddInfo   */ strMthInArgs );

    CDrawingSize drawingSize;

    QStringList strlstConfigValueKeys = i_pTestStep->getConfigValueKeys();
    QStringList strlstMethodCallsSorted;

    for (const QString& strKey : strlstConfigValueKeys) {
        QStringList strlst = strKey.split(":");
        if (strlst.size() == 2) {
            bool bOk = false;
            int iNr = strlst[0].toInt(&bOk);
            if (bOk && iNr >= 0) {
                while (strlstMethodCallsSorted.size() < (iNr+1)) {
                    strlstMethodCallsSorted.append("");
                }
                strlstMethodCallsSorted[iNr] = strKey;
            }
        }
        else {
            strlstMethodCallsSorted.append(strKey);
        }
    }
    for (const QString& strKey : strlstMethodCallsSorted) {
        QString strConfigValue = i_pTestStep->getConfigValue(strKey).toString();
        if (strKey.contains("setDimensionUnit", Qt::CaseInsensitive)) {
            CEnumScaleDimensionUnit dimensionUnit = strConfigValue;
            drawingSize.setDimensionUnit(dimensionUnit);
        }
        if (strKey.contains("setScreenResolutionInPxPerMM", Qt::CaseInsensitive)) {
            double fRes_px_mm = strConfigValue.toDouble();
            drawingSize.setScreenResolutionInPxPerMM(fRes_px_mm);
        }
        if (strKey.contains("setMetricImageCoorsDecimals", Qt::CaseInsensitive)) {
            int iDecimals = strConfigValue.toInt();
            drawingSize.setMetricImageCoorsDecimals(iDecimals);
        }
        if (strKey.contains("setMetricUnit", Qt::CaseInsensitive)) {
            CUnit unit = strConfigValue.simplified();
            drawingSize.setMetricUnit(unit);
        }
        if (strKey.contains("setNormedPaperSize", Qt::CaseInsensitive)) {
            CEnumNormedPaperSize paperSize = strConfigValue;
            drawingSize.setNormedPaperSize(paperSize);
        }
        if (strKey.contains("setNormedPaperOrientation", Qt::CaseInsensitive)) {
            CEnumOrientation orientation = strConfigValue;
            drawingSize.setNormedPaperOrientation(orientation);
        }
        if (strKey.contains("setScaleFactor", Qt::CaseInsensitive)) {
            QStringList strlstFactor = strConfigValue.split(":");
            if (strlstFactor.size() == 2) {
                bool bDividendOk = false;
                bool bDivisorOk = false;
                int iDividend = strlstFactor[0].toInt(&bDividendOk);
                int iDivisor = strlstFactor[1].toInt(&bDivisorOk);
                if (bDividendOk && iDividend > 0 && bDivisorOk && iDivisor > 0) {
                    drawingSize.setScaleFactor(iDividend, iDivisor);
                }
            }
        }
        if (strKey.contains("setYScaleAxisOrientation", Qt::CaseInsensitive)) {
            CEnumYScaleAxisOrientation yScaleAxisOrientation = strConfigValue;
            drawingSize.setYScaleAxisOrientation(yScaleAxisOrientation);
        }
        if (strKey.contains("setImageSize", Qt::CaseInsensitive)) {
            QStringList strlstImageSize = strConfigValue.split(",");
            if (strlstImageSize.size() == 4) {
                bool bWidthOk = false;
                double fWidth = strlstImageSize[0].toDouble(&bWidthOk);
                bool bHeightOk = false;
                double fHeight = strlstImageSize[1].toDouble(&bHeightOk);
                bool bResolutionOk = false;
                double fResolution = strlstImageSize[2].toDouble(&bResolutionOk);
                CUnit unit = strlstImageSize[3].simplified();
                if (bWidthOk && bHeightOk && bResolutionOk) {
                    drawingSize.setImageSize(CPhysValSize(*m_pDrawingScene, fWidth, fHeight, unit));
                }
            }
        }
    }

    QStringList strlstResultValues;
    strlstResultValues.append(drawingSize.toString());
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepMainWindowSetGeometry( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepMainWindowSetGeometry",
        /* strAddInfo   */ strMthInArgs );

    QSize size = i_pTestStep->getConfigValue("Size").toSize();

    QRect rectMainWindow = m_pMainWindow->geometry();
    rectMainWindow.setSize(size);
    m_pMainWindow->setGeometry(rectMainWindow);

    QString strResultValue = qSize2Str(m_pMainWindow->size());
    i_pTestStep->setResultValue(strResultValue);
}

//------------------------------------------------------------------------------
void CTest::doTestStepClearDrawingScene( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepClearDrawingScene",
        /* strAddInfo   */ strMthInArgs );

    m_pDrawingScene->clear();

    initInstCounts();

    i_pTestStep->setResultValue("");
}

//------------------------------------------------------------------------------
void CTest::doTestStepSetDrawingSize( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepSetDrawingSize",
        /* strAddInfo   */ strMthInArgs );

#pragma message(__TODO__"Test with different scale factors")

    CDrawingSize drawingSize;
    drawingSize.setDimensionUnit(
        i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlElemNameDimensionUnit).toString());
    if (i_pTestStep->hasConfigValue(XmlStreamParser::c_strXmlElemNameScreenResolutionPxPerMilliMeter)) {
        double fResPxPerMM = i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlElemNameScreenResolutionPxPerMilliMeter).toDouble();
        drawingSize.setScreenResolutionInPxPerMM(fResPxPerMM);
    }
    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        int cxWidth = i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlElemNameWidth).toInt();
        int cyHeight = i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlElemNameHeight).toInt();
        drawingSize.setImageSize(CPhysVal(cxWidth, Units.Length.px), CPhysVal(cyHeight, Units.Length.px));
    }
    else {
        CUnit unit = drawingSize.metricUnit();
        if (i_pTestStep->hasConfigValue(XmlStreamParser::c_strXmlElemNameUnit)) {
            unit = i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlElemNameUnit).toString();
            drawingSize.setMetricUnit(unit);
        }
        if (i_pTestStep->hasConfigValue(XmlStreamParser::c_strXmlElemNameMetricImageCoorsDecimals)) {
            int iDecimals = i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlElemNameMetricImageCoorsDecimals).toInt();
            drawingSize.setMetricImageCoorsDecimals(iDecimals);
        }
        double fxWidth = i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlElemNameWidth).toDouble();
        double fyHeight = i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlElemNameHeight).toDouble();
        drawingSize.setImageSize(CPhysVal(fxWidth, unit), CPhysVal(fyHeight, unit));
        bool bOk = false;
        if (i_pTestStep->hasConfigValue(XmlStreamParser::c_strXmlElemNameYScaleAxisOrientation)) {
            CEnumYScaleAxisOrientation eYScaleOrientation = CEnumYScaleAxisOrientation::fromString(
                i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlElemNameYScaleAxisOrientation).toString(), &bOk);
            if (bOk) {
                drawingSize.setYScaleAxisOrientation(eYScaleOrientation);
            }
        }
    }

    m_pDrawingScene->setDrawingSize(drawingSize);

    i_pTestStep->setResultValue(m_pDrawingScene->drawingSize().toString());
}

//------------------------------------------------------------------------------
void CTest::doTestStepSetGridSettings( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepSetGridSettings",
        /* strAddInfo   */ strMthInArgs );

    CDrawGridSettings gridSettings;
    gridSettings.setLinesVisible(
        i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlElemNameLinesVisible).toBool());
    gridSettings.setLinesDistMin(
        i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlElemNameLinesDistMin).toInt());
    gridSettings.setLabelsVisible(
        i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlElemNameLabelsVisible).toBool());
    gridSettings.setLabelsFont(
        i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlElemNameFont).toString());

    m_pDrawingScene->setGridSettings(gridSettings);

    i_pTestStep->setResultValue(m_pDrawingScene->gridSettings().toString());
}

//------------------------------------------------------------------------------
void CTest::doTestStepSetPainterRenderHints( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepSetPainterRenderHints",
        /* strAddInfo   */ strMthInArgs );

    //CGraphObjPoint::resetPainterRenderHints();
    CGraphObjLine::resetPainterRenderHints();
    //CGraphObjRect::resetPainterRenderHints();
    //CGraphObjEllipse::resetPainterRenderHints();
    //CGraphObjPolygon::resetPainterRenderHints();
    //CGraphObjPolyline::resetPainterRenderHints();
    //CGraphObjText::resetPainterRenderHints();
    //CGraphObjImage::resetPainterRenderHints();
    //CGraphObjConnectionPoint::resetPainterRenderHints();
    //CGraphObjConnectionLine::resetPainterRenderHints();
    CGraphObjGroup::resetPainterRenderHints();
    //CGraphObjSelectionPoint::resetPainterRenderHints();
    //CGraphObjLabel::resetPainterRenderHints();
    //CGraphObjPosition::resetPainterRenderHints();
    //CGraphObjLabelGeometryDX::resetPainterRenderHints();
    //CGraphObjLabelGeometryDY::resetPainterRenderHints();
    //CGraphObjGeometryLength::resetPainterRenderHints();
    //CGraphObjGeometryAngle::resetPainterRenderHints();

    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow) {
        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);
        QString strGraphObjType = dataRow["GraphObjType"].toString();
        EGraphObjType graphObjType = str2GraphObjType(strGraphObjType);
        QPainter::RenderHints uPainterRenderHints = dataRow["PainterRenderHints"].toUInt();
        if (graphObjType == EGraphObjTypePoint) {
            //CGraphObjPoint::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeLine) {
            CGraphObjLine::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeRect) {
            //CGraphObjRect::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeEllipse) {
            //CGraphObjEllipse::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypePolygon) {
            //CGraphObjPolygon::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypePolyline) {
            //CGraphObjPolyline::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeText) {
            //CGraphObjText::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeImage) {
            //CGraphObjImage::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeConnectionPoint) {
            //CGraphObjConnectionPoint::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeConnectionLine) {
            //CGraphObjConnectionLine::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeGroup) {
            CGraphObjGroup::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeSelectionPoint) {
            //CGraphObjSelectionPoint::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeLabel) {
            //CGraphObjLabel::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeLabelGeometryPosition) {
            //CGraphObjPosition::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeLabelGeometryDX) {
            //CGraphObjLabelGeometryDX::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeLabelGeometryDY) {
            //CGraphObjLabelGeometryDY::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeLabelGeometryLength) {
            //CGraphObjGeometryLength::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeLabelGeometryAngle) {
            //CGraphObjGeometryAngle::setPainterRenderHints(uPainterRenderHints);
        }
    }
    i_pTestStep->setResultValue("");
}

//------------------------------------------------------------------------------
void CTest::doTestStepTransformPhysValRect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepTransformPhysValRect",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysValRect physValRectResult(*m_pDrawingScene);

    QPointF ptTL;
    if (i_pTestStep->hasConfigValue("create.topLeft")) {
        ptTL = i_pTestStep->getConfigValue("create.topLeft").toPointF();
    }
    if (i_pTestStep->hasConfigValue("create.bottomRight")) {
        QPointF ptBR = i_pTestStep->getConfigValue("create.bottomRight").toPointF();
        if (i_pTestStep->hasConfigValue("create.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("create.unit").toString();
            CUnit unit = strUnitPoint;
            physValRectResult = CPhysValRect(*m_pDrawingScene, ptTL, ptBR, unit);
        }
        else {
            physValRectResult = CPhysValRect(*m_pDrawingScene, ptTL, ptBR);
        }
    }
    else if (i_pTestStep->hasConfigValue("create.size")) {
        QSizeF size = i_pTestStep->getConfigValue("create.size").toSizeF();
        if (i_pTestStep->hasConfigValue("create.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("create.unit").toString();
            CUnit unit = strUnitPoint;
            physValRectResult = CPhysValRect(*m_pDrawingScene, ptTL, size, unit);
        }
        else {
            physValRectResult = CPhysValRect(*m_pDrawingScene, ptTL, size);
        }
    }

    if (i_pTestStep->hasConfigValue("removeAndDeleteAllPhysValShapes")) {
        m_pDrawingScene->removeAndDeleteAllPhysValShapes();
    }
    if (i_pTestStep->hasConfigValue("addPhysValShape")) {
        CDrawSettings drawSettings(EGraphObjTypeRect);
        drawSettings.setPenColor(Qt::blue);
        CPhysValRect* pPhysValRect = new CPhysValRect(physValRectResult);
        m_pDrawingScene->addPhysValShape(pPhysValRect, drawSettings);
        pPhysValRect = nullptr;
    }
    if (i_pTestStep->hasConfigValue("setAngle")) {
        CPhysVal physValAngle = i_pTestStep->getConfigValue("setAngle").toString();
        physValAngle.setRes(0.1);
        physValRectResult.setAngle(physValAngle);
        if (physValAngle.getVal() != 0.0 && i_pTestStep->hasConfigValue("addPhysValShape")) {
            CDrawSettings drawSettings(EGraphObjTypeRect);
            drawSettings.setPenColor(Qt::darkGreen);
            CPhysValRect* pPhysValRect = new CPhysValRect(physValRectResult);
            m_pDrawingScene->addPhysValShape(pPhysValRect, drawSettings);
            pPhysValRect = nullptr;
        }
    }
    if (i_pTestStep->hasConfigValue("setCenter")) {
        QPointF pt = i_pTestStep->getConfigValue("setCenter").toPointF();
        if (i_pTestStep->hasConfigValue("setCenter.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setCenter.unit").toString();
            CUnit unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setCenter(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setCenter(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setSize")) {
        QSizeF sizeF = i_pTestStep->getConfigValue("setSize").toSizeF();
        if (i_pTestStep->hasConfigValue("setSize.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setSize.unit").toString();
            CUnit unit = strUnitPoint;
            CPhysValSize physValSize(*m_pDrawingScene, sizeF, unit);
            physValRectResult.setSize(physValSize);
        }
        else {
            CPhysValSize physValSize(*m_pDrawingScene, sizeF);
            physValRectResult.setSize(physValSize);
        }
    }
    if (i_pTestStep->hasConfigValue("setWidth")) {
        CPhysVal physValWidth = i_pTestStep->getConfigValue("setWidth").toString();
        physValRectResult.setWidth(physValWidth);
    }
    if (i_pTestStep->hasConfigValue("setWidthByMovingLeftCenter")) {
        QPointF pt = i_pTestStep->getConfigValue("setWidthByMovingLeftCenter").toPointF();
        if (i_pTestStep->hasConfigValue("setWidthByMovingLeftCenter.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setWidthByMovingLeftCenter.unit").toString();
            CUnit unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setWidthByMovingLeftCenter(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setWidthByMovingLeftCenter(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setWidthByMovingRightCenter")) {
        QPointF pt = i_pTestStep->getConfigValue("setWidthByMovingRightCenter").toPointF();
        if (i_pTestStep->hasConfigValue("setWidthByMovingRightCenter.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setWidthByMovingRightCenter.unit").toString();
            CUnit unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setWidthByMovingRightCenter(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setWidthByMovingRightCenter(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setHeight")) {
        CPhysVal physValHeight = i_pTestStep->getConfigValue("setHeight").toString();
        physValRectResult.setHeight(physValHeight);
    }
    if (i_pTestStep->hasConfigValue("setHeightByMovingTopCenter")) {
        QPointF pt = i_pTestStep->getConfigValue("setHeightByMovingTopCenter").toPointF();
        if (i_pTestStep->hasConfigValue("setTopCenter.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setHeightByMovingTopCenter.unit").toString();
            CUnit unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setHeightByMovingTopCenter(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setHeightByMovingTopCenter(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setHeightByMovingBottomCenter")) {
        QPointF pt = i_pTestStep->getConfigValue("setHeightByMovingBottomCenter").toPointF();
        if (i_pTestStep->hasConfigValue("setHeightByMovingBottomCenter.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setHeightByMovingBottomCenter.unit").toString();
            CUnit unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setHeightByMovingBottomCenter(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setHeightByMovingBottomCenter(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setTopLeft")) {
        QPointF pt = i_pTestStep->getConfigValue("setTopLeft").toPointF();
        if (i_pTestStep->hasConfigValue("setTopLeft.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setTopLeft.unit").toString();
            CUnit unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setTopLeft(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setTopLeft(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setTopRight")) {
        QPointF pt = i_pTestStep->getConfigValue("setTopRight").toPointF();
        if (i_pTestStep->hasConfigValue("setTopRight.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setTopRight.unit").toString();
            CUnit unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setTopRight(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setTopRight(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setBottomRight")) {
        QPointF pt = i_pTestStep->getConfigValue("setBottomRight").toPointF();
        if (i_pTestStep->hasConfigValue("setBottomRight.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setBottomRight.unit").toString();
            CUnit unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setBottomRight(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setBottomRight(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setBottomLeft")) {
        QPointF pt = i_pTestStep->getConfigValue("setBottomLeft").toPointF();
        if (i_pTestStep->hasConfigValue("setBottomLeft.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setBottomLeft.unit").toString();
            CUnit unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setBottomLeft(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setBottomLeft(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("addPhysValShape")) {
        CDrawSettings drawSettings(EGraphObjTypeRect);
        drawSettings.setPenColor(Qt::red);
        CPhysValRect* pPhysValRect = new CPhysValRect(physValRectResult);
        m_pDrawingScene->addPhysValShape(pPhysValRect, drawSettings);
        pPhysValRect = nullptr;
    }

    QStringList strlstResultValues;
    if (!ptTL.isNull()) {
        strlstResultValues.append("Center {" + physValRectResult.center().toString() + "} " + physValRectResult.center().unit().symbol());
        strlstResultValues.append("Width: " + physValRectResult.width().toString());
        strlstResultValues.append("Height: " + physValRectResult.height().toString());
        strlstResultValues.append("Angle: " + physValRectResult.angle().toString());
        strlstResultValues.append("TopLeft {" + physValRectResult.topLeft().toString() + "} " + physValRectResult.topLeft().unit().symbol());
        strlstResultValues.append("TopRight {" + physValRectResult.topRight().toString() + "} " + physValRectResult.topRight().unit().symbol());
        strlstResultValues.append("BottomRight {" + physValRectResult.bottomRight().toString() + "} " + physValRectResult.bottomRight().unit().symbol());
        strlstResultValues.append("BottomLeft {" + physValRectResult.bottomLeft().toString() + "} " + physValRectResult.bottomLeft().unit().symbol());
        strlstResultValues.append("TopCenter {" + physValRectResult.topCenter().toString() + "} " + physValRectResult.topCenter().unit().symbol());
        strlstResultValues.append("RightCenter {" + physValRectResult.rightCenter().toString() + "} " + physValRectResult.rightCenter().unit().symbol());
        strlstResultValues.append("BottomCenter {" + physValRectResult.bottomCenter().toString() + "} " + physValRectResult.bottomCenter().unit().symbol());
        strlstResultValues.append("LeftCenter {" + physValRectResult.leftCenter().toString() + "} " + physValRectResult.leftCenter().unit().symbol());
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepDrawingSceneConvertToPhysValPoint( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepDrawingSceneConvertToPhysValPoint",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unitSource = Units.Length.px;
    CUnit unitDest = drawingSize.unit();
    if (i_pTestStep->hasConfigValue("Point.Unit")) {
        QString strUnitPoint = i_pTestStep->getConfigValue("Point.Unit").toString();
        unitSource = strUnitPoint;
    }
    if (i_pTestStep->hasConfigValue("UnitDest")) {
        QString strUnitDest = i_pTestStep->getConfigValue("UnitDest").toString();
        unitDest = strUnitDest;
    }
    QPointF pt = i_pTestStep->getConfigValue("Point").toPointF();
    CPhysValPoint physValPointResult(*m_pDrawingScene);
    if (Units.Length.isMetricUnit(unitSource)) {
        CPhysValPoint physValPoint(*m_pDrawingScene, pt, unitSource);
        physValPointResult = m_pDrawingScene->convert(physValPoint, unitDest);
    }
    else {
        physValPointResult = m_pDrawingScene->convert(pt, unitDest);
    }
    QString strResultValue = "{" + physValPointResult.toString() + "} " + physValPointResult.unit().symbol();
    i_pTestStep->setResultValue(strResultValue);
}

//------------------------------------------------------------------------------
void CTest::doTestStepDrawingSceneConvertToPhysValSize( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepDrawingSceneConvertToPhysValSize",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unitSource = Units.Length.px;
    CUnit unitDest = drawingSize.unit();
    if (i_pTestStep->hasConfigValue("Size.Unit")) {
        QString strUnitPoint = i_pTestStep->getConfigValue("Size.Unit").toString();
        unitSource = strUnitPoint;
    }
    if (i_pTestStep->hasConfigValue("UnitDest")) {
        QString strUnitDest = i_pTestStep->getConfigValue("UnitDest").toString();
        unitDest = strUnitDest;
    }
    QSizeF sizeF = i_pTestStep->getConfigValue("Size").toSizeF();
    CPhysValSize physValSizeResult(*m_pDrawingScene);
    if (Units.Length.isMetricUnit(unitSource)) {
        CPhysValSize physValSize(*m_pDrawingScene, sizeF, unitSource);
        physValSizeResult = m_pDrawingScene->convert(physValSize, unitDest);
    }
    else {
        physValSizeResult = m_pDrawingScene->convert(sizeF, unitDest);
    }
    QString strResultValue = "{" + physValSizeResult.toString() + "} " + physValSizeResult.unit().symbol();
    i_pTestStep->setResultValue(strResultValue);
}

//------------------------------------------------------------------------------
void CTest::doTestStepDrawingSceneConvertToPhysValLine( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepDrawingSceneConvertToPhysValLine",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unitSource = Units.Length.px;
    CUnit unitDest = drawingSize.unit();
    if (i_pTestStep->hasConfigValue("Line.Unit")) {
        QString strUnitPoint = i_pTestStep->getConfigValue("Line.Unit").toString();
        unitSource = strUnitPoint;
    }
    if (i_pTestStep->hasConfigValue("UnitDest")) {
        QString strUnitDest = i_pTestStep->getConfigValue("UnitDest").toString();
        unitDest = strUnitDest;
    }
    QLineF lineF = i_pTestStep->getConfigValue("Line").toLineF();
    CPhysValLine physValLineResult(*m_pDrawingScene);
    if (Units.Length.isMetricUnit(unitSource)) {
        CPhysValLine physValLine(*m_pDrawingScene, lineF, unitSource);
        physValLineResult = m_pDrawingScene->convert(physValLine, unitDest);
    }
    else {
        physValLineResult = m_pDrawingScene->convert(lineF, unitDest);
    }
    QStringList strlstResultValues;
    strlstResultValues.append("P1 {" + physValLineResult.p1().toString() + "} " + physValLineResult.unit().symbol());
    strlstResultValues.append("P2 {" + physValLineResult.p2().toString() + "} " + physValLineResult.unit().symbol());
    strlstResultValues.append("Center {" + physValLineResult.center().toString() + "} " + physValLineResult.unit().symbol());
    strlstResultValues.append("dx: " + physValLineResult.dx().toString());
    strlstResultValues.append("dy: " + physValLineResult.dy().toString());
    strlstResultValues.append("Length: " + physValLineResult.length().toString());
    strlstResultValues.append("Angle: " + physValLineResult.angle().toString());
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepDrawingSceneConvertToPhysValRect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepDrawingSceneConvertToPhysValRect",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unitSource = Units.Length.px;
    CUnit unitDest = drawingSize.unit();
    if (i_pTestStep->hasConfigValue("Rect.Unit")) {
        QString strUnitPoint = i_pTestStep->getConfigValue("Rect.Unit").toString();
        unitSource = strUnitPoint;
    }
    if (i_pTestStep->hasConfigValue("UnitDest")) {
        QString strUnitDest = i_pTestStep->getConfigValue("UnitDest").toString();
        unitDest = strUnitDest;
    }
    QPointF ptTL = i_pTestStep->getConfigValue("Rect.topLeft").toPointF();
    QSizeF size = i_pTestStep->getConfigValue("Rect.size").toSizeF();
    CPhysValRect physValRectResult(*m_pDrawingScene);
    if (Units.Length.isMetricUnit(unitSource)) {
        CPhysValRect physValRect(*m_pDrawingScene, ptTL, size, unitSource);
        physValRectResult = m_pDrawingScene->convert(physValRect, unitDest);
    }
    else {
        physValRectResult = m_pDrawingScene->convert(QRectF(ptTL, size), unitDest);
    }
    QStringList strlstResultValues;
    strlstResultValues.append("TopLeft {" + physValRectResult.topLeft().toString() + "} " + physValRectResult.unit().symbol());
    strlstResultValues.append("TopRight {" + physValRectResult.topRight().toString() + "} " + physValRectResult.unit().symbol());
    strlstResultValues.append("BottomRight {" + physValRectResult.bottomRight().toString() + "} " + physValRectResult.unit().symbol());
    strlstResultValues.append("BottomLeft {" + physValRectResult.bottomLeft().toString() + "} " + physValRectResult.unit().symbol());
    strlstResultValues.append("Center {" + physValRectResult.center().toString() + "} " + physValRectResult.unit().symbol());
    strlstResultValues.append("Width: " + physValRectResult.width().toString());
    strlstResultValues.append("Height: " + physValRectResult.height().toString());
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepSaveLoadFile( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepSaveLoadFile",
        /* strAddInfo   */ strMthInArgs );

    QString strAbsDirPath = i_pTestStep->getConfigValue("AbsDirPath").toString();
    QString strFileName = i_pTestStep->getConfigValue("FileName").toString();
    QString strAbsFilePath = strAbsDirPath + QDir::separator() + strFileName;

    QFileInfo fileInfo(strAbsFilePath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }

    SErrResultInfo errResultInfoSave = m_pDrawingScene->save(strAbsFilePath);
    if (errResultInfoSave.isErrorResult()) {
        CErrLog::GetInstance()->addEntry(errResultInfoSave);
        i_pTestStep->setExpectedValue(SErrResultInfo().toString());
        i_pTestStep->setResultValue(errResultInfoSave.toString());
    }
    else {
        QStringList strlstExpectedValues;
        CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();
        CIdxTree::iterator itIdxTree = pIdxTree->begin(CIdxTree::iterator::ETraversalOrder::PreOrder);
        while (itIdxTree != pIdxTree->end()) {
            CIdxTreeEntry* pTreeEntry = *itIdxTree;
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pTreeEntry);
            if (pGraphObj != nullptr) {
                strlstExpectedValues.append(resultValuesForGraphObj(pGraphObj, true));
            }
            ++itIdxTree;
        }
        i_pTestStep->setExpectedValues(strlstExpectedValues);
    }
    if (!errResultInfoSave.isErrorResult()) {
        SErrResultInfo errResultInfoLoad = m_pDrawingScene->load(strAbsFilePath);
        if (errResultInfoLoad.isErrorResult()) {
            CErrLog::GetInstance()->addEntry(errResultInfoLoad);
            i_pTestStep->setExpectedValue(SErrResultInfo().toString());
            i_pTestStep->setResultValue(errResultInfoLoad.toString());
        }
        else {
            QStringList strlstResultValues;
            CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();
            CIdxTree::iterator itIdxTree = pIdxTree->begin(CIdxTree::iterator::ETraversalOrder::PreOrder);
            while (itIdxTree != pIdxTree->end()) {
                CIdxTreeEntry* pTreeEntry = *itIdxTree;
                CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pTreeEntry);
                if (pGraphObj != nullptr) {
                    strlstResultValues.append(resultValuesForGraphObj(pGraphObj, true));
                }
                ++itIdxTree;
            }
            i_pTestStep->setResultValues(strlstResultValues);
        }

    }
}

//------------------------------------------------------------------------------
void CTest::doTestStepSetMode( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepSetMode",
        /* strAddInfo   */ strMthInArgs );

    QString strMode = i_pTestStep->getConfigValue("Mode").toString();
    CEnumMode eMode = strMode;

    i_pTestStep->setExpectedValue("");

    m_pDrawingScene->setMode(eMode);

    i_pTestStep->setResultValue("");
}

//------------------------------------------------------------------------------
void CTest::doTestStepSetCurrentDrawingTool( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepSetCurrentDrawingTool",
        /* strAddInfo   */ strMthInArgs );

    QStringList strlstExpectedValues;
    QString strFactoryGroupName = i_pTestStep->getConfigValue("FactoryGroupName").toString();
    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();

    if (!strFactoryGroupName.isEmpty() && !strGraphObjType.isEmpty()) {
        if (strFactoryGroupName == CObjFactory::c_strGroupNameStandardShapes && strGraphObjType == graphObjType2Str(EGraphObjTypeGroup)) {
            m_pDrawingScene->groupGraphObjsSelected();
        }
        else {
            m_pDrawingScene->setCurrentDrawingTool(strFactoryGroupName, strGraphObjType);
            strlstExpectedValues.append(strFactoryGroupName + "::" + strGraphObjType);
        }
    }
    else {
        m_pDrawingScene->setCurrentDrawingTool(nullptr);
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    QStringList strlstResultValues;

    if (!strFactoryGroupName.isEmpty() && !strGraphObjType.isEmpty()) {
        if (strFactoryGroupName == CObjFactory::c_strGroupNameStandardShapes && strGraphObjType == graphObjType2Str(EGraphObjTypeGroup)) {
        }
        else {
            CObjFactory* pObjFactory = m_pDrawingScene->getCurrentDrawingTool();
            if (pObjFactory != nullptr) {
                strFactoryGroupName = pObjFactory->getGroupName();
                strGraphObjType = pObjFactory->getGraphObjTypeAsString();
                strlstResultValues.append(strFactoryGroupName + "::" + strGraphObjType);
            }
        }
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepUnsetCurrentDrawingTool( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepUnsetCurrentDrawingTool",
        /* strAddInfo   */ strMthInArgs );

    i_pTestStep->setExpectedValue("");

    m_pDrawingScene->setCurrentDrawingTool(nullptr);

    i_pTestStep->setResultValue("");
}

//------------------------------------------------------------------------------
void CTest::doTestStepAddGraphObjLine( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepAddGraphObjLine",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);

    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    CPhysValPoint physValPoint1(*m_pDrawingScene);
    physValPoint1 = i_pTestStep->getConfigValue("P1").toPointF();
    CPhysValPoint physValPoint2(*m_pDrawingScene);
    physValPoint2 = i_pTestStep->getConfigValue("P2").toPointF();

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);
    if (pObjFactory != nullptr) {
        CDrawSettings drawSettings(EGraphObjTypeLine);
        CGraphObj* pGraphObj = pObjFactory->createGraphObj(m_pDrawingScene, physValPoint1, drawSettings);
        m_pDrawingScene->addGraphObj(pGraphObj);
        CGraphObjLine* pGraphObjLine = dynamic_cast<CGraphObjLine*>(pGraphObj);
        if (pGraphObjLine != nullptr) {
            pGraphObjLine->setP2(physValPoint2);
        }
        pGraphObjLine->rename(strGraphObjName);
    }

    QStringList strlstResultValues;
    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strKeyInTree);
    if (pGraphObj != nullptr) {
        strlstResultValues.append(resultValuesForGraphObj(pGraphObj));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepAddGraphObjGroup( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepAddGraphObjGroup",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);

    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    QStringList strlstGraphObjsAddToGroup = i_pTestStep->getConfigValue("AddToGroup").toStringList();

    QStringList strlstGraphObjsKeyInTreeGetResultValues;
    if (i_pTestStep->hasConfigValue("GraphObjsKeyInTreeGetResultValues")) {
        strlstGraphObjsKeyInTreeGetResultValues = i_pTestStep->getConfigValue("GraphObjsKeyInTreeGetResultValues").toStringList();
    }

    CGraphObjGroup* pGraphObjGroup = nullptr;
    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);
    if (pObjFactory != nullptr) {
        CDrawSettings drawSettings(EGraphObjTypeGroup);
        CGraphObj* pGraphObj = pObjFactory->createGraphObj(m_pDrawingScene, CPhysValPoint(*m_pDrawingScene), drawSettings);
        m_pDrawingScene->addGraphObj(pGraphObj);
        pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphObj);
        if (pGraphObjGroup != nullptr) {
            pGraphObjGroup->rename(strGraphObjName);
            for (const QString& strGraphObjNameChild : strlstGraphObjsAddToGroup) {
                strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjNameChild);
                CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strKeyInTree);
                if (pGraphObj != nullptr) {
                    pGraphObjGroup->addToGroup(pGraphObj);
                }
            }
        }
    }

    if (pGraphObjGroup != nullptr && strlstGraphObjsKeyInTreeGetResultValues.isEmpty()) {
        strlstGraphObjsKeyInTreeGetResultValues.append(pGraphObjGroup->keyInTree());
        for (CGraphObj* pGraphObjChild : pGraphObjGroup->childs()) {
            strlstGraphObjsKeyInTreeGetResultValues.append(pGraphObjChild->keyInTree());
        }
    }

    QStringList strlstResultValues;
    for (const QString& strGraphObjKeyInTree : strlstGraphObjsKeyInTreeGetResultValues) {
        CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
        if (pGraphObj != nullptr) {
            strlstResultValues.append(resultValuesForGraphObj(pGraphObj));
        }
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepModifyGraphObjLine( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepModifyGraphObjLine",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);

    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();
    QString strMethod = i_pTestStep->getConfigValue("Method").toString();

    CGraphObjLine* pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pDrawingScene->findGraphObj(strGraphObjKeyInTree));
    if (pGraphObjLine != nullptr) {
        if (strMethod.compare("setLine", Qt::CaseInsensitive) == 0) {
            if (i_pTestStep->hasConfigValue("P1")) {
                CPhysValPoint physValPoint1(*m_pDrawingScene);
                physValPoint1 = i_pTestStep->getConfigValue("P1").toPointF();
                pGraphObjLine->setP1(physValPoint1);
            }
            if (i_pTestStep->hasConfigValue("P2")) {
                CPhysValPoint physValPoint2(*m_pDrawingScene);
                physValPoint2 = i_pTestStep->getConfigValue("P2").toPointF();
                pGraphObjLine->setP2(physValPoint2);
            }
        }
        else if (strMethod.compare("setPosition", Qt::CaseInsensitive) == 0) {
            CPhysValPoint physValPos(*m_pDrawingScene);
            physValPos = i_pTestStep->getConfigValue("Pos").toPointF();
            pGraphObjLine->setPosition(physValPos);
        }
        else if (strMethod.compare("setRotationAngle", Qt::CaseInsensitive) == 0) {
            CPhysVal physValAngle(0.0, Units.Angle.Degree, 0.1);
            QString strAngle = i_pTestStep->getConfigValue("Angle").toString();
            physValAngle = strAngle;
            pGraphObjLine->setRotationAngle(physValAngle);
        }
    }

    QStringList strlstResultValues;
    if (pGraphObjLine != nullptr) {
        strlstResultValues.append(resultValuesForGraphObj(pGraphObjLine));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepModifyGraphObjGroup( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepModifyGraphObjGroup",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);

    QString strGroupName = i_pTestStep->getConfigValue("GroupName").toString();
    QString strGroupKeyInTree = i_pTestStep->getConfigValue("GroupKeyInTree").toString();
    QString strGraphObjChildName;
    if (i_pTestStep->hasConfigValue("GraphObjChildName")) {
        strGraphObjChildName = i_pTestStep->getConfigValue("GraphObjChildName").toString();
    }
    QString strGraphObjChildKeyInTree;
    if (i_pTestStep->hasConfigValue("GraphObjChildKeyInTree")) {
        strGraphObjChildKeyInTree = i_pTestStep->getConfigValue("GraphObjChildKeyInTree").toString();
    }
    QString strMethod = i_pTestStep->getConfigValue("Method").toString();

    QStringList strlstGraphObjsKeyInTreeGetResultValues;
    if (i_pTestStep->hasConfigValue("GraphObjsKeyInTreeGetResultValues")) {
        strlstGraphObjsKeyInTreeGetResultValues = i_pTestStep->getConfigValue("GraphObjsKeyInTreeGetResultValues").toStringList();
    }

    CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(m_pDrawingScene->findGraphObj(strGroupKeyInTree));
    CGraphObj* pGraphObjChild = nullptr;
    if (!strGraphObjChildKeyInTree.isEmpty()) {
        pGraphObjChild = m_pDrawingScene->findGraphObj(strGraphObjChildKeyInTree);
    }
    if (pGraphObjGroup != nullptr) {
        if (strMethod.compare("ungroup", Qt::CaseInsensitive) == 0) {
            m_pDrawingScene->ungroup(pGraphObjGroup);
            pGraphObjGroup = nullptr;
        }
        else if (strMethod.compare("addToGroup", Qt::CaseInsensitive) == 0) {
            if (pGraphObjChild != nullptr) {
                pGraphObjGroup->addToGroup(pGraphObjChild);
            }
        }
        else if (strMethod.compare("removeFromGroup", Qt::CaseInsensitive) == 0) {
            if (pGraphObjChild != nullptr) {
                pGraphObjGroup->removeFromGroup(pGraphObjChild);
                if (strlstGraphObjsKeyInTreeGetResultValues.isEmpty()) {
                    strlstGraphObjsKeyInTreeGetResultValues.append(pGraphObjChild->keyInTree());
                    if (pGraphObjChild->isGroup()) {
                        CGraphObjGroup* pGraphObjGroupChild = dynamic_cast<CGraphObjGroup*>(pGraphObjChild);
                        for (CGraphObj* pGraphObjChildTmp : pGraphObjGroupChild->childs()) {
                            strlstGraphObjsKeyInTreeGetResultValues.append(pGraphObjChildTmp->keyInTree());
                        }
                    }
                }
            }
        }
        else if (strMethod.compare("setRotationAngle", Qt::CaseInsensitive) == 0) {
            CPhysVal physValAngle(0.0, Units.Angle.Degree, 0.1);
            QString strAngle = i_pTestStep->getConfigValue("Angle").toString();
            physValAngle = strAngle;
            pGraphObjGroup->setRotationAngle(physValAngle);
        }
        else if (strMethod.compare("resizeToContent", Qt::CaseInsensitive) == 0) {
            pGraphObjGroup->resizeToContent();
        }
        else if (strMethod.compare("setPosition", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("Pos").toPointF();
            if (i_pTestStep->hasConfigValue("Pos.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("Pos.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObjGroup->setPosition(physValPoint);
        }
        else if (strMethod.compare("setCenter", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("Center").toPointF();
            if (i_pTestStep->hasConfigValue("Center.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("Center.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObjGroup->setCenter(physValPoint);
        }
        else if (strMethod.compare("setTopLeft", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("TopLeft").toPointF();
            if (i_pTestStep->hasConfigValue("TopLeft.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("TopLeft.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObjGroup->setTopLeft(physValPoint);
        }
        else if (strMethod.compare("setTopRight", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("TopRight").toPointF();
            if (i_pTestStep->hasConfigValue("TopRight.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("TopRight.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObjGroup->setTopRight(physValPoint);
        }
        else if (strMethod.compare("setBottomRight", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("BottomRight").toPointF();
            if (i_pTestStep->hasConfigValue("BottomRight.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("BottomRight.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObjGroup->setBottomRight(physValPoint);
        }
        else if (strMethod.compare("setBottomLeft", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("BottomLeft").toPointF();
            if (i_pTestStep->hasConfigValue("BottomLeft.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("BottomLeft.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObjGroup->setBottomLeft(physValPoint);
        }
        else if (strMethod.compare("setSize", Qt::CaseInsensitive) == 0) {
            QSizeF size = i_pTestStep->getConfigValue("Size").toSizeF();
            if (i_pTestStep->hasConfigValue("Size.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("Size.unit").toString();
                unit = strUnit;
            }
            CPhysValSize physValSize(*m_pDrawingScene, size, unit);
            pGraphObjGroup->setSize(physValSize);
        }
        else if (strMethod.compare("setWidth", Qt::CaseInsensitive) == 0) {
            CPhysVal physVal(unit);
            if (i_pTestStep->hasConfigValue("Width.unit")) {
                double fVal = i_pTestStep->getConfigValue("Width").toDouble();
                QString strUnit = i_pTestStep->getConfigValue("Width.unit").toString();
                unit = strUnit;
                physVal = CPhysVal(fVal, unit);
            }
            else {
                QString strVal = i_pTestStep->getConfigValue("Width").toString();
                physVal = strVal;
            }
            pGraphObjGroup->setWidth(physVal);
        }
        else if (strMethod.compare("setHeight", Qt::CaseInsensitive) == 0) {
            CPhysVal physVal(unit);
            if (i_pTestStep->hasConfigValue("Height.unit")) {
                double fVal = i_pTestStep->getConfigValue("Height").toDouble();
                QString strUnit = i_pTestStep->getConfigValue("Height.unit").toString();
                unit = strUnit;
                physVal = CPhysVal(fVal, unit);
            }
            else {
                QString strVal = i_pTestStep->getConfigValue("Height").toString();
                physVal = strVal;
            }
            pGraphObjGroup->setHeight(physVal);
        }

        if (pGraphObjGroup != nullptr && strlstGraphObjsKeyInTreeGetResultValues.isEmpty()) {
            strlstGraphObjsKeyInTreeGetResultValues.append(pGraphObjGroup->keyInTree());
            for (CGraphObj* pGraphObjChild : pGraphObjGroup->childs()) {
                strlstGraphObjsKeyInTreeGetResultValues.append(pGraphObjChild->keyInTree());
            }
        }
    }

    QStringList strlstResultValues;
    for (const QString& strGraphObjKeyInTree : strlstGraphObjsKeyInTreeGetResultValues) {
        CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
        if (pGraphObj != nullptr) {
            strlstResultValues.append(resultValuesForGraphObj(pGraphObj));
        }
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepShowLabels( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepShowLabels",
        /* strAddInfo   */ strMthInArgs );

    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strLabelName = i_pTestStep->getConfigValue("LabelName").toString();

    QString strKeyInTree = "B:" + strGraphObjName;
    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strKeyInTree);
    if (pGraphObj != nullptr) {
        pGraphObj->showLabel(strLabelName);
        pGraphObj->showLabelAnchorLine(strLabelName);
    }

    QStringList strlstExpectedValues;
    QStringList strlstResultValues;
    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow)
    {
        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);
        QString strGraphObjName = dataRow["GraphObjName"].toString();
        QString strGraphObjKeyInTree = dataRow["GraphObjKeyInTree"].toString();
        QString strLabelName = dataRow["LabelName"].toString();
        QPointF pos = dataRow["setPos"].toPointF();
        QString strExpectedText = dataRow["ExpectedText"].toString();
        strlstExpectedValues.append(resultValuesForLabel(
            strGraphObjName + "." + strLabelName, pos, strExpectedText));
        CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
        if (pGraphObj != nullptr) {
            pGraphObj->showLabel(strLabelName);
            pGraphObj->showLabelAnchorLine(strLabelName);
            CGraphObjLabel* pGraphObjLabel = pGraphObj->getLabel(strLabelName);
            QGraphicsSimpleTextItem* pGraphicsItemLabel = dynamic_cast<QGraphicsSimpleTextItem*>(pGraphObjLabel);
            if (pGraphicsItemLabel != nullptr) {
                pGraphicsItemLabel->setPos(pos);
                QString strText = pGraphicsItemLabel->text();
                strlstResultValues.append(resultValuesForLabel(
                    pGraphObj->name() + "." + pGraphObjLabel->name(), pGraphicsItemLabel->pos(), strText));
            }
            else {
                strlstResultValues.append(strGraphObjName + "." + strLabelName + " not found");
            }
        }
        else {
            strlstResultValues.append(strGraphObjName + " not found");
        }
    }
    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepHideLabels( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepHideLabels",
        /* strAddInfo   */ strMthInArgs );

    QStringList strlstExpectedValues;
    QStringList strlstResultValues;
    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow)
    {
        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);
        QString strGraphObjName = dataRow["GraphObjName"].toString();
        QString strGraphObjKeyInTree = dataRow["GraphObjKeyInTree"].toString();
        QString strLabelName = dataRow["LabelName"].toString();
        strlstExpectedValues.append(dataRow["ExpectedValue"].toString());
        CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
        if (pGraphObj != nullptr) {
            pGraphObj->hideLabel(strLabelName);
            pGraphObj->hideLabelAnchorLine(strLabelName);
            CGraphObjLabel* pGraphObjLabel = pGraphObj->getLabel(strLabelName);
            QGraphicsSimpleTextItem* pGraphicsItemLabel = dynamic_cast<QGraphicsSimpleTextItem*>(pGraphObjLabel);
            if (pGraphicsItemLabel != nullptr) {
                QString strText = pGraphicsItemLabel->text();
                strlstResultValues.append(resultValuesForLabel(
                    pGraphObj->name() + "." + pGraphObjLabel->name(), pGraphicsItemLabel->pos(), strText));
            }
            else {
                strlstResultValues.append(strGraphObjName + "." + strLabelName + " not found");
            }
        }
        else {
            strlstResultValues.append(strGraphObjName + " not found");
        }
    }
    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepShowGeometryLabels( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepShowGeometryLabels",
        /* strAddInfo   */ strMthInArgs );

    QStringList strlstExpectedValues;
    QStringList strlstResultValues;
    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow)
    {
        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);
        QString strGraphObjName = dataRow["GraphObjName"].toString();
        QString strGraphObjKeyInTree = dataRow["GraphObjKeyInTree"].toString();
        QString strLabelName = dataRow["LabelName"].toString();
        QPointF pos;
        if (dataRow.contains("setPos")) {
            pos = dataRow["setPos"].toPointF();
        }
        QString strExpectedText = dataRow["ExpectedText"].toString();
        strlstExpectedValues.append(resultValuesForLabel(
            strGraphObjName + "." + strLabelName, pos, strExpectedText));
        CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
        if (pGraphObj != nullptr) {
            pGraphObj->showGeometryLabel(strLabelName);
            pGraphObj->showGeometryLabelAnchorLine(strLabelName);
            CGraphObjLabel* pGraphObjLabel = pGraphObj->getGeometryLabel(strLabelName);
            QGraphicsSimpleTextItem* pGraphicsItemLabel = dynamic_cast<QGraphicsSimpleTextItem*>(pGraphObjLabel);
            if (pGraphicsItemLabel != nullptr) {
                if (!pos.isNull()) {
                    pGraphicsItemLabel->setPos(pos);
                    pos = pGraphicsItemLabel->pos();
                }
                QString strText = pGraphicsItemLabel->text();
                strlstResultValues.append(resultValuesForLabel(
                    pGraphObj->name() + "." + pGraphObjLabel->name(), pos, strText));
            }
            else {
                strlstResultValues.append(strGraphObjName + "." + strLabelName + " not found");
            }
        }
        else {
            strlstResultValues.append(strGraphObjName + " not found");
        }
    }
    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepHideGeometryLabels( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepHideGeometryLabels",
        /* strAddInfo   */ strMthInArgs );

    QStringList strlstExpectedValues;
    QStringList strlstResultValues;
    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow)
    {
        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);
        QString strGraphObjName = dataRow["GraphObjName"].toString();
        QString strGraphObjKeyInTree = dataRow["GraphObjKeyInTree"].toString();
        QString strLabelName = dataRow["LabelName"].toString();
        strlstExpectedValues.append(dataRow["ExpectedValue"].toString());
        CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
        if (pGraphObj != nullptr) {
            pGraphObj->hideGeometryLabel(strLabelName);
            pGraphObj->hideGeometryLabelAnchorLine(strLabelName);
            CGraphObjLabel* pGraphObjLabel = pGraphObj->getGeometryLabel(strLabelName);
            QGraphicsSimpleTextItem* pGraphicsItemLabel = dynamic_cast<QGraphicsSimpleTextItem*>(pGraphObjLabel);
            if (pGraphicsItemLabel != nullptr) {
                QString strText = pGraphicsItemLabel->text();
                strlstResultValues.append(resultValuesForLabel(
                    pGraphObj->name() + "." + pGraphObjLabel->name(), pGraphicsItemLabel->pos(), strText));
            }
            else {
                strlstResultValues.append(strGraphObjName + "." + strLabelName + " not found");
            }
        }
        else {
            strlstResultValues.append(strGraphObjName + " not found");
        }
    }
    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepMousePressEvent( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepMousePressEvent",
        /* strAddInfo   */ strMthInArgs );

    QPoint ptMousePos = i_pTestStep->getConfigValue("MousePos").toPoint();
    Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
    if (i_pTestStep->hasConfigValue("KeyboardModifiers")) {
        keyboardModifiers = static_cast<Qt::KeyboardModifiers>(i_pTestStep->getConfigValue("KeyboardModifiers").toInt());
    }
    QPoint ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptMousePos);
    QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptDrawingViewMousePos);
    ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
    ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

    QMouseEvent* pMouseEv = new QMouseEvent(
        /* type      */ QEvent::MouseButtonPress,
        /* pos       */ ptDrawingViewMousePos,
        /* globalPos */ ptMousePosGlobal,
        /* button    */ Qt::LeftButton,
        /* button    */ Qt::LeftButton,
        /* modifiers */ keyboardModifiers );
    m_pDrawingView->mousePressEvent(pMouseEv);
    delete pMouseEv;
    pMouseEv = nullptr;

    QStringList strlstExpectedValues;
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    QStringList strlstResultValues;
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepMouseReleaseEvent( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepMouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    QPoint ptMousePos = i_pTestStep->getConfigValue("MousePos").toPoint();
    Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
    if (i_pTestStep->hasConfigValue("KeyboardModifiers")) {
        keyboardModifiers = static_cast<Qt::KeyboardModifiers>(i_pTestStep->getConfigValue("KeyboardModifiers").toInt());
    }
    QPoint ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptMousePos);
    QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptDrawingViewMousePos);
    ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
    ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

    QMouseEvent* pMouseEv = new QMouseEvent(
        /* type      */ QEvent::MouseButtonRelease,
        /* pos       */ ptDrawingViewMousePos,
        /* globalPos */ ptMousePosGlobal,
        /* button    */ Qt::LeftButton,
        /* buttons   */ Qt::NoButton,
        /* modifiers */ keyboardModifiers );
    m_pDrawingView->mouseReleaseEvent(pMouseEv);
    delete pMouseEv;
    pMouseEv = nullptr;

    QStringList strlstExpectedValues;
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    QStringList strlstResultValues;
    i_pTestStep->setResultValues(strlstResultValues);
}
//------------------------------------------------------------------------------
void CTest::doTestStepMouseMoveEvent( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepMouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    Qt::MouseButton mouseBtn = Qt::LeftButton;
    if (i_pTestStep->hasConfigValue("MouseButtons")) {
        mouseBtn = static_cast<Qt::MouseButton>(i_pTestStep->getConfigValue("MouseButtons").toInt());
    }
    QPoint ptMousePos = i_pTestStep->getConfigValue("MousePos").toPoint();
    QPoint ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptMousePos);
    QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptDrawingViewMousePos);
    ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
    ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

    QMouseEvent* pMouseEv = new QMouseEvent(
        /* type      */ QEvent::MouseMove,
        /* pos       */ ptDrawingViewMousePos,
        /* globalPos */ ptMousePosGlobal,
        /* button    */ Qt::NoButton,
        /* buttons   */ mouseBtn,
        /* modifiers */ Qt::NoModifier );
    m_pDrawingView->mouseMoveEvent(pMouseEv);
    delete pMouseEv;
    pMouseEv = nullptr;

    QStringList strlstExpectedValues;
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    QStringList strlstResultValues;
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepMouseMoveEvents( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepMouseMoveEvents",
        /* strAddInfo   */ strMthInArgs );

    int iMovesCount = 0;
    Qt::MouseButton mouseBtn = Qt::LeftButton;
    if (i_pTestStep->hasConfigValue("MouseButtons")) {
        mouseBtn = static_cast<Qt::MouseButton>(i_pTestStep->getConfigValue("MouseButtons").toInt());
    }
    QPoint ptMousePosStart = i_pTestStep->getConfigValue("MousePosStart").toPoint();
    QPoint ptMousePosStop = i_pTestStep->getConfigValue("MousePosStop").toPoint();
    if (i_pTestStep->hasConfigValue("MovesCount")) {
        iMovesCount = i_pTestStep->getConfigValue("MovesCount").toInt();
    }

    bool bMoveRight = ptMousePosStop.x() > ptMousePosStart.x();
    bool bMoveLeft = ptMousePosStop.x() < ptMousePosStart.x();
    bool bMoveDown = ptMousePosStop.y() > ptMousePosStart.y();
    bool bMoveUp = ptMousePosStop.y() < ptMousePosStart.y();

    int xDist_px = abs(ptMousePosStop.x() - ptMousePosStart.x());
    int yDist_px = abs(ptMousePosStop.y() - ptMousePosStart.y());
    if (iMovesCount == 0) {
        iMovesCount = xDist_px;
        if (xDist_px < yDist_px) {
            iMovesCount = yDist_px;
        }
    }

    double fXStep_px = (static_cast<double>(xDist_px) / static_cast<double>(iMovesCount));
    double fYStep_px = (static_cast<double>(yDist_px) / static_cast<double>(iMovesCount));

    QPoint ptMouseMovePos = ptMousePosStart;
    for (int iMoves = 0; iMoves < iMovesCount; ++iMoves) {
        ptMouseMovePos.setX(ptMouseMovePos.x() + Math::round2Nearest(fXStep_px));
        ptMouseMovePos.setY(ptMouseMovePos.y() + Math::round2Nearest(fYStep_px));
        if (bMoveRight && ptMouseMovePos.x() > ptMousePosStop.x()) {
            ptMouseMovePos.setX(ptMousePosStop.x());
        }
        else if (bMoveLeft && ptMouseMovePos.x() < ptMousePosStop.x()) {
            ptMouseMovePos.setX(ptMousePosStop.x());
        }
        if (bMoveDown && ptMouseMovePos.y() > ptMousePosStop.y()) {
            ptMouseMovePos.setY(ptMousePosStop.y());
        }
        else if (bMoveUp && ptMouseMovePos.y() < ptMousePosStop.y()) {
            ptMouseMovePos.setY(ptMousePosStop.y());
        }

        QPoint ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptMouseMovePos);
        QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptDrawingViewMousePos);

        QMouseEvent* pMouseEv = new QMouseEvent(
            /* type      */ QEvent::MouseMove,
            /* pos       */ ptDrawingViewMousePos,
            /* globalPos */ ptMousePosGlobal,
            /* button    */ Qt::NoButton,
            /* buttons   */ mouseBtn,
            /* modifiers */ Qt::NoModifier );
        m_pDrawingView->mouseMoveEvent(pMouseEv);
        delete pMouseEv;
        pMouseEv = nullptr;
        if (bMoveRight && ptMouseMovePos.x() >= ptMousePosStop.x()) {
            break;
        }
        else if (bMoveLeft && ptMouseMovePos.x() <= ptMousePosStop.x()) {
            break;
        }
        if (bMoveDown && ptMouseMovePos.y() >= ptMousePosStop.y()) {
            break;
        }
        else if (bMoveUp && ptMouseMovePos.y() <= ptMousePosStop.y()) {
            break;
        }
    }

    QStringList strlstExpectedValues;
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    QStringList strlstResultValues;
    i_pTestStep->setResultValues(strlstResultValues);
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::initInstCounts()
//------------------------------------------------------------------------------
{
    CGraphObjPoint::s_iInstCount = 0;
    CGraphObjLine::s_iInstCount = 0;
    CGraphObjRect::s_iInstCount = 0;
    CGraphObjEllipse::s_iInstCount = 0;
    CGraphObjPolygon::s_iInstCount = 0;
    CGraphObjPolyline::s_iInstCount = 0;
    CGraphObjText::s_iInstCount = 0;
    CGraphObjImage::s_iInstCount = 0;
    CGraphObjConnectionPoint::s_iInstCount = 0;
    CGraphObjConnectionLine::s_iInstCount = 0;
    CGraphObjGroup::s_iInstCount = 0;

    m_hshGraphObjNameToKeys.clear();

    m_ptPosSmallPlusSign = QPointF();
    m_sizeSmallPlusSign = QSizeF();
    *m_pPhysValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene);
    m_physValAngleSmallPlusSign = CPhysVal(0.0, Units.Angle.Degree, 0.1);
    m_ptPosSmallPlusSignVerticalLine = QPointF();
    m_lineSmallPlusSignVerticalLine = QLineF();
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene);
    m_ptPosSmallPlusSignHorizontalLine = QPointF();
    m_lineSmallPlusSignHorizontalLine = QLineF();
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene);

    m_ptPosBigPlusSign = QPointF();
    m_sizeBigPlusSign = QSizeF();
    *m_pPhysValRectBigPlusSign = CPhysValRect(*m_pDrawingScene);
    m_physValAngleBigPlusSign = CPhysVal(0.0, Units.Angle.Degree, 0.1);
    m_ptPosBigPlusSignVerticalLine = QPointF();
    m_lineBigPlusSignVerticalLine = QLineF();
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene);
    m_ptPosBigPlusSignHorizontalLine = QPointF();
    m_lineBigPlusSignHorizontalLine = QLineF();
    *m_pPhysValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene);

    m_ptPosCheckmark = QPointF();
    m_sizeCheckmark = QSizeF();
    *m_pPhysValRectCheckmark = CPhysValRect(*m_pDrawingScene);
    m_physValAngleCheckmark = CPhysVal(0.0, Units.Angle.Degree, 0.1);
    m_ptPosCheckmarkLeftLine = QPointF();
    m_lineCheckmarkLeftLine = QLineF();
    *m_pPhysValLineCheckmarkLeftLine = CPhysValLine(*m_pDrawingScene);
    m_ptPosCheckmarkRightLine = QPointF();
    m_lineCheckmarkRightLine = QLineF();
    *m_pPhysValLineCheckmarkRightLine = CPhysValLine(*m_pDrawingScene);

    m_ptPosSmallRect = QPointF();
    m_sizeSmallRect = QSizeF();
    *m_pPhysValRectSmallRect = CPhysValRect(*m_pDrawingScene);
    m_physValAngleSmallRect = CPhysVal(0.0, Units.Angle.Degree, 0.1);
    m_ptPosSmallRectTopLine = QPointF();
    m_lineSmallRectTopLine = QLineF();
    *m_pPhysValLineSmallRectTopLine = CPhysValLine(*m_pDrawingScene);
    m_ptPosSmallRectRightLine = QPointF();
    m_lineSmallRectRightLine = QLineF();
    *m_pPhysValLineSmallRectRightLine = CPhysValLine(*m_pDrawingScene);
    m_ptPosSmallRectBottomLine = QPointF();
    m_lineSmallRectBottomLine = QLineF();
    *m_pPhysValLineSmallRectBottomLine = CPhysValLine(*m_pDrawingScene);
    m_ptPosSmallRectLeftLine = QPointF();
    m_lineSmallRectLeftLine = QLineF();
    *m_pPhysValLineSmallRectLeftLine = CPhysValLine(*m_pDrawingScene);

    m_ptPosTopGroup = QPointF();
    m_sizeTopGroup = QSizeF();
    *m_pPhysValRectTopGroup = CPhysValRect(*m_pDrawingScene);
    m_physValAngleTopGroup = CPhysVal(0.0, Units.Angle.Degree, 0.1);
}

//------------------------------------------------------------------------------
void CTest::initObjectCoors()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
SErrResultInfo CTest::readFile(const QString& i_strAbsFilePath, QStringList& o_strlstLines) const
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), "readFile(" + i_strAbsFilePath + ")");
    QFile file;
    if (i_strAbsFilePath.isEmpty()) {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidFileName);
    }
    else {
        QFileInfo fileInfo(i_strAbsFilePath);
        file.setFileName(i_strAbsFilePath);
        if (!file.open(QIODevice::ReadOnly)) {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileOpenForRead);
            errResultInfo.setAddErrInfoDscr(i_strAbsFilePath);
        }
    }
    if (!errResultInfo.isErrorResult()) {
        QTextStream in(&file);
        QString line;
        while (in.readLineInto(&line)) {
            o_strlstLines << line;
        }
    }
    return errResultInfo;
}

//------------------------------------------------------------------------------
QStringList CTest::resultValuesForGraphObj(const CGraphObj* i_pGraphObj, bool i_bAddLabelResultValues) const
//------------------------------------------------------------------------------
{
    QStringList strlstResultValues;
    if (i_pGraphObj != nullptr) {
        const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(i_pGraphObj);
        if (i_pGraphObj->isGroup()) {
            const CGraphObjGroup* pGraphObjGroup = dynamic_cast<const CGraphObjGroup*>(i_pGraphObj);
            if (pGraphObjGroup != nullptr) {
                strlstResultValues = resultValuesForGroup(
                    i_pGraphObj->name(), pGraphicsItem->pos(), pGraphObjGroup->getRect());
            }
        }
        else if (i_pGraphObj->isLine()) {
            const CGraphObjLine* pGraphObjLine = dynamic_cast<const CGraphObjLine*>(i_pGraphObj);
            if (pGraphObjLine != nullptr) {
                strlstResultValues = resultValuesForLine(
                    i_pGraphObj->name(), pGraphicsItem->pos(),
                    pGraphObjLine->line(), pGraphObjLine->getLine());
            }
        }
        if (i_bAddLabelResultValues) {
            QStringList strlstLabelNames = i_pGraphObj->getLabelNames();
            for (const QString& strLabelName : strlstLabelNames) {
                CGraphObjLabel* pGraphObjLabel = i_pGraphObj->getLabel(strLabelName);
                if (pGraphObjLabel != nullptr) {
                    pGraphicsItem = dynamic_cast<const QGraphicsItem*>(pGraphObjLabel);
                    strlstResultValues += resultValuesForLabel(
                        i_pGraphObj->name() + "." + pGraphObjLabel->name(),
                        pGraphicsItem->pos(), pGraphObjLabel->text());
                }
            }
            strlstLabelNames = i_pGraphObj->getGeometryLabelNames();
            for (const QString& strLabelName : strlstLabelNames) {
                CGraphObjLabel* pGraphObjLabel = i_pGraphObj->getGeometryLabel(strLabelName);
                if (pGraphObjLabel != nullptr) {
                    pGraphicsItem = dynamic_cast<const QGraphicsItem*>(pGraphObjLabel);
                    strlstResultValues += resultValuesForLabel(
                        i_pGraphObj->name() + "." + pGraphObjLabel->name(),
                        pGraphicsItem->pos(), pGraphObjLabel->text());
                }
            }
        }
    }
    return strlstResultValues;
}

//------------------------------------------------------------------------------
QStringList CTest::resultValuesForGroup(
    const QString& strGraphObjName, const QPointF& i_pos,
    const CPhysValRect& i_physValRect) const
//------------------------------------------------------------------------------
{
    QSizeF size = m_pDrawingScene->convert(i_physValRect.size(), Units.Length.px).toQSizeF();
    QRectF rctBounding(QPointF(-size.width()/2.0, -size.height()/2.0), size);
    return QStringList({
        strGraphObjName + ".pos {" + qPoint2Str(i_pos) + "} px",
        strGraphObjName + ".boundingRect {" + qRect2Str(rctBounding) + "} px",
        strGraphObjName + ".position {" + i_physValRect.center().toString() + "} " + i_physValRect.unit().symbol(),
        strGraphObjName + ".getRect {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol(),
        strGraphObjName + ".getSize {" + i_physValRect.size().toString() + "} " + i_physValRect.unit().symbol(),
        strGraphObjName + ".rotationAngle: " + i_physValRect.angle().toString()
    });
}

//------------------------------------------------------------------------------
QStringList CTest::resultValuesForLine(
    const QString& strGraphObjName, const QPointF& i_pos,
    const QLineF& i_line, const CPhysValLine& i_physValLine) const
//------------------------------------------------------------------------------
{
    QRectF rctBounding(i_line.p1(), i_line.p2());
    return QStringList({
        strGraphObjName + ".pos {" + qPoint2Str(i_pos) + "} px",
        strGraphObjName + ".boundingRect {" + qRect2Str(rctBounding) + "} px",
        strGraphObjName + ".line {" + qLine2Str(i_line) + "} px",
        strGraphObjName + ".position {" + i_physValLine.center().toString() + "} " + i_physValLine.unit().symbol(),
        strGraphObjName + ".getLine {" + i_physValLine.toString() + "} " + i_physValLine.unit().symbol(),
        strGraphObjName + ".getLength {" + i_physValLine.length().toString() + "}",
        strGraphObjName + ".rotationAngle: " + i_physValLine.angle().toString()
    });
}

//------------------------------------------------------------------------------
QStringList CTest::resultValuesForLabel(
    const QString& strGraphObjName, const QPointF& i_pos, const QString& i_strText) const
//------------------------------------------------------------------------------
{
    QStringList strlstResultValues;
    if (!i_pos.isNull()) {
        strlstResultValues = QStringList({
            strGraphObjName + ".pos {" + qPoint2Str(i_pos) + "} px"});
    }
    strlstResultValues.append(strGraphObjName + ".text: " + i_strText);
    return strlstResultValues;
}
