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
ZS::Test::CTestStepGroup* CTest::createTestGroupAddObjects(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddObjects",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpAddObjects = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add Objects",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    CDrawGridSettings gridSettings;
    gridSettings.setLinesVisible(true);
    gridSettings.setLinesDistMin(20);
    gridSettings.setLabelsVisible(true);
    gridSettings.setLabelsFont(QFont("Terminal"));

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Pixels Drawing",
        /* pTSGrpParent */ pGrpAddObjects );

    CDrawingSize drawingSize;
    drawingSize.setDimensionUnit(EScaleDimensionUnit::Pixels);
    drawingSize.setScreenResolutionInPxPerMM(1.0);
    drawingSize.setImageSize(CPhysVal(800, Units.Length.px), CPhysVal(600, Units.Length.px));

    createTestGroupPrepareScene(pGrpPixelsDrawing, io_idxGroup, drawingSize, gridSettings);
    createTestGroupAddStandardShapes(pGrpPixelsDrawing, io_idxGroup, drawingSize);
    createTestGroupSaveLoadFile(pGrpPixelsDrawing, io_idxGroup, drawingSize);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawing = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Metrics Drawing",
        /* pTSGrpParent */ pGrpAddObjects );

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Y-Scale TopDown",
        /* pTSGrpParent */ pGrpMetricsDrawing );

    drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::TopDown);

    //createTestGroupPrepareScene(pGrpMetricsDrawingYScaleTopDown, io_idxGroup, drawingSize, gridSettings);
    //createTestGroupAddStandardShapes(pGrpMetricsDrawingYScaleTopDown, io_idxGroup, drawingSize);
    //createTestGroupSaveLoadFile(pGrpMetricsDrawingYScaleTopDown, io_idxGroup, drawingSize);

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Y-Scale BottomUp",
        /* pTSGrpParent */ pGrpMetricsDrawing );

    drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::BottomUp);

    //createTestGroupPrepareScene(pGrpMetricsDrawingYScaleBottomUp, io_idxGroup, drawingSize, gridSettings);
    //createTestGroupAddStandardShapes(pGrpMetricsDrawingYScaleBottomUp, io_idxGroup, drawingSize);
    //createTestGroupSaveLoadFile(pGrpMetricsDrawingYScaleBottomUp, io_idxGroup, drawingSize);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpAddObjects->path());
    }
    return pGrpAddObjects;

} // createTestGroupAddObjects

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupAddStandardShapes(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
                       ", DimensionUnit: " + i_drawingSize.dimensionUnit().toString() +
                       ", YAxisOrientation: " + i_drawingSize.yScaleAxisOrientation().toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapes",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpAddStandardShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add " + CObjFactory::c_strGroupNameStandardShapes,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    createTestGroupAddStandardShapesLines(pGrpAddStandardShapes, io_idxGroup, i_drawingSize);
    createTestGroupAddStandardShapesGroups(pGrpAddStandardShapes, io_idxGroup, i_drawingSize);
    createTestGroupModifyStandardShapes(pGrpAddStandardShapes, io_idxGroup, i_drawingSize);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpAddStandardShapes->path());
    }
    return pGrpAddStandardShapes;

} // createTestGroupAddStandardShapes

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupAddStandardShapesLines(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
                       ", DimensionUnit: " + i_drawingSize.dimensionUnit().toString() +
                       ", YAxisOrientation: " + i_drawingSize.yScaleAxisOrientation().toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesLines",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpAddLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add Lines",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    //QString strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    bool bYAxisTopDown = (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : i_drawingSize.metricImageCoorsDecimals();

    //QString strGraphObjKeyInTree;

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    int idxStep = 0;
    QStringList strlstExpectedValues;

    // Small Plus Sign
    //================

    ZS::Test::CTestStepGroup* pGrpAddSmallPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " SmallPlusSign",
        /* pTSGrpParent */ pGrpAddLines );

    // Vertical Line
    //--------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameSmallPlusSignVerticalLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallPlusSignVerticalLine + ")",
        /* pGrpParent      */ pGrpAddSmallPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1SmallPlusSignVerticalLine(275.0, bYAxisTopDown ? 310.0 : 290.0);
    QPointF ptP2SmallPlusSignVerticalLine(275.0, bYAxisTopDown ? 340.0 : 260.0);
    m_ptPosSmallPlusSignVerticalLine = QPointF(275.0, 325.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -15.0), QPointF(0.0, 15.0));
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, ptP1SmallPlusSignVerticalLine, ptP2SmallPlusSignVerticalLine);
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
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameSmallPlusSignHorizontalLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallPlusSignHorizontalLine + ")",
        /* pGrpParent      */ pGrpAddSmallPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1SmallPlusSignHorizontalLine(260.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptP2SmallPlusSignHorizontalLine(290.0, bYAxisTopDown ? 325.0 : 275.0);
    m_ptPosSmallPlusSignHorizontalLine = QPointF(275.0, 325.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, ptP1SmallPlusSignHorizontalLine, ptP2SmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("P1", ptP1SmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("P2", ptP2SmallPlusSignHorizontalLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Big Plus Sign
    //================

    ZS::Test::CTestStepGroup* pGrpAddBigPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " BigPlusSign",
        /* pTSGrpParent */ pGrpAddLines );

    // Vertical Line
    //--------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameBigPlusSignVerticalLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameBigPlusSignVerticalLine + ")",
        /* pGrpParent      */ pGrpAddBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1BigPlusSignVerticalLine(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptP2BigPlusSignVerticalLine(300.0, bYAxisTopDown ? 350.0 : 250.0);
    m_ptPosBigPlusSignVerticalLine = QPointF(300.0, 300.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, ptP1BigPlusSignVerticalLine, ptP2BigPlusSignVerticalLine);
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
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameBigPlusSignHorizontalLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameBigPlusSignHorizontalLine + ")",
        /* pGrpParent      */ pGrpAddBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1BigPlusSignHorizontalLine(250.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptP2BigPlusSignHorizontalLine(350.0, bYAxisTopDown ? 300.0 : 300.0);
    m_ptPosBigPlusSignHorizontalLine = QPointF(300.0, 300.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    *m_pPhysValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, ptP1BigPlusSignHorizontalLine, ptP2BigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("P1", ptP1BigPlusSignHorizontalLine);
    pTestStep->setConfigValue("P2", ptP2BigPlusSignHorizontalLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Checkmark
    //================

    ZS::Test::CTestStepGroup* pGrpAddCheckmark = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Checkmark",
        /* pTSGrpParent */ pGrpAddLines );

    // Left Line
    //----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameCheckmarkLeftLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameCheckmarkLeftLine + ")",
        /* pGrpParent      */ pGrpAddCheckmark,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1CheckmarkLeftLine(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptP2CheckmarkLeftLine(325.0, bYAxisTopDown ? 350.0 : 250.0);
    m_ptPosCheckmarkLeftLine = QPointF(287.5, 300.0);
    m_lineCheckmarkLeftLine = QLineF(QPointF(-37.5, -50.0), QPointF(37.5, 50.0));
    *m_pPhysValLineCheckmarkLeftLine = CPhysValLine(*m_pDrawingScene, ptP1CheckmarkLeftLine, ptP2CheckmarkLeftLine);
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
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameCheckmarkRightLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameCheckmarkRightLine + ")",
        /* pGrpParent      */ pGrpAddCheckmark,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1CheckmarkRightLine(325.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptP2CheckmarkRightLine(350.0, bYAxisTopDown ? 325.0 : 275.0);
    m_ptPosCheckmarkRightLine = QPointF(337.5, 337.5);
    m_lineCheckmarkRightLine = QLineF(QPointF(-12.5, 12.5), QPointF(12.5, -12.5));
    *m_pPhysValLineCheckmarkRightLine = CPhysValLine(*m_pDrawingScene, ptP1CheckmarkRightLine, ptP2CheckmarkRightLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmarkRightLine);
    pTestStep->setConfigValue("P1", ptP1CheckmarkRightLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkRightLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkRightLine, m_ptPosCheckmarkRightLine,
        m_lineCheckmarkRightLine, *m_pPhysValLineCheckmarkRightLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Small Rectangle
    //================

    ZS::Test::CTestStepGroup* pGrpAddSmallRect = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " SmallRect",
        /* pTSGrpParent */ pGrpAddLines );

    // Top Line
    //---------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameSmallRectTopLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectTopLine + ")",
        /* pGrpParent      */ pGrpAddSmallRect,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1SmallRectTopLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectTopLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    m_ptPosSmallRectTopLine = QPointF(325.0, 270.0);
    m_lineSmallRectTopLine = QLineF(QPointF(-5.0, 0.0), QPointF(5.0, 0.0));
    *m_pPhysValLineSmallRectTopLine = CPhysValLine(*m_pDrawingScene, ptP1SmallRectTopLine, ptP2SmallRectTopLine);
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
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameSmallRectRightLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectRightLine + ")",
        /* pGrpParent      */ pGrpAddSmallRect,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1SmallRectRightLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectRightLine(330.0, bYAxisTopDown ? 280.0 : 320.0);
    m_ptPosSmallRectRightLine = QPointF(330.0, 275.0);
    m_lineSmallRectRightLine = QLineF(QPointF(0.0, -5.0), QPointF(0.0, 5.0));
    *m_pPhysValLineSmallRectRightLine = CPhysValLine(*m_pDrawingScene, ptP1SmallRectRightLine, ptP2SmallRectRightLine);
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
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameSmallRectBottomLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectBottomLine + ")",
        /* pGrpParent      */ pGrpAddSmallRect,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1SmallRectBottomLine(330.0, bYAxisTopDown ? 280.0 : 320.0);
    QPointF ptP2SmallRectBottomLine(320.0, bYAxisTopDown ? 280.0 : 320.0);
    m_ptPosSmallRectBottomLine = QPointF(325.0, 280.0);
    m_lineSmallRectBottomLine = QLineF(QPointF(5.0, 0.0), QPointF(-5.0, 0.0));
    *m_pPhysValLineSmallRectBottomLine = CPhysValLine(*m_pDrawingScene, ptP1SmallRectBottomLine, ptP2SmallRectBottomLine);
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
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameSmallRectLeftLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectLeftLine + ")",
        /* pGrpParent      */ pGrpAddSmallRect,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1SmallRectLeftLine(320.0, bYAxisTopDown ? 280.0 : 320.0);
    QPointF ptP2SmallRectLeftLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    m_ptPosSmallRectLeftLine = QPointF(320.0, 275.0);
    m_lineSmallRectLeftLine = QLineF(QPointF(0.0, 5.0), QPointF(0.0, -5.0));
    *m_pPhysValLineSmallRectLeftLine = CPhysValLine(*m_pDrawingScene, ptP1SmallRectLeftLine, ptP2SmallRectLeftLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallRectLeftLine);
    pTestStep->setConfigValue("P1", ptP1SmallRectLeftLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectLeftLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectLeftLine, m_ptPosSmallRectLeftLine,
        m_lineSmallRectLeftLine, *m_pPhysValLineSmallRectLeftLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpAddLines->path());
    }
    return pGrpAddLines;

} // createTestGroupAddStandardShapesLines

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupAddStandardShapesGroups(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
                       ", DimensionUnit: " + i_drawingSize.dimensionUnit().toString() +
                       ", YAxisOrientation: " + i_drawingSize.yScaleAxisOrientation().toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddStandardShapesGroups",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpAddGroups = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add Groups",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    bool bYAxisTopDown = (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : i_drawingSize.metricImageCoorsDecimals();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    int idxStep = 0;
    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;

    // Small Plus Sign
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameSmallPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallPlusSign + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
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
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(15.0, 0.0), QPointF(15.0, 30.0));
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

    // Big Plus Sign
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
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
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, 0.0), QPointF(50.0, 100.0));
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

    // Checkmark
    //----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameCheckmark + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameCheckmark + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameCheckmarkLeftLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameCheckmarkRightLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmark);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group
    m_ptPosCheckmark = QPointF(300.0, 300.0);
    QPointF ptTLCheckmark(250.0, bYAxisTopDown ? 250.0 : 350.0);
    m_sizeCheckmark = QSizeF(100.0, 100.0);
    *m_pPhysValRectCheckmark = CPhysValRect(*m_pDrawingScene, ptTLCheckmark, m_sizeCheckmark);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // Checkmark-LeftLine
    m_ptPosCheckmarkLeftLine = QPointF(-12.5, 0.0);
    m_lineCheckmarkLeftLine = QLineF(QPointF(-37.5, -50.0), QPointF(37.5, 50.0));
    *m_pPhysValLineCheckmarkLeftLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 0.0), QPointF(75.0, 100.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkLeftLine, m_ptPosCheckmarkLeftLine,
        m_lineCheckmarkLeftLine, *m_pPhysValLineCheckmarkLeftLine));
    // Checkmark-RightLine
    m_ptPosCheckmarkRightLine = QPointF(37.5, 37.5);
    m_lineCheckmarkRightLine = QLineF(QPointF(-12.5, 12.5), QPointF(12.5, -12.5));
    *m_pPhysValLineCheckmarkRightLine = CPhysValLine(*m_pDrawingScene, QPointF(75.0, 100.0), QPointF(100.0, 75.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkRightLine, m_ptPosCheckmarkRightLine,
        m_lineCheckmarkRightLine, *m_pPhysValLineCheckmarkRightLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Small Rectangle
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameSmallRect + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRect + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
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
    QPointF ptTLSmallRect(320.0, bYAxisTopDown ? 270.0 : 330.0);
    m_sizeSmallRect = QSizeF(10.0, 10.0);
    *m_pPhysValRectSmallRect = CPhysValRect(*m_pDrawingScene, ptTLSmallRect, m_sizeSmallRect);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallRect, m_ptPosSmallRect, *m_pPhysValRectSmallRect));
    // SmallRect-TopLine
    m_ptPosSmallRectTopLine = QPointF(0.0, -5.0);
    m_lineSmallRectTopLine = QLineF(QPointF(-5.0, 0.0), QPointF(5.0, 0.0));
    *m_pPhysValLineSmallRectTopLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 0.0), QPointF(10.0, 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectTopLine, m_ptPosSmallRectTopLine,
        m_lineSmallRectTopLine, *m_pPhysValLineSmallRectTopLine));
    // SmallRect-RightLine
    m_ptPosSmallRectRightLine = QPointF(5.0, 0.0);
    m_lineSmallRectRightLine = QLineF(QPointF(0.0, -5.0), QPointF(0.0, 5.0));
    *m_pPhysValLineSmallRectRightLine = CPhysValLine(*m_pDrawingScene, QPointF(10.0, 0.0), QPointF(10.0, 10.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectRightLine, m_ptPosSmallRectRightLine,
        m_lineSmallRectRightLine, *m_pPhysValLineSmallRectRightLine));
    // SmallRect-BottomLine
    m_ptPosSmallRectBottomLine = QPointF(0.0, 5.0);
    m_lineSmallRectBottomLine = QLineF(QPointF(5.0, 0.0), QPointF(-5.0, 0.0));
    *m_pPhysValLineSmallRectBottomLine = CPhysValLine(*m_pDrawingScene, QPointF(10.0, 10.0), QPointF(0.0, 10.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectBottomLine, m_ptPosSmallRectBottomLine,
        m_lineSmallRectBottomLine, *m_pPhysValLineSmallRectBottomLine));
    // SmallRect-LeftLine
    m_ptPosSmallRectLeftLine = QPointF(-5.0, 0.0);
    m_lineSmallRectLeftLine = QLineF(QPointF(0.0, 5.0), QPointF(0.0, -5.0));
    *m_pPhysValLineSmallRectLeftLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 10.0), QPointF(0.0, 0.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallRectLeftLine, m_ptPosSmallRectLeftLine,
        m_lineSmallRectLeftLine, *m_pPhysValLineSmallRectLeftLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Top Group
    //----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameTopGroup + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSign);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSign);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameCheckmark);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallRect);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group
    m_ptPosTopGroup = QPointF(300.0, 300.0);
    QPointF ptTLPlusSignLinesCheckmarkSmallRect(250.0, bYAxisTopDown ? 250.0 : 350.0);
    m_sizeTopGroup = QSizeF(100.0, 100.0);
    *m_pPhysValRectTopGroup = CPhysValRect(*m_pDrawingScene, ptTLPlusSignLinesCheckmarkSmallRect, m_sizeTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(-25.0, 25.0);
    ptTLSmallPlusSign = QPointF(10.0, bYAxisTopDown ? 60.0 : 40.0);
    *m_pPhysValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, m_sizeSmallPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    ptTLBigPlusSign = QPointF(0.0, bYAxisTopDown ? 0.0 : 100.0);
    *m_pPhysValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, m_sizeBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // Checkmark
    m_ptPosCheckmark = QPointF(0.0, 0.0);
    ptTLCheckmark = QPointF(0.0, bYAxisTopDown ? 0.0 : 100.0);
    *m_pPhysValRectCheckmark = CPhysValRect(*m_pDrawingScene, ptTLCheckmark, m_sizeCheckmark);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // SmallRect
    m_ptPosSmallRect = QPointF(25.0, -25.0);
    ptTLSmallRect = QPointF(70.0, bYAxisTopDown ? 20.0 : 80.0);
    *m_pPhysValRectSmallRect = CPhysValRect(*m_pDrawingScene, ptTLSmallRect, m_sizeSmallRect);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallRect, m_ptPosSmallRect, *m_pPhysValRectSmallRect));
    pTestStep->setExpectedValues(strlstExpectedValues);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpAddGroups->path());
    }
    return pGrpAddGroups;

} // createTestGroupAddStandardShapesGroups

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupModifyStandardShapes(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
                       ", DimensionUnit: " + i_drawingSize.dimensionUnit().toString() +
                       ", YAxisOrientation: " + i_drawingSize.yScaleAxisOrientation().toString();
    }
    CMethodTracer mthTracer(+
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupModifyStandardShapes",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjTypeGroup = graphObjType2Str(EGraphObjTypeGroup);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    CUnit unit = i_drawingSize.unit();
    QString strUnit = unit.symbol();
    bool bUnitPixel = (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    int iDigits = bUnitPixel ? 0 : i_drawingSize.metricImageCoorsDecimals();
    bool bYAxisTopDown = (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStep* pTestStep = nullptr;
    int idxStep = 0;
    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;
    QStringList strlstGraphObjsKeyInTreeGetResultValues;

    ZS::Test::CTestStepGroup* pGrpModifyShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Modify Standard Shapes",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Checkmark
    //==========
#if 0
    ZS::Test::CTestStepGroup* pGrpModifyCheckmarkLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Checkmark Lines",
        /* pTSGrpParent */ pGrpModifyShapes );

    // Left Line
    //----------

    QPointF ptP1CheckmarkLeftLine(25.0, bYAxisTopDown ? 25.0 : 75.0);
    QPointF ptP2CheckmarkLeftLine(60.0, bYAxisTopDown ? 75.0 : 25.0);
    QPointF ptPosCheckmarkLeftLine(-7.5, 0.0);
    QPointF ptP1PosCheckmarkLeftLine(-17.5, -25.0);
    QPointF ptP2PosCheckmarkLeftLine(17.5, 25.0);
    CPhysValPoint physValPointCenterCheckmarkLeftLine(*m_pDrawingScene, 43.0, 50.0);
    double fLengthCheckmarkLeftLine = 61.0;
    CPhysValLine physValLineCheckmarkLeftLine(*m_pDrawingScene, ptP1CheckmarkLeftLine, ptP2CheckmarkLeftLine);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameCheckmarkLeftLine + ".setLine()",
        /* strOperation    */ c_strGraphObjNameCheckmarkLeftLine + ".setLine()",
        /* pGrpParent      */ pGrpModifyCheckmarkLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmarkLeftLine);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameCheckmark, c_strGraphObjNameCheckmarkLeftLine));
    pTestStep->setConfigValue("Method", "setLine");
    pTestStep->setConfigValue("P1", ptP1CheckmarkLeftLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkLeftLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkLeftLine + ".pos {" + qPoint2Str(ptPosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkLeftLine + ".p1 {" + qPoint2Str(ptP1PosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkLeftLine + ".p2 {" + qPoint2Str(ptP2PosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkLeftLine + ".length: " + QString::number(Math::round2Nearest(fLengthCheckmarkLeftLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkLeftLine + ".position {" + physValPointCenterCheckmarkLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkLeftLine + ".getLine {" + physValLineCheckmarkLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkLeftLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkLeftLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkLeftLine + ".rotationAngle: 55.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Right Line
    //-----------

    QPointF ptP1CheckmarkRightLine(60.0, bYAxisTopDown ? 75.0 : 25.0);
    QPointF ptP2CheckmarkRightLine(75.0, bYAxisTopDown ? 60.0 : 40.0);
    QPointF ptPosCheckmarkRightLine(17.5, 17.5);
    QPointF ptP1PosCheckmarkRightLine(-7.5, 7.5);
    QPointF ptP2PosCheckmarkRightLine(7.5, -7.5);
    CPhysValPoint physValPointCenterCheckmarkRightLine(*m_pDrawingScene, 68.0, 68.0);
    double fLengthCheckmarkRightLine = 21.2;
    CPhysValLine physValLineCheckmarkRightLine(*m_pDrawingScene, ptP1CheckmarkRightLine, ptP2CheckmarkRightLine);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameCheckmarkRightLine + ".setLine()",
        /* strOperation    */ c_strGraphObjNameCheckmarkRightLine + ".setLine()",
        /* pGrpParent      */ pGrpModifyCheckmarkLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmarkRightLine);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameCheckmark, c_strGraphObjNameCheckmarkRightLine));
    pTestStep->setConfigValue("Method", "setLine");
    pTestStep->setConfigValue("P1", ptP1CheckmarkRightLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkRightLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkRightLine + ".pos {" + qPoint2Str(ptPosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkRightLine + ".p1 {" + qPoint2Str(ptP1PosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkRightLine + ".p2 {" + qPoint2Str(ptP2PosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkRightLine + ".length: " + QString::number(Math::round2Nearest(fLengthCheckmarkRightLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkRightLine + ".position {" + physValPointCenterCheckmarkRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkRightLine + ".getLine {" + physValLineCheckmarkRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkRightLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkRightLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkRightLine + ".rotationAngle: 315.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Checkmark - resizeToContent
    //----------------------------
#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameCheckmark + ".resizeToContent()",
        /* strOperation    */ c_strGraphObjNameCheckmark + ".resizeToContent()",
        /* pGrpParent      */ pGrpModifyCheckmarkLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmark);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameCheckmark));
    pTestStep->setConfigValue("Method", "resizeToContent");
    // Group
    QPointF ptPosCheckmark(0.0, 0.0);
    QPointF ptTLCheckmark(25.0, bYAxisTopDown ? 25.0 : 75.0);
    QSizeF sizeCheckmark(50.0, 50.0);
    QRectF rectBoundingCheckmark(QPointF(-25.0, -25.0), sizeCheckmark);
    CPhysValRect physValRectCheckmark(*m_pDrawingScene, ptTLCheckmark, sizeCheckmark);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".pos {" + qPoint2Str(ptPosCheckmark) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".boundingRect {" + qRect2Str(rectBoundingCheckmark) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".position {" + physValRectCheckmark.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".getRect {" + physValRectCheckmark.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".getSize {" + physValRectCheckmark.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // Checkmark-LeftLine
    ptPosCheckmarkLeftLine = QPointF(-7.5, 0.0);
    QRectF rectBoundingCheckmarkLeftLine(QPointF(-17.5, -25.0), QSizeF(35.0, 50.0));
    fLengthCheckmarkLeftLine = 61.0;
    physValLineCheckmarkLeftLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 0.0), QPointF(35.0, 50.0));
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkLeftLine + ".pos {" + qPoint2Str(ptPosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkLeftLine + ".boundingRect {" + qRect2Str(rectBoundingCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkLeftLine + ".position {" + physValLineCheckmarkLeftLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkLeftLine + ".getLine {" + physValLineCheckmarkLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkLeftLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkLeftLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkLeftLine + ".rotationAngle: 55.0 " + Math::c_strSymbolDegree);
    // Checkmark-RightLine
    ptPosCheckmarkRightLine = QPointF(17.5, 17.5);
    QRectF rectBoundingCheckmarkRightLine(QPointF(-7.5, -7.5), QSizeF(15.0, 15.0));
    fLengthCheckmarkRightLine = 21.0;
    physValLineCheckmarkRightLine = CPhysValLine(*m_pDrawingScene, QPointF(35.0, 50.0), QPointF(50.0, 35.0));
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkRightLine + ".pos {" + qPoint2Str(ptPosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkRightLine + ".boundingRect {" + qRect2Str(rectBoundingCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkRightLine + ".position {" + physValLineCheckmarkRightLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkRightLine + ".getLine {" + physValLineCheckmarkRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkRightLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkRightLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmarkRightLine + ".rotationAngle: 315.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Modify PlusSigns
    //=================

    ZS::Test::CTestStepGroup* pGrpModifyPlusSigns = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Plus Sign Lines",
        /* pTSGrpParent */ pGrpModifyShapes );

    // Remove SmallPlusSign from TopGroup
    //-----------------------------------
#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameTopGroup + ".removeFromGroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".removeFromGroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupNameParent", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupNameParentKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign));
    pTestStep->setConfigValue("Method", "removeFromGroup");
    // Group
    QPointF ptPosSmallPlusSign(275.0, 325.0);
    QPointF ptTLSmallPlusSign(260.0, bYAxisTopDown ? 310.0 : 290.0);
    QSizeF sizeSmallPlusSign(30.0, 30.0);
    QRectF rectBoundingSmallPlusSign(QPointF(-15.0, -15.0), sizeSmallPlusSign);
    CPhysValRect physValRectSmallPlusSign(*m_pDrawingScene, ptTLSmallPlusSign, sizeSmallPlusSign);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".pos {" + qPoint2Str(ptPosSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".position {" + qPoint2Str(ptPosSmallPlusSign, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getRect {" + physValRectSmallPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getSize {" + physValRectSmallPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-VerticalLine
    QPointF ptPosSmallPlusSignVerticalLine(0.0, 0.0);
    QRectF rectBoundingSmallPlusSignVerticalLine(QPointF(0.0, -15.0), QSizeF(0.0, 30.0));
    double fLengthSmallPlusSignVerticalLine = 30.0;
    CPhysValLine physValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(15.0, 0.0), QPointF(15.0, 30.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    QPointF ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    QRectF rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-15.0, 0.0), QSizeF(30.0, 0.0));
    double fLengthSmallPlusSignHorizontalLine = 30.0;
    CPhysValLine physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 15.0), QPointF(30.0, 15.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Remove BigPlusSign from TopGroup
    //-----------------------------------
#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameTopGroup + ".removeFromGroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".removeFromGroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupNameParent", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupNameParentKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("Method", "removeFromGroup");
    // Group
    QPointF ptPosBigPlusSign(300.0, 300.0);
    QPointF ptTLBigPlusSign(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QSizeF sizeBigPlusSign(100.0, 100.0);
    QRectF rectBoundingBigPlusSign(QPointF(-50.0, -50.0), sizeBigPlusSign);
    CPhysValRect physValRectBigPlusSign(*m_pDrawingScene, ptTLBigPlusSign, sizeBigPlusSign);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".pos {" + qPoint2Str(ptPosBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".position {" + qPoint2Str(ptPosBigPlusSign, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getRect {" + physValRectBigPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getSize {" + physValRectBigPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    QPointF ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    QRectF rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -50.0), QSizeF(0.0, 100.0));
    double fLengthBigPlusSignVerticalLine = 100.0;
    CPhysValLine physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, 0.0), QPointF(50.0, 100.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    QPointF ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    QRectF rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-50.0, 0.0), QSizeF(100.0, 0.0));
    double fLengthBigPlusSignHorizontalLine = 100.0;
    CPhysValLine physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Ungroup SmallPlusSign
    //----------------------
#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " DrawingScene.ungroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSign));
    pTestStep->setConfigValue("Method", "ungroup");
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSignHorizontalLine));
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Vertical Line
    QPointF ptP1SmallPlusSignVerticalLine(275.0, bYAxisTopDown ? 310.0 : 290.0);
    QPointF ptP2SmallPlusSignVerticalLine(275.0, bYAxisTopDown ? 340.0 : 260.0);
    ptPosSmallPlusSignVerticalLine = QPointF(275.0, 325.0);
    rectBoundingSmallPlusSignVerticalLine = QRectF(QPointF(0.0, -15.0), QSizeF(0.0, 30.0));
    QPointF ptP1PosSmallPlusSignVerticalLine(0.0, -15.0);
    QPointF ptP2PosSmallPlusSignVerticalLine(0.0, 15.0);
    fLengthSmallPlusSignVerticalLine = 30.0;
    physValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, ptP1SmallPlusSignVerticalLine, ptP2SmallPlusSignVerticalLine);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".position {" + qPoint2Str(ptPosSmallPlusSignVerticalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // Horizontal Line
    QPointF ptP1SmallPlusSignHorizontalLine(260.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptP2SmallPlusSignHorizontalLine(290.0, bYAxisTopDown ? 325.0 : 275.0);
    ptPosSmallPlusSignHorizontalLine = QPointF(275.0, 325.0);
    rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-15.0, 0.0), QSizeF(30.0, 0.0));
    QPointF ptP1PosSmallPlusSignHorizontalLine(-15.0, 0.0);
    QPointF ptP2PosSmallPlusSignHorizontalLine(15.0, 0.0);
    fLengthSmallPlusSignHorizontalLine = 30.0;
    physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, ptP1SmallPlusSignHorizontalLine, ptP2SmallPlusSignHorizontalLine);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Ungroup BigPlusSign
    //----------------------
#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " DrawingScene.ungroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("Method", "ungroup");
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSignHorizontalLine));
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // Vertical Line
    QPointF ptP1BigPlusSignVerticalLine(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptP2BigPlusSignVerticalLine(300.0, bYAxisTopDown ? 350.0 : 250.0);
    ptPosBigPlusSignVerticalLine = QPointF(300.0, 300.0);
    rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -50.0), QSizeF(0.0, 100.0));
    QPointF ptP1PosBigPlusSignVerticalLine(0.0, -50.0);
    QPointF ptP2PosBigPlusSignVerticalLine(0.0, 50.0);
    fLengthBigPlusSignVerticalLine = 100.0;
    physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, ptP1BigPlusSignVerticalLine, ptP2BigPlusSignVerticalLine);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".position {" + qPoint2Str(ptPosBigPlusSignVerticalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // Horizontal Line
    QPointF ptP1BigPlusSignHorizontalLine(250.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptP2BigPlusSignHorizontalLine(350.0, bYAxisTopDown ? 300.0 : 300.0);
    ptPosBigPlusSignHorizontalLine = QPointF(300.0, 300.0);
    rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-50.0, 0.0), QSizeF(100.0, 0.0));
    QPointF ptP1PosBigPlusSignHorizontalLine(-50.0, 0.0);
    QPointF ptP2PosBigPlusSignHorizontalLine(50.0, 0.0);
    fLengthBigPlusSignHorizontalLine = 100.0;
    physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, ptP1BigPlusSignHorizontalLine, ptP2BigPlusSignHorizontalLine);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".position {" + qPoint2Str(ptPosBigPlusSignHorizontalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Group Plus Signs
    //-----------------
#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjGroupNamePlusSigns + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + strGraphObjGroupNamePlusSigns + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNamePlusSigns);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    // Group
    QPointF ptPosPlusSigns(300.0, 300.0);
    QPointF ptTLPlusSigns(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QSizeF sizePlusSigns(100.0, 100.0);
    QRectF rectBoundingPlusSigns(QPointF(-50.0, -50.0), sizePlusSigns);
    CPhysValRect physValRectPlusSigns(*m_pDrawingScene, ptTLPlusSigns, sizePlusSigns);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".pos {" + qPoint2Str(ptPosPlusSigns) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".boundingRect {" + qRect2Str(rectBoundingPlusSigns) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".position {" + qPoint2Str(ptPosPlusSigns, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".getRect {" + physValRectPlusSigns.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".getSize {" + physValRectPlusSigns.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-VerticalLine
    ptPosSmallPlusSignVerticalLine = QPointF(-25.0, 25.0);
    rectBoundingSmallPlusSignVerticalLine = QRectF(QPointF(0.0, -15.0), QSizeF(0.0, 30.0));
    fLengthSmallPlusSignVerticalLine = 30.0;
    physValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(25.0, 60.0), QPointF(25.0, 90.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    ptPosSmallPlusSignHorizontalLine = QPointF(-25.0, 25.0);
    rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-15.0, 0.0), QSizeF(30.0, 0.0));
    fLengthSmallPlusSignHorizontalLine = 30.0;
    physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(10.0, 75.0), QPointF(40.0, 75.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -50.0), QSizeF(0.0, 100.0));
    fLengthBigPlusSignVerticalLine = 100.0;
    physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, 0.0), QPointF(50.0, 100.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-50.0, 0.0), QSizeF(100.0, 0.0));
    fLengthBigPlusSignHorizontalLine = 100.0;
    physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Resize PlusSigns
    //-----------------
