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
may result in using the software modules.

*******************************************************************************/

#include "Test/Test.h"

#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Products::Draw;


/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawObjects(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawObjects",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpDrawStandardShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw " + CObjFactory::c_strGroupNameStandardShapes,
        /* pTSGrpParent */ i_pTestStepGroupParent );

#if TEST_DRAW_OBJECTS_STANDARDSHAPES == 1
    createTestGroupDrawStandardShapes(pGrpDrawStandardShapes);
#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapes(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapes",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);

    ZS::Test::CTestStep* pTestStep = nullptr;

    // Lines
    //======

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_LINES == 1

    ZS::Test::CTestStepGroup* pGrpLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Lines",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpLines,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    // SmallPlusSign
    //--------------

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_LINES_SMALLPLUSSIGN == 1
    ZS::Test::CTestStepGroup* pGrpLinesSmallPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameSmallPlusSign,
        /* pTSGrpParent */ pGrpLines );
    createTestGroupDrawStandardShapesLinesSmallPlusSign(pGrpLinesSmallPlusSign);
#endif

    // BigPlusSign
    //------------

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_LINES_BIGPLUSSIGN == 1
    ZS::Test::CTestStepGroup* pGrpLinesBigPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameBigPlusSign,
        /* pTSGrpParent */ pGrpLines );
    createTestGroupDrawStandardShapesLinesBigPlusSign(pGrpLinesBigPlusSign);
#endif

    // Checkmark
    //----------

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_LINES_CHECKMARK == 1
    ZS::Test::CTestStepGroup* pGrpLinesCheckmark = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameCheckmark,
        /* pTSGrpParent */ pGrpLines );
    createTestGroupDrawStandardShapesLinesCheckmark(pGrpLinesCheckmark);
#endif

    // SmallRect
    //----------

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_LINES_SMALLRECT == 1
    ZS::Test::CTestStepGroup* pGrpLinesSmallRect = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameSmallRect,
        /* pTSGrpParent */ pGrpLines );
    createTestGroupDrawStandardShapesLinesSmallRect(pGrpLinesSmallRect);
#endif

    createTestStepSaveLoadFile(pGrpLines);
#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_LINES

    // Rectangles
    //===========

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_RECTANGLES == 1

    ZS::Test::CTestStepGroup* pGrpRectangles = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Rectangles",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpRectangles,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    // Rectangle
    //----------

    ZS::Test::CTestStepGroup* pGrpRectanglesRectangle = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameRect1,
        /* pTSGrpParent */ pGrpRectangles );
    createTestGroupDrawStandardShapesRect(pGrpRectanglesRectangle);
    createTestGroupDrawStandardShapesRectModifications(pGrpRectanglesRectangle);

    if (bYAxisTopDown) {
        createTestStepSaveLoadFile(pGrpRectangles, 2);
    }

#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_RECTANGLES

    // Ellipses
    //===========

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_ELLIPSES == 1

    ZS::Test::CTestStepGroup* pGrpEllipses = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Ellipses",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpEllipses,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    // Ellipse
    //----------

    ZS::Test::CTestStepGroup* pGrpEllipsesEllipse = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameEllipse1,
        /* pTSGrpParent */ pGrpEllipses );
    createTestGroupDrawStandardShapesEllipse(pGrpEllipsesEllipse);
    createTestGroupDrawStandardShapesEllipseModifications(pGrpEllipsesEllipse);

    if (bYAxisTopDown) {
        createTestStepSaveLoadFile(pGrpEllipses, 1);
    }

#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_ELLIPSES

    // Texts
    //===========

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_TEXTS == 1

    ZS::Test::CTestStepGroup* pGrpTexts = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Texts",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpTexts,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    // Text
    //----------

    ZS::Test::CTestStepGroup* pGrpTextsText = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameText1,
        /* pTSGrpParent */ pGrpTexts );
    createTestGroupDrawStandardShapesText(pGrpTextsText);
    createTestGroupDrawStandardShapesTextModifications(pGrpTextsText);

    createTestStepSaveLoadFile(pGrpTexts, 1);

#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_TEXTS

    // Polygons
    //=========

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_POLYGONS == 1

    ZS::Test::CTestStepGroup* pGrpPolygons = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Polygons",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpPolygons,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    // Triangle
    //---------

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_POLYGONS_TRIANGLE == 1
    ZS::Test::CTestStepGroup* pGrpPolygonsTriangle = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameTriangle,
        /* pTSGrpParent */ pGrpPolygons );
    createTestGroupDrawStandardShapesPolygonTriangle(pGrpPolygonsTriangle);
    createTestGroupDrawStandardShapesPolygonTriangleModifications(pGrpPolygonsTriangle);
#endif

    // Star
    //-----

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_POLYGONS_STAR == 1
    ZS::Test::CTestStepGroup* pGrpPolygonsStar = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameStar1,
        /* pTSGrpParent */ pGrpPolygons );
    createTestGroupDrawStandardShapesPolygonStar(pGrpPolygonsStar);
#endif

    // OpenArrow
    //----------

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_POLYGONS_OPEN_ARROW == 1
    ZS::Test::CTestStepGroup* pGrpPolygonsOpenArrow = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameOpenArrow,
        /* pTSGrpParent */ pGrpPolygons );
    createTestGroupDrawStandardShapesPolygonOpenArrow(pGrpPolygonsOpenArrow);
#endif

    createTestStepSaveLoadFile(pGrpPolygons);

#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_POLYGONS

    // Connections
    //============

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS == 1

    ZS::Test::CTestStepGroup* pGrpConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_NOT_ANCHORED == 1
    ZS::Test::CTestStepGroup* pGrpConnectionPointsNotAnchored = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " NotAnchored ConnectionPoints",
        /* pTSGrpParent */ pGrpConnections );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpConnectionPointsNotAnchored,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    createTestGroupDrawStandardShapesConnectionPointsNotAnchored(pGrpConnectionPointsNotAnchored);
    createTestGroupDrawStandardShapesConnectionPointsNotAnchoredModifications(pGrpConnectionPointsNotAnchored);

    createTestStepSaveLoadFile(pGrpConnectionPointsNotAnchored, 1);
#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_NOT_ANCHORED

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED == 1
    ZS::Test::CTestStepGroup* pGrpConnectionPointsAnchored = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Anchored ConnectionPoints",
        /* pTSGrpParent */ pGrpConnections );

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED_TOLINES == 1
    ZS::Test::CTestStepGroup* pGrpConnectionPointsAnchoredToLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ConnectionPoints Anchored To Lines",
        /* pTSGrpParent */ pGrpConnectionPointsAnchored );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpConnectionPointsAnchoredToLines,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    createTestGroupDrawStandardShapesConnectionPointsAnchoredToLines(pGrpConnectionPointsAnchoredToLines);
    createTestGroupDrawStandardShapesConnectionPointsAnchoredToLinesModifications(pGrpConnectionPointsAnchoredToLines);

    createTestStepSaveLoadFile(pGrpConnectionPointsAnchoredToLines, 1);
#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED_TOLINES

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED_TORECTS == 1
    ZS::Test::CTestStepGroup* pGrpConnectionPointsAnchoredToRects = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ConnectionPoints Anchored To Rectangles",
        /* pTSGrpParent */ pGrpConnectionPointsAnchored );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpConnectionPointsAnchoredToRects,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    createTestGroupDrawStandardShapesConnectionPointsAnchoredToRects(pGrpConnectionPointsAnchoredToRects);
    createTestGroupDrawStandardShapesConnectionPointsAnchoredToRectsModifications(pGrpConnectionPointsAnchoredToRects);

    createTestStepSaveLoadFile(pGrpConnectionPointsAnchoredToRects, 1);
#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED_TORECTS

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED_TOELLIPSES == 1
    ZS::Test::CTestStepGroup* pGrpConnectionPointsAnchoredToEllipses = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ConnectionPoints Anchored To Ellipses",
        /* pTSGrpParent */ pGrpConnectionPointsAnchored );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpConnectionPointsAnchoredToEllipses,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    createTestGroupDrawStandardShapesConnectionPointsAnchoredToEllipses(pGrpConnectionPointsAnchoredToEllipses);
    createTestGroupDrawStandardShapesConnectionPointsAnchoredToEllipsesModifications(pGrpConnectionPointsAnchoredToEllipses);

    createTestStepSaveLoadFile(pGrpConnectionPointsAnchoredToEllipses, 1);
#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED_TOELLIPSES

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED_TOPOLYGONS == 1
    ZS::Test::CTestStepGroup* pGrpConnectionPointsAnchoredToPolygons = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ConnectionPoints Anchored To Polygons",
        /* pTSGrpParent */ pGrpConnectionPointsAnchored );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpConnectionPointsAnchoredToPolygons,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    createTestGroupDrawStandardShapesConnectionPointsAnchoredToPolygons(pGrpConnectionPointsAnchoredToPolygons);
    createTestGroupDrawStandardShapesConnectionPointsAnchoredToPolygonsModifications(pGrpConnectionPointsAnchoredToPolygons);

    createTestStepSaveLoadFile(pGrpConnectionPointsAnchoredToPolygons, 1);
#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED_TOPOLYGONS

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED_TOTEXTS == 1
    ZS::Test::CTestStepGroup* pGrpConnectionPointsAnchoredToTexts = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ConnectionPoints Anchored To Texts",
        /* pTSGrpParent */ pGrpConnectionPointsAnchored );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpConnectionPointsAnchoredToTexts,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    createTestGroupDrawStandardShapesConnectionPointsAnchoredToTexts(pGrpConnectionPointsAnchoredToTexts);
    createTestGroupDrawStandardShapesConnectionPointsAnchoredToTextsModifications(pGrpConnectionPointsAnchoredToTexts);

    createTestStepSaveLoadFile(pGrpConnectionPointsAnchoredToTexts, 1);
#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED_TOTEXTS

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED_TOBITMAPS == 1
    ZS::Test::CTestStepGroup* pGrpConnectionPointsAnchoredToBitmaps = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ConnectionPoints Anchored To Bitmaps",
        /* pTSGrpParent */ pGrpConnectionPointsAnchored );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpConnectionPointsAnchoredToBitmaps,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    createTestGroupDrawStandardShapesConnectionPointsAnchoredToBitmaps(pGrpConnectionPointsAnchoredToBitmaps);
    createTestGroupDrawStandardShapesConnectionPointsAnchoredToBitmapsModifications(pGrpConnectionPointsAnchoredToBitmaps);

    createTestStepSaveLoadFile(pGrpConnectionPointsAnchoredToBitmaps, 1);
#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED_TOBITMAPS

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED_TOGROUPS == 1
    ZS::Test::CTestStepGroup* pGrpConnectionPointsAnchoredToGroups = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ConnectionPoints Anchored To Groups",
        /* pTSGrpParent */ pGrpConnectionPointsAnchored );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpConnectionPointsAnchoredToGroups,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    createTestGroupDrawStandardShapesConnectionPointsAnchoredToGroups(pGrpConnectionPointsAnchoredToGroups);
    createTestGroupDrawStandardShapesConnectionPointsAnchoredToGroupsModifications(pGrpConnectionPointsAnchoredToGroups);

    createTestStepSaveLoadFile(pGrpConnectionPointsAnchoredToGroups, 1);
#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED_TOGROUPS
#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS_ANCHORED
#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONPOINTS

    // Groups
    //=======

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS == 1

    ZS::Test::CTestStepGroup* pGrpGroups = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Groups",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // SmallPlusSign
    //--------------

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS_LINES_SMALLPLUSSIGN == 1

    ZS::Test::CTestStepGroup* pGrpGroupsSmallPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " "  + c_strGraphObjNameSmallPlusSign,
        /* pTSGrpParent */ pGrpGroups );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpGroupsSmallPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    ZS::Test::CTestStepGroup* pGrpGroupsSmallPlusSignDrawShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Shapes",
        /* pTSGrpParent */ pGrpGroupsSmallPlusSign );

    createTestGroupDrawStandardShapesGroupLinesSmallPlusSign(pGrpGroupsSmallPlusSignDrawShapes);
    createTestGroupDrawStandardShapesGroupLinesSmallPlusSignModifications(pGrpGroupsSmallPlusSign);
    createTestStepSaveLoadFile(pGrpGroupsSmallPlusSign);

#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS_LINES_SMALLPLUSSIGN

    // BigPlusSign
    //------------

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS_LINES_BIGPLUSSIGN == 1

    ZS::Test::CTestStepGroup* pGrpGroupsBigPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " "  + c_strGraphObjNameBigPlusSign,
        /* pTSGrpParent */ pGrpGroups );

    ZS::Test::CTestStepGroup* pGrpGroupsBigPlusSignRotateTop = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Rotate "  + c_strGraphObjNameTopGroup,
        /* pTSGrpParent */ pGrpGroupsBigPlusSign );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpGroupsBigPlusSignRotateTop,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    ZS::Test::CTestStepGroup* pGrpGroupsBigPlusSignRotateTopDrawShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Shapes",
        /* pTSGrpParent */ pGrpGroupsBigPlusSignRotateTop );

    createTestGroupDrawStandardShapesGroupLinesBigPlusSign(pGrpGroupsBigPlusSignRotateTopDrawShapes);
    createTestGroupDrawStandardShapesGroupLinesBigPlusSignRotateParent(pGrpGroupsBigPlusSignRotateTop);
    createTestStepSaveLoadFile(pGrpGroupsBigPlusSignRotateTop, 1);

    ZS::Test::CTestStepGroup* pGrpGroupsBigPlusSignRotateAndResize = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " RotateResize "  + c_strGraphObjNameBigPlusSign,
        /* pTSGrpParent */ pGrpGroupsBigPlusSign );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpGroupsBigPlusSignRotateAndResize,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    ZS::Test::CTestStepGroup* pGrpGroupsBigPlusSignRotateAndResizeDrawShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Shapes",
        /* pTSGrpParent */ pGrpGroupsBigPlusSignRotateAndResize );

    createTestGroupDrawStandardShapesGroupLinesBigPlusSign(pGrpGroupsBigPlusSignRotateAndResizeDrawShapes);
    createTestGroupDrawStandardShapesGroupLinesBigPlusSignModfications(pGrpGroupsBigPlusSignRotateAndResize);
    createTestStepSaveLoadFile(pGrpGroupsBigPlusSignRotateAndResize);

