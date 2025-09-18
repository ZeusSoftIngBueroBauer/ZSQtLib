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
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameRect,
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
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameEllipse,
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
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameText,
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
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameStar,
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

#if TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONS == 1

    ZS::Test::CTestStepGroup* pGrpConnections = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Connections",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpConnections,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    // ConnectionLines
    //----------------

    ZS::Test::CTestStepGroup* pGrpConnectionLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " ConnectionLines",
        /* pTSGrpParent */ pGrpConnections );
    createTestGroupDrawStandardShapesConnectionLines(pGrpConnectionLines);
    createTestGroupDrawStandardShapesConnectionLineModifications(pGrpConnectionLines);

    createTestStepSaveLoadFile(pGrpConnections, 1);

#endif // TEST_DRAW_OBJECTS_STANDARDSHAPES_CONNECTIONS

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
    createTestGroupDrawStandardShapesGroupLinesSmallPlusSignResize(pGrpGroupsSmallPlusSign);
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
    createTestStepSaveLoadFile(pGrpGroupsBigPlusSignRotateTop, 2);

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
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine, iResultValuesPrecision));
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
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine, iResultValuesPrecision));
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
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
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
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        m_lineCheckmarkLeftLine, *m_pPhysValLineCheckmarkLeftLine, iResultValuesPrecision));
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
        m_lineCheckmarkRightLine, *m_pPhysValLineCheckmarkRightLine, iResultValuesPrecision));
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
        m_lineSmallRectTopLine, *m_pPhysValLineSmallRectTopLine, iResultValuesPrecision));
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
        m_lineSmallRectRightLine, *m_pPhysValLineSmallRectRightLine, iResultValuesPrecision));
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
        m_lineSmallRectBottomLine, *m_pPhysValLineSmallRectBottomLine, iResultValuesPrecision));
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
        m_lineSmallRectLeftLine, *m_pPhysValLineSmallRectLeftLine, iResultValuesPrecision));
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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameRect + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameRect + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObj(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameRect, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameRect));
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameRect);
    pTestStep->setConfigValue("P1", QPoint(275, 275));
    pTestStep->setConfigValue("P2", QPoint(325, 325));
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    QSizeF sizeRect = QSizeF(50.0, 50.0);
    m_ptPosRectangle = QPointF(300, 300.0);
    m_rectRectangle = QRectF(QPointF(-25.0, -25.0), sizeRect);
    m_pPhysValRectRectangle->setSize(sizeRect);
    m_pPhysValRectRectangle->setCenter(QPointF(300, bYAxisTopDown ? 300.0 : fYAxisMaxVal - 300.0));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        c_strGraphObjNameRect, m_ptPosRectangle, m_rectRectangle, *m_pPhysValRectRectangle, iResultValuesPrecision));
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
    QString strObjName = c_strGraphObjNameRect;
    QString strMethod;
    QString strMthArgs;
    CPhysValRect physValRectNew = *m_pPhysValRectRectangle;
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
    strObjName = c_strGraphObjNameRect;
    eSelPt = bYAxisTopDown ? ESelectionPoint::RotateTop : ESelectionPoint::RotateBottom;
    physValRectNew = *m_pPhysValRectRectangle;
    physValRectNew.setAngle(45.0);
    strMethod = "setRotationAngle";
    strMthArgs = physValRectNew.angle().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyRectangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectRectangle, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosRectangle);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_physValAngleRectangle = physValRectNew.angle();
    *m_pPhysValRectRectangle = physValRectNew;
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosRectangle, m_rectRectangle, *m_pPhysValRectRectangle, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setWidth
    //---------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameRect;
    QSizeF sizeRectangle = m_pPhysValRectRectangle->size().toQSizeF();
    sizeRectangle.setWidth(100.0);
    eSelPt = ESelectionPoint::RightCenter;
    physValRectNew = *m_pPhysValRectRectangle;
    physValRectNew.setWidth(sizeRectangle.width());
    strMethod = "setWidth";
    strMthArgs = physValRectNew.width().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyRectangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectRectangle, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosRectangle);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosRectangle = QPointF(317.661165, 317.661165);
    m_rectRectangle = QRectF(QPointF(-49.976659, -25.0), QSizeF(99.953319, 50.0));
    m_pPhysValRectRectangle->setSize(QSizeF(99.953319, 50.0));
    m_pPhysValRectRectangle->setCenter(QPointF(317.661165, bYAxisTopDown ? 317.661165 : 282.338835));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosRectangle, m_rectRectangle, *m_pPhysValRectRectangle, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setHeight
    //----------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameRect;
    sizeRectangle = m_pPhysValRectRectangle->size().toQSizeF();
    sizeRectangle.setHeight(100.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectNew = *m_pPhysValRectRectangle;
    physValRectNew.setHeight(sizeRectangle.height());
    strMethod = "setHeight";
    strMthArgs = physValRectNew.height().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyRectangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectRectangle, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosRectangle);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosRectangle = QPointF(bYAxisTopDown ? 300.0 : 335.322330, bYAxisTopDown ? 335.322330 : 300.0);
    m_rectRectangle = QRectF(QPointF(-49.976659, -49.976659), QSizeF(99.953319, 99.953319));
    m_pPhysValRectRectangle->setSize(QSizeF(99.953319, 99.953319));
    m_pPhysValRectRectangle->setCenter(QPointF(bYAxisTopDown ? 300.0 : 335.322330, bYAxisTopDown ? 335.322330 : 300.0));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosRectangle, m_rectRectangle, *m_pPhysValRectRectangle, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setSize
    //--------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameRect;
    sizeRectangle = QSizeF(150.0, 150.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomRight : ESelectionPoint::TopRight;
    physValRectNew = *m_pPhysValRectRectangle;
    physValRectNew.setSize(sizeRectangle);
    strMethod = "setSize";
    strMthArgs = physValRectNew.size().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyRectangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectRectangle, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosRectangle);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosRectangle = QPointF(bYAxisTopDown ? 300.0 : 370.822330, bYAxisTopDown ? 370.822330 : 300.0);
    m_rectRectangle = QRectF(QPointF(-75.078950, -75.078950), QSizeF(150.157900, 150.157900));
    m_pPhysValRectRectangle->setSize(QSizeF(150.157900, 150.157900));
    m_pPhysValRectRectangle->setCenter(QPointF(bYAxisTopDown ? 300.000000 : 370.822330, bYAxisTopDown ? 370.822330 : 300.000000));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosRectangle, m_rectRectangle, *m_pPhysValRectRectangle, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setWidth
    //---------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameRect;
    sizeRectangle.setWidth(50.0);
    eSelPt = ESelectionPoint::RightCenter;
    physValRectNew = *m_pPhysValRectRectangle;
    physValRectNew.setWidth(sizeRectangle.width());
    strMethod = "setWidth";
    strMthArgs = physValRectNew.width().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyRectangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectRectangle, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosRectangle);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosRectangle = QPointF(bYAxisTopDown ? 264.5 : 335.322330, bYAxisTopDown ? 335.322330 : 264.5);
    m_rectRectangle = QRectF(QPointF(-24.874369, -75.078950), QSizeF(49.748737, 150.157900));
    m_pPhysValRectRectangle->setSize(QSizeF(49.748737, 150.157900));
    m_pPhysValRectRectangle->setCenter(QPointF(bYAxisTopDown ? 264.5 : 335.322330, bYAxisTopDown ? 335.322330 : 335.5));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosRectangle, m_rectRectangle, *m_pPhysValRectRectangle, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setHeight
    //----------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameRect;
    sizeRectangle.setHeight(50.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectNew = *m_pPhysValRectRectangle;
    physValRectNew.setHeight(sizeRectangle.height());
    strMethod = "setHeight";
    strMthArgs = physValRectNew.height().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyRectangle,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectRectangle, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosRectangle);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosRectangle = QPointF(bYAxisTopDown ? 299.750000 : 300.072330, bYAxisTopDown ? 300.072330 : 299.749000);
    m_rectRectangle = QRectF(QPointF(-24.874369, -25.227922), QSizeF(49.748737, 50.455844));
    m_pPhysValRectRectangle->setSize(QSizeF(49.748737, 50.455844));
    m_pPhysValRectRectangle->setCenter(QPointF(bYAxisTopDown ? 299.750000 : 300.072330, bYAxisTopDown ? 300.072330 : 300.250000));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosRectangle, m_rectRectangle, *m_pPhysValRectRectangle, iResultValuesPrecision));
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

    QString strObjName = c_strGraphObjNameEllipse;
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
    m_ptPosEllipse = QPointF(300.0, 300.0);
    m_rectEllipse = QRectF(QPointF(-25.0, -25.0), sizeEllipse);
    m_pPhysValRectEllipse->setSize(sizeEllipse);
    m_pPhysValRectEllipse->setCenter(QPointF(300, bYAxisTopDown ? 300.0 : fYAxisMaxVal - 300.0));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        c_strGraphObjNameEllipse, m_ptPosEllipse,
        m_rectEllipse, *m_pPhysValRectEllipse, iResultValuesPrecision));
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
    QString strObjName = c_strGraphObjNameEllipse;
    QString strMethod;
    QString strMthArgs;
    CPhysValRect physValRectNew = *m_pPhysValRectEllipse;
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
    strObjName = c_strGraphObjNameEllipse;
    eSelPt = bYAxisTopDown ? ESelectionPoint::RotateTop : ESelectionPoint::RotateBottom;
    physValRectNew = *m_pPhysValRectEllipse;
    physValRectNew.setAngle(45.0);
    strMethod = "setRotationAngle";
    strMthArgs = physValRectNew.angle().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyEllipse,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectEllipse, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosEllipse);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_physValAngleEllipse = physValRectNew.angle();
    *m_pPhysValRectEllipse = physValRectNew;
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosEllipse, m_rectEllipse, *m_pPhysValRectEllipse, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setWidth
    //---------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameEllipse;
    QSizeF sizeRectangle = m_pPhysValRectEllipse->size().toQSizeF();
    sizeRectangle.setWidth(100.0);
    eSelPt = ESelectionPoint::RightCenter;
    physValRectNew = *m_pPhysValRectEllipse;
    physValRectNew.setWidth(sizeRectangle.width());
    strMethod = "setWidth";
    strMthArgs = physValRectNew.width().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyEllipse,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectEllipse, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosEllipse);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosEllipse = QPointF(317.661165, 317.661165);
    m_rectEllipse = QRectF(QPointF(-49.976659, -25.0), QSizeF(99.953319, 50.0));
    m_pPhysValRectEllipse->setSize(QSizeF(99.953319, 50.0));
    m_pPhysValRectEllipse->setCenter(QPointF(317.661165, bYAxisTopDown ? 317.661165 : 282.338835));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosEllipse, m_rectEllipse, *m_pPhysValRectEllipse, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setHeight
    //----------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameEllipse;
    sizeRectangle = m_pPhysValRectEllipse->size().toQSizeF();
    sizeRectangle.setHeight(100.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectNew = *m_pPhysValRectEllipse;
    physValRectNew.setHeight(sizeRectangle.height());
    strMethod = "setHeight";
    strMthArgs = physValRectNew.height().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyEllipse,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectEllipse, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosEllipse);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosEllipse = QPointF(bYAxisTopDown ? 300.0 : 335.322330, bYAxisTopDown ? 335.322330 : 300.0);
    m_rectEllipse = QRectF(QPointF(-49.976659, -49.976659), QSizeF(99.953319, 99.953319));
    m_pPhysValRectEllipse->setSize(QSizeF(99.953319, 99.953319));
    m_pPhysValRectEllipse->setCenter(QPointF(bYAxisTopDown ? 300.0 : 335.322330, bYAxisTopDown ? 335.322330 : 300.0));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosEllipse, m_rectEllipse, *m_pPhysValRectEllipse, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setSize
    //--------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameEllipse;
    sizeRectangle = QSizeF(150.0, 150.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomRight : ESelectionPoint::TopRight;
    physValRectNew = *m_pPhysValRectEllipse;
    physValRectNew.setSize(sizeRectangle);
    strMethod = "setSize";
    strMthArgs = physValRectNew.size().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyEllipse,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectEllipse, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosEllipse);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosEllipse = QPointF(bYAxisTopDown ? 300.0 : 370.822330, bYAxisTopDown ? 370.822330 : 300.0);
    m_rectEllipse = QRectF(QPointF(-75.078950, -75.078950), QSizeF(150.157900, 150.157900));
    m_pPhysValRectEllipse->setSize(QSizeF(150.157900, 150.157900));
    m_pPhysValRectEllipse->setCenter(QPointF(bYAxisTopDown ? 300.000000 : 370.822330, bYAxisTopDown ? 370.822330 : 300.000000));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosEllipse, m_rectEllipse, *m_pPhysValRectEllipse, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setWidth
    //---------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameEllipse;
    sizeRectangle.setWidth(50.0);
    eSelPt = ESelectionPoint::RightCenter;
    physValRectNew = *m_pPhysValRectEllipse;
    physValRectNew.setWidth(sizeRectangle.width());
    strMethod = "setWidth";
    strMthArgs = physValRectNew.width().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyEllipse,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectEllipse, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosEllipse);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosEllipse = QPointF(bYAxisTopDown ? 264.5 : 335.322330, bYAxisTopDown ? 335.322330 : 264.5);
    m_rectEllipse = QRectF(QPointF(-24.874369, -75.078950), QSizeF(49.748737, 150.157900));
    m_pPhysValRectEllipse->setSize(QSizeF(49.748737, 150.157900));
    m_pPhysValRectEllipse->setCenter(QPointF(bYAxisTopDown ? 264.5 : 335.322330, bYAxisTopDown ? 335.322330 : 335.5));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosEllipse, m_rectEllipse, *m_pPhysValRectEllipse, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setHeight
    //----------

    iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameEllipse;
    sizeRectangle.setHeight(50.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectNew = *m_pPhysValRectEllipse;
    physValRectNew.setHeight(sizeRectangle.height());
    strMethod = "setHeight";
    strMthArgs = physValRectNew.height().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyEllipse,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectEllipse, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosEllipse);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosEllipse = QPointF(bYAxisTopDown ? 299.750000 : 300.072330, bYAxisTopDown ? 300.072330 : 299.749000);
    m_rectEllipse = QRectF(QPointF(-24.874369, -25.227922), QSizeF(49.748737, 50.455844));
    m_pPhysValRectEllipse->setSize(QSizeF(49.748737, 50.455844));
    m_pPhysValRectEllipse->setCenter(QPointF(bYAxisTopDown ? 299.750000 : 300.072330, bYAxisTopDown ? 300.072330 : 300.250000));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForRect(
        strObjName, m_ptPosEllipse, m_rectEllipse, *m_pPhysValRectEllipse, iResultValuesPrecision));
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

    QString strObjName = c_strGraphObjNameText;
    QString strMethod = "DrawingScene.drawGraphObj";
    QString strText = "Hello World";
    QString strMthArgs = strObjName + ", " + strText;

    // Text
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
    pTestStep->setConfigValue("P1", QPoint(264.0, 384.0));
    pTestStep->setConfigValue("P2", QPoint(264.0 + 100.0, 384.0 + 50.0));
    pTestStep->setConfigValue("Text", strText);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    QSizeF sizeText(72.0, 31.0); // adjusted to text size with margins
    m_pPhysValRectText->setSize(sizeText);
    m_pPhysValRectText->setCenter(QPointF(300.0, bYAxisTopDown ? 399.5 : 181.5));
    m_ptPosText.setX(m_pPhysValRectText->center().toQPointF().x());
    m_ptPosText.setY(bYAxisTopDown ? m_pPhysValRectText->center().toQPointF().y() : fYAxisMaxVal - m_pPhysValRectText->center().toQPointF().y());
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForText(
        strObjName, m_ptPosText, *m_pPhysValRectText, strText, iResultValuesPrecision));
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
    QString strObjName = c_strGraphObjNameText;
    QString strText = "Hello World";
    CPhysValRect physValRectNew = *m_pPhysValRectText;
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
    strObjName = c_strGraphObjNameText;
    eSelPt = bYAxisTopDown ? ESelectionPoint::RotateTop : ESelectionPoint::RotateBottom;
    physValRectNew = *m_pPhysValRectText;
    physValRectNew.setAngle(45.0);
    strMethod = "setRotationAngle";
    strMthArgs = physValRectNew.angle().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyText,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectText, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosText);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    physValRectNew.setAngle(44.4);
    m_physValAngleText = physValRectNew.angle();
    *m_pPhysValRectText = physValRectNew;
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForText(
        strObjName, m_ptPosText, *m_pPhysValRectText, strText, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setWidth
    //---------

    iResultValuesPrecision = 6; //bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameText;
    QSizeF sizeRectangle = m_pPhysValRectText->size().toQSizeF();
    sizeRectangle.setWidth(100.0);
    eSelPt = ESelectionPoint::RightCenter;
    physValRectNew = *m_pPhysValRectText;
    physValRectNew.setWidth(sizeRectangle.width());
    strMethod = "setWidth";
    strMthArgs = physValRectNew.width().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyText,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectText, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosText);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosText = QPointF(309.997208, bYAxisTopDown ? 409.301185 : fYAxisMaxVal - 171.698815);
    m_pPhysValRectText->setSize(QSizeF(100.000528, 31.0));
    m_pPhysValRectText->setCenter(QPointF(309.997208, bYAxisTopDown ? 409.301185 : 171.698815));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForText(
        strObjName, m_ptPosText, *m_pPhysValRectText, strText, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setHeight
    //----------

    iResultValuesPrecision = 6; //bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameText;
    sizeRectangle = m_pPhysValRectText->size().toQSizeF();
    sizeRectangle.setHeight(100.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectNew = *m_pPhysValRectText;
    physValRectNew.setHeight(sizeRectangle.height());
    strMethod = "setHeight";
    strMthArgs = physValRectNew.height().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyText,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectText, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosText);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosText = QPointF(bYAxisTopDown ? 285.793768 : 334.102629, bYAxisTopDown ? 433.988694 : fYAxisMaxVal - 196.286345);
    m_pPhysValRectText->setSize(QSizeF(100.000528, bYAxisTopDown ? 100.145634 : 99.865606));
    m_pPhysValRectText->setCenter(QPointF(bYAxisTopDown ? 285.793768 : 334.102629, bYAxisTopDown ? 433.988694 : 196.286345));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForText(
        strObjName, m_ptPosText, *m_pPhysValRectText, strText, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setSize
    //--------

    iResultValuesPrecision = 6; //bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameText;
    sizeRectangle = QSizeF(150.0, 150.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomRight : ESelectionPoint::TopRight;
    physValRectNew = *m_pPhysValRectText;
    physValRectNew.setSize(sizeRectangle);
    strMethod = "setSize";
    strMthArgs = physValRectNew.size().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyText,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectText, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosText);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosText = QPointF(bYAxisTopDown ? 286.072242 : 369.721135, bYAxisTopDown ? 469.114650 : fYAxisMaxVal - 196.817221);
    m_pPhysValRectText->setSize(QSizeF(bYAxisTopDown ? 149.579586 : 150.125641, bYAxisTopDown ? 149.920712 : 150.494770));
    m_pPhysValRectText->setCenter(QPointF(bYAxisTopDown ? 286.072242 : 369.721135, bYAxisTopDown ? 469.114650 : 196.817221));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForText(
        strObjName, m_ptPosText, *m_pPhysValRectText, strText, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setWidth
    //---------

    iResultValuesPrecision = 6; //bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameText;
    sizeRectangle.setWidth(50.0);
    eSelPt = ESelectionPoint::RightCenter;
    physValRectNew = *m_pPhysValRectText;
    physValRectNew.setWidth(sizeRectangle.width());
    strMethod = "setWidth";
    strMthArgs = physValRectNew.width().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyText,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectText, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosText);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosText = QPointF(bYAxisTopDown ? 250.484220 : 334.108118, bYAxisTopDown ? 434.224432 : fYAxisMaxVal - 231.731943);
    m_pPhysValRectText->setSize(QSizeF(bYAxisTopDown ? 49.903418 : 50.379467, bYAxisTopDown ? 149.920712 : 150.494770));
    m_pPhysValRectText->setCenter(QPointF(bYAxisTopDown ? 250.484220 : 334.108118, bYAxisTopDown ? 434.224432 : 231.731943));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForText(
        strObjName, m_ptPosText, *m_pPhysValRectText, strText, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // setHeight
    //----------

    iResultValuesPrecision = 6; //bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    strObjName = c_strGraphObjNameText;
    sizeRectangle.setHeight(50.0);
    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    physValRectNew = *m_pPhysValRectText;
    physValRectNew.setHeight(sizeRectangle.height());
    strMethod = "setHeight";
    strMthArgs = physValRectNew.height().toString();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyText,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValRectText, eSelPt.enumerator());
    pt2SelPt = getSelectionPointCoors(physValRectNew, eSelPt.enumerator());
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P0", m_ptPosText);
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    m_ptPosText = QPointF(bYAxisTopDown ? 285.570478 : 298.899336, bYAxisTopDown ? 398.436450 : fYAxisMaxVal - 195.818985);
    m_pPhysValRectText->setSize(QSizeF(bYAxisTopDown ? 49.903418 : 50.379467, bYAxisTopDown ? 49.684487 : 49.908510));
    m_pPhysValRectText->setCenter(QPointF(bYAxisTopDown ? 285.570478 : 298.899336, bYAxisTopDown ? 398.436450 : 195.818985));
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForText(
        strObjName, m_ptPosText, *m_pPhysValRectText, strText, iResultValuesPrecision));
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
        c_strGraphObjNameOpenArrow, m_ptPosOpenArrow, m_polygonOpenArrow, *m_pPhysValPolygonOpenArrow, iResultValuesPrecision));
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
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, iResultValuesPrecision));
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
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, m_ptMousePos, QPoint(260, 300));
    pTestStep->addDataRow({ // Deselect object by clicking on empty area
        {"Method", "mousePressEvent"},
        {"MousePos", m_ptMousePos}
    });
    pTestStep->addDataRow({
        {"Method", "mouseReleaseEvent"},
        {"MousePos", m_ptMousePos}
    });
    // Move mouse into object
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(260, 300), m_ptPosTriangle.toPoint());
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
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, iResultValuesPrecision));
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
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, iResultValuesPrecision));
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
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, iResultValuesPrecision));
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
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, iResultValuesPrecision));
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
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, iResultValuesPrecision));
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
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, iResultValuesPrecision));
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
        c_strGraphObjNameTriangle, m_ptPosTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, iResultValuesPrecision));
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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameStar + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameStar + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjPolygon(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameStar, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar));
    m_ptPosStar = QPointF(500.0, 400.0);
    m_polygonStar = QPolygonF({
        {   0.0, -100.0},
        {  25.0,  -25.0},
        { 100.0,    0.0},
        {  25.0,   25.0},
        {   0.0,  100.0},
        { -25.0,   25.0},
        {-100.0,    0.0},
        { -25.0,  -25.0}
    });
    *m_pPhysValPolygonStar = QPolygonF({
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
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameStar);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameStar, m_ptPosStar, m_polygonStar, *m_pPhysValPolygonStar, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameStar + ".showLabel(" + CGraphObj::c_strLabelName + ")",
        /* strOperation    */ c_strGraphObjNameStar + ".showLabel(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameStar]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(540.0, 320.0)},
        {"ExpectedText", c_strGraphObjNameStar}
    });
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesConnectionLines(
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
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionLines",
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
        /* pGrpParent      */ i_pTestStepGroupParent,
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
        strGraphObjName, m_ptPosConnectionPoint1, *m_pPhysValConnectionPoint1, iResultValuesPrecision));
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
        /* pGrpParent      */ i_pTestStepGroupParent,
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
        strGraphObjName, m_ptPosConnectionPoint2, *m_pPhysValConnectionPoint2, iResultValuesPrecision));
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
        /* pGrpParent      */ i_pTestStepGroupParent,
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
        strGraphObjName, m_ptPosConnectionPoint3, *m_pPhysValConnectionPoint3, iResultValuesPrecision));
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
        /* pGrpParent      */ i_pTestStepGroupParent,
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
        strGraphObjName, m_ptPosConnectionPoint4, *m_pPhysValConnectionPoint4, iResultValuesPrecision));
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
        /* pGrpParent      */ i_pTestStepGroupParent,
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
    // We need a start position, from which we move to the first connection point,
    // and the position of the two connection points. If more than 3 points are defined,
    // the points from index 2 to the penultimate index are additional polygon points.
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
        m_polygonConnectionLineCnctPt1CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

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
        /* pGrpParent      */ i_pTestStepGroupParent,
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
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(strGraphObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName));
    m_polygonConnectionLineCnctPt3CnctPt4 = QPolygonF({
        { 0.0, -50.0},
        { 0.0,  50.0}
    });
    *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4 = QPolygonF({
        {m_pPhysValConnectionPoint3->toQPointF()},
        {m_pPhysValConnectionPoint4->toQPointF()}
    });
    // We need a start position, from which we move to the first connection point,
    // and the position of the two connection points. If more than 3 points are defined,
    // the points from index 2 to the penultimate index are additional polygon points.
    points = QPolygon({
        QPoint(640, 250),
        m_ptPosConnectionPoint3.toPoint(),
        QPoint((m_ptPosConnectionPoint3.x() + m_ptPosConnectionPoint4.x()) / 2,
               (m_ptPosConnectionPoint3.y() + m_ptPosConnectionPoint4.y()) / 2),
        m_ptPosConnectionPoint4.toPoint()
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt3CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

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
        /* pGrpParent      */ i_pTestStepGroupParent,
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
        /* pGrpParent      */ i_pTestStepGroupParent,
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
    // We need a start position, from which we move to the first connection point,
    // and the position of the two connection points. If more than 3 points are defined,
    // the points from index 2 to the penultimate index are additional polygon points.
    points = QPolygon({
        QPoint(240, 250),
        m_ptPosConnectionPoint1.toPoint(),
        QPoint((m_ptPosConnectionPoint1.x() + m_ptPosConnectionPoint4.x()) / 2,
               (m_ptPosConnectionPoint1.y() + m_ptPosConnectionPoint4.y()) / 2),
        m_ptPosConnectionPoint4.toPoint()
    });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strGraphObjName, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt1CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt4, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

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
        /* pGrpParent      */ i_pTestStepGroupParent,
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
        /* pGrpParent      */ i_pTestStepGroupParent,
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
    // We need a start position, from which we move to the first connection point,
    // and the position of the two connection points. If more than 3 points are defined,
    // the points from index 2 to the penultimate index are additional polygon points.
    points = QPolygon({
        QPoint(640, 250),
        m_ptPosConnectionPoint3.toPoint(),
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
        m_polygonConnectionLineCnctPt3CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt2, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

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
        /* pGrpParent      */ i_pTestStepGroupParent,
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
        /* pGrpParent      */ i_pTestStepGroupParent,
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
void CTest::createTestGroupDrawStandardShapesConnectionLineModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesConnectionLineModifications",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameConnections;
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
    QString strGraphObjType;
    QString strGraphObjName;
    QString strMethod;
    QString strMthArgs;
    CPhysValPoint physValPoint(*m_pDrawingScene);
    CPhysValPolygon physValConnectionLineNew(*m_pDrawingScene);
    SGraphObjSelectionPoint selPt;
    QPointF pt1SelPt;
    QPointF pt2SelPt;

    ZS::Test::CTestStepGroup* pGrpModifyCnctPts = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify " + c_strGraphObjNameConnectionLineCnctPt1CnctPt2,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Hide Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + ".hideLabels(" + CGraphObj::c_strLabelName + ")",
        /* strOperation    */ "CnctLines.hideLabels(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ pGrpModifyCnctPts,
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
        /* pGrpParent      */ pGrpModifyCnctPts,
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
        strGraphObjName, m_ptPosConnectionPoint1, *m_pPhysValConnectionPoint1, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt2, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt1CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt4, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt1CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt4, iResultValuesPrecision));
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
        /* pGrpParent      */ pGrpModifyCnctPts,
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
        strGraphObjName, m_ptPosConnectionPoint2, *m_pPhysValConnectionPoint2, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt2, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt1CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt3CnctPt2, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt3CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt2, iResultValuesPrecision));
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
        /* pGrpParent      */ pGrpModifyCnctPts,
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
        { -50.0, -75.0},
        {  50.0,  75.0}
    });
    m_polygonConnectionLineCnctPt3CnctPt2 = QPolygonF({
        { 100.0, -100.0},
        {-100.0,  100.0}
    });
    *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4 = QPolygonF({
        {m_pPhysValConnectionPoint3->toQPointF()},
        {m_pPhysValConnectionPoint4->toQPointF()}
    });
    *m_pPhysValPolygonConnectionLineCnctPt3CnctPt2 = QPolygonF({
        {m_pPhysValConnectionPoint3->toQPointF()},
        {m_pPhysValConnectionPoint2->toQPointF()}
    });
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionPoint(
        strGraphObjName, m_ptPosConnectionPoint3, *m_pPhysValConnectionPoint3, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt3CnctPt4, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt3CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt3CnctPt2, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt3CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt2, iResultValuesPrecision));
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
        /* pGrpParent      */ pGrpModifyCnctPts,
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
        {0.0, -100.0},
        {0.0,  100.0}
    });
    m_polygonConnectionLineCnctPt1CnctPt4 = QPolygonF({
        {-100.0, -100.0},
        { 100.0,  100.0}
    });
    *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4 = QPolygonF({
        {m_pPhysValConnectionPoint3->toQPointF()},
        {m_pPhysValConnectionPoint4->toQPointF()}
    });
    *m_pPhysValPolygonConnectionLineCnctPt1CnctPt4 = QPolygonF({
        {m_pPhysValConnectionPoint1->toQPointF()},
        {m_pPhysValConnectionPoint4->toQPointF()}
    });
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionPoint(
        strGraphObjName, m_ptPosConnectionPoint4, *m_pPhysValConnectionPoint4, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt3CnctPt4, c_strGraphObjNameConnectionPoint3, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt3CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt3CnctPt4, iResultValuesPrecision));
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt4, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint4,
        m_polygonConnectionLineCnctPt1CnctPt4, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt4, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // CnctLinePt1Pt2.insert(1, )
    //---------------------------

    selPt.m_selPtType = ESelectionPointType::LineCenterPoint;
    selPt.m_selPt = ESelectionPoint::LineCenterPoint;
    selPt.m_idxPt = 0;
    physValConnectionLineNew = *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2;
    physValPoint = m_pPhysValPolygonConnectionLineCnctPt1CnctPt2->at(selPt.m_idxPt);
    physValPoint.setX(280.0);
    physValPoint.setY(bYAxisTopDown ? 250.0 : fYAxisMaxVal - 250.0);
    physValConnectionLineNew.insert(selPt.m_idxPt+1, physValPoint);
    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strGraphObjName = c_strGraphObjNameConnectionLineCnctPt1CnctPt2;
    strMethod = "insert";
    strMthArgs = QString::number(selPt.m_idxPt+1) + ", {" + physValPoint.toString() + "}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strGraphObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpModifyCnctPts,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep*)) );
    pt1SelPt = getSelectionPointCoors(*m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, selPt);
    pt2SelPt = m_pDrawingScene->convert(physValPoint, Units.Length.px).toQPointF();
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strGraphObjName]);
    pTestStep->setConfigValue("SelectionPoint", selPt.toString());
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    pTestStep->addDataRow({
        {"Method", "setCurrentDrawingTool"},
        {"FactoryGroupName", ""},
        {"FactoryGraphObjType", ""}
    });
    // Move mouse to line segment between first and second polygon point and
    // select connection line to edit polygon shape points
    m_ptMousePos = addMouseMoveEventDataRows(pTestStep, QPoint(280, 250), pt1SelPt.toPoint());
    pTestStep->addDataRow({ // Select object by clicking on it
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
    m_polygonConnectionLineCnctPt1CnctPt2 = QPolygonF({
        { 10.0, -100.0},
        {-10.0,  -50.0},
        { 10.0,  100.0}
    });
    m_pPhysValPolygonConnectionLineCnctPt1CnctPt2->insert(selPt.m_idxPt+1, physValPoint);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        c_strGraphObjNameConnectionLineCnctPt1CnctPt2, c_strGraphObjNameConnectionPoint1, c_strGraphObjNameConnectionPoint2,
        m_polygonConnectionLineCnctPt1CnctPt2, *m_pPhysValPolygonConnectionLineCnctPt1CnctPt2, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // CnctLinePt1Pt2.insert(2, )
    //---------------------------

    // CnctLinePt1Pt2.insert(2, )
    //---------------------------


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
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign, iResultValuesPrecision));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -15.0), QPointF(0.0, 15.0));
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(15.0, bYAxisTopDown ? 0.0 : 30.0), QPointF(15.0, bYAxisTopDown ? 30.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine, iResultValuesPrecision));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 15.0), QPointF(30.0, 15.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesGroupLinesSmallPlusSignResize(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesGroupLinesSmallPlusSignResize",
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
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign, iResultValuesPrecision));
    // SmallPlusSign-VerticalLine
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -30.0), QPointF(0.0, 30.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP1(QPointF(15.0, bYAxisTopDown ? 0.0 : 60.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP2(QPointF(15.0, bYAxisTopDown ? 60.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine, iResultValuesPrecision));
    // SmallPlusSign-HorizontalLine
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP1(QPointF(0.0, 30.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP2(QPointF(30.0, 30.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign, iResultValuesPrecision));
    // SmallPlusSign-VerticalLine
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -60.0), QPointF(0.0, 60.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP1(QPointF(15.0, bYAxisTopDown ? 0.0 : 120.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP2(QPointF(15.0, bYAxisTopDown ? 120.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine, iResultValuesPrecision));
    // SmallPlusSign-HorizontalLine
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP1(QPointF(0.0, 60.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP2(QPointF(30.0, 60.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0), QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    *m_pPhysValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, iResultValuesPrecision));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(50.0, 50.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, iResultValuesPrecision));
    // BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // Vertical Line
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // Horizontal Line
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, bYAxisTopDown ? 50.0 : 50.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(100.0, bYAxisTopDown ? 50.0 : 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // Horizontal Line
    m_ptPosBigPlusSignHorizontalLine = QPointF(300.0, 300.0);
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(250.0, bYAxisTopDown ? 300.0 : 300.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(350.0, bYAxisTopDown ? 300.0 : 300.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, bYAxisTopDown ? 50.0 : 50.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(100.0, bYAxisTopDown ? 50.0 : 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -49.748737), QPointF(0.0, 49.748737));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(99.953319, bYAxisTopDown ? 0.0 : 99.497475));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(99.953319, bYAxisTopDown ? 99.497475 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-99.953319, 0.0), QPointF(99.953319, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, 49.748737));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(199.906638, 49.748737));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, bYAxisTopDown ? -100.082671 : -100.085735), QPointF(0.0, bYAxisTopDown ? 100.082671 : 100.085735));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(99.953319, bYAxisTopDown ? 0.0 : 200.171470));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(99.953319, bYAxisTopDown ? 200.165342 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-99.953319, 0.0), QPointF(99.953319, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, bYAxisTopDown ? 100.082671 : 100.085735));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(199.906638, bYAxisTopDown ? 100.082671 : 100.085735));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, bYAxisTopDown ? -49.998755 : -49.874904), QPointF(0.0, bYAxisTopDown ? 49.998755 : 49.874904));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(bYAxisTopDown ? 50.129052 : 49.863397, bYAxisTopDown ? 0.0 : 99.749808));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(bYAxisTopDown ? 50.129052 : 49.863397, bYAxisTopDown ? 99.997511 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(bYAxisTopDown ? -50.129052 : -49.863397, 0.0), QPointF(bYAxisTopDown ? 50.129052 : 49.863397, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, bYAxisTopDown ? 49.998755 : 49.874904));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(bYAxisTopDown ? 100.258105 : 99.726794, bYAxisTopDown ? 49.998755 : 49.874904));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, iResultValuesPrecision));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(100.258105, 99.997511) : QSizeF(99.726794, 99.749808);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(70.800540, 70.801041) : QPointF(70.525534, 70.525559));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, iResultValuesPrecision));
    // Group BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(141.465189, 99.997511) : QSizeF(141.069771, 99.749808);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(99.900270, 70.801041) : QPointF(99.762767, 70.525559));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine.setP1(bYAxisTopDown ? QPointF(0.0, -49.998755) : QPointF(0.0, -49.874904));
    m_lineBigPlusSignVerticalLine.setP2(bYAxisTopDown ? QPointF(0.0, 49.998755) : QPointF(0.0, 49.874904));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(70.732595, 0.000000) : QPointF(70.534885, 99.749808));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(70.732595, 99.997511) : QPointF(70.534885, 0.000000));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine.setP1(bYAxisTopDown ? QPointF(-70.732595, 0.000000) : QPointF(-70.534885, 0.000000));
    m_lineBigPlusSignHorizontalLine.setP2(bYAxisTopDown ? QPointF(70.732595, 0.000000) : QPointF(70.534885, 0.000000));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.000000, 49.998755) : QPointF(0.000000, 49.874904));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(141.465189, 49.998755) : QPointF(141.069771, 49.874904));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, iResultValuesPrecision));
    // Group BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(141.465189, 211.715516) : QSizeF(141.069771, 211.821200);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(99.900270, 149.900520) : QPointF(99.762767, 149.762779));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine.setP1(bYAxisTopDown ? QPointF(0.000000, -105.857758) : QPointF(0.000000, -105.910600));
    m_lineBigPlusSignVerticalLine.setP2(bYAxisTopDown ? QPointF(0.000000, 105.857758) : QPointF(0.0, 105.910600));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(70.732595, 0.000000) : QPointF(70.534885, 211.821200));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(70.732595, 211.715516) : QPointF(70.534885, 0.000000));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine.setP1(bYAxisTopDown ? QPointF(-70.732595, 0.000000) : QPointF(-70.534885, 0.000000));
    m_lineBigPlusSignHorizontalLine.setP2(bYAxisTopDown ? QPointF(70.732595, 0.000000) : QPointF(70.534885, 0.000000));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.000000, 105.857758) : QPointF(0.000000, 105.910600));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(141.465189, 105.857758) : QPointF(141.069771, 105.910600));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, iResultValuesPrecision));
    // Group BigPlusSign
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(100.399329, 100.138013) : QSizeF(100.062254, 100.085328);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(70.900270, 70.900520) : QPointF(70.762767, 70.762779));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine.setP1(bYAxisTopDown ? QPointF(0.000000, -50.069007) : QPointF(0.0, -50.042664));
    m_lineBigPlusSignVerticalLine.setP2(bYAxisTopDown ? QPointF(0.000000, 50.069007) : QPointF(0.0, 50.042664));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(50.199665, 0.000000) : QPointF(50.031127, 100.085328));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(50.199665, 100.138013) : QPointF(50.031127, 0.000000));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine.setP1(bYAxisTopDown ? QPointF(-50.199665, 0.000000) : QPointF(-50.031127, 0.000000));
    m_lineBigPlusSignHorizontalLine.setP2(bYAxisTopDown ? QPointF(50.199665, 0.000000) : QPointF(50.031127, 0.000000));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.000000, 50.069007) : QPointF(0.000000, 50.042664));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(100.399329, 50.069007) : QPointF(100.062254, 50.042664));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, iResultValuesPrecision));
    // BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, iResultValuesPrecision));
    // Group BigPlusSign
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(141.720330, 100.138013) : QSizeF(141.520687, 100.085328);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(100.080446, 70.900520) : QPointF(100.081650, 70.762779));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine.setP1(bYAxisTopDown ? QPointF(0.000000, -50.069007) : QPointF(0.0, -50.042664));
    m_lineBigPlusSignVerticalLine.setP2(bYAxisTopDown ? QPointF(0.000000, 50.069007) : QPointF(0.0, 50.042664));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(70.860165, 0.000000) : QPointF(70.760344, 100.085328));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(70.860165, 100.138013) : QPointF(70.760344, 0.000000));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine.setP1(bYAxisTopDown ? QPointF(-70.860165, 0.000000) : QPointF(-70.760344, 0.000000));
    m_lineBigPlusSignHorizontalLine.setP2(bYAxisTopDown ? QPointF(70.860165, 0.000000) : QPointF(70.760344, 0.000000));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.000000, 50.069007) : QPointF(0.000000, 50.042664));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(141.720330, 50.069007) : QPointF(141.520687, 50.042664));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, iResultValuesPrecision));
    // Group BigPlusSign
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(141.720330, 140.983103) : QSizeF(141.520687, 141.316055);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(100.080446, 99.819989) : QPointF(100.081650, 99.913914));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine.setP1(bYAxisTopDown ? QPointF(0.000000, -70.491551) : QPointF(0.0, -70.658027));
    m_lineBigPlusSignVerticalLine.setP2(bYAxisTopDown ? QPointF(0.000000, 70.491551) : QPointF(0.0, 70.658027));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(70.860165, 0.000000) : QPointF(70.760344, 141.316055));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(70.860165, 140.983103) : QPointF(70.760344, 0.000000));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine.setP1(bYAxisTopDown ? QPointF(-70.860165, 0.000000) : QPointF(-70.760344, 0.000000));
    m_lineBigPlusSignHorizontalLine.setP2(bYAxisTopDown ? QPointF(70.860165, 0.000000) : QPointF(70.760344, 0.000000));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.000000, 70.491551) : QPointF(0.000000, 70.658027));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(141.720330, 70.491551) : QPointF(141.520687, 70.658027));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, iResultValuesPrecision));
    // Group BigPlusSign
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(100.087378, 100.081574) : QSizeF(100.025424, 99.811223);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(70.679975, 70.860560) : QPointF(70.736722, 70.568981));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine.setP1(bYAxisTopDown ? QPointF(0.000000, -50.040787) : QPointF(0.0, -49.905611));
    m_lineBigPlusSignVerticalLine.setP2(bYAxisTopDown ? QPointF(0.000000, 50.040787) : QPointF(0.0, 49.905611));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(50.043689, 0.000000) : QPointF(50.012712, 99.811223));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(50.043689, 100.081574) : QPointF(50.012712, 0.000000));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine.setP1(bYAxisTopDown ? QPointF(-50.043689, 0.000000) : QPointF(-50.012712, 0.000000));
    m_lineBigPlusSignHorizontalLine.setP2(bYAxisTopDown ? QPointF(50.043689, 0.000000) : QPointF(50.012712, 0.000000));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.000000, 50.040787) : QPointF(0.000000, 49.905611));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(100.087378, 50.040787) : QPointF(100.025424, 49.905611));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, iResultValuesPrecision));
    // BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // Vertical Line
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(50.043689, 0.0) : QPointF(50.012712, 99.811223));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(50.043689, 100.081574) : QPointF(50.012712, 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // Horizontal Line
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.0, 50.040787) : QPointF(0.0, 49.905611));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(100.087378, 50.040787) : QPointF(100.025424, 49.905611));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, iResultValuesPrecision));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = bYAxisTopDown ? QSizeF(100.087378, 100.081574) : QSizeF(100.025424, 99.811223);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? QPointF(70.770081, 70.770093) : QPointF(70.652900, 70.652532));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, iResultValuesPrecision));
    // BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign, iResultValuesPrecision));
    // Vertical Line
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignVerticalLine->setP1(bYAxisTopDown ? QPointF(50.043689, 0.0) : QPointF(50.012712, 99.811223));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(bYAxisTopDown ? QPointF(50.043689, 100.081574) : QPointF(50.012712, 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine, iResultValuesPrecision));
    // Horizontal Line
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(bYAxisTopDown ? QPointF(0.0, 50.040787) : QPointF(0.0, 49.905611));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(bYAxisTopDown ? QPointF(100.087378, 50.040787) : QPointF(100.025424, 49.905611));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}