#if 0
    ptTLPlusSigns = QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0);
    sizePlusSigns = QSizeF(400.0, 200.0);
    QPointF ptBRPlusSigns(650.0, bYAxisTopDown ? 450.0 : 150.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjGroupNamePlusSigns + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* strOperation    */ strGraphObjGroupNamePlusSigns + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNamePlusSigns);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNamePlusSigns));
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBRPlusSigns);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    // Group PlusSigns
    ptPosPlusSigns = QPointF(450.0, 350.0);
    rectBoundingPlusSigns = QRectF(QPointF(-200.0, -100.0), sizePlusSigns);
    physValRectPlusSigns = CPhysValRect(*m_pDrawingScene, ptTLPlusSigns, sizePlusSigns);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".pos {" + qPoint2Str(ptPosPlusSigns) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".boundingRect {" + qRect2Str(rectBoundingPlusSigns) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".position {" + physValRectPlusSigns.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".getRect {" + physValRectPlusSigns.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".getSize {" + physValRectPlusSigns.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-VerticalLine
    ptPosSmallPlusSignVerticalLine = QPointF(-100.0, 50.0);
    rectBoundingSmallPlusSignVerticalLine = QRectF(QPointF(0.0, -30.0), QSizeF(0.0, 60.0));
    fLengthSmallPlusSignVerticalLine = 60.0;
    physValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(100.0, 120.0), QPointF(100.0, 180.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    ptPosSmallPlusSignHorizontalLine = QPointF(-100.0, 50.0);
    rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-60.0, 0.0), QSizeF(120.0, 0.0));
    fLengthSmallPlusSignHorizontalLine = 120.0;
    physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(40.0, 150.0), QPointF(160.0, 150.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -100.0), QSizeF(0.0, 200.0));
    fLengthBigPlusSignVerticalLine = 200.0;
    physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(200.0, 0.0), QPointF(200.0, 200.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-200.0, 0.0), QSizeF(400.0, 0.0));
    fLengthBigPlusSignHorizontalLine = 400.0;
    physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 100.0), QPointF(400.0, 100.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Ungroup PlusSigns
    //------------------
#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " DrawingScene.ungroup(" + strGraphObjGroupNamePlusSigns + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + strGraphObjGroupNamePlusSigns + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNamePlusSigns);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNamePlusSigns));
    pTestStep->setConfigValue("Method", "ungroup");
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSignHorizontalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSignHorizontalLine));
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // SmallPlusSign-VerticalLine
    ptPosSmallPlusSignVerticalLine = QPointF(350.0, 400.0);
    rectBoundingSmallPlusSignVerticalLine = QRectF(QPointF(0.0, -30.0), QSizeF(0.0, 60.0));
    fLengthSmallPlusSignVerticalLine = 60.0;
    physValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(350.0, 370.0), QPointF(350.0, 430.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    ptPosSmallPlusSignHorizontalLine = QPointF(350.0, 400.0);
    rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-60.0, 0.0), QSizeF(120.0, 0.0));
    fLengthSmallPlusSignHorizontalLine = 120.0;
    physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(290.0, 400.0), QPointF(410.0, 400.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    ptPosBigPlusSignVerticalLine = QPointF(450.0, 350.0);
    rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -100.0), QSizeF(0.0, 200.0));
    fLengthBigPlusSignVerticalLine = 200.0;
    physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(450.0, 250.0), QPointF(450.0, 450.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    ptPosBigPlusSignHorizontalLine = QPointF(450.0, 350.0);
    rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-200.0, 0.0), QSizeF(400.0, 0.0));
    fLengthBigPlusSignHorizontalLine = 400.0;
    physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(250.0, 350.0), QPointF(650.0, 350.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Small Plus Sign
    //----------------
#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameSmallPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameSmallPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    // Group
    ptPosSmallPlusSign = QPointF(350.0, 400.0);
    ptTLSmallPlusSign = QPointF(290.0, bYAxisTopDown ? 370.0 : 230.0);
    sizeSmallPlusSign = QSizeF(120.0, 60.0);
    rectBoundingSmallPlusSign = QRectF(QPointF(-60.0, -30.0), sizeSmallPlusSign);
    physValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, sizeSmallPlusSign);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".pos {" + qPoint2Str(ptPosSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".position {" + qPoint2Str(ptPosSmallPlusSign, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getRect {" + physValRectSmallPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getSize {" + physValRectSmallPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-VerticalLine
    ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    rectBoundingSmallPlusSignVerticalLine = QRectF(QPointF(0.0, -30.0), QSizeF(0.0, 60.0));
    fLengthSmallPlusSignVerticalLine = 60.0;
    physValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(60.0, 0.0), QPointF(60.0, 60.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-60.0, 0.0), QSizeF(120.0, 0.0));
    fLengthSmallPlusSignHorizontalLine = 120.0;
    physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 30.0), QPointF(120.0, 30.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Big Plus Sign
    //----------------
#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    // Group
    ptPosBigPlusSign = QPointF(450.0, 350.0);
    ptTLBigPlusSign = QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0);
    sizeBigPlusSign = QSizeF(400.0, 200.0);
    rectBoundingBigPlusSign = QRectF(QPointF(-200.0, -100.0), sizeBigPlusSign);
    physValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, sizeBigPlusSign);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".pos {" + qPoint2Str(ptPosBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".position {" + qPoint2Str(ptPosBigPlusSign, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getRect {" + physValRectBigPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getSize {" + physValRectBigPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -100.0), QSizeF(0.0, 200.0));
    fLengthBigPlusSignVerticalLine = 200.0;
    physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(200.0, 0.0), QPointF(200.0, 200.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-200.0, 0.0), QSizeF(400.0, 0.0));
    fLengthBigPlusSignHorizontalLine = 400.0;
    physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 100.0), QPointF(400.0, 100.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Group Plus Signs
    //-----------------
#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjGroupNamePlusSigns + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + strGraphObjGroupNamePlusSigns + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameSmallPlusSign);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNamePlusSigns);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNamePlusSigns));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNamePlusSigns, c_strGraphObjNameSmallPlusSign));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNamePlusSigns, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNamePlusSigns, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNamePlusSigns, c_strGraphObjNameBigPlusSign));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNamePlusSigns, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNamePlusSigns, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine));
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    // Group PlusSigns
    ptPosPlusSigns = QPointF(450.0, 350.0);
    ptTLPlusSigns = QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0);
    sizePlusSigns = QSizeF(400.0, 200.0);
    rectBoundingPlusSigns = QRectF(QPointF(-200.0, -100.0), sizePlusSigns);
    physValRectPlusSigns = CPhysValRect(*m_pDrawingScene, ptTLPlusSigns, sizePlusSigns);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".pos {" + qPoint2Str(ptPosPlusSigns) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".boundingRect {" + qRect2Str(rectBoundingPlusSigns) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".position {" + qPoint2Str(ptPosPlusSigns, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".getRect {" + physValRectPlusSigns.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".getSize {" + physValRectPlusSigns.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign
    ptPosSmallPlusSign = QPointF(-100.0, 50.0);
    ptTLSmallPlusSign = QPointF(40.0, bYAxisTopDown ? 120.0 : 80.0);
    sizeSmallPlusSign = QSizeF(120.0, 60.0);
    rectBoundingSmallPlusSign = QRectF(QPointF(-60.0, -30.0), sizeSmallPlusSign);
    physValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, sizeSmallPlusSign);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".pos {" + qPoint2Str(ptPosSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".position {" + physValRectSmallPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getRect {" + physValRectSmallPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getSize {" + physValRectSmallPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-VerticalLine
    ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    rectBoundingSmallPlusSignVerticalLine = QRectF(QPointF(0.0, -30.0), QSizeF(0.0, 60.0));
    fLengthSmallPlusSignVerticalLine = 60.0;
    physValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(60.0, 0.0), QPointF(60.0, 60.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-60.0, 0.0), QSizeF(120.0, 0.0));
    fLengthSmallPlusSignHorizontalLine = 120.0;
    physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 30.0), QPointF(120.0, 30.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign
    ptPosBigPlusSign = QPointF(0.0, 0.0);
    ptTLBigPlusSign = QPointF(0.0, bYAxisTopDown ? 0.0 : 200.0);
    sizeBigPlusSign = QSizeF(400.0, 200.0);
    rectBoundingBigPlusSign = QRectF(QPointF(-200.0, -100.0), sizeBigPlusSign);
    physValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, sizeBigPlusSign);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".pos {" + qPoint2Str(ptPosBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".position {" + physValRectBigPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getRect {" + physValRectBigPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getSize {" + physValRectBigPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -100.0), QSizeF(0.0, 200.0));
    fLengthBigPlusSignVerticalLine = 200.0;
    physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(200.0, 0.0), QPointF(200.0, 200.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-200.0, 0.0), QSizeF(400.0, 0.0));
    fLengthBigPlusSignHorizontalLine = 400.0;
    physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 100.0), QPointF(400.0, 100.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Resize PlusSigns
    //-----------------
#if 0
    ptTLPlusSigns = QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0);
    sizePlusSigns = QSizeF(100.0, 100.0);
    ptBRPlusSigns = QPointF(350.0, bYAxisTopDown ? 350.0 : 250.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjGroupNamePlusSigns + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* strOperation    */ strGraphObjGroupNamePlusSigns + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNamePlusSigns);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNamePlusSigns));
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBRPlusSigns);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNamePlusSigns));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNamePlusSigns, c_strGraphObjNameSmallPlusSign));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNamePlusSigns, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNamePlusSigns, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNamePlusSigns, c_strGraphObjNameBigPlusSign));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNamePlusSigns, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNamePlusSigns, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine));
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    // Group PlusSigns
    ptPosPlusSigns = QPointF(300.0, 300.0);
    rectBoundingPlusSigns = QRectF(QPointF(-50.0, -50.0), sizePlusSigns);
    physValRectPlusSigns = CPhysValRect(*m_pDrawingScene, ptTLPlusSigns, sizePlusSigns);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".pos {" + qPoint2Str(ptPosPlusSigns) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".boundingRect {" + qRect2Str(rectBoundingPlusSigns) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".position {" + physValRectPlusSigns.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".getRect {" + physValRectPlusSigns.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".getSize {" + physValRectPlusSigns.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign
    ptPosSmallPlusSign = QPointF(-25.0, 25.0);
    ptTLSmallPlusSign = QPointF(10.0, bYAxisTopDown ? 60.0 : 40.0);
    sizeSmallPlusSign = QSizeF(30.0, 30.0);
    rectBoundingSmallPlusSign = QRectF(QPointF(-15.0, -15.0), sizeSmallPlusSign);
    physValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, sizeSmallPlusSign);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".pos {" + qPoint2Str(ptPosSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".position {" + physValRectSmallPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getRect {" + physValRectSmallPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getSize {" + physValRectSmallPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-VerticalLine
    ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    rectBoundingSmallPlusSignVerticalLine = QRectF(QPointF(0.0, -15.0), QSizeF(0.0, 30.0));
    fLengthSmallPlusSignVerticalLine = 30.0;
    physValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(15.0, 0.0), QPointF(15.0, 30.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-15.0, 0.0), QSizeF(30.0, 0.0));
    fLengthSmallPlusSignHorizontalLine = 30.0;
    physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 15.0), QPointF(30.0, 15.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign
    ptPosBigPlusSign = QPointF(0.0, 0.0);
    ptTLBigPlusSign = QPointF(0.0, bYAxisTopDown ? 0.0 : 100.0);
    sizeBigPlusSign = QSizeF(100.0, 100.0);
    rectBoundingBigPlusSign = QRectF(QPointF(-50.0, -50.0), sizeBigPlusSign);
    physValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, sizeBigPlusSign);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".pos {" + qPoint2Str(ptPosBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".position {" + physValRectBigPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getRect {" + physValRectBigPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getSize {" + physValRectBigPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -50.0), QSizeF(0.0, 100.0));
    fLengthBigPlusSignVerticalLine = 100.0;
    physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, 0.0), QPointF(50.0, 100.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-50.0, 0.0), QSizeF(100.0, 0.0));
    fLengthBigPlusSignHorizontalLine = 100.0;
    physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Ungroup PlusSigns
    //------------------
