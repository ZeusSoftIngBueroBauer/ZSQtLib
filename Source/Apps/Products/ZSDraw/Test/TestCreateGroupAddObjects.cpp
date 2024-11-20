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
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPolygon.h"
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
void CTest::createTestGroupAddObjects(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddObjects",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpAddStandardShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Add " + CObjFactory::c_strGroupNameStandardShapes,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    createTestGroupAddStandardShapes(pGrpAddStandardShapes);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapes(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapes",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStep* pTestStep = nullptr;

    // Lines
    //======
#if 0
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

    ZS::Test::CTestStepGroup* pGrpLinesSmallPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameSmallPlusSign,
        /* pTSGrpParent */ pGrpLines );
    createTestGroupAddStandardShapesLinesSmallPlusSign(pGrpLinesSmallPlusSign);

    // BigPlusSign
    //------------

    ZS::Test::CTestStepGroup* pGrpLinesBigPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameBigPlusSign,
        /* pTSGrpParent */ pGrpLines );
    createTestGroupAddStandardShapesLinesBigPlusSign(pGrpLinesBigPlusSign);

    // Checkmark
    //----------

    ZS::Test::CTestStepGroup* pGrpLinesCheckmark = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameCheckmark,
        /* pTSGrpParent */ pGrpLines );
    createTestGroupAddStandardShapesLinesCheckmark(pGrpLinesCheckmark);

    // SmallRect
    //----------

    ZS::Test::CTestStepGroup* pGrpLinesSmallRect = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameSmallRect,
        /* pTSGrpParent */ pGrpLines );
    createTestGroupAddStandardShapesLinesSmallRect(pGrpLinesSmallRect);

    createTestStepSaveLoadFile(pGrpLines);
#endif

    // Polylines
    //==========

#if 0
    ZS::Test::CTestStepGroup* pGrpPolylines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Polylines",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpPolylines,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    createTestGroupAddStandardShapesPolylineStar(pGrpPolylines);
    createTestStepSaveLoadFile(pGrpPolylines);
#endif

    // Polygons
    //=========

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
#if 0
    createTestGroupAddStandardShapesPolygonStar(pGrpPolygons);
#endif
    createTestGroupAddStandardShapesPolygonTriangle(pGrpPolygons);
    createTestStepSaveLoadFile(pGrpPolygons);

    // Groups
    //=======

    ZS::Test::CTestStepGroup* pGrpGroups = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Groups",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // SmallPlusSign
    //--------------

//#if 0
    ZS::Test::CTestStepGroup* pGrpGroupsSmallPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " "  + c_strGraphObjNameSmallPlusSign,
        /* pTSGrpParent */ pGrpGroups );

#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpGroupsSmallPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();
#endif
    ZS::Test::CTestStepGroup* pGrpGroupsSmallPlusSignAddShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Add Shapes",
        /* pTSGrpParent */ pGrpGroupsSmallPlusSign );

    createTestGroupAddStandardShapesGroupSmallPlusSign(pGrpGroupsSmallPlusSignAddShapes);
#if 0
    createTestGroupAddStandardShapesGroupSmallPlusSignResize(pGrpGroupsSmallPlusSign);
#endif
    createTestStepSaveLoadFile(pGrpGroupsSmallPlusSign);
//#endif

    // BigPlusSign
    //------------

#if 0
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

    ZS::Test::CTestStepGroup* pGrpGroupsBigPlusSignRotateTopAddShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Add Shapes",
        /* pTSGrpParent */ pGrpGroupsBigPlusSignRotateTop );

    createTestGroupAddStandardShapesGroupBigPlusSign(pGrpGroupsBigPlusSignRotateTopAddShapes);
    createTestGroupAddStandardShapesGroupBigPlusSignRotateParent(pGrpGroupsBigPlusSignRotateTop);
    createTestStepSaveLoadFile(pGrpGroupsBigPlusSignRotateTop);

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

    ZS::Test::CTestStepGroup* pGrpGroupsBigPlusSignRotateAndResizeAddShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Add Shapes",
        /* pTSGrpParent */ pGrpGroupsBigPlusSignRotateAndResize );

    createTestGroupAddStandardShapesGroupBigPlusSign(pGrpGroupsBigPlusSignRotateAndResizeAddShapes);
    createTestGroupAddStandardShapesGroupBigPlusSignModfications(pGrpGroupsBigPlusSignRotateAndResize);
    createTestStepSaveLoadFile(pGrpGroupsBigPlusSignRotateAndResize);
#endif

    // PlusSigns
    //----------

#if 0
    ZS::Test::CTestStepGroup* pGrpGroupsPlusSigns = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " PlusSigns",
        /* pTSGrpParent */ pGrpGroups );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpGroupsPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    ZS::Test::CTestStepGroup* pGrpGroupsPlusSignsAddShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Add Shapes",
        /* pTSGrpParent */ pGrpGroupsPlusSigns );

    createTestGroupAddStandardShapesGroupPlusSigns(pGrpGroupsPlusSignsAddShapes);
    createTestGroupAddStandardShapesGroupPlusSignsModifications(pGrpGroupsPlusSigns);
    createTestStepSaveLoadFile(pGrpGroupsPlusSigns);
#endif

    // Checkmark
    //------------

#if 0
    ZS::Test::CTestStepGroup* pGrpGroupsCheckmark = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " "  + c_strGraphObjNameCheckmark,
        /* pTSGrpParent */ pGrpGroups );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpGroupsCheckmark,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    ZS::Test::CTestStepGroup* pGrpGroupsCheckmarkAddShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Add Shapes",
        /* pTSGrpParent */ pGrpGroupsCheckmark );

    createTestGroupAddStandardShapesGroupCheckmark(pGrpGroupsCheckmarkAddShapes);
    createTestGroupAddStandardShapesGroupCheckmarkModifications(pGrpGroupsCheckmark);
    createTestStepSaveLoadFile(pGrpGroupsCheckmark);
#endif

    // Polygons
    //---------

#if 0
    ZS::Test::CTestStepGroup* pGrpGroupsPolygons = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " "  + c_strGraphObjNamePolygons,
        /* pTSGrpParent */ pGrpGroups );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpGroupsPolygons,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    ZS::Test::CTestStepGroup* pGrpGroupsPolygonsAddShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Add Shapes",
        /* pTSGrpParent */ pGrpGroupsPolygons );

    createTestGroupAddStandardShapesGroupPolygons(pGrpGroupsPolygonsAddShapes);
    createTestGroupAddStandardShapesGroupPolygonsModifications(pGrpGroupsPolygons);
    createTestStepSaveLoadFile(pGrpGroupsPolygons);
#endif

    // All
    //----

#if 0
    ZS::Test::CTestStepGroup* pGrpGroupsAll = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " All",
        /* pTSGrpParent */ pGrpGroups );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpGroupsAll,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    ZS::Test::CTestStepGroup* pGrpGroupsAllAddShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Add Shapes",
        /* pTSGrpParent */ pGrpGroupsAll );

    createTestGroupAddStandardShapesAllGroups(pGrpGroupsAllAddShapes);
    createTestGroupAddStandardShapesAllGroupsModifications(pGrpGroupsAll);
    createTestStepSaveLoadFile(pGrpGroupsAll);
