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
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryAngle.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryDX.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryDY.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryLength.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryPosition.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPolygon.h"
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
#include <QtGui/qapplication.h>
#include <QtGui/qaction.h>
#else
#include <QtWidgets/qapplication.h>
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
    ZS::Test::CTest("Apps-Products-Draw"),
    m_physValAngleSmallPlusSign(0.0, Units.Angle.Degree, 0.1),
    m_physValAngleBigPlusSign(0.0, Units.Angle.Degree, 0.1),
    m_physValAngleCheckmark(0.0, Units.Angle.Degree, 0.1),
    m_physValAngleSmallRect(0.0, Units.Angle.Degree, 0.1),
    m_physValAngleTopGroup(0.0, Units.Angle.Degree, 0.1)
{
    m_pTrcAdminObjDrawTestSteps = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), "theTest");

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    SErrResultInfo errResultInfo = saveTestSteps();
    if (errResultInfo.isErrorResult()) {
        if(CErrLog::GetInstance() != nullptr) {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    mthTracer.onAdminObjAboutToBeReleased();
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjDrawTestSteps);
    m_pTrcAdminObjDrawTestSteps = nullptr;

    delete m_pPhysValLineSmallPlusSignVerticalLine;
    m_pPhysValLineSmallPlusSignVerticalLine = nullptr;
    delete m_pPhysValLineSmallPlusSignHorizontalLine;
    m_pPhysValLineSmallPlusSignHorizontalLine = nullptr;

    delete m_pPhysValLineBigPlusSignVerticalLine;
    m_pPhysValLineBigPlusSignVerticalLine = nullptr;
    delete m_pPhysValLineBigPlusSignHorizontalLine;
    m_pPhysValLineBigPlusSignHorizontalLine = nullptr;

    delete m_pPhysValLineCheckmarkLeftLine;
    m_pPhysValLineCheckmarkLeftLine = nullptr;
    delete m_pPhysValLineCheckmarkRightLine;
    m_pPhysValLineCheckmarkRightLine = nullptr;

    delete m_pPhysValLineSmallRectTopLine;
    m_pPhysValLineSmallRectTopLine = nullptr;
    delete m_pPhysValLineSmallRectRightLine;
    m_pPhysValLineSmallRectRightLine = nullptr;
    delete m_pPhysValLineSmallRectBottomLine;
    m_pPhysValLineSmallRectBottomLine = nullptr;
    delete m_pPhysValLineSmallRectLeftLine;
    m_pPhysValLineSmallRectLeftLine = nullptr;

    delete m_pPhysValRectRectangle;
    m_pPhysValRectRectangle = nullptr;
    delete m_pPhysValRectBigCrossVerticalBar;
    m_pPhysValRectBigCrossVerticalBar = nullptr;
    delete m_pPhysValRectBigCrossHorizontalBar;
    m_pPhysValRectBigCrossHorizontalBar = nullptr;
    delete m_pPhysValRectSmallCross1VerticalBar;
    m_pPhysValRectSmallCross1VerticalBar = nullptr;
    delete m_pPhysValRectSmallCross1HorizontalBar;
    m_pPhysValRectSmallCross1HorizontalBar = nullptr;
    delete m_pPhysValRectSmallCross2VerticalBar;
    m_pPhysValRectSmallCross2VerticalBar = nullptr;
    delete m_pPhysValRectSmallCross2HorizontalBar;
    m_pPhysValRectSmallCross2HorizontalBar = nullptr;

    delete m_pPhysValRectEllipse;
    m_pPhysValRectEllipse = nullptr;

    delete m_pPhysValRectText;
    m_pPhysValRectText = nullptr;

    delete m_pPhysValPolygonOpenArrow;
    m_pPhysValPolygonOpenArrow = nullptr;
    delete m_pPhysValPolygonTriangle;
    m_pPhysValPolygonTriangle = nullptr;
    delete m_pPhysValPolygonStar;
    m_pPhysValPolygonStar = nullptr;

    delete m_pPhysValConnectionPoint1;
    m_pPhysValConnectionPoint1 = nullptr;
    delete m_pPhysValConnectionPoint2;
    m_pPhysValConnectionPoint2 = nullptr;
    delete m_pPhysValConnectionPoint3;
    m_pPhysValConnectionPoint3 = nullptr;
    delete m_pPhysValConnectionPoint4;
    m_pPhysValConnectionPoint4 = nullptr;

    delete m_pPhysValPolygonConnectionLineCnctPt1CnctPt2;
    m_pPhysValPolygonConnectionLineCnctPt1CnctPt2 = nullptr;
    delete m_pPhysValPolygonConnectionLineCnctPt3CnctPt4;
    m_pPhysValPolygonConnectionLineCnctPt3CnctPt4 = nullptr;
    delete m_pPhysValPolygonConnectionLineCnctPt1CnctPt4;
    m_pPhysValPolygonConnectionLineCnctPt1CnctPt4 = nullptr;
    delete m_pPhysValPolygonConnectionLineCnctPt3CnctPt2;
    m_pPhysValPolygonConnectionLineCnctPt3CnctPt2 = nullptr;

    delete m_pPhysValRectSmallPlusSign;
    m_pPhysValRectSmallPlusSign = nullptr;
    delete m_pPhysValRectBigPlusSign;
    m_pPhysValRectBigPlusSign = nullptr;
    delete m_pPhysValRectCheckmark;
    m_pPhysValRectCheckmark = nullptr;
    delete m_pPhysValRectSmallRect;
    m_pPhysValRectSmallRect = nullptr;

    delete m_pPhysValRectCrosses;
    m_pPhysValRectCrosses = nullptr;
    delete m_pPhysValRectBigCross;
    m_pPhysValRectBigCross = nullptr;
    delete m_pPhysValRectSmallCross1;
    m_pPhysValRectSmallCross1 = nullptr;
    delete m_pPhysValRectSmallCross2;
    m_pPhysValRectSmallCross2 = nullptr;
    delete m_pPhysValRectPolygons;
    m_pPhysValRectPolygons = nullptr;
    delete m_pPhysValRectConnectionPoints;
    m_pPhysValRectConnectionPoints = nullptr;
    delete m_pPhysValRectTopGroup;
    m_pPhysValRectTopGroup = nullptr;

    m_pMainWindow = nullptr;
    m_pDrawingView = nullptr;
    m_pDrawingScene = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::setMainWindow( CMainWindow* i_pMainWindow )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pMainWindow == nullptr ? "null" : i_pMainWindow->objectName());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMainWindow",
        /* strAddInfo   */ strMthInArgs );

    m_pMainWindow = i_pMainWindow;
    m_pDrawingView = CWidgetCentral::GetInstance()->drawingView();
    m_pDrawingScene = CWidgetCentral::GetInstance()->drawingScene();

    // Lines
    //------

    m_pPhysValLineSmallPlusSignVerticalLine = new CPhysValLine(*m_pDrawingScene);
    m_pPhysValLineSmallPlusSignHorizontalLine = new CPhysValLine(*m_pDrawingScene);

    m_pPhysValLineBigPlusSignVerticalLine = new CPhysValLine(*m_pDrawingScene);
    m_pPhysValLineBigPlusSignHorizontalLine = new CPhysValLine(*m_pDrawingScene);

    m_pPhysValLineCheckmarkLeftLine = new CPhysValLine(*m_pDrawingScene);
    m_pPhysValLineCheckmarkRightLine = new CPhysValLine(*m_pDrawingScene);

    m_pPhysValLineSmallRectTopLine = new CPhysValLine(*m_pDrawingScene);
    m_pPhysValLineSmallRectRightLine = new CPhysValLine(*m_pDrawingScene);
    m_pPhysValLineSmallRectBottomLine = new CPhysValLine(*m_pDrawingScene);
    m_pPhysValLineSmallRectLeftLine = new CPhysValLine(*m_pDrawingScene);

    // Rectangles
    //-----------
    m_pPhysValRectRectangle = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectBigCrossVerticalBar = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectBigCrossHorizontalBar = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectSmallCross1VerticalBar = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectSmallCross1HorizontalBar = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectSmallCross2VerticalBar = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectSmallCross2HorizontalBar = new CPhysValRect(*m_pDrawingScene);

    // Ellipses
    //----------
    m_pPhysValRectEllipse = new CPhysValRect(*m_pDrawingScene);

    // Texts
    //----------
    m_pPhysValRectText = new CPhysValRect(*m_pDrawingScene);

    // Polygons
    //----------
    m_pPhysValPolygonOpenArrow = new CPhysValPolygon(*m_pDrawingScene);
    m_pPhysValPolygonTriangle = new CPhysValPolygon(*m_pDrawingScene);
    m_pPhysValPolygonStar = new CPhysValPolygon(*m_pDrawingScene);

    // Connection Points
    //------------------
    m_pPhysValConnectionPoint1 = new CPhysValPoint(*m_pDrawingScene);
    m_pPhysValConnectionPoint2 = new CPhysValPoint(*m_pDrawingScene);
    m_pPhysValConnectionPoint3 = new CPhysValPoint(*m_pDrawingScene);
    m_pPhysValConnectionPoint4 = new CPhysValPoint(*m_pDrawingScene);

    // Connection Lines
    //-----------------
    m_pPhysValPolygonConnectionLineCnctPt1CnctPt2 = new CPhysValPolygon(*m_pDrawingScene);
    m_pPhysValPolygonConnectionLineCnctPt3CnctPt4 = new CPhysValPolygon(*m_pDrawingScene);
    m_pPhysValPolygonConnectionLineCnctPt1CnctPt4 = new CPhysValPolygon(*m_pDrawingScene);
    m_pPhysValPolygonConnectionLineCnctPt3CnctPt2 = new CPhysValPolygon(*m_pDrawingScene);

    // Groups
    //-------
    m_pPhysValRectSmallPlusSign = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectBigPlusSign = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectCheckmark = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectSmallRect = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectCrosses = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectBigCross = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectSmallCross1 = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectSmallCross2 = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectPolygons = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectConnectionPoints = new CPhysValRect(*m_pDrawingScene);
    m_pPhysValRectTopGroup = new CPhysValRect(*m_pDrawingScene);

    CDrawGridSettings gridSettings;
    gridSettings.setLinesVisible(true);
    gridSettings.setLinesDistMin(20);
    gridSettings.setLabelsVisible(true);
    gridSettings.setLabelsFont(QFont("Terminal"));

#if TEST_AUX_METHODS == 1
    createTestGroupAuxMethods(nullptr);
#endif
#if TEST_DRAWINGSIZE == 1
    createTestGroupDrawingSize(nullptr);
#endif

    // Pixels Drawings
    //----------------

#if TEST_PIXELS_DRAWINGS == 1

    CDrawingSize drawingSize;
    drawingSize.setDimensionUnit(EScaleDimensionUnit::Pixels);
    drawingSize.setScreenResolutionInPxPerMM(1.0);

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Pixels Drawing",
        /* pTSGrpParent */ nullptr );

    // For 100x100 pixel drawing we want to show that the right and bottom lines don't become visible.
    // But for this the painter must not use Antialiasing.
    const QMap<EGraphObjType, QPainter::RenderHints> mapGraphObjTypeRenderHints({
        {EGraphObjTypeLine, QPainter::RenderHints()}
    });

#if TEST_PIXELS_DRAWINGS_100x100 == 1
    drawingSize.setImageSize(CPhysVal(100, Units.Length.px), CPhysVal(100, Units.Length.px));
    // Must be set before creating test groups and test steps as the methods access the drawing scenes
    // drawing size to name test groups and steps and to parametrize the test steps.
    m_pDrawingScene->setDrawingSize(drawingSize);

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing100x100px = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ImageSize(" + qSize2Str(drawingSize.imageSizeInPixels()) + " " + drawingSize.unit().symbol(),
        /* pTSGrpParent */ pGrpPixelsDrawing );

    createTestGroupPrepareScene(pGrpPixelsDrawing100x100px, drawingSize, gridSettings, mapGraphObjTypeRenderHints);

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing100x100pxObjectCoordinates = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Object Coordinates",
        /* pTSGrpParent */ pGrpPixelsDrawing100x100px );
    createTestGroupObjectCoordinatesAddLines(pGrpPixelsDrawing100x100pxObjectCoordinates);
#endif // TEST_PIXELS_DRAWINGS_100x100

#if TEST_PIXELS_DRAWINGS_101x101 == 1
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
#endif // TEST_PIXELS_DRAWINGS_101x101

#if TEST_PIXELS_DRAWINGS_800x600 == 1
    drawingSize.setImageSize(CPhysVal(800, Units.Length.px, 1.0), CPhysVal(600, Units.Length.px, 1.0));
    // Must be set before creating test groups and test steps as the methods access the drawing scenes
    // drawing size to name test groups and steps and to parametrize the test steps.
    m_pDrawingScene->setDrawingSize(drawingSize);

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing800x600px = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ImageSize(" + qSize2Str(drawingSize.imageSizeInPixels()) + " " + drawingSize.unit().symbol(),
        /* pTSGrpParent */ pGrpPixelsDrawing );

    createTestGroupPrepareScene(pGrpPixelsDrawing800x600px, drawingSize, gridSettings);

#if TEST_PIXELS_DRAWINGS_800x600_OJBECT_COORDINATES == 1
    ZS::Test::CTestStepGroup* pGrpPixelsDrawing800x600pxObjectCoordinates = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Object Coordinates",
        /* pTSGrpParent */ pGrpPixelsDrawing800x600px );
    createTestGroupObjectCoordinatesTransformPhysValShapes(pGrpPixelsDrawing800x600pxObjectCoordinates);
#endif // TEST_PIXELS_DRAWINGS_OJBECT_COORDINATES

#if TEST_PIXELS_DRAWINGS_800x600_ADD_OBJECTS == 1
    ZS::Test::CTestStepGroup* pGrpPixelsDrawing800x600pxAddObjects = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Add Objects",
        /* pTSGrpParent */ pGrpPixelsDrawing800x600px );
    createTestGroupAddObjects(pGrpPixelsDrawing800x600pxAddObjects);
#endif // TEST_PIXELS_DRAWINGS_ADD_OBJECTS

#if TEST_PIXELS_DRAWINGS_800x600_DRAW_OBJECTS == 1
    ZS::Test::CTestStepGroup* pGrpPixelsDrawing800x600pxDrawObjects = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Objects",
        /* pTSGrpParent */ pGrpPixelsDrawing800x600px );
    createTestGroupDrawObjects(pGrpPixelsDrawing800x600pxDrawObjects);
#endif // TEST_PIXELS_DRAWINGS_DRAW_OBJECTS
#endif // TEST_PIXELS_DRAWINGS_800x600
#endif // TEST_PIXELS_DRAWINGS

    // Metrics Drawings
    //-----------------

#if TEST_METRICS_DRAWINGS == 1

    drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
    drawingSize.setMetricUnit(Units.Length.mm);
    drawingSize.setMetricImageCoorsDecimals(1);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawing = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Metrics Drawing",
        /* pTSGrpParent */ nullptr );

    // YScaleTopDown
    //--------------

#if TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN == 1

    drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::TopDown);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Y-Scale TopDown",
        /* pTSGrpParent */ pGrpMetricsDrawing );

#if TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_100x100 == 1
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
#endif // TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_100x100

#if TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600 == 1
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

#if TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600_OJBECT_COORDINATES == 1
    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown800x600mmObjectCoordinates = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Object Coordinates",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleTopDown800x600mm );
    createTestGroupObjectCoordinatesTransformPhysValShapes(pGrpMetricsDrawingYScaleTopDown800x600mmObjectCoordinates);
#endif // TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600_OJBECT_COORDINATES

#if TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600_ADD_OBJECTS == 1
    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown800x600mmAddObjects = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Add Objects",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleTopDown800x600mm );
    createTestGroupAddObjects(pGrpMetricsDrawingYScaleTopDown800x600mmAddObjects);
#endif // TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600_ADD_OBJECTS

#if TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600_DRAW_OBJECTS == 1
    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown800x600mmDrawObjects = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Objects",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleTopDown800x600mm );
    createTestGroupDrawObjects(pGrpMetricsDrawingYScaleTopDown800x600mmDrawObjects);
#endif // TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600_DRAW_OBJECTS
#endif // TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600
#endif // TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN

    // YScaleBottomUp
    //---------------

#if TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP == 1

    drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::BottomUp);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Y-Scale BottomUp",
        /* pTSGrpParent */ pGrpMetricsDrawing );

#if TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_100x100 == 1
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
#endif // TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_100x100

#if TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600 == 1
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

#if TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600_OJBECT_COORDINATES == 1
    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp800x600mmObjectCoordinates = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Object Coordinates",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleBottomUp800x600mm );
    createTestGroupObjectCoordinatesTransformPhysValShapes(pGrpMetricsDrawingYScaleBottomUp800x600mmObjectCoordinates);
#endif // TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600_OJBECT_COORDINATES

#if TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600_ADD_OBJECTS == 1
    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp800x600mmAddObjects = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Add Objects",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleBottomUp800x600mm );
    createTestGroupAddObjects(pGrpMetricsDrawingYScaleBottomUp800x600mmAddObjects);
#endif // TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600_ADD_OBJECTS

#if TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600_DRAW_OBJECTS == 1
    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp800x600mmDrawObjects = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Objects",
        /* pTSGrpParent */ pGrpMetricsDrawingYScaleBottomUp800x600mm );
    createTestGroupDrawObjects(pGrpMetricsDrawingYScaleBottomUp800x600mmDrawObjects);