#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " DrawingScene.ungroup(" + strGraphObjGroupNamePlusSigns + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + strGraphObjGroupNamePlusSigns + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNamePlusSigns);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNamePlusSigns));
    pTestStep->setConfigValue("Method", "ungroup");
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameSmallPlusSign, c_strGraphObjNameSmallPlusSignHorizontalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameBigPlusSign, c_strGraphObjNameBigPlusSignHorizontalLine));
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // SmallPlusSign
    ptPosSmallPlusSign = QPointF(275.0, 325.0);
    ptTLSmallPlusSign = QPointF(260.0, bYAxisTopDown ? 310.0 : 290.0);
    sizeSmallPlusSign = QSizeF(30.0, 30.0);
    rectBoundingSmallPlusSign = QRectF(QPointF(-15.0, -15.0), sizeSmallPlusSign);
    physValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, sizeSmallPlusSign);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".pos {" + qPoint2Str(ptPosSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".position {" + physValRectSmallPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getRect {" + physValRectSmallPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getSize {" + physValRectSmallPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-VerticalLine
    ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    rectBoundingSmallPlusSignVerticalLine = QRectF(QPointF(0.0, -15.0), QSizeF(0.0, 30.0));
    fLengthSmallPlusSignVerticalLine = 30.0;
    physValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(15.0, 0.0), QPointF(15.0, 30.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-15.0, 0.0), QSizeF(30.0, 0.0));
    fLengthSmallPlusSignHorizontalLine = 30.0;
    physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 15.0), QPointF(30.0, 15.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign
    ptPosBigPlusSign = QPointF(300.0, 300.0);
    ptTLBigPlusSign = QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0);
    sizeBigPlusSign = QSizeF(100.0, 100.0);
    rectBoundingBigPlusSign = QRectF(QPointF(-50.0, -50.0), sizeBigPlusSign);
    physValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, sizeBigPlusSign);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".pos {" + qPoint2Str(ptPosBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".position {" + physValRectBigPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getRect {" + physValRectBigPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getSize {" + physValRectBigPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -50.0), QSizeF(0.0, 100.0));
    fLengthBigPlusSignVerticalLine = 100.0;
    physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, 0.0), QPointF(50.0, 100.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-50.0, 0.0), QSizeF(100.0, 0.0));
    fLengthBigPlusSignHorizontalLine = 100.0;
    physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Move SmallPlusSign to Top Group
    //--------------------------------