#endif
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesLinesSmallPlusSign(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesLinesSmallPlusSign",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

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

    // Vertical Line
    //--------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameSmallPlusSignVerticalLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallPlusSignVerticalLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallPlusSignVerticalLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSignVerticalLine));
    QPointF ptP1SmallPlusSignVerticalLine(275.0, bYAxisTopDown ? 310.0 : 290.0);
    QPointF ptP2SmallPlusSignVerticalLine(275.0, bYAxisTopDown ? 340.0 : 260.0);
    m_ptPosSmallPlusSignVerticalLine = QPointF(275.0, 325.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -15.0), QPointF(0.0, 15.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP1(ptP1SmallPlusSignVerticalLine);
    m_pPhysValLineSmallPlusSignVerticalLine->setP2(ptP2SmallPlusSignVerticalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSignVerticalLine);
    pTestStep->setConfigValue("P1", ptP1SmallPlusSignVerticalLine);
    pTestStep->setConfigValue("P2", ptP2SmallPlusSignVerticalLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Horizontal Line
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameSmallPlusSignHorizontalLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallPlusSignHorizontalLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallPlusSignHorizontalLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSignHorizontalLine));
    QPointF ptP1SmallPlusSignHorizontalLine(260.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptP2SmallPlusSignHorizontalLine(290.0, bYAxisTopDown ? 325.0 : 275.0);
    m_ptPosSmallPlusSignHorizontalLine = QPointF(275.0, 325.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP1(ptP1SmallPlusSignHorizontalLine);
    m_pPhysValLineSmallPlusSignHorizontalLine->setP2(ptP2SmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("P1", ptP1SmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("P2", ptP2SmallPlusSignHorizontalLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesLinesBigPlusSign(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesLinesBigPlusSign",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

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

    // Vertical Line
    //--------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameBigPlusSignVerticalLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameBigPlusSignVerticalLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameBigPlusSignVerticalLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSignVerticalLine));
    QPointF ptP1BigPlusSignVerticalLine(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptP2BigPlusSignVerticalLine(300.0, bYAxisTopDown ? 350.0 : 250.0);
    m_ptPosBigPlusSignVerticalLine = QPointF(300.0, 300.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(ptP1BigPlusSignVerticalLine);
    m_pPhysValLineBigPlusSignVerticalLine->setP2(ptP2BigPlusSignVerticalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSignVerticalLine);
    pTestStep->setConfigValue("P1", ptP1BigPlusSignVerticalLine);
    pTestStep->setConfigValue("P2", ptP2BigPlusSignVerticalLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Horizontal Line
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameBigPlusSignHorizontalLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameBigPlusSignHorizontalLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameBigPlusSignHorizontalLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSignHorizontalLine));
    QPointF ptP1BigPlusSignHorizontalLine(250.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptP2BigPlusSignHorizontalLine(350.0, bYAxisTopDown ? 300.0 : 300.0);
    m_ptPosBigPlusSignHorizontalLine = QPointF(300.0, 300.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(ptP1BigPlusSignHorizontalLine);
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(ptP2BigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("P1", ptP1BigPlusSignHorizontalLine);
    pTestStep->setConfigValue("P2", ptP2BigPlusSignHorizontalLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesLinesCheckmark(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesLinesCheckmark",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

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

    // Left Line
    //----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameCheckmarkLeftLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameCheckmarkLeftLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameCheckmarkLeftLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameCheckmarkLeftLine));
    QPointF ptP1CheckmarkLeftLine(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptP2CheckmarkLeftLine(325.0, bYAxisTopDown ? 350.0 : 250.0);
    m_ptPosCheckmarkLeftLine = QPointF(287.5, 300.0);
    m_lineCheckmarkLeftLine = QLineF(QPointF(-37.5, -50.0), QPointF(37.5, 50.0));
    m_pPhysValLineCheckmarkLeftLine->setP1(ptP1CheckmarkLeftLine);
    m_pPhysValLineCheckmarkLeftLine->setP2(ptP2CheckmarkLeftLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmarkLeftLine);
    pTestStep->setConfigValue("P1", ptP1CheckmarkLeftLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkLeftLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkLeftLine, m_ptPosCheckmarkLeftLine,
        m_lineCheckmarkLeftLine, *m_pPhysValLineCheckmarkLeftLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Right Line
    //-----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameCheckmarkRightLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameCheckmarkRightLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameCheckmarkRightLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameCheckmarkRightLine));
    QPointF ptP1CheckmarkRightLine(325.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptP2CheckmarkRightLine(350.0, bYAxisTopDown ? 325.0 : 275.0);
    m_ptPosCheckmarkRightLine = QPointF(337.5, 337.5);
    m_lineCheckmarkRightLine = QLineF(QPointF(-12.5, 12.5), QPointF(12.5, -12.5));
    m_pPhysValLineCheckmarkRightLine->setP1(ptP1CheckmarkRightLine);
    m_pPhysValLineCheckmarkRightLine->setP2(ptP2CheckmarkRightLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmarkRightLine);
    pTestStep->setConfigValue("P1", ptP1CheckmarkRightLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkRightLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkRightLine, m_ptPosCheckmarkRightLine,
        m_lineCheckmarkRightLine, *m_pPhysValLineCheckmarkRightLine));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesLinesSmallRect(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesLinesSmallRect",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

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

    // Top Line
    //---------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameSmallRectTopLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectTopLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallRectTopLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallRectTopLine));
    QPointF ptP1SmallRectTopLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectTopLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    m_ptPosSmallRectTopLine = QPointF(325.0, 270.0);
    m_lineSmallRectTopLine = QLineF(QPointF(-5.0, 0.0), QPointF(5.0, 0.0));
    m_pPhysValLineSmallRectTopLine->setP1(ptP1SmallRectTopLine);
    m_pPhysValLineSmallRectTopLine->setP2(ptP2SmallRectTopLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallRectTopLine);
    pTestStep->setConfigValue("P1", ptP1SmallRectTopLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectTopLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectTopLine, m_ptPosSmallRectTopLine,
        m_lineSmallRectTopLine, *m_pPhysValLineSmallRectTopLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Right Line
    //-----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameSmallRectRightLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectRightLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallRectRightLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallRectRightLine));
    QPointF ptP1SmallRectRightLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectRightLine(330.0, bYAxisTopDown ? 280.0 : 320.0);
    m_ptPosSmallRectRightLine = QPointF(330.0, 275.0);
    m_lineSmallRectRightLine = QLineF(QPointF(0.0, -5.0), QPointF(0.0, 5.0));
    m_pPhysValLineSmallRectRightLine->setP1(ptP1SmallRectRightLine);
    m_pPhysValLineSmallRectRightLine->setP2(ptP2SmallRectRightLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallRectRightLine);
    pTestStep->setConfigValue("P1", ptP1SmallRectRightLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectRightLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectRightLine, m_ptPosSmallRectRightLine,
        m_lineSmallRectRightLine, *m_pPhysValLineSmallRectRightLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Bottom Line
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameSmallRectBottomLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectBottomLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallRectBottomLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallRectBottomLine));
    QPointF ptP1SmallRectBottomLine(330.0, bYAxisTopDown ? 280.0 : 320.0);
    QPointF ptP2SmallRectBottomLine(320.0, bYAxisTopDown ? 280.0 : 320.0);
    m_ptPosSmallRectBottomLine = QPointF(325.0, 280.0);
    m_lineSmallRectBottomLine = QLineF(QPointF(5.0, 0.0), QPointF(-5.0, 0.0));
    m_pPhysValLineSmallRectBottomLine->setP1(ptP1SmallRectBottomLine);
    m_pPhysValLineSmallRectBottomLine->setP2(ptP2SmallRectBottomLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallRectBottomLine);
    pTestStep->setConfigValue("P1", ptP1SmallRectBottomLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectBottomLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectBottomLine, m_ptPosSmallRectBottomLine,
        m_lineSmallRectBottomLine, *m_pPhysValLineSmallRectBottomLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Left Line
    //----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameSmallRectLeftLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectLeftLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallRectLeftLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallRectLeftLine));
    QPointF ptP1SmallRectLeftLine(320.0, bYAxisTopDown ? 280.0 : 320.0);
    QPointF ptP2SmallRectLeftLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    m_ptPosSmallRectLeftLine = QPointF(320.0, 275.0);
    m_lineSmallRectLeftLine = QLineF(QPointF(0.0, 5.0), QPointF(0.0, -5.0));
    m_pPhysValLineSmallRectLeftLine->setP1(ptP1SmallRectLeftLine);
    m_pPhysValLineSmallRectLeftLine->setP2(ptP2SmallRectLeftLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallRectLeftLine);
    pTestStep->setConfigValue("P1", ptP1SmallRectLeftLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectLeftLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectLeftLine, m_ptPosSmallRectLeftLine,
        m_lineSmallRectLeftLine, *m_pPhysValLineSmallRectLeftLine));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesPolylineStar(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesPolylineStar",
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
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

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

    // Star
    //-----

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameStar + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameStar + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjPolygon(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameStar, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar));
    m_ptPosPolygonStar = QPointF(300.0, 300.0);
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
        {300.0, bYAxisTopDown ? 200.0 : fYAxisMaxVal - 200.0},
        {325.0, bYAxisTopDown ? 275.0 : fYAxisMaxVal - 275.0},
        {400.0, bYAxisTopDown ? 300.0 : fYAxisMaxVal - 300.0},
        {325.0, bYAxisTopDown ? 325.0 : fYAxisMaxVal - 325.0},
        {300.0, bYAxisTopDown ? 400.0 : fYAxisMaxVal - 400.0},
        {275.0, bYAxisTopDown ? 325.0 : fYAxisMaxVal - 325.0},
        {200.0, bYAxisTopDown ? 300.0 : fYAxisMaxVal - 300.0},
        {275.0, bYAxisTopDown ? 275.0 : fYAxisMaxVal - 275.0}
    });
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypePolyline));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameStar);
    pTestStep->setConfigValue("Points", m_pPhysValPolygonStar->toQPolygonF());
    pTestStep->setConfigValue("Points.Unit", strUnit);
    pTestStep->setConfigValue("ResultValuesPrecision", iDigits);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameStar, m_ptPosPolygonStar, m_polygonStar, *m_pPhysValPolygonStar, iDigits));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesPolygonStar(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesPolygonStar",
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
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameStar + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameStar + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjPolygon(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameStar, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameStar));
    m_ptPosPolygonStar = QPointF(300.0, 300.0);
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
        {300.0, bYAxisTopDown ? 200.0 : fYAxisMaxVal - 200.0},
        {325.0, bYAxisTopDown ? 275.0 : fYAxisMaxVal - 275.0},
        {400.0, bYAxisTopDown ? 300.0 : fYAxisMaxVal - 300.0},
        {325.0, bYAxisTopDown ? 325.0 : fYAxisMaxVal - 325.0},
        {300.0, bYAxisTopDown ? 400.0 : fYAxisMaxVal - 400.0},
        {275.0, bYAxisTopDown ? 325.0 : fYAxisMaxVal - 325.0},
        {200.0, bYAxisTopDown ? 300.0 : fYAxisMaxVal - 300.0},
        {275.0, bYAxisTopDown ? 275.0 : fYAxisMaxVal - 275.0}
    });
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypePolygon));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameStar);
    pTestStep->setConfigValue("Points", m_pPhysValPolygonStar->toQPolygonF());
    pTestStep->setConfigValue("Points.Unit", strUnit);
    pTestStep->setConfigValue("ResultValuesPrecision", iDigits);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameStar, m_ptPosPolygonStar, m_polygonStar, *m_pPhysValPolygonStar, iDigits));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameStar + ".showLabels",
        /* strOperation    */ c_strGraphObjNameStar + ".showLabels()",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameStar]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(340.0, 220.0)},
        {"ExpectedText", c_strGraphObjNameStar}
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameStar},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameStar]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameP + "3"},
        {"setPos", QPointF(420.0, 290.0)},
        {"ExpectedText", CGraphObj::c_strGeometryLabelNameP + "3"}
    });
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesPolygonTriangle(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesPolygonTriangle",
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
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameTriangle + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameTriangle + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjPolygon(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameTriangle, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTriangle));
    m_ptPosPolygonTriangle = QPointF(300.0, 287.5);
    m_polygonTriangle = QPolygonF({
        {-25.0,  12.5},
        { 25.0,  12.5},
        {  0.0, -12.5}
    });
    *m_pPhysValPolygonTriangle = QPolygonF({
        {275.0, bYAxisTopDown ? 300.0 : fYAxisMaxVal - 300.0},
        {325.0, bYAxisTopDown ? 300.0 : fYAxisMaxVal - 300.0},
        {300.0, bYAxisTopDown ? 275.0 : fYAxisMaxVal - 275.0}
    });
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypePolygon));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTriangle);
    pTestStep->setConfigValue("Points", m_pPhysValPolygonTriangle->toQPolygonF());
    pTestStep->setConfigValue("Points.Unit", strUnit);
    pTestStep->setConfigValue("ResultValuesPrecision", iDigits);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameTriangle, m_ptPosPolygonTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, iDigits));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTriangle + ".showLabels",
        /* strOperation    */ c_strGraphObjNameTriangle + ".showLabels()",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameTriangle},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]},
        {"LabelName", CGraphObj::c_strLabelName},
        {"AnchorPoint", SGraphObjSelectionPoint(nullptr, ESelectionPointType::LineCenterPoint, 1).toString()},
        {"setPos", QPointF(315.0, 270.0)},
        {"ExpectedText", c_strGraphObjNameTriangle}
    });
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesGroupSmallPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesGroupSmallPlusSign",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

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
    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;

    createTestGroupAddStandardShapesLinesSmallPlusSign(i_pTestStepGroupParent);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameSmallPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallPlusSign + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallPlusSign, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSign));
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group
    m_ptPosSmallPlusSign = QPointF(275.0, 325.0);
    QPointF ptTLSmallPlusSign(260.0, bYAxisTopDown ? 310.0 : 290.0);
    m_sizeSmallPlusSign = QSizeF(30.0, 30.0);
    *m_pPhysValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, m_sizeSmallPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -15.0), QPointF(0.0, 15.0));
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(15.0, bYAxisTopDown ? 0.0 : 30.0), QPointF(15.0, bYAxisTopDown ? 30.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 15.0), QPointF(30.0, 15.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesGroupSmallPlusSignResize(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesGroupSmallPlusSignResize",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjTypeGroup = graphObjType2Str(EGraphObjTypeGroup);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();
    QString strUnit = unit.symbol();
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
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
    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;
    QStringList strlstGraphObjsKeyInTreeGetResultValues;

    ZS::Test::CTestStepGroup* pGrpModifySmallPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify " + c_strGraphObjNameSmallPlusSign,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Resize SmallPlusSign
    //---------------------

    m_sizeSmallPlusSign = QSizeF(30.0, 60.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameSmallPlusSign + ".setSize(" + qSize2Str(m_sizeSmallPlusSign) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameSmallPlusSign + ".setSize(" + qSize2Str(m_sizeSmallPlusSign) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifySmallPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign]);
    pTestStep->setConfigValue("Method", "setSize");
    pTestStep->setConfigValue("Size", m_sizeSmallPlusSign);
    pTestStep->setConfigValue("Size.unit", unit.symbol());
    strlstExpectedValues.clear();
    // Group SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(275.0, bYAxisTopDown ? 340.0 : 310);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -30.0), QPointF(0.0, 30.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP1(QPointF(15.0, bYAxisTopDown ? 0.0 : 60.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP2(QPointF(15.0, bYAxisTopDown ? 60.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP1(QPointF(0.0, 30.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP2(QPointF(30.0, 30.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize SmallPlusSign
    //---------------------

    m_sizeSmallPlusSign.setHeight(120.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameSmallPlusSign + ".setHeight(" + QString::number(m_sizeSmallPlusSign.height()) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameSmallPlusSign + ".setHeight(" + QString::number(m_sizeSmallPlusSign.height()) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifySmallPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign]);
    pTestStep->setConfigValue("Method", "setHeight");
    pTestStep->setConfigValue("Height", m_sizeSmallPlusSign.height());
    pTestStep->setConfigValue("Height.unit", unit.symbol());
    strlstExpectedValues.clear();
    // Group SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(275.0, bYAxisTopDown ? 370.0 : 280.0);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -60.0), QPointF(0.0, 60.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP1(QPointF(15.0, bYAxisTopDown ? 0.0 : 120.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP2(QPointF(15.0, bYAxisTopDown ? 120.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP1(QPointF(0.0, 60.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP2(QPointF(30.0, 60.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesGroupBigPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesGroupBigPlusSign",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

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
    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;

    createTestGroupAddStandardShapesLinesBigPlusSign(i_pTestStepGroupParent);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameBigPlusSign, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group
    m_ptPosBigPlusSign = QPointF(300.0, 300.0);
    QPointF ptTLBigPlusSign(250.0, bYAxisTopDown ? 250.0 : 350.0);
    m_sizeBigPlusSign = QSizeF(100.0, 100.0);
    *m_pPhysValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, m_sizeBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0), QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    *m_pPhysValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesGroupBigPlusSignRotateParent(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesGroupBigPlusSignRotateParent",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjTypeGroup = graphObjType2Str(EGraphObjTypeGroup);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();
    QString strUnit = unit.symbol();
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
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
    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;
    QStringList strlstGraphObjsKeyInTreeGetResultValues;

    ZS::Test::CTestStepGroup* pGrpModifyBigPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify " + c_strGraphObjNameTopGroup,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Create TopGroup
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + graphObjType2Str(EGraphObjTypeGroup) + ", " + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameTopGroup, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
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
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(50.0, 50.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    pTestStep->setExpectedValues(strlstExpectedValues);
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".showLabel(" + CGraphObj::c_strLabelName + ")",
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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".showLabels",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".showLabels()",
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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".showGeometryLabels",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".showGeometryLabels()",
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

    m_physValAngleTopGroup.setVal(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setRotationAngle(" + m_physValAngleTopGroup.toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setRotationAngle(" + m_physValAngleTopGroup.toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", m_physValAngleTopGroup.toString());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // BigPlusSignParentGroup
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_pPhysValRectTopGroup->setAngle(m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
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
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    m_hshGraphObjNameToKeys.remove(c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("Method", "ungroup");
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
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // Vertical Line
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // Horizontal Line
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, bYAxisTopDown ? 50.0 : 50.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(100.0, bYAxisTopDown ? 50.0 : 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
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
void CTest::createTestGroupAddStandardShapesGroupBigPlusSignModfications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesGroupBigPlusSignModfications",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjTypeGroup = graphObjType2Str(EGraphObjTypeGroup);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();
    QString strUnit = unit.symbol();
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
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
    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;
    QStringList strlstGraphObjsKeyInTreeGetResultValues;

    ZS::Test::CTestStepGroup* pGrpModifyBigPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify " + c_strGraphObjNameBigPlusSign,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Ungroup BigPlusSign
    //----------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingScene.ungroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    m_hshGraphObjNameToKeys.remove(c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("Method", "ungroup");
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
    QPointF ptP1BigPlusSignHorizontalLine(250.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptP2BigPlusSignHorizontalLine(350.0, bYAxisTopDown ? 300.0 : 300.0);
    m_ptPosBigPlusSignHorizontalLine = QPointF(300.0, 300.0);
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(ptP1BigPlusSignHorizontalLine);
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(ptP2BigPlusSignHorizontalLine);
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Group BigPlusSign
    //------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, bYAxisTopDown ? 50.0 : 50.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(100.0, bYAxisTopDown ? 50.0 : 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate BigPlusSign
    //-------------------

    m_physValAngleBigPlusSign.setVal(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", m_physValAngleBigPlusSign.toString());
    strlstExpectedValues.clear();
    // BigPlusSign
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Labels
    //------------

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
        {"setPos", QPointF(340.0, 240.0)},
        {"ExpectedText", c_strGraphObjNameBigPlusSign}
    });

    // Show Geometry Labels
    //---------------------

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
        {"setPos", QPointF(280.0, 200.0)},
        {"ExpectedText", m_pPhysValRectBigPlusSign->topLeft().toString()},
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameBigPlusSign},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameCenter},
        {"setPos", QPointF(310.0, 300.0)},
        {"ExpectedText", m_pPhysValRectBigPlusSign->center().toString()},
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameBigPlusSign},
        {"GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"setPos", QPointF(350.0, bYAxisTopDown ? 310.0 : 290.0)},
        {"ExpectedText", m_physValAngleBigPlusSign.toString()},
    });

    // Resize BigPlusSign
    //-------------------

    m_sizeBigPlusSign = QSizeF(200.0, 100.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setSize(" + qSize2Str(m_sizeBigPlusSign) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setBottomRight(" + qSize2Str(m_sizeBigPlusSign) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    pTestStep->setConfigValue("Method", "setSize");
    pTestStep->setConfigValue("Size", m_sizeBigPlusSign);
    pTestStep->setConfigValue("Size.unit", unit.symbol());
    strlstExpectedValues.clear();
    // Group PlusSigns
    m_ptPosBigPlusSign = QPointF(335.4, 335.4);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(100.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(100.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-100.0, 0.0), QPointF(100.0, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(200.0, 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate BigPlusSign
    //-------------------

    m_physValAngleBigPlusSign.setVal(120.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", m_physValAngleBigPlusSign.toString());
    strlstExpectedValues.clear();
    // Group
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
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

    // Rotate BigPlusSign
    //-------------------

    m_physValAngleBigPlusSign.setVal(255.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", m_physValAngleBigPlusSign.toString());
    strlstExpectedValues.clear();
    // Group
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize BigPlusSign
    //-------------------

    m_sizeBigPlusSign = QSizeF(200.0, 200.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setSize(" + qSize2Str(m_sizeBigPlusSign) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setSize(" + qSize2Str(m_sizeBigPlusSign) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    pTestStep->setConfigValue("Method", "setSize");
    pTestStep->setConfigValue("Size", m_sizeBigPlusSign);
    pTestStep->setConfigValue("Size.unit", unit.symbol());
    strlstExpectedValues.clear();
    // Group BigPlusSign
    m_ptPosBigPlusSign = bYAxisTopDown ? QPointF(383.7, 322.4) : QPointF(287.1, 348.3);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -100.0), QPointF(0.0, 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(100.0, bYAxisTopDown ? 0.0 : 200.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(100.0, bYAxisTopDown ? 200.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-100.0, 0.0), QPointF(100.0, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, 100.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(200.0, 100.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize BigPlusSign
    //-------------------

    m_sizeBigPlusSign = QSizeF(100.0, 100.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setSize(" + qSize2Str(m_sizeBigPlusSign) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setSize(" + qSize2Str(m_sizeBigPlusSign) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    pTestStep->setConfigValue("Method", "setSize");
    pTestStep->setConfigValue("Size", m_sizeBigPlusSign);
    pTestStep->setConfigValue("Size.unit", unit.symbol());
    strlstExpectedValues.clear();
    // Group BigPlusSigns
    m_ptPosBigPlusSign = QPointF(348.3, 383.7);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(100.0, 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate BigPlusSign
    //-------------------

    m_physValAngleBigPlusSign.setVal(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", m_physValAngleBigPlusSign.toString());
    strlstExpectedValues.clear();
    // Group
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Move BigPlusSign
    //-----------------

    m_pPhysValRectBigPlusSign->setCenter(QPointF(300.0, 300.0));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setCenter(" + m_pPhysValRectBigPlusSign->center().toString() + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setCenter(" + m_pPhysValRectBigPlusSign->center().toString() + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("Method", "setCenter");
    pTestStep->setConfigValue("Center", m_pPhysValRectBigPlusSign->center().toQPointF());
    pTestStep->setConfigValue("Center.unit", unit.symbol());
    strlstExpectedValues.clear();
    // Group
    m_ptPosBigPlusSign = QPointF(300.0, 300.0);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Move BigPlusSign
    //-----------------

    m_pPhysValRectBigPlusSign->setCenter(QPointF(400.0, 400.0));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setPosition(" + m_pPhysValRectBigPlusSign->center().toString() + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setPosition(" + m_pPhysValRectBigPlusSign->center().toString() + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("Method", "setPosition");
    pTestStep->setConfigValue("Pos", m_pPhysValRectBigPlusSign->center().toQPointF());
    pTestStep->setConfigValue("Pos.unit", unit.symbol());
    strlstExpectedValues.clear();
    // Group
    m_ptPosBigPlusSign = QPointF(400.0, bYAxisTopDown ? 400.0 : 200.0);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Move BigPlusSign
    //-----------------

    m_pPhysValRectBigPlusSign->setCenter(QPointF(300.0, 300.0));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setCenter(" + m_pPhysValRectBigPlusSign->center().toString() + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setCenter(" + m_pPhysValRectBigPlusSign->center().toString() + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("Method", "setCenter");
    pTestStep->setConfigValue("Center", m_pPhysValRectBigPlusSign->center().toQPointF());
    pTestStep->setConfigValue("Center.unit", unit.symbol());
    strlstExpectedValues.clear();
    // Group
    m_ptPosBigPlusSign = QPointF(300.0, 300.0);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Create TopGroup
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + graphObjType2Str(EGraphObjTypeGroup) + ", " + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameTopGroup, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // TopGroup
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_sizeTopGroup = QSizeF(Math::sqrt(2.0) * 100.0, Math::sqrt(2.0) * 100.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(QPointF(300.0, bYAxisTopDown ? 300.0 : 300.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(Math::sqrt(2.0)/2.0 * 100.0, Math::sqrt(2.0)/2.0 * 100.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    pTestStep->setExpectedValues(strlstExpectedValues);
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize TopGroup
    //----------------

    m_sizeTopGroup.setWidth(200.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setWidth(" + QString::number(m_sizeTopGroup.width()) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setWidth(" + QString::number(m_sizeTopGroup.width()) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("Method", "setWidth");
    pTestStep->setConfigValue("Width", m_sizeTopGroup.width());
    pTestStep->setConfigValue("Width.unit", unit.symbol());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSignParent
    m_ptPosTopGroup.setX(329.3);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // Group BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign.setWidth(Math::sqrt(2.0) * 100.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(100.0, Math::sqrt(2.0)/2.0 * 100.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(Math::sqrt(2.0)/2.0 * 100.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(Math::sqrt(2.0)/2.0 * 100.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-Math::sqrt(2.0)/2.0 * 100.0, 0.0), QPointF(Math::sqrt(2.0)/2.0 * 100.0, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(Math::sqrt(2.0) * 100.0, 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize TopGroup
    //----------------

    m_sizeTopGroup.setHeight(300.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setHeight(" + QString::number(m_sizeTopGroup.height()) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setHeight(" + QString::number(m_sizeTopGroup.height()) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("Method", "setHeight");
    pTestStep->setConfigValue("Height", m_sizeTopGroup.height());
    pTestStep->setConfigValue("Height.unit", unit.symbol());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSignParent
    m_ptPosTopGroup.setY(bYAxisTopDown ? 379.3 : 220.7);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // Group BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = QSizeF(141.42, 212.13);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(100.0, 150.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -106.065), QPointF(0.0, 106.065));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(Math::sqrt(2.0)/2.0 * 100.0, bYAxisTopDown ? 0.0 : 212.13));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(Math::sqrt(2.0)/2.0 * 100.0, bYAxisTopDown ? 212.13 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-Math::sqrt(2.0)/2.0 * 100.0, 0.0), QPointF(Math::sqrt(2.0)/2.0 * 100.0, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, 106.065));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(Math::sqrt(2.0) * 100.0, 106.065));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize TopGroup
    //----------------

    m_sizeTopGroup = QSizeF(Math::sqrt(2.0) * 100.0, Math::sqrt(2.0) * 100.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setSize(" + qSize2Str(m_sizeTopGroup) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setSize(" + qSize2Str(m_sizeTopGroup) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("Method", "setSize");
    pTestStep->setConfigValue("Size", m_sizeTopGroup);
    pTestStep->setConfigValue("Size.unit", unit.symbol());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSignParent
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // Group BigPlusSign
    m_sizeBigPlusSign = QSizeF(100.0, 100.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(Math::sqrt(2.0)/2.0 * 100.0, Math::sqrt(2.0)/2.0 * 100.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(100.0, 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate TopGroup
    //----------------

    m_physValAngleTopGroup.setVal(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setRotationAngle(" + m_physValAngleTopGroup.toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setRotationAngle(" + m_physValAngleTopGroup.toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", m_physValAngleTopGroup.toString());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // BigPlusSignParentGroup
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_pPhysValRectTopGroup->setAngle(m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize TopGroup
    //----------------

    m_sizeTopGroup.setWidth(200.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setWidth(" + QString::number(m_sizeTopGroup.width()) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setWidth(" + QString::number(m_sizeTopGroup.width()) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("Method", "setWidth");
    pTestStep->setConfigValue("Width", m_sizeTopGroup.width());
    pTestStep->setConfigValue("Width.unit", unit.symbol());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSignParent
    m_ptPosTopGroup = QPointF(320.7, 320.7);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // Group BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = QSizeF(Math::sqrt(2.0) * 100.0, 100.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(100.0, Math::sqrt(2.0)/2.0 * 100.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(Math::sqrt(2.0)/2.0 * 100.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(Math::sqrt(2.0)/2.0 * 100.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-Math::sqrt(2.0)/2.0 * 100.0, 0.0), QPointF(Math::sqrt(2.0)/2.0 * 100.0, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(Math::sqrt(2.0) * 100.0, 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize TopGroup
    //----------------

    m_sizeTopGroup.setHeight(200.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setHeight(" + QString::number(m_sizeTopGroup.height()) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setHeight(" + QString::number(m_sizeTopGroup.height()) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("Method", "setHeight");
    pTestStep->setConfigValue("Height", m_sizeTopGroup.height());
    pTestStep->setConfigValue("Height.unit", unit.symbol());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSignParent
    m_ptPosTopGroup = bYAxisTopDown ? QPointF(300.0, 341.4) : QPointF(341.4, 300.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // Group BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = QSizeF(Math::sqrt(2.0) * 100.0, Math::sqrt(2.0) * 100.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(100.0, 100.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -Math::sqrt(2.0)/2.0 * 100.0), QPointF(0.0, Math::sqrt(2.0)/2.0 * 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(Math::sqrt(2.0)/2.0 * 100.0, bYAxisTopDown ? 0.0 : Math::sqrt(2.0) * 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(Math::sqrt(2.0)/2.0 * 100.0, bYAxisTopDown ? Math::sqrt(2.0) * 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-Math::sqrt(2.0)/2.0 * 100.0, 0.0), QPointF(Math::sqrt(2.0)/2.0 * 100.0, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, Math::sqrt(2.0)/2.0 * 100.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(Math::sqrt(2.0) * 100.0, Math::sqrt(2.0)/2.0 * 100.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize TopGroup
    //----------------

    m_sizeTopGroup = QSizeF(Math::sqrt(2.0) * 100.0, Math::sqrt(2.0) * 100.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setSize(" + qSize2Str(m_sizeTopGroup) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setSize(" + qSize2Str(m_sizeTopGroup) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("Method", "setSize");
    pTestStep->setConfigValue("Size", m_sizeTopGroup);
    pTestStep->setConfigValue("Size.unit", unit.symbol());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSignParent
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // Group BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = QSizeF(100.0, 100.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(Math::sqrt(2.0)/2.0 * 100.0, Math::sqrt(2.0)/2.0 * 100.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(100.0, 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate TopGroup
    //----------------

    m_physValAngleTopGroup.setVal(0.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setRotationAngle(" + m_physValAngleTopGroup.toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setRotationAngle(" + m_physValAngleTopGroup.toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", m_physValAngleTopGroup.toString());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // BigPlusSignParentGroup
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_pPhysValRectTopGroup->setAngle(m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Ungroup TopGroup
    //-----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    m_hshGraphObjNameToKeys.remove(c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("Method", "ungroup");
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSign
    m_ptPosBigPlusSign = QPointF(300.0, 300.0);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(300.0, 300.0));
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign + m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // Vertical Line
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // Horizontal Line
    ptP1BigPlusSignHorizontalLine = QPointF(0.0, bYAxisTopDown ? 50.0 : 50.0);
    ptP2BigPlusSignHorizontalLine = QPointF(100.0, bYAxisTopDown ? 50.0 : 50.0);
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(ptP1BigPlusSignHorizontalLine);
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(ptP2BigPlusSignHorizontalLine);
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Create TopGroup
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + graphObjType2Str(EGraphObjTypeGroup) + ", " + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameTopGroup, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // TopGroup
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_sizeTopGroup = QSizeF(Math::sqrt(2.0) * 100.0, Math::sqrt(2.0) * 100.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(QPointF(300.0, bYAxisTopDown ? 300.0 : 300.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(Math::sqrt(2.0)/2.0 * 100.0, Math::sqrt(2.0)/2.0 * 100.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    pTestStep->setExpectedValues(strlstExpectedValues);
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate TopGroup
    //----------------

    m_physValAngleTopGroup.setVal(215.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setRotationAngle(" + m_physValAngleTopGroup.toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setRotationAngle(" + m_physValAngleTopGroup.toString() + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", m_physValAngleTopGroup.toString());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // BigPlusSignParentGroup
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_pPhysValRectTopGroup->setAngle(m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Ungroup TopGroup
    //-----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    m_hshGraphObjNameToKeys.remove(c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("Method", "ungroup");
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group BigPlusSign
    m_ptPosBigPlusSign = QPointF(300.0, 300.0);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(300.0, 300.0));
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign + m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // Vertical Line
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // Horizontal Line
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, bYAxisTopDown ? 50.0 : 50.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(100.0, bYAxisTopDown ? 50.0 : 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesGroupPlusSigns(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesGroups",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

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
    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;

    createTestGroupAddStandardShapesLinesSmallPlusSign(i_pTestStepGroupParent);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameSmallPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallPlusSign + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallPlusSign, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSign));
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group
    m_ptPosSmallPlusSign = QPointF(275.0, 325.0);
    QPointF ptTLSmallPlusSign(260.0, bYAxisTopDown ? 310.0 : 290.0);
    m_sizeSmallPlusSign = QSizeF(30.0, 30.0);
    *m_pPhysValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, m_sizeSmallPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -15.0), QPointF(0.0, 15.0));
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(15.0, bYAxisTopDown ? 0.0 : 30.0), QPointF(15.0, bYAxisTopDown ? 30.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 15.0), QPointF(30.0, 15.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    createTestGroupAddStandardShapesLinesBigPlusSign(i_pTestStepGroupParent);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameBigPlusSign, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group
    m_ptPosBigPlusSign = QPointF(300.0, 300.0);
    QPointF ptTLBigPlusSign(250.0, bYAxisTopDown ? 250.0 : 350.0);
    m_sizeBigPlusSign = QSizeF(100.0, 100.0);
    *m_pPhysValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, m_sizeBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0), QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    *m_pPhysValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesGroupPlusSignsModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(+
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesGroupPlusSignsModifications",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjTypeGroup = graphObjType2Str(EGraphObjTypeGroup);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();
    QString strUnit = unit.symbol();
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
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
    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;
    QStringList strlstGraphObjsKeyInTreeGetResultValues;

    ZS::Test::CTestStepGroup* pGrpModifyPlusSigns = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify PlusSigns",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Ungroup SmallPlusSign
    //----------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingScene.ungroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.remove(c_strGraphObjNameSmallPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSign));
    pTestStep->setConfigValue("Method", "ungroup");
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSignHorizontalLine));
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Vertical Line
    QPointF ptP1SmallPlusSignVerticalLine(275.0, bYAxisTopDown ? 310.0 : 290.0);
    QPointF ptP2SmallPlusSignVerticalLine(275.0, bYAxisTopDown ? 340.0 : 260.0);
    m_ptPosSmallPlusSignVerticalLine = QPointF(275.0, 325.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -15.0), QPointF(0.0, 15.0));
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, ptP1SmallPlusSignVerticalLine, ptP2SmallPlusSignVerticalLine);
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // Horizontal Line
    QPointF ptP1SmallPlusSignHorizontalLine(260.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptP2SmallPlusSignHorizontalLine(290.0, bYAxisTopDown ? 325.0 : 275.0);
    m_ptPosSmallPlusSignHorizontalLine = QPointF(275.0, 325.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, ptP1SmallPlusSignHorizontalLine, ptP2SmallPlusSignHorizontalLine);
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Ungroup BigPlusSign
    //----------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingScene.ungroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.remove(c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("Method", "ungroup");
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSignHorizontalLine));
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Vertical Line
    QPointF ptP1BigPlusSignVerticalLine(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptP2BigPlusSignVerticalLine(300.0, bYAxisTopDown ? 350.0 : 250.0);
    m_ptPosBigPlusSignVerticalLine = QPointF(300.0, 300.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, ptP1BigPlusSignVerticalLine, ptP2BigPlusSignVerticalLine);
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // Horizontal Line
    QPointF ptP1BigPlusSignHorizontalLine(250.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptP2BigPlusSignHorizontalLine(350.0, bYAxisTopDown ? 300.0 : 300.0);
    m_ptPosBigPlusSignHorizontalLine = QPointF(300.0, 300.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    *m_pPhysValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, ptP1BigPlusSignHorizontalLine, ptP2BigPlusSignHorizontalLine);
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Create TopGroup
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup] = c_strGraphObjNameTopGroup;
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSignHorizontalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group PlusSigns
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_sizeTopGroup = QSizeF(100.0, 100.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(m_ptPosTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(-25.0, 25.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -15.0), QPointF(0.0, 15.0));
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(25.0, bYAxisTopDown ? 60.0 : 40.0), QPointF(25.0, bYAxisTopDown ? 90.0 : 10.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(-25.0, 25.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(10.0, bYAxisTopDown ? 75.0 : 25.0), QPointF(40.0, bYAxisTopDown ? 75.0 : 25.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0), QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    *m_pPhysValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize PlusSigns
    //-----------------

    m_sizeTopGroup = QSizeF(400.0, 200.0);
    QPointF ptBRPlusSigns(650.0, bYAxisTopDown ? 450.0 : 150.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBRPlusSigns);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    // Group PlusSigns
    m_ptPosTopGroup = QPointF(450.0, 350.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(bYAxisTopDown ? m_ptPosTopGroup : QPointF(450.0, 250.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(-100.0, 50.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -30.0), QPointF(0.0, 30.0));
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(100.0, bYAxisTopDown ? 120.0 : 80), QPointF(100.0, bYAxisTopDown ? 180.0 : 20.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(-100.0, 50.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-60.0, 0.0), QPointF(60.0, 0.0));
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(40.0, bYAxisTopDown ? 150.0 : 50.0), QPointF(160.0, bYAxisTopDown ? 150.0 : 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -100.0), QPointF(0.0, 100.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(200.0, bYAxisTopDown ? 0.0 : 200.0), QPointF(200.0, bYAxisTopDown ? 200.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-200.0, 0.0), QPointF(200.0, 0.0));
    *m_pPhysValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 100.0), QPointF(400.0, 100.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Ungroup PlusSigns
    //------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.remove(c_strGraphObjNameTopGroup);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSignHorizontalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "ungroup");
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSignHorizontalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSignHorizontalLine));
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(350.0, 400.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -30.0), QPointF(0.0, 30.0));
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(350.0, bYAxisTopDown ? 370.0 : 230.0), QPointF(350.0, bYAxisTopDown ? 430.0 : 170.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(350.0, 400.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-60.0, 0.0), QPointF(60.0, 0.0));
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(290.0, bYAxisTopDown ? 400.0 : 200.0), QPointF(410.0, bYAxisTopDown ? 400.0 : 200.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(450.0, 350.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -100.0), QPointF(0.0, 100.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(450.0, bYAxisTopDown ? 250.0 : 350.0), QPointF(450.0, bYAxisTopDown ? 450.0 : 150.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(450.0, 350.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-200.0, 0.0), QPointF(200.0, 0.0));
    *m_pPhysValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(250.0, bYAxisTopDown ? 350.0 : 250.0), QPointF(650.0, bYAxisTopDown ? 350.0 : 250.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Small Plus Sign
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameSmallPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameSmallPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(350.0, 400.0);
    QPointF ptTLSmallPlusSign(290.0, bYAxisTopDown ? 370.0 : 230.0);
    m_sizeSmallPlusSign = QSizeF(120.0, 60.0);
    *m_pPhysValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, m_sizeSmallPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -30.0), QPointF(0.0, 30.0));
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(60.0, bYAxisTopDown ? 0.0 : 60.0), QPointF(60.0, bYAxisTopDown ? 60.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-60.0, 0.0), QPointF(60.0, 0.0));
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 30.0), QPointF(120.0, 30.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Big Plus Sign
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group BigPlusSign
    m_ptPosBigPlusSign = QPointF(450.0, 350.0);
    QPointF ptTLBigPlusSign(250.0, bYAxisTopDown ? 250.0 : 350.0);
    m_sizeBigPlusSign = QSizeF(400.0, 200.0);
    *m_pPhysValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, m_sizeBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -100.0), QPointF(0.0, 100.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(200.0, bYAxisTopDown ? 0.0 : 200.0), QPointF(200.0, bYAxisTopDown ? 200.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-200.0, 0.0), QPointF(200.0, 0.0));
    *m_pPhysValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 100.0), QPointF(400.0, 100.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Group Plus Signs
    //-----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSign);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine));
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group PlusSigns
    m_ptPosTopGroup = QPointF(450.0, 350.0);
    m_sizeTopGroup = QSizeF(400.0, 200.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(bYAxisTopDown ? m_ptPosTopGroup : QPointF(450.0, 250.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(-100.0, 50.0);
    ptTLSmallPlusSign = QPointF(40.0, bYAxisTopDown ? 120.0 : 80.0);
    m_sizeSmallPlusSign = QSizeF(120.0, 60.0);
    *m_pPhysValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, m_sizeSmallPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -30.0), QPointF(0.0, 30.0));
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(60.0, bYAxisTopDown ? 0.0 : 60.0), QPointF(60.0, bYAxisTopDown ? 60.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-60.0, 0.0), QPointF(60.0, 0.0));
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 30.0), QPointF(120.0, 30.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    ptTLBigPlusSign = QPointF(0.0, bYAxisTopDown ? 0.0 : 200.0);
    m_sizeBigPlusSign = QSizeF(400.0, 200.0);
    *m_pPhysValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, m_sizeBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -100.0), QPointF(0.0, 100.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(200.0, bYAxisTopDown ? 0.0 : 200.0), QPointF(200.0, bYAxisTopDown ? 200.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-200.0, 0.0), QPointF(200.0, 0.0));
    *m_pPhysValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 100.0), QPointF(400.0, 100.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize PlusSigns
    //-----------------

    m_sizeTopGroup = QSizeF(100.0, 100.0);
    ptBRPlusSigns = QPointF(350.0, bYAxisTopDown ? 350.0 : 250.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBRPlusSigns);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine));
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group PlusSigns
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(m_ptPosTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(-25.0, 25.0);
    ptTLSmallPlusSign = QPointF(10.0, bYAxisTopDown ? 60.0 : 40.0);
    m_sizeSmallPlusSign = QSizeF(30.0, 30.0);
    *m_pPhysValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, m_sizeSmallPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -15.0), QPointF(0.0, 15.0));
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(15.0, bYAxisTopDown ? 0.0 : 30.0), QPointF(15.0, bYAxisTopDown ? 30.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 15.0), QPointF(30.0, 15.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    ptTLBigPlusSign = QPointF(0.0, bYAxisTopDown ? 0.0 : 100.0);
    m_sizeBigPlusSign = QSizeF(100.0, 100.0);
    *m_pPhysValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, m_sizeBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0), QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    *m_pPhysValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize PlusSigns
    //-----------------

    m_sizeTopGroup = QSizeF(400.0, 200.0);
    ptBRPlusSigns = QPointF(650.0, bYAxisTopDown ? 450.0 : 150.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBRPlusSigns);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    // Group PlusSigns
    m_ptPosTopGroup = QPointF(450.0, 350.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(bYAxisTopDown ? m_ptPosTopGroup : QPointF(450.0, 250.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(-100.0, 50.0);
    ptTLSmallPlusSign = QPointF(40.0, bYAxisTopDown ? 120.0 : 80.0);
    m_sizeSmallPlusSign = QSizeF(120.0, 60.0);
    *m_pPhysValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, m_sizeSmallPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    ptTLBigPlusSign = QPointF(0.0, bYAxisTopDown ? 0.0 : 200.0);
    m_sizeBigPlusSign = QSizeF(400.0, 200.0);
    *m_pPhysValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, m_sizeBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize PlusSigns
    //-----------------

    m_sizeTopGroup = QSizeF(100.0, 100.0);
    ptBRPlusSigns = QPointF(350.0, bYAxisTopDown ? 350.0 : 250.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBRPlusSigns);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    // Group PlusSigns
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(m_ptPosTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(-25.0, 25.0);
    m_sizeSmallPlusSign = QSizeF(30.0, 30.0);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    m_pPhysValRectSmallPlusSign->setCenter(QPointF(25.0, bYAxisTopDown ? 75.0 : 25.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = QSizeF(100.0, 100.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(50.0, 50.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Ungroup PlusSigns
    //------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.remove(c_strGraphObjNameTopGroup);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "ungroup");
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSign));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(275.0, 325.0);
    m_sizeSmallPlusSign = QSizeF(30.0, 30.0);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    m_pPhysValRectSmallPlusSign->setCenter(QPointF(275.0, bYAxisTopDown ? 325.0 : 275.0));
    m_pPhysValRectSmallPlusSign->setAngle(m_physValAngleSmallPlusSign + m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(300.0, 300.0);
    m_sizeBigPlusSign = QSizeF(100.0, 100.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(300.0, 300.0));
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign + m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate BigPlusSign
    //-------------------

    m_physValAngleBigPlusSign.setVal(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", m_physValAngleBigPlusSign.toString());
    strlstExpectedValues.clear();
    // BigPlusSign
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Group Plus Signs
    //-----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSign);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group PlusSigns
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_sizeTopGroup = QSizeF(Math::sqrt(2.0) * 100.0, Math::sqrt(2.0) * 100.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(m_ptPosTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(-25.0, 25.0);
    m_sizeSmallPlusSign = QSizeF(30.0, 30.0);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    m_pPhysValRectSmallPlusSign->setCenter(QPointF(45.71, bYAxisTopDown ? 95.71 : 45.71));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = QSizeF(100.0, 100.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(Math::sqrt(2.0) * 100.0/2.0, Math::sqrt(2.0) * 100.0/2.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize PlusSigns
    //-----------------

    m_sizeTopGroup.setWidth(200.0);
    m_pPhysValRectTopGroup->setWidth(200.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setWidth(" + m_pPhysValRectTopGroup->width().toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setWidth(" + m_pPhysValRectTopGroup->width().toString() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "setWidth");
    pTestStep->setConfigValue("Width", m_pPhysValRectTopGroup->width().toString());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group PlusSigns
    m_ptPosTopGroup = QPointF(329.29, 300.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(m_ptPosTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(-35.4, 25.0);
    m_sizeSmallPlusSign = QSizeF(42.43, 30.0);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    m_pPhysValRectSmallPlusSign->setCenter(QPointF(64.64, bYAxisTopDown ? 95.71 : 45.71));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -15.0), QPointF(0.0, 15.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP1(QPointF(21.21, bYAxisTopDown ? 0.0 : 30.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP2(QPointF(21.21, bYAxisTopDown ? 30.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-21.21, 0.0), QPointF(21.21, 0.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP1(QPointF(0.0, 15.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP2(QPointF(42.426, 15.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = QSizeF(Math::sqrt(2.0) * 100.0, 100.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(100.0, Math::sqrt(2.0)/2.0 * 100.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(100.0 * Math::sqrt(2.0)/2.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(100.0 * Math::sqrt(2.0)/2.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-100.0 * Math::sqrt(2.0)/2.0, 0.0), QPointF(100.0 * Math::sqrt(2.0)/2.0, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(100.0 * Math::sqrt(2.0), 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Ungroup PlusSigns
    //------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.remove(c_strGraphObjNameTopGroup);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "ungroup");
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(293.93, 325.0);
    m_sizeSmallPlusSign = QSizeF(42.43, 30.0);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    m_pPhysValRectSmallPlusSign->setCenter(bYAxisTopDown ? m_ptPosSmallPlusSign : QPointF(293.93, 275.0));
    m_pPhysValRectSmallPlusSign->setAngle(m_physValAngleSmallPlusSign + m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(329.29, 300.0);
    m_sizeBigPlusSign = QSizeF(Math::sqrt(2.0) * 100.0, 100.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(m_ptPosBigPlusSign);
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign + m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Group Plus Signs
    //-----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSign);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group PlusSigns
    m_ptPosTopGroup = QPointF(329.29, 300.0);
    m_sizeTopGroup = QSizeF(170.71, 170.71);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(m_ptPosTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(-35.4, 25.0);
    m_sizeSmallPlusSign = QSizeF(42.43, 30.0);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    m_pPhysValRectSmallPlusSign->setCenter(QPointF(50.0, bYAxisTopDown ? 110.36 : 60.36));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = QSizeF(Math::sqrt(2.0) * 100.0, 100.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(85.36, 85.36));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize PlusSigns
    //-----------------

    ptBRPlusSigns = QPointF(350.0, bYAxisTopDown ? 350.0 : 250.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBRPlusSigns);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group PlusSigns
    m_ptPosTopGroup = QPointF(296.97, 282.32);
    m_sizeTopGroup = QSizeF(106.07, 135.36);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(bYAxisTopDown ? m_ptPosTopGroup : QPointF(296.97, 317.68));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(-21.97, 19.82);
    m_sizeSmallPlusSign = QSizeF(26.36, 23.787);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    m_pPhysValRectSmallPlusSign->setCenter(QPointF(31.066, bYAxisTopDown ? 87.5 : 47.86));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -11.9), QPointF(0.0, 11.9));
    m_pPhysValLineSmallPlusSignVerticalLine->setP1(QPointF(13.18, bYAxisTopDown ? 0.0 : 23.786));
    m_pPhysValLineSmallPlusSignVerticalLine->setP2(QPointF(13.18, bYAxisTopDown ? 23.786 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-13.2, 0.0), QPointF(13.2, 0.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP1(QPointF(0.0, 11.89));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP2(QPointF(26.36, 11.89));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = QSizeF(87.868, 79.289);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(53.033, 67.678));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -39.64), QPointF(0.0, 39.64));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(43.93, bYAxisTopDown ? 0.0 : 79.287));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(43.93, bYAxisTopDown ? 79.287 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-43.94, 0.0), QPointF(43.94, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, 39.64));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(87.867, 39.64));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Ungroup PlusSigns
    //------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.remove(c_strGraphObjNameTopGroup);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "ungroup");
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(275.0, 302.145);
    m_sizeSmallPlusSign = QSizeF(26.360, 23.787);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    m_pPhysValRectSmallPlusSign->setCenter(QPointF(275.0, bYAxisTopDown ? 302.144 : 297.86));
    m_pPhysValRectSmallPlusSign->setAngle(m_physValAngleSmallPlusSign + m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(296.967, 282.322);
    m_sizeBigPlusSign = QSizeF(87.868, 79.289);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(296.967, bYAxisTopDown ? 282.322 : 317.68));
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign + m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".showLabel(Name)",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".showLabel(" + CGraphObj::c_strLabelName + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameBigPlusSign},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign)},
        {"LabelName", CGraphObj::c_strLabelName},
        {"setPos", QPointF(320.0, 220.0)},
        {"ExpectedText", c_strGraphObjNameBigPlusSign}
    });

    // Show Geometry Labels
    //---------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".showGeometryLabels(TopLeft, Angle)",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".showGeometryLabels(TopLeft, Angle)",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepShowGeometryLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameBigPlusSign},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign)},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"setPos", QPointF(225.0, 235.0)},
        {"ExpectedText", m_pPhysValRectBigPlusSign->topLeft().toString()},
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameBigPlusSign},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign)},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"setPos", QPointF(350.0, bYAxisTopDown ? 270.0 : 330.0)},
        {"ExpectedText", m_physValAngleBigPlusSign.toString()},
    });

    // Rotate BigPlusSign
    //-------------------

    m_physValAngleBigPlusSign.setVal(60.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", m_physValAngleBigPlusSign.toString());
    strlstExpectedValues.clear();
    // Group
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_lineBigPlusSignVerticalLine.setP1(QPointF(0.0, -39.645));
    m_lineBigPlusSignVerticalLine.setP2(QPointF(0.0, 39.645));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(43.934, bYAxisTopDown ? 0.0 : 79.289));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(43.934, bYAxisTopDown ? 79.289 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_lineBigPlusSignHorizontalLine.setP1(QPointF(-43.934, 0.0));
    m_lineBigPlusSignHorizontalLine.setP2(QPointF(43.934, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, 39.644));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(87.868, 39.644));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Geometry Labels
    //---------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".showGeometryLabels(TopLeft, Angle)",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".showGeometryLabels(TopLeft, Angle)",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepShowGeometryLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameBigPlusSign},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign)},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"ExpectedText", m_physValAngleBigPlusSign.toString()},
    });

    // Rotate BigPlusSign
    //-------------------

    m_physValAngleBigPlusSign.setVal(135.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", m_physValAngleBigPlusSign.toString());
    strlstExpectedValues.clear();
    // Group
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesGroupCheckmark(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesGroupCheckmark",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjTypeGroup = graphObjType2Str(EGraphObjTypeGroup);
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
    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;

    createTestGroupAddStandardShapesLinesCheckmark(i_pTestStepGroupParent);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameCheckmark + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameCheckmark + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameCheckmark, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameCheckmark));
    m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmarkLeftLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameCheckmark, c_strGraphObjNameCheckmarkLeftLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmarkRightLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameCheckmark, c_strGraphObjNameCheckmarkRightLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameCheckmarkLeftLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameCheckmarkRightLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmark);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group
    m_ptPosCheckmark = QPointF(300.0, 300.0);
    m_sizeCheckmark = QSizeF(100.0, 100.0);
    m_pPhysValRectCheckmark->setSize(m_sizeCheckmark);
    m_pPhysValRectCheckmark->setCenter(QPointF(300.0, bYAxisTopDown ? 300.0 : 300.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // Checkmark-LeftLine
    m_ptPosCheckmarkLeftLine = QPointF(-12.5, 0.0);
    m_lineCheckmarkLeftLine = QLineF(QPointF(-37.5, -50.0), QPointF(37.5, 50.0));
    m_pPhysValLineCheckmarkLeftLine->setP1(QPointF(0.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineCheckmarkLeftLine->setP2(QPointF(75.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkLeftLine, m_ptPosCheckmarkLeftLine,
        m_lineCheckmarkLeftLine, *m_pPhysValLineCheckmarkLeftLine));
    // Checkmark-RightLine
    m_ptPosCheckmarkRightLine = QPointF(37.5, 37.5);
    m_lineCheckmarkRightLine = QLineF(QPointF(-12.5, 12.5), QPointF(12.5, -12.5));
    m_pPhysValLineCheckmarkRightLine->setP1(QPointF(75.0, bYAxisTopDown ? 100.0 : 0.0));
    m_pPhysValLineCheckmarkRightLine->setP2(QPointF(100.0, bYAxisTopDown ? 75.0 : 25.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkRightLine, m_ptPosCheckmarkRightLine,
        m_lineCheckmarkRightLine, *m_pPhysValLineCheckmarkRightLine));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesGroupCheckmarkModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesGroupCheckmarkModifications",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjTypeGroup = graphObjType2Str(EGraphObjTypeGroup);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();
    QString strUnit = unit.symbol();
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
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
    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;
    QStringList strlstGraphObjsKeyInTreeGetResultValues;

    ZS::Test::CTestStepGroup* pGrpModifyCheckmark = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify " + c_strGraphObjNameCheckmark,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Create TopGroup
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyCheckmark,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameTopGroup, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmark] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameCheckmark);
    m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmarkLeftLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameCheckmark, c_strGraphObjNameCheckmarkLeftLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmarkRightLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameCheckmark, c_strGraphObjNameCheckmarkRightLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameCheckmark);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmark]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmarkLeftLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmarkRightLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // TopGroup
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_sizeTopGroup = QSizeF(100.0, 100.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(QPointF(300.0, bYAxisTopDown ? 300.0 : 300.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // Checkmark
    m_ptPosCheckmark = QPointF(0.0, 0.0);
    m_pPhysValRectCheckmark->setSize(m_sizeCheckmark);
    m_pPhysValRectCheckmark->setCenter(QPointF(50.0, bYAxisTopDown ? 50.0 : 50.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // Checkmark-LeftLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkLeftLine, m_ptPosCheckmarkLeftLine,
        m_lineCheckmarkLeftLine, *m_pPhysValLineCheckmarkLeftLine));
    // Checkmark-RightLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkRightLine, m_ptPosCheckmarkRightLine,
        m_lineCheckmarkRightLine, *m_pPhysValLineCheckmarkRightLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Left Line
    //----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameCheckmarkLeftLine + ".setLine()",
        /* strOperation    */ c_strGraphObjNameCheckmarkLeftLine + ".setLine()",
        /* pGrpParent      */ pGrpModifyCheckmark,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjLineByDirectMethodCalls(ZS::Test::CTestStep*)) );
    m_ptPosCheckmarkLeftLine = QPointF(-7.5, 0.0);
    m_lineCheckmarkLeftLine = QLineF(QPointF(-17.5, -25.0), QPointF(17.5, 25.0));
    m_pPhysValLineCheckmarkLeftLine->setP1(QPointF(25.0, bYAxisTopDown ? 25.0 : 75.0));
    m_pPhysValLineCheckmarkLeftLine->setP2(QPointF(60.0, bYAxisTopDown ? 75.0 : 25.0));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmarkLeftLine);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmarkLeftLine]);
    pTestStep->setConfigValue("Method", "setLine");
    pTestStep->setConfigValue("P1", m_pPhysValLineCheckmarkLeftLine->p1().toQPointF());
    pTestStep->setConfigValue("P2", m_pPhysValLineCheckmarkLeftLine->p2().toQPointF());
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkLeftLine, m_ptPosCheckmarkLeftLine,
        m_lineCheckmarkLeftLine, *m_pPhysValLineCheckmarkLeftLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Right Line
    //-----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameCheckmarkRightLine + ".setLine()",
        /* strOperation    */ c_strGraphObjNameCheckmarkRightLine + ".setLine()",
        /* pGrpParent      */ pGrpModifyCheckmark,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjLineByDirectMethodCalls(ZS::Test::CTestStep*)) );
    m_ptPosCheckmarkRightLine = QPointF(17.5, 17.5);
    m_lineCheckmarkRightLine = QLineF(QPointF(-7.5, 7.5), QPointF(7.5, -7.5));
    m_pPhysValLineCheckmarkRightLine->setP1(QPointF(60.0, bYAxisTopDown ? 75.0 : 25.0));
    m_pPhysValLineCheckmarkRightLine->setP2(QPointF(75.0, bYAxisTopDown ? 60.0 : 40.0));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmarkRightLine);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmarkRightLine]);
    pTestStep->setConfigValue("Method", "setLine");
    pTestStep->setConfigValue("P1", m_pPhysValLineCheckmarkRightLine->p1().toQPointF());
    pTestStep->setConfigValue("P2", m_pPhysValLineCheckmarkRightLine->p2().toQPointF());
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkRightLine, m_ptPosCheckmarkRightLine,
        m_lineCheckmarkRightLine, *m_pPhysValLineCheckmarkRightLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Checkmark - resizeToContent
    //----------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameCheckmark + ".resizeToContent()",
        /* strOperation    */ c_strGraphObjNameCheckmark + ".resizeToContent()",
        /* pGrpParent      */ pGrpModifyCheckmark,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameCheckmark);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmark]);
    pTestStep->setConfigValue("Method", "resizeToContent");
    strlstExpectedValues.clear();
    // Group
    m_sizeCheckmark = QSizeF(50.0, 50.0);
    m_pPhysValRectCheckmark->setSize(m_sizeCheckmark);
    m_pPhysValRectCheckmark->setCenter(QPointF(50.0, bYAxisTopDown ? 50.0 : 50.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // Checkmark-LeftLine
    m_pPhysValLineCheckmarkLeftLine->setP1(QPointF(0.0, bYAxisTopDown ? 0.0 : 50.0));
    m_pPhysValLineCheckmarkLeftLine->setP2(QPointF(35.0, bYAxisTopDown ? 50.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkLeftLine, m_ptPosCheckmarkLeftLine,
        m_lineCheckmarkLeftLine, *m_pPhysValLineCheckmarkLeftLine));
    // Checkmark-RightLine
    m_pPhysValLineCheckmarkRightLine->setP1(QPointF(35.0, bYAxisTopDown ? 50.0 : 0.0));
    m_pPhysValLineCheckmarkRightLine->setP2(QPointF(50.0, bYAxisTopDown ? 35.0 : 15.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkRightLine, m_ptPosCheckmarkRightLine,
        m_lineCheckmarkRightLine, *m_pPhysValLineCheckmarkRightLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize top group
    //-----------------

    QPoint ptBRTopGroup(450.0, bYAxisTopDown ? 450.0 : 150.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRTopGroup) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRTopGroup) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyCheckmark,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBRTopGroup);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmark]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmarkLeftLine]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmarkRightLine]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Group Top
    m_ptPosTopGroup = QPointF(350.0, 350.0);
    m_sizeTopGroup = QSizeF(200.0, 200.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(QPointF(350.0, bYAxisTopDown ? 350.0 : 250.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // Checkmark
    m_ptPosCheckmark = QPointF(0.0, 0.0);
    m_sizeCheckmark = QSizeF(100.0, 100.0);
    m_pPhysValRectCheckmark->setSize(m_sizeCheckmark);
    m_pPhysValRectCheckmark->setCenter(QPointF(100.0, 100.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // Checkmark-LeftLine
    m_ptPosCheckmarkLeftLine = QPointF(-15.0, 0.0);
    m_lineCheckmarkLeftLine = QLineF(QPointF(-35.0, -50.0), QPointF(35.0, 50.0));
    m_pPhysValLineCheckmarkLeftLine->setP1(QPointF(0.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineCheckmarkLeftLine->setP2(QPointF(70.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkLeftLine, m_ptPosCheckmarkLeftLine,
        m_lineCheckmarkLeftLine, *m_pPhysValLineCheckmarkLeftLine));
    // Checkmark-RightLine
    m_ptPosCheckmarkRightLine = QPointF(35.0, 35.0);
    m_lineCheckmarkRightLine = QLineF(QPointF(-15.0, 15.0), QPointF(15.0, -15.0));
    m_pPhysValLineCheckmarkRightLine->setP1(QPointF(70.0, bYAxisTopDown ? 100.0 : 0.0));
    m_pPhysValLineCheckmarkRightLine->setP2(QPointF(100.0, bYAxisTopDown ? 70.0 : 30.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkRightLine, m_ptPosCheckmarkRightLine,
        m_lineCheckmarkRightLine, *m_pPhysValLineCheckmarkRightLine));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesGroupPolygons(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesGroupPolygons",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjTypeGroup = graphObjType2Str(EGraphObjTypeGroup);
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
    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;

    createTestGroupAddStandardShapesPolygonStar(i_pTestStepGroupParent);
    createTestGroupAddStandardShapesPolygonTriangle(i_pTestStepGroupParent);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNamePolygons + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNamePolygons + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNamePolygons, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNamePolygons));
    m_hshGraphObjNameToKeys[c_strGraphObjNameStar] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNamePolygons, c_strGraphObjNameStar);
    m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNamePolygons, c_strGraphObjNameTriangle);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameStar);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameTriangle);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNamePolygons);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group
    m_ptPosPolygons = QPointF(300.0, 300.0);
    m_sizePolygons = QSizeF(200.0, 200.0);
    m_pPhysValRectPolygons->setSize(m_sizePolygons);
    m_pPhysValRectPolygons->setCenter(QPointF(300.0, bYAxisTopDown ? 300.0 : 300.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNamePolygons, m_ptPosPolygons, *m_pPhysValRectPolygons));
    // Star
    m_ptPosPolygonStar = QPointF(0.0, 0.0);
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
        {100.0, bYAxisTopDown ?   0.0 : m_sizePolygons.height() -   0.0},
        {125.0, bYAxisTopDown ?  75.0 : m_sizePolygons.height() -  75.0},
        {200.0, bYAxisTopDown ? 100.0 : m_sizePolygons.height() - 100.0},
        {125.0, bYAxisTopDown ? 125.0 : m_sizePolygons.height() - 125.0},
        {100.0, bYAxisTopDown ? 200.0 : m_sizePolygons.height() - 200.0},
        { 75.0, bYAxisTopDown ? 125.0 : m_sizePolygons.height() - 125.0},
        {  0.0, bYAxisTopDown ? 100.0 : m_sizePolygons.height() - 100.0},
        { 75.0, bYAxisTopDown ?  75.0 : m_sizePolygons.height() -  75.0}
    });
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameStar, m_ptPosPolygonStar, m_polygonStar, *m_pPhysValPolygonStar));
    // Triangle
    m_ptPosPolygonTriangle = QPointF(0.0, -12.5);
    m_polygonTriangle = QPolygonF({
        {-25.0,  12.5},
        { 25.0,  12.5},
        {  0.0, -12.5}
    });
    *m_pPhysValPolygonTriangle = QPolygonF({
        { 75.0, bYAxisTopDown ? 100.0 : m_sizePolygons.height() - 100.0},
        {125.0, bYAxisTopDown ? 100.0 : m_sizePolygons.height() - 100.0},
        {100.0, bYAxisTopDown ?  75.0 : m_sizePolygons.height() -  75.0}
    });
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameTriangle, m_ptPosPolygonTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesGroupPolygonsModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesGroupPolygonsModifications",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjTypeGroup = graphObjType2Str(EGraphObjTypeGroup);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();
    QString strUnit = unit.symbol();
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    double fYAxisMaxVal = 600.0;

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;
    QStringList strlstGraphObjsKeyInTreeGetResultValues;

    ZS::Test::CTestStepGroup* pGrpModifyCheckmark = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify " + c_strGraphObjNamePolygons,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Create TopGroup
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyCheckmark,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameTopGroup, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    m_hshGraphObjNameToKeys[c_strGraphObjNamePolygons] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNamePolygons);
    m_hshGraphObjNameToKeys[c_strGraphObjNameStar] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNamePolygons, c_strGraphObjNameStar);
    m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNamePolygons, c_strGraphObjNameTriangle);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNamePolygons);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNamePolygons]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameStar]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    pTestStep->setConfigValue("ResultValuesPrecision", iDigits);
    strlstExpectedValues.clear();
    // TopGroup
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_sizeTopGroup = QSizeF(200.0, 200.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(QPointF(300.0, bYAxisTopDown ? 300.0 : fYAxisMaxVal - 300.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, iDigits));
    // Polygons
    m_ptPosPolygons = QPointF(0.0, 0.0);
    m_sizePolygons = m_sizeTopGroup;
    m_pPhysValRectPolygons->setSize(m_sizePolygons);
    m_pPhysValRectPolygons->setCenter(QPointF(100.0, bYAxisTopDown ? 100.0 : m_sizeTopGroup.height() - 100.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNamePolygons, m_ptPosPolygons, *m_pPhysValRectPolygons, iDigits));
    // Star
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameStar, m_ptPosPolygonStar, m_polygonStar, *m_pPhysValPolygonStar, iDigits));
    // Triangle
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameTriangle, m_ptPosPolygonTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, iDigits));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize top group
    //-----------------

    QPoint ptBRTopGroup(500.0, bYAxisTopDown ? 500.0 : fYAxisMaxVal - 500.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRTopGroup) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRTopGroup) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyCheckmark,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    iDigits = 6;
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBRTopGroup);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTopGroup]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNamePolygons]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameStar]);
    strlstGraphObjsKeyInTreeGetResultValues.append(m_hshGraphObjNameToKeys[c_strGraphObjNameTriangle]);
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    pTestStep->setConfigValue("ResultValuesPrecision", iDigits);
    strlstExpectedValues.clear();
    // TopGroup
    m_ptPosTopGroup = QPointF(350.0, 350.0);
    m_sizeTopGroup = QSizeF(300.0, 300.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(QPointF(350.0, bYAxisTopDown ? 350.0 : fYAxisMaxVal - 350.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup, iDigits));
    // Polygons
    m_ptPosPolygons = QPointF(0.0, 0.0);
    m_sizePolygons = m_sizeTopGroup;
    m_pPhysValRectPolygons->setSize(m_sizePolygons);
    m_pPhysValRectPolygons->setCenter(QPointF(150.0, bYAxisTopDown ? 150.0 : m_sizeTopGroup.height() - 150.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNamePolygons, m_ptPosPolygons, *m_pPhysValRectPolygons, iDigits));
    // Star
    m_ptPosPolygonStar = QPointF(0.0, 0.0);
    m_polygonStar = QPolygonF({
        {   0.0, -150.0},
        {  37.5,  -37.5},
        { 150.0,    0.0},
        {  37.5,   37.5},
        {   0.0,  150.0},
        { -37.5,   37.5},
        {-150.0,    0.0},
        { -37.5,  -37.5}
    });
    *m_pPhysValPolygonStar = QPolygonF({
        {150.0, bYAxisTopDown ?   0.0 : m_sizePolygons.height() -   0.0},
        {187.5, bYAxisTopDown ? 112.5 : m_sizePolygons.height() - 112.5},
        {300.0, bYAxisTopDown ? 150.0 : m_sizePolygons.height() - 150.0},
        {187.5, bYAxisTopDown ? 187.5 : m_sizePolygons.height() - 187.5},
        {150.0, bYAxisTopDown ? 300.0 : m_sizePolygons.height() - 300.0},
        {112.5, bYAxisTopDown ? 187.5 : m_sizePolygons.height() - 187.5},
        {  0.0, bYAxisTopDown ? 150.0 : m_sizePolygons.height() - 150.0},
        {112.5, bYAxisTopDown ? 112.5 : m_sizePolygons.height() - 112.5}
    });
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameStar, m_ptPosPolygonStar, m_polygonStar, *m_pPhysValPolygonStar, iDigits));
    // Triangle
    m_ptPosPolygonTriangle = QPointF(0.0, -18.75);
    m_polygonTriangle = QPolygonF({
        {-37.5,  18.75},
        { 37.5,  18.75},
        {  0.0, -18.75}
    });
    *m_pPhysValPolygonTriangle = QPolygonF({
        {112.5, bYAxisTopDown ? 150.0 : m_sizePolygons.height() - 150.0},
        {187.5, bYAxisTopDown ? 150.0 : m_sizePolygons.height() - 150.0},
        {150.0, bYAxisTopDown ? 112.5 : m_sizePolygons.height() - 112.5}
    });
    strlstExpectedValues.append(resultValuesForPolygon(
        c_strGraphObjNameTriangle, m_ptPosPolygonTriangle, m_polygonTriangle, *m_pPhysValPolygonTriangle, iDigits));
    pTestStep->setExpectedValues(strlstExpectedValues);
    pTestStep->setExpectedValues(strlstExpectedValues);
    iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesAllGroups(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesAllGroups",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjTypeGroup = graphObjType2Str(EGraphObjTypeGroup);
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
    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;

    // SmallPlusSign
    //--------------

    createTestGroupAddStandardShapesLinesSmallPlusSign(i_pTestStepGroupParent);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameSmallPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameSmallPlusSign + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallPlusSign, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSign));
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group
    m_ptPosSmallPlusSign = QPointF(275.0, 325.0);
    m_sizeSmallPlusSign = QSizeF(30.0, 30.0);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    m_pPhysValRectSmallPlusSign->setCenter(QPointF(275.0, bYAxisTopDown ? 325.0 : 275.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_pPhysValLineSmallPlusSignVerticalLine->setP1(QPointF(15.0, bYAxisTopDown ? 0.0 : 30.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP2(QPointF(15.0, bYAxisTopDown ? 30.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP1(QPointF(0.0, 15.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP2(QPointF(30.0, 15.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // BigPlusSign
    //------------

    createTestGroupAddStandardShapesLinesBigPlusSign(i_pTestStepGroupParent);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameBigPlusSign, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group
    m_ptPosBigPlusSign = QPointF(300.0, 300.0);
    m_sizeBigPlusSign = QSizeF(100.0, 100.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(300.0, bYAxisTopDown ? 300.0 : 300.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(50.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(50.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(100.0, 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Checkmark
    //----------

    createTestGroupAddStandardShapesLinesCheckmark(i_pTestStepGroupParent);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameCheckmark + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameCheckmark + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameCheckmark, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameCheckmark));
    m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmarkLeftLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameCheckmark, c_strGraphObjNameCheckmarkLeftLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmarkRightLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameCheckmark, c_strGraphObjNameCheckmarkRightLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameCheckmarkLeftLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameCheckmarkRightLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmark);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group
    m_ptPosCheckmark = QPointF(300.0, 300.0);
    m_sizeCheckmark = QSizeF(100.0, 100.0);
    m_pPhysValRectCheckmark->setSize(m_sizeCheckmark);
    m_pPhysValRectCheckmark->setCenter(QPointF(300.0, bYAxisTopDown ? 300.0 : 300.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // Checkmark-LeftLine
    m_ptPosCheckmarkLeftLine = QPointF(-12.5, 0.0);
    m_lineCheckmarkLeftLine = QLineF(QPointF(-37.5, -50.0), QPointF(37.5, 50.0));
    m_pPhysValLineCheckmarkLeftLine->setP1(QPointF(0.0, bYAxisTopDown ? 0.0 : 100.0));
    m_pPhysValLineCheckmarkLeftLine->setP2(QPointF(75.0, bYAxisTopDown ? 100.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkLeftLine, m_ptPosCheckmarkLeftLine,
        m_lineCheckmarkLeftLine, *m_pPhysValLineCheckmarkLeftLine));
    // Checkmark-RightLine
    m_ptPosCheckmarkRightLine = QPointF(37.5, 37.5);
    m_lineCheckmarkRightLine = QLineF(QPointF(-12.5, 12.5), QPointF(12.5, -12.5));
    m_pPhysValLineCheckmarkRightLine->setP1(QPointF(75.0, bYAxisTopDown ? 100.0 : 0.0));
    m_pPhysValLineCheckmarkRightLine->setP2(QPointF(100.0, bYAxisTopDown ? 75.0 : 25.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkRightLine, m_ptPosCheckmarkRightLine,
        m_lineCheckmarkRightLine, *m_pPhysValLineCheckmarkRightLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // SmallRect
    //----------

    createTestGroupAddStandardShapesLinesSmallRect(i_pTestStepGroupParent);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameSmallRect + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameSmallRect + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallRect, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallRect));
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallRectTopLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallRect, c_strGraphObjNameSmallRectTopLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallRectRightLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallRect, c_strGraphObjNameSmallRectRightLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallRectBottomLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallRect, c_strGraphObjNameSmallRectBottomLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallRectLeftLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallRect, c_strGraphObjNameSmallRectLeftLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallRectTopLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallRectRightLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallRectBottomLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallRectLeftLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallRect);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group
    m_ptPosSmallRect = QPointF(325.0, 275.0);
    m_sizeSmallRect = QSizeF(10.0, 10.0);
    m_pPhysValRectSmallRect->setSize(m_sizeSmallRect);
    m_pPhysValRectSmallRect->setCenter(QPointF(325.0, bYAxisTopDown ? 275.0 : 325.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallRect, m_ptPosSmallRect, *m_pPhysValRectSmallRect));
    // SmallRect-TopLine
    m_ptPosSmallRectTopLine = QPointF(0.0, -5.0);
    m_lineSmallRectTopLine = QLineF(QPointF(-5.0, 0.0), QPointF(5.0, 0.0));
    m_pPhysValLineSmallRectTopLine->setP1(QPointF(0.0, bYAxisTopDown ? 0.0 : 10.0));
    m_pPhysValLineSmallRectTopLine->setP2(QPointF(10.0, bYAxisTopDown ? 0.0 : 10.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectTopLine, m_ptPosSmallRectTopLine,
        m_lineSmallRectTopLine, *m_pPhysValLineSmallRectTopLine));
    // SmallRect-RightLine
    m_ptPosSmallRectRightLine = QPointF(5.0, 0.0);
    m_lineSmallRectRightLine = QLineF(QPointF(0.0, -5.0), QPointF(0.0, 5.0));
    m_pPhysValLineSmallRectRightLine->setP1(QPointF(10.0, bYAxisTopDown ? 0.0 : 10.0));
    m_pPhysValLineSmallRectRightLine->setP2(QPointF(10.0, bYAxisTopDown ? 10.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectRightLine, m_ptPosSmallRectRightLine,
        m_lineSmallRectRightLine, *m_pPhysValLineSmallRectRightLine));
    // SmallRect-BottomLine
    m_ptPosSmallRectBottomLine = QPointF(0.0, 5.0);
    m_lineSmallRectBottomLine = QLineF(QPointF(5.0, 0.0), QPointF(-5.0, 0.0));
    m_pPhysValLineSmallRectBottomLine->setP1(QPointF(10.0, bYAxisTopDown ? 10.0 : 0.0));
    m_pPhysValLineSmallRectBottomLine->setP2(QPointF(0.0, bYAxisTopDown ? 10.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectBottomLine, m_ptPosSmallRectBottomLine,
        m_lineSmallRectBottomLine, *m_pPhysValLineSmallRectBottomLine));
    // SmallRect-LeftLine
    m_ptPosSmallRectLeftLine = QPointF(-5.0, 0.0);
    m_lineSmallRectLeftLine = QLineF(QPointF(0.0, 5.0), QPointF(0.0, -5.0));
    m_pPhysValLineSmallRectLeftLine->setP1(QPointF(0.0, bYAxisTopDown ? 10.0 : 0.0));
    m_pPhysValLineSmallRectLeftLine->setP2(QPointF(0.0, bYAxisTopDown ? 0.0 : 10.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectLeftLine, m_ptPosSmallRectLeftLine,
        m_lineSmallRectLeftLine, *m_pPhysValLineSmallRectLeftLine));
    pTestStep->setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddStandardShapesAllGroupsModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesAllGroupsModifications",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjTypeGroup = graphObjType2Str(EGraphObjTypeGroup);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();
    QString strUnit = unit.symbol();
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    int iDigits = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
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
    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;
    QStringList strlstGraphObjsKeyInTreeGetResultValues;

    ZS::Test::CTestStepGroup* pGrpModifyAll = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify All Groups",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Create TopGroup
    //================

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Add(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpModifyAll,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameTopGroup, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmark] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameCheckmark);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallRect] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallRect);
    m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmarkLeftLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameCheckmark, c_strGraphObjNameCheckmarkLeftLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameCheckmarkRightLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameCheckmark, c_strGraphObjNameCheckmarkRightLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallRectTopLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallRect, c_strGraphObjNameSmallRectTopLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallRectRightLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallRect, c_strGraphObjNameSmallRectRightLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallRectBottomLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallRect, c_strGraphObjNameSmallRectBottomLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallRectLeftLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallRect, c_strGraphObjNameSmallRectLeftLine);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSign);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSign);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameCheckmark);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallRect);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // TopGroup
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    m_sizeTopGroup = QSizeF(100.0, 100.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(QPointF(300.0, bYAxisTopDown ? 300.0 : 300.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(-25.0, 25.0);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    m_pPhysValRectSmallPlusSign->setCenter(QPointF(25.0, bYAxisTopDown ? 75.0 : 25.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(50.0, bYAxisTopDown ? 50.0 : 50.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // Checkmark
    m_ptPosCheckmark = QPointF(0.0, 0.0);
    m_pPhysValRectCheckmark->setSize(m_sizeCheckmark);
    m_pPhysValRectCheckmark->setCenter(QPointF(50.0, bYAxisTopDown ? 50.0 : 50.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // SmallRect
    m_ptPosSmallRect = QPointF(25.0, -25.0);
    m_pPhysValRectSmallRect->setSize(m_sizeSmallRect);
    m_pPhysValRectSmallRect->setCenter(QPointF(75.0, bYAxisTopDown ? 25.0 : 75.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallRect, m_ptPosSmallRect, *m_pPhysValRectSmallRect));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Modify Checkmark
    //=================

    ZS::Test::CTestStepGroup* pGrpModifyCheckmarkLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameCheckmark,
        /* pTSGrpParent */ pGrpModifyAll );

    // Left Line
    //----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameCheckmarkLeftLine + ".setLine()",
        /* strOperation    */ c_strGraphObjNameCheckmarkLeftLine + ".setLine()",
        /* pGrpParent      */ pGrpModifyCheckmarkLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjLineByDirectMethodCalls(ZS::Test::CTestStep*)) );
    m_ptPosCheckmarkLeftLine = QPointF(-7.5, 0.0);
    m_lineCheckmarkLeftLine = QLineF(QPointF(-17.5, -25.0), QPointF(17.5, 25.0));
    m_pPhysValLineCheckmarkLeftLine->setP1(QPointF(25.0, bYAxisTopDown ? 25.0 : 75.0));
    m_pPhysValLineCheckmarkLeftLine->setP2(QPointF(60.0, bYAxisTopDown ? 75.0 : 25.0));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmarkLeftLine);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameCheckmark, c_strGraphObjNameCheckmarkLeftLine));
    pTestStep->setConfigValue("Method", "setLine");
    pTestStep->setConfigValue("P1", m_pPhysValLineCheckmarkLeftLine->p1().toQPointF());
    pTestStep->setConfigValue("P2", m_pPhysValLineCheckmarkLeftLine->p2().toQPointF());
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkLeftLine, m_ptPosCheckmarkLeftLine,
        m_lineCheckmarkLeftLine, *m_pPhysValLineCheckmarkLeftLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Right Line
    //-----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameCheckmarkRightLine + ".setLine()",
        /* strOperation    */ c_strGraphObjNameCheckmarkRightLine + ".setLine()",
        /* pGrpParent      */ pGrpModifyCheckmarkLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjLineByDirectMethodCalls(ZS::Test::CTestStep*)) );
    QPointF ptP1CheckmarkRightLine(60.0, bYAxisTopDown ? 75.0 : 25.0);
    QPointF ptP2CheckmarkRightLine(75.0, bYAxisTopDown ? 60.0 : 40.0);
    m_ptPosCheckmarkRightLine = QPointF(17.5, 17.5);
    m_lineCheckmarkRightLine = QLineF(QPointF(-7.5, 7.5), QPointF(7.5, -7.5));
    *m_pPhysValLineCheckmarkRightLine = CPhysValLine(*m_pDrawingScene, ptP1CheckmarkRightLine, ptP2CheckmarkRightLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmarkRightLine);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameCheckmark, c_strGraphObjNameCheckmarkRightLine));
    pTestStep->setConfigValue("Method", "setLine");
    pTestStep->setConfigValue("P1", ptP1CheckmarkRightLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkRightLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkRightLine, m_ptPosCheckmarkRightLine,
        m_lineCheckmarkRightLine, *m_pPhysValLineCheckmarkRightLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Checkmark - resizeToContent
    //----------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameCheckmark + ".resizeToContent()",
        /* strOperation    */ c_strGraphObjNameCheckmark + ".resizeToContent()",
        /* pGrpParent      */ pGrpModifyCheckmarkLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameCheckmark);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameCheckmark));
    pTestStep->setConfigValue("Method", "resizeToContent");
    strlstExpectedValues.clear();
    // Group
    m_ptPosCheckmark = QPointF(0.0, 0.0);
    m_sizeCheckmark = QSizeF(50.0, 50.0);
    m_pPhysValRectCheckmark->setSize(m_sizeCheckmark);
    m_pPhysValRectCheckmark->setCenter(QPointF(50.0, bYAxisTopDown ? 50.0 : 50.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // Checkmark-LeftLine
    m_ptPosCheckmarkLeftLine = QPointF(-7.5, 0.0);
    m_lineCheckmarkLeftLine = QLineF(QPointF(-17.5, -25.0), QPointF(17.5, 25.0));
    m_pPhysValLineCheckmarkLeftLine->setP1(QPointF(0.0, bYAxisTopDown ? 0.0 : 50.0));
    m_pPhysValLineCheckmarkLeftLine->setP2(QPointF(35.0, bYAxisTopDown ? 50.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkLeftLine, m_ptPosCheckmarkLeftLine,
        m_lineCheckmarkLeftLine, *m_pPhysValLineCheckmarkLeftLine));
    // Checkmark-RightLine
    m_ptPosCheckmarkRightLine = QPointF(17.5, 17.5);
    m_lineCheckmarkRightLine = QLineF(QPointF(-7.5, 7.5), QPointF(7.5, -7.5));
    m_pPhysValLineCheckmarkRightLine->setP1(QPointF(35.0, bYAxisTopDown ? 50.0 : 0.0));
    m_pPhysValLineCheckmarkRightLine->setP2(QPointF(50.0, bYAxisTopDown ? 35.0 : 15.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkRightLine, m_ptPosCheckmarkRightLine,
        m_lineCheckmarkRightLine, *m_pPhysValLineCheckmarkRightLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Modify SmallRect
    //=================

    ZS::Test::CTestStepGroup* pGrpModifySmallRect = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Small Rect",
        /* pTSGrpParent */ pGrpModifyAll );

    // Rotate SmallRect
    //-----------------

    m_physValAngleSmallRect.setVal(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameSmallRect + ".setRotationAngle(" + m_physValAngleSmallRect.toString() + ")",
        /* strOperation    */ c_strGraphObjNameSmallRect + ".setRotationAngle(" + m_physValAngleSmallRect.toString() + ")",
        /* pGrpParent      */ pGrpModifySmallRect,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameSmallRect);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallRect));
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle",m_physValAngleSmallRect.toString());
    strlstExpectedValues.clear();
    // Group
    m_pPhysValRectSmallRect->setAngle(m_physValAngleSmallRect);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallRect, m_ptPosSmallRect, *m_pPhysValRectSmallRect));
    // SmallRect-TopLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectTopLine, m_ptPosSmallRectTopLine,
        m_lineSmallRectTopLine, *m_pPhysValLineSmallRectTopLine));
    // SmallRect-RightLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectRightLine, m_ptPosSmallRectRightLine,
        m_lineSmallRectRightLine, *m_pPhysValLineSmallRectRightLine));
    // SmallRect-BottomLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectBottomLine, m_ptPosSmallRectBottomLine,
        m_lineSmallRectBottomLine, *m_pPhysValLineSmallRectBottomLine));
    // SmallRect-LeftLine
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectLeftLine, m_ptPosSmallRectLeftLine,
        m_lineSmallRectLeftLine, *m_pPhysValLineSmallRectLeftLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Modify Top Group
    //=================

    ZS::Test::CTestStepGroup* pGrpModifyTopGroup = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Top Group",
        /* pTSGrpParent */ pGrpModifyAll );

    // Resize top group
    //-----------------

    QPoint ptBRTopGroup(450.0, bYAxisTopDown ? 450.0 : 150.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRTopGroup) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRTopGroup) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyTopGroup,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBRTopGroup);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    // Group Top
    m_ptPosTopGroup = QPointF(350.0, 350.0);
    m_sizeTopGroup = QSizeF(200.0, 200.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(QPointF(350.0, bYAxisTopDown ? 350.0 : 250.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(-50.0, 50.0);
    m_sizeSmallPlusSign = QSizeF(60.0, 60.0);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    m_pPhysValRectSmallPlusSign->setCenter(QPointF(50.0, bYAxisTopDown ? 150.0 : 50.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = QSizeF(200.0, 200.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(100.0, 100.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // Checkmark
    m_ptPosCheckmark = QPointF(0.0, 0.0);
    m_sizeCheckmark = QSizeF(100.0, 100.0);
    m_pPhysValRectCheckmark->setSize(m_sizeCheckmark);
    m_pPhysValRectCheckmark->setCenter(QPointF(100.0, 100.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // SmallRect
    m_ptPosSmallRect = QPointF(50.0, -50.0);
    m_sizeSmallRect = QSizeF(20.0, 20.0);
    m_pPhysValRectSmallRect->setSize(m_sizeSmallRect);
    m_pPhysValRectSmallRect->setCenter(QPointF(150.0, bYAxisTopDown ? 50.0 : 150.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallRect, m_ptPosSmallRect, *m_pPhysValRectSmallRect));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate top group
    //-----------------

    m_physValAngleTopGroup.setVal(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setRotationAngle(" + m_physValAngleTopGroup.toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setRotationAngle(" + m_physValAngleTopGroup.toString() + ")",
        /* pGrpParent      */ pGrpModifyTopGroup,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", m_physValAngleTopGroup.toString());
    strlstExpectedValues.clear();
    // Group Top
    m_pPhysValRectTopGroup->setAngle(m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // SmallPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // Checkmark
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // SmallRect
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallRect, m_ptPosSmallRect, *m_pPhysValRectSmallRect));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize top group (setWidth)
    //----------------------------

    m_sizeTopGroup.setWidth(300.0);
    m_pPhysValRectTopGroup->setWidth(300.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".setWidth(" + m_pPhysValRectTopGroup->width().toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setWidth(" + m_pPhysValRectTopGroup->width().toString() + ")",
        /* pGrpParent      */ pGrpModifyTopGroup,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "setWidth");
    pTestStep->setConfigValue("Width", m_pPhysValRectTopGroup->width().toString());
    strlstExpectedValues.clear();
    // Group Top
    m_ptPosTopGroup = QPointF(385.4, 385.4);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(-75.0, 50.0);
    m_sizeSmallPlusSign = QSizeF(90.0, 60.0);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    m_pPhysValRectSmallPlusSign->setCenter(QPointF(75.0, bYAxisTopDown ? 150.0 : 50.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = QSizeF(300.0, 200.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(150.0, 100.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // Checkmark
    m_ptPosCheckmark = QPointF(0.0, 0.0);
    m_sizeCheckmark = QSizeF(150.0, 100.0);
    m_pPhysValRectCheckmark->setSize(m_sizeCheckmark);
    m_pPhysValRectCheckmark->setCenter(QPointF(150.0, 100.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // SmallRect
    m_ptPosSmallRect = QPointF(75.0, -50.0);
    m_sizeSmallRect = QSizeF(30.0, 20.0);
    m_pPhysValRectSmallRect->setSize(m_sizeSmallRect);
    m_pPhysValRectSmallRect->setCenter(QPointF(225.0, bYAxisTopDown ? 50.0 : 150.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallRect, m_ptPosSmallRect, *m_pPhysValRectSmallRect));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Remove SmallPlusSign from TopGroup
    //-----------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".removeFromGroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".removeFromGroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* pGrpParent      */ pGrpModifyTopGroup,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("GraphObjChildName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("GraphObjChildKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign));
    pTestStep->setConfigValue("Method", "removeFromGroup");
    strlstExpectedValues.clear();
    // Group
    m_ptPosSmallPlusSign = QPointF(296.97, 367.68);
    m_pPhysValRectSmallPlusSign->setCenter(bYAxisTopDown ? m_ptPosSmallPlusSign : QPointF(296.97, 232.32));
    m_pPhysValRectSmallPlusSign->setAngle(m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -30.0), QPointF(0.0, 30.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP1(QPointF(45.0, bYAxisTopDown ? 0.0 : 60.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP2(QPointF(45.0, bYAxisTopDown ? 60.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-45.0, 0.0), QPointF(45.0, 0.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP1(QPointF(0.0, 30.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP2(QPointF(90.0, 30.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Remove BigPlusSign from TopGroup
    //-----------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameTopGroup + ".removeFromGroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".removeFromGroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyTopGroup,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSign] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignVerticalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine);
    m_hshGraphObjNameToKeys[c_strGraphObjNameBigPlusSignHorizontalLine] = pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("GraphObjChildName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjChildKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("Method", "removeFromGroup");
    strlstExpectedValues.clear();
    // Group
    m_ptPosBigPlusSign = QPointF(385.36, 385.36);
    m_pPhysValRectBigPlusSign->setCenter(bYAxisTopDown ? m_ptPosBigPlusSign : QPointF(385.36, 214.64));
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -100.0), QPointF(0.0, 100.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(150.0, bYAxisTopDown ? 0.0 : 200.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(150.0, bYAxisTopDown ? 200.0 : 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-150.0, 0.0), QPointF(150.0, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(0.0, 100.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(300.0, 100.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);
}