#endif // TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600_DRAW_OBJECTS
#endif // TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600
#endif // TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP
#endif // TEST_METRICS_DRAWINGS == 1

    // Recall test step settings
    //--------------------------

    recallTestSteps();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::createTestGroupAuxMethods(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAuxMethods",
        /* strAddInfo   */ strMthInArgs );

    QStringList strlstExpectedValues;

    ZS::Test::CTestStepGroup* pGrpAuxMethods = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Auxiliary Math Methods",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // getLineFromPolar
    //-----------------

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " getLineFromPolar",
        /* strOperation    */ "Draw::getLineFromPolar",
        /* pGrpParent      */ pGrpAuxMethods,
        /* szDoTestStepFct */ SLOT(doTestStepGetLineFromPolar(ZS::Test::CTestStep*)) );
    int iPrecision = 6;
    strlstExpectedValues.clear();
    pTestStep->addDataRow({
        {"Length", "100.0 px"},
        {"Angle", "45.0 " + QString(Math::c_chSymbolDegree)},
        {"Line", QLineF(QPointF(100.0, 100.0), QPointF(200.0, 100.0))},
        {"ResultValuesPrecision", iPrecision}
    });
    strlstExpectedValues.append(qLine2Str(QLineF(QPointF(100.0, 100.0), QPointF(170.710678, 170.710678)), ", ", 'f', iPrecision));
    pTestStep->addDataRow({
        {"Length", "100.0 px"},
        {"Angle", "315.0 " + QString(Math::c_chSymbolDegree)},
        {"Line", QLineF(QPointF(100.0, 100.0), QPointF(100.0, 200.0))},
        {"ResultValuesPrecision", iPrecision}
    });
    strlstExpectedValues.append(qLine2Str(QLineF(QPointF(100.0, 100.0), QPointF(170.710678, 170.710678)), ", ", 'f', iPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // getPolarCoors
    //--------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " getPolarCoors",
        /* strOperation    */ "Draw::getPolarCoors",
        /* pGrpParent      */ pGrpAuxMethods,
        /* szDoTestStepFct */ SLOT(doTestStepGetPolarCoors(ZS::Test::CTestStep*)) );
    SPolarCoors polarCoors;
    strlstExpectedValues.clear();
    pTestStep->addDataRow({
        {"Line", QLineF(QPointF(100.0, 100.0), QPointF(200.0, 100.0))},
        {"Point", QPointF(100.0, 50.0)}
    });
    polarCoors = SPolarCoors(50.0, 270.0);
    strlstExpectedValues.append(polarCoors.toString(true, ", "));
    pTestStep->addDataRow({
        {"Line", QLineF(QPointF(100.0, 100.0), QPointF(200.0, 200.0))},
        {"Point", QPointF(200.0, 100.0)}
    });
    polarCoors = SPolarCoors(100.0, 315.0);
    strlstExpectedValues.append(polarCoors.toString(true, ", "));
    pTestStep->addDataRow({
        {"Line", QLineF(QPointF(100.0, 100.0), QPointF(200.0, 200.0))},
        {"Point", QPointF(100.0, 200.0)}
    });
    polarCoors = SPolarCoors(100.0, 45.0);
    strlstExpectedValues.append(polarCoors.toString(true, ", "));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // getPerpendicularLine
    //---------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " getPerpendicularLine",
        /* strOperation    */ "Draw::getPerpendicularLine",
        /* pGrpParent      */ pGrpAuxMethods,
        /* szDoTestStepFct */ SLOT(doTestStepGetPerpendicularLine(ZS::Test::CTestStep*)) );
    QLineF perpendicularLine;
    strlstExpectedValues.clear();
    pTestStep->addDataRow({
        {"Line", QLineF(QPointF(100.0, 100.0), QPointF(200.0, 100.0))},
        {"Point", QPointF(100.0, 200.0)},
        {"MinLength", 100.0},
        {"ResultValuesPrecision", iPrecision}
    });
    strlstExpectedValues.append(qLine2Str(QLineF(QPointF(100.0, 200.0), QPointF(100.0, 100.0)), ", ", 'f', iPrecision));
    pTestStep->addDataRow({
        {"Line", QLineF(QPointF(100.0, 100.0), QPointF(200.0, 100.0))},
        {"Point", QPointF(150.0, 200.0)},
        {"MinLength", 100.0},
        {"ResultValuesPrecision", iPrecision}
    });
    strlstExpectedValues.append(qLine2Str(QLineF(QPointF(150.0, 200.0), QPointF(150.0, 100.0)), ", ", 'f', iPrecision));
    pTestStep->addDataRow({
        {"Line", QLineF(QPointF(100.0, 100.0), QPointF(200.0, 200.0))},
        {"Point", QPointF(200.0, 100.0)},
        {"MinLength", 100.0},
        {"ResultValuesPrecision", iPrecision}
    });
    strlstExpectedValues.append(qLine2Str(QLineF(QPointF(200.0, 100.0), QPointF(129.289322, 170.710678)), ", ", 'f', iPrecision));
    pTestStep->addDataRow({
        {"Line", QLineF(QPointF(100.0, 100.0), QPointF(200.0, 200.0))},
        {"Point", QPointF(100.0, 200.0)},
        {"MinLength", 100.0},
        {"ResultValuesPrecision", iPrecision}
    });
    strlstExpectedValues.append(qLine2Str(QLineF(QPointF(100.0, 200.0), QPointF(170.710678, 129.289322)), ", ", 'f', iPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawingSize(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
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
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
void CTest::createTestStepSaveLoadFile(ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int i_iResultValuesPrecision)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
    pTestStep->setConfigValue("ResultValuesPrecision", i_iResultValuesPrecision);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepGetLineFromPolar(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepGetLineFromPolar",
        /* strAddInfo   */ strMthInArgs );

    QStringList strlstResultValues;

    CPhysVal physValLength(Units.Length.px);
    CPhysVal physValAngle(Units.Angle.Degree);
    QLineF line;

    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow) {
        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);
        physValLength = dataRow["Length"].toString();
        physValAngle = dataRow["Angle"].toString();
        line = dataRow["Line"].toLineF();
        int iPrecision = 1;
        if (dataRow.contains("ResultValuesPrecision")) {
            iPrecision = dataRow["ResultValuesPrecision"].toInt();
        }
        double fLength_px = physValLength.getVal(Units.Length.px);
        double fAngle_degrees = physValAngle.getVal(Units.Angle.Degree);
        QLineF lineResult = ZS::Draw::getLineFromPolar(fLength_px, fAngle_degrees, line);
        strlstResultValues.append(qLine2Str(lineResult, ", ", 'f', iPrecision));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepGetPolarCoors(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepGetPolarCoors",
        /* strAddInfo   */ strMthInArgs );

    QStringList strlstResultValues;
    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow) {
        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);
        QLineF line = dataRow["Line"].toLineF();
        QPointF pt = dataRow["Point"].toPointF();
        SPolarCoors polarCoors = ZS::Draw::getPolarCoors(line, pt);
        strlstResultValues.append(polarCoors.toString(true, ", "));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepGetPerpendicularLine(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepGetPerpendicularLine",
        /* strAddInfo   */ strMthInArgs );

    QStringList strlstResultValues;
    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow) {
        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);
        QLineF line = dataRow["Line"].toLineF();
        QPointF pt = dataRow["Point"].toPointF();
        double fMinLength  = dataRow["MinLength"].toDouble();
        int iPrecision = 1;
        if (dataRow.contains("ResultValuesPrecision")) {
            iPrecision = dataRow["ResultValuesPrecision"].toInt();
        }
        QLineF lineResult = ZS::Draw::getPerpendicularLine(line, pt, fMinLength);
        strlstResultValues.append(qLine2Str(lineResult, ", ", 'f', iPrecision));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepDrawingSize(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
void CTest::doTestStepMainWindowSetGeometry(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
void CTest::doTestStepClearDrawingScene(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepClearDrawingScene",
        /* strAddInfo   */ strMthInArgs );

    m_pDrawingScene->clear();
    m_pDrawingScene->removeAndDeleteAllPhysValShapes();

    initInstCounts();
    initObjectCoors();

    i_pTestStep->setResultValue("");
}

//------------------------------------------------------------------------------
void CTest::doTestStepSetDrawingSize(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
void CTest::doTestStepSetGridSettings(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
void CTest::doTestStepSetPainterRenderHints(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepSetPainterRenderHints",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjLine::resetPainterRenderHints();
    CGraphObjRect::resetPainterRenderHints();
    CGraphObjEllipse::resetPainterRenderHints();
    CGraphObjPolygon::resetPainterRenderHints();
    CGraphObjText::resetPainterRenderHints();
    //CGraphObjImage::resetPainterRenderHints();
    //CGraphObjConnectionPoint::resetPainterRenderHints();
    //CGraphObjConnectionLine::resetPainterRenderHints();
    CGraphObjGroup::resetPainterRenderHints();
    //CGraphObjSelectionPoint::resetPainterRenderHints();
    CGraphObjLabel::resetPainterRenderHints();
    CGraphObjLabelGeometryPosition::resetPainterRenderHints();
    CGraphObjLabelGeometryDX::resetPainterRenderHints();
    CGraphObjLabelGeometryDY::resetPainterRenderHints();
    CGraphObjLabelGeometryLength::resetPainterRenderHints();
    CGraphObjLabelGeometryAngle::resetPainterRenderHints();

    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow) {
        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);
        QString strGraphObjType = dataRow["GraphObjType"].toString();
        EGraphObjType graphObjType = str2GraphObjType(strGraphObjType);
        QPainter::RenderHints uPainterRenderHints = dataRow["PainterRenderHints"].value<QPainter::RenderHints>();
        if (graphObjType == EGraphObjTypeLine) {
            CGraphObjLine::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeRect) {
            CGraphObjRect::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeEllipse) {
            CGraphObjEllipse::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypePolygon || graphObjType == EGraphObjTypePolyline) {
            CGraphObjPolygon::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeText) {
            CGraphObjText::setPainterRenderHints(uPainterRenderHints);
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
            CGraphObjLabel::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeLabelGeometryPosition) {
            CGraphObjLabelGeometryPosition::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeLabelGeometryDX) {
            CGraphObjLabelGeometryDX::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeLabelGeometryDY) {
            CGraphObjLabelGeometryDY::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeLabelGeometryLength) {
            CGraphObjLabelGeometryLength::setPainterRenderHints(uPainterRenderHints);
        }
        else if (graphObjType == EGraphObjTypeLabelGeometryAngle) {
            CGraphObjLabelGeometryAngle::setPainterRenderHints(uPainterRenderHints);
        }
    }
    i_pTestStep->setResultValue("");
}

//------------------------------------------------------------------------------
void CTest::doTestStepTransformPhysValRect(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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

    // Draw original shape in blue color
    if (i_pTestStep->hasConfigValue("addPhysValShape")) {
        CDrawSettings drawSettings(EGraphObjTypeRect);
        drawSettings.setPenColor(Qt::blue);
        CPhysValRect* pPhysValRect = new CPhysValRect(physValRectResult);
        m_pDrawingScene->addPhysValShape(pPhysValRect, drawSettings);
        pPhysValRect = nullptr;
    }

    // Draw rotated shape in green color
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

    // Apply further methods and draw resulting shape in red color
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
        int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
            i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;

        strlstResultValues.append(
            "Center {" + physValRectResult.center().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValRectResult.center().unit().symbol());
        strlstResultValues.append(
            "Width: " + QString::number(Math::round2Nearest(physValRectResult.width().getVal(), iResultValuesPrecision), 'f', iResultValuesPrecision) + " " +
            physValRectResult.width().unit().symbol());
        strlstResultValues.append(
            "Height: " + QString::number(Math::round2Nearest(physValRectResult.height().getVal(), iResultValuesPrecision), 'f', iResultValuesPrecision) + " " +
            physValRectResult.height().unit().symbol());
        strlstResultValues.append("Angle: " + physValRectResult.angle().toString());
        strlstResultValues.append(
            "TopLeft {" + physValRectResult.topLeft().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValRectResult.topLeft().unit().symbol());
        strlstResultValues.append(
            "TopRight {" + physValRectResult.topRight().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValRectResult.topRight().unit().symbol());
        strlstResultValues.append(
            "BottomRight {" + physValRectResult.bottomRight().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValRectResult.bottomRight().unit().symbol());
        strlstResultValues.append(
            "BottomLeft {" + physValRectResult.bottomLeft().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValRectResult.bottomLeft().unit().symbol());
        strlstResultValues.append(
            "TopCenter {" + physValRectResult.topCenter().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValRectResult.topCenter().unit().symbol());
        strlstResultValues.append(
            "RightCenter {" + physValRectResult.rightCenter().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValRectResult.rightCenter().unit().symbol());
        strlstResultValues.append(
            "BottomCenter {" + physValRectResult.bottomCenter().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValRectResult.bottomCenter().unit().symbol());
        strlstResultValues.append(
            "LeftCenter {" + physValRectResult.leftCenter().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValRectResult.leftCenter().unit().symbol());
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepTransformPhysValPolygon(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepTransformPhysValPolygon",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysValPolygon physValPolygonResult(*m_pDrawingScene);
    CPhysValPoint physValPointTaken(*m_pDrawingScene);

    if (i_pTestStep->hasConfigValue("removeAndDeleteAllPhysValShapes")) {
        m_pDrawingScene->removeAndDeleteAllPhysValShapes();
    }

    QString strMethod = "create";
    if (i_pTestStep->hasConfigValue(strMethod + ".numberOfPoints")) {
        int iNumberOfPoints = i_pTestStep->getConfigValue(strMethod + ".numberOfPoints").toInt();
        QPolygonF polygon;
        for (int idxPt = 0; idxPt < iNumberOfPoints; ++idxPt) {
            polygon.append(i_pTestStep->getConfigValue(strMethod + ".P" + QString::number(idxPt)).toPointF());
        }
        if (i_pTestStep->hasConfigValue(strMethod + ".unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue(strMethod + ".unit").toString();
            CUnit unit = strUnitPoint;
            physValPolygonResult = CPhysValPolygon(*m_pDrawingScene, polygon, unit);
        }
        else {
            physValPolygonResult = CPhysValPolygon(*m_pDrawingScene, polygon);
        }
        if (i_pTestStep->hasConfigValue("addPhysValShape")) {
            CDrawSettings drawSettings(EGraphObjTypePolygon);
            drawSettings.setPenColor(QColor(i_pTestStep->getConfigValue("addPhysValShape").toString()));
            CPhysValPolygon* pPhysValPolygon = new CPhysValPolygon(physValPolygonResult);
            m_pDrawingScene->addPhysValShape(pPhysValPolygon, drawSettings);
            pPhysValPolygon = nullptr;
        }

        int idxMethod = 0;
        bool bHasMethod = true;
        while (bHasMethod) {
            bHasMethod = false;
            QString strIdxMethod = "_" + QString::number(idxMethod);
            strMethod = "setAngle" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                CPhysVal physValAngle;
                if (i_pTestStep->hasConfigValue(strMethod + ".unit")) {
                    QString strUnit = i_pTestStep->getConfigValue(strMethod + ".unit").toString();
                    CUnit unit = strUnit;
                    physValAngle = CPhysVal(i_pTestStep->getConfigValue(strMethod).toDouble(), unit);
                }
                else {
                    physValAngle = i_pTestStep->getConfigValue(strMethod).toString();
                }
                physValAngle.setRes(0.1);
                physValPolygonResult.setAngle(physValAngle);
            }
            strMethod = "setCenter" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                QPointF pt = i_pTestStep->getConfigValue(strMethod).toPointF();
                if (i_pTestStep->hasConfigValue(strMethod + ".unit")) {
                    QString strUnitPoint = i_pTestStep->getConfigValue(strMethod + ".unit").toString();
                    CUnit unit = strUnitPoint;
                    CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
                    physValPolygonResult.setCenter(physValPt);
                }
                else {
                    CPhysValPoint physValPt(*m_pDrawingScene, pt);
                    physValPolygonResult.setCenter(physValPt);
                }
            }
            strMethod = "setSize" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                QSizeF sizeF = i_pTestStep->getConfigValue(strMethod).toSizeF();
                if (i_pTestStep->hasConfigValue(strMethod + ".unit")) {
                    QString strUnitPoint = i_pTestStep->getConfigValue(strMethod + ".unit").toString();
                    CUnit unit = strUnitPoint;
                    CPhysValSize physValSize(*m_pDrawingScene, sizeF, unit);
                    physValPolygonResult.setSize(physValSize);
                }
                else {
                    CPhysValSize physValSize(*m_pDrawingScene, sizeF);
                    physValPolygonResult.setSize(physValSize);
                }
            }
            strMethod = "setWidth" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                CPhysVal physValWidth;
                if (i_pTestStep->hasConfigValue(strMethod + ".unit")) {
                    QString strUnit = i_pTestStep->getConfigValue(strMethod + ".unit").toString();
                    CUnit unit = strUnit;
                    physValWidth = CPhysVal(i_pTestStep->getConfigValue(strMethod).toDouble(), unit);
                }
                else {
                    physValWidth = i_pTestStep->getConfigValue(strMethod).toString();
                }
                physValPolygonResult.setWidth(physValWidth);
            }
            strMethod = "setWidthByMovingLeftCenter" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                QPointF pt = i_pTestStep->getConfigValue(strMethod).toPointF();
                if (i_pTestStep->hasConfigValue(strMethod + ".unit")) {
                    QString strUnitPoint = i_pTestStep->getConfigValue(strMethod + ".unit").toString();
                    CUnit unit = strUnitPoint;
                    CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
                    physValPolygonResult.setWidthByMovingLeftCenter(physValPt);
                }
                else {
                    CPhysValPoint physValPt(*m_pDrawingScene, pt);
                    physValPolygonResult.setWidthByMovingLeftCenter(physValPt);
                }
            }
            strMethod = "setWidthByMovingRightCenter" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                QPointF pt = i_pTestStep->getConfigValue(strMethod).toPointF();
                if (i_pTestStep->hasConfigValue(strMethod + ".unit")) {
                    QString strUnitPoint = i_pTestStep->getConfigValue(strMethod + ".unit").toString();
                    CUnit unit = strUnitPoint;
                    CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
                    physValPolygonResult.setWidthByMovingRightCenter(physValPt);
                }
                else {
                    CPhysValPoint physValPt(*m_pDrawingScene, pt);
                    physValPolygonResult.setWidthByMovingRightCenter(physValPt);
                }
            }
            strMethod = "setHeight" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                CPhysVal physValHeight;
                if (i_pTestStep->hasConfigValue(strMethod + ".unit")) {
                    QString strUnit = i_pTestStep->getConfigValue(strMethod + ".unit").toString();
                    CUnit unit = strUnit;
                    physValHeight = CPhysVal(i_pTestStep->getConfigValue(strMethod).toDouble(), unit);
                }
                else {
                    physValHeight = i_pTestStep->getConfigValue(strMethod).toString();
                }
                physValPolygonResult.setHeight(physValHeight);
            }
            strMethod = "setHeightByMovingTopCenter" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                QPointF pt = i_pTestStep->getConfigValue(strMethod).toPointF();
                if (i_pTestStep->hasConfigValue(strMethod + ".unit")) {
                    QString strUnitPoint = i_pTestStep->getConfigValue(strMethod + ".unit").toString();
                    CUnit unit = strUnitPoint;
                    CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
                    physValPolygonResult.setHeightByMovingTopCenter(physValPt);
                }
                else {
                    CPhysValPoint physValPt(*m_pDrawingScene, pt);
                    physValPolygonResult.setHeightByMovingTopCenter(physValPt);
                }
            }
            strMethod = "setHeightByMovingBottomCenter" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                QPointF pt = i_pTestStep->getConfigValue(strMethod).toPointF();
                if (i_pTestStep->hasConfigValue(strMethod + ".unit")) {
                    QString strUnitPoint = i_pTestStep->getConfigValue(strMethod + ".unit").toString();
                    CUnit unit = strUnitPoint;
                    CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
                    physValPolygonResult.setHeightByMovingBottomCenter(physValPt);
                }
                else {
                    CPhysValPoint physValPt(*m_pDrawingScene, pt);
                    physValPolygonResult.setHeightByMovingBottomCenter(physValPt);
                }
            }
            strMethod = "setTopLeft" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                QPointF pt = i_pTestStep->getConfigValue(strMethod).toPointF();
                if (i_pTestStep->hasConfigValue(strMethod + ".unit")) {
                    QString strUnitPoint = i_pTestStep->getConfigValue(strMethod + ".unit").toString();
                    CUnit unit = strUnitPoint;
                    CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
                    physValPolygonResult.setTopLeft(physValPt);
                }
                else {
                    CPhysValPoint physValPt(*m_pDrawingScene, pt);
                    physValPolygonResult.setTopLeft(physValPt);
                }
            }
            strMethod = "setTopRight" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                QPointF pt = i_pTestStep->getConfigValue(strMethod).toPointF();
                if (i_pTestStep->hasConfigValue(strMethod + ".unit")) {
                    QString strUnitPoint = i_pTestStep->getConfigValue(strMethod + ".unit").toString();
                    CUnit unit = strUnitPoint;
                    CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
                    physValPolygonResult.setTopRight(physValPt);
                }
                else {
                    CPhysValPoint physValPt(*m_pDrawingScene, pt);
                    physValPolygonResult.setTopRight(physValPt);
                }
            }
            strMethod = "setBottomRight" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                QPointF pt = i_pTestStep->getConfigValue(strMethod).toPointF();
                if (i_pTestStep->hasConfigValue(strMethod + ".unit")) {
                    QString strUnitPoint = i_pTestStep->getConfigValue(strMethod + ".unit").toString();
                    CUnit unit = strUnitPoint;
                    CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
                    physValPolygonResult.setBottomRight(physValPt);
                }
                else {
                    CPhysValPoint physValPt(*m_pDrawingScene, pt);
                    physValPolygonResult.setBottomRight(physValPt);
                }
            }
            strMethod = "setBottomLeft" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                QPointF pt = i_pTestStep->getConfigValue(strMethod).toPointF();
                if (i_pTestStep->hasConfigValue(strMethod + ".unit")) {
                    QString strUnitPoint = i_pTestStep->getConfigValue(strMethod + ".unit").toString();
                    CUnit unit = strUnitPoint;
                    CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
                    physValPolygonResult.setBottomLeft(physValPt);
                }
                else {
                    CPhysValPoint physValPt(*m_pDrawingScene, pt);
                    physValPolygonResult.setBottomLeft(physValPt);
                }
            }
            strMethod = "replace" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                int idxPt = i_pTestStep->getConfigValue(strMethod).toInt();
                if (i_pTestStep->hasConfigValue(strMethod + ".point")) {
                    QPointF pt = i_pTestStep->getConfigValue(strMethod + ".point").toPointF();
                    if (i_pTestStep->hasConfigValue(strMethod + ".point.unit")) {
                        QString strUnitPoint = i_pTestStep->getConfigValue(strMethod + ".point.unit").toString();
                        CUnit unit = strUnitPoint;
                        CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
                        physValPolygonResult.replace(idxPt, physValPt);
                    }
                    else {
                        CPhysValPoint physValPt(*m_pDrawingScene, pt);
                        physValPolygonResult.replace(idxPt, physValPt);
                    }
                }
            }
            strMethod = "append" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                QPointF pt = i_pTestStep->getConfigValue(strMethod).toPointF();
                if (i_pTestStep->hasConfigValue(strMethod + ".unit")) {
                    QString strUnitPoint = i_pTestStep->getConfigValue(strMethod + ".unit").toString();
                    CUnit unit = strUnitPoint;
                    CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
                    physValPolygonResult.append(physValPt);
                }
                else {
                    CPhysValPoint physValPt(*m_pDrawingScene, pt);
                    physValPolygonResult.append(physValPt);
                }
            }
            strMethod = "insert" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                int idxPt = i_pTestStep->getConfigValue(strMethod).toInt();
                if (i_pTestStep->hasConfigValue(strMethod + ".point")) {
                    QPointF pt = i_pTestStep->getConfigValue(strMethod + ".point").toPointF();
                    if (i_pTestStep->hasConfigValue(strMethod + ".point.unit")) {
                        QString strUnitPoint = i_pTestStep->getConfigValue(strMethod + ".point.unit").toString();
                        CUnit unit = strUnitPoint;
                        CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
                        physValPolygonResult.insert(idxPt, physValPt);
                    }
                    else {
                        CPhysValPoint physValPt(*m_pDrawingScene, pt);
                        physValPolygonResult.insert(idxPt, physValPt);
                    }
                }
            }
            strMethod = "remove" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                int idxPt = i_pTestStep->getConfigValue(strMethod).toInt();
                if (i_pTestStep->hasConfigValue(strMethod + ".count")) {
                    int iCount = i_pTestStep->getConfigValue(strMethod + ".count").toInt();
                    physValPolygonResult.remove(idxPt, iCount);
                }
            }
            strMethod = "removeAt" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                int idxPt = i_pTestStep->getConfigValue(strMethod).toInt();
                physValPolygonResult.removeAt(idxPt);
            }
            strMethod = "removeFirst" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                physValPolygonResult.removeFirst();
            }
            strMethod = "removeLast" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                physValPolygonResult.removeLast();
            }
            strMethod = "takeAt" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                int idxPt = i_pTestStep->getConfigValue(strMethod).toInt();
                physValPointTaken = physValPolygonResult.takeAt(idxPt);
            }
            strMethod = "takeFirst" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                physValPointTaken = physValPolygonResult.takeFirst();
            }
            strMethod = "takeLast" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                bHasMethod = true;
                physValPointTaken = physValPolygonResult.takeLast();
            }
            strMethod = "addPhysValShape" + strIdxMethod;
            if (i_pTestStep->hasConfigValue(strMethod)) {
                CDrawSettings drawSettings(EGraphObjTypePolygon);
                drawSettings.setPenColor(QColor(i_pTestStep->getConfigValue(strMethod).toString()));
                CPhysValPolygon* pPhysValPolygon = new CPhysValPolygon(physValPolygonResult);
                m_pDrawingScene->addPhysValShape(pPhysValPolygon, drawSettings);
                pPhysValPolygon = nullptr;
            }
            ++idxMethod;
        }
    }

    QStringList strlstResultValues;
    if (!physValPolygonResult.isNull()) {
        int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
            i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;

        strlstResultValues.append(
            "Center {" + physValPolygonResult.center().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValPolygonResult.center().unit().symbol());
        strlstResultValues.append(
            "Size {" + physValPolygonResult.size().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValPolygonResult.size().unit().symbol());
        strlstResultValues.append("Angle: " + physValPolygonResult.angle().toString());
        strlstResultValues.append(
            "TopLeft {" + physValPolygonResult.topLeft().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValPolygonResult.topLeft().unit().symbol());
        strlstResultValues.append(
            "TopRight {" + physValPolygonResult.topRight().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValPolygonResult.topRight().unit().symbol());
        strlstResultValues.append(
            "BottomRight {" + physValPolygonResult.bottomRight().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValPolygonResult.bottomRight().unit().symbol());
        strlstResultValues.append(
            "BottomLeft {" + physValPolygonResult.bottomLeft().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValPolygonResult.bottomLeft().unit().symbol());
        strlstResultValues.append(
            "TopCenter {" + physValPolygonResult.topCenter().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValPolygonResult.topCenter().unit().symbol());
        strlstResultValues.append(
            "RightCenter {" + physValPolygonResult.rightCenter().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValPolygonResult.rightCenter().unit().symbol());
        strlstResultValues.append(
            "BottomCenter {" + physValPolygonResult.bottomCenter().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValPolygonResult.bottomCenter().unit().symbol());
        strlstResultValues.append(
            "LeftCenter {" + physValPolygonResult.leftCenter().toString(false, ", ", iResultValuesPrecision) + "} " +
            physValPolygonResult.leftCenter().unit().symbol());
        if (!physValPointTaken.isNull()) {
            strlstResultValues.append(
                "Taken: {" +
                physValPointTaken.toString(false, ", ", iResultValuesPrecision) + "} " +
                physValPointTaken.unit().symbol());
        }
        for (int idxPt = 0; idxPt < physValPolygonResult.count(); ++idxPt) {
            strlstResultValues.append(
                "P" + QString::number(idxPt) + ": {" +
                physValPolygonResult.at(idxPt).toString(false, ", ", iResultValuesPrecision) + "} " +
                physValPolygonResult.at(idxPt).unit().symbol());
        }
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepDrawingSceneConvertToPhysValPoint(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
void CTest::doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
void CTest::doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
void CTest::doTestStepDrawingSceneConvertToPhysValRect(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
void CTest::doTestStepSetDrawSettings(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepSetDrawSettings",
        /* strAddInfo   */ strMthInArgs );

    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();
    bool bImmediatelyApplySetting = i_pTestStep->getConfigValue("ImmediatelyApplySettings").toBool();
    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
    if (pGraphObj != nullptr) {
        for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow) {
            QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);
            QString strMethod = dataRow["Method"].toString();
            if (strMethod == "setPenColor") {
                QColor penColor = dataRow["PenColor"].toString();
                pGraphObj->setPenColor(penColor, bImmediatelyApplySetting);
            }
            else if (strMethod == "setPenWidth") {
                int iPenWidth = dataRow["PenWidth"].toInt();
                pGraphObj->setPenWidth(iPenWidth, bImmediatelyApplySetting);
            }
            else if (strMethod == "setLineStyle") {
                CEnumLineStyle lineStyle = dataRow["LineStyle"].toString();
                pGraphObj->setLineStyle(lineStyle, bImmediatelyApplySetting);
            }
            else if (strMethod == "setFillColor") {
                QColor fillColor = dataRow["FillColor"].toString();
                pGraphObj->setFillColor(fillColor, bImmediatelyApplySetting);
            }
            else if (strMethod == "setFillStyle") {
                CEnumFillStyle fillStyle = dataRow["FillStyle"].toString();
                pGraphObj->setFillStyle(fillStyle, bImmediatelyApplySetting);
            }
            else if (strMethod == "setLineRecordType") {
                CEnumLineRecordType lineRecordType = dataRow["LineRecordType"].toString();
                pGraphObj->setLineRecordType(lineRecordType, bImmediatelyApplySetting);
            }
            else if (strMethod == "setLineExtent") {
                int iLineExtent = dataRow["LineExtent"].toInt();
                pGraphObj->setLineExtent(iLineExtent, bImmediatelyApplySetting);
            }
            else if (strMethod == "setLineEndStyle") {
                CEnumLinePoint linePoint = dataRow["LinePoint"].toString();
                CEnumLineEndStyle lineEndStyle = dataRow["LineEndStyle"].toString();
                pGraphObj->setLineEndStyle(linePoint, lineEndStyle, bImmediatelyApplySetting);
            }
            else if (strMethod == "setArrowHeadBaseLineType") {
                CEnumLinePoint linePoint = dataRow["LinePoint"].toString();
                CEnumArrowHeadBaseLineType arrowHeadBaseLineType = dataRow["ArrowHeadBaseLineType"].toString();
                pGraphObj->setArrowHeadBaseLineType(linePoint, arrowHeadBaseLineType, bImmediatelyApplySetting);
            }
            else if (strMethod == "setArrowHeadFillStyle") {
                CEnumLinePoint linePoint = dataRow["LinePoint"].toString();
                CEnumArrowHeadFillStyle arrowHeadFillStyle = dataRow["ArrowHeadFillStyle"].toString();
                pGraphObj->setArrowHeadFillStyle(linePoint, arrowHeadFillStyle, bImmediatelyApplySetting);
            }
            else if (strMethod == "setArrowHeadWidth") {
                CEnumLinePoint linePoint = dataRow["LinePoint"].toString();
                CEnumArrowHeadWidth arrowHeadWidth = dataRow["ArrowHeadWidth"].toString();
                pGraphObj->setArrowHeadWidth(linePoint, arrowHeadWidth, bImmediatelyApplySetting);
            }
            else if (strMethod == "setArrowHeadLength") {
                CEnumLinePoint linePoint = dataRow["LinePoint"].toString();
                CEnumArrowHeadLength arrowHeadLength = dataRow["ArrowHeadLength"].toString();
                pGraphObj->setArrowHeadLength(linePoint, arrowHeadLength, bImmediatelyApplySetting);
            }
            else if (strMethod == "setTextColor") {
                QColor textColor = dataRow["TextColor"].toString();
                pGraphObj->setTextColor(textColor, bImmediatelyApplySetting);
            }
            else if (strMethod == "setFont") {
                QFont font = dataRow["Font"].toString();
                pGraphObj->setFont(font, bImmediatelyApplySetting);
            }
            else if (strMethod == "setTextSize") {
                ETextSize textSize = str2TextSize(dataRow["TextSize"].toString());
                pGraphObj->setTextSize(textSize, bImmediatelyApplySetting);
            }
            else if (strMethod == "setTextStyle") {
                CEnumTextStyle textStyle = dataRow["TextStyle"].toString();
                pGraphObj->setTextStyle(textStyle, bImmediatelyApplySetting);
            }
            else if (strMethod == "setTextEffect") {
                CEnumTextEffect textEffect = dataRow["TextEffect"].toString();
                pGraphObj->setTextEffect(textEffect, bImmediatelyApplySetting);
            }
        }
        if (!bImmediatelyApplySetting) {
            pGraphObj->updateDrawSettings();
        }
    }
    i_pTestStep->setExpectedValues(QStringList());
    i_pTestStep->setResultValues(QStringList());
}