#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameTopGroup + ".addToGroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".addToGroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupNameParent", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupNameParentKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSign));
    pTestStep->setConfigValue("Method", "addToGroup");
    // Moved SmallPlusSign
    ptPosSmallPlusSign = QPointF(-25.0, 25.0);
    ptTLSmallPlusSign = QPointF(10.0, bYAxisTopDown ? 60.0 : 40.0);
    sizeSmallPlusSign = QSizeF(30.0, 30.0);
    rectBoundingSmallPlusSign = QRectF(QPointF(-15.0, -15.0), sizeSmallPlusSign);
    physValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, sizeSmallPlusSign);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".pos {" + qPoint2Str(ptPosSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".position {" + physValRectSmallPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getRect {" + physValRectSmallPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getSize {" + physValRectSmallPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-VerticalLine
    ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    rectBoundingSmallPlusSignVerticalLine = QRectF(QPointF(0.0, -15.0), QSizeF(0.0, 30.0));
    fLengthSmallPlusSignVerticalLine = 30.0;
    physValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(15.0, 0.0), QPointF(15.0, 30.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-15.0, 0.0), QSizeF(30.0, 0.0));
    fLengthSmallPlusSignHorizontalLine = 30.0;
    physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 15.0), QPointF(30.0, 15.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " showLabels",
        /* strOperation    */ "showLabels()",
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
        /* strName         */ "Step " + QString::number(++idxStep) + " showGeometryLabels",
        /* strOperation    */ "showGeometryLabels()",
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

    m_physValAngleBigPlusSign = CPhysVal(45.0, Units.Angle.Degree, 0.1);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", m_physValAngleBigPlusSign.toString());
    strlstExpectedValues.clear();
    // Group
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(m_ptPosBigPlusSign);
    m_pPhysValRectBigPlusSign->setAngle(m_physValAngleBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, 0.0), QPointF(50.0, 100.0));
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

    // Show Geometry Labels
    //---------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " showGeometryLabels",
        /* strOperation    */ "showGeometryLabels()",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepShowGeometryLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameBigPlusSign},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign)},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"ExpectedText", m_pPhysValRectBigPlusSign->topLeft().toString()},
    });
    pTestStep->addDataRow({
        {"GraphObjName", c_strGraphObjNameBigPlusSign},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign)},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"ExpectedText", m_physValAngleBigPlusSign.toString()},
    });

    // Rotate BigPlusSign
    //-------------------