#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS_LINES_BIGPLUSSIGN
#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesLinesSmallPlusSign(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesLinesSmallPlusSign",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;
    iResultValuesPrecision = -1;

    // Vertical Line
    //--------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameSmallPlusSignVerticalLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallPlusSignVerticalLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallPlusSignVerticalLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSignVerticalLine));
    QPoint ptP1SmallPlusSignVerticalLine(275, 310);
    QPoint ptP2SmallPlusSignVerticalLine(275, 340);
    m_ptPosSmallPlusSignVerticalLine = QPointF(275.0, 325.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -15.0), QPointF(0.0, 15.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP1(QPointF(275.0, bYAxisTopDown ? 310.0 : 290.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP2(QPointF(275.0, bYAxisTopDown ? 340.0 : 260.0));
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeLine));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSignVerticalLine);
    pTestStep->setConfigValue("P1", ptP1SmallPlusSignVerticalLine);
    pTestStep->setConfigValue("P2", ptP2SmallPlusSignVerticalLine);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Horizontal Line
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameSmallPlusSignHorizontalLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallPlusSignHorizontalLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallPlusSignHorizontalLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSignHorizontalLine));
    QPoint ptP1SmallPlusSignHorizontalLine(260, 325);
    QPoint ptP2SmallPlusSignHorizontalLine(290, 325);
    m_ptPosSmallPlusSignHorizontalLine = QPointF(275.0, 325.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP1(QPointF(260.0, bYAxisTopDown ? 325.0 : 275.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP2(QPointF(290.0, bYAxisTopDown ? 325.0 : 275.0));
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeLine));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("P1", ptP1SmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("P2", ptP2SmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesLinesBigPlusSign(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesLinesBigPlusSign",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;
    iResultValuesPrecision = -1;

    // Vertical Line
    //--------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameBigPlusSignVerticalLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameBigPlusSignVerticalLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameBigPlusSignVerticalLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSignVerticalLine));
    QPoint ptP1BigPlusSignVerticalLine(300, 250);
    QPoint ptP2BigPlusSignVerticalLine(300, 350);
    m_ptPosBigPlusSignVerticalLine = QPointF(300.0, 300.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(300.0, bYAxisTopDown ? 250.0 : 350.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(300.0, bYAxisTopDown ? 350.0 : 250.0));
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeLine));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSignVerticalLine);
    pTestStep->setConfigValue("P1", ptP1BigPlusSignVerticalLine);
    pTestStep->setConfigValue("P2", ptP2BigPlusSignVerticalLine);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Horizontal Line
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameBigPlusSignHorizontalLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameBigPlusSignHorizontalLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameBigPlusSignHorizontalLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSignHorizontalLine));
    QPoint ptP1BigPlusSignHorizontalLine(250, 300);
    QPoint ptP2BigPlusSignHorizontalLine(350, 300);
    m_ptPosBigPlusSignHorizontalLine = QPointF(300.0, 300.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(250.0, bYAxisTopDown ? 300.0 : 300.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(350.0, bYAxisTopDown ? 300.0 : 300.0));
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeLine));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("P1", ptP1BigPlusSignHorizontalLine);
    pTestStep->setConfigValue("P2", ptP2BigPlusSignHorizontalLine);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesLinesCheckmark(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesLinesCheckmark",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;
    iResultValuesPrecision = -1;

    // Left Line
    //----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameCheckmarkLeftLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameCheckmarkLeftLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameCheckmarkLeftLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameCheckmarkLeftLine));
    QPoint ptP1CheckmarkLeftLine(250, 250);
    QPoint ptP2CheckmarkLeftLine(325, 350);
    m_ptPosCheckmarkLeftLine = QPointF(287.5, 300.0);
    m_lineCheckmarkLeftLine = QLineF(QPointF(-37.5, -50.0), QPointF(37.5, 50.0));
    m_pPhysValLineCheckmarkLeftLine->setP1(QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0));
    m_pPhysValLineCheckmarkLeftLine->setP2(QPointF(325.0, bYAxisTopDown ? 350.0 : 250.0));
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeLine));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmarkLeftLine);
    pTestStep->setConfigValue("P1", ptP1CheckmarkLeftLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkLeftLine);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkLeftLine, m_ptPosCheckmarkLeftLine,
        m_lineCheckmarkLeftLine, *m_pPhysValLineCheckmarkLeftLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Right Line
    //-----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameCheckmarkRightLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameCheckmarkRightLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameCheckmarkRightLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameCheckmarkRightLine));
    QPoint ptP1CheckmarkRightLine(325, 350);
    QPoint ptP2CheckmarkRightLine(350, 325);
    m_ptPosCheckmarkRightLine = QPointF(337.5, 337.5);
    m_lineCheckmarkRightLine = QLineF(QPointF(-12.5, 12.5), QPointF(12.5, -12.5));
    m_pPhysValLineCheckmarkRightLine->setP1(QPointF(325.0, bYAxisTopDown ? 350.0 : 250.0));
    m_pPhysValLineCheckmarkRightLine->setP2(QPointF(350.0, bYAxisTopDown ? 325.0 : 275.0));
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeLine));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmarkRightLine);
    pTestStep->setConfigValue("P1", ptP1CheckmarkRightLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkRightLine);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkRightLine, m_ptPosCheckmarkRightLine,
        m_lineCheckmarkRightLine, *m_pPhysValLineCheckmarkRightLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesLinesSmallRect(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesLinesSmallRect",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;
    iResultValuesPrecision = -1;

    // Top Line
    //---------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameSmallRectTopLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectTopLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallRectTopLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallRectTopLine));
    QPoint ptP1SmallRectTopLine(320, 270);
    QPoint ptP2SmallRectTopLine(330, 270);
    m_ptPosSmallRectTopLine = QPointF(325.0, 270.0);
    m_lineSmallRectTopLine = QLineF(QPointF(-5.0, 0.0), QPointF(5.0, 0.0));
    m_pPhysValLineSmallRectTopLine->setP1(QPointF(320.0, bYAxisTopDown ? 270.0 : 330.0));
    m_pPhysValLineSmallRectTopLine->setP2(QPointF(330.0, bYAxisTopDown ? 270.0 : 330.0));
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeLine));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallRectTopLine);
    pTestStep->setConfigValue("P1", ptP1SmallRectTopLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectTopLine);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectTopLine, m_ptPosSmallRectTopLine,
        m_lineSmallRectTopLine, *m_pPhysValLineSmallRectTopLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Right Line
    //-----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameSmallRectRightLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectRightLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallRectRightLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallRectRightLine));
    QPoint ptP1SmallRectRightLine(330, 270);
    QPoint ptP2SmallRectRightLine(330, 280);
    m_ptPosSmallRectRightLine = QPointF(330.0, 275.0);
    m_lineSmallRectRightLine = QLineF(QPointF(0.0, -5.0), QPointF(0.0, 5.0));
    m_pPhysValLineSmallRectRightLine->setP1(QPointF(330.0, bYAxisTopDown ? 270.0 : 330.0));
    m_pPhysValLineSmallRectRightLine->setP2(QPointF(330.0, bYAxisTopDown ? 280.0 : 320.0));
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeLine));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallRectRightLine);
    pTestStep->setConfigValue("P1", ptP1SmallRectRightLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectRightLine);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectRightLine, m_ptPosSmallRectRightLine,
        m_lineSmallRectRightLine, *m_pPhysValLineSmallRectRightLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Bottom Line
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameSmallRectBottomLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectBottomLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallRectBottomLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallRectBottomLine));
    QPoint ptP1SmallRectBottomLine(330, 280);
    QPoint ptP2SmallRectBottomLine(320, 280);
    m_ptPosSmallRectBottomLine = QPointF(325.0, 280.0);
    m_lineSmallRectBottomLine = QLineF(QPointF(5.0, 0.0), QPointF(-5.0, 0.0));
    m_pPhysValLineSmallRectBottomLine->setP1(QPointF(330.0, bYAxisTopDown ? 280.0 : 320.0));
    m_pPhysValLineSmallRectBottomLine->setP2(QPointF(320.0, bYAxisTopDown ? 280.0 : 320.0));
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeLine));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallRectBottomLine);
    pTestStep->setConfigValue("P1", ptP1SmallRectBottomLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectBottomLine);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectBottomLine, m_ptPosSmallRectBottomLine,
        m_lineSmallRectBottomLine, *m_pPhysValLineSmallRectBottomLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Left Line
    //----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameSmallRectLeftLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectLeftLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallRectLeftLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallRectLeftLine));
    QPoint ptP1SmallRectLeftLine(320, 280);
    QPoint ptP2SmallRectLeftLine(320, 270);
    m_ptPosSmallRectLeftLine = QPointF(320.0, 275.0);
    m_lineSmallRectLeftLine = QLineF(QPointF(0.0, 5.0), QPointF(0.0, -5.0));
    m_pPhysValLineSmallRectLeftLine->setP1(QPointF(320.0, bYAxisTopDown ? 280.0 : 320.0));
    m_pPhysValLineSmallRectLeftLine->setP2(QPointF(320.0, bYAxisTopDown ? 270.0 : 330.0));
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeLine));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallRectLeftLine);
    pTestStep->setConfigValue("P1", ptP1SmallRectLeftLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectLeftLine);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectLeftLine, m_ptPosSmallRectLeftLine,
        m_lineSmallRectLeftLine, *m_pPhysValLineSmallRectLeftLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesRect(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesRect",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;
    iResultValuesPrecision = -1;

    // Rectangle
    //----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameRect1 + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameRect1 + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameRect1, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameRect1);
    pTestStep->setConfigValue("P1", QPoint(275, 275));
    pTestStep->setConfigValue("P2", QPoint(325, 325));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    QSizeF sizeRect = QSizeF(50.0, 50.0);
    m_ptPosRect1 = QPointF(300, 300.0);
    m_rectRect1 = QRectF(QPointF(-25.0, -25.0), sizeRect);
    m_pPhysValRect1->setSize(sizeRect);
    m_pPhysValRect1->setCenter(QPointF(300, bYAxisTopDown ? 300.0 : fYAxisMaxVal - 300.0));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        c_strGraphObjNameRect1, m_ptPosRect1, m_rectRect1, *m_pPhysValRect1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesRectModifications(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesRectModifications",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;
    QString strObjName = c_strGraphObjNameRect1;
    QString strMethod;
    QString strMthArgs;
    CPhysValRect physValRectNew = *m_pPhysValRect1;
    CEnumSelectionPoint eSelPt;
    QPointF pt1SelPt;
    QPointF pt2SelPt;

    ZS::Test::CTestStepGroup* pGrpModifyRectangle = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify " + strObjName,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // setRotationAngle
    //-----------------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameRect1;
    eSelPt = bYAxisTopDown ? ESelectionPoint::RotateTop : ESelectionPoint::RotateBottom;
    physValRectNew = *m_pPhysValRect1;
    physValRectNew.setAngle(45.0);
    strMethod = "setRotationAngle";
    strMthArgs = physValRectNew.angle().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyRectangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRect1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosRect1);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_pPhysValAngleRect1 = physValRectNew.angle();
    *m_pPhysValRect1 = physValRectNew;
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosRect1, m_rectRect1, *m_pPhysValRect1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setWidth
    //---------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameRect1;
    QSizeF sizeRectangle = m_pPhysValRect1->size().toQSizeF();
    sizeRectangle.setWidth(100.0);
    eSelPt = ESelectionPoint::RightCenter;
    physValRectNew = *m_pPhysValRect1;
    physValRectNew.setWidth(sizeRectangle.width());
    strMethod = "setWidth";
    strMthArgs = physValRectNew.width().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyRectangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRect1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosRect1);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosRect1 = QPointF(317.661165, 317.661165);
    m_rectRect1 = QRectF(QPointF(-49.976659, -25.0), QSizeF(99.953319, 50.0));
    m_pPhysValRect1->setSize(QSizeF(99.953319, 50.0));
    m_pPhysValRect1->setCenter(QPointF(317.661165, bYAxisTopDown ? 317.661165 : 282.338835));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosRect1, m_rectRect1, *m_pPhysValRect1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setHeight
    //----------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameRect1;
    sizeRectangle = m_pPhysValRect1->size().toQSizeF();
    sizeRectangle.setHeight(100.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectNew = *m_pPhysValRect1;
    physValRectNew.setHeight(sizeRectangle.height());
    strMethod = "setHeight";
    strMthArgs = physValRectNew.height().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyRectangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRect1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosRect1);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosRect1 = QPointF(bYAxisTopDown ? 300.0 : 335.322330, bYAxisTopDown ? 335.322330 : 300.0);
    m_rectRect1 = QRectF(QPointF(-49.976659, -49.976659), QSizeF(99.953319, 99.953319));
    m_pPhysValRect1->setSize(QSizeF(99.953319, 99.953319));
    m_pPhysValRect1->setCenter(QPointF(bYAxisTopDown ? 300.0 : 335.322330, bYAxisTopDown ? 335.322330 : 300.0));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosRect1, m_rectRect1, *m_pPhysValRect1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setSize
    //--------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameRect1;
    sizeRectangle = QSizeF(150.0, 150.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomRight : ESelectionPoint::TopRight;
    physValRectNew = *m_pPhysValRect1;
    physValRectNew.setSize(sizeRectangle);
    strMethod = "setSize";
    strMthArgs = physValRectNew.size().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyRectangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRect1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosRect1);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosRect1 = QPointF(bYAxisTopDown ? 300.0 : 370.822330, bYAxisTopDown ? 370.822330 : 300.0);
    m_rectRect1 = QRectF(QPointF(-75.078950, -75.078950), QSizeF(150.157900, 150.157900));
    m_pPhysValRect1->setSize(QSizeF(150.157900, 150.157900));
    m_pPhysValRect1->setCenter(QPointF(bYAxisTopDown ? 300.000000 : 370.822330, bYAxisTopDown ? 370.822330 : 300.000000));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosRect1, m_rectRect1, *m_pPhysValRect1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setWidth
    //---------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameRect1;
    sizeRectangle.setWidth(50.0);
    eSelPt = ESelectionPoint::RightCenter;
    physValRectNew = *m_pPhysValRect1;
    physValRectNew.setWidth(sizeRectangle.width());
    strMethod = "setWidth";
    strMthArgs = physValRectNew.width().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyRectangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRect1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosRect1);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosRect1 = QPointF(bYAxisTopDown ? 264.5 : 335.322330, bYAxisTopDown ? 335.322330 : 264.5);
    m_rectRect1 = QRectF(QPointF(-24.874369, -75.078950), QSizeF(49.748737, 150.157900));
    m_pPhysValRect1->setSize(QSizeF(49.748737, 150.157900));
    m_pPhysValRect1->setCenter(QPointF(bYAxisTopDown ? 264.5 : 335.322330, bYAxisTopDown ? 335.322330 : 335.5));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosRect1, m_rectRect1, *m_pPhysValRect1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setHeight
    //----------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameRect1;
    sizeRectangle.setHeight(50.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectNew = *m_pPhysValRect1;
    physValRectNew.setHeight(sizeRectangle.height());
    strMethod = "setHeight";
    strMthArgs = physValRectNew.height().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyRectangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRect1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosRect1);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosRect1 = QPointF(bYAxisTopDown ? 299.750000 : 300.072330, bYAxisTopDown ? 300.072330 : 299.749000);
    m_rectRect1 = QRectF(QPointF(-24.874369, -25.227922), QSizeF(49.748737, 50.455844));
    m_pPhysValRect1->setSize(QSizeF(49.748737, 50.455844));
    m_pPhysValRect1->setCenter(QPointF(bYAxisTopDown ? 299.750000 : 300.072330, bYAxisTopDown ? 300.072330 : 300.250000));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosRect1, m_rectRect1, *m_pPhysValRect1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesEllipse(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesEllipse",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeEllipse);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;

    QString strObjName = c_strGraphObjNameEllipse1;
    QString strMethod = "DrawingScene.drawGraphObj";
    QString strMthArgs = strObjName;

    // Ellipse
    //----------

    iResultValuesPrecision = -1;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strObjName));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("P1", QPoint(275, 275));
    pTestStep->setConfigValue("P2", QPoint(325, 325));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    QSizeF sizeEllipse = QSizeF(50.0, 50.0);
    m_ptPosEllipse1 = QPointF(300.0, 300.0);
    m_rectEllipse1 = QRectF(QPointF(-25.0, -25.0), sizeEllipse);
    m_pPhysValRectEllipse1->setSize(sizeEllipse);
    m_pPhysValRectEllipse1->setCenter(QPointF(300, bYAxisTopDown ? 300.0 : fYAxisMaxVal - 300.0));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        c_strGraphObjNameEllipse1, m_ptPosEllipse1,
        m_rectEllipse1, *m_pPhysValRectEllipse1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesEllipseModifications(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesEllipseModifications",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeEllipse);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;
    QString strObjName = c_strGraphObjNameEllipse1;
    QString strMethod;
    QString strMthArgs;
    CPhysValRect physValRectNew = *m_pPhysValRectEllipse1;
    CEnumSelectionPoint eSelPt;
    QPointF pt1SelPt;
    QPointF pt2SelPt;

    ZS::Test::CTestStepGroup* pGrpModifyEllipse = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify " + strObjName,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // setRotationAngle
    //-----------------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameEllipse1;
    eSelPt = bYAxisTopDown ? ESelectionPoint::RotateTop : ESelectionPoint::RotateBottom;
    physValRectNew = *m_pPhysValRectEllipse1;
    physValRectNew.setAngle(45.0);
    strMethod = "setRotationAngle";
    strMthArgs = physValRectNew.angle().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyEllipse,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectEllipse1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosEllipse1);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_physValAngleEllipse1 = physValRectNew.angle();
    *m_pPhysValRectEllipse1 = physValRectNew;
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosEllipse1, m_rectEllipse1, *m_pPhysValRectEllipse1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setWidth
    //---------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameEllipse1;
    QSizeF sizeRectangle = m_pPhysValRectEllipse1->size().toQSizeF();
    sizeRectangle.setWidth(100.0);
    eSelPt = ESelectionPoint::RightCenter;
    physValRectNew = *m_pPhysValRectEllipse1;
    physValRectNew.setWidth(sizeRectangle.width());
    strMethod = "setWidth";
    strMthArgs = physValRectNew.width().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyEllipse,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectEllipse1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosEllipse1);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosEllipse1 = QPointF(317.661165, 317.661165);
    m_rectEllipse1 = QRectF(QPointF(-49.976659, -25.0), QSizeF(99.953319, 50.0));
    m_pPhysValRectEllipse1->setSize(QSizeF(99.953319, 50.0));
    m_pPhysValRectEllipse1->setCenter(QPointF(317.661165, bYAxisTopDown ? 317.661165 : 282.338835));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosEllipse1, m_rectEllipse1, *m_pPhysValRectEllipse1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setHeight
    //----------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameEllipse1;
    sizeRectangle = m_pPhysValRectEllipse1->size().toQSizeF();
    sizeRectangle.setHeight(100.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectNew = *m_pPhysValRectEllipse1;
    physValRectNew.setHeight(sizeRectangle.height());
    strMethod = "setHeight";
    strMthArgs = physValRectNew.height().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyEllipse,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectEllipse1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosEllipse1);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosEllipse1 = QPointF(bYAxisTopDown ? 300.0 : 335.322330, bYAxisTopDown ? 335.322330 : 300.0);
    m_rectEllipse1 = QRectF(QPointF(-49.976659, -49.976659), QSizeF(99.953319, 99.953319));
    m_pPhysValRectEllipse1->setSize(QSizeF(99.953319, 99.953319));
    m_pPhysValRectEllipse1->setCenter(QPointF(bYAxisTopDown ? 300.0 : 335.322330, bYAxisTopDown ? 335.322330 : 300.0));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosEllipse1, m_rectEllipse1, *m_pPhysValRectEllipse1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setSize
    //--------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameEllipse1;
    sizeRectangle = QSizeF(150.0, 150.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomRight : ESelectionPoint::TopRight;
    physValRectNew = *m_pPhysValRectEllipse1;
    physValRectNew.setSize(sizeRectangle);
    strMethod = "setSize";
    strMthArgs = physValRectNew.size().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyEllipse,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectEllipse1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosEllipse1);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosEllipse1 = QPointF(bYAxisTopDown ? 300.0 : 370.822330, bYAxisTopDown ? 370.822330 : 300.0);
    m_rectEllipse1 = QRectF(QPointF(-75.078950, -75.078950), QSizeF(150.157900, 150.157900));
    m_pPhysValRectEllipse1->setSize(QSizeF(150.157900, 150.157900));
    m_pPhysValRectEllipse1->setCenter(QPointF(bYAxisTopDown ? 300.000000 : 370.822330, bYAxisTopDown ? 370.822330 : 300.000000));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosEllipse1, m_rectEllipse1, *m_pPhysValRectEllipse1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setWidth
    //---------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameEllipse1;
    sizeRectangle.setWidth(50.0);
    eSelPt = ESelectionPoint::RightCenter;
    physValRectNew = *m_pPhysValRectEllipse1;
    physValRectNew.setWidth(sizeRectangle.width());
    strMethod = "setWidth";
    strMthArgs = physValRectNew.width().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyEllipse,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectEllipse1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosEllipse1);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosEllipse1 = QPointF(bYAxisTopDown ? 264.5 : 335.322330, bYAxisTopDown ? 335.322330 : 264.5);
    m_rectEllipse1 = QRectF(QPointF(-24.874369, -75.078950), QSizeF(49.748737, 150.157900));
    m_pPhysValRectEllipse1->setSize(QSizeF(49.748737, 150.157900));
    m_pPhysValRectEllipse1->setCenter(QPointF(bYAxisTopDown ? 264.5 : 335.322330, bYAxisTopDown ? 335.322330 : 335.5));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosEllipse1, m_rectEllipse1, *m_pPhysValRectEllipse1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setHeight
    //----------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameEllipse1;
    sizeRectangle.setHeight(50.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectNew = *m_pPhysValRectEllipse1;
    physValRectNew.setHeight(sizeRectangle.height());
    strMethod = "setHeight";
    strMthArgs = physValRectNew.height().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyEllipse,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectEllipse1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosEllipse1);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosEllipse1 = QPointF(bYAxisTopDown ? 299.750000 : 300.072330, bYAxisTopDown ? 300.072330 : 299.749000);
    m_rectEllipse1 = QRectF(QPointF(-24.874369, -25.227922), QSizeF(49.748737, 50.455844));
    m_pPhysValRectEllipse1->setSize(QSizeF(49.748737, 50.455844));
    m_pPhysValRectEllipse1->setCenter(QPointF(bYAxisTopDown ? 299.750000 : 300.072330, bYAxisTopDown ? 300.072330 : 300.250000));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosEllipse1, m_rectEllipse1, *m_pPhysValRectEllipse1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesText(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesText",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeText);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;

    QString strObjName = c_strGraphObjNameText1;
    QString strMethod = "DrawingScene.drawGraphObj";
    QString strText = "Hello World";
    QString strMthArgs = strObjName + ", " + strText;

    // Text
    //----------

    iResultValuesPrecision = -1;
    QSizeF sizeText(100.0, 50.0); // adjusted to text size with margins
    QPoint pt1(250, 275);
    QPoint pt2(350, 325);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strObjName));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("P1", pt1);
    pTestStep->setConfigValue("P2", pt2);
    pTestStep->setConfigValue("Text", strText);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    QRect rectText(pt1, pt2);
    m_ptPosText1 = rectText.center();
    m_pPhysValRectText1->setSize(sizeText);
    m_pPhysValRectText1->setCenter(m_ptPosText1);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForText(
        strObjName, m_ptPosText1, *m_pPhysValRectText1, strText, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesTextModifications(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesTextModifications",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeText);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;
    QString strObjName = c_strGraphObjNameText1;
    QString strText = "Hello World";
    CPhysValRect physValRectNew = *m_pPhysValRectText1;
    CEnumSelectionPoint eSelPt;
    QString strMethod;
    QString strMthArgs;
    QPointF pt1SelPt;
    QPointF pt2SelPt;

    ZS::Test::CTestStepGroup* pGrpModifyText = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify " + strObjName,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // setRotationAngle
    //-----------------

    iResultValuesPrecision = 6;//bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameText1;
    eSelPt = bYAxisTopDown ? ESelectionPoint::RotateTop : ESelectionPoint::RotateBottom;
    physValRectNew = *m_pPhysValRectText1;
    physValRectNew.setAngle(45.0);
    strMethod = "setRotationAngle";
    strMthArgs = physValRectNew.angle().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyText,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectText1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", QPoint(m_ptPosText1.x() + 25, m_ptPosText1.y() + 10));
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    physValRectNew.setAngle(45.0);
    m_physValAngleText1 = physValRectNew.angle();
    *m_pPhysValRectText1 = physValRectNew;
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForText(
        strObjName, m_ptPosText1, *m_pPhysValRectText1, strText, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setWidth
    //---------

    iResultValuesPrecision = 6; //bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameText1;
    QSizeF sizeRectangle = m_pPhysValRectText1->size().toQSizeF();
    sizeRectangle.setWidth(125.0);
    eSelPt = ESelectionPoint::RightCenter;
    physValRectNew = *m_pPhysValRectText1;
    physValRectNew.setWidth(sizeRectangle.width());
    strMethod = "setWidth";
    strMthArgs = physValRectNew.width().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyText,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectText1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", QPoint(m_ptPosText1.x(), m_ptPosText1.y() + 20));
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    m_ptPosText1 = QPointF(308.822330, 308.822330);
    m_pPhysValRectText1->setSize(QSizeF(124.953319, 50.000000));
    m_pPhysValRectText1->setCenter(QPointF(m_ptPosText1.x(), bYAxisTopDown ? m_ptPosText1.y() : fYAxisMaxVal - m_ptPosText1.y()));
    strlstExpectedValues.append(resultValuesForText(
        strObjName, m_ptPosText1, *m_pPhysValRectText1, strText, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setHeight
    //----------

    iResultValuesPrecision = 6; //bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameText1;
    sizeRectangle = m_pPhysValRectText1->size().toQSizeF();
    sizeRectangle.setHeight(125.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectNew = *m_pPhysValRectText1;
    physValRectNew.setHeight(sizeRectangle.height());
    strMethod = "setHeight";
    strMthArgs = physValRectNew.height().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyText,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectText1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", QPoint(m_ptPosText1.x(), m_ptPosText1.y() + 20));
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosText1 = bYAxisTopDown ? QPointF(282.161165, 335.483496) : QPointF(335.483496, 282.161165);
    m_pPhysValRectText1->setSize(QSizeF(124.953319, 125.409163));
    m_pPhysValRectText1->setCenter(QPointF(m_ptPosText1.x(), bYAxisTopDown ? m_ptPosText1.y() : fYAxisMaxVal - m_ptPosText1.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForText(
        strObjName, m_ptPosText1, *m_pPhysValRectText1, strText, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setSize
    //--------

    iResultValuesPrecision = 6; //bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameText1;
    sizeRectangle = QSizeF(150.0, 150.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomRight : ESelectionPoint::TopRight;
    physValRectNew = *m_pPhysValRectText1;
    physValRectNew.setSize(sizeRectangle);
    strMethod = "setSize";
    strMthArgs = physValRectNew.size().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyText,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectText1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosText1);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosText1 = bYAxisTopDown ? QPointF(282.161165, 352.983496) : QPointF(352.983496, 282.161165);
    m_pPhysValRectText1->setSize(QSizeF(149.702056, 150.157900));
    m_pPhysValRectText1->setCenter(QPointF(m_ptPosText1.x(), bYAxisTopDown ? m_ptPosText1.y() : fYAxisMaxVal - m_ptPosText1.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForText(
        strObjName, m_ptPosText1, *m_pPhysValRectText1, strText, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setWidth
    //---------

    iResultValuesPrecision = 6; //bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameText1;
    sizeRectangle.setWidth(50.0);
    eSelPt = ESelectionPoint::RightCenter;
    physValRectNew = *m_pPhysValRectText1;
    physValRectNew.setWidth(sizeRectangle.width());
    strMethod = "setWidth";
    strMthArgs = physValRectNew.width().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyText,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectText1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosText1);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosText1 = bYAxisTopDown ? QPointF(246.911165, 317.733496) : QPointF(317.733496, 246.911165);
    m_pPhysValRectText1->setSize(QSizeF(50.000000, 150.157900));
    m_pPhysValRectText1->setCenter(QPointF(m_ptPosText1.x(), bYAxisTopDown ? m_ptPosText1.y() : fYAxisMaxVal - m_ptPosText1.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForText(
        strObjName, m_ptPosText1, *m_pPhysValRectText1, strText, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setHeight
    //----------

    iResultValuesPrecision = 6; //bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameText1;
    sizeRectangle.setHeight(50.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectNew = *m_pPhysValRectText1;
    physValRectNew.setHeight(sizeRectangle.height());
    strMethod = "setHeight";
    strMthArgs = physValRectNew.height().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyText,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectText1, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosText1);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosText1 = bYAxisTopDown ? QPointF(282.411165, 282.233496) : QPointF(282.233496, 282.411165);
    m_pPhysValRectText1->setSize(QSizeF(50.000000, 49.748737));
    m_pPhysValRectText1->setCenter(QPointF(m_ptPosText1.x(), bYAxisTopDown ? m_ptPosText1.y() : fYAxisMaxVal - m_ptPosText1.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForText(
        strObjName, m_ptPosText1, *m_pPhysValRectText1, strText, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesPolygonOpenArrow(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesPolygonOpenArrow",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypePolyline);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameOpenArrow + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameOpenArrow + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjPolygon(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameOpenArrow, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameOpenArrow));
    m_ptPosOpenArrow = QPointF(525, 200.0);
    m_polygonOpenArrow = QPolygonF({
        {-25.0,  -5.0},
        {  0.0,  -5.0},
        {  0.0, -20.0},
        { 25.0,   0.0},
        {  0.0,  20.0},
        {  0.0,   5.0},
        {-25.0,   5.0}
    });
    *m_pPhysValPolygonOpenArrow = QPolygonF({
        {500.0, bYAxisTopDown ? 195.0 : fYAxisMaxVal - 195.0},
        {525.0, bYAxisTopDown ? 195.0 : fYAxisMaxVal - 195.0},
        {525.0, bYAxisTopDown ? 180.0 : fYAxisMaxVal - 180.0},
        {550.0, bYAxisTopDown ? 200.0 : fYAxisMaxVal - 200.0},
        {525.0, bYAxisTopDown ? 220.0 : fYAxisMaxVal - 220.0},
        {525.0, bYAxisTopDown ? 205.0 : fYAxisMaxVal - 205.0},
        {500.0, bYAxisTopDown ? 205.0 : fYAxisMaxVal - 205.0}
    });
    QPolygon points({
        QPoint(500, 195),
        QPoint(525, 195),
        QPoint(525, 180),
        QPoint(550, 200),
        QPoint(525, 220),
        QPoint(525, 205),
        QPoint(500, 205)
    });
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypePolyline));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameOpenArrow);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameOpenArrow, m_ptPosOpenArrow, m_polygonOpenArrow, *m_pPhysValPolygonOpenArrow, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Label
    //-----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameOpenArrow + ".showLabel(" + CGraphObj::c_strLabelName + ")",
        /* strOperation    */ c_strGraphObjNameOpenArrow + ".showLabel(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameOpenArrow},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameOpenArrow]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"AnchorPoint", SGraphObjSelectionPoint(nullptr, ESelectionPointType::LineCenterPoint, 2).toString()},
        {"setPos", QPointF(560.0, 195.0)},
        {"ExpectedText", c_strGraphObjNameOpenArrow}
    });
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesPolygonTriangle(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesPolygonTriangle",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypePolygon);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameTriangle + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameTriangle + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjPolygon(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameTriangle, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTriangle));
    m_ptPosTriangle = QPointF(300.0, 275.0);
    m_polygonTriangle = QPolygonF({
        {-25.0,  25.0},
        { 25.0,  25.0},
        {  0.0, -25.0}
    });
    *m_pPhysValPolygonTriangle = QPolygonF({
        {275.0, bYAxisTopDown ? 300.0 : fYAxisMaxVal - 300.0},
        {325.0, bYAxisTopDown ? 300.0 : fYAxisMaxVal - 300.0},
        {300.0, bYAxisTopDown ? 250.0 : fYAxisMaxVal - 250.0}
    });
    QPolygon points({
        QPoint(275, 300),
        QPoint(325, 300),
        QPoint(300, 250)
    });
    m_ptMousePos = points[2];
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypePolygon));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTriangle);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Label
    //-----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTriangle + ".showLabel(" + CGraphObj::c_strLabelName + ")",
        /* strOperation    */ c_strGraphObjNameTriangle + ".showLabel(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"AnchorPoint", SGraphObjSelectionPoint(nullptr, ESelectionPointType::LineCenterPoint, 1).toString()},
        {"setPos", QPointF(320.0, 255.0)},
        {"ExpectedText", c_strGraphObjNameTriangle}
    });
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesPolygonTriangleModifications(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesPolygonTriangleModifications",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypePolyline);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;
    QString strObjName;
    QString strMethod;
    QString strMthArgs;
    CPhysValPoint physValPoint(*m_pDrawingScene);
    CPhysValLine physValLine(*m_pDrawingScene);
    CPhysValPolygon physValPolygonTriangleNew = *m_pPhysValPolygonTriangle;
    SGraphObjSelectionPoint selPt;
    QPointF pt1SelPt;
    QPointF pt2SelPt;

    ZS::Test::CTestStepGroup* pGrpModifyTriangle = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify " + c_strGraphObjNameTriangle,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTriangle + ".showLabels",
        /* strOperation    */ c_strGraphObjNameTriangle + ".showLabels()",
        /* pGrpParent      */ pGrpModifyTriangle,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "1"},
        {"AnchorPoint", SGraphObjSelectionPoint(nullptr, ESelectionPointType::PolygonPoint, 0).toString()},
        {"setPos", QPointF(255.0, 290.0)},
        {"ExpectedText", CGraphObj::c_strGeometryLabelNameP + "1"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "2"},
        {"AnchorPoint", SGraphObjSelectionPoint(nullptr, ESelectionPointType::PolygonPoint, 1).toString()},
        {"setPos", QPointF(330.0, 290.0)},
        {"ExpectedText", CGraphObj::c_strGeometryLabelNameP + "2"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "3"},
        {"AnchorPoint", SGraphObjSelectionPoint(nullptr, ESelectionPointType::PolygonPoint, 2).toString()},
        {"setPos", QPointF(295.0, 230.0)},
        {"ExpectedText", CGraphObj::c_strGeometryLabelNameP + "3"}
    });

    // Show Geometry Labels
    //---------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTriangle + ".showGeometryLabels",
        /* strOperation    */ c_strGraphObjNameTriangle + ".showGeometryLabels()",
        /* pGrpParent      */ pGrpModifyTriangle,
        /* szDoTestStepFct */ SLOT(doTestStepShowGeometryLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameCenter},
        {"setPos", QPointF(320.0, 270.0)},
        {"ExpectedText", m_pPhysValPolygonTriangle->center().toString()}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameWidth},
        {"setPos", QPointF(295.0, 330.0)},
        {"ExpectedText", QString::number(m_pPhysValPolygonTriangle->width().getVal(), 'f', iResultValuesPrecision)}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameHeight},
        {"setPos", QPointF(230.0, 265.0)},
        {"ExpectedText", QString::number(m_pPhysValPolygonTriangle->height().getVal(), 'f', iResultValuesPrecision)}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"setPos", QPointF(370.0, 285.0)},
        {"ExpectedText", m_pPhysValPolygonTriangle->angle().toString()}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "1"},
        {"setPos", QPointF(240.0, 305.0)},
        {"ExpectedText", m_pPhysValPolygonTriangle->at(0).toString()}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "2"},
        {"setPos", QPointF(315.0, 305.0)},
        {"ExpectedText", m_pPhysValPolygonTriangle->at(1).toString()}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "3"},
        {"setPos", QPointF(280.0, 215.0)},
        {"ExpectedText", m_pPhysValPolygonTriangle->at(2).toString()}
    });

    // Rotate
    //-------

    selPt.m_selPtType = ESelectionPointType::BoundingRectangle;
    selPt.m_selPt = bYAxisTopDown ? ESelectionPoint::RotateTop : ESelectionPoint::RotateBottom;
    physValPolygonTriangleNew = *m_pPhysValPolygonTriangle;
    physValPolygonTriangleNew.setAngle(90.0);
    strObjName = c_strGraphObjNameTriangle;
    strMethod = "setRotationAngle";
    strMthArgs = physValPolygonTriangleNew.angle().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyTriangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(m_pPhysValPolygonTriangle->physValBoundingRect(), selPt.m_selPt);
    pt2SelPt = getSelectionPointCoors(physValPolygonTriangleNew.physValBoundingRect(), selPt.m_selPt);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTriangle);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]);
    pTestStep->setConfigValue("SelectionPoint", selPt.toString());
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move from current position to empty area
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, QPoint(260, 270));
    pTestStep->addDataRow({ // Deselect object by clicking on empty area
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos}
    });
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos}
    });
    // Move mouse into object (but not into the center as there is a label)
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, QPoint(m_ptPosTriangle.toPoint().x(), m_ptPosTriangle.toPoint().y() + 10));
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos}
    });
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos}
    });
    pTestStep->addDataRow({ // Click on selection point
        {"Method", "mousePressEvent"},
        {"MousePos", pt1SelPt}
    });
    // Move selection point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, pt1SelPt.toPoint(), pt2SelPt.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", pt2SelPt}
    });
    strlstExpectedValues.clear();
    m_physValAngleTriangle = physValPolygonTriangleNew.angle();
    *m_pPhysValPolygonTriangle = physValPolygonTriangleNew;
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, true, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // replace(2, )
    //-------------

    selPt.m_selPtType = ESelectionPointType::PolygonPoint;
    selPt.m_selPt = ESelectionPoint::PolygonPoint;
    selPt.m_idxPt = 2;
    physValPolygonTriangleNew = *m_pPhysValPolygonTriangle;
    physValPoint = physValPolygonTriangleNew.at(selPt.m_idxPt);
    physValPoint.setX(375.0);
    physValPolygonTriangleNew.replace(selPt.m_idxPt, physValPoint);
    strObjName = c_strGraphObjNameTriangle;
    strMethod = "replace";
    strMthArgs = QString::number(selPt.m_idxPt) + ", {" + physValPoint.toString() + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyTriangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValPolygonTriangle, selPt);
    pt2SelPt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypePolygon));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTriangle);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]);
    pTestStep->setConfigValue("SelectionPoint", selPt.toString());
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse into object and select object to edit polygon shape points
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, m_ptPosTriangle.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    // Move mouse to polygon point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, pt1SelPt.toPoint());
    pTestStep->addDataRow({ // Click on selection point
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos}
    });
    // Move polygon point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, pt2SelPt.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos}
    });
    m_ptPosTriangle = QPointF(325.0, 275.0);
    m_polygonTriangle = QPolygonF({
        {-25.0,  50.0},
        { 25.0,  50.0},
        {  0.0, -50.0}
    });
    *m_pPhysValPolygonTriangle = physValPolygonTriangleNew;
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, true, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // append()
    //---------

    selPt.m_selPtType = ESelectionPointType::LineCenterPoint;
    selPt.m_selPt = ESelectionPoint::LineCenterPoint;
    selPt.m_idxPt = m_pPhysValPolygonTriangle->count() - 1;
    physValPolygonTriangleNew = *m_pPhysValPolygonTriangle;
    physValPoint = physValPolygonTriangleNew.at(selPt.m_idxPt);
    physValPoint.setX(300.0);
    physValPoint.setY(bYAxisTopDown ? 200.0 : fYAxisMaxVal - 200.0);
    physValPolygonTriangleNew.append(physValPoint);
    strObjName = c_strGraphObjNameTriangle;
    strMethod = "append";
    strMthArgs = "{" + physValPoint.toString() + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyTriangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValPolygonTriangle, selPt);
    pt2SelPt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypePolygon));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTriangle);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]);
    pTestStep->setConfigValue("SelectionPoint", selPt.toString());
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse into object and select object to edit polygon shape points
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, m_ptPosTriangle.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    // Move mouse to line segment between last and first polygon point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, pt1SelPt.toPoint());
    pTestStep->addDataRow({ // Click on line segment to create new polygon point
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    // Move newly created polygon point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, pt2SelPt.toPoint(), 0, Qt::LeftButton, Qt::ControlModifier);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    m_ptPosTriangle = QPointF(325.0, 250.0);
    m_polygonTriangle = QPolygonF({
        {  0.0,  50.0},
        { 50.0,  50.0},
        { 25.0, -50.0},
        {-50.0,  25.0}
    });
    *m_pPhysValPolygonTriangle = physValPolygonTriangleNew;
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, true, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTriangle + ".showLabels",
        /* strOperation    */ c_strGraphObjNameTriangle + ".showLabels()",
        /* pGrpParent      */ pGrpModifyTriangle,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "4"},
        {"AnchorPoint", SGraphObjSelectionPoint(nullptr, ESelectionPointType::PolygonPoint, 3).toString()},
        {"setPos", QPointF(295.0, 180.0)},
        {"ExpectedText", CGraphObj::c_strGeometryLabelNameP + "4"}
    });

    // Hide Geometry Labels
    //---------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTriangle + ".hideGeometryLabels",
        /* strOperation    */ c_strGraphObjNameTriangle + ".hideGeometryLabels()",
        /* pGrpParent      */ pGrpModifyTriangle,
        /* szDoTestStepFct */ SLOT(doTestStepHideGeometryLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameCenter},
        {"ExpectedValue", c_strGraphObjNameTriangle + "." + CGraphObj::c_strGeometryLabelNameCenter + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameWidth},
        {"ExpectedValue", c_strGraphObjNameTriangle + "." + CGraphObj::c_strGeometryLabelNameWidth + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameHeight},
        {"ExpectedValue", c_strGraphObjNameTriangle + "." + CGraphObj::c_strGeometryLabelNameHeight + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"ExpectedValue", c_strGraphObjNameTriangle + "." + CGraphObj::c_strGeometryLabelNameAngle + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "1"},
        {"ExpectedValue", c_strGraphObjNameTriangle + "." + CGraphObj::c_strGeometryLabelNameP + "1 not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "2"},
        {"ExpectedValue", c_strGraphObjNameTriangle + "." + CGraphObj::c_strGeometryLabelNameP + "2 not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "3"},
        {"ExpectedValue", c_strGraphObjNameTriangle + "." + CGraphObj::c_strGeometryLabelNameP + "3 not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "4"},
        {"ExpectedValue", c_strGraphObjNameTriangle + "." + CGraphObj::c_strGeometryLabelNameP + "4 not found"}
    });

    // insert(1, )
    //-------------

    selPt.m_selPtType = ESelectionPointType::LineCenterPoint;
    selPt.m_selPt = ESelectionPoint::LineCenterPoint;
    selPt.m_idxPt = 0;
    physValPolygonTriangleNew = *m_pPhysValPolygonTriangle;
    physValPoint = m_pPhysValPolygonTriangle->at(selPt.m_idxPt);
    physValPoint.setX(200.0);
    physValPoint.setY(bYAxisTopDown ? 270.0 : fYAxisMaxVal - 270.0);
    physValPolygonTriangleNew.insert(selPt.m_idxPt+1, physValPoint);
    strObjName = c_strGraphObjNameTriangle;
    strMethod = "insert";
    strMthArgs = QString::number(selPt.m_idxPt+1) + ", {" + physValPoint.toString() + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyTriangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValPolygonTriangle, selPt);
    pt2SelPt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypePolygon));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTriangle);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]);
    pTestStep->setConfigValue("SelectionPoint", selPt.toString());
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse into object and select object to edit polygon shape points
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, m_ptPosTriangle.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    // Move mouse to line segment between first and second polygon point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, pt1SelPt.toPoint());
    pTestStep->addDataRow({ // Click on line segment to create new polygon point
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    // Move newly created polygon point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, pt2SelPt.toPoint(), 0, Qt::LeftButton, Qt::ControlModifier);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    m_ptPosTriangle = QPointF(287.5, 250.0);
    m_polygonTriangle = QPolygonF({
        {  0.0,  12.5},
        { 20.0,  87.5},
        { 50.0,  12.5},
        { 25.0, -87.5},
        {-50.0, -12.5}
    });
    *m_pPhysValPolygonTriangle = physValPolygonTriangleNew;
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, true, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTriangle + ".showLabels",
        /* strOperation    */ c_strGraphObjNameTriangle + ".showLabels()",
        /* pGrpParent      */ pGrpModifyTriangle,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "2"},
        {"AnchorPoint", SGraphObjSelectionPoint(nullptr, ESelectionPointType::PolygonPoint, 1).toString()},
        {"setPos", QPointF(180.0, 260.0)},
        {"ExpectedText", CGraphObj::c_strGeometryLabelNameP + "2"}
    });

    // append()
    //---------

    selPt.m_selPtType = ESelectionPointType::LineCenterPoint;
    selPt.m_selPt = ESelectionPoint::LineCenterPoint;
    selPt.m_idxPt = m_pPhysValPolygonTriangle->count() - 1;
    physValPolygonTriangleNew = *m_pPhysValPolygonTriangle;
    physValPoint = physValPolygonTriangleNew.at(selPt.m_idxPt);
    physValPoint.setX(350.0);
    physValPoint.setY(bYAxisTopDown ? 220.0 : fYAxisMaxVal - 220.0);
    strObjName = c_strGraphObjNameTriangle;
    strMethod = "append";
    strMthArgs = "{" + physValPoint.toString() + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyTriangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValPolygonTriangle, selPt);
    pt2SelPt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypePolygon));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTriangle);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]);
    pTestStep->setConfigValue("SelectionPoint", selPt.toString());
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse into object and select object to edit polygon shape points
    // The P1 label is on the position of the polygon.
    // We need to move a little bit away from this label to select the polygon.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, QPoint(m_ptPosTriangle.toPoint().x() + 10, m_ptPosTriangle.toPoint().y() + 10));
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    // Move mouse to line segment between last and first polygon point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, pt1SelPt.toPoint());
    pTestStep->addDataRow({ // Click on line segment to create new polygon point
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    // Move newly created polygon point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, pt2SelPt.toPoint(), 0, Qt::LeftButton, Qt::ControlModifier);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    m_ptPosTriangle = QPointF(287.5, 250.0);
    m_polygonTriangle = QPolygonF({
        {  0.0,  12.5},
        { 20.0,  87.5},
        { 50.0,  12.5},
        { 25.0, -87.5},
        {-50.0, -12.5},
        {-30.0, -62.5}
    });
    m_pPhysValPolygonTriangle->append(physValPoint);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, true, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTriangle + ".showLabels",
        /* strOperation    */ c_strGraphObjNameTriangle + ".showLabels()",
        /* pGrpParent      */ pGrpModifyTriangle,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "6"},
        {"AnchorPoint", SGraphObjSelectionPoint(nullptr, ESelectionPointType::PolygonPoint, 5).toString()},
        {"setPos", QPointF(355.0, 210.0)},
        {"ExpectedText", CGraphObj::c_strGeometryLabelNameP + "6"}
    });

    // removeLast
    //-------------

    iResultValuesPrecision = bUnitPixel ? 1 : drawingSize.metricImageCoorsDecimals();
    selPt.m_selPtType = ESelectionPointType::PolygonPoint;
    selPt.m_selPt = ESelectionPoint::PolygonPoint;
    selPt.m_idxPt = m_pPhysValPolygonTriangle->count() - 1;
    physValPolygonTriangleNew = *m_pPhysValPolygonTriangle;
    physValPolygonTriangleNew.removeLast();
    strObjName = c_strGraphObjNameTriangle;
    strMethod = "removeLast";
    strMthArgs = "";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyTriangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValPolygonTriangle, selPt);
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypePolygon));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTriangle);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]);
    pTestStep->setConfigValue("SelectionPoint", selPt.toString());
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse into object and select object to edit polygon shape points
    // The P1 label is on the position of the polygon.
    // We need to move a little bit away from this label to select the polygon.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, QPoint(m_ptPosTriangle.toPoint().x() + 10, m_ptPosTriangle.toPoint().y() + 10));
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    // Move mouse to polygon point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, pt1SelPt.toPoint());
    pTestStep->addDataRow({ // Click on selection point
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButton", Qt::RightButton}
    });
    pTestStep->addDataRow({ // Click on selection point
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButton", Qt::RightButton}
    });
    // Move mouse to popup context menu, entry Delete Point
    pTestStep->addDataRow({
        {"Method", "activePopupWidget.keyPressEvent"},
        {"Key", Qt::Key_Up}
    });
    pTestStep->addDataRow({
        {"Method", "activePopupWidget.keyReleaseEvent"},
        {"Key", Qt::Key_Up}
    });
    pTestStep->addDataRow({
        {"Method", "activePopupWidget.keyPressEvent"},
        {"Key", Qt::Key_Enter}
    });
    pTestStep->addDataRow({
        {"Method", "activePopupWidget.keyReleaseEvent"},
        {"Key", Qt::Key_Enter}
    });
    m_ptPosTriangle = QPointF(287.5, 250.0);
    m_polygonTriangle = QPolygonF({
        {  0.0,  12.5},
        { 20.0,  87.5},
        { 50.0,  12.5},
        { 25.0, -87.5},
        {-50.0, -12.5}
    });
    m_pPhysValPolygonTriangle->removeLast();
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, true, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Hide Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTriangle + ".hideLabels",
        /* strOperation    */ c_strGraphObjNameTriangle + ".hideLabels()",
        /* pGrpParent      */ pGrpModifyTriangle,
        /* szDoTestStepFct */ SLOT(doTestStepHideLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameTriangle + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "1"},
        {"ExpectedValue", c_strGraphObjNameTriangle + "." + CGraphObj::c_strGeometryLabelNameP + "1 not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "3"},
        {"ExpectedValue", c_strGraphObjNameTriangle + "." + CGraphObj::c_strGeometryLabelNameP + "3 not found"}
    });

    // Resize (setBottomRight)
    //------------------------

    selPt.m_selPtType = ESelectionPointType::BoundingRectangle;
    selPt.m_selPt = ESelectionPoint::BottomRight;
    physValPolygonTriangleNew = *m_pPhysValPolygonTriangle;
    physValPolygonTriangleNew.setBottomRight(QPointF(260.0, bYAxisTopDown ? 400.0 : fYAxisMaxVal - 400.0));
    strObjName = c_strGraphObjNameTriangle;
    strMethod = "setBottomRight";
    strMthArgs = physValPolygonTriangleNew.bottomRight().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyTriangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(m_pPhysValPolygonTriangle->physValBoundingRect(), selPt.m_selPt);
    pt2SelPt = getSelectionPointCoors(physValPolygonTriangleNew.physValBoundingRect(), selPt.m_selPt);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTriangle);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]);
    pTestStep->setConfigValue("SelectionPoint", selPt.toString());
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse into object and select object to edit the bounding rectangle
    // The P1 label is on the position of the polygon.
    // We need to move a little bit away from this label to select the polygon.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, QPoint(m_ptPosTriangle.toPoint().x() + 10, m_ptPosTriangle.toPoint().y() + 10));
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos}
    });
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos}
    });
    // Move selection point
    pTestStep->addDataRow({
        {"Method", "mousePressEvent"}, // Click on selection point
        {"MousePos", pt1SelPt}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, pt1SelPt.toPoint(), pt2SelPt.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", pt2SelPt}
    });
    m_ptPosTriangle = QPointF(317.5, 300.0);
    m_polygonTriangle = QPolygonF({
        {   0.000,   8.214},
        {  40.000,  57.500},
        { 100.000,   8.214},
        {  50.000, -57.499},
        {-100.000,  -8.214}
    });
    *m_pPhysValPolygonTriangle = physValPolygonTriangleNew;
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, true, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesPolygonStar(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesPolygonStar",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypePolygon);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameStar1 + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameStar1 + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjPolygon(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameStar1, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar1));
    m_ptPosStar1 = QPointF(500.0, 400.0);
    m_polygonStar1 = QPolygonF({
        {   0.0, -100.0},
        {  25.0,  -25.0},
        { 100.0,    0.0},
        {  25.0,   25.0},
        {   0.0,  100.0},
        { -25.0,   25.0},
        {-100.0,    0.0},
        { -25.0,  -25.0}
    });
    *m_pPhysValPolygonStar1 = QPolygonF({
        {500.0, bYAxisTopDown ? 300.0 : fYAxisMaxVal - 300.0},
        {525.0, bYAxisTopDown ? 375.0 : fYAxisMaxVal - 375.0},
        {600.0, bYAxisTopDown ? 400.0 : fYAxisMaxVal - 400.0},
        {525.0, bYAxisTopDown ? 425.0 : fYAxisMaxVal - 425.0},
        {500.0, bYAxisTopDown ? 500.0 : fYAxisMaxVal - 500.0},
        {475.0, bYAxisTopDown ? 425.0 : fYAxisMaxVal - 425.0},
        {400.0, bYAxisTopDown ? 400.0 : fYAxisMaxVal - 400.0},
        {475.0, bYAxisTopDown ? 375.0 : fYAxisMaxVal - 375.0}
    });
    QPolygon points({
        QPoint(500, 300),
        QPoint(525, 375),
        QPoint(600, 400),
        QPoint(525, 425),
        QPoint(500, 500),
        QPoint(475, 425),
        QPoint(400, 400),
        QPoint(475, 375)
    });
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypePolygon));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameStar1);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameStar1, m_ptPosStar1, m_polygonStar1, *m_pPhysValPolygonStar1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameStar1 + ".showLabel(" + CGraphObj::c_strLabelName + ")",
        /* strOperation    */ c_strGraphObjNameStar1 + ".showLabel(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar1},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameStar1]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(540.0, 320.0)},
        {"ExpectedText", c_strGraphObjNameStar1}
    });
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsNotAnchored(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsNotAnchored",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameConnections;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strGraphObjFactoryGroup = CObjFactory::c_strGroupNameConnections;
    QString strGraphObjType;
    QString strGraphObjName;
    QString strMethod;
    QString strMthArgs;

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpDrawConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;

    // Connection Point 1
    //---------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionPoint);
    strGraphObjName = c_strGraphObjNameConnectionPoint1;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    m_ptPosConnectionPoint1 = QPointF(200.0, 250.0);
    *m_pPhysValConnectionPoint1 = QPointF(200.0, bYAxisTopDown ? 250.0 : fYAxisMaxVal - 250.0);
    pTestStep->setConfigValue("GraphObjFactoryGroup", strGraphObjFactoryGroup);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", m_ptPosConnectionPoint1);
    pTestStep->setConfigValue("P2", m_ptPosConnectionPoint1);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionPoint(
        strGraphObjName, m_ptPosConnectionPoint1, *m_pPhysValConnectionPoint1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Point 2
    //---------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionPoint);
    strGraphObjName = c_strGraphObjNameConnectionPoint2;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    m_ptPosConnectionPoint2 = QPointF(200.0, 350.0);
    *m_pPhysValConnectionPoint2 = QPointF(200.0, bYAxisTopDown ? 350.0 : fYAxisMaxVal - 350.0);
    pTestStep->setConfigValue("GraphObjFactoryGroup", strGraphObjFactoryGroup);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", m_ptPosConnectionPoint2);
    pTestStep->setConfigValue("P2", m_ptPosConnectionPoint2);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionPoint(
        strGraphObjName, m_ptPosConnectionPoint2, *m_pPhysValConnectionPoint2, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Point 3
    //---------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionPoint);
    strGraphObjName = c_strGraphObjNameConnectionPoint3;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    m_ptPosConnectionPoint3 = QPointF(600.0, 250.0);
    *m_pPhysValConnectionPoint3 = QPointF(600.0, bYAxisTopDown ? 250.0 : fYAxisMaxVal - 250.0);
    pTestStep->setConfigValue("GraphObjFactoryGroup", strGraphObjFactoryGroup);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", m_ptPosConnectionPoint3);
    pTestStep->setConfigValue("P2", m_ptPosConnectionPoint3);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionPoint(
        strGraphObjName, m_ptPosConnectionPoint3, *m_pPhysValConnectionPoint3, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Point 4
    //---------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionPoint);
    strGraphObjName = c_strGraphObjNameConnectionPoint4;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    m_ptPosConnectionPoint4 = QPointF(600.0, 350.0);
    *m_pPhysValConnectionPoint4 = QPointF(600.0, bYAxisTopDown ? 350.0 : fYAxisMaxVal - 350.0);
    pTestStep->setConfigValue("GraphObjFactoryGroup", strGraphObjFactoryGroup);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", m_ptPosConnectionPoint4);
    pTestStep->setConfigValue("P2", m_ptPosConnectionPoint4);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionPoint(
        strGraphObjName, m_ptPosConnectionPoint4, *m_pPhysValConnectionPoint4, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: CnctPt1 -> CnctPt2
    //------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt1CnctPt2;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    m_polygonConnectionLineCnctPt1CnctPt2 = QPolygonF({
        { 0.0, -50.0},
        { 0.0,  50.0}
    });
    *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2 = QPolygonF({
        {m_pPhysValConnectionPoint1->toQPointF()},
        {m_pPhysValConnectionPoint2->toQPointF()}
    });
    // We need a start position, from which we move to the first connection point.
    // The we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    QPolygon points({
        QPoint(240, 250),
        m_ptPosConnectionPoint1.toPoint(),
        QPoint((m_ptPosConnectionPoint1.x() + m_ptPosConnectionPoint2.x()) / 2,
               (m_ptPosConnectionPoint1.y() + m_ptPosConnectionPoint2.y()) / 2),
        m_ptPosConnectionPoint2.toPoint()
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt1CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: CnctPt1 -> CnctPt2: setLineStyle(Color, SolidLine)
    //--------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt1CnctPt2;
    strMethod = "setDrawSettings";
    strMthArgs = "{LineStyle, blue, Solid}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setPenColor"},
        {"PenColor", "blue"}
    });
    pTestStep->addDataRow({
        {"Method", "setLineStyle"},
        {"LineStyle", CEnumLineStyle(ELineStyle::SolidLine).toString()}
    });

    // Connection Line: CnctPt1 -> CnctPt2: setLineEndStyle(EndPoint, Arrow)
    //----------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt1CnctPt2;
    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: CnctPt3 -> CnctPt4
    //------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt3CnctPt4;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    m_polygonConnectionLineCnctPt3CnctPt4 = QPolygonF({
        { -10.0, -50.0},
        {  10.0, -30.0},
        {   0.0,   0.0},
        {  10.0,  30.0},
        { -10.0,  50.0}
    });
    *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4 = QPolygonF({
        {m_pPhysValConnectionPoint3->toQPointF()},
        {QPointF(620.0, bYAxisTopDown ? 270.0 : 330.0)},
        {QPointF(610.0, bYAxisTopDown ? 300.0 : 300.0)},
        {QPointF(620.0, bYAxisTopDown ? 330.0 : 270.0)},
        {m_pPhysValConnectionPoint4->toQPointF()}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(640, 250),
        m_ptPosConnectionPoint3.toPoint(),
        QPoint(m_ptPosConnectionPoint3.x() + 20, m_ptPosConnectionPoint3.y() + 20),
        QPoint(m_ptPosConnectionPoint3.x() + 10, m_ptPosConnectionPoint3.y() + 50),
        QPoint(m_ptPosConnectionPoint3.x() + 20, m_ptPosConnectionPoint3.y() + 80),
        m_ptPosConnectionPoint4.toPoint()
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt3CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: CnctPt3 -> CnctPt4: setLineStyle(Color, SolidLine)
    //--------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt3CnctPt4;
    strMethod = "setDrawSettings";
    strMthArgs = "{LineStyle, blue, Solid}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setPenColor"},
        {"PenColor", "blue"}
    });
    pTestStep->addDataRow({
        {"Method", "setLineStyle"},
        {"LineStyle", CEnumLineStyle(ELineStyle::SolidLine).toString()}
    });

    // Connection Line: CnctPt3 -> CnctPt4: setLineEndStyle(EndPoint, Arrow)
    //----------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt3CnctPt4;
    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: CnctPt1 -> CnctPt4
    //------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt1CnctPt4;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    m_polygonConnectionLineCnctPt1CnctPt4 = QPolygonF({
        {-200.0, -50.0},
        { 200.0,  50.0}
    });
    *m_pPhysValPolygonConnectionLineCnctPt1CnctPt4 = QPolygonF({
        {m_pPhysValConnectionPoint1->toQPointF()},
        {m_pPhysValConnectionPoint4->toQPointF()}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(240, 250),
        m_ptPosConnectionPoint1.toPoint(),
        m_ptPosConnectionPoint4.toPoint()
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt1CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt4, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: CnctPt1 -> CnctPt4: setLineStyle(Color, SolidLine)
    //--------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt1CnctPt4;
    strMethod = "setDrawSettings";
    strMthArgs = "{LineStyle, blue, Solid}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setPenColor"},
        {"PenColor", "blue"}
    });
    pTestStep->addDataRow({
        {"Method", "setLineStyle"},
        {"LineStyle", CEnumLineStyle(ELineStyle::SolidLine).toString()}
    });

    // Connection Line: CnctPt1 -> CnctPt4: setLineEndStyle(EndPoint, Arrow)
    //----------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt1CnctPt4;
    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: CnctPt3 -> CnctPt2
    //------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt3CnctPt2;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    m_polygonConnectionLineCnctPt3CnctPt2 = QPolygonF({
        { 200.0, -50.0},
        {-200.0,  50.0}
    });
    *m_pPhysValPolygonConnectionLineCnctPt3CnctPt2 = QPolygonF({
        {m_pPhysValConnectionPoint3->toQPointF()},
        {m_pPhysValConnectionPoint2->toQPointF()}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(640, 250),
        m_ptPosConnectionPoint3.toPoint(),
        // The additional point will be removed by normalizing the polygon.
        QPoint((m_ptPosConnectionPoint3.x() + m_ptPosConnectionPoint2.x()) / 2,
               (m_ptPosConnectionPoint3.y() + m_ptPosConnectionPoint2.y()) / 2),
        m_ptPosConnectionPoint2.toPoint()
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt3CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt2, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: CnctPt3 -> CnctPt2: setLineStyle(Color, SolidLine)
    //--------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt3CnctPt2;
    strMethod = "setDrawSettings";
    strMthArgs = "{LineStyle, blue, Solid}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setPenColor"},
        {"PenColor", "blue"}
    });
    pTestStep->addDataRow({
        {"Method", "setLineStyle"},
        {"LineStyle", CEnumLineStyle(ELineStyle::SolidLine).toString()}
    });

    // Connection Line: CnctPt3 -> CnctPt2: setLineEndStyle(EndPoint, Arrow)
    //----------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt3CnctPt2;
    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Cncts.showLabels(" + CGraphObj::c_strLabelName + ")",
        /* strOperation    */ "Cncts.showLabels(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionPoint1},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionPoint1]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(180.0, 230.0)},
        {"ExpectedText", c_strGraphObjNameConnectionPoint1}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionPoint2},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionPoint2]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(180.0, 330.0)},
        {"ExpectedText", c_strGraphObjNameConnectionPoint2}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionPoint3},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionPoint3]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(580.0, 230.0)},
        {"ExpectedText", c_strGraphObjNameConnectionPoint3}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionPoint4},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionPoint4]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(580.0, 330.0)},
        {"ExpectedText", c_strGraphObjNameConnectionPoint4}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineCnctPt1CnctPt2},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt1CnctPt2]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(180.0, 280.0)},
        {"ExpectedText", c_strGraphObjNameConnectionLineCnctPt1CnctPt2}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineCnctPt3CnctPt4},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt3CnctPt4]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(620.0, 280.0)},
        {"ExpectedText", c_strGraphObjNameConnectionLineCnctPt3CnctPt4}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineCnctPt1CnctPt4},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt1CnctPt4]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(380.0, 280.0)},
        {"ExpectedText", c_strGraphObjNameConnectionLineCnctPt1CnctPt4}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineCnctPt3CnctPt2},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt3CnctPt2]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(420.0, 320.0)},
        {"ExpectedText", c_strGraphObjNameConnectionLineCnctPt3CnctPt2}
    });
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsNotAnchoredModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsNotAnchoredModifications",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameConnections;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strGraphObjFactoryGroup = CObjFactory::c_strGroupNameConnections;

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpModifyConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;

    QString strGraphObjType;
    QString strGraphObjName;
    QString strMethod;
    QString strMthArgs;
    CPhysValPoint physValPoint(*m_pDrawingScene);
    QPoint ptMousePos;
    int idxPt;

    // Hide Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + ".hideLabels(" + CGraphObj::c_strLabelName + ")",
        /* strOperation    */ "CnctLines.hideLabels(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepHideLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineCnctPt1CnctPt2},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt1CnctPt2]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameConnectionLineCnctPt1CnctPt2 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineCnctPt3CnctPt4},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt3CnctPt4]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameConnectionLineCnctPt3CnctPt4 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineCnctPt1CnctPt4},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt1CnctPt4]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameConnectionLineCnctPt1CnctPt4 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineCnctPt3CnctPt2},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt3CnctPt2]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameConnectionLineCnctPt3CnctPt2 + "." + CGraphObj::c_strLabelName + " not found"}
    });

    // move CntPt1
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionPoint);
    strGraphObjName = c_strGraphObjNameConnectionPoint1;
    strMethod = "move";
    QPointF ptPosConnectionPoint1New(300.0, 200.0);
    strMthArgs = qPoint2Str(ptPosConnectionPoint1New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt1CnctPt2]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt1CnctPt4]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto connection point, press mouse to select connection point, move the connection point while
    // mouse is pressed to the new position and release the mouse event.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 250), m_ptPosConnectionPoint1.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosConnectionPoint1.toPoint(), ptPosConnectionPoint1New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosConnectionPoint1 = ptPosConnectionPoint1New;
    m_pPhysValConnectionPoint1->setX(ptPosConnectionPoint1New.x());
    m_pPhysValConnectionPoint1->setY(bYAxisTopDown ? ptPosConnectionPoint1New.y() : fYAxisMaxVal - ptPosConnectionPoint1New.y());
    m_polygonConnectionLineCnctPt1CnctPt2 = QPolygonF({
        {  50.0, -75.0},
        { -50.0,  75.0}
    });
    m_polygonConnectionLineCnctPt1CnctPt4 = QPolygonF({
        {-150.0, -75.0},
        { 150.0,  75.0}
    });
    *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2 = QPolygonF({
        {m_pPhysValConnectionPoint1->toQPointF()},
        {m_pPhysValConnectionPoint2->toQPointF()}
    });
    *m_pPhysValPolygonConnectionLineCnctPt1CnctPt4 = QPolygonF({
        {m_pPhysValConnectionPoint1->toQPointF()},
        {m_pPhysValConnectionPoint4->toQPointF()}
    });
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionPoint(
        strGraphObjName, m_ptPosConnectionPoint1, *m_pPhysValConnectionPoint1, true, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt2, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt1CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, false, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt4, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt1CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt4, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // move CntPt2
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionPoint);
    strGraphObjName = c_strGraphObjNameConnectionPoint2;
    strMethod = "move";
    QPointF ptPosConnectionPoint2New(300.0, 400.0);
    strMthArgs = qPoint2Str(ptPosConnectionPoint2New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt1CnctPt2]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt3CnctPt2]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto connection point, press mouse to select connection point, move the connection point while
    // mouse is pressed to the new position and release the mouse event.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosConnectionPoint2.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosConnectionPoint2.toPoint(), ptPosConnectionPoint2New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosConnectionPoint2 = ptPosConnectionPoint2New;
    m_pPhysValConnectionPoint2->setX(ptPosConnectionPoint2New.x());
    m_pPhysValConnectionPoint2->setY(bYAxisTopDown ? ptPosConnectionPoint2New.y() : fYAxisMaxVal - ptPosConnectionPoint2New.y());
    m_polygonConnectionLineCnctPt1CnctPt2 = QPolygonF({
        {0.0, -100.0},
        {0.0,  100.0}
    });
    m_polygonConnectionLineCnctPt3CnctPt2 = QPolygonF({
        { 150.0, -75.0},
        {-150.0,  75.0}
    });
    *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2 = QPolygonF({
        {m_pPhysValConnectionPoint1->toQPointF()},
        {m_pPhysValConnectionPoint2->toQPointF()}
    });
    *m_pPhysValPolygonConnectionLineCnctPt3CnctPt2 = QPolygonF({
        {m_pPhysValConnectionPoint3->toQPointF()},
        {m_pPhysValConnectionPoint2->toQPointF()}
    });
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionPoint(
        strGraphObjName, m_ptPosConnectionPoint2, *m_pPhysValConnectionPoint2, true, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt2, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt1CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, false, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt3CnctPt2, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt3CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt2, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // move CntPt3
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionPoint);
    strGraphObjName = c_strGraphObjNameConnectionPoint3;
    strMethod = "move";
    QPointF ptPosConnectionPoint3New(500.0, 200.0);
    strMthArgs = qPoint2Str(ptPosConnectionPoint3New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt3CnctPt4]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt3CnctPt2]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto connection point, press mouse to select connection point, move the connection point while
    // mouse is pressed to the new position and release the mouse event.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(640, 250), m_ptPosConnectionPoint3.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosConnectionPoint3.toPoint(), ptPosConnectionPoint3New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosConnectionPoint3 = ptPosConnectionPoint3New;
    m_pPhysValConnectionPoint3->setX(ptPosConnectionPoint3New.x());
    m_pPhysValConnectionPoint3->setY(bYAxisTopDown ? ptPosConnectionPoint3New.y() : fYAxisMaxVal - ptPosConnectionPoint3New.y());
    m_polygonConnectionLineCnctPt3CnctPt4 = QPolygonF({
        { -60.0, -75.0},
        {  60.0,  -5.0},
        {  50.0,  25.0},
        {  60.0,  55.0},
        {  40.0,  75.0}
    });
    m_polygonConnectionLineCnctPt3CnctPt2 = QPolygonF({
        { 100.0, -100.0},
        {-100.0,  100.0}
    });
    *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4 = QPolygonF({
        {m_pPhysValConnectionPoint3->toQPointF()},
        {QPointF(620.0, bYAxisTopDown ? 270.0 : 330.0)},
        {QPointF(610.0, bYAxisTopDown ? 300.0 : 300.0)},
        {QPointF(620.0, bYAxisTopDown ? 330.0 : 270.0)},
        {m_pPhysValConnectionPoint4->toQPointF()}
    });
    *m_pPhysValPolygonConnectionLineCnctPt3CnctPt2 = QPolygonF({
        {m_pPhysValConnectionPoint3->toQPointF()},
        {m_pPhysValConnectionPoint2->toQPointF()}
    });
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionPoint(
        strGraphObjName, m_ptPosConnectionPoint3, *m_pPhysValConnectionPoint3, true, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt3CnctPt4, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt3CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4, false, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt3CnctPt2, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt3CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt2, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // move CntPt4
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionPoint);
    strGraphObjName = c_strGraphObjNameConnectionPoint4;
    strMethod = "move";
    QPointF ptPosConnectionPoint4New(500.0, 400.0);
    strMthArgs = qPoint2Str(ptPosConnectionPoint4New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt3CnctPt4]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt1CnctPt4]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto connection point, press mouse to select connection point, move the connection point while
    // mouse is pressed to the new position and release the mouse event.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(640, 350), m_ptPosConnectionPoint4.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosConnectionPoint4.toPoint(), ptPosConnectionPoint4New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosConnectionPoint4 = ptPosConnectionPoint4New;
    m_pPhysValConnectionPoint4->setX(ptPosConnectionPoint4New.x());
    m_pPhysValConnectionPoint4->setY(bYAxisTopDown ? ptPosConnectionPoint4New.y() : fYAxisMaxVal - ptPosConnectionPoint4New.y());
    m_polygonConnectionLineCnctPt3CnctPt4 = QPolygonF({
        { -60.0, -100.0},
        {  60.0,  -30.0},
        {  50.0,    0.0},
        {  60.0,   30.0},
        { -60.0,  100.0}
    });
    m_polygonConnectionLineCnctPt1CnctPt4 = QPolygonF({
        {-100.0, -100.0},
        { 100.0,  100.0}
    });
    *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4 = QPolygonF({
        {m_pPhysValConnectionPoint3->toQPointF()},
        {QPointF(620.0, bYAxisTopDown ? 270.0 : 330.0)},
        {QPointF(610.0, bYAxisTopDown ? 300.0 : 300.0)},
        {QPointF(620.0, bYAxisTopDown ? 330.0 : 270.0)},
        {m_pPhysValConnectionPoint4->toQPointF()}
    });
    *m_pPhysValPolygonConnectionLineCnctPt1CnctPt4 = QPolygonF({
        {m_pPhysValConnectionPoint1->toQPointF()},
        {m_pPhysValConnectionPoint4->toQPointF()}
    });
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionPoint(
        strGraphObjName, m_ptPosConnectionPoint4, *m_pPhysValConnectionPoint4, true, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt3CnctPt4, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt3CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4, false, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt4, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt1CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt4, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // CnctLinePt1Pt2.insert(1, )
    //---------------------------

    idxPt = 1;
    physValPoint.setX(280.0);
    physValPoint.setY(bYAxisTopDown ? 250.0 : fYAxisMaxVal - 250.0);
    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt1CnctPt2;
    strMethod = "insert";
    strMthArgs = QString::number(idxPt) + ", {" + physValPoint.toString() + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse to line segment between first and second polygon point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, QPoint(300, 250));
    pTestStep->addDataRow({ // Click on line segment to create new polygon point
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    // Move newly created polygon point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, QPoint(280, 250), 0, Qt::LeftButton, Qt::ControlModifier);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    m_polygonConnectionLineCnctPt1CnctPt2 = QPolygonF({
        { 10.0, -100.0},
        {-10.0,  -50.0},
        { 10.0,  100.0}
    });
    m_pPhysValPolygonConnectionLineCnctPt1CnctPt2->insert(idxPt, physValPoint);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt2, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt1CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, true, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // CnctLinePt1Pt2.insert(2, )
    //---------------------------

    idxPt = 2;
    physValPoint.setX(280.0);
    physValPoint.setY(bYAxisTopDown ? 350.0 : fYAxisMaxVal - 350.0);
    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt1CnctPt2;
    strMethod = "insert";
    strMthArgs = QString::number(idxPt) + ", {" + physValPoint.toString() + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse to line segment between second and last polygon point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, QPoint(293, 350));
    pTestStep->addDataRow({ // Click on line segment to create new polygon point
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    // Move newly created polygon point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, QPoint(280, 350), 0, Qt::LeftButton, Qt::ControlModifier);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    m_polygonConnectionLineCnctPt1CnctPt2 = QPolygonF({
        { 10.0, -100.0},
        {-10.0,  -50.0},
        {-10.0,   50.0},
        { 10.0,  100.0}
    });
    m_pPhysValPolygonConnectionLineCnctPt1CnctPt2->insert(idxPt, physValPoint);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt2, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt1CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, true, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // CnctLinePt1Pt2.insert(2, )
    //---------------------------

    idxPt = 2;
    physValPoint.setX(290.0);
    physValPoint.setY(bYAxisTopDown ? 300.0 : fYAxisMaxVal - 300.0);
    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt1CnctPt2;
    strMethod = "insert";
    strMthArgs = QString::number(idxPt) + ", {" + physValPoint.toString() + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse to line segment between second and third polygon point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, QPoint(280, 300));
    pTestStep->addDataRow({ // Click on line segment to create new polygon point
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    // Move newly created polygon point
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, QPoint(290, 300), 0, Qt::LeftButton, Qt::ControlModifier);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    m_polygonConnectionLineCnctPt1CnctPt2 = QPolygonF({
        { 10.0, -100.0},
        {-10.0,  -50.0},
        {  0.0,    0.0},
        {-10.0,   50.0},
        { 10.0,  100.0}
    });
    m_pPhysValPolygonConnectionLineCnctPt1CnctPt2->insert(idxPt, physValPoint);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt2, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt1CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, true, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // CnctLinePt1Pt2.hoverLeave()
    //----------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt1CnctPt2;
    strMethod = "hoverLeave";
    ptMousePos = QPoint(310, 300);
    strMthArgs = "{" + qPoint2Str(m_ptMousePos) + " -> " + qPoint2Str(ptMousePos) + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse before line
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, ptMousePos, 20);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt2, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt1CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, true, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // CnctLinePt1Pt2.unselect()
    //---------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt1CnctPt2;
    strMethod = "unselect";
    strMthArgs = "{" + qPoint2Str(m_ptMousePos) + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    pTestStep->addDataRow({ // Click within bounding rectangle of connection line, but not on the line segments
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos}
    });
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos}
    });
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt2, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt1CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // mouseMove
    //----------

    strMethod = "mouseMove";
    ptMousePos = QPoint(310, 275);
    strMthArgs = "{" + qPoint2Str(m_ptMousePos) + " -> " + qPoint2Str(ptMousePos) + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse before line
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, ptMousePos);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt2, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt1CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // mouseMove (CnctLinePt1Pt2.hoverEnter, hoverMove, hoverLeave)
    //-------------------------------------------------------------

    strMethod = "mouseMove";
    ptMousePos = QPoint(270, 275);
    strMthArgs = "{" + qPoint2Str(m_ptMousePos) + " -> " + qPoint2Str(ptMousePos) + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse before line
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, ptMousePos, 40);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt2, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt1CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // mouseMove (CnctLinePt1Pt2.hoverEnter, hoverMove, hoverLeave)
    //-------------------------------------------------------------

    strMethod = "mouseMove";
    ptMousePos = QPoint(310, 275);
    strMthArgs = "{" + qPoint2Str(m_ptMousePos) + " -> " + qPoint2Str(ptMousePos) + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse before line
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, ptMousePos, 40);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt2, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt1CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // CnctLinePt1Pt4.select()
    //------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt1CnctPt4;
    strMethod = "select";
    ptMousePos = QPoint(360, 260);
    strMthArgs = "{" + qPoint2Str(ptMousePos) + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto line
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, ptMousePos);
    pTestStep->addDataRow({ // Click on line segment to select the connection line
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt4, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt1CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt4, true, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // CnctLinePt1Pt4.unselect()
    //---------------------------

    // Click within the bounding rectangle of the connection line CnctLinePt3Pt4.
    // CnctLinePt1Pt4 should be unselected, CnctLinePt3Pt4 shound NOT be selected.
    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt1CnctPt4;
    strMethod = "unselect";
    ptMousePos = QPoint(380, 300);
    strMthArgs = "{" + qPoint2Str(ptMousePos) + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse into bounding rectangle of CnctLinePt3Pt4
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, ptMousePos);
    pTestStep->addDataRow({ // Click within bounding rectangle of connection line, but not on the line segments
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos}
    });
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos}
    });
    pTestStep->addDataRow({ // Click again and move mouse.
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, QPoint(580, 300));
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos}
    });
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt4, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt1CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt4, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // mouseMove
    //----------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt3CnctPt4;
    strMethod = "mouseMove";
    ptMousePos = QPoint(550, 300);
    strMthArgs = "{" + qPoint2Str(m_ptMousePos) + " -> " + qPoint2Str(ptMousePos) + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse into bounding rectangle of ConnectionLineCnctPt3CnctPt4
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, ptMousePos);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt3CnctPt4, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt3CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // mouseClick
    //-----------

    strMethod = "mouseClick";
    strMthArgs = "{" + qPoint2Str(m_ptMousePos) + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Click into bounding rectangle of CnctLinePt3Pt4 (but line should not be selected)
    pTestStep->addDataRow({ // Click within bounding rectangle of connection line, but not on the line segments
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos}
    });
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos}
    });
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt3CnctPt4, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt3CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // mousePressAndMove
    //------------------

    strMethod = "mousePressMouseMoveMouseRelease";
    strMthArgs = "{" + qPoint2Str(m_ptMousePos) + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Click into bounding rectangle of CnctLinePt3Pt4 and move mouse (line should not be selected and not moved)
    pTestStep->addDataRow({
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, QPoint(580, 300), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos}
    });
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt3CnctPt4, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt3CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // CnctLinePt3Pt4.select()
    //------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt3CnctPt4;
    strMethod = "select";
    ptMousePos = QPoint(540, 223);
    strMthArgs = "{" + qPoint2Str(ptMousePos) + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto line
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, ptMousePos);
    pTestStep->addDataRow({ // Click on line segment to select the connection line
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"KeyboardModifiers", static_cast<int>(Qt::ControlModifier)}
    });
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt3CnctPt4, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt3CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4, true, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // mouseMove
    //----------

    // Move mouse to line start point at connection point.
    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt3CnctPt4;
    strMethod = "mouseMove";
    ptMousePos = QPoint(502, 202);
    strMthArgs = "{" + qPoint2Str(m_ptMousePos) + " -> " + qPoint2Str(ptMousePos) + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, ptMousePos);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt3CnctPt4, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt3CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4, true, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // move CnctPt3
    //-------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionPoint);
    strGraphObjName = c_strGraphObjNameConnectionPoint3;
    strMethod = "move";
    ptPosConnectionPoint3New = QPointF(542, 202);
    strMthArgs = "{" + qPoint2Str(m_ptMousePos) + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt3CnctPt4]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineCnctPt3CnctPt2]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Mouse press at the line start point.
    pTestStep->addDataRow({
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    // Move mouse pos. The connection point should be moved.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, ptPosConnectionPoint3New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosConnectionPoint3 = QPointF(540, 200);
    m_pPhysValConnectionPoint3->setX(m_ptPosConnectionPoint3.x());
    m_pPhysValConnectionPoint3->setY(bYAxisTopDown ? m_ptPosConnectionPoint3.y() : fYAxisMaxVal - m_ptPosConnectionPoint3.y());
    m_polygonConnectionLineCnctPt3CnctPt4 = QPolygonF({
        { -20.0, -100.0},
        {  60.0,  -30.0},
        {  50.0,    0.0},
        {  60.0,   30.0},
        { -60.0,  100.0}
    });
    m_polygonConnectionLineCnctPt3CnctPt2 = QPolygonF({
        { 120.0, -100.0},
        {-120.0,  100.0}
    });
    *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4 = QPolygonF({
        {m_pPhysValConnectionPoint3->toQPointF()},
        {QPointF(620.0, bYAxisTopDown ? 270.0 : 330.0)},
        {QPointF(610.0, bYAxisTopDown ? 300.0 : 300.0)},
        {QPointF(620.0, bYAxisTopDown ? 330.0 : 270.0)},
        {m_pPhysValConnectionPoint4->toQPointF()}
    });
    *m_pPhysValPolygonConnectionLineCnctPt3CnctPt2 = QPolygonF({
        {m_pPhysValConnectionPoint3->toQPointF()},
        {m_pPhysValConnectionPoint2->toQPointF()}
    });
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionPoint(
        strGraphObjName, m_ptPosConnectionPoint3, *m_pPhysValConnectionPoint3, true, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt3CnctPt4, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt3CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4, false, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt3CnctPt2, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt3CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt2, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsAnchoredToLines(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsAnchoredToLines",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameConnections;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strGraphObjFactoryGroup = CObjFactory::c_strGroupNameConnections;
    QString strGraphObjKeyInTree;
    QString strGraphObjType;
    QString strGraphObjName;
    QString strGraphObjKeyInTreeCnctPt1;
    QString strGraphObjCnctPt1Name;
    QPointF ptCnctPt1;
    QString strGraphObjKeyInTreeCnctPt2;
    QString strGraphObjCnctPt2Name;
    QPointF ptCnctPt2;
    QString strMethod;
    QString strMthArgs;
    QStringList strlstExpectedValues;

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpDrawConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;

    // Line 1
    //-------

    strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    strGraphObjName = c_strGraphObjNameLine1;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    QPoint ptP1Line1(175, 225);
    QPoint ptP2Line1(225, 275);
    m_ptPosLine1 = QPointF((ptP2Line1.x() + ptP1Line1.x())/2, (ptP2Line1.y() + ptP1Line1.y())/2);
    m_lineLine1 = QLineF(QPointF(ptP1Line1.x() - m_ptPosLine1.x(), ptP1Line1.y() - m_ptPosLine1.y()),
                         QPointF(ptP2Line1.x() - m_ptPosLine1.x(), ptP2Line1.y() - m_ptPosLine1.y()));
    m_pPhysValLine1->setP1(QPointF(ptP1Line1.x(), bYAxisTopDown ? ptP1Line1.y() : fYAxisMaxVal - ptP1Line1.y()));
    m_pPhysValLine1->setP2(QPointF(ptP2Line1.x(), bYAxisTopDown ? ptP2Line1.y() : fYAxisMaxVal - ptP2Line1.y()));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", ptP1Line1);
    pTestStep->setConfigValue("P2", ptP2Line1);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        strGraphObjName, m_ptPosLine1, m_lineLine1, *m_pPhysValLine1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line 2
    //-------

    strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    strGraphObjName = c_strGraphObjNameLine2;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    QPoint ptP1Line2(175, 375);
    QPoint ptP2Line2(225, 325);
    m_ptPosLine2 = QPointF((ptP2Line2.x() + ptP1Line2.x())/2, (ptP2Line2.y() + ptP1Line2.y())/2);
    m_lineLine2 = QLineF(QPointF(ptP1Line2.x() - m_ptPosLine2.x(), ptP1Line2.y() - m_ptPosLine2.y()),
                         QPointF(ptP2Line2.x() - m_ptPosLine2.x(), ptP2Line2.y() - m_ptPosLine2.y()));
    m_pPhysValLine2->setP1(QPointF(ptP1Line2.x(), bYAxisTopDown ? ptP1Line2.y() : fYAxisMaxVal - ptP1Line2.y()));
    m_pPhysValLine2->setP2(QPointF(ptP2Line2.x(), bYAxisTopDown ? ptP2Line2.y() : fYAxisMaxVal - ptP2Line2.y()));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", ptP1Line2);
    pTestStep->setConfigValue("P2", ptP2Line2);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        strGraphObjName, m_ptPosLine2, m_lineLine2, *m_pPhysValLine2, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line 3
    //-------

    strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    strGraphObjName = c_strGraphObjNameLine3;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    QPoint ptP1Line3(575, 275);
    QPoint ptP2Line3(625, 225);
    m_ptPosLine3 = QPointF((ptP2Line3.x() + ptP1Line3.x())/2, (ptP2Line3.y() + ptP1Line3.y())/2);
    m_lineLine3 = QLineF(QPointF(ptP1Line3.x() - m_ptPosLine3.x(), ptP1Line3.y() - m_ptPosLine3.y()),
                         QPointF(ptP2Line3.x() - m_ptPosLine3.x(), ptP2Line3.y() - m_ptPosLine3.y()));
    m_pPhysValLine3->setP1(QPointF(ptP1Line3.x(), bYAxisTopDown ? ptP1Line3.y() : fYAxisMaxVal - ptP1Line3.y()));
    m_pPhysValLine3->setP2(QPointF(ptP2Line3.x(), bYAxisTopDown ? ptP2Line3.y() : fYAxisMaxVal - ptP2Line3.y()));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", ptP1Line3);
    pTestStep->setConfigValue("P2", ptP2Line3);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        strGraphObjName, m_ptPosLine3, m_lineLine3, *m_pPhysValLine3, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line 4
    //-------

    strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    strGraphObjName = c_strGraphObjNameLine4;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    QPoint ptP1Line4(575, 325);
    QPoint ptP2Line4(625, 375);
    m_ptPosLine4 = QPointF((ptP2Line4.x() + ptP1Line4.x())/2, (ptP2Line4.y() + ptP1Line4.y())/2);
    m_lineLine4 = QLineF(QPointF(ptP1Line4.x() - m_ptPosLine4.x(), ptP1Line4.y() - m_ptPosLine4.y()),
                         QPointF(ptP2Line4.x() - m_ptPosLine4.x(), ptP2Line4.y() - m_ptPosLine4.y()));
    m_pPhysValLine4->setP1(QPointF(ptP1Line4.x(), bYAxisTopDown ? ptP1Line4.y() : fYAxisMaxVal - ptP1Line4.y()));
    m_pPhysValLine4->setP2(QPointF(ptP2Line4.x(), bYAxisTopDown ? ptP2Line4.y() : fYAxisMaxVal - ptP2Line4.y()));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", ptP1Line4);
    pTestStep->setConfigValue("P2", ptP2Line4);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        strGraphObjName, m_ptPosLine4, m_lineLine4, *m_pPhysValLine4, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Line 1 - Pt2 -> Line 2 - Pt2
    //----------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineLine1Pt2Line2Pt2;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    strGraphObjCnctPt1Name = "CnctPoint-P1-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine1, strGraphObjCnctPt1Name);
    ptCnctPt1 = m_pPhysValLine1->p2().toQPointF();
    strGraphObjCnctPt2Name = "CnctPoint-P1-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine2, strGraphObjCnctPt2Name);
    ptCnctPt2 = m_pPhysValLine2->p2().toQPointF();
    m_polygonConnectionLineLine1P2Line2P2 = QPolygonF({
        {QPointF(0.0, -25.0)},
        {QPointF(0.0,  25.0)}
    });
    *m_pPhysValPolygonConnectionLineLine1P2Line2P2 = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    QPolygon points({
        QPoint(ptCnctPt1.x(), bYAxisTopDown ? ptCnctPt1.y() - 25 : fYAxisMaxVal - ptCnctPt1.y() - 25),
        QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    });
    pTestStep->setConfigValue("GraphObjFactoryGroup", CObjFactory::c_strGroupNameConnections);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineLine1P2Line2P2, *m_pPhysValPolygonConnectionLineLine1P2Line2P2,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Line 1 - P2 -> Line 2 - P2: setLineEndStyle(EndPoint, Arrow)
    //------------------------------------------------------------------------------

    strGraphObjName = c_strGraphObjNameConnectionLineLine1Pt2Line2Pt2;
    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setPenColor"},
        {"PenColor", "red"}
    });
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: Line 2 - Pt1 -> Line 4 - Pt2
    //----------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineLine2Pt1Line4Pt2;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    strGraphObjCnctPt1Name = "CnctPoint-P0-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine2, strGraphObjCnctPt1Name);
    ptCnctPt1 = m_pPhysValLine2->p1().toQPointF();
    strGraphObjCnctPt2Name = "CnctPoint-P1-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine4, strGraphObjCnctPt2Name);
    ptCnctPt2 = m_pPhysValLine4->p2().toQPointF();
    m_polygonConnectionLineLine2P1Line4P2 = QPolygonF({
        {QPointF(-225.0, 0.0)},
        {QPointF( 225.0, 0.0)}
    });
    *m_pPhysValPolygonConnectionLineLine2P1Line4P2 = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(ptCnctPt1.x(), bYAxisTopDown ? ptCnctPt1.y() + 25 : fYAxisMaxVal - ptCnctPt1.y() + 25),
        QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    });
    pTestStep->setConfigValue("GraphObjFactoryGroup", CObjFactory::c_strGroupNameConnections);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineLine2P1Line4P2, *m_pPhysValPolygonConnectionLineLine2P1Line4P2,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Line 2 - P1 -> Line 4 - P2: setLineEndStyle(EndPoint, Arrow)
    //------------------------------------------------------------------------------

    strGraphObjName = c_strGraphObjNameConnectionLineLine2Pt1Line4Pt2;
    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setPenColor"},
        {"PenColor", "red"}
    });
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: Line 4 - Pt1 -> Line 3 - Pt1
    //----------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineLine4Pt1Line3Pt1;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    strGraphObjCnctPt1Name = "CnctPoint-P0-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine1, strGraphObjCnctPt1Name);
    ptCnctPt1 = m_pPhysValLine4->p1().toQPointF();
    strGraphObjCnctPt2Name = "CnctPoint-P0-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine2, strGraphObjCnctPt2Name);
    ptCnctPt2 = m_pPhysValLine3->p1().toQPointF();
    m_polygonConnectionLineLine4P1Line3P1 = QPolygonF({
        {QPointF(0.0,  25.0)},
        {QPointF(0.0, -25.0)}
    });
    *m_pPhysValPolygonConnectionLineLine4P1Line3P1 = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(ptCnctPt1.x(), bYAxisTopDown ? ptCnctPt1.y() + 25 : fYAxisMaxVal - ptCnctPt1.y() + 25),
        QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    });
    pTestStep->setConfigValue("GraphObjFactoryGroup", CObjFactory::c_strGroupNameConnections);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineLine4P1Line3P1, *m_pPhysValPolygonConnectionLineLine4P1Line3P1,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Line 4 - Pt1 -> Line 3 - Pt1: setLineEndStyle(EndPoint, Arrow)
    //--------------------------------------------------------------------------------

    strGraphObjName = c_strGraphObjNameConnectionLineLine4Pt1Line3Pt1;
    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setPenColor"},
        {"PenColor", "red"}
    });
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: Line 3 - Pt2 -> Line 1 - Pt1
    //----------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineLine3Pt2Line1Pt1;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    strGraphObjCnctPt1Name = "CnctPoint-P1-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine2, strGraphObjCnctPt1Name);
    ptCnctPt1 = m_pPhysValLine3->p2().toQPointF();
    strGraphObjCnctPt2Name = "CnctPoint-P0-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine4, strGraphObjCnctPt2Name);
    ptCnctPt2 = m_pPhysValLine1->p1().toQPointF();
    m_polygonConnectionLineLine3P2Line1P1 = QPolygonF({
        {QPointF( 225.0, 0.0)},
        {QPointF(-225.0, 0.0)}
    });
    *m_pPhysValPolygonConnectionLineLine3P2Line1P1 = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(ptCnctPt1.x(), bYAxisTopDown ? ptCnctPt1.y() - 25 : fYAxisMaxVal - ptCnctPt1.y() - 25),
        QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    });
    pTestStep->setConfigValue("GraphObjFactoryGroup", CObjFactory::c_strGroupNameConnections);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineLine3P2Line1P1, *m_pPhysValPolygonConnectionLineLine3P2Line1P1,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Line 3 - Pt2 -> Line 1 - Pt1: setLineEndStyle(EndPoint, Arrow)
    //--------------------------------------------------------------------------------

    strGraphObjName = c_strGraphObjNameConnectionLineLine3Pt2Line1Pt1;
    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setPenColor"},
        {"PenColor", "red"}
    });
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Cncts.showLabels()",
        /* strOperation    */ "Cncts.showLabels(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine1);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedText", c_strGraphObjNameLine1},
        {"ExpectedPos", QPointF(183.5, 242.0)}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP1},
        {"setPos", QPointF(158.5, 207.0)},
        {"ExpectedText", "P1"},
        {"ExpectedPos", QPointF(158.5, 207.0)}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP2},
        {"setPos", QPointF(228.5, 277.0)},
        {"ExpectedText", "P2"},
        {"ExpectedPos", QPointF(228.5, 277.0)}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine2);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine2},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedText", c_strGraphObjNameLine2},
        {"ExpectedPos", QPointF(183.5, 342.0)}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine2},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP1},
        {"setPos", QPointF(158.5, 377.0)},
        {"ExpectedText", "P1"},
        {"ExpectedPos", QPointF(158.5, 377.0)}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine2},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP2},
        {"setPos", QPointF(228.5, 307.0)},
        {"ExpectedText", "P2"},
        {"ExpectedPos", QPointF(228.5, 307.0)}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine3);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine3},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedText", c_strGraphObjNameLine3},
        {"ExpectedPos", QPointF(583.5, 242.0)}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine3},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP1},
        {"setPos", QPointF(558.5, 277.0)},
        {"ExpectedText", "P1"},
        {"ExpectedPos", QPointF(558.5, 277.0)}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine3},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP2},
        {"setPos", QPointF(628.5, 207.0)},
        {"ExpectedText", "P2"},
        {"ExpectedPos", QPointF(628.5, 207.0)}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine4);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine4},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedText", c_strGraphObjNameLine4},
        {"ExpectedPos", QPointF(583.5, 342.0)}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine4},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP1},
        {"setPos", QPointF(558.5, 307.0)},
        {"ExpectedText", "P1"},
        {"ExpectedPos", QPointF(558.5, 307.0)}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine4},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP2},
        {"setPos", QPointF(628.5, 377.0)},
        {"ExpectedText", "P2"},
        {"ExpectedPos", QPointF(628.5, 377.0)}
    });
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsAnchoredToLinesModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsAnchoredToLinesModifications",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpModifyConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;

    QString strGraphObjType;
    QString strFactoryGroupName;
    QString strGraphObjName;
    QString strGraphObjKeyInTree;
    QString strGraphObjKeyInTreeCnctPt1;
    QString strGraphObjCnctPt1Name;
    QString strGraphObjKeyInTreeCnctPt2;
    QString strGraphObjCnctPt2Name;
    QString strMethod;
    QString strMthArgs;

    // Hide Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Cncts.hideLabels()",
        /* strOperation    */ "Cncts.hideLabels(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepHideLabels(ZS::Test::CTestStep*)) );
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine1);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameLine1 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP1},
        {"ExpectedValue", c_strGraphObjNameLine1 + "." + CGraphObj::c_strGeometryLabelNameP1 + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP2},
        {"ExpectedValue", c_strGraphObjNameLine1 + "." + CGraphObj::c_strGeometryLabelNameP2 + " not found"}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine2);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine2},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameLine2 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine2},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP1},
        {"ExpectedValue", c_strGraphObjNameLine2 + "." + CGraphObj::c_strGeometryLabelNameP1 + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine2},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP2},
        {"ExpectedValue", c_strGraphObjNameLine2 + "." + CGraphObj::c_strGeometryLabelNameP2 + " not found"}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine3);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine3},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameLine3 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine3},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP1},
        {"ExpectedValue", c_strGraphObjNameLine3 + "." + CGraphObj::c_strGeometryLabelNameP1 + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine3},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP2},
        {"ExpectedValue", c_strGraphObjNameLine3 + "." + CGraphObj::c_strGeometryLabelNameP2 + " not found"}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine4);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine4},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameLine4 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine4},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP1},
        {"ExpectedValue", c_strGraphObjNameLine4 + "." + CGraphObj::c_strGeometryLabelNameP1 + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameLine4},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP2},
        {"ExpectedValue", c_strGraphObjNameLine4 + "." + CGraphObj::c_strGeometryLabelNameP2 + " not found"}
    });

    // move Line 1 - P1
    //-----------------

    strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    strGraphObjName = c_strGraphObjNameLine1;
    strMethod = "setP1";
    QPointF ptPosLine1P1New(275.0, 175.0);
    strMthArgs = qPoint2Str(ptPosLine1P1New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineLine1Pt2Line2Pt2]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineLine3Pt2Line1Pt1]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto line and press mouse to select line.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 250), m_ptPosLine1.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    // Move mouse to P1 of line, press mouse to select P1 and move P1 of the line to the new position
    // while the mouse is pressed and release the mouse.
    QPointF ptPosLine1Pt1(m_ptPosLine1.x() - m_lineLine1.dx()/2.0, m_ptPosLine1.y() - m_lineLine1.dy()/2.0);
    QPointF ptPosLine1Pt2(m_ptPosLine1.x() + m_lineLine1.dx()/2.0, m_ptPosLine1.y() + m_lineLine1.dy()/2.0);
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, ptPosLine1Pt1.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, ptPosLine1P1New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    ptPosLine1Pt1 = ptPosLine1P1New;
    m_pPhysValLine1->setP1(QPointF(ptPosLine1P1New.x(), bYAxisTopDown ? ptPosLine1P1New.y() : fYAxisMaxVal - ptPosLine1P1New.y()));
    QPointF ptLine1Center = m_pPhysValLine1->center().toQPointF();
    m_ptPosLine1 = QPointF(ptLine1Center.x(), bYAxisTopDown ? ptLine1Center.y() : fYAxisMaxVal - ptLine1Center.y());
    m_lineLine1.setP1(QPointF(25.0, -50.0));
    m_lineLine1.setP2(QPointF(-25.0, 50.0));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        strGraphObjName, m_ptPosLine1, m_lineLine1, *m_pPhysValLine1, true, iResultValuesPrecision));
    strGraphObjCnctPt1Name = "CnctPoint-P1-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine1, strGraphObjCnctPt1Name);
    strGraphObjCnctPt2Name = "CnctPoint-P1-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine2, strGraphObjCnctPt2Name);
    m_polygonConnectionLineLine1P2Line2P2 = QPolygonF({
        {QPointF(0.0, -25.0)},
        {QPointF(0.0,  25.0)}
    });
    *m_pPhysValPolygonConnectionLineLine1P2Line2P2 = QPolygonF({
        {m_pPhysValLine1->p2().toQPointF()},
        {m_pPhysValLine2->p2().toQPointF()}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineLine1Pt2Line2Pt2,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineLine1P2Line2P2,
        *m_pPhysValPolygonConnectionLineLine1P2Line2P2,
        false, iResultValuesPrecision));
    strGraphObjCnctPt1Name = "CnctPoint-P1-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine3, strGraphObjCnctPt1Name);
    strGraphObjCnctPt2Name = "CnctPoint-P0-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine1, strGraphObjCnctPt2Name);
    m_polygonConnectionLineLine3P2Line1P1 = QPolygonF({
        {QPointF( 175.0,  25.0)},
        {QPointF(-175.0, -25.0)}
    });
    *m_pPhysValPolygonConnectionLineLine3P2Line1P1 = QPolygonF({
        {m_pPhysValLine3->p2().toQPointF()},
        {m_pPhysValLine1->p1().toQPointF()}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineLine3Pt2Line1Pt1,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineLine3P2Line1P1,
        *m_pPhysValPolygonConnectionLineLine3P2Line1P1,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // move Line 1 - P2
    //-----------------

    strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    strGraphObjName = c_strGraphObjNameLine1;
    strMethod = "setP2";
    QPointF ptPosLine1P2New(325.0, 225.0);
    strMthArgs = qPoint2Str(ptPosLine1P2New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineLine1Pt2Line2Pt2]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineLine3Pt2Line1Pt1]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto line and press mouse to select line.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(220, 220), m_ptPosLine1.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    // Move mouse to P2 of line, press mouse to select P2 and move P2 of the line to the new position
    // while the mouse is pressed and release the mouse.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, ptPosLine1Pt2.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, ptPosLine1P2New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    ptPosLine1Pt2 = ptPosLine1P2New;
    m_pPhysValLine1->setP2(QPointF(ptPosLine1P2New.x(), bYAxisTopDown ? ptPosLine1P2New.y() : fYAxisMaxVal - ptPosLine1P2New.y()));
    ptLine1Center = m_pPhysValLine1->center().toQPointF();
    m_ptPosLine1 = QPointF(ptLine1Center.x(), bYAxisTopDown ? ptLine1Center.y() : fYAxisMaxVal - ptLine1Center.y());
    m_lineLine1.setP1(QPointF(-25.0, -25.0));
    m_lineLine1.setP2(QPointF(25.0, 25.0));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        strGraphObjName, m_ptPosLine1, m_lineLine1, *m_pPhysValLine1, true, iResultValuesPrecision));
    strGraphObjCnctPt1Name = "CnctPoint-P1-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine1, strGraphObjCnctPt1Name);
    strGraphObjCnctPt2Name = "CnctPoint-P1-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine2, strGraphObjCnctPt2Name);
    m_polygonConnectionLineLine1P2Line2P2 = QPolygonF({
        {QPointF( 50.0, -50.0)},
        {QPointF(-50.0,  50.0)}
    });
    *m_pPhysValPolygonConnectionLineLine1P2Line2P2 = QPolygonF({
        {m_pPhysValLine1->p2().toQPointF()},
        {m_pPhysValLine2->p2().toQPointF()}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineLine1Pt2Line2Pt2,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineLine1P2Line2P2,
        *m_pPhysValPolygonConnectionLineLine1P2Line2P2,
        false, iResultValuesPrecision));
    strGraphObjCnctPt1Name = "CnctPoint-P1-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine3, strGraphObjCnctPt1Name);
    strGraphObjCnctPt2Name = "CnctPoint-P0-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine1, strGraphObjCnctPt2Name);
    m_polygonConnectionLineLine3P2Line1P1 = QPolygonF({
        {QPointF( 175.0,  25.0)},
        {QPointF(-175.0, -25.0)}
    });
    *m_pPhysValPolygonConnectionLineLine3P2Line1P1 = QPolygonF({
        {m_pPhysValLine3->p2().toQPointF()},
        {m_pPhysValLine1->p1().toQPointF()}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineLine3Pt2Line1Pt1,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineLine3P2Line1P1,
        *m_pPhysValPolygonConnectionLineLine3P2Line1P1,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // move Line 2
    //----------------

    strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    strGraphObjName = c_strGraphObjNameLine2;
    strMethod = "move";
    QPointF ptPosLine2New(300.0, 400.0);
    strMthArgs = qPoint2Str(ptPosLine2New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineLine1Pt2Line2Pt2]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineLine2Pt1Line4Pt2]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto line, press mouse to select line, move the line to the new position
    // while the mouse is pressed and release the mouse.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosLine2.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosLine2.toPoint(), ptPosLine2New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosLine2 = ptPosLine2New;
    m_pPhysValLine2->setCenter(QPointF(ptPosLine2New.x(), bYAxisTopDown ? ptPosLine2New.y() : fYAxisMaxVal - ptPosLine2New.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        strGraphObjName, m_ptPosLine2, m_lineLine2, *m_pPhysValLine2, true, iResultValuesPrecision));
    strGraphObjCnctPt1Name = "CnctPoint-P1-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine1, strGraphObjCnctPt1Name);
    strGraphObjCnctPt2Name = "CnctPoint-P1-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine2, strGraphObjCnctPt2Name);
    m_polygonConnectionLineLine1P2Line2P2 = QPolygonF({
        {QPointF(0.0, -75.0)},
        {QPointF(0.0,  75.0)}
    });
    *m_pPhysValPolygonConnectionLineLine1P2Line2P2 = QPolygonF({
        {m_pPhysValLine1->p2().toQPointF()},
        {m_pPhysValLine2->p2().toQPointF()}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineLine1Pt2Line2Pt2,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineLine1P2Line2P2,
        *m_pPhysValPolygonConnectionLineLine1P2Line2P2,
        false, iResultValuesPrecision));
    strGraphObjCnctPt1Name = "CnctPoint-P0-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine2, strGraphObjCnctPt1Name);
    strGraphObjCnctPt2Name = "CnctPoint-P1-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine4, strGraphObjCnctPt2Name);
    m_polygonConnectionLineLine2P1Line4P2 = QPolygonF({
        {QPointF(-175.0,  25.0)},
        {QPointF( 175.0, -25.0)}
    });
    *m_pPhysValPolygonConnectionLineLine2P1Line4P2 = QPolygonF({
        {m_pPhysValLine2->p1().toQPointF()},
        {m_pPhysValLine4->p2().toQPointF()}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineLine2Pt1Line4Pt2,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineLine2P1Line4P2,
        *m_pPhysValPolygonConnectionLineLine2P1Line4P2,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // move Line 3
    //----------------

    strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    strGraphObjName = c_strGraphObjNameLine3;
    strMethod = "move";
    QPointF ptPosLine3New(500.0, 200.0);
    strMthArgs = qPoint2Str(ptPosLine3New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineLine4Pt1Line3Pt1]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineLine3Pt2Line1Pt1]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto line, press mouse to select line, move the line to the new position
    // while the mouse is pressed and release the mouse.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(660, 250), m_ptPosLine3.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosLine3.toPoint(), ptPosLine3New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosLine3 = ptPosLine3New;
    m_pPhysValLine3->setCenter(QPointF(ptPosLine3New.x(), bYAxisTopDown ? ptPosLine3New.y() : fYAxisMaxVal - ptPosLine3New.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        strGraphObjName, m_ptPosLine3, m_lineLine3, *m_pPhysValLine3, true, iResultValuesPrecision));
    strGraphObjCnctPt1Name = "CnctPoint-P0-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine4, strGraphObjCnctPt1Name);
    strGraphObjCnctPt2Name = "CnctPoint-P0-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine3, strGraphObjCnctPt2Name);
    m_polygonConnectionLineLine4P1Line3P1 = QPolygonF({
        {QPointF( 50.0,  50.0)},
        {QPointF(-50.0, -50.0)}
    });
    *m_pPhysValPolygonConnectionLineLine4P1Line3P1 = QPolygonF({
        {m_pPhysValLine4->p1().toQPointF()},
        {m_pPhysValLine3->p1().toQPointF()}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineLine4Pt1Line3Pt1,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineLine4P1Line3P1,
        *m_pPhysValPolygonConnectionLineLine4P1Line3P1,
        false, iResultValuesPrecision));
    strGraphObjCnctPt1Name = "CnctPoint-P1-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine3, strGraphObjCnctPt1Name);
    strGraphObjCnctPt2Name = "CnctPoint-P0-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine1, strGraphObjCnctPt2Name);
    m_polygonConnectionLineLine3P2Line1P1 = QPolygonF({
        {QPointF( 125.0, 0.0)},
        {QPointF(-125.0, 0.0)}
    });
    *m_pPhysValPolygonConnectionLineLine3P2Line1P1 = QPolygonF({
        {m_pPhysValLine3->p2().toQPointF()},
        {m_pPhysValLine1->p1().toQPointF()}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineLine3Pt2Line1Pt1,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineLine3P2Line1P1,
        *m_pPhysValPolygonConnectionLineLine3P2Line1P1,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // move Line 4
    //----------------

    strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    strGraphObjName = c_strGraphObjNameLine4;
    strMethod = "move";
    QPointF ptPosLine4New(500.0, 400.0);
    strMthArgs = qPoint2Str(ptPosLine4New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineLine4Pt1Line3Pt1]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineLine3Pt2Line1Pt1]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto line, press mouse to select line, move the line to the new position
    // while the mouse is pressed and release the mouse.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(660, 350), m_ptPosLine4.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosLine4.toPoint(), ptPosLine4New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosLine4 = ptPosLine4New;
    m_pPhysValLine4->setCenter(QPointF(ptPosLine4New.x(), bYAxisTopDown ? ptPosLine4New.y() : fYAxisMaxVal - ptPosLine4New.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        strGraphObjName, m_ptPosLine4, m_lineLine4, *m_pPhysValLine4, true, iResultValuesPrecision));
    strGraphObjCnctPt1Name = "CnctPoint-P0-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine4, strGraphObjCnctPt1Name);
    strGraphObjCnctPt2Name = "CnctPoint-P0-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine3, strGraphObjCnctPt2Name);
    m_polygonConnectionLineLine4P1Line3P1 = QPolygonF({
        {QPointF(0.0,  75.0)},
        {QPointF(0.0, -75.0)}
    });
    *m_pPhysValPolygonConnectionLineLine4P1Line3P1 = QPolygonF({
        {m_pPhysValLine4->p1().toQPointF()},
        {m_pPhysValLine3->p1().toQPointF()}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineLine4Pt1Line3Pt1,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineLine4P1Line3P1,
        *m_pPhysValPolygonConnectionLineLine4P1Line3P1,
        false, iResultValuesPrecision));
    strGraphObjCnctPt1Name = "CnctPoint-P1-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine3, strGraphObjCnctPt1Name);
    strGraphObjCnctPt2Name = "CnctPoint-P0-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameLine1, strGraphObjCnctPt2Name);
    m_polygonConnectionLineLine3P2Line1P1 = QPolygonF({
        {QPointF( 125.0, 0.0)},
        {QPointF(-125.0, 0.0)}
    });
    *m_pPhysValPolygonConnectionLineLine3P2Line1P1 = QPolygonF({
        {m_pPhysValLine3->p2().toQPointF()},
        {m_pPhysValLine1->p1().toQPointF()}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineLine3Pt2Line1Pt1,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineLine3P2Line1P1,
        *m_pPhysValPolygonConnectionLineLine3P2Line1P1,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsAnchoredToRects(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsAnchoredToRects",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameConnections;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strGraphObjFactoryGroup = CObjFactory::c_strGroupNameConnections;
    QString strGraphObjKeyInTree;
    QString strGraphObjType;
    QString strGraphObjName;
    QString strGraphObjKeyInTreeCnctPt1;
    QString strGraphObjCnctPt1Name;
    CEnumSelectionPoint eSelPt1;
    QPointF ptCnctPt1;
    QString strGraphObjKeyInTreeCnctPt2;
    QString strGraphObjCnctPt2Name;
    CEnumSelectionPoint eSelPt2;
    QPointF ptCnctPt2;
    QString strMethod;
    QString strMthArgs;
    QStringList strlstExpectedValues;

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpDrawConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;

    // Rectangle 1
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    strGraphObjName = c_strGraphObjNameRect1;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", QPoint(175.0, 225.0));
    pTestStep->setConfigValue("P2", QPoint(225.0, 275.0));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosRect1 = QPointF(200.0, 250.0);
    QPointF ptCenterRect1(m_ptPosRect1.x(), bYAxisTopDown ? m_ptPosRect1.y() : fYAxisMaxVal - m_ptPosRect1.y());
    QSizeF sizeRect1(50.0, 50.0);
    m_rectRect1 = QRectF(QPointF(-sizeRect1.width()/2.0, -sizeRect1.height()/2.0), sizeRect1);
    m_pPhysValRect1->setSize(sizeRect1);
    m_pPhysValRect1->setCenter(ptCenterRect1);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strGraphObjName, m_ptPosRect1, m_rectRect1, *m_pPhysValRect1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rectangle 2
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    strGraphObjName = c_strGraphObjNameRect2;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", QPoint(175.0, 325.0));
    pTestStep->setConfigValue("P2", QPoint(225.0, 375.0));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosRect2 = QPointF(200.0, 350.0);
    QPointF ptCenterRect2(m_ptPosRect2.x(), bYAxisTopDown ? m_ptPosRect2.y() : fYAxisMaxVal - m_ptPosRect2.y());
    QSizeF sizeRect2(50.0, 50.0);
    m_rectRect2 = QRectF(QPointF(-sizeRect2.width()/2.0, -sizeRect2.height()/2.0), sizeRect2);
    m_pPhysValRect2->setSize(sizeRect2);
    m_pPhysValRect2->setCenter(ptCenterRect2);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strGraphObjName, m_ptPosRect2, m_rectRect2, *m_pPhysValRect2, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rectangle 3
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    strGraphObjName = c_strGraphObjNameRect3;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", QPoint(575.0, 225.0));
    pTestStep->setConfigValue("P2", QPoint(625.0, 275.0));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosRect3 = QPointF(600.0, 250.0);
    QPointF ptCenterRect3(m_ptPosRect3.x(), bYAxisTopDown ? m_ptPosRect3.y() : fYAxisMaxVal - m_ptPosRect3.y());
    QSizeF sizeRect3(50.0, 50.0);
    m_rectRect3 = QRectF(QPointF(-sizeRect3.width()/2.0, -sizeRect3.height()/2.0), sizeRect3);
    m_pPhysValRect3->setSize(sizeRect3);
    m_pPhysValRect3->setCenter(ptCenterRect3);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strGraphObjName, m_ptPosRect3, m_rectRect3, *m_pPhysValRect3, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rectangle 4
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    strGraphObjName = c_strGraphObjNameRect4;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", QPoint(575.0, 325.0));
    pTestStep->setConfigValue("P2", QPoint(625.0, 375.0));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosRect4 = QPointF(600.0, 350.0);
    QPointF ptCenterRect4(m_ptPosRect4.x(), bYAxisTopDown ? m_ptPosRect4.y() : fYAxisMaxVal - m_ptPosRect4.y());
    QSizeF sizeRect4(50.0, 50.0);
    m_rectRect4 = QRectF(QPointF(-sizeRect4.width()/2.0, -sizeRect4.height()/2.0), sizeRect4);
    m_pPhysValRect4->setSize(sizeRect4);
    m_pPhysValRect4->setCenter(ptCenterRect4);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strGraphObjName, m_ptPosRect4, m_rectRect4, *m_pPhysValRect4, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Rectangle 1 - BottomCenter -> Rectangle 2 - TopCenter
    //-----------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
        { 0.0, -25.0},
        { 0.0,  25.0}
    });
    *m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    QPolygon points({
        QPoint(200, 300),
        QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineRect1BottomCenterRect2TopCenter, *m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Rectangle 1 - BottomCenter -> Rectangle 2 - TopCenter: setLineEndStyle(EndPoint, Arrow)
    //---------------------------------------------------------------------------------------------------------

    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: Rectangle 1 - RightCenter -> Rectangle 4 - LeftCenter
    //-----------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
        { -175.0, -50.0},
        {  175.0,  50.0}
    });
    *m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(200, 300),
        QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineRect1RightCenterRect4LeftCenter, *m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Rectangle 1 - RightCenter -> Rectangle 4 - LeftCenter: setLineEndStyle(EndPoint, Arrow)
    //---------------------------------------------------------------------------------------------------------

    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: Rectangle 3 - BottomCenter -> Rectangle 4 - TopCenter
    //-----------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
        { 0.0, -25.0},
        { 0.0,  25.0}
    });
    *m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(200, 300),
        QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineRect3BottomCenterRect4TopCenter, *m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Rectangle 3 - BottomCenter -> Rectangle 4 - TopCenter: setLineEndStyle(EndPoint, Arrow)
    //---------------------------------------------------------------------------------------------------------

    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: Rectangle 3 - LeftCenter -> Rectangle 2 - RightCenter
    //-----------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
        {  175.0, -50.0},
        { -175.0,  50.0}
    });
    *m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(200, 300),
        QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineRect3LeftCenterRect2RightCenter, *m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Rectangle 3 - LeftCenter -> Rectangle 2 - RightCenter: setLineEndStyle(EndPoint, Arrow)
    //---------------------------------------------------------------------------------------------------------

    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Cncts.showLabels(" + CGraphObj::c_strLabelName + ")",
        /* strOperation    */ "Cncts.showLabels(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameRect1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedText", c_strGraphObjNameRect1},
        {"ExpectedPos", QPointF(183.0, 242.0)}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameRect2},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedText", c_strGraphObjNameRect2},
        {"ExpectedPos", QPointF(183.0, 342.0)}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameRect3},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedText", c_strGraphObjNameRect3},
        {"ExpectedPos", QPointF(583.0, 242.0)}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameRect4},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedText", c_strGraphObjNameRect4},
        {"ExpectedPos", QPointF(583.0, 342.0)}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(100.0, 300.0)},
        {"ExpectedText", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(300.0, 220.0)},
        {"ExpectedText", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(500.0, 300.0)},
        {"ExpectedText", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(300.0, 360.0)},
        {"ExpectedText", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter}
    });
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsAnchoredToRectsModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsAnchoredToRectsModifications",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpModifyConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;

    QString strGraphObjType;
    QString strFactoryGroupName;
    QString strGraphObjName;
    QString strGraphObjKeyInTreeCnctPt1;
    QString strGraphObjCnctPt1Name;
    CEnumSelectionPoint eSelPt1;
    QPointF ptCnctPt1;
    QString strGraphObjKeyInTreeCnctPt2;
    QString strGraphObjCnctPt2Name;
    CEnumSelectionPoint eSelPt2;
    QPointF ptCnctPt2;
    QString strMethod;
    QString strMthArgs;

    // Hide Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + ".hideLabels(" + CGraphObj::c_strLabelName + ")",
        /* strOperation    */ "CnctLines.hideLabels(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepHideLabels(ZS::Test::CTestStep*)) );

    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameRect1},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect1]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameRect1 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameRect2},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect2]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameRect2 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameRect3},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect3]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameRect3 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameRect4},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect4]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameRect4 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter + "." + CGraphObj::c_strLabelName + " not found"}
    });

    // move Rectangle1
    //----------------

    strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    strGraphObjName = c_strGraphObjNameRect1;
    strMethod = "move";
    QPointF ptPosRect1New(300.0, 200.0);
    strMthArgs = qPoint2Str(ptPosRect1New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    // while mouse is pressed and release the mouse event.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 250), m_ptPosRect1.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect1.toPoint(), ptPosRect1New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosRect1 = ptPosRect1New;
    m_pPhysValRect1->setCenter(QPointF(ptPosRect1New.x(), bYAxisTopDown ? ptPosRect1New.y() : fYAxisMaxVal - ptPosRect1New.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strGraphObjName, m_ptPosRect1, m_rectRect1, *m_pPhysValRect1, true, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
        {  50.0, -50.0},
        { -50.0,  50.0}
    });
    *m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineRect1BottomCenterRect2TopCenter,
        *m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter,
        false, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
        {-125.0, -75.0},
        { 125.0,  75.0}
    });
    *m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineRect1RightCenterRect4LeftCenter,
        *m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // move Rectangle2
    //----------------

    strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    strGraphObjName = c_strGraphObjNameRect2;
    strMethod = "move";
    QPointF ptPosRect2New(300.0, 400.0);
    strMthArgs = qPoint2Str(ptPosRect2New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    // while mouse is pressed and release the mouse event.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosRect2.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect2.toPoint(), ptPosRect2New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosRect2 = ptPosRect2New;
    m_pPhysValRect2->setCenter(QPointF(ptPosRect2New.x(), bYAxisTopDown ? ptPosRect2New.y() : fYAxisMaxVal - ptPosRect2New.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strGraphObjName, m_ptPosRect2, m_rectRect2, *m_pPhysValRect2, true, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
        {  0.0, -75.0},
        {  0.0,  75.0}
    });
    *m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineRect1BottomCenterRect2TopCenter,
        *m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter,
        false, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
        { 125.0, -75.0},
        {-125.0,  75.0}
    });
    *m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineRect3LeftCenterRect2RightCenter,
        *m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // move Rectangle3
    //----------------

    strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    strGraphObjName = c_strGraphObjNameRect3;
    strMethod = "move";
    QPointF ptPosRect3New(500.0, 200.0);
    strMthArgs = qPoint2Str(ptPosRect3New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    // while mouse is pressed and release the mouse event.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosRect3.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect3.toPoint(), ptPosRect3New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosRect3 = ptPosRect3New;
    m_pPhysValRect3->setCenter(QPointF(ptPosRect3New.x(), bYAxisTopDown ? ptPosRect3New.y() : fYAxisMaxVal - ptPosRect3New.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strGraphObjName, m_ptPosRect3, m_rectRect3, *m_pPhysValRect3, true, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
        { -50.0, -50.0},
        {  50.0,  50.0}
    });
    *m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineRect3BottomCenterRect4TopCenter,
        *m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter,
        false, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
        {  75.0, -100.0},
        { -75.0,  100.0}
    });
    *m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineRect3LeftCenterRect2RightCenter,
        *m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // move Rectangle4
    //----------------

    strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    strGraphObjName = c_strGraphObjNameRect4;
    QPointF ptPosRect4New(500.0, 400.0);
    strMthArgs = qPoint2Str(ptPosRect4New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    // while mouse is pressed and release the mouse event.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosRect4.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect4.toPoint(), ptPosRect4New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosRect4 = ptPosRect4New;
    m_pPhysValRect4->setCenter(QPointF(ptPosRect4New.x(), bYAxisTopDown ? ptPosRect4New.y() : fYAxisMaxVal - ptPosRect4New.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strGraphObjName, m_ptPosRect4, m_rectRect4, *m_pPhysValRect4, true, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
        {  0.0, -75.0},
        {  0.0,  75.0}
    });
    *m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineRect3BottomCenterRect4TopCenter,
        *m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter,
        false, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
        { -75.0, -100.0},
        {  75.0,  100.0}
    });
    *m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineRect1RightCenterRect4LeftCenter,
        *m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsAnchoredToEllipses(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsAnchoredToEllipses",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameConnections;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strGraphObjFactoryGroup = CObjFactory::c_strGroupNameConnections;
    QString strGraphObjKeyInTree;
    QString strGraphObjType;
    QString strGraphObjName;
    QString strGraphObjKeyInTreeCnctPt1;
    QString strGraphObjCnctPt1Name;
    CEnumSelectionPoint eSelPt1;
    QPointF ptCnctPt1;
    QString strGraphObjKeyInTreeCnctPt2;
    QString strGraphObjCnctPt2Name;
    CEnumSelectionPoint eSelPt2;
    QPointF ptCnctPt2;
    QString strMethod;
    QString strMthArgs;
    QStringList strlstExpectedValues;

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpDrawConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;

    // Ellipse 1
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeEllipse);
    strGraphObjName = c_strGraphObjNameEllipse1;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", QPoint(175.0, 225.0));
    pTestStep->setConfigValue("P2", QPoint(225.0, 275.0));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosEllipse1 = QPointF(200.0, 250.0);
    QPointF ptCenterEllipse1(m_ptPosEllipse1.x(), bYAxisTopDown ? m_ptPosEllipse1.y() : fYAxisMaxVal - m_ptPosEllipse1.y());
    QSizeF sizeEllipse1(50.0, 50.0);
    m_rectEllipse1 = QRectF(QPointF(-sizeEllipse1.width()/2.0, -sizeEllipse1.height()/2.0), sizeEllipse1);
    m_pPhysValRectEllipse1->setSize(sizeEllipse1);
    m_pPhysValRectEllipse1->setCenter(ptCenterEllipse1);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strGraphObjName, m_ptPosEllipse1, m_rectEllipse1, *m_pPhysValRectEllipse1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Ellipse 2
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeEllipse);
    strGraphObjName = c_strGraphObjNameEllipse2;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", QPoint(175.0, 325.0));
    pTestStep->setConfigValue("P2", QPoint(225.0, 375.0));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosEllipse2 = QPointF(200.0, 350.0);
    QPointF ptCenterEllipse2(m_ptPosEllipse2.x(), bYAxisTopDown ? m_ptPosEllipse2.y() : fYAxisMaxVal - m_ptPosEllipse2.y());
    QSizeF sizeEllipse2(50.0, 50.0);
    m_rectEllipse2 = QRectF(QPointF(-sizeEllipse2.width()/2.0, -sizeEllipse2.height()/2.0), sizeEllipse2);
    m_pPhysValRectEllipse2->setSize(sizeEllipse2);
    m_pPhysValRectEllipse2->setCenter(ptCenterEllipse2);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strGraphObjName, m_ptPosEllipse2, m_rectEllipse2, *m_pPhysValRectEllipse2, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Ellipse 3
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeEllipse);
    strGraphObjName = c_strGraphObjNameEllipse3;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", QPoint(575.0, 225.0));
    pTestStep->setConfigValue("P2", QPoint(625.0, 275.0));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosEllipse3 = QPointF(600.0, 250.0);
    QPointF ptCenterEllipse3(m_ptPosEllipse3.x(), bYAxisTopDown ? m_ptPosEllipse3.y() : fYAxisMaxVal - m_ptPosEllipse3.y());
    QSizeF sizeEllipse3(50.0, 50.0);
    m_rectEllipse3 = QRectF(QPointF(-sizeEllipse3.width()/2.0, -sizeEllipse3.height()/2.0), sizeEllipse3);
    m_pPhysValRectEllipse3->setSize(sizeEllipse3);
    m_pPhysValRectEllipse3->setCenter(ptCenterEllipse3);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strGraphObjName, m_ptPosEllipse3, m_rectEllipse3, *m_pPhysValRectEllipse3, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Ellipse 4
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeEllipse);
    strGraphObjName = c_strGraphObjNameEllipse4;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", QPoint(575.0, 325.0));
    pTestStep->setConfigValue("P2", QPoint(625.0, 375.0));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosEllipse4 = QPointF(600.0, 350.0);
    QPointF ptCenterEllipse4(m_ptPosEllipse4.x(), bYAxisTopDown ? m_ptPosEllipse4.y() : fYAxisMaxVal - m_ptPosEllipse4.y());
    QSizeF sizeEllipse4(50.0, 50.0);
    m_rectEllipse4 = QRectF(QPointF(-sizeEllipse4.width()/2.0, -sizeEllipse4.height()/2.0), sizeEllipse4);
    m_pPhysValRectEllipse4->setSize(sizeEllipse4);
    m_pPhysValRectEllipse4->setCenter(ptCenterEllipse4);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strGraphObjName, m_ptPosEllipse4, m_rectEllipse4, *m_pPhysValRectEllipse4, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Ellipse 1 - BottomCenter -> Ellipse 2 - TopCenter
    //-----------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineEllipse1BottomCenterEllipse2TopCenter;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse1, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRectEllipse1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse2, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRectEllipse2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineEllipse1BottomCenterEllipse2TopCenter = QPolygonF({
        { 0.0, -25.0},
        { 0.0,  25.0}
    });
    *m_pPhysValPolygonConnectionLineEllipse1BottomCenterEllipse2TopCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    QPolygon points({
        QPoint(200, 300),
        QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineEllipse1BottomCenterEllipse2TopCenter, *m_pPhysValPolygonConnectionLineEllipse1BottomCenterEllipse2TopCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Ellipse 1 - BottomCenter -> Ellipse 2 - TopCenter: setLineEndStyle(EndPoint, Arrow)
    //---------------------------------------------------------------------------------------------------------

    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: Ellipse 1 - RightCenter -> Ellipse 4 - LeftCenter
    //-----------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineEllipse1RightCenterEllipse4LeftCenter;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse1, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRectEllipse1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse4, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRectEllipse4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineEllipse1RightCenterEllipse4LeftCenter = QPolygonF({
        { -175.0, -50.0},
        {  175.0,  50.0}
    });
    *m_pPhysValPolygonConnectionLineEllipse1RightCenterEllipse4LeftCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(200, 300),
        QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineEllipse1RightCenterEllipse4LeftCenter, *m_pPhysValPolygonConnectionLineEllipse1RightCenterEllipse4LeftCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Ellipse 1 - RightCenter -> Ellipse 4 - LeftCenter: setLineEndStyle(EndPoint, Arrow)
    //---------------------------------------------------------------------------------------------------------

    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: Ellipse 3 - BottomCenter -> Ellipse 4 - TopCenter
    //-----------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineEllipse3BottomCenterEllipse4TopCenter;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse3, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRectEllipse3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse4, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRectEllipse4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineEllipse3BottomCenterEllipse4TopCenter = QPolygonF({
        { 0.0, -25.0},
        { 0.0,  25.0}
    });
    *m_pPhysValPolygonConnectionLineEllipse3BottomCenterEllipse4TopCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(200, 300),
        QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineEllipse3BottomCenterEllipse4TopCenter, *m_pPhysValPolygonConnectionLineEllipse3BottomCenterEllipse4TopCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Ellipse 3 - BottomCenter -> Ellipse 4 - TopCenter: setLineEndStyle(EndPoint, Arrow)
    //---------------------------------------------------------------------------------------------------------

    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: Ellipse 3 - LeftCenter -> Ellipse 2 - RightCenter
    //-----------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineEllipse3LeftCenterEllipse2RightCenter;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse3, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRectEllipse3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse2, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRectEllipse2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineEllipse3LeftCenterEllipse2RightCenter = QPolygonF({
        {  175.0, -50.0},
        { -175.0,  50.0}
    });
    *m_pPhysValPolygonConnectionLineEllipse3LeftCenterEllipse2RightCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(200, 300),
        QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineEllipse3LeftCenterEllipse2RightCenter, *m_pPhysValPolygonConnectionLineEllipse3LeftCenterEllipse2RightCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Ellipse 3 - LeftCenter -> Ellipse 2 - RightCenter: setLineEndStyle(EndPoint, Arrow)
    //---------------------------------------------------------------------------------------------------------

    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Cncts.showLabels(" + CGraphObj::c_strLabelName + ")",
        /* strOperation    */ "Cncts.showLabels(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse1);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameEllipse1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedText", c_strGraphObjNameEllipse1},
        {"ExpectedPos", QPointF(178.0, 242.0)}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse2);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameEllipse2},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedText", c_strGraphObjNameEllipse2},
        {"ExpectedPos", QPointF(178.0, 342.0)}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse3);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameEllipse3},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedText", c_strGraphObjNameEllipse3},
        {"ExpectedPos", QPointF(578.0, 242.0)}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse4);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameEllipse4},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedText", c_strGraphObjNameEllipse4},
        {"ExpectedPos", QPointF(578.0, 342.0)}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineEllipse1BottomCenterEllipse2TopCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse1BottomCenterEllipse2TopCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(100.0, 300.0)},
        {"ExpectedText", c_strGraphObjNameConnectionLineEllipse1BottomCenterEllipse2TopCenter}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineEllipse1RightCenterEllipse4LeftCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse1RightCenterEllipse4LeftCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(300.0, 220.0)},
        {"ExpectedText", c_strGraphObjNameConnectionLineEllipse1RightCenterEllipse4LeftCenter}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineEllipse3BottomCenterEllipse4TopCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse3BottomCenterEllipse4TopCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(500.0, 300.0)},
        {"ExpectedText", c_strGraphObjNameConnectionLineEllipse3BottomCenterEllipse4TopCenter}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineEllipse3LeftCenterEllipse2RightCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse3LeftCenterEllipse2RightCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(300.0, 360.0)},
        {"ExpectedText", c_strGraphObjNameConnectionLineEllipse3LeftCenterEllipse2RightCenter}
    });
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsAnchoredToEllipsesModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsAnchoredToEllipsesModifications",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpModifyConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;

    QString strGraphObjType;
    QString strFactoryGroupName;
    QString strGraphObjName;
    QString strGraphObjKeyInTreeCnctPt1;
    QString strGraphObjCnctPt1Name;
    CEnumSelectionPoint eSelPt1;
    QPointF ptCnctPt1;
    QString strGraphObjKeyInTreeCnctPt2;
    QString strGraphObjCnctPt2Name;
    CEnumSelectionPoint eSelPt2;
    QPointF ptCnctPt2;
    QString strMethod;
    QString strMthArgs;

    // Hide Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + ".hideLabels(" + CGraphObj::c_strLabelName + ")",
        /* strOperation    */ "CnctLines.hideLabels(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepHideLabels(ZS::Test::CTestStep*)) );

    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameEllipse1},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameEllipse1]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameEllipse1 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameEllipse2},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameEllipse2]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameEllipse2 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameEllipse3},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameEllipse3]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameEllipse3 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameEllipse4},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameEllipse4]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameEllipse4 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineEllipse1BottomCenterEllipse2TopCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse1BottomCenterEllipse2TopCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameConnectionLineEllipse1BottomCenterEllipse2TopCenter + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineEllipse1RightCenterEllipse4LeftCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse1RightCenterEllipse4LeftCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameConnectionLineEllipse1RightCenterEllipse4LeftCenter + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineEllipse3BottomCenterEllipse4TopCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse3BottomCenterEllipse4TopCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameConnectionLineEllipse3BottomCenterEllipse4TopCenter + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameConnectionLineEllipse3LeftCenterEllipse2RightCenter},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse3LeftCenterEllipse2RightCenter]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameConnectionLineEllipse3LeftCenterEllipse2RightCenter + "." + CGraphObj::c_strLabelName + " not found"}
    });

    // move Ellipse 1
    //----------------

    strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    strGraphObjType = graphObjType2Str(EGraphObjTypeEllipse);
    strGraphObjName = c_strGraphObjNameEllipse1;
    strMethod = "move";
    QPointF ptPosEllipse1New(300.0, 200.0);
    strMthArgs = qPoint2Str(ptPosEllipse1New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse1BottomCenterEllipse2TopCenter]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse1RightCenterEllipse4LeftCenter]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    // while mouse is pressed and release the mouse event.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 250), m_ptPosEllipse1.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosEllipse1.toPoint(), ptPosEllipse1New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosEllipse1 = ptPosEllipse1New;
    m_pPhysValRectEllipse1->setCenter(QPointF(ptPosEllipse1New.x(), bYAxisTopDown ? ptPosEllipse1New.y() : fYAxisMaxVal - ptPosEllipse1New.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForEllipse(
        strGraphObjName, m_ptPosEllipse1, m_rectEllipse1, *m_pPhysValRectEllipse1, true, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse1, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRectEllipse1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse2, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRectEllipse2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineEllipse1BottomCenterEllipse2TopCenter = QPolygonF({
        {  50.0, -50.0},
        { -50.0,  50.0}
    });
    *m_pPhysValPolygonConnectionLineEllipse1BottomCenterEllipse2TopCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineEllipse1BottomCenterEllipse2TopCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineEllipse1BottomCenterEllipse2TopCenter,
        *m_pPhysValPolygonConnectionLineEllipse1BottomCenterEllipse2TopCenter,
        false, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse1, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRectEllipse1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse4, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRectEllipse4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineEllipse1RightCenterEllipse4LeftCenter = QPolygonF({
        {-125.0, -75.0},
        { 125.0,  75.0}
    });
    *m_pPhysValPolygonConnectionLineEllipse1RightCenterEllipse4LeftCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineEllipse1RightCenterEllipse4LeftCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineEllipse1RightCenterEllipse4LeftCenter,
        *m_pPhysValPolygonConnectionLineEllipse1RightCenterEllipse4LeftCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // move Ellipse 2
    //----------------

    strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    strGraphObjType = graphObjType2Str(EGraphObjTypeEllipse);
    strGraphObjName = c_strGraphObjNameEllipse2;
    strMethod = "move";
    QPointF ptPosEllipse2New(300.0, 400.0);
    strMthArgs = qPoint2Str(ptPosEllipse2New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse1BottomCenterEllipse2TopCenter]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse3LeftCenterEllipse2RightCenter]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    // while mouse is pressed and release the mouse event.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosEllipse2.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosEllipse2.toPoint(), ptPosEllipse2New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosEllipse2 = ptPosEllipse2New;
    m_pPhysValRectEllipse2->setCenter(QPointF(ptPosEllipse2New.x(), bYAxisTopDown ? ptPosEllipse2New.y() : fYAxisMaxVal - ptPosEllipse2New.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForEllipse(
        strGraphObjName, m_ptPosEllipse2, m_rectEllipse2, *m_pPhysValRectEllipse2, true, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse1, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRectEllipse1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse2, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRectEllipse2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineEllipse1BottomCenterEllipse2TopCenter = QPolygonF({
        {  0.0, -75.0},
        {  0.0,  75.0}
    });
    *m_pPhysValPolygonConnectionLineEllipse1BottomCenterEllipse2TopCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineEllipse1BottomCenterEllipse2TopCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineEllipse1BottomCenterEllipse2TopCenter,
        *m_pPhysValPolygonConnectionLineEllipse1BottomCenterEllipse2TopCenter,
        false, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse3, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRectEllipse3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse2, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRectEllipse2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineEllipse3LeftCenterEllipse2RightCenter = QPolygonF({
        { 125.0, -75.0},
        {-125.0,  75.0}
    });
    *m_pPhysValPolygonConnectionLineEllipse3LeftCenterEllipse2RightCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineEllipse3LeftCenterEllipse2RightCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineEllipse3LeftCenterEllipse2RightCenter,
        *m_pPhysValPolygonConnectionLineEllipse3LeftCenterEllipse2RightCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // move Ellipse 3
    //----------------

    strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    strGraphObjType = graphObjType2Str(EGraphObjTypeEllipse);
    strGraphObjName = c_strGraphObjNameEllipse3;
    strMethod = "move";
    QPointF ptPosEllipse3New(500.0, 200.0);
    strMthArgs = qPoint2Str(ptPosEllipse3New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse3BottomCenterEllipse4TopCenter]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse3LeftCenterEllipse2RightCenter]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    // while mouse is pressed and release the mouse event.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosEllipse3.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosEllipse3.toPoint(), ptPosEllipse3New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosEllipse3 = ptPosEllipse3New;
    m_pPhysValRectEllipse3->setCenter(QPointF(ptPosEllipse3New.x(), bYAxisTopDown ? ptPosEllipse3New.y() : fYAxisMaxVal - ptPosEllipse3New.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForEllipse(
        strGraphObjName, m_ptPosEllipse3, m_rectEllipse3, *m_pPhysValRectEllipse3, true, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse3, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRectEllipse3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse4, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRectEllipse4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineEllipse3BottomCenterEllipse4TopCenter = QPolygonF({
        { -50.0, -50.0},
        {  50.0,  50.0}
    });
    *m_pPhysValPolygonConnectionLineEllipse3BottomCenterEllipse4TopCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineEllipse3BottomCenterEllipse4TopCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineEllipse3BottomCenterEllipse4TopCenter,
        *m_pPhysValPolygonConnectionLineEllipse3BottomCenterEllipse4TopCenter,
        false, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse3, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRectEllipse3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse2, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRectEllipse2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineEllipse3LeftCenterEllipse2RightCenter = QPolygonF({
        {  75.0, -100.0},
        { -75.0,  100.0}
    });
    *m_pPhysValPolygonConnectionLineEllipse3LeftCenterEllipse2RightCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineEllipse3LeftCenterEllipse2RightCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineEllipse3LeftCenterEllipse2RightCenter,
        *m_pPhysValPolygonConnectionLineEllipse3LeftCenterEllipse2RightCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // move Ellipse 4
    //----------------

    strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    strGraphObjType = graphObjType2Str(EGraphObjTypeEllipse);
    strGraphObjName = c_strGraphObjNameEllipse4;
    QPointF ptPosEllipse4New(500.0, 400.0);
    strMthArgs = qPoint2Str(ptPosEllipse4New) + " " + strUnit;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse3BottomCenterEllipse4TopCenter]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineEllipse1RightCenterEllipse4LeftCenter]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    // while mouse is pressed and release the mouse event.
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosEllipse4.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosEllipse4.toPoint(), ptPosEllipse4New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosEllipse4 = ptPosEllipse4New;
    m_pPhysValRectEllipse4->setCenter(QPointF(ptPosEllipse4New.x(), bYAxisTopDown ? ptPosEllipse4New.y() : fYAxisMaxVal - ptPosEllipse4New.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForEllipse(
        strGraphObjName, m_ptPosEllipse4, m_rectEllipse4, *m_pPhysValRectEllipse4, true, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse4, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRectEllipse3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse1, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRectEllipse4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineEllipse3BottomCenterEllipse4TopCenter = QPolygonF({
        {  0.0, -75.0},
        {  0.0,  75.0}
    });
    *m_pPhysValPolygonConnectionLineEllipse3BottomCenterEllipse4TopCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineEllipse3BottomCenterEllipse4TopCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineEllipse3BottomCenterEllipse4TopCenter,
        *m_pPhysValPolygonConnectionLineEllipse3BottomCenterEllipse4TopCenter,
        false, iResultValuesPrecision));
    eSelPt1 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse1, strGraphObjCnctPt1Name);
    ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRectEllipse1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    eSelPt2 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameEllipse4, strGraphObjCnctPt2Name);
    ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRectEllipse4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    m_polygonConnectionLineEllipse1RightCenterEllipse4LeftCenter = QPolygonF({
        { -75.0, -100.0},
        {  75.0,  100.0}
    });
    *m_pPhysValPolygonConnectionLineEllipse1RightCenterEllipse4LeftCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineEllipse1RightCenterEllipse4LeftCenter,
        strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineEllipse1RightCenterEllipse4LeftCenter,
        *m_pPhysValPolygonConnectionLineEllipse1RightCenterEllipse4LeftCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsAnchoredToPolygons(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsAnchoredToPolygons",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameConnections;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strGraphObjFactoryGroup = CObjFactory::c_strGroupNameConnections;
    QString strGraphObjKeyInTree;
    QString strGraphObjType;
    QString strGraphObjName;
    QString strGraphObjKeyInTreeCnctPt1;
    QString strGraphObjCnctPt1Name;
    int idxPt;
    int idxLine;
    QPointF ptCnctPt1;
    QString strGraphObjKeyInTreeCnctPt2;
    QString strGraphObjCnctPt2Name;
    CEnumSelectionPoint eSelPt2;
    QPointF ptCnctPt2;
    QString strMethod;
    QString strMthArgs;
    QStringList strlstExpectedValues;

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpDrawConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;

    // Star 1
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypePolygon);
    strGraphObjName = c_strGraphObjNameStar1;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjPolygon(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    m_ptPosStar1 = QPointF(200.0, 200.0);
    m_polygonStar1 = QPolygonF({
        {   0.0,  -40.0},
        {  10.0,  -10.0},
        {  40.0,    0.0},
        {  10.0,   10.0},
        {   0.0,   40.0},
        { -10.0,   10.0},
        { -40.0,    0.0},
        { -10.0,  -10.0}
    });
    *m_pPhysValPolygonStar1 = QPolygonF();
    QPolygon points;
    for (const QPointF& pt : m_polygonStar1) {
        double fX = m_ptPosStar1.x() + pt.x();
        double fY = m_ptPosStar1.y() + pt.y();
        points.append(QPoint(fX, fY));
        if (!bYAxisTopDown) {
            fY = fYAxisMaxVal - (m_ptPosStar1.y() + pt.y());
        }
        m_pPhysValPolygonStar1->append(CPhysValPoint(*m_pDrawingScene, fX, fY));
    };
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypePolygon));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameStar1);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameStar1, m_ptPosStar1, m_polygonStar1, *m_pPhysValPolygonStar1, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Star 2
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypePolygon);
    strGraphObjName = c_strGraphObjNameStar2;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjPolygon(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    m_ptPosStar2 = QPointF(200.0, 400.0);
    m_polygonStar2 = QPolygonF({
        {   0.0,  -40.0},
        {  10.0,  -10.0},
        {  40.0,    0.0},
        {  10.0,   10.0},
        {   0.0,   40.0},
        { -10.0,   10.0},
        { -40.0,    0.0},
        { -10.0,  -10.0}
    });
    *m_pPhysValPolygonStar2 = QPolygonF();
    points.clear();
    for (const QPointF& pt : m_polygonStar2) {
        double fX = m_ptPosStar2.x() + pt.x();
        double fY = m_ptPosStar2.y() + pt.y();
        points.append(QPoint(fX, fY));
        if (!bYAxisTopDown) {
            fY = fYAxisMaxVal - (m_ptPosStar2.y() + pt.y());
        }
        m_pPhysValPolygonStar2->append(CPhysValPoint(*m_pDrawingScene, fX, fY));
    };
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypePolygon));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameStar2);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameStar2, m_ptPosStar2, m_polygonStar2, *m_pPhysValPolygonStar2, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Star 3
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypePolygon);
    strGraphObjName = c_strGraphObjNameStar3;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjPolygon(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    m_ptPosStar3 = QPointF(600.0, 200.0);
    m_polygonStar3 = QPolygonF({
        {   0.0,  -40.0},
        {  10.0,  -10.0},
        {  40.0,    0.0},
        {  10.0,   10.0},
        {   0.0,   40.0},
        { -10.0,   10.0},
        { -40.0,    0.0},
        { -10.0,  -10.0}
    });
    *m_pPhysValPolygonStar3 = QPolygonF();
    points.clear();
    for (const QPointF& pt : m_polygonStar3) {
        double fX = m_ptPosStar3.x() + pt.x();
        double fY = m_ptPosStar3.y() + pt.y();
        points.append(QPoint(fX, fY));
        if (!bYAxisTopDown) {
            fY = fYAxisMaxVal - (m_ptPosStar3.y() + pt.y());
        }
        m_pPhysValPolygonStar3->append(CPhysValPoint(*m_pDrawingScene, fX, fY));
    };
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypePolygon));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameStar3);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameStar3, m_ptPosStar3, m_polygonStar3, *m_pPhysValPolygonStar3, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Star 4
    //------------

    strGraphObjType = graphObjType2Str(EGraphObjTypePolygon);
    strGraphObjName = c_strGraphObjNameStar4;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjPolygon(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    m_ptPosStar4 = QPointF(600.0, 400.0);
    m_polygonStar4 = QPolygonF({
        {   0.0,  -40.0},
        {  10.0,  -10.0},
        {  40.0,    0.0},
        {  10.0,   10.0},
        {   0.0,   40.0},
        { -10.0,   10.0},
        { -40.0,    0.0},
        { -10.0,  -10.0}
    });
    *m_pPhysValPolygonStar4 = QPolygonF();
    points.clear();
    for (const QPointF& pt : m_polygonStar4) {
        double fX = m_ptPosStar4.x() + pt.x();
        double fY = m_ptPosStar4.y() + pt.y();
        points.append(QPoint(fX, fY));
        if (!bYAxisTopDown) {
            fY = fYAxisMaxVal - (m_ptPosStar4.y() + pt.y());
        }
        m_pPhysValPolygonStar4->append(CPhysValPoint(*m_pDrawingScene, fX, fY));
    };
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypePolygon));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameStar4);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameStar4, m_ptPosStar4, m_polygonStar4, *m_pPhysValPolygonStar4, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Star 1 - Pt5 (BottomCenter) -> Star 2 - Pt1 (TopCenter)
    //-------------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineStar1BottomCenterStar2TopCenter;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    idxPt = 4;
    strGraphObjCnctPt1Name = "CnctPoint-P" + QString::number(idxPt) + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar1, strGraphObjCnctPt1Name);
    ptCnctPt1 = m_pPhysValPolygonStar1->at(idxPt).toQPointF();
    idxPt = 0;
    strGraphObjCnctPt2Name = "CnctPoint-P" + QString::number(idxPt) + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar2, strGraphObjCnctPt2Name);
    ptCnctPt2 = m_pPhysValPolygonStar2->at(idxPt).toQPointF();
    m_polygonConnectionLineStar1BottomCenterStar2TopCenter = QPolygonF({
        { 0.0, -60.0},
        { 0.0,  60.0}
    });
    *m_pPhysValPolygonConnectionLineStar1BottomCenterStar2TopCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(200, 140),
        QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineStar1BottomCenterStar2TopCenter,
        *m_pPhysValPolygonConnectionLineStar1BottomCenterStar2TopCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Star 1 - Pt5 (BottomCenter) -> Star 2 - Pt1 (TopCenter): setLineEndStyle(EndPoint, Arrow)
    //-----------------------------------------------------------------------------------------------------------

    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setPenColor"},
        {"PenColor", "red"}
    });
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: Star 2 - Pt3 (RightCenter) -> Star 4 - Pt7 (LeftCenter)
    //-------------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineStar2RightCenterStar4LeftCenter;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    idxPt = 2;
    strGraphObjCnctPt1Name = "CnctPoint-P" + QString::number(idxPt) + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar2, strGraphObjCnctPt1Name);
    ptCnctPt1 = m_pPhysValPolygonStar2->at(idxPt).toQPointF();
    idxPt = 6;
    strGraphObjCnctPt2Name = "CnctPoint-P" + QString::number(idxPt) + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar4, strGraphObjCnctPt2Name);
    ptCnctPt2 = m_pPhysValPolygonStar4->at(idxPt).toQPointF();
    m_polygonConnectionLineStar2RightCenterStar4LeftCenter = QPolygonF({
        { -160.0, 0.0},
        {  160.0, 0.0}
    });
    *m_pPhysValPolygonConnectionLineStar2RightCenterStar4LeftCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(240, 440),
        QPoint(ptCnctPt1.toPoint().x() - 1, bYAxisTopDown ? ptCnctPt1.toPoint().y() : fYAxisMaxVal - ptCnctPt1.toPoint().y()),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x() + 1, bYAxisTopDown ? ptCnctPt2.toPoint().y() : fYAxisMaxVal - ptCnctPt2.toPoint().y()),
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineStar2RightCenterStar4LeftCenter,
        *m_pPhysValPolygonConnectionLineStar2RightCenterStar4LeftCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Star 2 - Pt3 (RightCenter) -> Star 4 - Pt7 (LeftCenter): setLineEndStyle(EndPoint, Arrow)
    //-----------------------------------------------------------------------------------------------------------

    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setPenColor"},
        {"PenColor", "red"}
    });
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: Star 4 - Pt1 (TopCenter) -> Star 3 - Pt5 (BottomCenter)
    //-------------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineStar4TopCenterStar3BottomCenter;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    idxPt = 0;
    strGraphObjCnctPt1Name = "CnctPoint-P" + QString::number(idxPt) + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar4, strGraphObjCnctPt1Name);
    ptCnctPt1 = m_pPhysValPolygonStar4->at(idxPt).toQPointF();
    idxPt = 4;
    strGraphObjCnctPt2Name = "CnctPoint-P" + QString::number(idxPt) + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar3, strGraphObjCnctPt2Name);
    ptCnctPt2 = m_pPhysValPolygonStar3->at(idxPt).toQPointF();
    m_polygonConnectionLineStar4TopCenterStar3BottomCenter = QPolygonF({
        { 0.0,  60.0},
        { 0.0, -60.0}
    });
    *m_pPhysValPolygonConnectionLineStar4TopCenterStar3BottomCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(580, 360),
        QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() + 1),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() - 1),
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineStar4TopCenterStar3BottomCenter,
        *m_pPhysValPolygonConnectionLineStar4TopCenterStar3BottomCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Star 4 - Pt1 (TopCenter) -> Star 3 - Pt5 (BottomCenter): setLineEndStyle(EndPoint, Arrow)
    //-----------------------------------------------------------------------------------------------------------

    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setPenColor"},
        {"PenColor", "red"}
    });
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: Star 3 - Pt7 (LeftCenter) -> Star 1 - Pt3 (RightCenter)
    //-------------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineStar3LeftCenterStar1RightCenter;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    idxPt = 6;
    strGraphObjCnctPt1Name = "CnctPoint-P" + QString::number(idxPt) + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar3, strGraphObjCnctPt1Name);
    ptCnctPt1 = m_pPhysValPolygonStar3->at(idxPt).toQPointF();
    idxPt = 2;
    strGraphObjCnctPt2Name = "CnctPoint-P" + QString::number(idxPt) + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar1, strGraphObjCnctPt2Name);
    ptCnctPt2 = m_pPhysValPolygonStar1->at(idxPt).toQPointF();
    m_polygonConnectionLineStar3LeftCenterStar1RightCenter = QPolygonF({
        {  160.0, 0.0},
        { -160.0, 0.0}
    });
    *m_pPhysValPolygonConnectionLineStar3LeftCenterStar1RightCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(540, 220),
        QPoint(ptCnctPt1.toPoint().x() + 1, bYAxisTopDown ? ptCnctPt1.toPoint().y() : fYAxisMaxVal - ptCnctPt1.toPoint().y()),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x() - 1, bYAxisTopDown ? ptCnctPt2.toPoint().y() : fYAxisMaxVal - ptCnctPt2.toPoint().y()),
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineStar3LeftCenterStar1RightCenter,
        *m_pPhysValPolygonConnectionLineStar3LeftCenterStar1RightCenter,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Star 3 - Pt7 (LeftCenter) -> Star 1 - Pt3 (RightCenter): setLineEndStyle(EndPoint, Arrow)
    //-----------------------------------------------------------------------------------------------------------

    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    strlstExpectedValues.clear();
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->addDataRow({
        {"Method", "setPenColor"},
        {"PenColor", "red"}
    });
    pTestStep->addDataRow({
        {"Method", "setLineEndStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadBaseLineType"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadFillStyle"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadWidth"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    });
    pTestStep->addDataRow({
        {"Method", "setArrowHeadLength"},
        {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
        {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    });

    // Connection Line: Star 1 - LineCenter (Pt1, Pt2) -> Star 3 - LineCenter (Pt8, Pt0)
    //----------------------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineStar1LineCenterPt1Pt2Star3LineCenterPt8Pt0;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    idxLine = 0;
    strGraphObjCnctPt1Name = "CnctPoint-L" + QString::number(idxLine) + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar1, strGraphObjCnctPt1Name);
    ptCnctPt1 = QLineF(m_pPhysValPolygonStar1->at(0).toQPointF(), m_pPhysValPolygonStar1->at(1).toQPointF()).center();
    idxLine = 7;
    strGraphObjCnctPt2Name = "CnctPoint-L" + QString::number(idxLine) + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar3, strGraphObjCnctPt2Name);
    ptCnctPt2 = QLineF(m_pPhysValPolygonStar3->at(7).toQPointF(), m_pPhysValPolygonStar3->at(0).toQPointF()).center();
    m_polygonConnectionLineStar1LineCenterPt1Pt2Star3LineCenterPt8Pt0 = QPolygonF({
        { -195.0, 0.0},
        {  195.0, 0.0}
    });
    *m_pPhysValPolygonConnectionLineStar1LineCenterPt1Pt2Star3LineCenterPt8Pt0 = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(220, 160),
        QPoint(ptCnctPt1.toPoint().x() - 1, bYAxisTopDown ? ptCnctPt1.toPoint().y() : fYAxisMaxVal - ptCnctPt1.toPoint().y()),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x() + 1, bYAxisTopDown ? ptCnctPt2.toPoint().y() : fYAxisMaxVal - ptCnctPt2.toPoint().y()),
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineStar1LineCenterPt1Pt2Star3LineCenterPt8Pt0,
        *m_pPhysValPolygonConnectionLineStar1LineCenterPt1Pt2Star3LineCenterPt8Pt0,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: Star 2 - LineCenter (Pt4, Pt5) -> Star 4 - LineCenter (Pt5, Pt6)
    //----------------------------------------------------------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineStar2LineCenterPt4Pt5Star4LineCenterPt5Pt6;
    strMethod = "DrawingScene.drawGraphObj";
    strMthArgs = strGraphObjName;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    idxLine = 3;
    strGraphObjCnctPt1Name = "CnctPoint-L" + QString::number(idxLine) + "-1";
    strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar2, strGraphObjCnctPt1Name);
    ptCnctPt1 = QLineF(m_pPhysValPolygonStar2->at(3).toQPointF(), m_pPhysValPolygonStar2->at(4).toQPointF()).center();
    idxLine = 4;
    strGraphObjCnctPt2Name = "CnctPoint-L" + QString::number(idxLine) + "-1";
    strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar4, strGraphObjCnctPt2Name);
    ptCnctPt2 = QLineF(m_pPhysValPolygonStar4->at(4).toQPointF(), m_pPhysValPolygonStar4->at(5).toQPointF()).center();
    m_polygonConnectionLineStar2LineCenterPt4Pt5Star4LineCenterPt5Pt6 = QPolygonF({
        { -195.0, 0.0},
        {  195.0, 0.0}
    });
    *m_pPhysValPolygonConnectionLineStar2LineCenterPt4Pt5Star4LineCenterPt5Pt6 = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    // We need a start position, from which we move to the first connection point.
    // Then we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    points = QPolygon({
        QPoint(220, 160),
        QPoint(ptCnctPt1.toPoint().x() - 1, bYAxisTopDown ? ptCnctPt1.toPoint().y() : fYAxisMaxVal - ptCnctPt1.toPoint().y()),
        QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
               bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
        QPoint(ptCnctPt2.toPoint().x() + 1, bYAxisTopDown ? ptCnctPt2.toPoint().y() : fYAxisMaxVal - ptCnctPt2.toPoint().y()),
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineStar2LineCenterPt4Pt5Star4LineCenterPt5Pt6,
        *m_pPhysValPolygonConnectionLineStar2LineCenterPt4Pt5Star4LineCenterPt5Pt6,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Cncts.showLabels(" + CGraphObj::c_strLabelName + ")",
        /* strOperation    */ "Cncts.showLabels(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ pGrpDrawConnections,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar1);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(m_ptPosStar1.x() - 5, m_ptPosStar1.y() - 5)},
        {"ExpectedText", c_strGraphObjNameStar1},
        {"ExpectedPos", QPointF(195.0, 195.0)}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "1"},
        {"AnchorPoint", SGraphObjSelectionPoint(nullptr, ESelectionPointType::PolygonPoint, 0).toString()},
        {"setPos", QPointF(190.0, 135.0)},
        {"ExpectedText", CGraphObj::c_strGeometryLabelNameP + "1"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "3"},
        {"AnchorPoint", SGraphObjSelectionPoint(nullptr, ESelectionPointType::PolygonPoint, 2).toString()},
        {"setPos", QPointF(245.0, 210.0)},
        {"ExpectedText", CGraphObj::c_strGeometryLabelNameP + "3"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "5"},
        {"AnchorPoint", SGraphObjSelectionPoint(nullptr, ESelectionPointType::PolygonPoint, 4).toString()},
        {"setPos", QPointF(180.0, 250.0)},
        {"ExpectedText", CGraphObj::c_strGeometryLabelNameP + "5"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "7"},
        {"AnchorPoint", SGraphObjSelectionPoint(nullptr, ESelectionPointType::PolygonPoint, 6).toString()},
        {"setPos", QPointF(145.0, 200.0)},
        {"ExpectedText", CGraphObj::c_strGeometryLabelNameP + "7"}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar2);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar2},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(m_ptPosStar2.x() - 5, m_ptPosStar2.y() - 5)},
        {"ExpectedText", c_strGraphObjNameStar2},
        {"ExpectedPos", QPointF(195.0, 395.0)}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar3);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar3},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(m_ptPosStar3.x() - 5, m_ptPosStar3.y() - 5)},
        {"ExpectedText", c_strGraphObjNameStar3},
        {"ExpectedPos", QPointF(595.0, 195.0)}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar4);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar4},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(m_ptPosStar4.x() - 5, m_ptPosStar4.y() - 5)},
        {"ExpectedText", c_strGraphObjNameStar4},
        {"ExpectedPos", QPointF(595.0, 395.0)}
    });
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsAnchoredToPolygonsModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsAnchoredToPolygonsModifications",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpModifyConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;

    QString strFactoryGroupName;
    QString strGraphObjType;
    QString strGraphObjKeyInTree;
    QString strGraphObjName;
    QString strGraphObjKeyInTreeCnctPt1;
    QString strGraphObjCnctPt1Name;
    int idxPt;
    int idxLine;
    QPointF ptCnctPt1;
    QString strGraphObjKeyInTreeCnctPt2;
    QString strGraphObjCnctPt2Name;
    CEnumSelectionPoint eSelPt2;
    QPointF ptCnctPt2;
    QString strMethod;
    QString strMthArgs;

    // Hide Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + ".hideLabels(" + CGraphObj::c_strLabelName + ")",
        /* strOperation    */ "CnctLines.hideLabels(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepHideLabels(ZS::Test::CTestStep*)) );
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar1);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameStar1 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "1"},
        {"ExpectedValue", c_strGraphObjNameStar1 + "." + CGraphObj::c_strGeometryLabelNameP + "1" + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "3"},
        {"ExpectedValue", c_strGraphObjNameStar1 + "." + CGraphObj::c_strGeometryLabelNameP + "3" + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "5"},
        {"ExpectedValue", c_strGraphObjNameStar1 + "." + CGraphObj::c_strGeometryLabelNameP + "5" + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar1},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "7"},
        {"ExpectedValue", c_strGraphObjNameStar1 + "." + CGraphObj::c_strGeometryLabelNameP + "7" + " not found"}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar2);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar2},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameStar2 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar3);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar3},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameStar3 + "." + CGraphObj::c_strLabelName + " not found"}
    });
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar4);
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar4},
        {"GraphObjKeyInTree", strGraphObjKeyInTree},
        {"LabelName", CGraphObj::c_strLabelName},
        {"ExpectedValue", c_strGraphObjNameStar4 + "." + CGraphObj::c_strLabelName + " not found"}
    });

    // move Star 1
    //----------------

    strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    strGraphObjType = graphObjType2Str(EGraphObjTypePolygon);
    strGraphObjName = c_strGraphObjNameStar1;
    strMethod = "move";
    QPointF ptPosStar1New(300.0, 200.0);
    strMthArgs = qPoint2Str(ptPosStar1New) + " px";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyConnections,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
        {m_hshGraphObjNameToKeys[strGraphObjName]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineStar1BottomCenterStar2TopCenter]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineStar3LeftCenterStar1RightCenter]},
        {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineStar1LineCenterPt1Pt2Star3LineCenterPt8Pt0]}});
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse onto polygon, press mouse to select polygon, move the polygon to the new position
    // while mouse is pressed and release the mouse event.
    QPointF ptP1(m_pPhysValPolygonStar1->at(1).toQPointF());
    if (!bYAxisTopDown) ptP1.setY(fYAxisMaxVal - m_pPhysValPolygonStar1->at(1).toQPointF().y());
    QPointF ptP2(m_pPhysValPolygonStar1->at(2).toQPointF());
    if (!bYAxisTopDown) ptP2.setY(fYAxisMaxVal - m_pPhysValPolygonStar1->at(2).toQPointF().y());
    m_ptMousePos = QLineF(ptP1, ptP2).center().toPoint();
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 250), m_ptMousePos);
    pTestStep->addDataRow({ // Select object by clicking on it
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, m_ptPosStar1.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({ // Select object by clicking in center
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosStar1.toPoint(), ptPosStar1New.toPoint(), 0, Qt::LeftButton);
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos},
        {"MouseButtons", Qt::LeftButton},
        {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    });
    m_ptPosStar1 = ptPosStar1New;
    m_pPhysValPolygonStar1->setCenter(QPointF(ptPosStar1New.x(), bYAxisTopDown ? ptPosStar1New.y() : fYAxisMaxVal - ptPosStar1New.y()));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameStar1, m_ptPosStar1, m_polygonStar1, *m_pPhysValPolygonStar1, true, iResultValuesPrecision));
    strGraphObjName = c_strGraphObjNameConnectionLineStar1BottomCenterStar2TopCenter;
    idxPt = 4;
    strGraphObjCnctPt1Name = "CnctPoint-P" + QString::number(idxPt) + "-1";
    ptCnctPt1 = m_pPhysValPolygonStar1->at(idxPt).toQPointF();
    idxPt = 0;
    strGraphObjCnctPt2Name = "CnctPoint-P" + QString::number(idxPt) + "-1";
    ptCnctPt2 = m_pPhysValPolygonStar2->at(idxPt).toQPointF();
    m_polygonConnectionLineStar1BottomCenterStar2TopCenter = QPolygonF({
        {  50.0, -60.0},
        { -50.0,  60.0}
    });
    *m_pPhysValPolygonConnectionLineStar1BottomCenterStar2TopCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineStar1BottomCenterStar2TopCenter,
        *m_pPhysValPolygonConnectionLineStar1BottomCenterStar2TopCenter,
        false, iResultValuesPrecision));
    strGraphObjName = c_strGraphObjNameConnectionLineStar3LeftCenterStar1RightCenter;
    idxPt = 6;
    strGraphObjCnctPt1Name = "CnctPoint-P" + QString::number(idxPt) + "-1";
    ptCnctPt1 = m_pPhysValPolygonStar3->at(idxPt).toQPointF();
    idxPt = 2;
    strGraphObjCnctPt2Name = "CnctPoint-P" + QString::number(idxPt) + "-1";
    ptCnctPt2 = m_pPhysValPolygonStar1->at(idxPt).toQPointF();
    m_polygonConnectionLineStar3LeftCenterStar1RightCenter = QPolygonF({
        {  110.0, 0.0},
        { -110.0, 0.0}
    });
    *m_pPhysValPolygonConnectionLineStar3LeftCenterStar1RightCenter = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineStar3LeftCenterStar1RightCenter,
        *m_pPhysValPolygonConnectionLineStar3LeftCenterStar1RightCenter,
        false, iResultValuesPrecision));
    strGraphObjName = c_strGraphObjNameConnectionLineStar1LineCenterPt1Pt2Star3LineCenterPt8Pt0;
    idxLine = 0;
    strGraphObjCnctPt1Name = "CnctPoint-L" + QString::number(idxLine) + "-1";
    ptCnctPt1 = QLineF(m_pPhysValPolygonStar1->at(0).toQPointF(), m_pPhysValPolygonStar1->at(1).toQPointF()).center();
    idxLine = 7;
    strGraphObjCnctPt2Name = "CnctPoint-L" + QString::number(idxLine) + "-1";
    ptCnctPt2 = QLineF(m_pPhysValPolygonStar3->at(7).toQPointF(), m_pPhysValPolygonStar3->at(0).toQPointF()).center();
    m_polygonConnectionLineStar1LineCenterPt1Pt2Star3LineCenterPt8Pt0 = QPolygonF({
        { -145.0, 0.0},
        {  145.0, 0.0}
    });
    *m_pPhysValPolygonConnectionLineStar1LineCenterPt1Pt2Star3LineCenterPt8Pt0 = QPolygonF({
        {ptCnctPt1},
        {ptCnctPt2}
    });
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
        m_polygonConnectionLineStar1LineCenterPt1Pt2Star3LineCenterPt8Pt0,
        *m_pPhysValPolygonConnectionLineStar1LineCenterPt1Pt2Star3LineCenterPt8Pt0,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsAnchoredToTexts(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsAnchoredToTexts",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameConnections;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strGraphObjFactoryGroup = CObjFactory::c_strGroupNameConnections;
    QString strGraphObjKeyInTree;
    QString strGraphObjType;
    QString strGraphObjName;
    QString strGraphObjKeyInTreeCnctPt1;
    QString strGraphObjCnctPt1Name;
    CEnumSelectionPoint eSelPt1;
    QPointF ptCnctPt1;
    QString strGraphObjKeyInTreeCnctPt2;
    QString strGraphObjCnctPt2Name;
    CEnumSelectionPoint eSelPt2;
    QPointF ptCnctPt2;
    QString strMethod;
    QString strMthArgs;
    QStringList strlstExpectedValues;

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpDrawConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;

    // Rectangle 1
    //------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect1;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("P1", QPoint(175.0, 225.0));
    //pTestStep->setConfigValue("P2", QPoint(225.0, 275.0));
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //m_ptPosRect1 = QPointF(200.0, 250.0);
    //QPointF ptCenterRect1(m_ptPosRect1.x(), bYAxisTopDown ? m_ptPosRect1.y() : fYAxisMaxVal - m_ptPosRect1.y());
    //QSizeF sizeRect1(50.0, 50.0);
    //m_rectRect1 = QRectF(QPointF(-sizeRect1.width()/2.0, -sizeRect1.height()/2.0), sizeRect1);
    //m_pPhysValRect1->setSize(sizeRect1);
    //m_pPhysValRect1->setCenter(ptCenterRect1);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect1, m_rectRect1, *m_pPhysValRect1, false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Rectangle 2
    ////------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect2;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("P1", QPoint(175.0, 325.0));
    //pTestStep->setConfigValue("P2", QPoint(225.0, 375.0));
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //m_ptPosRect2 = QPointF(200.0, 350.0);
    //QPointF ptCenterRect2(m_ptPosRect2.x(), bYAxisTopDown ? m_ptPosRect2.y() : fYAxisMaxVal - m_ptPosRect2.y());
    //QSizeF sizeRect2(50.0, 50.0);
    //m_rectRect2 = QRectF(QPointF(-sizeRect2.width()/2.0, -sizeRect2.height()/2.0), sizeRect2);
    //m_pPhysValRect2->setSize(sizeRect2);
    //m_pPhysValRect2->setCenter(ptCenterRect2);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect2, m_rectRect2, *m_pPhysValRect2, false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Rectangle 3
    ////------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect3;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("P1", QPoint(575.0, 225.0));
    //pTestStep->setConfigValue("P2", QPoint(625.0, 275.0));
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //m_ptPosRect3 = QPointF(600.0, 250.0);
    //QPointF ptCenterRect3(m_ptPosRect3.x(), bYAxisTopDown ? m_ptPosRect3.y() : fYAxisMaxVal - m_ptPosRect3.y());
    //QSizeF sizeRect3(50.0, 50.0);
    //m_rectRect3 = QRectF(QPointF(-sizeRect3.width()/2.0, -sizeRect3.height()/2.0), sizeRect3);
    //m_pPhysValRect3->setSize(sizeRect3);
    //m_pPhysValRect3->setCenter(ptCenterRect3);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect3, m_rectRect3, *m_pPhysValRect3, false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Rectangle 4
    ////------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect4;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("P1", QPoint(575.0, 325.0));
    //pTestStep->setConfigValue("P2", QPoint(625.0, 375.0));
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //m_ptPosRect4 = QPointF(600.0, 350.0);
    //QPointF ptCenterRect4(m_ptPosRect4.x(), bYAxisTopDown ? m_ptPosRect4.y() : fYAxisMaxVal - m_ptPosRect4.y());
    //QSizeF sizeRect4(50.0, 50.0);
    //m_rectRect4 = QRectF(QPointF(-sizeRect4.width()/2.0, -sizeRect4.height()/2.0), sizeRect4);
    //m_pPhysValRect4->setSize(sizeRect4);
    //m_pPhysValRect4->setCenter(ptCenterRect4);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect4, m_rectRect4, *m_pPhysValRect4, false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Connection Line: Rectangle 1 - BottomCenter -> Rectangle 2 - TopCenter
    ////-----------------------------------------------------------------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    //strGraphObjName = c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    { 0.0, -25.0},
    //    { 0.0,  25.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //// We need a start position, from which we move to the first connection point.
    //// Then we add the position of the two connection points.
    //// If more than 3 points are defined, the points from index 2 to the penultimate
    //// index are additional polygon points.
    //QPolygon points({
    //    QPoint(200, 300),
    //    QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
    //    QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
    //           bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
    //    QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    //});
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("Points", points);
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1BottomCenterRect2TopCenter, *m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Connection Line: Rectangle 1 - BottomCenter -> Rectangle 2 - TopCenter: setLineEndStyle(EndPoint, Arrow)
    ////---------------------------------------------------------------------------------------------------------

    //strMethod = "setDrawSettings";
    //strMthArgs = "{LineEndPoint, Arrow}";
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    //strlstExpectedValues.clear();
    //pTestStep->setExpectedValues(strlstExpectedValues);
    //pTestStep->addDataRow({
    //    {"Method", "setLineEndStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadBaseLineType"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadFillStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadWidth"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadLength"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    //});

    //// Connection Line: Rectangle 1 - RightCenter -> Rectangle 4 - LeftCenter
    ////-----------------------------------------------------------------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    //strGraphObjName = c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    { -175.0, -50.0},
    //    {  175.0,  50.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //// We need a start position, from which we move to the first connection point.
    //// Then we add the position of the two connection points.
    //// If more than 3 points are defined, the points from index 2 to the penultimate
    //// index are additional polygon points.
    //points = QPolygon({
    //    QPoint(200, 300),
    //    QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
    //    QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
    //           bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
    //    QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    //});
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("Points", points);
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1RightCenterRect4LeftCenter, *m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Connection Line: Rectangle 1 - RightCenter -> Rectangle 4 - LeftCenter: setLineEndStyle(EndPoint, Arrow)
    ////---------------------------------------------------------------------------------------------------------

    //strMethod = "setDrawSettings";
    //strMthArgs = "{LineEndPoint, Arrow}";
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    //strlstExpectedValues.clear();
    //pTestStep->setExpectedValues(strlstExpectedValues);
    //pTestStep->addDataRow({
    //    {"Method", "setLineEndStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadBaseLineType"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadFillStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadWidth"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadLength"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    //});

    //// Connection Line: Rectangle 3 - BottomCenter -> Rectangle 4 - TopCenter
    ////-----------------------------------------------------------------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    //strGraphObjName = c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    { 0.0, -25.0},
    //    { 0.0,  25.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //// We need a start position, from which we move to the first connection point.
    //// Then we add the position of the two connection points.
    //// If more than 3 points are defined, the points from index 2 to the penultimate
    //// index are additional polygon points.
    //points = QPolygon({
    //    QPoint(200, 300),
    //    QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
    //    QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
    //           bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
    //    QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    //});
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("Points", points);
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3BottomCenterRect4TopCenter, *m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Connection Line: Rectangle 3 - BottomCenter -> Rectangle 4 - TopCenter: setLineEndStyle(EndPoint, Arrow)
    ////---------------------------------------------------------------------------------------------------------

    //strMethod = "setDrawSettings";
    //strMthArgs = "{LineEndPoint, Arrow}";
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    //strlstExpectedValues.clear();
    //pTestStep->setExpectedValues(strlstExpectedValues);
    //pTestStep->addDataRow({
    //    {"Method", "setLineEndStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadBaseLineType"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadFillStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadWidth"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadLength"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    //});

    //// Connection Line: Rectangle 3 - LeftCenter -> Rectangle 2 - RightCenter
    ////-----------------------------------------------------------------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    //strGraphObjName = c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    {  175.0, -50.0},
    //    { -175.0,  50.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //// We need a start position, from which we move to the first connection point.
    //// Then we add the position of the two connection points.
    //// If more than 3 points are defined, the points from index 2 to the penultimate
    //// index are additional polygon points.
    //points = QPolygon({
    //    QPoint(200, 300),
    //    QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
    //    QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
    //           bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
    //    QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    //});
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("Points", points);
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3LeftCenterRect2RightCenter, *m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Connection Line: Rectangle 3 - LeftCenter -> Rectangle 2 - RightCenter: setLineEndStyle(EndPoint, Arrow)
    ////---------------------------------------------------------------------------------------------------------

    //strMethod = "setDrawSettings";
    //strMthArgs = "{LineEndPoint, Arrow}";
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    //strlstExpectedValues.clear();
    //pTestStep->setExpectedValues(strlstExpectedValues);
    //pTestStep->addDataRow({
    //    {"Method", "setLineEndStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadBaseLineType"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadFillStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadWidth"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadLength"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    //});

    //// Show Labels
    ////------------

    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Cncts.showLabels(" + CGraphObj::c_strLabelName + ")",
    //    /* strOperation    */ "Cncts.showLabels(" + CGraphObj::c_strLabelName + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    //strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1);
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect1},
    //    {"GraphObjKeyInTree", strGraphObjKeyInTree},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedText", c_strGraphObjNameRect1},
    //    {"ExpectedPos", QPointF(183.0, 242.0)}
    //});
    //strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2);
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect2},
    //    {"GraphObjKeyInTree", strGraphObjKeyInTree},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedText", c_strGraphObjNameRect2},
    //    {"ExpectedPos", QPointF(183.0, 342.0)}
    //});
    //strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3);
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect3},
    //    {"GraphObjKeyInTree", strGraphObjKeyInTree},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedText", c_strGraphObjNameRect3},
    //    {"ExpectedPos", QPointF(583.0, 242.0)}
    //});
    //strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4);
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect4},
    //    {"GraphObjKeyInTree", strGraphObjKeyInTree},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedText", c_strGraphObjNameRect4},
    //    {"ExpectedPos", QPointF(583.0, 342.0)}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"setPos", QPointF(100.0, 300.0)},
    //    {"ExpectedText", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"setPos", QPointF(300.0, 220.0)},
    //    {"ExpectedText", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"setPos", QPointF(500.0, 300.0)},
    //    {"ExpectedText", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"setPos", QPointF(300.0, 360.0)},
    //    {"ExpectedText", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter}
    //});
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsAnchoredToTextsModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsAnchoredToTextsModifications",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpModifyConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;

    QString strGraphObjType;
    QString strFactoryGroupName;
    QString strGraphObjName;
    QString strGraphObjKeyInTreeCnctPt1;
    QString strGraphObjCnctPt1Name;
    CEnumSelectionPoint eSelPt1;
    QPointF ptCnctPt1;
    QString strGraphObjKeyInTreeCnctPt2;
    QString strGraphObjCnctPt2Name;
    CEnumSelectionPoint eSelPt2;
    QPointF ptCnctPt2;
    QString strMethod;
    QString strMthArgs;

    // Hide Labels
    //------------

    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + ".hideLabels(" + CGraphObj::c_strLabelName + ")",
    //    /* strOperation    */ "CnctLines.hideLabels(" + CGraphObj::c_strLabelName + ")",
    //    /* pGrpParent      */ pGrpModifyConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepHideLabels(ZS::Test::CTestStep*)) );

    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect1},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect1]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameRect1 + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect2},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect2]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameRect2 + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect3},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect3]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameRect3 + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect4},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect4]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameRect4 + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter + "." + CGraphObj::c_strLabelName + " not found"}
    //});

    //// move Rectangle1
    ////----------------

    //strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect1;
    //strMethod = "move";
    //QPointF ptPosRect1New(300.0, 200.0);
    //strMthArgs = qPoint2Str(ptPosRect1New) + " " + strUnit;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpModifyConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
    //    {m_hshGraphObjNameToKeys[strGraphObjName]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]}});
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //pTestStep->addDataRow({
    //    {"Method", "setCurrentDrawingTool"},
    //    {"FactoryGroupName", ""},
    //    {"FactoryGraphObjType", ""}
    //});
    //// Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    //// while mouse is pressed and release the mouse event.
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 250), m_ptPosRect1.toPoint());
    //pTestStep->addDataRow({ // Select object by clicking on it
    //    {"Method", "mousePressEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect1.toPoint(), ptPosRect1New.toPoint(), 0, Qt::LeftButton);
    //pTestStep->addDataRow({
    //    {"Method", "mouseReleaseEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptPosRect1 = ptPosRect1New;
    //m_pPhysValRect1->setCenter(QPointF(ptPosRect1New.x(), bYAxisTopDown ? ptPosRect1New.y() : fYAxisMaxVal - ptPosRect1New.y()));
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect1, m_rectRect1, *m_pPhysValRect1, true, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    {  50.0, -50.0},
    //    { -50.0,  50.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1BottomCenterRect2TopCenter,
    //    *m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter,
    //    false, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    {-125.0, -75.0},
    //    { 125.0,  75.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1RightCenterRect4LeftCenter,
    //    *m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// move Rectangle2
    ////----------------

    //strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect2;
    //strMethod = "move";
    //QPointF ptPosRect2New(300.0, 400.0);
    //strMthArgs = qPoint2Str(ptPosRect2New) + " " + strUnit;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpModifyConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
    //    {m_hshGraphObjNameToKeys[strGraphObjName]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]}});
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //pTestStep->addDataRow({
    //    {"Method", "setCurrentDrawingTool"},
    //    {"FactoryGroupName", ""},
    //    {"FactoryGraphObjType", ""}
    //});
    //// Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    //// while mouse is pressed and release the mouse event.
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosRect2.toPoint());
    //pTestStep->addDataRow({ // Select object by clicking on it
    //    {"Method", "mousePressEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect2.toPoint(), ptPosRect2New.toPoint(), 0, Qt::LeftButton);
    //pTestStep->addDataRow({
    //    {"Method", "mouseReleaseEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptPosRect2 = ptPosRect2New;
    //m_pPhysValRect2->setCenter(QPointF(ptPosRect2New.x(), bYAxisTopDown ? ptPosRect2New.y() : fYAxisMaxVal - ptPosRect2New.y()));
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect2, m_rectRect2, *m_pPhysValRect2, true, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    {  0.0, -75.0},
    //    {  0.0,  75.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1BottomCenterRect2TopCenter,
    //    *m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter,
    //    false, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    { 125.0, -75.0},
    //    {-125.0,  75.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3LeftCenterRect2RightCenter,
    //    *m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// move Rectangle3
    ////----------------

    //strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect3;
    //strMethod = "move";
    //QPointF ptPosRect3New(500.0, 200.0);
    //strMthArgs = qPoint2Str(ptPosRect3New) + " " + strUnit;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpModifyConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
    //    {m_hshGraphObjNameToKeys[strGraphObjName]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]}});
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //pTestStep->addDataRow({
    //    {"Method", "setCurrentDrawingTool"},
    //    {"FactoryGroupName", ""},
    //    {"FactoryGraphObjType", ""}
    //});
    //// Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    //// while mouse is pressed and release the mouse event.
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosRect3.toPoint());
    //pTestStep->addDataRow({ // Select object by clicking on it
    //    {"Method", "mousePressEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect3.toPoint(), ptPosRect3New.toPoint(), 0, Qt::LeftButton);
    //pTestStep->addDataRow({
    //    {"Method", "mouseReleaseEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptPosRect3 = ptPosRect3New;
    //m_pPhysValRect3->setCenter(QPointF(ptPosRect3New.x(), bYAxisTopDown ? ptPosRect3New.y() : fYAxisMaxVal - ptPosRect3New.y()));
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect3, m_rectRect3, *m_pPhysValRect3, true, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    { -50.0, -50.0},
    //    {  50.0,  50.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3BottomCenterRect4TopCenter,
    //    *m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter,
    //    false, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    {  75.0, -100.0},
    //    { -75.0,  100.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3LeftCenterRect2RightCenter,
    //    *m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// move Rectangle4
    ////----------------

    //strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect4;
    //QPointF ptPosRect4New(500.0, 400.0);
    //strMthArgs = qPoint2Str(ptPosRect4New) + " " + strUnit;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpModifyConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
    //    {m_hshGraphObjNameToKeys[strGraphObjName]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]}});
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //pTestStep->addDataRow({
    //    {"Method", "setCurrentDrawingTool"},
    //    {"FactoryGroupName", ""},
    //    {"FactoryGraphObjType", ""}
    //});
    //// Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    //// while mouse is pressed and release the mouse event.
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosRect4.toPoint());
    //pTestStep->addDataRow({ // Select object by clicking on it
    //    {"Method", "mousePressEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect4.toPoint(), ptPosRect4New.toPoint(), 0, Qt::LeftButton);
    //pTestStep->addDataRow({
    //    {"Method", "mouseReleaseEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptPosRect4 = ptPosRect4New;
    //m_pPhysValRect4->setCenter(QPointF(ptPosRect4New.x(), bYAxisTopDown ? ptPosRect4New.y() : fYAxisMaxVal - ptPosRect4New.y()));
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect4, m_rectRect4, *m_pPhysValRect4, true, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    {  0.0, -75.0},
    //    {  0.0,  75.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3BottomCenterRect4TopCenter,
    //    *m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter,
    //    false, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    { -75.0, -100.0},
    //    {  75.0,  100.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1RightCenterRect4LeftCenter,
    //    *m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsAnchoredToBitmaps(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsAnchoredToBitmaps",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameConnections;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strGraphObjFactoryGroup = CObjFactory::c_strGroupNameConnections;
    QString strGraphObjKeyInTree;
    QString strGraphObjType;
    QString strGraphObjName;
    QString strGraphObjKeyInTreeCnctPt1;
    QString strGraphObjCnctPt1Name;
    CEnumSelectionPoint eSelPt1;
    QPointF ptCnctPt1;
    QString strGraphObjKeyInTreeCnctPt2;
    QString strGraphObjCnctPt2Name;
    CEnumSelectionPoint eSelPt2;
    QPointF ptCnctPt2;
    QString strMethod;
    QString strMthArgs;
    QStringList strlstExpectedValues;

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpDrawConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;

    // Rectangle 1
    //------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect1;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("P1", QPoint(175.0, 225.0));
    //pTestStep->setConfigValue("P2", QPoint(225.0, 275.0));
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //m_ptPosRect1 = QPointF(200.0, 250.0);
    //QPointF ptCenterRect1(m_ptPosRect1.x(), bYAxisTopDown ? m_ptPosRect1.y() : fYAxisMaxVal - m_ptPosRect1.y());
    //QSizeF sizeRect1(50.0, 50.0);
    //m_rectRect1 = QRectF(QPointF(-sizeRect1.width()/2.0, -sizeRect1.height()/2.0), sizeRect1);
    //m_pPhysValRect1->setSize(sizeRect1);
    //m_pPhysValRect1->setCenter(ptCenterRect1);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect1, m_rectRect1, *m_pPhysValRect1, false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Rectangle 2
    ////------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect2;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("P1", QPoint(175.0, 325.0));
    //pTestStep->setConfigValue("P2", QPoint(225.0, 375.0));
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //m_ptPosRect2 = QPointF(200.0, 350.0);
    //QPointF ptCenterRect2(m_ptPosRect2.x(), bYAxisTopDown ? m_ptPosRect2.y() : fYAxisMaxVal - m_ptPosRect2.y());
    //QSizeF sizeRect2(50.0, 50.0);
    //m_rectRect2 = QRectF(QPointF(-sizeRect2.width()/2.0, -sizeRect2.height()/2.0), sizeRect2);
    //m_pPhysValRect2->setSize(sizeRect2);
    //m_pPhysValRect2->setCenter(ptCenterRect2);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect2, m_rectRect2, *m_pPhysValRect2, false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Rectangle 3
    ////------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect3;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("P1", QPoint(575.0, 225.0));
    //pTestStep->setConfigValue("P2", QPoint(625.0, 275.0));
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //m_ptPosRect3 = QPointF(600.0, 250.0);
    //QPointF ptCenterRect3(m_ptPosRect3.x(), bYAxisTopDown ? m_ptPosRect3.y() : fYAxisMaxVal - m_ptPosRect3.y());
    //QSizeF sizeRect3(50.0, 50.0);
    //m_rectRect3 = QRectF(QPointF(-sizeRect3.width()/2.0, -sizeRect3.height()/2.0), sizeRect3);
    //m_pPhysValRect3->setSize(sizeRect3);
    //m_pPhysValRect3->setCenter(ptCenterRect3);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect3, m_rectRect3, *m_pPhysValRect3, false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Rectangle 4
    ////------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect4;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("P1", QPoint(575.0, 325.0));
    //pTestStep->setConfigValue("P2", QPoint(625.0, 375.0));
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //m_ptPosRect4 = QPointF(600.0, 350.0);
    //QPointF ptCenterRect4(m_ptPosRect4.x(), bYAxisTopDown ? m_ptPosRect4.y() : fYAxisMaxVal - m_ptPosRect4.y());
    //QSizeF sizeRect4(50.0, 50.0);
    //m_rectRect4 = QRectF(QPointF(-sizeRect4.width()/2.0, -sizeRect4.height()/2.0), sizeRect4);
    //m_pPhysValRect4->setSize(sizeRect4);
    //m_pPhysValRect4->setCenter(ptCenterRect4);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect4, m_rectRect4, *m_pPhysValRect4, false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Connection Line: Rectangle 1 - BottomCenter -> Rectangle 2 - TopCenter
    ////-----------------------------------------------------------------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    //strGraphObjName = c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    { 0.0, -25.0},
    //    { 0.0,  25.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //// We need a start position, from which we move to the first connection point.
    //// Then we add the position of the two connection points.
    //// If more than 3 points are defined, the points from index 2 to the penultimate
    //// index are additional polygon points.
    //QPolygon points({
    //    QPoint(200, 300),
    //    QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
    //    QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
    //           bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
    //    QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    //});
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("Points", points);
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1BottomCenterRect2TopCenter, *m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Connection Line: Rectangle 1 - BottomCenter -> Rectangle 2 - TopCenter: setLineEndStyle(EndPoint, Arrow)
    ////---------------------------------------------------------------------------------------------------------

    //strMethod = "setDrawSettings";
    //strMthArgs = "{LineEndPoint, Arrow}";
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    //strlstExpectedValues.clear();
    //pTestStep->setExpectedValues(strlstExpectedValues);
    //pTestStep->addDataRow({
    //    {"Method", "setLineEndStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadBaseLineType"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadFillStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadWidth"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadLength"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    //});

    //// Connection Line: Rectangle 1 - RightCenter -> Rectangle 4 - LeftCenter
    ////-----------------------------------------------------------------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    //strGraphObjName = c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    { -175.0, -50.0},
    //    {  175.0,  50.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //// We need a start position, from which we move to the first connection point.
    //// Then we add the position of the two connection points.
    //// If more than 3 points are defined, the points from index 2 to the penultimate
    //// index are additional polygon points.
    //points = QPolygon({
    //    QPoint(200, 300),
    //    QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
    //    QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
    //           bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
    //    QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    //});
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("Points", points);
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1RightCenterRect4LeftCenter, *m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Connection Line: Rectangle 1 - RightCenter -> Rectangle 4 - LeftCenter: setLineEndStyle(EndPoint, Arrow)
    ////---------------------------------------------------------------------------------------------------------

    //strMethod = "setDrawSettings";
    //strMthArgs = "{LineEndPoint, Arrow}";
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    //strlstExpectedValues.clear();
    //pTestStep->setExpectedValues(strlstExpectedValues);
    //pTestStep->addDataRow({
    //    {"Method", "setLineEndStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadBaseLineType"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadFillStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadWidth"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadLength"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    //});

    //// Connection Line: Rectangle 3 - BottomCenter -> Rectangle 4 - TopCenter
    ////-----------------------------------------------------------------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    //strGraphObjName = c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    { 0.0, -25.0},
    //    { 0.0,  25.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //// We need a start position, from which we move to the first connection point.
    //// Then we add the position of the two connection points.
    //// If more than 3 points are defined, the points from index 2 to the penultimate
    //// index are additional polygon points.
    //points = QPolygon({
    //    QPoint(200, 300),
    //    QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
    //    QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
    //           bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
    //    QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    //});
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("Points", points);
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3BottomCenterRect4TopCenter, *m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Connection Line: Rectangle 3 - BottomCenter -> Rectangle 4 - TopCenter: setLineEndStyle(EndPoint, Arrow)
    ////---------------------------------------------------------------------------------------------------------

    //strMethod = "setDrawSettings";
    //strMthArgs = "{LineEndPoint, Arrow}";
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    //strlstExpectedValues.clear();
    //pTestStep->setExpectedValues(strlstExpectedValues);
    //pTestStep->addDataRow({
    //    {"Method", "setLineEndStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadBaseLineType"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadFillStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadWidth"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadLength"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    //});

    //// Connection Line: Rectangle 3 - LeftCenter -> Rectangle 2 - RightCenter
    ////-----------------------------------------------------------------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    //strGraphObjName = c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    {  175.0, -50.0},
    //    { -175.0,  50.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //// We need a start position, from which we move to the first connection point.
    //// Then we add the position of the two connection points.
    //// If more than 3 points are defined, the points from index 2 to the penultimate
    //// index are additional polygon points.
    //points = QPolygon({
    //    QPoint(200, 300),
    //    QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
    //    QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
    //           bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
    //    QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    //});
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("Points", points);
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3LeftCenterRect2RightCenter, *m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Connection Line: Rectangle 3 - LeftCenter -> Rectangle 2 - RightCenter: setLineEndStyle(EndPoint, Arrow)
    ////---------------------------------------------------------------------------------------------------------

    //strMethod = "setDrawSettings";
    //strMthArgs = "{LineEndPoint, Arrow}";
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    //strlstExpectedValues.clear();
    //pTestStep->setExpectedValues(strlstExpectedValues);
    //pTestStep->addDataRow({
    //    {"Method", "setLineEndStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadBaseLineType"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadFillStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadWidth"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadLength"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    //});

    //// Show Labels
    ////------------

    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Cncts.showLabels(" + CGraphObj::c_strLabelName + ")",
    //    /* strOperation    */ "Cncts.showLabels(" + CGraphObj::c_strLabelName + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    //strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1);
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect1},
    //    {"GraphObjKeyInTree", strGraphObjKeyInTree},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedText", c_strGraphObjNameRect1},
    //    {"ExpectedPos", QPointF(183.0, 242.0)}
    //});
    //strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2);
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect2},
    //    {"GraphObjKeyInTree", strGraphObjKeyInTree},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedText", c_strGraphObjNameRect2},
    //    {"ExpectedPos", QPointF(183.0, 342.0)}
    //});
    //strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3);
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect3},
    //    {"GraphObjKeyInTree", strGraphObjKeyInTree},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedText", c_strGraphObjNameRect3},
    //    {"ExpectedPos", QPointF(583.0, 242.0)}
    //});
    //strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4);
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect4},
    //    {"GraphObjKeyInTree", strGraphObjKeyInTree},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedText", c_strGraphObjNameRect4},
    //    {"ExpectedPos", QPointF(583.0, 342.0)}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"setPos", QPointF(100.0, 300.0)},
    //    {"ExpectedText", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"setPos", QPointF(300.0, 220.0)},
    //    {"ExpectedText", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"setPos", QPointF(500.0, 300.0)},
    //    {"ExpectedText", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"setPos", QPointF(300.0, 360.0)},
    //    {"ExpectedText", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter}
    //});
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsAnchoredToBitmapsModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsAnchoredToBitmapsModifications",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpModifyConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;

    QString strGraphObjType;
    QString strFactoryGroupName;
    QString strGraphObjName;
    QString strGraphObjKeyInTreeCnctPt1;
    QString strGraphObjCnctPt1Name;
    CEnumSelectionPoint eSelPt1;
    QPointF ptCnctPt1;
    QString strGraphObjKeyInTreeCnctPt2;
    QString strGraphObjCnctPt2Name;
    CEnumSelectionPoint eSelPt2;
    QPointF ptCnctPt2;
    QString strMethod;
    QString strMthArgs;

    // Hide Labels
    //------------

    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + ".hideLabels(" + CGraphObj::c_strLabelName + ")",
    //    /* strOperation    */ "CnctLines.hideLabels(" + CGraphObj::c_strLabelName + ")",
    //    /* pGrpParent      */ pGrpModifyConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepHideLabels(ZS::Test::CTestStep*)) );

    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect1},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect1]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameRect1 + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect2},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect2]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameRect2 + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect3},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect3]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameRect3 + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect4},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect4]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameRect4 + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter + "." + CGraphObj::c_strLabelName + " not found"}
    //});

    //// move Rectangle1
    ////----------------

    //strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect1;
    //strMethod = "move";
    //QPointF ptPosRect1New(300.0, 200.0);
    //strMthArgs = qPoint2Str(ptPosRect1New) + " " + strUnit;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpModifyConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
    //    {m_hshGraphObjNameToKeys[strGraphObjName]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]}});
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //pTestStep->addDataRow({
    //    {"Method", "setCurrentDrawingTool"},
    //    {"FactoryGroupName", ""},
    //    {"FactoryGraphObjType", ""}
    //});
    //// Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    //// while mouse is pressed and release the mouse event.
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 250), m_ptPosRect1.toPoint());
    //pTestStep->addDataRow({ // Select object by clicking on it
    //    {"Method", "mousePressEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect1.toPoint(), ptPosRect1New.toPoint(), 0, Qt::LeftButton);
    //pTestStep->addDataRow({
    //    {"Method", "mouseReleaseEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptPosRect1 = ptPosRect1New;
    //m_pPhysValRect1->setCenter(QPointF(ptPosRect1New.x(), bYAxisTopDown ? ptPosRect1New.y() : fYAxisMaxVal - ptPosRect1New.y()));
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect1, m_rectRect1, *m_pPhysValRect1, true, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    {  50.0, -50.0},
    //    { -50.0,  50.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1BottomCenterRect2TopCenter,
    //    *m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter,
    //    false, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    {-125.0, -75.0},
    //    { 125.0,  75.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1RightCenterRect4LeftCenter,
    //    *m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// move Rectangle2
    ////----------------

    //strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect2;
    //strMethod = "move";
    //QPointF ptPosRect2New(300.0, 400.0);
    //strMthArgs = qPoint2Str(ptPosRect2New) + " " + strUnit;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpModifyConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
    //    {m_hshGraphObjNameToKeys[strGraphObjName]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]}});
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //pTestStep->addDataRow({
    //    {"Method", "setCurrentDrawingTool"},
    //    {"FactoryGroupName", ""},
    //    {"FactoryGraphObjType", ""}
    //});
    //// Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    //// while mouse is pressed and release the mouse event.
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosRect2.toPoint());
    //pTestStep->addDataRow({ // Select object by clicking on it
    //    {"Method", "mousePressEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect2.toPoint(), ptPosRect2New.toPoint(), 0, Qt::LeftButton);
    //pTestStep->addDataRow({
    //    {"Method", "mouseReleaseEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptPosRect2 = ptPosRect2New;
    //m_pPhysValRect2->setCenter(QPointF(ptPosRect2New.x(), bYAxisTopDown ? ptPosRect2New.y() : fYAxisMaxVal - ptPosRect2New.y()));
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect2, m_rectRect2, *m_pPhysValRect2, true, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    {  0.0, -75.0},
    //    {  0.0,  75.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1BottomCenterRect2TopCenter,
    //    *m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter,
    //    false, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    { 125.0, -75.0},
    //    {-125.0,  75.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3LeftCenterRect2RightCenter,
    //    *m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// move Rectangle3
    ////----------------

    //strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect3;
    //strMethod = "move";
    //QPointF ptPosRect3New(500.0, 200.0);
    //strMthArgs = qPoint2Str(ptPosRect3New) + " " + strUnit;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpModifyConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
    //    {m_hshGraphObjNameToKeys[strGraphObjName]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]}});
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //pTestStep->addDataRow({
    //    {"Method", "setCurrentDrawingTool"},
    //    {"FactoryGroupName", ""},
    //    {"FactoryGraphObjType", ""}
    //});
    //// Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    //// while mouse is pressed and release the mouse event.
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosRect3.toPoint());
    //pTestStep->addDataRow({ // Select object by clicking on it
    //    {"Method", "mousePressEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect3.toPoint(), ptPosRect3New.toPoint(), 0, Qt::LeftButton);
    //pTestStep->addDataRow({
    //    {"Method", "mouseReleaseEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptPosRect3 = ptPosRect3New;
    //m_pPhysValRect3->setCenter(QPointF(ptPosRect3New.x(), bYAxisTopDown ? ptPosRect3New.y() : fYAxisMaxVal - ptPosRect3New.y()));
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect3, m_rectRect3, *m_pPhysValRect3, true, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    { -50.0, -50.0},
    //    {  50.0,  50.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3BottomCenterRect4TopCenter,
    //    *m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter,
    //    false, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    {  75.0, -100.0},
    //    { -75.0,  100.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3LeftCenterRect2RightCenter,
    //    *m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// move Rectangle4
    ////----------------

    //strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect4;
    //QPointF ptPosRect4New(500.0, 400.0);
    //strMthArgs = qPoint2Str(ptPosRect4New) + " " + strUnit;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpModifyConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
    //    {m_hshGraphObjNameToKeys[strGraphObjName]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]}});
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //pTestStep->addDataRow({
    //    {"Method", "setCurrentDrawingTool"},
    //    {"FactoryGroupName", ""},
    //    {"FactoryGraphObjType", ""}
    //});
    //// Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    //// while mouse is pressed and release the mouse event.
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosRect4.toPoint());
    //pTestStep->addDataRow({ // Select object by clicking on it
    //    {"Method", "mousePressEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect4.toPoint(), ptPosRect4New.toPoint(), 0, Qt::LeftButton);
    //pTestStep->addDataRow({
    //    {"Method", "mouseReleaseEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptPosRect4 = ptPosRect4New;
    //m_pPhysValRect4->setCenter(QPointF(ptPosRect4New.x(), bYAxisTopDown ? ptPosRect4New.y() : fYAxisMaxVal - ptPosRect4New.y()));
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect4, m_rectRect4, *m_pPhysValRect4, true, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    {  0.0, -75.0},
    //    {  0.0,  75.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3BottomCenterRect4TopCenter,
    //    *m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter,
    //    false, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    { -75.0, -100.0},
    //    {  75.0,  100.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1RightCenterRect4LeftCenter,
    //    *m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsAnchoredToGroups(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsAnchoredToGroups",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameConnections;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strGraphObjFactoryGroup = CObjFactory::c_strGroupNameConnections;
    QString strGraphObjKeyInTree;
    QString strGraphObjType;
    QString strGraphObjName;
    QString strGraphObjKeyInTreeCnctPt1;
    QString strGraphObjCnctPt1Name;
    CEnumSelectionPoint eSelPt1;
    QPointF ptCnctPt1;
    QString strGraphObjKeyInTreeCnctPt2;
    QString strGraphObjCnctPt2Name;
    CEnumSelectionPoint eSelPt2;
    QPointF ptCnctPt2;
    QString strMethod;
    QString strMthArgs;
    QStringList strlstExpectedValues;

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpDrawConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;

    // Rectangle 1
    //------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect1;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("P1", QPoint(175.0, 225.0));
    //pTestStep->setConfigValue("P2", QPoint(225.0, 275.0));
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //m_ptPosRect1 = QPointF(200.0, 250.0);
    //QPointF ptCenterRect1(m_ptPosRect1.x(), bYAxisTopDown ? m_ptPosRect1.y() : fYAxisMaxVal - m_ptPosRect1.y());
    //QSizeF sizeRect1(50.0, 50.0);
    //m_rectRect1 = QRectF(QPointF(-sizeRect1.width()/2.0, -sizeRect1.height()/2.0), sizeRect1);
    //m_pPhysValRect1->setSize(sizeRect1);
    //m_pPhysValRect1->setCenter(ptCenterRect1);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect1, m_rectRect1, *m_pPhysValRect1, false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Rectangle 2
    ////------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect2;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("P1", QPoint(175.0, 325.0));
    //pTestStep->setConfigValue("P2", QPoint(225.0, 375.0));
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //m_ptPosRect2 = QPointF(200.0, 350.0);
    //QPointF ptCenterRect2(m_ptPosRect2.x(), bYAxisTopDown ? m_ptPosRect2.y() : fYAxisMaxVal - m_ptPosRect2.y());
    //QSizeF sizeRect2(50.0, 50.0);
    //m_rectRect2 = QRectF(QPointF(-sizeRect2.width()/2.0, -sizeRect2.height()/2.0), sizeRect2);
    //m_pPhysValRect2->setSize(sizeRect2);
    //m_pPhysValRect2->setCenter(ptCenterRect2);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect2, m_rectRect2, *m_pPhysValRect2, false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Rectangle 3
    ////------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect3;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("P1", QPoint(575.0, 225.0));
    //pTestStep->setConfigValue("P2", QPoint(625.0, 275.0));
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //m_ptPosRect3 = QPointF(600.0, 250.0);
    //QPointF ptCenterRect3(m_ptPosRect3.x(), bYAxisTopDown ? m_ptPosRect3.y() : fYAxisMaxVal - m_ptPosRect3.y());
    //QSizeF sizeRect3(50.0, 50.0);
    //m_rectRect3 = QRectF(QPointF(-sizeRect3.width()/2.0, -sizeRect3.height()/2.0), sizeRect3);
    //m_pPhysValRect3->setSize(sizeRect3);
    //m_pPhysValRect3->setCenter(ptCenterRect3);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect3, m_rectRect3, *m_pPhysValRect3, false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Rectangle 4
    ////------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect4;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("P1", QPoint(575.0, 325.0));
    //pTestStep->setConfigValue("P2", QPoint(625.0, 375.0));
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //m_ptPosRect4 = QPointF(600.0, 350.0);
    //QPointF ptCenterRect4(m_ptPosRect4.x(), bYAxisTopDown ? m_ptPosRect4.y() : fYAxisMaxVal - m_ptPosRect4.y());
    //QSizeF sizeRect4(50.0, 50.0);
    //m_rectRect4 = QRectF(QPointF(-sizeRect4.width()/2.0, -sizeRect4.height()/2.0), sizeRect4);
    //m_pPhysValRect4->setSize(sizeRect4);
    //m_pPhysValRect4->setCenter(ptCenterRect4);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect4, m_rectRect4, *m_pPhysValRect4, false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Connection Line: Rectangle 1 - BottomCenter -> Rectangle 2 - TopCenter
    ////-----------------------------------------------------------------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    //strGraphObjName = c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    { 0.0, -25.0},
    //    { 0.0,  25.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //// We need a start position, from which we move to the first connection point.
    //// Then we add the position of the two connection points.
    //// If more than 3 points are defined, the points from index 2 to the penultimate
    //// index are additional polygon points.
    //QPolygon points({
    //    QPoint(200, 300),
    //    QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
    //    QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
    //           bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
    //    QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    //});
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("Points", points);
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1BottomCenterRect2TopCenter, *m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Connection Line: Rectangle 1 - BottomCenter -> Rectangle 2 - TopCenter: setLineEndStyle(EndPoint, Arrow)
    ////---------------------------------------------------------------------------------------------------------

    //strMethod = "setDrawSettings";
    //strMthArgs = "{LineEndPoint, Arrow}";
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    //strlstExpectedValues.clear();
    //pTestStep->setExpectedValues(strlstExpectedValues);
    //pTestStep->addDataRow({
    //    {"Method", "setLineEndStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadBaseLineType"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadFillStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadWidth"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadLength"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    //});

    //// Connection Line: Rectangle 1 - RightCenter -> Rectangle 4 - LeftCenter
    ////-----------------------------------------------------------------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    //strGraphObjName = c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    { -175.0, -50.0},
    //    {  175.0,  50.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //// We need a start position, from which we move to the first connection point.
    //// Then we add the position of the two connection points.
    //// If more than 3 points are defined, the points from index 2 to the penultimate
    //// index are additional polygon points.
    //points = QPolygon({
    //    QPoint(200, 300),
    //    QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
    //    QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
    //           bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
    //    QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    //});
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("Points", points);
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1RightCenterRect4LeftCenter, *m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Connection Line: Rectangle 1 - RightCenter -> Rectangle 4 - LeftCenter: setLineEndStyle(EndPoint, Arrow)
    ////---------------------------------------------------------------------------------------------------------

    //strMethod = "setDrawSettings";
    //strMthArgs = "{LineEndPoint, Arrow}";
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    //strlstExpectedValues.clear();
    //pTestStep->setExpectedValues(strlstExpectedValues);
    //pTestStep->addDataRow({
    //    {"Method", "setLineEndStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadBaseLineType"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadFillStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadWidth"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadLength"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    //});

    //// Connection Line: Rectangle 3 - BottomCenter -> Rectangle 4 - TopCenter
    ////-----------------------------------------------------------------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    //strGraphObjName = c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    { 0.0, -25.0},
    //    { 0.0,  25.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //// We need a start position, from which we move to the first connection point.
    //// Then we add the position of the two connection points.
    //// If more than 3 points are defined, the points from index 2 to the penultimate
    //// index are additional polygon points.
    //points = QPolygon({
    //    QPoint(200, 300),
    //    QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
    //    QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
    //           bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
    //    QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    //});
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("Points", points);
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3BottomCenterRect4TopCenter, *m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Connection Line: Rectangle 3 - BottomCenter -> Rectangle 4 - TopCenter: setLineEndStyle(EndPoint, Arrow)
    ////---------------------------------------------------------------------------------------------------------

    //strMethod = "setDrawSettings";
    //strMthArgs = "{LineEndPoint, Arrow}";
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    //strlstExpectedValues.clear();
    //pTestStep->setExpectedValues(strlstExpectedValues);
    //pTestStep->addDataRow({
    //    {"Method", "setLineEndStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadBaseLineType"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadFillStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadWidth"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadLength"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    //});

    //// Connection Line: Rectangle 3 - LeftCenter -> Rectangle 2 - RightCenter
    ////-----------------------------------------------------------------------

    //strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    //strGraphObjName = c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter;
    //strMethod = "DrawingScene.drawGraphObj";
    //strMthArgs = strGraphObjName;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    //m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    {  175.0, -50.0},
    //    { -175.0,  50.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //// We need a start position, from which we move to the first connection point.
    //// Then we add the position of the two connection points.
    //// If more than 3 points are defined, the points from index 2 to the penultimate
    //// index are additional polygon points.
    //points = QPolygon({
    //    QPoint(200, 300),
    //    QPoint(ptCnctPt1.toPoint().x(), bYAxisTopDown ? ptCnctPt1.toPoint().y() - 1 : fYAxisMaxVal - ptCnctPt1.toPoint().y() - 1),
    //    QPoint((ptCnctPt1.x() + ptCnctPt2.x()) / 2,
    //           bYAxisTopDown ? (ptCnctPt1.y() + ptCnctPt2.y()) / 2 : fYAxisMaxVal - (ptCnctPt1.y() + ptCnctPt2.y()) / 2),
    //    QPoint(ptCnctPt2.toPoint().x(), bYAxisTopDown ? ptCnctPt2.toPoint().y() + 1 : fYAxisMaxVal - ptCnctPt2.toPoint().y() + 1),
    //});
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("Points", points);
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    strGraphObjName, strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3LeftCenterRect2RightCenter, *m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// Connection Line: Rectangle 3 - LeftCenter -> Rectangle 2 - RightCenter: setLineEndStyle(EndPoint, Arrow)
    ////---------------------------------------------------------------------------------------------------------

    //strMethod = "setDrawSettings";
    //strMthArgs = "{LineEndPoint, Arrow}";
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("ImmediatelyApplySettings", false);
    //strlstExpectedValues.clear();
    //pTestStep->setExpectedValues(strlstExpectedValues);
    //pTestStep->addDataRow({
    //    {"Method", "setLineEndStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"LineEndStyle", CEnumLineEndStyle(ELineEndStyle::ArrowHead).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadBaseLineType"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadBaseLineType", CEnumArrowHeadBaseLineType(EArrowHeadBaseLineType::Normal).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadFillStyle"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadFillStyle", CEnumArrowHeadFillStyle(EArrowHeadFillStyle::SolidPattern).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadWidth"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadWidth", CEnumArrowHeadWidth(EArrowHeadWidth::Thin).toString()},
    //});
    //pTestStep->addDataRow({
    //    {"Method", "setArrowHeadLength"},
    //    {"LinePoint", CEnumLinePoint(ELinePoint::End).toString()},
    //    {"ArrowHeadLength", CEnumArrowHeadLength(EArrowHeadLength::Medium).toString()},
    //});

    //// Show Labels
    ////------------

    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Cncts.showLabels(" + CGraphObj::c_strLabelName + ")",
    //    /* strOperation    */ "Cncts.showLabels(" + CGraphObj::c_strLabelName + ")",
    //    /* pGrpParent      */ pGrpDrawConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    //strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1);
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect1},
    //    {"GraphObjKeyInTree", strGraphObjKeyInTree},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedText", c_strGraphObjNameRect1},
    //    {"ExpectedPos", QPointF(183.0, 242.0)}
    //});
    //strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2);
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect2},
    //    {"GraphObjKeyInTree", strGraphObjKeyInTree},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedText", c_strGraphObjNameRect2},
    //    {"ExpectedPos", QPointF(183.0, 342.0)}
    //});
    //strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3);
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect3},
    //    {"GraphObjKeyInTree", strGraphObjKeyInTree},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedText", c_strGraphObjNameRect3},
    //    {"ExpectedPos", QPointF(583.0, 242.0)}
    //});
    //strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4);
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect4},
    //    {"GraphObjKeyInTree", strGraphObjKeyInTree},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedText", c_strGraphObjNameRect4},
    //    {"ExpectedPos", QPointF(583.0, 342.0)}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"setPos", QPointF(100.0, 300.0)},
    //    {"ExpectedText", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"setPos", QPointF(300.0, 220.0)},
    //    {"ExpectedText", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"setPos", QPointF(500.0, 300.0)},
    //    {"ExpectedText", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"setPos", QPointF(300.0, 360.0)},
    //    {"ExpectedText", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter}
    //});
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionPointsAnchoredToGroupsModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionPointsAnchoredToGroupsModifications",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpModifyConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;

    QString strGraphObjType;
    QString strFactoryGroupName;
    QString strGraphObjName;
    QString strGraphObjKeyInTreeCnctPt1;
    QString strGraphObjCnctPt1Name;
    CEnumSelectionPoint eSelPt1;
    QPointF ptCnctPt1;
    QString strGraphObjKeyInTreeCnctPt2;
    QString strGraphObjCnctPt2Name;
    CEnumSelectionPoint eSelPt2;
    QPointF ptCnctPt2;
    QString strMethod;
    QString strMthArgs;

    // Hide Labels
    //------------

    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + ".hideLabels(" + CGraphObj::c_strLabelName + ")",
    //    /* strOperation    */ "CnctLines.hideLabels(" + CGraphObj::c_strLabelName + ")",
    //    /* pGrpParent      */ pGrpModifyConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepHideLabels(ZS::Test::CTestStep*)) );

    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect1},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect1]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameRect1 + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect2},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect2]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameRect2 + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect3},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect3]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameRect3 + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameRect4},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameRect4]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameRect4 + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter + "." + CGraphObj::c_strLabelName + " not found"}
    //});
    //pTestStep->addDataRow({
    //    {"GraphObjName", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter},
    //    {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]},
    //    {"LabelName", CGraphObj::c_strLabelName},
    //    {"ExpectedValue", c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter + "." + CGraphObj::c_strLabelName + " not found"}
    //});

    //// move Rectangle1
    ////----------------

    //strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect1;
    //strMethod = "move";
    //QPointF ptPosRect1New(300.0, 200.0);
    //strMthArgs = qPoint2Str(ptPosRect1New) + " " + strUnit;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpModifyConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
    //    {m_hshGraphObjNameToKeys[strGraphObjName]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]}});
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //pTestStep->addDataRow({
    //    {"Method", "setCurrentDrawingTool"},
    //    {"FactoryGroupName", ""},
    //    {"FactoryGraphObjType", ""}
    //});
    //// Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    //// while mouse is pressed and release the mouse event.
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 250), m_ptPosRect1.toPoint());
    //pTestStep->addDataRow({ // Select object by clicking on it
    //    {"Method", "mousePressEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect1.toPoint(), ptPosRect1New.toPoint(), 0, Qt::LeftButton);
    //pTestStep->addDataRow({
    //    {"Method", "mouseReleaseEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptPosRect1 = ptPosRect1New;
    //m_pPhysValRect1->setCenter(QPointF(ptPosRect1New.x(), bYAxisTopDown ? ptPosRect1New.y() : fYAxisMaxVal - ptPosRect1New.y()));
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect1, m_rectRect1, *m_pPhysValRect1, true, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    {  50.0, -50.0},
    //    { -50.0,  50.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1BottomCenterRect2TopCenter,
    //    *m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter,
    //    false, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    {-125.0, -75.0},
    //    { 125.0,  75.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1RightCenterRect4LeftCenter,
    //    *m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// move Rectangle2
    ////----------------

    //strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect2;
    //strMethod = "move";
    //QPointF ptPosRect2New(300.0, 400.0);
    //strMthArgs = qPoint2Str(ptPosRect2New) + " " + strUnit;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpModifyConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
    //    {m_hshGraphObjNameToKeys[strGraphObjName]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]}});
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //pTestStep->addDataRow({
    //    {"Method", "setCurrentDrawingTool"},
    //    {"FactoryGroupName", ""},
    //    {"FactoryGraphObjType", ""}
    //});
    //// Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    //// while mouse is pressed and release the mouse event.
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosRect2.toPoint());
    //pTestStep->addDataRow({ // Select object by clicking on it
    //    {"Method", "mousePressEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect2.toPoint(), ptPosRect2New.toPoint(), 0, Qt::LeftButton);
    //pTestStep->addDataRow({
    //    {"Method", "mouseReleaseEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptPosRect2 = ptPosRect2New;
    //m_pPhysValRect2->setCenter(QPointF(ptPosRect2New.x(), bYAxisTopDown ? ptPosRect2New.y() : fYAxisMaxVal - ptPosRect2New.y()));
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect2, m_rectRect2, *m_pPhysValRect2, true, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    {  0.0, -75.0},
    //    {  0.0,  75.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect1BottomCenterRect2TopCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1BottomCenterRect2TopCenter,
    //    *m_pPhysValPolygonConnectionLineRect1BottomCenterRect2TopCenter,
    //    false, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    { 125.0, -75.0},
    //    {-125.0,  75.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3LeftCenterRect2RightCenter,
    //    *m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// move Rectangle3
    ////----------------

    //strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect3;
    //strMethod = "move";
    //QPointF ptPosRect3New(500.0, 200.0);
    //strMthArgs = qPoint2Str(ptPosRect3New) + " " + strUnit;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpModifyConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
    //    {m_hshGraphObjNameToKeys[strGraphObjName]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter]}});
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //pTestStep->addDataRow({
    //    {"Method", "setCurrentDrawingTool"},
    //    {"FactoryGroupName", ""},
    //    {"FactoryGraphObjType", ""}
    //});
    //// Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    //// while mouse is pressed and release the mouse event.
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosRect3.toPoint());
    //pTestStep->addDataRow({ // Select object by clicking on it
    //    {"Method", "mousePressEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect3.toPoint(), ptPosRect3New.toPoint(), 0, Qt::LeftButton);
    //pTestStep->addDataRow({
    //    {"Method", "mouseReleaseEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptPosRect3 = ptPosRect3New;
    //m_pPhysValRect3->setCenter(QPointF(ptPosRect3New.x(), bYAxisTopDown ? ptPosRect3New.y() : fYAxisMaxVal - ptPosRect3New.y()));
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect3, m_rectRect3, *m_pPhysValRect3, true, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    { -50.0, -50.0},
    //    {  50.0,  50.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3BottomCenterRect4TopCenter,
    //    *m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter,
    //    false, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect3, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect2, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect2, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    {  75.0, -100.0},
    //    { -75.0,  100.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect3LeftCenterRect2RightCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3LeftCenterRect2RightCenter,
    //    *m_pPhysValPolygonConnectionLineRect3LeftCenterRect2RightCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);

    //// move Rectangle4
    ////----------------

    //strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    //strGraphObjType = graphObjType2Str(EGraphObjTypeRect);
    //strGraphObjName = c_strGraphObjNameRect4;
    //QPointF ptPosRect4New(500.0, 400.0);
    //strMthArgs = qPoint2Str(ptPosRect4New) + " " + strUnit;
    //pTestStep = new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
    //    /* pGrpParent      */ pGrpModifyConnections,
    //    /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    //pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    //pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    //pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    //pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    //pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", QStringList{
    //    {m_hshGraphObjNameToKeys[strGraphObjName]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter]},
    //    {m_hshGraphObjNameToKeys[c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter]}});
    //pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    //pTestStep->addDataRow({
    //    {"Method", "setCurrentDrawingTool"},
    //    {"FactoryGroupName", ""},
    //    {"FactoryGraphObjType", ""}
    //});
    //// Move mouse onto rectangle, press mouse to select rectangle, move the rectangle to the new position
    //// while mouse is pressed and release the mouse event.
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(160, 350), m_ptPosRect4.toPoint());
    //pTestStep->addDataRow({ // Select object by clicking on it
    //    {"Method", "mousePressEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptPosRect4.toPoint(), ptPosRect4New.toPoint(), 0, Qt::LeftButton);
    //pTestStep->addDataRow({
    //    {"Method", "mouseReleaseEvent"},
    //    {"MousePos", m_ptMousePos},
    //    {"MouseButtons", Qt::LeftButton},
    //    {"KeyboardModifiers", static_cast<int>(Qt::NoModifier)}
    //});
    //m_ptPosRect4 = ptPosRect4New;
    //m_pPhysValRect4->setCenter(QPointF(ptPosRect4New.x(), bYAxisTopDown ? ptPosRect4New.y() : fYAxisMaxVal - ptPosRect4New.y()));
    //strlstExpectedValues.clear();
    //strlstExpectedValues.append(resultValuesForRect(
    //    strGraphObjName, m_ptPosRect4, m_rectRect4, *m_pPhysValRect4, true, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::BottomCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect3, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::TopCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    {  0.0, -75.0},
    //    {  0.0,  75.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect3BottomCenterRect4TopCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect3BottomCenterRect4TopCenter,
    //    *m_pPhysValPolygonConnectionLineRect3BottomCenterRect4TopCenter,
    //    false, iResultValuesPrecision));
    //eSelPt1 = CEnumSelectionPoint(ESelectionPoint::RightCenter);
    //strGraphObjCnctPt1Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt1.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt1 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect1, strGraphObjCnctPt1Name);
    //ptCnctPt1 = getSelectionPointCoors(*m_pPhysValRect1, eSelPt1.enumerator(), drawingSize.dimensionUnit().enumerator());
    //eSelPt2 = CEnumSelectionPoint(ESelectionPoint::LeftCenter);
    //strGraphObjCnctPt2Name = "CnctPoint-" + CEnumSelectionPoint(eSelPt2.enumerator()).toString() + "-1";
    //strGraphObjKeyInTreeCnctPt2 = pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect4, strGraphObjCnctPt2Name);
    //ptCnctPt2 = getSelectionPointCoors(*m_pPhysValRect4, eSelPt2.enumerator(), drawingSize.dimensionUnit().enumerator());
    //m_polygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    { -75.0, -100.0},
    //    {  75.0,  100.0}
    //});
    //*m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter = QPolygonF({
    //    {ptCnctPt1},
    //    {ptCnctPt2}
    //});
    //strlstExpectedValues.append(resultValuesForConnectionLine(
    //    c_strGraphObjNameConnectionLineRect1RightCenterRect4LeftCenter,
    //    strGraphObjCnctPt1Name, strGraphObjCnctPt2Name,
    //    m_polygonConnectionLineRect1RightCenterRect4LeftCenter,
    //    *m_pPhysValPolygonConnectionLineRect1RightCenterRect4LeftCenter,
    //    false, iResultValuesPrecision));
    //pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesGroupLinesSmallPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesGroupLinesSmallPlusSign",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;
    QStringList strlstGraphObjsKeyInTreeGetResultValues;
    iResultValuesPrecision = -1;

    createTestGroupDrawStandardShapesLinesSmallPlusSign(i_pTestStepGroupParent);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameSmallPlusSign + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallPlusSign + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallPlusSign, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSign));
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("P1", QPoint(258, 308));
    pTestStep->setConfigValue("P2", QPoint(292, 342));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group
    m_ptPosSmallPlusSign = QPointF(275.0, 325.0);
    QPointF ptTLSmallPlusSign(260.0, bYAxisTopDown ? 310.0 : 290.0);
    m_sizeSmallPlusSign = QSizeF(30.0, 30.0);
    *m_pPhysValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, m_sizeSmallPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign, false, iResultValuesPrecision));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -15.0), QPointF(0.0, 15.0));
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(15.0, bYAxisTopDown ? 0.0 : 30.0), QPointF(15.0, bYAxisTopDown ? 30.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine, false, iResultValuesPrecision));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 15.0), QPointF(30.0, 15.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesGroupLinesSmallPlusSignModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesGroupLinesSmallPlusSignModifications",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjTypeGroup = graphObjType2Str(EGraphObjTypeGroup);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();
    QString strUnit = unit.symbol();
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;
    QStringList strlstGraphObjsKeyInTreeGetResultValues;
    CPhysValRect physValRectSmallPlusSignNew = *m_pPhysValRectSmallPlusSign;
    CEnumSelectionPoint eSelPt;
    QPointF pt1SelPt;
    QPointF pt2SelPt;

    ZS::Test::CTestStepGroup* pGrpModifySmallPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify " + c_strGraphObjNameSmallPlusSign,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Resize SmallPlusSign
    //---------------------

    iResultValuesPrecision = -1;
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomRight : ESelectionPoint::TopRight;
    physValRectSmallPlusSignNew = *m_pPhysValRectSmallPlusSign;
    physValRectSmallPlusSignNew.setSize(QSizeF(30.0, 60.0));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameSmallPlusSign + ".setSize(" + physValRectSmallPlusSignNew.size().toString(true) + ")",
        /* strOperation    */ c_strGraphObjNameSmallPlusSign + ".setSize(" + physValRectSmallPlusSignNew.size().toString(true) + ")",
        /* pGrpParent      */ pGrpModifySmallPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectSmallPlusSign, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectSmallPlusSignNew, eSelPt.enumerator());
    m_sizeSmallPlusSign = physValRectSmallPlusSignNew.size().toQSizeF();
    *m_pPhysValRectSmallPlusSign = physValRectSmallPlusSignNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosSmallPlusSign);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(275.0, bYAxisTopDown ? 340.0 : 310);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign, false, iResultValuesPrecision));
    // SmallPlusSign-VerticalLine
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -30.0), QPointF(0.0, 30.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP1(QPointF(15.0, bYAxisTopDown ? 0.0 : 60.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP2(QPointF(15.0, bYAxisTopDown ? 60.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine, false, iResultValuesPrecision));
    // SmallPlusSign-HorizontalLine
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP1(QPointF(0.0, 30.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP2(QPointF(30.0, 30.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize SmallPlusSign
    //---------------------

    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectSmallPlusSignNew = *m_pPhysValRectSmallPlusSign;
    physValRectSmallPlusSignNew.setHeight(120.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameSmallPlusSign + ".setHeight(" + physValRectSmallPlusSignNew.height().toString() + ")",
        /* strOperation    */ c_strGraphObjNameSmallPlusSign + ".setHeight(" + physValRectSmallPlusSignNew.height().toString() + ")",
        /* pGrpParent      */ pGrpModifySmallPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectSmallPlusSign, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectSmallPlusSignNew, eSelPt.enumerator());
    m_sizeSmallPlusSign = physValRectSmallPlusSignNew.size().toQSizeF();
    *m_pPhysValRectSmallPlusSign = physValRectSmallPlusSignNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosSmallPlusSign);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(275.0, bYAxisTopDown ? 370.0 : 280.0);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign, false, iResultValuesPrecision));
    // SmallPlusSign-VerticalLine
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -60.0), QPointF(0.0, 60.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP1(QPointF(15.0, bYAxisTopDown ? 0.0 : 120.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP2(QPointF(15.0, bYAxisTopDown ? 120.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine, false, iResultValuesPrecision));
    // SmallPlusSign-HorizontalLine
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP1(QPointF(0.0, 60.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP2(QPointF(30.0, 60.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesGroupLinesBigPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesGroupLinesBigPlusSign",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;
    QStringList strlstGraphObjsKeyInTreeGetResultValues;

    createTestGroupDrawStandardShapesLinesBigPlusSign(i_pTestStepGroupParent);

    iResultValuesPrecision = -1;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameBigPlusSign, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("P1", QPoint(248, 248));
    pTestStep->setConfigValue("P2", QPoint(352, 352));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group
    m_ptPosBigPlusSign = QPointF(300.0, 300.0);
    QPointF ptTLBigPlusSign(250.0, bYAxisTopDown ? 250.0 : 350.0);
    m_sizeBigPlusSign = QSizeF(100.0, 100.0);
    *m_pPhysValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, m_sizeBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0), QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    *m_pPhysValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesGroupLinesBigPlusSignRotateParent(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesGroupLinesBigPlusSignRotateParent",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjTypeGroup = graphObjType2Str(EGraphObjTypeGroup);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();
    QString strUnit = unit.symbol();
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;
    QStringList strlstGraphObjsKeyInTreeGetResultValues;
    CPhysValRect physValRectTopGroupNew = *m_pPhysValRectTopGroup;
    CEnumSelectionPoint eSelPt;
    QPointF pt1SelPt;
    QPointF pt2SelPt;

    ZS::Test::CTestStepGroup* pGrpModifyBigPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify " + c_strGraphObjNameTopGroup,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Create TopGroup
    //----------------

    iResultValuesPrecision = -1;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + graphObjType2Str(EGraphObjTypeGroup) + ", " + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameTopGroup, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("P1", QPoint(240, 240));
    pTestStep->setConfigValue("P2", QPoint(360, 360));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // TopGroup
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_sizeTopGroup = QSizeF(100.0, 100.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(QPointF(300.0, bYAxisTopDown ? 300.0 : 300.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, false, iResultValuesPrecision));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(50.0, 50.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".showLabel(Name)",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".showLabel(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTopGroup},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(340.0, 220.0)},
        {"ExpectedText", c_strGraphObjNameTopGroup}
    });

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".showLabel(Name)",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".showLabel(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameBigPlusSign},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(360.0, 230.0)},
        {"ExpectedText", c_strGraphObjNameBigPlusSign}
    });

    // Show Geometry Labels
    //---------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".showGeometryLabels(TopLeft, Angle)",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".showGeometryLabels(TopLeft, Angle)",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepShowGeometryLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTopGroup},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"setPos", QPointF(220.0, 220.0)},
        {"ExpectedText", m_pPhysValRectTopGroup->topLeft().toString()},
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTopGroup},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"setPos", QPointF(400.0, bYAxisTopDown ? 310.0 : 290.0)},
        {"ExpectedText", m_physValAngleTopGroup.toString()},
    });

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".showGeometryLabels(TopLeft, Angle)",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".showGeometryLabels(TopLeft, Angle)",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepShowGeometryLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameBigPlusSign},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"setPos", QPointF(260.0, 230.0)},
        {"ExpectedText", m_pPhysValRectBigPlusSign->topLeft().toString()},
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameBigPlusSign},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"setPos", QPointF(350.0, bYAxisTopDown ? 310.0 : 290.0)},
        {"ExpectedText", m_physValAngleBigPlusSign.toString()},
    });

    // Rotate TopGroup
    //----------------

    eSelPt = ESelectionPoint::RotateTop;
    physValRectTopGroupNew = *m_pPhysValRectTopGroup;
    physValRectTopGroupNew.setAngle(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setRotationAngle(" + physValRectTopGroupNew.angle().toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setRotationAngle(" + physValRectTopGroupNew.angle().toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectTopGroup, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectTopGroupNew, eSelPt.enumerator());
    m_physValAngleTopGroup = physValRectTopGroupNew.angle();
    *m_pPhysValRectTopGroup = physValRectTopGroupNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosTopGroup);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // BigPlusSignParentGroup
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, false, iResultValuesPrecision));
    // BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Check Geometry Labels
    //---------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".checkGeometryLabels(TopLeft, Angle)",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".showGeometryLabels(TopLeft, Angle)",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepShowGeometryLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTopGroup},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"ExpectedText", m_pPhysValRectTopGroup->topLeft().toString()},
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTopGroup},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"ExpectedText", m_physValAngleTopGroup.toString()},
    });

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".checkGeometryLabels(TopLeft, Angle)",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".showGeometryLabels(TopLeft, Angle)",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepShowGeometryLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameBigPlusSign},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"ExpectedText", m_pPhysValRectBigPlusSign->topLeft().toString()},
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameBigPlusSign},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"ExpectedText", m_physValAngleBigPlusSign.toString()},
    });

    // Ungroup TopGroup
    //-----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepSelectAndUngroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("P0", m_ptPosTopGroup);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_hshGraphObjNameToKeys.remove(c_strGraphObjNameTopGroup);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSign
    m_ptPosBigPlusSign = QPointF(300.0, 300.0);
    m_physValAngleBigPlusSign += m_physValAngleTopGroup;
    m_pPhysValRectBigPlusSign->setCenter(QPointF(300.0, 300.0));
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // Vertical Line
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // Horizontal Line
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, bYAxisTopDown ? 50.0 : 50.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(100.0, bYAxisTopDown ? 50.0 : 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Check Geometry Labels
    //---------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".checkGeometryLabels(TopLeft, Angle)",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".showGeometryLabels(TopLeft, Angle)",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepShowGeometryLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameBigPlusSign},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"ExpectedText", m_pPhysValRectBigPlusSign->topLeft().toString()},
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameBigPlusSign},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"ExpectedText", m_physValAngleBigPlusSign.toString()},
    });
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesGroupLinesBigPlusSignModfications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesGroupLinesBigPlusSignModfications",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjTypeGroup = graphObjType2Str(EGraphObjTypeGroup);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();
    QString strUnit = unit.symbol();
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstExpectedValues;
    QStringList strlstGraphObjsKeyInTreeGetResultValues;
    CPhysValRect physValRectTopGroupNew = *m_pPhysValRectTopGroup;
    CPhysValRect physValRectBigPlusSignNew = *m_pPhysValRectBigPlusSign;
    CEnumSelectionPoint eSelPt;
    QPointF pt1SelPt;
    QPointF pt2SelPt;

    ZS::Test::CTestStepGroup* pGrpModifyBigPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify " + c_strGraphObjNameBigPlusSign,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Ungroup BigPlusSign
    //----------------------

    iResultValuesPrecision = -1;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingScene.ungroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepSelectAndUngroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    pTestStep->setConfigValue("P0", m_ptPosBigPlusSign);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_hshGraphObjNameToKeys.remove(c_strGraphObjNameBigPlusSign);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Vertical Line
    m_ptPosBigPlusSignVerticalLine = QPointF(300.0, 300.0);
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(300.0, bYAxisTopDown ? 250.0 : 350.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(300.0, bYAxisTopDown ? 350.0 : 250.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false));
    // Horizontal Line
    m_ptPosBigPlusSignHorizontalLine = QPointF(300.0, 300.0);
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(250.0, bYAxisTopDown ? 300.0 : 300.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(350.0, bYAxisTopDown ? 300.0 : 300.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Group BigPlusSign
    //------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("P1", QPoint(240, 240));
    pTestStep->setConfigValue("P2", QPoint(360, 360));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, bYAxisTopDown ? 50.0 : 50.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(100.0, bYAxisTopDown ? 50.0 : 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate BigPlusSign
    //-------------------

    eSelPt = bYAxisTopDown ? ESelectionPoint::RotateTop : ESelectionPoint::RotateBottom;
    physValRectBigPlusSignNew = *m_pPhysValRectBigPlusSign;
    physValRectBigPlusSignNew.setAngle(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + physValRectBigPlusSignNew.angle().toString() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + physValRectBigPlusSignNew.angle().toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectBigPlusSign, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectBigPlusSignNew, eSelPt.enumerator());
    m_physValAngleBigPlusSign = physValRectBigPlusSignNew.angle();
    *m_pPhysValRectBigPlusSign = physValRectBigPlusSignNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosBigPlusSign);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize BigPlusSign
    //-------------------

    iResultValuesPrecision = 5;
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomRight : ESelectionPoint::TopRight;
    physValRectBigPlusSignNew = *m_pPhysValRectBigPlusSign;
    physValRectBigPlusSignNew.setSize(QSizeF(200.0, 100.0));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setSize(" + physValRectBigPlusSignNew.size().toString(true) + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setBottomRight(" + physValRectBigPlusSignNew.size().toString(true) + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectBigPlusSign, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectBigPlusSignNew, eSelPt.enumerator());
    m_sizeBigPlusSign = physValRectBigPlusSignNew.size().toQSizeF();
    *m_pPhysValRectBigPlusSign = physValRectBigPlusSignNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosBigPlusSign);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Because mouse positions are integers in pixel coordinates accuracy will get lost when modifying the objects
    // via mouse events through the drawing view. This inaccuracy will be taken into account by correcting
    // the object coordinates here (setting a breakpoint at "resultValuesFor.." when executing the test and
    // copying the values to the expected results here).
    m_ptPosBigPlusSign = bYAxisTopDown ? QPointF(335.500000, 335.144661) : QPointF(335.144661, 335.500000);
    m_sizeBigPlusSign = QSizeF(199.906638, 99.497475);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? m_ptPosBigPlusSign : QPointF(335.144661, 264.500000));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -49.748737), QPointF(0.0, 49.748737));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(99.953319, bYAxisTopDown ? 0.0 : 99.497475));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(99.953319, bYAxisTopDown ? 99.497475 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-99.953319, 0.0), QPointF(99.953319, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, 49.748737));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(199.906638, 49.748737));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate BigPlusSign
    //-------------------

    iResultValuesPrecision = 5;
    eSelPt = bYAxisTopDown ? ESelectionPoint::RotateTop : ESelectionPoint::RotateBottom;
    physValRectBigPlusSignNew = *m_pPhysValRectBigPlusSign;
    physValRectBigPlusSignNew.setAngle(120.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + physValRectBigPlusSignNew.angle().toString() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + physValRectBigPlusSignNew.angle().toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectBigPlusSign, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectBigPlusSignNew, eSelPt.enumerator());
    m_physValAngleBigPlusSign = physValRectBigPlusSignNew.angle();
    *m_pPhysValRectBigPlusSign = physValRectBigPlusSignNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosBigPlusSign);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group
    m_ptPosBigPlusSign = bYAxisTopDown ? QPointF(335.500000, 335.144661) : QPointF(335.144661, 335.500000);
    m_sizeBigPlusSign = QSizeF(199.906638, 99.497475);
    m_physValAngleBigPlusSign.setVal(bYAxisTopDown ? 119.9 : 119.8);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? m_ptPosBigPlusSign : QPointF(335.144661,264.500000));
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate BigPlusSign
    //-------------------

    iResultValuesPrecision = 5;
    eSelPt = ESelectionPoint::RotateTop;
    physValRectBigPlusSignNew = *m_pPhysValRectBigPlusSign;
    physValRectBigPlusSignNew.setAngle(255.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + physValRectBigPlusSignNew.angle().toString() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + physValRectBigPlusSignNew.angle().toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectBigPlusSign, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectBigPlusSignNew, eSelPt.enumerator());
    m_physValAngleBigPlusSign = physValRectBigPlusSignNew.angle();
    *m_pPhysValRectBigPlusSign = physValRectBigPlusSignNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosBigPlusSign);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group
    m_physValAngleBigPlusSign.setVal(bYAxisTopDown ? 255.2 : 254.6);
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize BigPlusSign
    //-------------------

    iResultValuesPrecision = bYAxisTopDown ? 5 : 6;
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectBigPlusSignNew = *m_pPhysValRectBigPlusSign;
    physValRectBigPlusSignNew.setSize(QSizeF(200.0, 200.0));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setSize(" + physValRectBigPlusSignNew.size().toString(true) + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setSize(" + physValRectBigPlusSignNew.size().toString(true) + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectBigPlusSign, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectBigPlusSignNew, eSelPt.enumerator());
    m_sizeBigPlusSign = physValRectBigPlusSignNew.size().toQSizeF();
    *m_pPhysValRectBigPlusSign = physValRectBigPlusSignNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosBigPlusSign);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSign
    m_ptPosBigPlusSign = bYAxisTopDown ? QPointF(384.160271, 322.272874) : QPointF(286.615970, 348.870844);
    m_sizeBigPlusSign = QSizeF(199.906638, bYAxisTopDown ? 200.165342 : 200.171470);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? m_ptPosBigPlusSign : QPointF(286.615970, 251.129156));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, bYAxisTopDown ? -100.082671 : -100.085735), QPointF(0.0, bYAxisTopDown ? 100.082671 : 100.085735));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(99.953319, bYAxisTopDown ? 0.0 : 200.171470));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(99.953319, bYAxisTopDown ? 200.165342 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-99.953319, 0.0), QPointF(99.953319, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, bYAxisTopDown ? 100.082671 : 100.085735));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(199.906638, bYAxisTopDown ? 100.082671 : 100.085735));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize BigPlusSign
    //-------------------

    iResultValuesPrecision = bYAxisTopDown ? -1 : 6;
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomRight : ESelectionPoint::TopRight;
    physValRectBigPlusSignNew = *m_pPhysValRectBigPlusSign;
    physValRectBigPlusSignNew.setSize(QSizeF(100.0, 100.0));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setSize(" + physValRectBigPlusSignNew.size().toString(true) + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setSize(" + physValRectBigPlusSignNew.size().toString(true) + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectBigPlusSign, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectBigPlusSignNew, eSelPt.enumerator());
    m_sizeBigPlusSign = physValRectBigPlusSignNew.size().toQSizeF();
    *m_pPhysValRectBigPlusSign = physValRectBigPlusSignNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosBigPlusSign);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSigns
    m_ptPosBigPlusSign = bYAxisTopDown ? QPointF(348.483156, 383.248275) : QPointF(348.328241, 383.824004);
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(100.258105, 99.997511) : QSizeF(99.726794, 99.749808);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? m_ptPosBigPlusSign : QPointF(348.328241,216.175996));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, bYAxisTopDown ? -49.998755 : -49.874904), QPointF(0.0, bYAxisTopDown ? 49.998755 : 49.874904));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(bYAxisTopDown ? 50.129052 : 49.863397, bYAxisTopDown ? 0.0 : 99.749808));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(bYAxisTopDown ? 50.129052 : 49.863397, bYAxisTopDown ? 99.997511 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(bYAxisTopDown ? -50.129052 : -49.863397, 0.0), QPointF(bYAxisTopDown ? 50.129052 : 49.863397, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, bYAxisTopDown ? 49.998755 : 49.874904));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(bYAxisTopDown ? 100.258105 : 99.726794, bYAxisTopDown ? 49.998755 : 49.874904));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate BigPlusSign
    //-------------------

    iResultValuesPrecision = 6;
    eSelPt = ESelectionPoint::RotateTop;
    physValRectBigPlusSignNew = *m_pPhysValRectBigPlusSign;
    physValRectBigPlusSignNew.setAngle(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + physValRectBigPlusSignNew.angle().toString() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + physValRectBigPlusSignNew.angle().toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectBigPlusSign, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectBigPlusSignNew, eSelPt.enumerator());
    m_physValAngleBigPlusSign = physValRectBigPlusSignNew.angle();
    *m_pPhysValRectBigPlusSign = physValRectBigPlusSignNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosBigPlusSign);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(100.258105, 99.997511) : QSizeF(99.726794, 99.749808);
    m_physValAngleBigPlusSign.setVal(bYAxisTopDown ? 45.2 : 44.9);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? m_ptPosBigPlusSign : QPointF(348.328241, 216.175996));
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Move BigPlusSign
    //-----------------

    iResultValuesPrecision = 6;
    eSelPt = ESelectionPoint::Center;
    physValRectBigPlusSignNew = *m_pPhysValRectBigPlusSign;
    physValRectBigPlusSignNew.setCenter(QPointF(300.0, 300.0));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setCenter(" + physValRectBigPlusSignNew.center().toString(true) + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setCenter(" + physValRectBigPlusSignNew.center().toString(true) + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectBigPlusSign, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectBigPlusSignNew, eSelPt.enumerator());
    *m_pPhysValRectBigPlusSign = physValRectBigPlusSignNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosBigPlusSign);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    // Group
    m_ptPosBigPlusSign = QPointF(300.0, 300.0);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Move BigPlusSign
    //-----------------

    iResultValuesPrecision = 6;
    eSelPt = ESelectionPoint::Center;
    physValRectBigPlusSignNew = *m_pPhysValRectBigPlusSign;
    physValRectBigPlusSignNew.setCenter(QPointF(400.0, 400.0));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setCenter(" + physValRectBigPlusSignNew.center().toString(true) + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setCenter(" + physValRectBigPlusSignNew.center().toString(true) + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectBigPlusSign, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectBigPlusSignNew, eSelPt.enumerator());
    *m_pPhysValRectBigPlusSign = physValRectBigPlusSignNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosBigPlusSign);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group
    m_ptPosBigPlusSign = QPointF(400.0, bYAxisTopDown ? 400.0 : 200.0);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Move BigPlusSign
    //-----------------

    iResultValuesPrecision = 6;
    eSelPt = ESelectionPoint::Center;
    physValRectBigPlusSignNew = *m_pPhysValRectBigPlusSign;
    physValRectBigPlusSignNew.setCenter(QPointF(300.0, 300.0));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setCenter(" + physValRectBigPlusSignNew.center().toString(true) + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setCenter(" + physValRectBigPlusSignNew.center().toString(true) + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectBigPlusSign, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectBigPlusSignNew, eSelPt.enumerator());
    *m_pPhysValRectBigPlusSign = physValRectBigPlusSignNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosBigPlusSign);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group
    m_ptPosBigPlusSign = QPointF(300.0, 300.0);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Create TopGroup
    //----------------

    iResultValuesPrecision = 6;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + graphObjType2Str(EGraphObjTypeGroup) + ", " + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameTopGroup, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("P1", QPoint(220, 220));
    pTestStep->setConfigValue("P2", QPoint(380, 380));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // TopGroup
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_sizeTopGroup = bYAxisTopDown ? QSizeF(141.601079, 141.602081) : QSizeF(141.051068, 141.051118);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(QPointF(300.0, bYAxisTopDown ? 300.0 : 300.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, false, iResultValuesPrecision));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(100.258105, 99.997511) : QSizeF(99.726794, 99.749808);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(70.800540, 70.801041) : QPointF(70.525534, 70.525559));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize TopGroup
    //----------------

    iResultValuesPrecision = 6;
    eSelPt = ESelectionPoint::RightCenter;
    physValRectTopGroupNew = *m_pPhysValRectTopGroup;
    physValRectTopGroupNew.setWidth(200.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setWidth(" + physValRectTopGroupNew.width().toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setWidth(" + physValRectTopGroupNew.width().toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectTopGroup, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectTopGroupNew, eSelPt.enumerator());
    m_sizeTopGroup = physValRectTopGroupNew.size().toQSizeF();
    *m_pPhysValRectTopGroup = physValRectTopGroupNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosTopGroup);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSignParent
    m_ptPosTopGroup.setX(bYAxisTopDown ? 329.099730 : 329.237233);
    m_sizeTopGroup = bYAxisTopDown ? QSizeF(199.800540, 141.602081) : QSizeF(199.525534, 141.051118);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(m_ptPosTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, false, iResultValuesPrecision));
    // Group BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(141.465189, 99.997511) : QSizeF(141.069771, 99.749808);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(99.900270, 70.801041) : QPointF(99.762767, 70.525559));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine.setP1(bYAxisTopDown ? QPointF(0.0, -49.998755) : QPointF(0.0, -49.874904));
    m_lineBigPlusSignVerticalLine.setP2(bYAxisTopDown ? QPointF(0.0, 49.998755) : QPointF(0.0, 49.874904));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(70.732595, 0.000000) : QPointF(70.534885, 99.749808));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(70.732595, 99.997511) : QPointF(70.534885, 0.000000));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine.setP1(bYAxisTopDown ? QPointF(-70.732595, 0.000000) : QPointF(-70.534885, 0.000000));
    m_lineBigPlusSignHorizontalLine.setP2(bYAxisTopDown ? QPointF(70.732595, 0.000000) : QPointF(70.534885, 0.000000));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.000000, 49.998755) : QPointF(0.000000, 49.874904));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(141.465189, 49.998755) : QPointF(141.069771, 49.874904));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize TopGroup
    //----------------

    iResultValuesPrecision = 6;
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectTopGroupNew = *m_pPhysValRectTopGroup;
    physValRectTopGroupNew.setHeight(300.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setHeight(" + physValRectTopGroupNew.height().toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setHeight(" + physValRectTopGroupNew.height().toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectTopGroup, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectTopGroupNew, eSelPt.enumerator());
    m_sizeTopGroup = physValRectTopGroupNew.size().toQSizeF();
    *m_pPhysValRectTopGroup = physValRectTopGroupNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosTopGroup);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSignParent
    m_ptPosTopGroup.setY(bYAxisTopDown ? 379.099480 : 600.0 - 379.237221);
    m_sizeTopGroup = bYAxisTopDown ? QSizeF(199.800540, 299.801041) : QSizeF(199.525534, 299.525559);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(bYAxisTopDown ? m_ptPosTopGroup : QPointF(329.237233, 379.237221));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, false, iResultValuesPrecision));
    // Group BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(141.465189, 211.715516) : QSizeF(141.069771, 211.821200);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(99.900270, 149.900520) : QPointF(99.762767, 149.762779));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine.setP1(bYAxisTopDown ? QPointF(0.000000, -105.857758) : QPointF(0.000000, -105.910600));
    m_lineBigPlusSignVerticalLine.setP2(bYAxisTopDown ? QPointF(0.000000, 105.857758) : QPointF(0.0, 105.910600));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(70.732595, 0.000000) : QPointF(70.534885, 211.821200));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(70.732595, 211.715516) : QPointF(70.534885, 0.000000));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine.setP1(bYAxisTopDown ? QPointF(-70.732595, 0.000000) : QPointF(-70.534885, 0.000000));
    m_lineBigPlusSignHorizontalLine.setP2(bYAxisTopDown ? QPointF(70.732595, 0.000000) : QPointF(70.534885, 0.000000));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.000000, 105.857758) : QPointF(0.000000, 105.910600));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(141.465189, 105.857758) : QPointF(141.069771, 105.910600));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize TopGroup
    //----------------

    iResultValuesPrecision = 6;
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomRight : ESelectionPoint::TopRight;
    physValRectTopGroupNew = *m_pPhysValRectTopGroup;
    physValRectTopGroupNew.setSize(QSizeF(Math::sqrt(2.0) * 100.0, Math::sqrt(2.0) * 100.0));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setSize(" + physValRectTopGroupNew.size().toString(true) + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setSize(" + physValRectTopGroupNew.size().toString(true) + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectTopGroup, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectTopGroupNew, eSelPt.enumerator());
    m_sizeTopGroup = physValRectTopGroupNew.size().toQSizeF();
    *m_pPhysValRectTopGroup = physValRectTopGroupNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosTopGroup);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSignParent
    m_ptPosTopGroup = bYAxisTopDown ? QPointF(300.099730, 300.099480) : QPointF(300.237233, 600.0 - 300.237221);
    m_sizeTopGroup = bYAxisTopDown ? QSizeF(141.800540, 141.801041) : QSizeF(141.525534, 141.525559);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(bYAxisTopDown ? m_ptPosTopGroup : QPointF(300.237233, 300.237221));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, false, iResultValuesPrecision));
    // Group BigPlusSign
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(100.399329, 100.138013) : QSizeF(100.062254, 100.085328);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(70.900270, 70.900520) : QPointF(70.762767, 70.762779));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine.setP1(bYAxisTopDown ? QPointF(0.000000, -50.069007) : QPointF(0.0, -50.042664));
    m_lineBigPlusSignVerticalLine.setP2(bYAxisTopDown ? QPointF(0.000000, 50.069007) : QPointF(0.0, 50.042664));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(50.199665, 0.000000) : QPointF(50.031127, 100.085328));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(50.199665, 100.138013) : QPointF(50.031127, 0.000000));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine.setP1(bYAxisTopDown ? QPointF(-50.199665, 0.000000) : QPointF(-50.031127, 0.000000));
    m_lineBigPlusSignHorizontalLine.setP2(bYAxisTopDown ? QPointF(50.199665, 0.000000) : QPointF(50.031127, 0.000000));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.000000, 50.069007) : QPointF(0.000000, 50.042664));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(100.399329, 50.069007) : QPointF(100.062254, 50.042664));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate TopGroup
    //----------------

    iResultValuesPrecision = 6;
    eSelPt = ESelectionPoint::RotateTop;
    physValRectTopGroupNew = *m_pPhysValRectTopGroup;
    physValRectTopGroupNew.setAngle(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setRotationAngle(" + physValRectTopGroupNew.angle().toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setRotationAngle(" + physValRectTopGroupNew.angle().toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectTopGroup, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectTopGroupNew, eSelPt.enumerator());
    m_physValAngleTopGroup = physValRectTopGroupNew.angle();
    *m_pPhysValRectTopGroup = physValRectTopGroupNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosTopGroup);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // BigPlusSignParentGroup
    m_physValAngleTopGroup.setVal(bYAxisTopDown ? 44.9 : 45.0);
    m_pPhysValRectTopGroup->setAngle(m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, false, iResultValuesPrecision));
    // BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize TopGroup
    //----------------

    iResultValuesPrecision = 6;
    eSelPt = ESelectionPoint::RightCenter;
    physValRectTopGroupNew = *m_pPhysValRectTopGroup;
    physValRectTopGroupNew.setWidth(200.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setWidth(" +physValRectTopGroupNew.width().toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setWidth(" + physValRectTopGroupNew.width().toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectTopGroup, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectTopGroupNew, eSelPt.enumerator());
    m_sizeTopGroup = physValRectTopGroupNew.size().toQSizeF();
    *m_pPhysValRectTopGroup = physValRectTopGroupNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosTopGroup);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSignParent
    m_ptPosTopGroup = bYAxisTopDown ? QPointF(320.765318, 320.700843) : QPointF(320.968816, 600.0 - 279.505642);
    m_sizeTopGroup = bYAxisTopDown ? QSizeF(200.160892, 141.801041) : QSizeF(200.163299, 141.525559);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(bYAxisTopDown ? m_ptPosTopGroup : QPointF(320.968816, 279.505642));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, false, iResultValuesPrecision));
    // Group BigPlusSign
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(141.720330, 100.138013) : QSizeF(141.520687, 100.085328);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(100.080446, 70.900520) : QPointF(100.081650, 70.762779));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine.setP1(bYAxisTopDown ? QPointF(0.000000, -50.069007) : QPointF(0.0, -50.042664));
    m_lineBigPlusSignVerticalLine.setP2(bYAxisTopDown ? QPointF(0.000000, 50.069007) : QPointF(0.0, 50.042664));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(70.860165, 0.000000) : QPointF(70.760344, 100.085328));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(70.860165, 100.138013) : QPointF(70.760344, 0.000000));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine.setP1(bYAxisTopDown ? QPointF(-70.860165, 0.000000) : QPointF(-70.760344, 0.000000));
    m_lineBigPlusSignHorizontalLine.setP2(bYAxisTopDown ? QPointF(70.860165, 0.000000) : QPointF(70.760344, 0.000000));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.000000, 50.069007) : QPointF(0.000000, 50.042664));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(141.720330, 50.069007) : QPointF(141.520687, 50.042664));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize TopGroup
    //----------------

    iResultValuesPrecision = 6;
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectTopGroupNew = *m_pPhysValRectTopGroup;
    physValRectTopGroupNew.setHeight(200.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setHeight(" + physValRectTopGroupNew.height().toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setHeight(" + physValRectTopGroupNew.height().toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectTopGroup, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectTopGroupNew, eSelPt.enumerator());
    m_sizeTopGroup = physValRectTopGroupNew.size().toQSizeF();
    *m_pPhysValRectTopGroup = physValRectTopGroupNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosTopGroup);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSignParent
    m_ptPosTopGroup = bYAxisTopDown ? QPointF(300.348016, 341.181796) : QPointF(341.581779, 600.0 - 300.118609);
    m_sizeTopGroup = bYAxisTopDown ? QSizeF(200.160892, 199.639977) : QSizeF(200.163299, 199.827828);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(bYAxisTopDown ? m_ptPosTopGroup : QPointF(341.581779, 300.118609));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, false, iResultValuesPrecision));
    // Group BigPlusSign
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(141.720330, 140.983103) : QSizeF(141.520687, 141.316055);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(100.080446, 99.819989) : QPointF(100.081650, 99.913914));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine.setP1(bYAxisTopDown ? QPointF(0.000000, -70.491551) : QPointF(0.0, -70.658027));
    m_lineBigPlusSignVerticalLine.setP2(bYAxisTopDown ? QPointF(0.000000, 70.491551) : QPointF(0.0, 70.658027));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(70.860165, 0.000000) : QPointF(70.760344, 141.316055));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(70.860165, 140.983103) : QPointF(70.760344, 0.000000));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine.setP1(bYAxisTopDown ? QPointF(-70.860165, 0.000000) : QPointF(-70.760344, 0.000000));
    m_lineBigPlusSignHorizontalLine.setP2(bYAxisTopDown ? QPointF(70.860165, 0.000000) : QPointF(70.760344, 0.000000));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.000000, 70.491551) : QPointF(0.000000, 70.658027));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(141.720330, 70.491551) : QPointF(141.520687, 70.658027));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize TopGroup
    //----------------

    iResultValuesPrecision = 6;
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomRight : ESelectionPoint::TopRight;
    physValRectTopGroupNew = *m_pPhysValRectTopGroup;
    physValRectTopGroupNew.setSize(QSizeF(Math::sqrt(2.0) * 100.0, Math::sqrt(2.0) * 100.0));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setSize(" + physValRectTopGroupNew.size().toString(true) + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setSize(" + physValRectTopGroupNew.size().toString(true) + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectTopGroup, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectTopGroupNew, eSelPt.enumerator());
    m_sizeTopGroup = physValRectTopGroupNew.size().toQSizeF();
    *m_pPhysValRectTopGroup = physValRectTopGroupNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosTopGroup);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSignParent
    m_ptPosTopGroup = bYAxisTopDown ? QPointF(299.971928, 299.915650) : QPointF(300.081780, 299.915650);
    m_sizeTopGroup = bYAxisTopDown ? QSizeF(141.359950, 141.721120) : QSizeF(141.473443, 141.137961);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(bYAxisTopDown ? m_ptPosTopGroup : QPointF(300.081780, 300.118601));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, false, iResultValuesPrecision));
    // Group BigPlusSign
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(100.087378, 100.081574) : QSizeF(100.025424, 99.811223);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(70.679975, 70.860560) : QPointF(70.736722, 70.568981));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine.setP1(bYAxisTopDown ? QPointF(0.000000, -50.040787) : QPointF(0.0, -49.905611));
    m_lineBigPlusSignVerticalLine.setP2(bYAxisTopDown ? QPointF(0.000000, 50.040787) : QPointF(0.0, 49.905611));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(50.043689, 0.000000) : QPointF(50.012712, 99.811223));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(50.043689, 100.081574) : QPointF(50.012712, 0.000000));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine.setP1(bYAxisTopDown ? QPointF(-50.043689, 0.000000) : QPointF(-50.012712, 0.000000));
    m_lineBigPlusSignHorizontalLine.setP2(bYAxisTopDown ? QPointF(50.043689, 0.000000) : QPointF(50.012712, 0.000000));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.000000, 50.040787) : QPointF(0.000000, 49.905611));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(100.087378, 50.040787) : QPointF(100.025424, 49.905611));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate TopGroup
    //----------------

    iResultValuesPrecision = 6;
    eSelPt = ESelectionPoint::RotateTop;
    physValRectTopGroupNew = *m_pPhysValRectTopGroup;
    physValRectTopGroupNew.setAngle(0.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setRotationAngle(" + physValRectTopGroupNew.angle().toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setRotationAngle(" + physValRectTopGroupNew.angle().toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectTopGroup, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectTopGroupNew, eSelPt.enumerator());
    m_physValAngleTopGroup = physValRectTopGroupNew.angle();
    *m_pPhysValRectTopGroup = physValRectTopGroupNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosTopGroup);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // BigPlusSignParentGroup
    m_physValAngleTopGroup.setVal(bYAxisTopDown ? 0.0 : 359.9);
    m_pPhysValRectTopGroup->setAngle(m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, false, iResultValuesPrecision));
    // BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Ungroup TopGroup
    //-----------------

    iResultValuesPrecision = 6;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepSelectAndUngroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("P0", m_ptPosTopGroup);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_hshGraphObjNameToKeys.remove(c_strGraphObjNameTopGroup);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSign
    m_ptPosBigPlusSign = bYAxisTopDown ? QPointF(299.971928, 299.915650) : QPointF(300.081780, 600.0 - 300.118601);
    m_physValAngleBigPlusSign.setVal(bYAxisTopDown ? 45.2 : 44.9);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? m_ptPosBigPlusSign : QPointF(300.081780, 300.118601));
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // Vertical Line
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(50.043689, 0.0) : QPointF(50.012712, 99.811223));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(50.043689, 100.081574) : QPointF(50.012712, 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // Horizontal Line
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.0, 50.040787) : QPointF(0.0, 49.905611));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(100.087378, 50.040787) : QPointF(100.025424, 49.905611));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Create TopGroup
    //----------------

    iResultValuesPrecision = 6;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + graphObjType2Str(EGraphObjTypeGroup) + ", " + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameTopGroup, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("P1", QPoint(220, 220));
    pTestStep->setConfigValue("P2", QPoint(380, 380));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // TopGroup
    m_ptPosTopGroup = bYAxisTopDown ? QPointF(299.971928, 299.915650) : QPointF(300.081780, 600.0 - 300.118601);
    m_sizeTopGroup = bYAxisTopDown ? QSizeF(141.540162, 141.540187) : QSizeF(141.305799, 141.305063);
    m_physValAngleTopGroup.setVal(0.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(bYAxisTopDown ? m_ptPosTopGroup : QPointF(300.081780, 300.118601));
    m_pPhysValRectTopGroup->setAngle(m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, false, iResultValuesPrecision));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(100.087378, 100.081574) : QSizeF(100.025424, 99.811223);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(70.770081, 70.770093) : QPointF(70.652900, 70.652532));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate TopGroup
    //----------------

    iResultValuesPrecision = 6;
    eSelPt = ESelectionPoint::RotateTop;
    physValRectTopGroupNew = *m_pPhysValRectTopGroup;
    physValRectTopGroupNew.setAngle(215.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setRotationAngle(" + physValRectTopGroupNew.angle().toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setRotationAngle(" + physValRectTopGroupNew.angle().toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectTopGroup, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectTopGroupNew, eSelPt.enumerator());
    m_physValAngleTopGroup = physValRectTopGroupNew.angle();
    *m_pPhysValRectTopGroup = physValRectTopGroupNew;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosTopGroup);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // BigPlusSignParentGroup
    m_physValAngleTopGroup.setVal(215.1);
    m_pPhysValRectTopGroup->setAngle(m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, false, iResultValuesPrecision));
    // BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Ungroup TopGroup
    //-----------------

    iResultValuesPrecision = 6;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepSelectAndUngroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("P0", m_ptPosTopGroup);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_hshGraphObjNameToKeys.remove(c_strGraphObjNameTopGroup);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSign
    m_physValAngleBigPlusSign.setVal(bYAxisTopDown ? 260.2 : 260.0);
    m_ptPosBigPlusSign = bYAxisTopDown ? QPointF(299.971928, 299.915650) : QPointF(300.081780, 600.0 - 300.118601);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? m_ptPosBigPlusSign : QPointF(300.081780, 300.118601));
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, false, iResultValuesPrecision));
    // Vertical Line
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(50.043689, 0.0) : QPointF(50.012712, 99.811223));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(50.043689, 100.081574) : QPointF(50.012712, 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, false, iResultValuesPrecision));
    // Horizontal Line
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.0, 50.040787) : QPointF(0.0, 49.905611));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(100.087378, 50.040787) : QPointF(100.025424, 49.905611));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}