//------------------------------------------------------------------------------
void CTest::doTestStepShowLabels(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
        SGraphObjSelectionPoint selPt;
        if (dataRow.contains("AnchorPoint")) {
            selPt = SGraphObjSelectionPoint::fromString(dataRow["AnchorPoint"].toString());
        }
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
                if (selPt.isValid()) {
                    if (selPt.m_selPtType == ESelectionPointType::BoundingRectangle) {
                        pGraphObj->setLabelAnchorPoint(strLabelName, selPt.m_selPtType, selPt.m_selPt);
                    }
                    else {
                        pGraphObj->setLabelAnchorPoint(strLabelName, selPt.m_selPtType, selPt.m_idxPt);
                    }
                }
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
void CTest::doTestStepHideLabels(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
void CTest::doTestStepShowGeometryLabels(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
void CTest::doTestStepHideGeometryLabels(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
void CTest::doTestStepAddGraphObjLine(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
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
        CGraphObj* pGraphObj = pObjFactory->createGraphObj(m_pDrawingScene, drawSettings);
        m_pDrawingScene->addGraphObj(pGraphObj);
        CGraphObjLine* pGraphObjLine = dynamic_cast<CGraphObjLine*>(pGraphObj);
        if (pGraphObjLine != nullptr) {
            pGraphObjLine->setP1(physValPoint1);
            pGraphObjLine->setP2(physValPoint2);
        }
        pGraphObj->rename(strGraphObjName);
    }

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstResultValues;
    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strKeyInTree);
    if (pGraphObj != nullptr) {
        strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepAddGraphObjRect(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepAddGraphObjRect",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;

    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    EGraphObjType graphObjType = str2GraphObjType(strGraphObjType);
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);
    if (pObjFactory != nullptr) {
        QRectF rect = i_pTestStep->getConfigValue("Rect").value<QRectF>();
        CUnit unit = drawingSize.unit();
        if (i_pTestStep->hasConfigValue("Rect.Unit")) {
            QString strUnit = i_pTestStep->getConfigValue("Rect.Unit").toString();
            unit = strUnit;
        }
        CDrawSettings drawSettings(graphObjType);
        CGraphObj* pGraphObj = pObjFactory->createGraphObj(m_pDrawingScene, drawSettings);
        m_pDrawingScene->addGraphObj(pGraphObj);
        CGraphObjRect* pGraphObjRect = dynamic_cast<CGraphObjRect*>(pGraphObj);
        if (pGraphObjRect != nullptr) {
            pGraphObjRect->setRect(CPhysValRect(*m_pDrawingScene, rect, unit));
        }
        pGraphObj->rename(strGraphObjName);
    }

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstResultValues;
    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strKeyInTree);
    if (pGraphObj != nullptr) {
        strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepAddGraphObjEllipse(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepAddGraphObjEllipse",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;

    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    EGraphObjType graphObjType = str2GraphObjType(strGraphObjType);
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);
    if (pObjFactory != nullptr) {
        QRectF rect = i_pTestStep->getConfigValue("Rect").value<QRectF>();
        CUnit unit = drawingSize.unit();
        if (i_pTestStep->hasConfigValue("Rect.Unit")) {
            QString strUnit = i_pTestStep->getConfigValue("Rect.Unit").toString();
            unit = strUnit;
        }
        CDrawSettings drawSettings(graphObjType);
        CGraphObj* pGraphObj = pObjFactory->createGraphObj(m_pDrawingScene, drawSettings);
        m_pDrawingScene->addGraphObj(pGraphObj);
        CGraphObjEllipse* pGraphObjEllipse = dynamic_cast<CGraphObjEllipse*>(pGraphObj);
        if (pGraphObjEllipse != nullptr) {
            pGraphObjEllipse->setRect(CPhysValRect(*m_pDrawingScene, rect, unit));
        }
        pGraphObj->rename(strGraphObjName);
    }

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstResultValues;
    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strKeyInTree);
    if (pGraphObj != nullptr) {
        strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepAddGraphObjText(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepAddGraphObjText",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;

    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    EGraphObjType graphObjType = str2GraphObjType(strGraphObjType);
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);
    if (pObjFactory != nullptr) {
        QRectF rect = i_pTestStep->getConfigValue("Rect").value<QRectF>();
        CUnit unit = drawingSize.unit();
        if (i_pTestStep->hasConfigValue("Rect.Unit")) {
            QString strUnit = i_pTestStep->getConfigValue("Rect.Unit").toString();
            unit = strUnit;
        }
        QString strText = i_pTestStep->getConfigValue("Text").toString();
        CDrawSettings drawSettings(graphObjType);
        CGraphObj* pGraphObj = pObjFactory->createGraphObj(m_pDrawingScene, drawSettings);
        m_pDrawingScene->addGraphObj(pGraphObj);
        CGraphObjText* pGraphObjText = dynamic_cast<CGraphObjText*>(pGraphObj);
        if (pGraphObjText != nullptr) {
            pGraphObjText->setPlainText(strText);
            pGraphObjText->setRect(CPhysValRect(*m_pDrawingScene, rect, unit));
            QMargins margins = CGraphObjText::defaultMarginsInPx();
            if (i_pTestStep->hasConfigValue("Margins.Left")) {
                int iMarginLeft = i_pTestStep->getConfigValue("Margins.Left").toInt();
                int iMarginTop = i_pTestStep->getConfigValue("Margins.Top").toInt();
                int iMarginRight = i_pTestStep->getConfigValue("Margins.Right").toInt();
                int iMarginBottom = i_pTestStep->getConfigValue("Margins.Bottom").toInt();
                margins = QMargins(iMarginLeft, iMarginTop, iMarginRight, iMarginBottom);
                pGraphObjText->setMargins(margins);
            }
            if (i_pTestStep->hasConfigValue("TextWidth")) {
                double fTextWidth = i_pTestStep->getConfigValue("TextWidth").toDouble();
                pGraphObjText->setTextWidth(fTextWidth);
            }
            if (i_pTestStep->hasConfigValue("AdjustSize")) {
                bool bAdjustSize = i_pTestStep->getConfigValue("AdjustSize").toBool();
                if (bAdjustSize) {
                    pGraphObjText->adjustSize();
                }
            }
        }
        pGraphObj->rename(strGraphObjName);
    }

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstResultValues;
    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strKeyInTree);
    if (pGraphObj != nullptr) {
        strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepAddGraphObjPolygon(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepAddGraphObjPolygon",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;

    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    EGraphObjType graphObjType = str2GraphObjType(strGraphObjType);
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    QPolygonF polygon = i_pTestStep->getConfigValue("Points").value<QPolygonF>();

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);
    if (pObjFactory != nullptr) {
        CDrawSettings drawSettings(graphObjType);
        CGraphObj* pGraphObj = pObjFactory->createGraphObj(m_pDrawingScene, drawSettings);
        m_pDrawingScene->addGraphObj(pGraphObj);
        CGraphObjPolygon* pGraphObjPolygon = dynamic_cast<CGraphObjPolygon*>(pGraphObj);
        if (pGraphObjPolygon != nullptr) {
            pGraphObjPolygon->setPolygon(polygon, drawingSize.unit());
        }
        pGraphObj->rename(strGraphObjName);
    }

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstResultValues;
    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strKeyInTree);
    if (pGraphObj != nullptr) {
        strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepAddGraphObjConnectionPoint(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepAddGraphObjConnectionPoint",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameConnections;

    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    EGraphObjType graphObjType = str2GraphObjType(strGraphObjType);
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);
    if (pObjFactory != nullptr) {
        QPointF ptPos = i_pTestStep->getConfigValue("Point").value<QPointF>();
        CUnit unit = drawingSize.unit();
        if (i_pTestStep->hasConfigValue("Point.Unit")) {
            QString strUnit = i_pTestStep->getConfigValue("Point.Unit").toString();
            unit = strUnit;
        }
        CDrawSettings drawSettings(graphObjType);
        CGraphObj* pGraphObj = pObjFactory->createGraphObj(m_pDrawingScene, drawSettings);
        m_pDrawingScene->addGraphObj(pGraphObj);
        CGraphObjConnectionPoint* pGraphObjConnectionPoint = dynamic_cast<CGraphObjConnectionPoint*>(pGraphObj);
        if (pGraphObjConnectionPoint != nullptr) {
            pGraphObjConnectionPoint->setPosition(CPhysValPoint(*m_pDrawingScene, ptPos, unit));
        }
        pGraphObj->rename(strGraphObjName);
    }

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstResultValues;
    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strKeyInTree);
    if (pGraphObj != nullptr) {
        strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepAddGraphObjConnectionLine(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepAddGraphObjConnectionLine",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameConnections;

    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    EGraphObjType graphObjType = str2GraphObjType(strGraphObjType);
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    QPolygonF polygon = i_pTestStep->getConfigValue("Points").value<QPolygonF>();
    QString strKeyInTreeCnctPt1 = i_pTestStep->getConfigValue("GraphObjKeyInTreeCnctPt1").toString();
    QString strKeyInTreeCnctPt2 = i_pTestStep->getConfigValue("GraphObjKeyInTreeCnctPt2").toString();
    CGraphObjConnectionPoint* pCnctPt1 = dynamic_cast<CGraphObjConnectionPoint*>(m_pDrawingScene->findGraphObj(strKeyInTreeCnctPt1));
    CGraphObjConnectionPoint* pCnctPt2 = dynamic_cast<CGraphObjConnectionPoint*>(m_pDrawingScene->findGraphObj(strKeyInTreeCnctPt2));

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);

    if (pObjFactory != nullptr) {
        CDrawSettings drawSettings(EGraphObjTypeConnectionLine);
        CGraphObj* pGraphObj = pObjFactory->createGraphObj(m_pDrawingScene, drawSettings);
        m_pDrawingScene->addGraphObj(pGraphObj);
        CGraphObjConnectionLine* pGraphObjConnectionLine = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj);
        if (pGraphObjConnectionLine != nullptr) {
            pGraphObjConnectionLine->setConnectionPoint(ELinePoint::Start, pCnctPt1);
            pGraphObjConnectionLine->setConnectionPoint(ELinePoint::End, pCnctPt2);
            if (!polygon.isEmpty()) {
                pGraphObjConnectionLine->setPolygon(polygon, drawingSize.unit());
            }
        }
        pGraphObj->rename(strGraphObjName);
    }

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstResultValues;
    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strKeyInTree);
    if (pGraphObj != nullptr) {
        strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepAddGraphObjGroup(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepAddGraphObjGroup",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);

    QString strGroupName = i_pTestStep->getConfigValue("GroupName").toString();
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupName);

    QStringList strlstGraphObjsAddToGroup = i_pTestStep->getConfigValue("AddToGroup").toStringList();

    QStringList strlstGraphObjsKeyInTreeGetResultValues;
    if (i_pTestStep->hasConfigValue("GraphObjsKeyInTreeGetResultValues")) {
        strlstGraphObjsKeyInTreeGetResultValues = i_pTestStep->getConfigValue("GraphObjsKeyInTreeGetResultValues").toStringList();
    }

    CGraphObjGroup* pGraphObjGroup = nullptr;
    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);
    if (pObjFactory != nullptr) {
        CDrawSettings drawSettings(EGraphObjTypeGroup);
        pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pObjFactory->createGraphObj(m_pDrawingScene, drawSettings));
        if (pGraphObjGroup != nullptr) {
            m_pDrawingScene->addGraphObj(pGraphObjGroup);
            pGraphObjGroup->rename(strGroupName);
            for (const QString& strGraphObjNameChild : strlstGraphObjsAddToGroup) {
                strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjNameChild);
                CGraphObj* pGraphObjChild = m_pDrawingScene->findGraphObj(strKeyInTree);
                if (pGraphObjChild != nullptr) {
                    pGraphObjGroup->addToGroup(pGraphObjChild);
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

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstResultValues;
    for (const QString& strGraphObjKeyInTree : strlstGraphObjsKeyInTreeGetResultValues) {
        CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
        if (pGraphObj != nullptr) {
            strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
        }
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepDrawGraphObj(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepDrawGraphObj",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    if (i_pTestStep->hasConfigValue("GraphObjFactoryGroup")) {
        strFactoryGroupName = i_pTestStep->getConfigValue("GraphObjFactoryGroup").toString();
    }

    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    EGraphObjType graphObjType = str2GraphObjType(strGraphObjType);
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);
    if (pObjFactory != nullptr) {
        QPointF pt1 = i_pTestStep->getConfigValue("P1").toPointF();
        QPointF pt2 = i_pTestStep->getConfigValue("P2").toPointF();
        QString strMethod = "setCurrentDrawingTool";
        if (i_pTestStep->hasConfigValue("Method")) {
            strMethod = i_pTestStep->getConfigValue("Method").toString();
        }

        if (strMethod == "setCurrentDrawingTool") {
            m_pDrawingScene->setCurrentDrawingTool(strFactoryGroupName, strGraphObjType);
            i_pTestStep->setConfigValue("Method", "mousePressEvent");
            triggerDoTestStep();
        }
        else if (strMethod == "mousePressEvent") {
            QPoint ptMousePosStart = pt1.toPoint();
            QPoint ptMousePosStop = pt2.toPoint();
            Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
            if (i_pTestStep->hasConfigValue("KeyboardModifiers")) {
                keyboardModifiers = static_cast<Qt::KeyboardModifiers>(i_pTestStep->getConfigValue("KeyboardModifiers").toInt());
            }
            QPoint ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptMousePosStart);
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
            int iMovesCount = 0;
            if (i_pTestStep->hasConfigValue("MouseMovesCount")) {
                iMovesCount = i_pTestStep->getConfigValue("MouseMovesCount").toInt();
            }
            addMouseMoveEventDataRows(i_pTestStep, ptMousePosStart, ptMousePosStop, iMovesCount);
            i_pTestStep->setConfigValue("Method", "mouseMoveEvent");
            triggerDoTestStep();
        }
        else if (strMethod == "mouseMoveEvent") {
            int iMouseMovesRemaining = i_pTestStep->getDataRowCount();
            if (iMouseMovesRemaining > 0) {
                Qt::MouseButton mouseBtn = Qt::LeftButton;
                if (i_pTestStep->hasConfigValue("MouseButtons")) {
                    mouseBtn = static_cast<Qt::MouseButton>(i_pTestStep->getConfigValue("MouseButtons").toInt());
                }
                QHash<QString, QVariant> dataRow = i_pTestStep->takeDataRow(0);
                QPoint ptMouseMovePos = dataRow["MousePos"].toPoint();
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
            }
            else {
                i_pTestStep->setConfigValue("Method", "mouseReleaseEvent");
            }
            triggerDoTestStep();
        }
        else if (strMethod == "mouseReleaseEvent") {
            QPoint ptMousePos = pt2.toPoint();
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
            if (graphObjType == EGraphObjTypeText) {
                i_pTestStep->setConfigValue("Method", "setPlainText");
            }
            else {
                i_pTestStep->setConfigValue("Method", "setResultValues");
            }
            triggerDoTestStep();
        }
        else if (strMethod == "setPlainText") {
            int iInstCount = pObjFactory->getNumberOfCreatedGraphObjects();
            QString strKeyInTreeCreated = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjType + QString::number(iInstCount-1));
            QStringList strlstResultValues;
            CGraphObjText* pGraphObjText = dynamic_cast<CGraphObjText*>(m_pDrawingScene->findGraphObj(strKeyInTreeCreated));
            if (pGraphObjText != nullptr) {
                QString strText = i_pTestStep->getConfigValue("Text").toString();
                pGraphObjText->setPlainText(strText);
                pGraphObjText->adjustSize();
            }
            i_pTestStep->setConfigValue("Method", "setResultValues");
            triggerDoTestStep();
        }
        else if (strMethod == "setResultValues") {
            int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
                i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
            int iInstCount = pObjFactory->getNumberOfCreatedGraphObjects();
            QString strKeyInTreeCreated = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjType + QString::number(iInstCount-1));
            QStringList strlstResultValues;
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(m_pDrawingScene->findGraphObj(strKeyInTreeCreated));
            if (pGraphObj != nullptr) {
                pGraphObj->rename(strGraphObjName);
                strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
            }
            i_pTestStep->setResultValues(strlstResultValues);
            i_pTestStep->removeConfigValue("Method"); // to allow that the test may be called several times
        }
    }
}