#if 0
    physValAngleBigPlusSign.setVal(135.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + physValAngleBigPlusSign.toString() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + physValAngleBigPlusSign.toString() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", physValAngleBigPlusSign.toString());
    strlstExpectedValues.clear();
    // Group
    ptPosBigPlusSign = QPointF(300.0, 300.0);
    sizeBigPlusSign = QSizeF(100.0, 100.0);
    rectBoundingPlusSign = QRectF(QPointF(-50.0, -50.0), sizeBigPlusSign);
    physValRectBigPlusSign = CPhysValRect(*m_pDrawingScene);
    physValRectBigPlusSign.setSize(sizeBigPlusSign);
    physValRectBigPlusSign.setCenter(ptPosBigPlusSign);
    physValRectBigPlusSign.setAngle(physValAngleBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, ptPosBigPlusSign, rectBoundingPlusSign, physValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    rctBoundingBigPlusSignVerticalLine = QRectF(lineBigPlusSignVerticalLine.p1(), lineBigPlusSignVerticalLine.p2());
    physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, 0.0), QPointF(50.0, 100.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, ptPosBigPlusSignVerticalLine, rctBoundingBigPlusSignVerticalLine,
        lineBigPlusSignVerticalLine, physValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    rctBoundingBigPlusSignHorizontalLine = QRectF(lineBigPlusSignHorizontalLine.p1(), lineBigPlusSignHorizontalLine.p2());
    physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, ptPosBigPlusSignHorizontalLine, rctBoundingBigPlusSignHorizontalLine,
        lineBigPlusSignHorizontalLine, physValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Move BigPlusSign to Top Group
    //--------------------------------
#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameTopGroup + ".addToGroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".addToGroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupNameParent", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupNameParentKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("Method", "addToGroup");
    // Moved BigPlusSign
    ptPosBigPlusSign = QPointF(0.0, 0.0);
    ptTLBigPlusSign = QPointF(0.0, bYAxisTopDown ? 0.0 : 100.0);
    sizeBigPlusSign = QSizeF(100.0, 100.0);
    rectBoundingBigPlusSign = QRectF(QPointF(-50.0, -50.0), sizeBigPlusSign);
    physValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, sizeBigPlusSign);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".pos {" + qPoint2Str(ptPosBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".position {" + physValRectBigPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getRect {" + physValRectBigPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getSize {" + physValRectBigPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -50.0), QSizeF(0.0, 100.0));
    fLengthBigPlusSignVerticalLine = 100.0;
    physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, 0.0), QPointF(50.0, 100.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-50.0, 0.0), QSizeF(100.0, 0.0));
    fLengthBigPlusSignHorizontalLine = 100.0;
    physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Show Geometry Labels
    //---------------------
#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " showGeometryLabels",
        /* strOperation    */ "showGeometryLabels()",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepShowGeometryLabels(ZS::Test::CTestStep*)) );
    QPointF ptTLPlusSignLinesCheckmarkSmallRect(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QSizeF sizePlusSignLinesCheckmarkSmallRect(100.0, 100.0);
    CPhysValRect physValRectPlusSignLinesCheckmarkSmallRect(*m_pDrawingScene, ptTLPlusSignLinesCheckmarkSmallRect, sizePlusSignLinesCheckmarkSmallRect);
    QPointF ptTLCheckmark(0.0, bYAxisTopDown ? 0.0 : 100.0);
    QSizeF sizeCheckmark(100.0, 100.0);
    CPhysValRect physValRectCheckmark(*m_pDrawingScene, ptTLCheckmark, sizeCheckmark);
    QPointF ptTLSmallRect(70.0, bYAxisTopDown ? 20.0 : 80.0);
    QSizeF sizeSmallRect(10.0, 10.0);
    CPhysValRect physValRectSmallRect(*m_pDrawingScene, ptTLSmallRect, sizeSmallRect);
    pTestStep->addDataRow({
        {"GraphObjName", "Top"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "Top")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"setPos", QPointF(225.0, 235.0)},
        {"ExpectedText", physValRectPlusSignLinesCheckmarkSmallRect.topLeft().toString()},
    });
    pTestStep->addDataRow({
        {"GraphObjName", "Top"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "Top")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"setPos", QPointF(380.0, 300.0)},
        {"ExpectedText", physValRectPlusSignLinesCheckmarkSmallRect.angle().toString()}
    });
    pTestStep->addDataRow({
        {"GraphObjName", "Checkmark"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "Top", "Checkmark")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"setPos", QPointF(250.0, 260.0)},
        {"ExpectedText", physValRectCheckmark.topLeft().toString()}
    });
    pTestStep->addDataRow({
        {"GraphObjName", "Checkmark"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "Top", "Checkmark")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"setPos", QPointF(340.0, 300.0)},
        {"ExpectedText", physValRectCheckmark.angle().toString()}
    });
    pTestStep->addDataRow({
        {"GraphObjName", "SmallRect"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "Top", "SmallRect")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"setPos", QPointF(305.0, 255.0)},
        {"ExpectedText", physValRectSmallRect.topLeft().toString()}
    });
    pTestStep->addDataRow({
        {"GraphObjName", "SmallRect"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "Top", "SmallRect")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"setPos", QPointF(350.0, 270.0)},
        {"ExpectedText", physValRectSmallRect.angle().toString()}
    });