//------------------------------------------------------------------------------
void CTest::doTestStepDrawGraphObjPolygon(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepDrawGraphObjPolygon",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;

    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    EGraphObjType graphObjType = str2GraphObjType(strGraphObjType);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    QPolygon points = i_pTestStep->getConfigValue("Points").value<QPolygon>();
    QString strMethod = "setCurrentDrawingTool";
    if (i_pTestStep->hasConfigValue("Method")) {
        strMethod = i_pTestStep->getConfigValue("Method").toString();
    }

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        QString strRuntimeInfo = "Method: " + strMethod;
        if (i_pTestStep->hasConfigValue("Points.Idx")) {
            strRuntimeInfo += ", PointsIdx: " + QString::number(i_pTestStep->getConfigValue("Points.Idx").toInt());
        }
        mthTracer.trace(strRuntimeInfo);
    }

    if (strMethod == "setCurrentDrawingTool") {
        m_pDrawingScene->setCurrentDrawingTool(strFactoryGroupName, strGraphObjType);
        i_pTestStep->setConfigValue("Method", "mousePressEvent");
        if (points.size() >= 1) {
            i_pTestStep->setConfigValue("Points.Idx", 0);
        }
        else {
            i_pTestStep->setConfigValue("Method", "setResultValues");
        }
        triggerDoTestStep();
    }
    else if (strMethod == "mousePressEvent") {
        int idxPt = i_pTestStep->getConfigValue("Points.Idx").toInt();
        if (idxPt < points.size()) {
            QPoint ptMousePos = points.at(idxPt);
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
            i_pTestStep->setConfigValue("Method", "mouseReleaseEvent");
        }
        else {
            i_pTestStep->setConfigValue("Method", "setResultValues");
        }
        triggerDoTestStep();
    }
    else if (strMethod == "mouseReleaseEvent") {
        int idxPt = i_pTestStep->getConfigValue("Points.Idx").toInt();
        if (idxPt < points.size()) {
            QPoint ptMousePos = points.at(idxPt);
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
            if (idxPt < points.size() - 1) {
                QPoint ptMousePosStop = points.at(idxPt+1);
                int iMovesCount = 0;
                if (i_pTestStep->hasConfigValue("MouseMovesCount")) {
                    iMovesCount = i_pTestStep->getConfigValue("MouseMovesCount").toInt();
                }
                addMouseMoveEventDataRows(i_pTestStep, ptMousePos, ptMousePosStop, iMovesCount);
                i_pTestStep->setConfigValue("Method", "mouseMoveEvent");
            }
            else {
                i_pTestStep->setConfigValue("Method", "setResultValues");
            }
        }
        else {
            i_pTestStep->setConfigValue("Method", "setResultValues");
        }
        triggerDoTestStep();
    }
    else if (strMethod == "mouseMoveEvent") {
        int iMouseMovesRemaining = i_pTestStep->getDataRowCount();
        if (iMouseMovesRemaining > 0) {
            Qt::MouseButton mouseBtn = Qt::LeftButton;
            if (i_pTestStep->hasConfigValue("MouseButtons")) {
                mouseBtn = static_cast<Qt::MouseButton>(i_pTestStep->getConfigValue("MouseButtons").toInt());
            }
            QHash<QString, QVariant> dataRow = i_pTestStep->takeDataRow(0);
            QPoint ptMouseMovePos = dataRow["MousePos"].toPoint();
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
        }
        else {
            int idxPt = i_pTestStep->getConfigValue("Points.Idx").toInt();
            if (idxPt < points.size() - 2) {
                i_pTestStep->setConfigValue("Method", "mousePressEvent");
                i_pTestStep->setConfigValue("Points.Idx", idxPt + 1);
            }
            else if (idxPt == points.size() - 2) {
                i_pTestStep->setConfigValue("Method", "mouseDoubleClickEvent");
                i_pTestStep->setConfigValue("Points.Idx", idxPt + 1);
            }
            else {
                i_pTestStep->setConfigValue("Method", "setResultValues");
            }
        }
        triggerDoTestStep();
    }
    else if (strMethod == "mouseDoubleClickEvent") {
        int idxPt = i_pTestStep->getConfigValue("Points.Idx").toInt();
        if (idxPt < points.size()) {
            QPoint ptMousePos = points.at(idxPt);
            Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
            if (i_pTestStep->hasConfigValue("KeyboardModifiers")) {
                keyboardModifiers = static_cast<Qt::KeyboardModifiers>(i_pTestStep->getConfigValue("KeyboardModifiers").toInt());
            }
            QPoint ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptMousePos);
            QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.
            QMouseEvent* pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonDblClick,
                /* pos       */ ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* button    */ Qt::LeftButton,
                /* modifiers */ keyboardModifiers );
            m_pDrawingView->mouseDoubleClickEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
        }
        i_pTestStep->setConfigValue("Method", "setResultValues");
        i_pTestStep->removeConfigValue("Points.Idx");
        triggerDoTestStep();
    }
    else if (strMethod == "setResultValues") {
        int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
            i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
        QStringList strlstResultValues;
        // Also for EGraphObjTypePolyline a CGraphObjPolygon object is created (the type can be changed during runtime).
        QString strKeyInTreeCreated = pIdxTree->buildKeyInTreeStr(
            strEntryType, graphObjType2Str(EGraphObjTypePolygon) + QString::number(CGraphObjPolygon::s_iInstCount-1));
        CGraphObjPolygon* pGraphObjPolygon = dynamic_cast<CGraphObjPolygon*>(m_pDrawingScene->findGraphObj(strKeyInTreeCreated));
        if (pGraphObjPolygon != nullptr) {
            pGraphObjPolygon->rename(strGraphObjName);
            strlstResultValues.append(resultValuesForGraphObj(pGraphObjPolygon, false, iResultValuesPrecision));
        }
        i_pTestStep->setResultValues(strlstResultValues);
        i_pTestStep->removeConfigValue("Method"); // to allow that the test may be called several times
        i_pTestStep->removeConfigValue("Points.Idx");
    }

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        QString strRuntimeInfo = "Method: ";
        if (i_pTestStep->hasConfigValue("Method")) {
            strRuntimeInfo += i_pTestStep->getConfigValue("Method").toString();
        }
        if (i_pTestStep->hasConfigValue("Points.Idx")) {
            strRuntimeInfo += ", PointsIdx: " + QString::number(i_pTestStep->getConfigValue("Points.Idx").toInt());
        }
        mthTracer.trace(strRuntimeInfo);
    }
}

//------------------------------------------------------------------------------
void CTest::doTestStepDrawGraphObjGroup(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepDrawGraphObjGroup",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);

    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    QPointF pt1 = i_pTestStep->getConfigValue("P1").toPointF();
    QPointF pt2 = i_pTestStep->getConfigValue("P2").toPointF();
    QString strMethod = "unsetCurrentDrawingTool";
    if (i_pTestStep->hasConfigValue("Method")) {
        strMethod = i_pTestStep->getConfigValue("Method").toString();
    }

    if (strMethod == "unsetCurrentDrawingTool") {
        m_pDrawingScene->setCurrentDrawingTool(nullptr);
        i_pTestStep->setConfigValue("Method", "mousePressEvent");
        triggerDoTestStep();
    }
    else if (strMethod == "mousePressEvent") {
        QPoint ptMousePosStart = pt1.toPoint();
        QPoint ptMousePosStop = pt2.toPoint();
        Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
        if (i_pTestStep->hasConfigValue("KeyboardModifiers")) {
            keyboardModifiers = static_cast<Qt::KeyboardModifiers>(i_pTestStep->getConfigValue("KeyboardModifiers").toInt());
        }
        QPoint ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptMousePosStart);
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
        int iMovesCount = 0;
        if (i_pTestStep->hasConfigValue("MouseMovesCount")) {
            iMovesCount = i_pTestStep->getConfigValue("MouseMovesCount").toInt();
        }
        addMouseMoveEventDataRows(i_pTestStep, ptMousePosStart, ptMousePosStop, iMovesCount);
        i_pTestStep->setConfigValue("Method", "mouseMoveEvent");
        triggerDoTestStep();
    }
    else if (strMethod == "mouseMoveEvent") {
        int iMouseMovesRemaining = i_pTestStep->getDataRowCount();
        if (iMouseMovesRemaining > 0) {
            Qt::MouseButton mouseBtn = Qt::LeftButton;
            if (i_pTestStep->hasConfigValue("MouseButtons")) {
                mouseBtn = static_cast<Qt::MouseButton>(i_pTestStep->getConfigValue("MouseButtons").toInt());
            }
            QHash<QString, QVariant> dataRow = i_pTestStep->takeDataRow(0);
            QPoint ptMouseMovePos = dataRow["MousePos"].toPoint();
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
        }
        else {
            i_pTestStep->setConfigValue("Method", "mouseReleaseEvent");
        }
        triggerDoTestStep();
    }
    else if (strMethod == "mouseReleaseEvent") {
        QPoint ptMousePos = pt2.toPoint();
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
        i_pTestStep->setConfigValue("Method", "groupGraphObjsSelected");
        triggerDoTestStep();
    }
    else if (strMethod == "groupGraphObjsSelected") {
        m_pDrawingScene->groupGraphObjsSelected();
        i_pTestStep->setConfigValue("Method", "setResultValues");
        triggerDoTestStep();
    }
    else if (strMethod == "setResultValues") {
        QStringList strlstResultValues;
        QString strKeyInTreeCreated = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjType + QString::number(CGraphObjGroup::s_iInstCount-1));
        QStringList strlstGraphObjsKeyInTreeGetResultValues;
        if (i_pTestStep->hasConfigValue("GraphObjsKeyInTreeGetResultValues")) {
            strlstGraphObjsKeyInTreeGetResultValues = i_pTestStep->getConfigValue("GraphObjsKeyInTreeGetResultValues").toStringList();
        }
        CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(m_pDrawingScene->findGraphObj(strKeyInTreeCreated));
        if (pGraphObjGroup != nullptr) {
            // Deselect the object before changing the name. Otherwise the selection points keep their current names.
            // This would not be a problem. But this might be confusing when reading the log file.
            pGraphObjGroup->setSelected(false);
            pGraphObjGroup->rename(strGraphObjName);
        }
        if (pGraphObjGroup != nullptr && strlstGraphObjsKeyInTreeGetResultValues.isEmpty()) {
            strlstGraphObjsKeyInTreeGetResultValues.append(pGraphObjGroup->keyInTree());
            for (CGraphObj* pGraphObjChild : pGraphObjGroup->childs()) {
                strlstGraphObjsKeyInTreeGetResultValues.append(pGraphObjChild->keyInTree());
            }
        }
        int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
            i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
        for (const QString& strGraphObjKeyInTree : strlstGraphObjsKeyInTreeGetResultValues) {
            CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
            if (pGraphObj != nullptr) {
                strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
            }
        }
        i_pTestStep->setResultValues(strlstResultValues);
        i_pTestStep->removeConfigValue("Method"); // to allow that the test may be called several times
    }
}

//------------------------------------------------------------------------------
void CTest::doTestStepModifyGraphObjLineByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepModifyGraphObjLineByDirectMethodCalls",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();
    QString strMethod = i_pTestStep->getConfigValue("Method").toString();

    CGraphObjLine* pGraphObj = dynamic_cast<CGraphObjLine*>(m_pDrawingScene->findGraphObj(strGraphObjKeyInTree));
    if (pGraphObj != nullptr) {
        if (strMethod.compare("setLine", Qt::CaseInsensitive) == 0) {
            if (i_pTestStep->hasConfigValue("P1")) {
                QPointF pt = i_pTestStep->getConfigValue("P1").toPointF();
                if (i_pTestStep->hasConfigValue("P1.unit")) {
                    QString strUnit = i_pTestStep->getConfigValue("P1.unit").toString();
                    unit = strUnit;
                }
                CPhysValPoint physValPoint1(*m_pDrawingScene, pt, unit);
                pGraphObj->setP1(physValPoint1);
            }
            if (i_pTestStep->hasConfigValue("P2")) {
                QPointF pt = i_pTestStep->getConfigValue("P2").toPointF();
                if (i_pTestStep->hasConfigValue("P2.unit")) {
                    QString strUnit = i_pTestStep->getConfigValue("P2.unit").toString();
                    unit = strUnit;
                }
                CPhysValPoint physValPoint2(*m_pDrawingScene, pt, unit);
                pGraphObj->setP2(physValPoint2);
            }
        }
        else if (strMethod.compare("setPosition", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("Pos").toPointF();
            if (i_pTestStep->hasConfigValue("Pos.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("Pos.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setPosition(physValPoint);
        }
        else if (strMethod.compare("setRotationAngle", Qt::CaseInsensitive) == 0) {
            CPhysVal physValAngle(0.0, Units.Angle.Degree, 0.1);
            QString strAngle = i_pTestStep->getConfigValue("Angle").toString();
            physValAngle = strAngle;
            pGraphObj->setRotationAngle(physValAngle);
        }
    }

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstResultValues;
    if (pGraphObj != nullptr) {
        strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepModifyGraphObjRectByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepModifyGraphObjRectByDirectMethodCalls",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();
    QString strMethod = i_pTestStep->getConfigValue("Method").toString();

    CGraphObjRect* pGraphObj= dynamic_cast<CGraphObjRect*>(m_pDrawingScene->findGraphObj(strGraphObjKeyInTree));
    if (pGraphObj != nullptr) {
        if (strMethod.compare("setRotationAngle", Qt::CaseInsensitive) == 0) {
            CPhysVal physValAngle(0.0, Units.Angle.Degree, 0.1);
            QString strAngle = i_pTestStep->getConfigValue("Angle").toString();
            physValAngle = strAngle;
            pGraphObj->setRotationAngle(physValAngle);
        }
        else if (strMethod.compare("setPosition", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("Pos").toPointF();
            if (i_pTestStep->hasConfigValue("Pos.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("Pos.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setPosition(physValPoint);
        }
        else if (strMethod.compare("setCenter", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("Center").toPointF();
            if (i_pTestStep->hasConfigValue("Center.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("Center.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setCenter(physValPoint);
        }
        else if (strMethod.compare("setTopLeft", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("TopLeft").toPointF();
            if (i_pTestStep->hasConfigValue("TopLeft.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("TopLeft.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setTopLeft(physValPoint);
        }
        else if (strMethod.compare("setTopRight", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("TopRight").toPointF();
            if (i_pTestStep->hasConfigValue("TopRight.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("TopRight.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setTopRight(physValPoint);
        }
        else if (strMethod.compare("setBottomRight", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("BottomRight").toPointF();
            if (i_pTestStep->hasConfigValue("BottomRight.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("BottomRight.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setBottomRight(physValPoint);
        }
        else if (strMethod.compare("setBottomLeft", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("BottomLeft").toPointF();
            if (i_pTestStep->hasConfigValue("BottomLeft.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("BottomLeft.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setBottomLeft(physValPoint);
        }
        else if (strMethod.compare("setSize", Qt::CaseInsensitive) == 0) {
            QSizeF size = i_pTestStep->getConfigValue("Size").toSizeF();
            if (i_pTestStep->hasConfigValue("Size.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("Size.unit").toString();
                unit = strUnit;
            }
            CPhysValSize physValSize(*m_pDrawingScene, size, unit);
            pGraphObj->setSize(physValSize);
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
            pGraphObj->setWidth(physVal);
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
            pGraphObj->setHeight(physVal);
        }
    }

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstResultValues;
    if (pGraphObj != nullptr) {
        strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepModifyGraphObjEllipseByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepModifyGraphObjEllipseByDirectMethodCalls",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();
    QString strMethod = i_pTestStep->getConfigValue("Method").toString();

    CGraphObjEllipse* pGraphObj= dynamic_cast<CGraphObjEllipse*>(m_pDrawingScene->findGraphObj(strGraphObjKeyInTree));
    if (pGraphObj != nullptr) {
        if (strMethod.compare("setRotationAngle", Qt::CaseInsensitive) == 0) {
            CPhysVal physValAngle(0.0, Units.Angle.Degree, 0.1);
            QString strAngle = i_pTestStep->getConfigValue("Angle").toString();
            physValAngle = strAngle;
            pGraphObj->setRotationAngle(physValAngle);
        }
        else if (strMethod.compare("setPosition", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("Pos").toPointF();
            if (i_pTestStep->hasConfigValue("Pos.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("Pos.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setPosition(physValPoint);
        }
        else if (strMethod.compare("setCenter", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("Center").toPointF();
            if (i_pTestStep->hasConfigValue("Center.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("Center.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setCenter(physValPoint);
        }
        else if (strMethod.compare("setTopLeft", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("TopLeft").toPointF();
            if (i_pTestStep->hasConfigValue("TopLeft.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("TopLeft.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setTopLeft(physValPoint);
        }
        else if (strMethod.compare("setTopRight", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("TopRight").toPointF();
            if (i_pTestStep->hasConfigValue("TopRight.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("TopRight.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setTopRight(physValPoint);
        }
        else if (strMethod.compare("setBottomRight", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("BottomRight").toPointF();
            if (i_pTestStep->hasConfigValue("BottomRight.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("BottomRight.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setBottomRight(physValPoint);
        }
        else if (strMethod.compare("setBottomLeft", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("BottomLeft").toPointF();
            if (i_pTestStep->hasConfigValue("BottomLeft.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("BottomLeft.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setBottomLeft(physValPoint);
        }
        else if (strMethod.compare("setSize", Qt::CaseInsensitive) == 0) {
            QSizeF size = i_pTestStep->getConfigValue("Size").toSizeF();
            if (i_pTestStep->hasConfigValue("Size.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("Size.unit").toString();
                unit = strUnit;
            }
            CPhysValSize physValSize(*m_pDrawingScene, size, unit);
            pGraphObj->setSize(physValSize);
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
            pGraphObj->setWidth(physVal);
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
            pGraphObj->setHeight(physVal);
        }
    }

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstResultValues;
    if (pGraphObj != nullptr) {
        strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepModifyGraphObjTextByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepModifyGraphObjTextByDirectMethodCalls",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();
    QString strMethod = i_pTestStep->getConfigValue("Method").toString();

    CGraphObjText* pGraphObj= dynamic_cast<CGraphObjText*>(m_pDrawingScene->findGraphObj(strGraphObjKeyInTree));
    if (pGraphObj != nullptr) {
        if (strMethod.compare("setRotationAngle", Qt::CaseInsensitive) == 0) {
            CPhysVal physValAngle(0.0, Units.Angle.Degree, 0.1);
            QString strAngle = i_pTestStep->getConfigValue("Angle").toString();
            physValAngle = strAngle;
            pGraphObj->setRotationAngle(physValAngle);
        }
        else if (strMethod.compare("setPosition", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("Pos").toPointF();
            if (i_pTestStep->hasConfigValue("Pos.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("Pos.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setPosition(physValPoint);
        }
        else if (strMethod.compare("setCenter", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("Center").toPointF();
            if (i_pTestStep->hasConfigValue("Center.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("Center.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setCenter(physValPoint);
        }
        else if (strMethod.compare("setTopLeft", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("TopLeft").toPointF();
            if (i_pTestStep->hasConfigValue("TopLeft.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("TopLeft.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setTopLeft(physValPoint);
        }
        else if (strMethod.compare("setTopRight", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("TopRight").toPointF();
            if (i_pTestStep->hasConfigValue("TopRight.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("TopRight.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setTopRight(physValPoint);
        }
        else if (strMethod.compare("setBottomRight", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("BottomRight").toPointF();
            if (i_pTestStep->hasConfigValue("BottomRight.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("BottomRight.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setBottomRight(physValPoint);
        }
        else if (strMethod.compare("setBottomLeft", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("BottomLeft").toPointF();
            if (i_pTestStep->hasConfigValue("BottomLeft.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("BottomLeft.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setBottomLeft(physValPoint);
        }
        else if (strMethod.compare("setSize", Qt::CaseInsensitive) == 0) {
            QSizeF size = i_pTestStep->getConfigValue("Size").toSizeF();
            if (i_pTestStep->hasConfigValue("Size.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("Size.unit").toString();
                unit = strUnit;
            }
            CPhysValSize physValSize(*m_pDrawingScene, size, unit);
            pGraphObj->setSize(physValSize);
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
            pGraphObj->setWidth(physVal);
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
            pGraphObj->setHeight(physVal);
        }
    }

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstResultValues;
    if (pGraphObj != nullptr) {
        strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepModifyGraphObjPolylineByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepModifyGraphObjPolylineByDirectMethodCalls",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();
    QString strMethod = i_pTestStep->getConfigValue("Method").toString();
    CPhysValPoint physValPointTaken(*m_pDrawingScene);

    CGraphObjPolygon* pGraphObj = dynamic_cast<CGraphObjPolygon*>(m_pDrawingScene->findGraphObj(strGraphObjKeyInTree));
    if (pGraphObj != nullptr) {
        if (strMethod.compare("setPolygon", Qt::CaseInsensitive) == 0) {
            if (i_pTestStep->hasConfigValue("polygon")) {
                CPhysValPolygon physValPolygon(*m_pDrawingScene);
                //physValPolygon = i_pTestStep->getConfigValue("polygon").toPolygonF();
                pGraphObj->setPolygon(physValPolygon);
            }
        }
        else if (strMethod.compare("setPosition", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("Pos").toPointF();
            if (i_pTestStep->hasConfigValue("Pos.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("Pos.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setPosition(physValPoint);
        }
        else if (strMethod.compare("setRotationAngle", Qt::CaseInsensitive) == 0) {
            CPhysVal physValAngle(0.0, Units.Angle.Degree, 0.1);
            QString strAngle = i_pTestStep->getConfigValue("Angle").toString();
            physValAngle = strAngle;
            pGraphObj->setRotationAngle(physValAngle);
        }
        else if (strMethod.compare("replace", Qt::CaseInsensitive) == 0) {
            int idxPt = i_pTestStep->getConfigValue("idxPt").toInt();
            QPointF pt = i_pTestStep->getConfigValue("point").toPointF();
            CUnit unit = m_pDrawingScene->drawingSize().unit();
            if (i_pTestStep->hasConfigValue("point.unit")) {
                unit = i_pTestStep->getConfigValue("point.unit").toString();
            }
            pGraphObj->replace(idxPt, CPhysValPoint(*m_pDrawingScene, pt, unit));
        }
        else if (strMethod.compare("append", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("point").toPointF();
            CUnit unit = m_pDrawingScene->drawingSize().unit();
            if (i_pTestStep->hasConfigValue("point.unit")) {
                unit = i_pTestStep->getConfigValue("point.unit").toString();
            }
            pGraphObj->append(CPhysValPoint(*m_pDrawingScene, pt, unit));
        }
        else if (strMethod.compare("insert", Qt::CaseInsensitive) == 0) {
            int idxPt = i_pTestStep->getConfigValue("idxPt").toInt();
            QPointF pt = i_pTestStep->getConfigValue("point").toPointF();
            CUnit unit = m_pDrawingScene->drawingSize().unit();
            if (i_pTestStep->hasConfigValue("point.unit")) {
                unit = i_pTestStep->getConfigValue("point.unit").toString();
            }
            pGraphObj->insert(idxPt, CPhysValPoint(*m_pDrawingScene, pt, unit));
        }
        else if (strMethod.compare("remove", Qt::CaseInsensitive) == 0) {
            int idxPt = i_pTestStep->getConfigValue("idxPt").toInt();
            int iCount = i_pTestStep->getConfigValue("count").toInt();
            pGraphObj->remove(idxPt, iCount);
        }
        else if (strMethod.compare("removeAt", Qt::CaseInsensitive) == 0) {
            int idxPt = i_pTestStep->getConfigValue("idxPt").toInt();
            pGraphObj->removeAt(idxPt);
        }
        else if (strMethod.compare("removeFirst", Qt::CaseInsensitive) == 0) {
            pGraphObj->removeFirst();
        }
        else if (strMethod.compare("removeLast", Qt::CaseInsensitive) == 0) {
            pGraphObj->removeLast();
        }
        else if (strMethod.compare("takeAt", Qt::CaseInsensitive) == 0) {
            int idxPt = i_pTestStep->getConfigValue("idxPt").toInt();
            physValPointTaken = pGraphObj->takeAt(idxPt);
        }
        else if (strMethod.compare("takeFirst", Qt::CaseInsensitive) == 0) {
            physValPointTaken = pGraphObj->takeFirst();
        }
        else if (strMethod.compare("takeLast", Qt::CaseInsensitive) == 0) {
            physValPointTaken = pGraphObj->takeLast();
        }
    }

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstResultValues;
    if (!physValPointTaken.isNull()) {
        strlstResultValues.append(
            "Taken: {" +
            physValPointTaken.toString(false, ", ", iResultValuesPrecision) + "} " +
            physValPointTaken.unit().symbol());
    }
    if (pGraphObj != nullptr) {
        strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepModifyGraphObjConnectionPointByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepModifyGraphObjConnectionPointByDirectMethodCalls",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionPoint);
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();
    QString strMethod = i_pTestStep->getConfigValue("Method").toString();

    CGraphObjConnectionPoint* pGraphObj = dynamic_cast<CGraphObjConnectionPoint*>(m_pDrawingScene->findGraphObj(strGraphObjKeyInTree));
    if (pGraphObj != nullptr) {
        if (strMethod.compare("setPosition", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("Pos").toPointF();
            if (i_pTestStep->hasConfigValue("Pos.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("Pos.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObj->setPosition(physValPoint);
        }
    }

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstGraphObjConnectionLinesKeyEntry = i_pTestStep->hasConfigValue("ResultValuesGraphObjsKeyEntry") ?
        i_pTestStep->getConfigValue("ResultValuesGraphObjsKeyEntry").toStringList() : QStringList();
    QStringList strlstResultValues;
    if (pGraphObj != nullptr) {
        if (!strlstGraphObjConnectionLinesKeyEntry.isEmpty()) {
            for (const QString& strGraphObjKeyEntry : strlstGraphObjConnectionLinesKeyEntry) {
                CGraphObjConnectionLine* pGraphObjConnectionLine =
                    dynamic_cast<CGraphObjConnectionLine*>(m_pDrawingScene->findGraphObj(strGraphObjKeyEntry));
                if (pGraphObjConnectionLine != nullptr) {
                    strlstResultValues.append(resultValuesForGraphObj(pGraphObjConnectionLine, false, iResultValuesPrecision));
                }
            }
        }
        else {
            strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
        }
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepModifyGraphObjConnectionLineByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepModifyGraphObjConnectionLineByDirectMethodCalls",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();
    QString strMethod = i_pTestStep->getConfigValue("Method").toString();
    CPhysValPoint physValPointTaken(*m_pDrawingScene);

    CGraphObjConnectionLine* pGraphObj = dynamic_cast<CGraphObjConnectionLine*>(m_pDrawingScene->findGraphObj(strGraphObjKeyInTree));
    if (pGraphObj != nullptr) {
        if (strMethod.compare("replace", Qt::CaseInsensitive) == 0) {
            int idxPt = i_pTestStep->getConfigValue("idxPt").toInt();
            QPointF pt = i_pTestStep->getConfigValue("point").toPointF();
            CUnit unit = m_pDrawingScene->drawingSize().unit();
            if (i_pTestStep->hasConfigValue("point.unit")) {
                unit = i_pTestStep->getConfigValue("point.unit").toString();
            }
            pGraphObj->replace(idxPt, CPhysValPoint(*m_pDrawingScene, pt, unit));
        }
        else if (strMethod.compare("append", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("point").toPointF();
            CUnit unit = m_pDrawingScene->drawingSize().unit();
            if (i_pTestStep->hasConfigValue("point.unit")) {
                unit = i_pTestStep->getConfigValue("point.unit").toString();
            }
            pGraphObj->append(CPhysValPoint(*m_pDrawingScene, pt, unit));
        }
        else if (strMethod.compare("insert", Qt::CaseInsensitive) == 0) {
            int idxPt = i_pTestStep->getConfigValue("idxPt").toInt();
            QPointF pt = i_pTestStep->getConfigValue("point").toPointF();
            CUnit unit = m_pDrawingScene->drawingSize().unit();
            if (i_pTestStep->hasConfigValue("point.unit")) {
                unit = i_pTestStep->getConfigValue("point.unit").toString();
            }
            pGraphObj->insert(idxPt, CPhysValPoint(*m_pDrawingScene, pt, unit));
        }
        else if (strMethod.compare("remove", Qt::CaseInsensitive) == 0) {
            int idxPt = i_pTestStep->getConfigValue("idxPt").toInt();
            int iCount = i_pTestStep->getConfigValue("count").toInt();
            pGraphObj->remove(idxPt, iCount);
        }
        else if (strMethod.compare("removeAt", Qt::CaseInsensitive) == 0) {
            int idxPt = i_pTestStep->getConfigValue("idxPt").toInt();
            pGraphObj->removeAt(idxPt);
        }
        else if (strMethod.compare("removeFirst", Qt::CaseInsensitive) == 0) {
            pGraphObj->removeFirst();
        }
        else if (strMethod.compare("removeLast", Qt::CaseInsensitive) == 0) {
            pGraphObj->removeLast();
        }
        else if (strMethod.compare("takeAt", Qt::CaseInsensitive) == 0) {
            int idxPt = i_pTestStep->getConfigValue("idxPt").toInt();
            physValPointTaken = pGraphObj->takeAt(idxPt);
        }
        else if (strMethod.compare("takeFirst", Qt::CaseInsensitive) == 0) {
            physValPointTaken = pGraphObj->takeFirst();
        }
        else if (strMethod.compare("takeLast", Qt::CaseInsensitive) == 0) {
            physValPointTaken = pGraphObj->takeLast();
        }
    }

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstResultValues;
    if (!physValPointTaken.isNull()) {
        strlstResultValues.append(
            "Taken: {" +
            physValPointTaken.toString(false, ", ", iResultValuesPrecision) + "} " +
            physValPointTaken.unit().symbol());
    }
    if (pGraphObj != nullptr) {
        strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepModifyGraphObjGroupByDirectMethodCalls",
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
        else if (strMethod.compare("setWidthByMovingLeftCenter", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("LeftCenter").toPointF();
            if (i_pTestStep->hasConfigValue("LeftCenter.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("LeftCenter.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObjGroup->setWidthByMovingLeftCenter(physValPoint);
        }
        else if (strMethod.compare("setWidthByMovingRightCenter", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("RightCenter").toPointF();
            if (i_pTestStep->hasConfigValue("RightCenter.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("RightCenter.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObjGroup->setWidthByMovingRightCenter(physValPoint);
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
        else if (strMethod.compare("setHeightByMovingTopCenter", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("TopCenter").toPointF();
            if (i_pTestStep->hasConfigValue("TopCenter.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("TopCenter.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObjGroup->setHeightByMovingTopCenter(physValPoint);
        }
        else if (strMethod.compare("setHeightByMovingBottomCenter", Qt::CaseInsensitive) == 0) {
            QPointF pt = i_pTestStep->getConfigValue("BottomCenter").toPointF();
            if (i_pTestStep->hasConfigValue("BottomCenter.unit")) {
                QString strUnit = i_pTestStep->getConfigValue("BottomCenter.unit").toString();
                unit = strUnit;
            }
            CPhysValPoint physValPoint(*m_pDrawingScene, pt, unit);
            pGraphObjGroup->setHeightByMovingBottomCenter(physValPoint);
        }

        if (pGraphObjGroup != nullptr && strlstGraphObjsKeyInTreeGetResultValues.isEmpty()) {
            strlstGraphObjsKeyInTreeGetResultValues.append(pGraphObjGroup->keyInTree());
            for (CGraphObj* pGraphObjChild : pGraphObjGroup->childs()) {
                strlstGraphObjsKeyInTreeGetResultValues.append(pGraphObjChild->keyInTree());
            }
        }
    }

    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
    QStringList strlstResultValues;
    for (const QString& strGraphObjKeyInTree : strlstGraphObjsKeyInTreeGetResultValues) {
        CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
        if (pGraphObj != nullptr) {
            strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
        }
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepModifyGraphObjByMovingSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    EGraphObjType graphObjType = str2GraphObjType(strGraphObjType);
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();

    // P0 got to be set if the object need to be selected before clicking on and moving the selection point.
    QPointF pt0 = i_pTestStep->getConfigValue("P0").toPointF();
    QPointF pt1 = i_pTestStep->getConfigValue("P1").toPointF();
    QPointF pt2 = i_pTestStep->getConfigValue("P2").toPointF();
    QString strMethod = "mousePressEventToSelectObject";
    if (i_pTestStep->hasConfigValue("Method")) {
        strMethod = i_pTestStep->getConfigValue("Method").toString();
    }

    if (strMethod == "mousePressEventToSelectObject") {
        m_pDrawingScene->setCurrentDrawingTool(nullptr);
        Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
        if (i_pTestStep->hasConfigValue("KeyboardModifiers")) {
            keyboardModifiers = static_cast<Qt::KeyboardModifiers>(i_pTestStep->getConfigValue("KeyboardModifiers").toInt());
        }
        QPoint ptMousePos = pt0.toPoint();
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
        // Mouse press event used to to select the object before clicking on the selection point?
        i_pTestStep->setConfigValue("Method", "mouseReleaseEventToSelectObject");
        triggerDoTestStep();
    }
    else if (strMethod == "mouseReleaseEventToSelectObject") {
        Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
        if (i_pTestStep->hasConfigValue("KeyboardModifiers")) {
            keyboardModifiers = static_cast<Qt::KeyboardModifiers>(i_pTestStep->getConfigValue("KeyboardModifiers").toInt());
        }
        QPoint ptMousePos = pt0.toPoint();
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
        i_pTestStep->setConfigValue("Method", "mousePressEventOnSelectionPoint");
        triggerDoTestStep();
    }
    else if (strMethod == "mousePressEventOnSelectionPoint") {
        Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
        if (i_pTestStep->hasConfigValue("KeyboardModifiers")) {
            keyboardModifiers = static_cast<Qt::KeyboardModifiers>(i_pTestStep->getConfigValue("KeyboardModifiers").toInt());
        }
        QPoint ptMousePos = pt1.toPoint();
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
        int iMovesCount = 0;
        if (i_pTestStep->hasConfigValue("MouseMovesCount")) {
            iMovesCount = i_pTestStep->getConfigValue("MouseMovesCount").toInt();
        }
        addMouseMoveEventDataRows(i_pTestStep, pt1.toPoint(), pt2.toPoint(), iMovesCount);
        i_pTestStep->setConfigValue("Method", "mouseMoveEventOnSelectionPoint");
        triggerDoTestStep();
    }
    else if (strMethod == "mouseMoveEventOnSelectionPoint") {
        int iMouseMovesRemaining = i_pTestStep->getDataRowCount();
        if (iMouseMovesRemaining > 0) {
            Qt::MouseButton mouseBtn = Qt::LeftButton;
            if (i_pTestStep->hasConfigValue("MouseButtons")) {
                mouseBtn = static_cast<Qt::MouseButton>(i_pTestStep->getConfigValue("MouseButtons").toInt());
            }
            QHash<QString, QVariant> dataRow = i_pTestStep->takeDataRow(0);
            QPoint ptMousePos = dataRow["MousePos"].toPoint();
            QPoint ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptMousePos);
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
        }
        else {
            i_pTestStep->setConfigValue("Method", "mouseReleaseEventOnSelectionPoint");
        }
        triggerDoTestStep();
    }
    else if (strMethod == "mouseReleaseEventOnSelectionPoint") {
        Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
        if (i_pTestStep->hasConfigValue("KeyboardModifiers")) {
            keyboardModifiers = static_cast<Qt::KeyboardModifiers>(i_pTestStep->getConfigValue("KeyboardModifiers").toInt());
        }
        QPoint ptMousePos = pt2.toPoint();
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
        i_pTestStep->setConfigValue("Method", "setResultValues");
        triggerDoTestStep();
    }
    else if (strMethod == "setResultValues") {
        QStringList strlstResultValues;
        QStringList strlstGraphObjsKeyInTreeGetResultValues;
        if (i_pTestStep->hasConfigValue("GraphObjsKeyInTreeGetResultValues")) {
            strlstGraphObjsKeyInTreeGetResultValues = i_pTestStep->getConfigValue("GraphObjsKeyInTreeGetResultValues").toStringList();
        }
        if (strlstGraphObjsKeyInTreeGetResultValues.isEmpty()) {
            strlstGraphObjsKeyInTreeGetResultValues.append(strGraphObjKeyInTree);
            if (graphObjType == EGraphObjTypeGroup) {
                CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(m_pDrawingScene->findGraphObj(strGraphObjKeyInTree));
                if (pGraphObjGroup != nullptr) {
                    for (CGraphObj* pGraphObjChild : pGraphObjGroup->childs()) {
                        strlstGraphObjsKeyInTreeGetResultValues.append(pGraphObjChild->keyInTree());
                    }
                }
            }
        }
        int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
            i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
        for (const QString& strGraphObjKeyInTree : strlstGraphObjsKeyInTreeGetResultValues) {
            CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
            if (pGraphObj != nullptr) {
                strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
            }
        }
        i_pTestStep->setResultValues(strlstResultValues);
        i_pTestStep->removeConfigValue("Method"); // to allow that the test may be called several times
    }
}

//------------------------------------------------------------------------------
void CTest::doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepModifyGraphObjByMouseEvents",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    EGraphObjType graphObjType = str2GraphObjType(strGraphObjType);
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();

    if (i_pTestStep->getDataRowCount() > 0) {
        QHash<QString, QVariant> dataRow = i_pTestStep->takeDataRow(0);
        QString strMethod = dataRow["Method"].toString();
        if (strMethod == "setCurrentDrawingTool") {
            QString strFactoryGroupName = dataRow["FactoryGroupName"].toString();
            if (strFactoryGroupName.isEmpty()) {
                m_pDrawingScene->setCurrentDrawingTool(nullptr);
            }
            else {
                QString strFactoryGraphObjType = dataRow["FactoryGraphObjType"].toString();
                m_pDrawingScene->setCurrentDrawingTool(strFactoryGroupName, strFactoryGraphObjType);
            }
            triggerDoTestStep();
        }
        else if (strMethod == "mousePressEvent") {
            Qt::MouseButton mouseButton = Qt::LeftButton;
            if (dataRow.contains("MouseButton")) {
                mouseButton = static_cast<Qt::MouseButton>(dataRow["MouseButton"].toInt());
            }
            Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
            if (dataRow.contains("KeyboardModifiers")) {
                keyboardModifiers = static_cast<Qt::KeyboardModifiers>(dataRow["KeyboardModifiers"].toInt());
            }
            QPoint ptMousePos = dataRow["MousePos"].toPoint();
            QPoint ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptMousePos);
            QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.
            QMouseEvent* pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ mouseButton,
                /* button    */ mouseButton,
                /* modifiers */ keyboardModifiers );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
            triggerDoTestStep();
        }
        else if (strMethod == "mouseReleaseEvent") {
            Qt::MouseButton mouseButton = Qt::LeftButton;
            if (dataRow.contains("MouseButton")) {
                mouseButton = static_cast<Qt::MouseButton>(dataRow["MouseButton"].toInt());
            }
            Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
            if (dataRow.contains("KeyboardModifiers")) {
                keyboardModifiers = static_cast<Qt::KeyboardModifiers>(dataRow["KeyboardModifiers"].toInt());
            }
            QPoint ptMousePos = dataRow["MousePos"].toPoint();
            QPoint ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptMousePos);
            QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.
            QMouseEvent* pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ mouseButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ keyboardModifiers );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
            triggerDoTestStep();
        }
        else if (strMethod == "mouseMoveEvent") {
            Qt::MouseButton mouseBtns = Qt::NoButton;
            if (dataRow.contains("MouseButtons")) {
                mouseBtns = static_cast<Qt::MouseButton>(dataRow["MouseButtons"].toInt());
            }
            Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
            if (dataRow.contains("KeyboardModifiers")) {
                keyboardModifiers = static_cast<Qt::KeyboardModifiers>(dataRow["KeyboardModifiers"].toInt());
            }
            QPoint ptMousePos = dataRow["MousePos"].toPoint();
            QPoint ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptMousePos);
            QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptDrawingViewMousePos);
            QMouseEvent* pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ mouseBtns,
                /* modifiers */ keyboardModifiers );
            m_pDrawingView->mouseMoveEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
            triggerDoTestStep();
        }
        else if (strMethod == "activePopupWidget.keyPressEvent") {
            QWidget* pWdgtReceiver = QApplication::activePopupWidget();
            if (pWdgtReceiver != nullptr) {
                Qt::Key key = Qt::Key_Escape;
                if (dataRow.contains("Key")) {
                    key = static_cast<Qt::Key>(dataRow["Key"].toInt());
                }
                Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
                if (dataRow.contains("KeyboardModifiers")) {
                    keyboardModifiers = static_cast<Qt::KeyboardModifiers>(dataRow["KeyboardModifiers"].toInt());
                }
                QKeyEvent* pKeyEvent = new QKeyEvent(
                    /* type      */ QEvent::KeyPress,
                    /* iKey      */ key,
                    /* modifiers */ keyboardModifiers,
                    /* strText   */ "",
                    /* bAutoRep  */ false,
                    /* uCount    */ 1 );
                QApplication::postEvent(pWdgtReceiver, pKeyEvent);
                pKeyEvent = nullptr;
            }
            triggerDoTestStep();
        }
        else if (strMethod == "activePopupWidget.keyReleaseEvent") {
            QWidget* pWdgtReceiver = QApplication::activePopupWidget();
            if (pWdgtReceiver != nullptr) {
                Qt::Key key = Qt::Key_Escape;
                if (dataRow.contains("Key")) {
                    key = static_cast<Qt::Key>(dataRow["Key"].toInt());
                }
                Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
                if (dataRow.contains("KeyboardModifiers")) {
                    keyboardModifiers = static_cast<Qt::KeyboardModifiers>(dataRow["KeyboardModifiers"].toInt());
                }
                QKeyEvent* pKeyEvent = new QKeyEvent(
                    /* type      */ QEvent::KeyRelease,
                    /* iKey      */ key,
                    /* modifiers */ keyboardModifiers,
                    /* strText   */ "",
                    /* bAutoRep  */ false,
                    /* uCount    */ 1 );
                QApplication::postEvent(pWdgtReceiver, pKeyEvent);
                pKeyEvent = nullptr;
            }
            triggerDoTestStep();
        }
    }
    else {
        QStringList strlstResultValues;
        QStringList strlstGraphObjsKeyInTreeGetResultValues;
        if (i_pTestStep->hasConfigValue("GraphObjsKeyInTreeGetResultValues")) {
            strlstGraphObjsKeyInTreeGetResultValues = i_pTestStep->getConfigValue("GraphObjsKeyInTreeGetResultValues").toStringList();
        }
        if (strlstGraphObjsKeyInTreeGetResultValues.isEmpty()) {
            strlstGraphObjsKeyInTreeGetResultValues.append(strGraphObjKeyInTree);
            if (graphObjType == EGraphObjTypeGroup) {
                CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(m_pDrawingScene->findGraphObj(strGraphObjKeyInTree));
                if (pGraphObjGroup != nullptr) {
                    for (CGraphObj* pGraphObjChild : pGraphObjGroup->childs()) {
                        strlstGraphObjsKeyInTreeGetResultValues.append(pGraphObjChild->keyInTree());
                    }
                }
            }
        }
        int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
            i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
        for (const QString& strGraphObjKeyInTree : strlstGraphObjsKeyInTreeGetResultValues) {
            CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
            if (pGraphObj != nullptr) {
                strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
            }
        }
        i_pTestStep->setResultValues(strlstResultValues);
        i_pTestStep->removeConfigValue("Method"); // to allow that the test may be called several times
    }
}

//------------------------------------------------------------------------------
void CTest::doTestStepSelectAndUngroup(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepSelectAndUngroup",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    EGraphObjType graphObjType = str2GraphObjType(strGraphObjType);
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();

    // P0 got to be set if the object need to be selected before clicking on and moving the selection point.
    QPointF pt0 = i_pTestStep->getConfigValue("P0").toPointF();
    QString strMethod = "mousePressEvent";
    if (i_pTestStep->hasConfigValue("Method")) {
        strMethod = i_pTestStep->getConfigValue("Method").toString();
    }

    if (strMethod == "mousePressEvent") {
        Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
        if (i_pTestStep->hasConfigValue("KeyboardModifiers")) {
            keyboardModifiers = static_cast<Qt::KeyboardModifiers>(i_pTestStep->getConfigValue("KeyboardModifiers").toInt());
        }
        QPoint ptMousePos = pt0.toPoint();
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
        i_pTestStep->setConfigValue("Method", "mouseReleaseEvent");
        triggerDoTestStep();
    }
    else if (strMethod == "mouseReleaseEvent") {
        Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
        if (i_pTestStep->hasConfigValue("KeyboardModifiers")) {
            keyboardModifiers = static_cast<Qt::KeyboardModifiers>(i_pTestStep->getConfigValue("KeyboardModifiers").toInt());
        }
        QPoint ptMousePos = pt0.toPoint();
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
        i_pTestStep->setConfigValue("Method", "ungroup");
        triggerDoTestStep();
    }
    else if (strMethod == "ungroup") {
        m_pDrawingScene->ungroupGraphObjsSelected();
        i_pTestStep->setConfigValue("Method", "setResultValues");
        triggerDoTestStep();
    }
    else if (strMethod == "setResultValues") {
        QStringList strlstResultValues;
        QStringList strlstGraphObjsKeyInTreeGetResultValues;
        if (i_pTestStep->hasConfigValue("GraphObjsKeyInTreeGetResultValues")) {
            strlstGraphObjsKeyInTreeGetResultValues = i_pTestStep->getConfigValue("GraphObjsKeyInTreeGetResultValues").toStringList();
        }
        if (strlstGraphObjsKeyInTreeGetResultValues.isEmpty()) {
            strlstGraphObjsKeyInTreeGetResultValues.append(strGraphObjKeyInTree);
            if (graphObjType == EGraphObjTypeGroup) {
                CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(m_pDrawingScene->findGraphObj(strGraphObjKeyInTree));
                if (pGraphObjGroup != nullptr) {
                    for (CGraphObj* pGraphObjChild : pGraphObjGroup->childs()) {
                        strlstGraphObjsKeyInTreeGetResultValues.append(pGraphObjChild->keyInTree());
                    }
                }
            }
        }
        int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
            i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;
        for (const QString& strGraphObjKeyInTree : strlstGraphObjsKeyInTreeGetResultValues) {
            CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
            if (pGraphObj != nullptr) {
                strlstResultValues.append(resultValuesForGraphObj(pGraphObj, false, iResultValuesPrecision));
            }
        }
        i_pTestStep->setResultValues(strlstResultValues);
        i_pTestStep->removeConfigValue("Method"); // to allow that the test may be called several times
    }
}

//------------------------------------------------------------------------------
void CTest::doTestStepSaveLoadFile(ZS::Test::CTestStep* i_pTestStep)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepSaveLoadFile",
        /* strAddInfo   */ strMthInArgs );

    QString strAbsDirPath = i_pTestStep->getConfigValue("AbsDirPath").toString();
    QString strFileName = i_pTestStep->getConfigValue("FileName").toString();
    QString strAbsFilePath = strAbsDirPath + QDir::separator() + strFileName;
    int iResultValuesPrecision = i_pTestStep->hasConfigValue("ResultValuesPrecision") ?
        i_pTestStep->getConfigValue("ResultValuesPrecision").toInt() : -1;

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
        // Connection lines will be saved at the end of the XML file.
        while (itIdxTree != pIdxTree->end()) {
            CIdxTreeEntry* pTreeEntry = *itIdxTree;
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pTreeEntry);
            if (pGraphObj != nullptr && !pGraphObj->isConnectionLine()) {
                strlstExpectedValues.append(resultValuesForGraphObj(pGraphObj, true, iResultValuesPrecision));
            }
            ++itIdxTree;
        }
        itIdxTree = pIdxTree->begin(CIdxTree::iterator::ETraversalOrder::PreOrder);
        while (itIdxTree != pIdxTree->end()) {
            CIdxTreeEntry* pTreeEntry = *itIdxTree;
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pTreeEntry);
            if (pGraphObj != nullptr && pGraphObj->isConnectionLine()) {
                strlstExpectedValues.append(resultValuesForGraphObj(pGraphObj, true, iResultValuesPrecision));
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
                    strlstResultValues.append(resultValuesForGraphObj(pGraphObj, true, iResultValuesPrecision));
                }
                ++itIdxTree;
            }
            i_pTestStep->setResultValues(strlstResultValues);
        }
    }
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::initInstCounts()
//------------------------------------------------------------------------------
{
    CGraphObjLine::s_iInstCount = 0;
    CGraphObjRect::s_iInstCount = 0;
    CGraphObjEllipse::s_iInstCount = 0;
    CGraphObjPolygon::s_iInstCount = 0;
    CGraphObjText::s_iInstCount = 0;
    CGraphObjImage::s_iInstCount = 0;
    CGraphObjConnectionPoint::s_iInstCount = 0;
    CGraphObjConnectionLine::s_iInstCount = 0;
    CGraphObjGroup::s_iInstCount = 0;

    m_hshGraphObjNameToKeys.clear();
}

//------------------------------------------------------------------------------
void CTest::initObjectCoors()
//------------------------------------------------------------------------------
{
    // Lines
    //------

    m_ptPosSmallPlusSignVerticalLine = QPointF();
    m_lineSmallPlusSignVerticalLine = QLineF();
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene);
    m_ptPosSmallPlusSignHorizontalLine = QPointF();
    m_lineSmallPlusSignHorizontalLine = QLineF();
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene);

    m_ptPosBigPlusSignVerticalLine = QPointF();
    m_lineBigPlusSignVerticalLine = QLineF();
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene);
    m_ptPosBigPlusSignHorizontalLine = QPointF();
    m_lineBigPlusSignHorizontalLine = QLineF();
    *m_pPhysValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene);

    m_ptPosCheckmarkLeftLine = QPointF();
    m_lineCheckmarkLeftLine = QLineF();
    *m_pPhysValLineCheckmarkLeftLine = CPhysValLine(*m_pDrawingScene);
    m_ptPosCheckmarkRightLine = QPointF();
    m_lineCheckmarkRightLine = QLineF();
    *m_pPhysValLineCheckmarkRightLine = CPhysValLine(*m_pDrawingScene);

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

    // Rectangles
    //-----------

    m_ptPosRectangle = QPointF();
    m_rectRectangle = QRectF();
    *m_pPhysValRectRectangle = CPhysValRect(*m_pDrawingScene);
    m_physValAngleRectangle = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosRectBigCrossVerticalBar = QPointF();
    m_rectRectBigCrossVerticalBar = QRectF();
    *m_pPhysValRectBigCrossVerticalBar = CPhysValRect(*m_pDrawingScene);
    m_physValAngleBigCrossVerticalBar = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosRectBigCrossHorizontalBar = QPointF();
    m_rectRectBigCrossHorizontalBar = QRectF();
    *m_pPhysValRectBigCrossHorizontalBar = CPhysValRect(*m_pDrawingScene);
    m_physValAngleBigCrossHorizontalBar = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosRectSmallCross1VerticalBar = QPointF();
    m_rectRectSmallCross1VerticalBar = QRectF();
    *m_pPhysValRectSmallCross1VerticalBar = CPhysValRect(*m_pDrawingScene);
    m_physValAngleSmallCross1VerticalBar = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosRectSmallCross1HorizontalBar = QPointF();
    m_rectRectSmallCross1HorizontalBar = QRectF();
    *m_pPhysValRectSmallCross1HorizontalBar = CPhysValRect(*m_pDrawingScene);
    m_physValAngleSmallCross1HorizontalBar = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosRectSmallCross2VerticalBar = QPointF();
    m_rectRectSmallCross2VerticalBar = QRectF();
    *m_pPhysValRectSmallCross2VerticalBar = CPhysValRect(*m_pDrawingScene);
    m_physValAngleSmallCross2VerticalBar = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosRectSmallCross2HorizontalBar = QPointF();
    m_rectRectSmallCross2HorizontalBar = QRectF();
    *m_pPhysValRectSmallCross2HorizontalBar = CPhysValRect(*m_pDrawingScene);
    m_physValAngleSmallCross2HorizontalBar = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    // Ellipses
    //----------

    m_ptPosEllipse = QPointF();
    m_rectEllipse = QRectF();
    *m_pPhysValRectEllipse = CPhysValRect(*m_pDrawingScene);
    m_physValAngleEllipse = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    // Texts
    //----------

    m_ptPosText = QPointF();
    *m_pPhysValRectText = CPhysValRect(*m_pDrawingScene);
    m_physValAngleText = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    // Polygons
    //----------

    m_ptPosOpenArrow = QPointF();
    m_polygonOpenArrow = QPolygonF();
    *m_pPhysValPolygonOpenArrow = CPhysValPolygon(*m_pDrawingScene);
    m_physValAngleOpenArrow = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosTriangle = QPointF();
    m_polygonTriangle = QPolygonF();
    *m_pPhysValPolygonTriangle = CPhysValPolygon(*m_pDrawingScene);
    m_physValAngleTriangle = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosStar = QPointF();
    m_polygonStar = QPolygonF();
    *m_pPhysValPolygonStar = CPhysValPolygon(*m_pDrawingScene);
    m_physValAngleStar = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    // Connection Points
    //------------------

    m_ptPosConnectionPoint1 = QPointF();
    *m_pPhysValConnectionPoint1 = CPhysValPoint(*m_pDrawingScene);

    m_ptPosConnectionPoint2 = QPointF();
    *m_pPhysValConnectionPoint2 = CPhysValPoint(*m_pDrawingScene);

    m_ptPosConnectionPoint3 = QPointF();
    *m_pPhysValConnectionPoint3 = CPhysValPoint(*m_pDrawingScene);

    m_ptPosConnectionPoint4 = QPointF();
    *m_pPhysValConnectionPoint4 = CPhysValPoint(*m_pDrawingScene);

    // Connection Lines
    //-----------------

    m_polygonConnectionLineCnctPt1CnctPt2 = QPolygonF();
    *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2 = CPhysValPolygon(*m_pDrawingScene);

    m_polygonConnectionLineCnctPt3CnctPt4 = QPolygonF();
    *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4 = CPhysValPolygon(*m_pDrawingScene);

    m_polygonConnectionLineCnctPt1CnctPt4 = QPolygonF();
    *m_pPhysValPolygonConnectionLineCnctPt1CnctPt4 = CPhysValPolygon(*m_pDrawingScene);

    m_polygonConnectionLineCnctPt3CnctPt2 = QPolygonF();
    *m_pPhysValPolygonConnectionLineCnctPt3CnctPt2 = CPhysValPolygon(*m_pDrawingScene);

    // Groups
    //-------

    m_ptPosSmallPlusSign = QPointF();
    m_sizeSmallPlusSign = QSizeF();
    *m_pPhysValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene);
    m_physValAngleSmallPlusSign = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosBigPlusSign = QPointF();
    m_sizeBigPlusSign = QSizeF();
    *m_pPhysValRectBigPlusSign = CPhysValRect(*m_pDrawingScene);
    m_physValAngleBigPlusSign = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosCheckmark = QPointF();
    m_sizeCheckmark = QSizeF();
    *m_pPhysValRectCheckmark = CPhysValRect(*m_pDrawingScene);
    m_physValAngleCheckmark = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosSmallRect = QPointF();
    m_sizeSmallRect = QSizeF();
    *m_pPhysValRectSmallRect = CPhysValRect(*m_pDrawingScene);
    m_physValAngleSmallRect = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosCrosses = QPointF();
    m_sizeCrosses = QSizeF();
    *m_pPhysValRectCrosses = CPhysValRect(*m_pDrawingScene);
    m_physValAngleCrosses = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosBigCross = QPointF();
    m_sizeBigCross = QSizeF();
    *m_pPhysValRectBigCross = CPhysValRect(*m_pDrawingScene);
    m_physValAngleBigCross = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosSmallCross1 = QPointF();
    m_sizeSmallCross1 = QSizeF();
    *m_pPhysValRectSmallCross1 = CPhysValRect(*m_pDrawingScene);
    m_physValAngleSmallCross1 = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosSmallCross2 = QPointF();
    m_sizeSmallCross2 = QSizeF();
    *m_pPhysValRectSmallCross2 = CPhysValRect(*m_pDrawingScene);
    m_physValAngleSmallCross2 = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosPolygons = QPointF();
    m_sizePolygons = QSizeF();
    *m_pPhysValRectPolygons = CPhysValRect(*m_pDrawingScene);
    m_physValAnglePolygons = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosConnectionPoints = QPointF();
    m_sizeConnectionPoints = QSizeF();
    *m_pPhysValRectConnectionPoints = CPhysValRect(*m_pDrawingScene);
    m_physValAngleConnectionPoints = CPhysVal(0.0, Units.Angle.Degree, 0.1);

    m_ptPosTopGroup = QPointF();
    m_sizeTopGroup = QSizeF();
    *m_pPhysValRectTopGroup = CPhysValRect(*m_pDrawingScene);
    m_physValAngleTopGroup = CPhysVal(0.0, Units.Angle.Degree, 0.1);
}

//------------------------------------------------------------------------------
/*! @brief Adds data rows to simulate mouse events and returns the final mouse position.
*/
QPoint CTest::addMouseMoveEventDataRows(
    ZS::Test::CTestStep* i_pTestStep,
    const QPoint& i_ptMousePosStart, const QPoint& i_ptMousePosStop,
    int i_iMovesCount,
    Qt::MouseButton i_mouseBtns, Qt::KeyboardModifiers i_modifiers)
//------------------------------------------------------------------------------
{
    int iMovesCount = i_iMovesCount;
    bool bMoveRight = i_ptMousePosStop.x() > i_ptMousePosStart.x();
    bool bMoveLeft = i_ptMousePosStop.x() < i_ptMousePosStart.x();
    bool bMoveDown = i_ptMousePosStop.y() > i_ptMousePosStart.y();
    bool bMoveUp = i_ptMousePosStop.y() < i_ptMousePosStart.y();
    int xDist_px = i_ptMousePosStop.x() - i_ptMousePosStart.x(); // might be negative
    int yDist_px = i_ptMousePosStop.y() - i_ptMousePosStart.y(); // might be negative
    if (iMovesCount == 0) {
        if (abs(xDist_px) >= abs(yDist_px)) {
            iMovesCount = abs(xDist_px) / 10;
            if (abs(xDist_px) % 10 != 0) {
                ++iMovesCount;
            }
        }
        else {
            iMovesCount = abs(yDist_px) / 10;
            if (abs(yDist_px) % 10 != 0) {
                ++iMovesCount;
            }
        }
    }
    double fXStep_px = (static_cast<double>(xDist_px) / static_cast<double>(iMovesCount));
    double fYStep_px = (static_cast<double>(yDist_px) / static_cast<double>(iMovesCount));
    QPointF ptMouseMovePos = i_ptMousePosStart;
    for (int iMoves = 0; iMoves < iMovesCount; ++iMoves) {
        ptMouseMovePos.setX(ptMouseMovePos.x() + fXStep_px);
        ptMouseMovePos.setY(ptMouseMovePos.y() + fYStep_px);
        if (bMoveRight && ptMouseMovePos.x() > i_ptMousePosStop.x()) {
            ptMouseMovePos.setX(i_ptMousePosStop.x());
        }
        else if (bMoveLeft && ptMouseMovePos.x() < i_ptMousePosStop.x()) {
            ptMouseMovePos.setX(i_ptMousePosStop.x());
        }
        if (bMoveDown && ptMouseMovePos.y() > i_ptMousePosStop.y()) {
            ptMouseMovePos.setY(i_ptMousePosStop.y());
        }
        else if (bMoveUp && ptMouseMovePos.y() < i_ptMousePosStop.y()) {
            ptMouseMovePos.setY(i_ptMousePosStop.y());
        }
        i_pTestStep->addDataRow({
            {"Method", "mouseMoveEvent"},
            {"MousePos", ptMouseMovePos.toPoint()},
            {"MouseButtons", static_cast<int>(i_mouseBtns)},
            {"KeyboardModifiers", static_cast<int>(i_modifiers)}
        });
        if (bMoveRight && ptMouseMovePos.x() >= i_ptMousePosStop.x()) {
            break;
        }
        else if (bMoveLeft && ptMouseMovePos.x() <= i_ptMousePosStop.x()) {
            break;
        }
        if (bMoveDown && ptMouseMovePos.y() >= i_ptMousePosStop.y()) {
            break;
        }
        else if (bMoveUp && ptMouseMovePos.y() <= i_ptMousePosStop.y()) {
            break;
        }
    }
    return i_ptMousePosStop;
}

//------------------------------------------------------------------------------
QPointF CTest::getSelectionPointCoors(
    const CPhysValPolygon& i_physValPolygon, const SGraphObjSelectionPoint& i_selPt) const
//------------------------------------------------------------------------------
{
    QPointF pt;
    if (i_selPt.m_selPtType == ESelectionPointType::BoundingRectangle) {
        pt = getSelectionPointCoors(i_physValPolygon.physValBoundingRect(), i_selPt.m_selPt);
    }
    else if (i_selPt.m_selPtType == ESelectionPointType::PolygonPoint) {
        CPhysValPoint physValPoint = i_physValPolygon.at(i_selPt.m_idxPt);
        pt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
    }
    else if (i_selPt.m_selPtType == ESelectionPointType::LineCenterPoint) {
        CPhysValPoint physValPointLineStart = i_physValPolygon.at(i_selPt.m_idxPt);
        CPhysValPoint physValPointLineEnd = i_selPt.m_idxPt+1 >= i_physValPolygon.count() ?
            i_physValPolygon.at(0) : i_physValPolygon.at(i_selPt.m_idxPt+1);
        CPhysValLine physValLine(physValPointLineStart, physValPointLineEnd);
        physValLine = m_pDrawingScene->convert(physValLine, Units.Length.px);
        pt = physValLine.center().toQPointF();
    }
    return pt;
}

//------------------------------------------------------------------------------
QPointF CTest::getSelectionPointCoors(
    const CPhysValRect& i_physValRect, ESelectionPoint i_selPt) const
//------------------------------------------------------------------------------
{
    QPointF pt;
    if (i_selPt == ESelectionPoint::TopLeft) {
        CPhysValPoint physValPoint = i_physValRect.topLeft();
        pt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
    }
    else if (i_selPt == ESelectionPoint::TopCenter) {
        CPhysValPoint physValPoint = i_physValRect.topCenter();
        pt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
    }
    else if (i_selPt == ESelectionPoint::TopRight) {
        CPhysValPoint physValPoint = i_physValRect.topRight();
        pt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
    }
    else if (i_selPt == ESelectionPoint::RightCenter) {
        CPhysValPoint physValPoint = i_physValRect.rightCenter();
        pt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
    }
    else if (i_selPt == ESelectionPoint::BottomRight) {
        CPhysValPoint physValPoint = i_physValRect.bottomRight();
        pt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
    }
    else if (i_selPt == ESelectionPoint::BottomCenter) {
        CPhysValPoint physValPoint = i_physValRect.bottomCenter();
        pt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
    }
    else if (i_selPt == ESelectionPoint::BottomLeft) {
        CPhysValPoint physValPoint = i_physValRect.bottomLeft();
        pt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
    }
    else if (i_selPt == ESelectionPoint::LeftCenter) {
        CPhysValPoint physValPoint = i_physValRect.leftCenter();
        pt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
    }
    else if (i_selPt == ESelectionPoint::Center) {
        CPhysValPoint physValPoint = i_physValRect.center();
        pt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
    }
    else if (i_selPt == ESelectionPoint::RotateTop || i_selPt == ESelectionPoint::RotateBottom) {
        // ESelectionPoint::RotateTop: 270° (clockwise counted)
        // ESelectionPoint::RotateBottom: 90° (clockwise counted)
        // Original position
        double fAngle_degree = i_physValRect.angle().getVal(Units.Angle.Degree);
        fAngle_degree += i_selPt == ESelectionPoint::RotateTop ? 270.0 : 90.0;
        fAngle_degree = ZS::System::Math::toCounterClockWiseAngleDegree(fAngle_degree);
        double fAngle_rad = ZS::System::Math::degree2Rad(fAngle_degree);
        double dxSelPt = ZS::Draw::getSelectionPointRotateDistance() * cos(fAngle_rad);
        double dySelPt = ZS::Draw::getSelectionPointRotateDistance() * sin(fAngle_rad);
        CPhysValPoint physValPoint = i_selPt == ESelectionPoint::RotateTop ?
            i_physValRect.topCenter() : i_physValRect.bottomCenter();
        pt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
        pt.setX(pt.x() + dxSelPt);
        pt.setY(pt.y() - dySelPt);
    }
    return pt;
}

////------------------------------------------------------------------------------
//void CTest::getSelectionPointCoors(
//    const ZS::Draw::SGraphObjSelectionPoint& i_selPt,
//    const ZS::Draw::CPhysValPolygon& i_physValPolygonCurr, const ZS::Draw::CPhysValPolygon& i_physValPolygonNew,
//    QPointF& o_ptSelPtRectCurr, QPointF& o_ptSelPtRectNew)
////------------------------------------------------------------------------------
//{
//    if (i_selPt.m_selPtType == ESelectionPointType::BoundingRectangle) {
//        getSelectionPointCoors(
//            i_selPt.m_selPt, i_physValPolygonCurr.physValBoundingRect(), i_physValPolygonNew.physValBoundingRect(),
//            o_ptSelPtRectCurr, o_ptSelPtRectNew);
//    }
//    else if (i_selPt.m_selPtType == ESelectionPointType::PolygonPoint) {
//        CPhysValPoint physValPoint = i_physValPolygonCurr.at(i_selPt.m_idxPt);
//        o_ptSelPtRectCurr = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//        physValPoint = i_physValPolygonNew.at(i_selPt.m_idxPt);
//        o_ptSelPtRectNew = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//    }
//    else if (i_selPt.m_selPtType == ESelectionPointType::LineCenterPoint) {
//        CPhysValPoint physValPointLineStart = i_physValPolygonCurr.at(i_selPt.m_idxPt);
//        CPhysValPoint physValPointLineEnd = i_selPt.m_idxPt+1 >= i_physValPolygonCurr.count() ? i_physValPolygonCurr.at(0) : i_physValPolygonCurr.at(i_selPt.m_idxPt+1);
//        CPhysValLine physValLine(physValPointLineStart, physValPointLineEnd);
//        physValLine = m_pDrawingScene->convert(physValLine, Units.Length.px);
//        o_ptSelPtRectCurr = physValLine.center().toQPointF();
//        physValPointLineStart = i_physValPolygonNew.at(i_selPt.m_idxPt);
//        physValPointLineEnd = i_selPt.m_idxPt+1 >= i_physValPolygonNew.count() ? i_physValPolygonNew.at(0) : i_physValPolygonNew.at(i_selPt.m_idxPt+1);
//        physValLine = CPhysValLine(physValPointLineStart, physValPointLineEnd);
//        physValLine = m_pDrawingScene->convert(physValLine, Units.Length.px);
//        o_ptSelPtRectNew = physValLine.center().toQPointF();
//    }
//}

////------------------------------------------------------------------------------
//void CTest::getSelectionPointCoors(
//    ESelectionPoint i_selPt, const CPhysValRect& i_physValRectCurr, const CPhysValRect& i_physValRectNew,
//    QPointF& o_ptSelPtRectCurr, QPointF& o_ptSelPtRectNew)
////------------------------------------------------------------------------------
//{
//    if (i_selPt == ESelectionPoint::TopLeft) {
//        CPhysValPoint physValPoint = i_physValRectCurr.topLeft();
//        o_ptSelPtRectCurr = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//        physValPoint = i_physValRectNew.topLeft();
//        o_ptSelPtRectNew = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//    }
//    else if (i_selPt == ESelectionPoint::TopCenter) {
//        CPhysValPoint physValPoint = i_physValRectCurr.topCenter();
//        o_ptSelPtRectCurr = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//        physValPoint = i_physValRectNew.topCenter();
//        o_ptSelPtRectNew = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//    }
//    else if (i_selPt == ESelectionPoint::TopRight) {
//        CPhysValPoint physValPoint = i_physValRectCurr.topRight();
//        o_ptSelPtRectCurr = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//        physValPoint = i_physValRectNew.topRight();
//        o_ptSelPtRectNew = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//    }
//    else if (i_selPt == ESelectionPoint::RightCenter) {
//        CPhysValPoint physValPoint = i_physValRectCurr.rightCenter();
//        o_ptSelPtRectCurr = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//        physValPoint = i_physValRectNew.rightCenter();
//        o_ptSelPtRectNew = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//    }
//    else if (i_selPt == ESelectionPoint::BottomRight) {
//        CPhysValPoint physValPoint = i_physValRectCurr.bottomRight();
//        o_ptSelPtRectCurr = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//        physValPoint = i_physValRectNew.bottomRight();
//        o_ptSelPtRectNew = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//    }
//    else if (i_selPt == ESelectionPoint::BottomCenter) {
//        CPhysValPoint physValPoint = i_physValRectCurr.bottomCenter();
//        o_ptSelPtRectCurr = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//        physValPoint = i_physValRectNew.bottomCenter();
//        o_ptSelPtRectNew = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//    }
//    else if (i_selPt == ESelectionPoint::BottomLeft) {
//        CPhysValPoint physValPoint = i_physValRectCurr.bottomLeft();
//        o_ptSelPtRectCurr = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//        physValPoint = i_physValRectNew.bottomLeft();
//        o_ptSelPtRectNew = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//    }
//    else if (i_selPt == ESelectionPoint::LeftCenter) {
//        CPhysValPoint physValPoint = i_physValRectCurr.leftCenter();
//        o_ptSelPtRectCurr = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//        physValPoint = i_physValRectNew.leftCenter();
//        o_ptSelPtRectNew = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//    }
//    else if (i_selPt == ESelectionPoint::Center) {
//        CPhysValPoint physValPoint = i_physValRectCurr.center();
//        o_ptSelPtRectCurr = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//        physValPoint = i_physValRectNew.center();
//        o_ptSelPtRectNew = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//    }
//    else if (i_selPt == ESelectionPoint::RotateTop || i_selPt == ESelectionPoint::RotateBottom) {
//        // ESelectionPoint::RotateTop: 270° (clockwise counted)
//        // ESelectionPoint::RotateBottom: 90° (clockwise counted)
//        // Original position
//        double fAngle_degree = i_physValRectCurr.angle().getVal(Units.Angle.Degree);
//        fAngle_degree += i_selPt == ESelectionPoint::RotateTop ? 270.0 : 90.0;
//        fAngle_degree = ZS::System::Math::toCounterClockWiseAngleDegree(fAngle_degree);
//        double fAngle_rad = ZS::System::Math::degree2Rad(fAngle_degree);
//        double dxSelPt = ZS::Draw::getSelectionPointRotateDistance() * cos(fAngle_rad);
//        double dySelPt = ZS::Draw::getSelectionPointRotateDistance() * sin(fAngle_rad);
//        CPhysValPoint physValPoint = i_selPt == ESelectionPoint::RotateTop ?
//            i_physValRectCurr.topCenter() : i_physValRectCurr.bottomCenter();
//        o_ptSelPtRectCurr = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//        o_ptSelPtRectCurr.setX(o_ptSelPtRectCurr.x() + dxSelPt);
//        o_ptSelPtRectCurr.setY(o_ptSelPtRectCurr.y() - dySelPt);
//        // New position
//        fAngle_degree = i_physValRectNew.angle().getVal(Units.Angle.Degree);
//        fAngle_degree += i_selPt == ESelectionPoint::RotateTop ? 270.0 : 90.0;
//        fAngle_degree = ZS::System::Math::toCounterClockWiseAngleDegree(fAngle_degree);
//        fAngle_rad = ZS::System::Math::degree2Rad(fAngle_degree);
//        dxSelPt = ZS::Draw::getSelectionPointRotateDistance() * cos(fAngle_rad);
//        dySelPt = ZS::Draw::getSelectionPointRotateDistance() * sin(fAngle_rad);
//        physValPoint = i_selPt == ESelectionPoint::RotateTop ?
//            i_physValRectNew.topCenter() : i_physValRectNew.bottomCenter();
//        o_ptSelPtRectNew = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
//        o_ptSelPtRectNew.setX(o_ptSelPtRectNew.x() + dxSelPt);
//        o_ptSelPtRectNew.setY(o_ptSelPtRectNew.y() - dySelPt);
//    }
//}

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
QStringList CTest::resultValuesForGraphObj(
    const CGraphObj* i_pGraphObj, bool i_bAddLabelResultValues, int i_iPrecision) const
//------------------------------------------------------------------------------
{
    QStringList strlstResultValues;
    if (i_pGraphObj != nullptr) {
        const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(i_pGraphObj);
        if (i_pGraphObj->isLine()) {
            const CGraphObjLine* pGraphObjLine = dynamic_cast<const CGraphObjLine*>(i_pGraphObj);
            if (pGraphObjLine != nullptr) {
                strlstResultValues = resultValuesForLine(
                    i_pGraphObj->name(), pGraphicsItem->pos(),
                    pGraphObjLine->line(), pGraphObjLine->getLine(), i_iPrecision);
            }
        }
        else if (i_pGraphObj->isRect()) {
            const CGraphObjRect* pGraphObjRect = dynamic_cast<const CGraphObjRect*>(i_pGraphObj);
            if (pGraphObjRect != nullptr) {
                strlstResultValues = resultValuesForRect(
                    i_pGraphObj->name(), pGraphicsItem->pos(),
                    pGraphObjRect->rect(), pGraphObjRect->getRect(), i_iPrecision);
            }
        }
        else if (i_pGraphObj->isEllipse()) {
            const CGraphObjEllipse* pGraphObjEllipse = dynamic_cast<const CGraphObjEllipse*>(i_pGraphObj);
            if (pGraphObjEllipse != nullptr) {
                strlstResultValues = resultValuesForEllipse(
                    i_pGraphObj->name(), pGraphicsItem->pos(),
                    pGraphObjEllipse->rect(), pGraphObjEllipse->getRect(), i_iPrecision);
            }
        }
        else if (i_pGraphObj->isText()) {
            const CGraphObjText* pGraphObjText = dynamic_cast<const CGraphObjText*>(i_pGraphObj);
            if (pGraphObjText != nullptr) {
                strlstResultValues = resultValuesForText(
                    i_pGraphObj->name(), pGraphicsItem->pos(),
                    pGraphObjText->getRect(), pGraphObjText->toPlainText(), i_iPrecision);
            }
        }
        else if (i_pGraphObj->isPolyline() || i_pGraphObj->isPolygon()) {
            const CGraphObjPolygon* pGraphObjPolygon = dynamic_cast<const CGraphObjPolygon*>(i_pGraphObj);
            if (pGraphObjPolygon != nullptr) {
                strlstResultValues = resultValuesForPolygon(
                    i_pGraphObj->name(), pGraphicsItem->pos(),
                    pGraphObjPolygon->polygon(), pGraphObjPolygon->getPolygon(), i_iPrecision);
            }
        }
        else if (i_pGraphObj->isConnectionPoint()) {
            const CGraphObjConnectionPoint* pGraphObjConnectionPoint = dynamic_cast<const CGraphObjConnectionPoint*>(i_pGraphObj);
            if (pGraphObjConnectionPoint != nullptr) {
                strlstResultValues = resultValuesForConnectionPoint(
                    i_pGraphObj->name(), pGraphicsItem->pos(),
                    pGraphObjConnectionPoint->position(), i_iPrecision);
            }
        }
        else if (i_pGraphObj->isConnectionLine()) {
            const CGraphObjConnectionLine* pGraphObjConnectionLine = dynamic_cast<const CGraphObjConnectionLine*>(i_pGraphObj);
            if (pGraphObjConnectionLine != nullptr) {
                CGraphObjConnectionPoint* pGraphObjConnectionPointP1 = pGraphObjConnectionLine->getConnectionPoint(ELinePoint::Start);
                CGraphObjConnectionPoint* pGraphObjConnectionPointP2 = pGraphObjConnectionLine->getConnectionPoint(ELinePoint::End);
                QString strNameCntPt1 = pGraphObjConnectionPointP1 == nullptr ? "" : pGraphObjConnectionPointP1->name();
                QString strNameCntPt2 = pGraphObjConnectionPointP2 == nullptr ? "" : pGraphObjConnectionPointP2->name();
                strlstResultValues = resultValuesForConnectionLine(
                    i_pGraphObj->name(), strNameCntPt1, strNameCntPt2,
                    pGraphObjConnectionLine->polygon(), pGraphObjConnectionLine->getPolygon(), i_iPrecision);
            }
        }
        else if (i_pGraphObj->isGroup()) {
            const CGraphObjGroup* pGraphObjGroup = dynamic_cast<const CGraphObjGroup*>(i_pGraphObj);
            if (pGraphObjGroup != nullptr) {
                strlstResultValues = resultValuesForGroup(
                    i_pGraphObj->name(), pGraphicsItem->pos(), pGraphObjGroup->getRect(), i_iPrecision);
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
                        pGraphicsItem->pos(), pGraphObjLabel->text(), i_iPrecision);
                }
            }
            strlstLabelNames = i_pGraphObj->getGeometryLabelNames();
            for (const QString& strLabelName : strlstLabelNames) {
                CGraphObjLabel* pGraphObjLabel = i_pGraphObj->getGeometryLabel(strLabelName);
                if (pGraphObjLabel != nullptr) {
                    pGraphicsItem = dynamic_cast<const QGraphicsItem*>(pGraphObjLabel);
                    strlstResultValues += resultValuesForLabel(
                        i_pGraphObj->name() + "." + pGraphObjLabel->name(),
                        pGraphicsItem->pos(), pGraphObjLabel->text(), i_iPrecision);
                }
            }
        }
    }
    return strlstResultValues;
}

//------------------------------------------------------------------------------
QStringList CTest::resultValuesForLine(
    const QString& strGraphObjName, const QPointF& i_pos,
    const QLineF& i_lineItemCoors, const CPhysValLine& i_physValLine, int i_iPrecision) const
//------------------------------------------------------------------------------
{
    QRectF rctBounding(i_lineItemCoors.p1(), i_lineItemCoors.p2());
    QStringList strlst;
    if (i_iPrecision < 0) {
        strlst = QStringList({
            strGraphObjName + ".pos {" + qPoint2Str(i_pos) + "} px",
            strGraphObjName + ".boundingRect {" + qRect2Str(rctBounding) + "} px",
            strGraphObjName + ".line {" + qLine2Str(i_lineItemCoors) + "} px",
            strGraphObjName + ".position {" + i_physValLine.center().toString() + "} " + i_physValLine.unit().symbol(),
            strGraphObjName + ".getLine {" + i_physValLine.toString() + "} " + i_physValLine.unit().symbol(),
            strGraphObjName + ".getLength {" + i_physValLine.length().toString() + "}",
            strGraphObjName + ".rotationAngle: " + i_physValLine.angle().toString()
        });
    }
    else {
        strlst = QStringList({
            strGraphObjName + ".pos {" + qPoint2Str(i_pos, ", ", 'f', 1) + "} px",
            strGraphObjName + ".boundingRect {" + qRect2Str(rctBounding, ", ", 'f', 1) + "} px",
            strGraphObjName + ".line {" + qLine2Str(i_lineItemCoors, ", ", 'f', i_iPrecision) + "} px",
            strGraphObjName + ".position {" + i_physValLine.center().toString(false, ", ", 1) + "} " + i_physValLine.unit().symbol(),
            strGraphObjName + ".getLine {" + i_physValLine.toString(false, ", ", i_iPrecision) + "} " + i_physValLine.unit().symbol(),
            strGraphObjName + ".getLength {" + i_physValLine.length().toString() + "}",
            strGraphObjName + ".rotationAngle: " + i_physValLine.angle().toString()
        });
    }
    return strlst;
}

//------------------------------------------------------------------------------
QStringList CTest::resultValuesForRect(
    const QString& strGraphObjName, const QPointF& i_pos,
    const QRectF& i_rectItemCoors, const CPhysValRect& i_physValRect, int i_iPrecision) const
//------------------------------------------------------------------------------
{
    QStringList strlst;
    if (i_iPrecision < 0) {
        strlst = QStringList({
            strGraphObjName + ".pos {" + qPoint2Str(i_pos) + "} px",
            strGraphObjName + ".boundingRect {" + qRect2Str(i_rectItemCoors) + "} px",
            strGraphObjName + ".rect {" + qRect2Str(i_rectItemCoors) + "} px",
            strGraphObjName + ".position {" + i_physValRect.center().toString() + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getRect {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getSize {" + i_physValRect.size().toString() + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".rotationAngle: " + i_physValRect.angle().toString()
        });
    }
    else {
        strlst = QStringList({
            strGraphObjName + ".pos {" + qPoint2Str(i_pos, ", ", 'f', 1) + "} px",
            strGraphObjName + ".boundingRect {" + qRect2Str(i_rectItemCoors, ", ", 'f', 1) + "} px",
            strGraphObjName + ".rect {" + qRect2Str(i_rectItemCoors, ", ", 'f', i_iPrecision) + "} px",
            strGraphObjName + ".position {" + i_physValRect.center().toString(false, ", ", 1) + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getRect {" + i_physValRect.toString(false, ", ", i_iPrecision) + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getSize {" + i_physValRect.size().toString(false, ", ", 1) + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".rotationAngle: " + i_physValRect.angle().toString()
        });
    }
    return strlst;
}

//------------------------------------------------------------------------------
QStringList CTest::resultValuesForEllipse(
    const QString& strGraphObjName, const QPointF& i_pos,
    const QRectF& i_rectItemCoors, const CPhysValRect& i_physValRect, int i_iPrecision) const
//------------------------------------------------------------------------------
{
    QStringList strlst;
    if (i_iPrecision < 0) {
        strlst = QStringList({
            strGraphObjName + ".pos {" + qPoint2Str(i_pos) + "} px",
            strGraphObjName + ".boundingRect {" + qRect2Str(i_rectItemCoors) + "} px",
            strGraphObjName + ".rect {" + qRect2Str(i_rectItemCoors) + "} px",
            strGraphObjName + ".position {" + i_physValRect.center().toString() + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getRect {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getSize {" + i_physValRect.size().toString() + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".rotationAngle: " + i_physValRect.angle().toString()
        });
    }
    else {
        strlst = QStringList({
            strGraphObjName + ".pos {" + qPoint2Str(i_pos, ", ", 'f', 1) + "} px",
            strGraphObjName + ".boundingRect {" + qRect2Str(i_rectItemCoors, ", ", 'f', 1) + "} px",
            strGraphObjName + ".rect {" + qRect2Str(i_rectItemCoors, ", ", 'f', i_iPrecision) + "} px",
            strGraphObjName + ".position {" + i_physValRect.center().toString(false, ", ", 1) + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getRect {" + i_physValRect.toString(false, ", ", i_iPrecision) + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getSize {" + i_physValRect.size().toString(false, ", ", 1) + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".rotationAngle: " + i_physValRect.angle().toString()
        });
    }
    return strlst;
}

//------------------------------------------------------------------------------
QStringList CTest::resultValuesForText(
    const QString& strGraphObjName, const QPointF& i_pos,
    const CPhysValRect& i_physValRect, const QString& i_strText, int i_iPrecision) const
//------------------------------------------------------------------------------
{
    QSizeF size = m_pDrawingScene->convert(i_physValRect.size(), Units.Length.px).toQSizeF();
    QRectF rctBounding(QPointF(-size.width()/2.0, -size.height()/2.0), size);
    QStringList strlst;
    if (i_iPrecision < 0) {
        strlst = QStringList({
            strGraphObjName + ".pos {" + qPoint2Str(i_pos) + "} px",
            strGraphObjName + ".boundingRect {" + qRect2Str(rctBounding) + "} px",
            strGraphObjName + ".position {" + i_physValRect.center().toString() + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getRect {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getSize {" + i_physValRect.size().toString() + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".rotationAngle: " + i_physValRect.angle().toString(),
            strGraphObjName + ".text: " + i_strText
        });
    }
    else {
        strlst = QStringList({
            strGraphObjName + ".pos {" + qPoint2Str(i_pos, ", ", 'f', 1) + "} px",
            strGraphObjName + ".boundingRect {" + qRect2Str(rctBounding, ", ", 'f', 1) + "} px",
            strGraphObjName + ".position {" + i_physValRect.center().toString(false, ", ", 1) + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getRect {" + i_physValRect.toString(false, ", ", i_iPrecision) + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getSize {" + i_physValRect.size().toString(false, ", ", 1) + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".rotationAngle: " + i_physValRect.angle().toString(),
            strGraphObjName + ".text: " + i_strText
        });
    }
    return strlst;
}

//------------------------------------------------------------------------------
QStringList CTest::resultValuesForPolygon(
    const QString& strGraphObjName, const QPointF& i_pos,
    const QPolygonF& i_polygonItemCoors, const CPhysValPolygon& i_physValPolygon, int i_iPrecision) const
//------------------------------------------------------------------------------
{
    QRectF rctBounding(i_polygonItemCoors.boundingRect());
    QStringList strlst;
    if (i_iPrecision < 0) {
        strlst = QStringList({
            strGraphObjName + ".pos {" + qPoint2Str(i_pos) + "} px",
            strGraphObjName + ".boundingRect {" + qRect2Str(rctBounding) + "} px",
            strGraphObjName + ".polygon {" + qPolygon2Str(i_polygonItemCoors) + "} px",
            strGraphObjName + ".position {" + i_physValPolygon.center().toString() + "} " + i_physValPolygon.unit().symbol(),
            strGraphObjName + ".getPolygon {" + i_physValPolygon.toString() + "} " + i_physValPolygon.unit().symbol(),
            strGraphObjName + ".rotationAngle: " + i_physValPolygon.angle().toString()
        });
    }
    else {
        strlst = QStringList({
            strGraphObjName + ".pos {" + qPoint2Str(i_pos, ", ", 'f', 1) + "} px",
            strGraphObjName + ".boundingRect {" + qRect2Str(rctBounding, ", ", 'f', 1) + "} px",
            strGraphObjName + ".polygon {" + qPolygon2Str(i_polygonItemCoors, ", ", 'f', i_iPrecision) + "} px",
            strGraphObjName + ".position {" + i_physValPolygon.center().toString(false, ", ", 1) + "} " + i_physValPolygon.unit().symbol(),
            strGraphObjName + ".getPolygon {" + i_physValPolygon.toString(false, ", ", i_iPrecision) + "} " + i_physValPolygon.unit().symbol(),
            strGraphObjName + ".rotationAngle: " + i_physValPolygon.angle().toString()
        });
    }
    return strlst;
}

//------------------------------------------------------------------------------
QStringList CTest::resultValuesForConnectionPoint(
    const QString& strGraphObjName, const QPointF& i_pos,
    const ZS::Draw::CPhysValPoint& i_physValPoint, int i_iPrecision) const
//------------------------------------------------------------------------------
{
    QStringList strlst;
    if (i_iPrecision < 0) {
        strlst = QStringList({
            strGraphObjName + ".pos {" + qPoint2Str(i_pos) + "} px",
            strGraphObjName + ".position {" + i_physValPoint.toString() + "} " + i_physValPoint.unit().symbol(),
        });
    }
    else {
        strlst = QStringList({
            strGraphObjName + ".pos {" + qPoint2Str(i_pos, ", ", 'f', 1) + "} px",
            strGraphObjName + ".position {" + i_physValPoint.toString(false, ", ", 1) + "} " + i_physValPoint.unit().symbol(),
        });
    }
    return strlst;
}

//------------------------------------------------------------------------------
QStringList CTest::resultValuesForConnectionLine(
    const QString& strGraphObjName, const QString& i_strGraphObjNameCntPt1, const QString& i_strGraphObjNameCntPt2,
    const QPolygonF& i_polygonItemCoors, const ZS::Draw::CPhysValPolygon& i_physValPolygon, int i_iPrecision) const
//------------------------------------------------------------------------------
{
    QRectF rctBounding(i_polygonItemCoors.boundingRect());
    QStringList strlst;
    if (i_iPrecision < 0) {
        strlst = QStringList({
            strGraphObjName + ".P1: " + i_strGraphObjNameCntPt1,
            strGraphObjName + ".P2: " + i_strGraphObjNameCntPt2,
            strGraphObjName + ".polygon {" + qPolygon2Str(i_polygonItemCoors) + "} px",
            strGraphObjName + ".getPolygon {" + i_physValPolygon.toString() + "} " + i_physValPolygon.unit().symbol(),
        });
    }
    else {
        strlst = QStringList({
            strGraphObjName + ".P1: " + i_strGraphObjNameCntPt1,
            strGraphObjName + ".P2: " + i_strGraphObjNameCntPt2,
            strGraphObjName + ".polygon {" + qPolygon2Str(i_polygonItemCoors, ", ", 'f', i_iPrecision) + "} px",
            strGraphObjName + ".getPolygon {" + i_physValPolygon.toString(false, ", ", i_iPrecision) + "} " + i_physValPolygon.unit().symbol(),
        });
    }
    return strlst;
}

//------------------------------------------------------------------------------
QStringList CTest::resultValuesForLabel(
    const QString& strGraphObjName, const QPointF& i_pos, const QString& i_strText, int i_iPrecision) const
//------------------------------------------------------------------------------
{
    QStringList strlstResultValues;
    if (!i_pos.isNull()) {
        if (i_iPrecision < 0) {
            strlstResultValues = QStringList({
                strGraphObjName + ".pos {" + qPoint2Str(i_pos) + "} px"});
        }
        else {
            strlstResultValues = QStringList({
                strGraphObjName + ".pos {" + qPoint2Str(i_pos, ", ", 'f', i_iPrecision) + "} px"});
        }
    }
    strlstResultValues.append(strGraphObjName + ".text: " + i_strText);
    return strlstResultValues;
}

//------------------------------------------------------------------------------
QStringList CTest::resultValuesForGroup(
    const QString& strGraphObjName, const QPointF& i_pos,
    const CPhysValRect& i_physValRect, int i_iPrecision) const
//------------------------------------------------------------------------------
{
    QSizeF size = m_pDrawingScene->convert(i_physValRect.size(), Units.Length.px).toQSizeF();
    QRectF rctBounding(QPointF(-size.width()/2.0, -size.height()/2.0), size);
    QStringList strlst;
    if (i_iPrecision < 0) {
        strlst = QStringList({
            strGraphObjName + ".pos {" + qPoint2Str(i_pos) + "} px",
            strGraphObjName + ".boundingRect {" + qRect2Str(rctBounding) + "} px",
            strGraphObjName + ".position {" + i_physValRect.center().toString() + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getRect {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getSize {" + i_physValRect.size().toString() + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".rotationAngle: " + i_physValRect.angle().toString()
        });
    }
    else {
        strlst = QStringList({
            strGraphObjName + ".pos {" + qPoint2Str(i_pos, ", ", 'f', 1) + "} px",
            strGraphObjName + ".boundingRect {" + qRect2Str(rctBounding, ", ", 'f', 1) + "} px",
            strGraphObjName + ".position {" + i_physValRect.center().toString(false, ", ", 1) + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getRect {" + i_physValRect.toString(false, ", ", i_iPrecision) + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".getSize {" + i_physValRect.size().toString(false, ", ", 1) + "} " + i_physValRect.unit().symbol(),
            strGraphObjName + ".rotationAngle: " + i_physValRect.angle().toString()
        });
    }
    return strlst;
}

//------------------------------------------------------------------------------
void CTest::triggerDoTestStep(int i_iInterval_ms)
//------------------------------------------------------------------------------
{
    ZS::Test::CTest::triggerDoTestStep(i_iInterval_ms);
}