#endif

    // Modify SmallRect
    //=================

    ZS::Test::CTestStepGroup* pGrpModifySmallRect = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Small Rect Lines",
        /* pTSGrpParent */ pGrpModifyShapes );

    // Rotate SmallRect
    //-----------------
#if 0
    physValAngleSmallRect.setVal(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameSmallRect + ".setRotationAngle(" + physValAngleSmallRect.toString() + ")",
        /* strOperation    */ c_strGraphObjNameSmallRect + ".setRotationAngle(" + physValAngleSmallRect.toString() + ")",
        /* pGrpParent      */ pGrpModifySmallRect,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallRect);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallRect));
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", physValAngleSmallRect.toString());
    // Group
    QPointF ptPosSmallRect(25.0, -25.0);
    QSizeF sizeSmallRect(10.0, 10.0);
    QRectF rectBoundingSmallRect(QPointF(-5.0, -5.0), sizeSmallRect);
    CPhysValRect physValRectSmallRect(*m_pDrawingScene, QPointF(70.0, 20.0), sizeSmallRect);
    physValRectSmallRect.setAngle(physValAngleSmallRect);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".pos {" + qPoint2Str(ptPosSmallRect) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".boundingRect {" + qRect2Str(rectBoundingSmallRect) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".position {" + physValRectSmallRect.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".getRect {" + physValRectSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".getSize {" + physValRectSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".rotationAngle: " + physValAngleSmallRect.toString());
    // SmallRect-TopLine
    QPointF ptP1SmallRectTopLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectTopLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectTopLine(0.0, -5.0);
    QRectF rectBoundingSmallRectTopLine(QPointF(-5.0, 0.0), QSizeF(10.0, 0.0));
    double fLengthSmallRectTopLine = 10.0;
    CPhysValLine physValLineSmallRectTopLine(*m_pDrawingScene, QPointF(0.0, 0.0), QPointF(10.0, 0.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallRectTopLine + ".pos {" + qPoint2Str(ptPosSmallRectTopLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallRectTopLine + ".boundingRect {" + qRect2Str(rectBoundingSmallRectTopLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallRectTopLine + ".position {" + physValLineSmallRectTopLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRectTopLine + ".getLine {" + physValLineSmallRectTopLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRectTopLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectTopLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRectTopLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallRect-RightLine
    QPointF ptP1SmallRectRightLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectRightLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectRightLine(5.0, 0.0);
    QRectF rectBoundingSmallRectRightLine(QPointF(0.0, -5.0), QSizeF(0.0, 10.0));
    double fLengthSmallRectRightLine = 10.0;
    CPhysValLine physValLineSmallRectRightLine(*m_pDrawingScene, QPointF(10.0, 0.0), QPointF(10.0, 10.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallRectRightLine + ".pos {" + qPoint2Str(ptPosSmallRectRightLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallRectRightLine + ".boundingRect {" + qRect2Str(rectBoundingSmallRectRightLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallRectRightLine + ".position {" + physValLineSmallRectRightLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRectRightLine + ".getLine {" + physValLineSmallRectRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRectRightLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectRightLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRectRightLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallRect-BottomLine
    QPointF ptP1SmallRectBottomLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectBottomLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectBottomLine(0.0, 5.0);
    QRectF rectBoundingSmallRectBottomLine(QPointF(-5.0, 0.0), QSizeF(10.0, 0.0));
    double fLengthSmallRectBottomLine = 10.0;
    CPhysValLine physValLineSmallRectBottomLine(*m_pDrawingScene, QPointF(10.0, 10.0), QPointF(0.0, 10.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallRectBottomLine + ".pos {" + qPoint2Str(ptPosSmallRectBottomLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallRectBottomLine + ".boundingRect {" + qRect2Str(rectBoundingSmallRectBottomLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallRectBottomLine + ".position {" + physValLineSmallRectBottomLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRectBottomLine + ".getLine {" + physValLineSmallRectBottomLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRectBottomLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectBottomLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRectBottomLine + ".rotationAngle: 180.0 " + Math::c_strSymbolDegree);
    // SmallRect-LeftLine
    QPointF ptP1SmallRectLeftLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectLeftLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectLeftLine(-5.0, 0.0);
    QRectF rectBoundingSmallRectLeftLine(QPointF(0.0, -5.0), QSizeF(0.0, 10.0));
    double fLengthSmallRectLeftLine = 10.0;
    CPhysValLine physValLineSmallRectLeftLine(*m_pDrawingScene, QPointF(0.0, 10.0), QPointF(0.0, 0.0));
    strlstExpectedValues.append(c_strGraphObjNameSmallRectLeftLine + ".pos {" + qPoint2Str(ptPosSmallRectLeftLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallRectLeftLine + ".boundingRect {" + qRect2Str(rectBoundingSmallRectLeftLine) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallRectLeftLine + ".position {" + physValLineSmallRectLeftLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRectLeftLine + ".getLine {" + physValLineSmallRectLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRectLeftLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectLeftLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRectLeftLine + ".rotationAngle: 270.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Hide Geometry Labels
    //---------------------
#if 0
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " hideGeometryLabels",
        /* strOperation    */ "hideGeometryLabels()",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepHideGeometryLabels(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"GraphObjName", "Top"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "Top")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"ExpectedValue", "Top." + CGraphObj::c_strGeometryLabelNameTopLeft + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", "Top"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "Top")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"ExpectedValue", "Top." + CGraphObj::c_strGeometryLabelNameAngle + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", "Checkmark"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "Top", "Checkmark")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"ExpectedValue", "Checkmark." + CGraphObj::c_strGeometryLabelNameTopLeft + " not found"},
    });
    pTestStep->addDataRow({
        {"GraphObjName", "Checkmark"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "Top", "Checkmark")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"ExpectedValue", "Checkmark." + CGraphObj::c_strGeometryLabelNameAngle + " not found"},
    });
    pTestStep->addDataRow({
        {"GraphObjName", "SmallRect"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "Top", "SmallRect")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"ExpectedValue", "SmallRect." + CGraphObj::c_strGeometryLabelNameTopLeft + " not found"},
    });
    pTestStep->addDataRow({
        {"GraphObjName", "SmallRect"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "Top", "SmallRect")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"ExpectedValue", "SmallRect." + CGraphObj::c_strGeometryLabelNameAngle + " not found"},
    });
#endif

    // Modify Top Group
    //=================

    ZS::Test::CTestStepGroup* pGrpModifyTopGroup = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Top Group",
        /* pTSGrpParent */ pGrpModifyShapes );

    // Resize top group
    //-----------------
#if 0
    QPointF ptTLGroupTop(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QSizeF sizeGroupTop(200.0, 200.0);
    QPoint ptBR(450.0, bYAxisTopDown ? 450.0 : 150.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBR) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBR) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyTopGroup,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBR);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    // Group Top
    QPointF ptPosGroupTop(350.0, 350.0);
    QRectF rectBoundingGroupTop(QPointF(-100.0, -100.0), sizeGroupTop);
    CPhysValRect physValRectGroupTop(*m_pDrawingScene, ptTLGroupTop, sizeGroupTop);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(c_strGraphObjNameTopGroup + ".pos {" + qPoint2Str(ptPosGroupTop) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameTopGroup + ".boundingRect {" + qRect2Str(rectBoundingGroupTop) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameTopGroup + ".position {" + physValRectGroupTop.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameTopGroup + ".getRect {" + physValRectGroupTop.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameTopGroup + ".getSize {" + physValRectGroupTop.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameTopGroup + ".rotationAngle: " + physValAngleGroupTop.toString());
    // Checkmark
    ptPosCheckmark = QPointF(0.0, 0.0);
    ptTLCheckmark = QPointF(50.0, 50.0);
    sizeCheckmark = QSizeF(100.0, 100.0);
    rectBoundingCheckmark = QRectF(QPointF(-50.0, -50.0), sizeCheckmark);
    physValRectCheckmark = CPhysValRect(*m_pDrawingScene, ptTLCheckmark, sizeCheckmark);
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".pos {" + qPoint2Str(ptPosCheckmark) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".boundingRect {" + qRect2Str(rectBoundingCheckmark) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".position {" + physValRectCheckmark.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".getRect {" + physValRectCheckmark.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".getSize {" + physValRectCheckmark.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallRect
    ptPosSmallRect = QPointF(50.0, -50.0);
    QPointF ptTLSmallRect(140.0, 40.0);
    sizeSmallRect = QSizeF(20.0, 20.0);
    rectBoundingSmallRect = QRectF(QPointF(-10.0, -10.0), sizeSmallRect);
    physValRectSmallRect = CPhysValRect(*m_pDrawingScene, ptTLSmallRect, sizeSmallRect);
    physValRectSmallRect.setAngle(45.0);
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".pos {" + qPoint2Str(ptPosSmallRect) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".boundingRect {" + qRect2Str(rectBoundingSmallRect) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".position {" + physValRectSmallRect.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".getRect {" + physValRectSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".getSize {" + physValRectSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".rotationAngle: 45.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign
    ptPosSmallPlusSign = QPointF(-50.0, 50.0);
    ptTLSmallPlusSign = QPointF(20.0, 120.0);
    sizeSmallPlusSign = QSizeF(60.0, 60.0);
    rectBoundingSmallPlusSign = QRectF(QPointF(-30.0, -30.0), sizeSmallPlusSign);
    physValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, sizeSmallPlusSign);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".pos {" + qPoint2Str(ptPosSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".position {" + physValRectSmallPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getRect {" + physValRectSmallPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getSize {" + physValRectSmallPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign
    ptPosBigPlusSign = QPointF(0.0, 0.0);
    ptTLBigPlusSign = QPointF(0.0, 0.0);
    sizeBigPlusSign = QSizeF(200.0, 200.0);
    rectBoundingBigPlusSign = QRectF(QPointF(-100.0, -100.0), sizeBigPlusSign);
    physValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, sizeBigPlusSign);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".pos {" + qPoint2Str(ptPosBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".position {" + physValRectBigPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getRect {" + physValRectBigPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getSize {" + physValRectBigPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Rotate top group
    //-----------------
#if 0
    physValAngleGroupTop.setVal(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameTopGroup + ".setRotationAngle(" + physValAngleGroupTop.toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setRotationAngle(" + physValAngleGroupTop.toString() + ")",
        /* pGrpParent      */ pGrpModifyTopGroup,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", physValAngleGroupTop.toString());
    // Group Top
    ptPosGroupTop = QPointF(350.0, 350.0);
    rectBoundingGroupTop = QRectF(QPointF(-100.0, -100.0), sizeGroupTop);
    physValRectGroupTop = CPhysValRect(*m_pDrawingScene, ptTLGroupTop, sizeGroupTop);
    physValRectGroupTop.setAngle(physValAngleGroupTop);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(c_strGraphObjNameTopGroup + ".pos {" + qPoint2Str(ptPosGroupTop) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameTopGroup + ".boundingRect {" + qRect2Str(rectBoundingGroupTop) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameTopGroup + ".position {" + physValRectGroupTop.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameTopGroup + ".getRect {" + physValRectGroupTop.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameTopGroup + ".getSize {" + physValRectGroupTop.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameTopGroup + ".rotationAngle: " + physValAngleGroupTop.toString());
    // Checkmark
    ptPosCheckmark = QPointF(0.0, 0.0);
    ptTLCheckmark = QPointF(50.0, 50.0);
    sizeCheckmark = QSizeF(100.0, 100.0);
    rectBoundingCheckmark = QRectF(QPointF(-50.0, -50.0), sizeCheckmark);
    physValRectCheckmark = CPhysValRect(*m_pDrawingScene, ptTLCheckmark, sizeCheckmark);
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".pos {" + qPoint2Str(ptPosCheckmark) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".boundingRect {" + qRect2Str(rectBoundingCheckmark) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".position {" + physValRectCheckmark.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".getRect {" + physValRectCheckmark.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".getSize {" + physValRectCheckmark.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameCheckmark + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallRect
    ptPosSmallRect = QPointF(50.0, -50.0);
    ptTLSmallRect = QPointF(140.0, 40.0);
    sizeSmallRect = QSizeF(20.0, 20.0);
    rectBoundingSmallRect = QRectF(QPointF(-10.0, -10.0), sizeSmallRect);
    physValRectSmallRect = CPhysValRect(*m_pDrawingScene, ptTLSmallRect, sizeSmallRect);
    physValRectSmallRect.setAngle(45.0);
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".pos {" + qPoint2Str(ptPosSmallRect) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".boundingRect {" + qRect2Str(rectBoundingSmallRect) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".position {" + physValRectSmallRect.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".getRect {" + physValRectSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".getSize {" + physValRectSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallRect + ".rotationAngle: 45.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign
    ptPosSmallPlusSign = QPointF(-50.0, 50.0);
    ptTLSmallPlusSign = QPointF(20.0, 120.0);
    sizeSmallPlusSign = QSizeF(60.0, 60.0);
    rectBoundingSmallPlusSign = QRectF(QPointF(-30.0, -30.0), sizeSmallPlusSign);
    physValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, sizeSmallPlusSign);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".pos {" + qPoint2Str(ptPosSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".position {" + physValRectSmallPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getRect {" + physValRectSmallPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".getSize {" + physValRectSmallPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameSmallPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign
    ptPosBigPlusSign = QPointF(0.0, 0.0);
    ptTLBigPlusSign = QPointF(0.0, 0.0);
    sizeBigPlusSign = QSizeF(200.0, 200.0);
    rectBoundingBigPlusSign = QRectF(QPointF(-100.0, -100.0), sizeBigPlusSign);
    physValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, sizeBigPlusSign);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".pos {" + qPoint2Str(ptPosBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSign) + "} px");
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".position {" + physValRectBigPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getRect {" + physValRectBigPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".getSize {" + physValRectBigPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(c_strGraphObjNameBigPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // Resize top group (setWidth)
    //----------------------------
#if 0
    CPhysValSize physValSizeGroupTop(*m_pDrawingScene, 300.0, 200.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameTopGroup + ".setWidth(" + physValSizeGroupTop.width().toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setWidth(" + physValSizeGroupTop.width().toString() + ")",
        /* pGrpParent      */ pGrpModifyTopGroup,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "setWidth");
    pTestStep->setConfigValue("Width", physValSizeGroupTop.width().toString());
    strlstExpectedValues.clear();
    // Group Top
    ptPosGroupTop = QPointF(385.4, 385.4);
    ptTLGroupTop = QPointF(235.4, 285.4);
    sizeGroupTop = QSizeF(300.0, 200.0);
    rectBoundingGroupTop = QRectF(QPointF(-150.0, -100.0), sizeGroupTop);
    physValRectGroupTop = CPhysValRect(*m_pDrawingScene, ptTLGroupTop, sizeGroupTop);
    physValRectGroupTop.setAngle(physValAngleGroupTop);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, ptPosGroupTop, rectBoundingGroupTop, physValRectGroupTop));
    // Checkmark
    ptPosCheckmark = QPointF(0.0, 0.0);
    ptTLCheckmark = QPointF(75.0, 50.0);
    sizeCheckmark = QSizeF(150.0, 100.0);
    rectBoundingCheckmark = QRectF(QPointF(-75.0, -50.0), sizeCheckmark);
    physValRectCheckmark = CPhysValRect(*m_pDrawingScene, ptTLCheckmark, sizeCheckmark);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, ptPosCheckmark, rectBoundingCheckmark, physValRectCheckmark));
    // SmallRect
    ptPosSmallRect = QPointF(75.0, -50.0);
    ptTLSmallRect = QPointF(210.0, 40.0);
    sizeSmallRect = QSizeF(30.0, 20.0);
    rectBoundingSmallRect = QRectF(QPointF(-15.0, -10.0), sizeSmallRect);
    physValRectSmallRect = CPhysValRect(*m_pDrawingScene, ptTLSmallRect, sizeSmallRect);
    physValRectSmallRect.setAngle(physValAngleSmallRect);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallRect, ptPosSmallRect, rectBoundingSmallRect, physValRectSmallRect));
    // SmallPlusSign
    ptPosSmallPlusSign = QPointF(-75.0, 50.0);
    ptTLSmallPlusSign = QPointF(30.0, 120.0);
    sizeSmallPlusSign = QSizeF(90.0, 60.0);
    rectBoundingSmallPlusSign = QRectF(QPointF(-45.0, -30.0), sizeSmallPlusSign);
    physValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, sizeSmallPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, ptPosSmallPlusSign, rectBoundingSmallPlusSign, physValRectSmallPlusSign));
    // BigPlusSign
    ptPosBigPlusSign = QPointF(0.0, 0.0);
    ptTLBigPlusSign = QPointF(0.0, 0.0);
    sizeBigPlusSign = QSizeF(300.0, 200.0);
    rectBoundingBigPlusSign = QRectF(QPointF(-150.0, -100.0), sizeBigPlusSign);
    physValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, sizeBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, ptPosBigPlusSign, rectBoundingBigPlusSign, physValRectBigPlusSign));
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpModifyShapes->path());
    }
    return pGrpModifyShapes;
}
