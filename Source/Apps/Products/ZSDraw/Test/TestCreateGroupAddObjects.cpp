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
    // TopGroup
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

    ZS::Test::CTestStepGroup* pGrpModifyCheckmarkLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Checkmark Lines",
        /* pTSGrpParent */ pGrpModifyShapes );

    // Left Line
    //----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameCheckmarkLeftLine + ".setLine()",
        /* strOperation    */ c_strGraphObjNameCheckmarkLeftLine + ".setLine()",
        /* pGrpParent      */ pGrpModifyCheckmarkLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1CheckmarkLeftLine(25.0, bYAxisTopDown ? 25.0 : 75.0);
    QPointF ptP2CheckmarkLeftLine(60.0, bYAxisTopDown ? 75.0 : 25.0);
    m_ptPosCheckmarkLeftLine = QPointF(-7.5, 0.0);
    m_lineCheckmarkLeftLine = QLineF(QPointF(-17.5, -25.0), QPointF(17.5, 25.0));
    *m_pPhysValLineCheckmarkLeftLine = CPhysValLine(*m_pDrawingScene, ptP1CheckmarkLeftLine, ptP2CheckmarkLeftLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameCheckmarkLeftLine);
    pTestStep->setConfigValue("GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameCheckmark, c_strGraphObjNameCheckmarkLeftLine));
    pTestStep->setConfigValue("Method", "setLine");
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
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameCheckmarkRightLine + ".setLine()",
        /* strOperation    */ c_strGraphObjNameCheckmarkRightLine + ".setLine()",
        /* pGrpParent      */ pGrpModifyCheckmarkLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjLine(ZS::Test::CTestStep*)) );
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
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameCheckmark + ".resizeToContent()",
        /* strOperation    */ c_strGraphObjNameCheckmark + ".resizeToContent()",
        /* pGrpParent      */ pGrpModifyCheckmarkLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameCheckmark);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameCheckmark));
    pTestStep->setConfigValue("Method", "resizeToContent");
    strlstExpectedValues.clear();
    // Group
    m_ptPosCheckmark = QPointF(0.0, 0.0);
    QPointF ptTLCheckmark(25.0, bYAxisTopDown ? 25.0 : 75.0);
    m_sizeCheckmark = QSizeF(50.0, 50.0);
    *m_pPhysValRectCheckmark = CPhysValRect(*m_pDrawingScene, ptTLCheckmark, m_sizeCheckmark);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // Checkmark-LeftLine
    m_ptPosCheckmarkLeftLine = QPointF(-7.5, 0.0);
    m_lineCheckmarkLeftLine = QLineF(QPointF(-17.5, -25.0), QPointF(17.5, 25.0));
    *m_pPhysValLineCheckmarkLeftLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 0.0), QPointF(35.0, 50.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkLeftLine, m_ptPosCheckmarkLeftLine,
        m_lineCheckmarkLeftLine, *m_pPhysValLineCheckmarkLeftLine));
    // Checkmark-RightLine
    m_ptPosCheckmarkRightLine = QPointF(17.5, 17.5);
    m_lineCheckmarkRightLine = QLineF(QPointF(-7.5, 7.5), QPointF(7.5, -7.5));
    *m_pPhysValLineCheckmarkRightLine = CPhysValLine(*m_pDrawingScene, QPointF(35.0, 50.0), QPointF(50.0, 35.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameCheckmarkRightLine, m_ptPosCheckmarkRightLine,
        m_lineCheckmarkRightLine, *m_pPhysValLineCheckmarkRightLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Modify PlusSigns
    //=================

    ZS::Test::CTestStepGroup* pGrpModifyPlusSigns = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Plus Sign Lines",
        /* pTSGrpParent */ pGrpModifyShapes );

    // Remove SmallPlusSign from TopGroup
    //-----------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameTopGroup + ".removeFromGroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".removeFromGroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("GraphObjChildName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("GraphObjChildKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameSmallPlusSign));
    pTestStep->setConfigValue("Method", "removeFromGroup");
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

    // Remove BigPlusSign from TopGroup
    //-----------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameTopGroup + ".removeFromGroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".removeFromGroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("GraphObjChildName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjChildKeyInTree", pIdxTree->buildKeyInTreeStr(
        strEntryType, c_strGraphObjNameTopGroup, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("Method", "removeFromGroup");
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

    // Ungroup SmallPlusSign
    //----------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " DrawingScene.ungroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
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
        /* strName         */ "Step " + QString::number(++idxStep) + " DrawingScene.ungroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
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

    // Group Plus Signs
    //-----------------

    const QString strGraphObjGroupNamePlusSigns = "PlusSigns";
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
    strlstExpectedValues.clear();
    // Group PlusSigns
    QPointF ptPosPlusSigns(300.0, 300.0);
    QPointF ptTLPlusSigns(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QSizeF sizePlusSigns(100.0, 100.0);
    CPhysValRect physValRectPlusSigns(*m_pDrawingScene, ptTLPlusSigns, sizePlusSigns);
    strlstExpectedValues.append(resultValuesForGroup(
        strGraphObjGroupNamePlusSigns, ptPosPlusSigns, physValRectPlusSigns));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(-25.0, 25.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -15.0), QPointF(0.0, 15.0));
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(25.0, 60.0), QPointF(25.0, 90.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(-25.0, 25.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(10.0, 75.0), QPointF(40.0, 75.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
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

    // Resize PlusSigns
    //-----------------

    ptTLPlusSigns = QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0);
    sizePlusSigns = QSizeF(400.0, 200.0);
    QPointF ptBRPlusSigns(650.0, bYAxisTopDown ? 450.0 : 150.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjGroupNamePlusSigns + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* strOperation    */ strGraphObjGroupNamePlusSigns + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", strGraphObjGroupNamePlusSigns);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNamePlusSigns));
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBRPlusSigns);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    // Group PlusSigns
    ptPosPlusSigns = QPointF(450.0, 350.0);
    physValRectPlusSigns = CPhysValRect(*m_pDrawingScene, ptTLPlusSigns, sizePlusSigns);
    strlstExpectedValues.append(resultValuesForGroup(
        strGraphObjGroupNamePlusSigns, ptPosPlusSigns, physValRectPlusSigns));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(-100.0, 50.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -30.0), QPointF(0.0, 30.0));
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(100.0, 120.0), QPointF(100.0, 180.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(-100.0, 50.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-60.0, 0.0), QPointF(60.0, 0.0));
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(40.0, 150.0), QPointF(160.0, 150.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -100.0), QPointF(0.0, 100.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(200.0, 0.0), QPointF(200.0, 200.0));
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
        /* strName         */ "Step " + QString::number(++idxStep) + " DrawingScene.ungroup(" + strGraphObjGroupNamePlusSigns + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + strGraphObjGroupNamePlusSigns + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", strGraphObjGroupNamePlusSigns);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNamePlusSigns));
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
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(350.0, 370.0), QPointF(350.0, 430.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignVerticalLine, m_ptPosSmallPlusSignVerticalLine,
        m_lineSmallPlusSignVerticalLine, *m_pPhysValLineSmallPlusSignVerticalLine));
    // SmallPlusSign-HorizontalLine
    m_ptPosSmallPlusSignHorizontalLine = QPointF(350.0, 400.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-60.0, 0.0), QPointF(60.0, 0.0));
    *m_pPhysValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(290.0, 400.0), QPointF(410.0, 400.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameSmallPlusSignHorizontalLine, m_ptPosSmallPlusSignHorizontalLine,
        m_lineSmallPlusSignHorizontalLine, *m_pPhysValLineSmallPlusSignHorizontalLine));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(450.0, 350.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -100.0), QPointF(0.0, 100.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(450.0, 250.0), QPointF(450.0, 450.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignVerticalLine, m_ptPosBigPlusSignVerticalLine,
        m_lineBigPlusSignVerticalLine, *m_pPhysValLineBigPlusSignVerticalLine));
    // BigPlusSign-HorizontalLine
    m_ptPosBigPlusSignHorizontalLine = QPointF(450.0, 350.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-200.0, 0.0), QPointF(200.0, 0.0));
    *m_pPhysValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(250.0, 350.0), QPointF(650.0, 350.0));
    strlstExpectedValues.append(resultValuesForLine(
        c_strGraphObjNameBigPlusSignHorizontalLine, m_ptPosBigPlusSignHorizontalLine,
        m_lineBigPlusSignHorizontalLine, *m_pPhysValLineBigPlusSignHorizontalLine));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Small Plus Sign
    //----------------

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
    strlstExpectedValues.clear();
    // Group SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(350.0, 400.0);
    ptTLSmallPlusSign = QPointF(290.0, bYAxisTopDown ? 370.0 : 230.0);
    m_sizeSmallPlusSign = QSizeF(120.0, 60.0);
    *m_pPhysValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, m_sizeSmallPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // SmallPlusSign-VerticalLine
    m_ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -30.0), QPointF(0.0, 30.0));
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(60.0, 0.0), QPointF(60.0, 60.0));
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
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjTypeGroup + ", " + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(c_strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    strlstExpectedValues.clear();
    // Group BigPlusSign
    m_ptPosBigPlusSign = QPointF(450.0, 350.0);
    ptTLBigPlusSign = QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0);
    m_sizeBigPlusSign = QSizeF(400.0, 200.0);
    *m_pPhysValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, m_sizeBigPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    // BigPlusSign-VerticalLine
    m_ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -100.0), QPointF(0.0, 100.0));
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(200.0, 0.0), QPointF(200.0, 200.0));
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
    strlstExpectedValues.clear();
    // Group PlusSigns
    ptPosPlusSigns = QPointF(450.0, 350.0);
    ptTLPlusSigns = QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0);
    sizePlusSigns = QSizeF(400.0, 200.0);
    physValRectPlusSigns = CPhysValRect(*m_pDrawingScene, ptTLPlusSigns, sizePlusSigns);
    strlstExpectedValues.append(resultValuesForGroup(
        strGraphObjGroupNamePlusSigns, ptPosPlusSigns, physValRectPlusSigns));
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
    *m_pPhysValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(60.0, 0.0), QPointF(60.0, 60.0));
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
    *m_pPhysValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(200.0, 0.0), QPointF(200.0, 200.0));
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

    ptTLPlusSigns = QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0);
    sizePlusSigns = QSizeF(100.0, 100.0);
    ptBRPlusSigns = QPointF(350.0, bYAxisTopDown ? 350.0 : 250.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjGroupNamePlusSigns + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* strOperation    */ strGraphObjGroupNamePlusSigns + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", strGraphObjGroupNamePlusSigns);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNamePlusSigns));
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
    strlstExpectedValues.clear();
    // Group PlusSigns
    ptPosPlusSigns = QPointF(300.0, 300.0);
    physValRectPlusSigns = CPhysValRect(*m_pDrawingScene, ptTLPlusSigns, sizePlusSigns);
    strlstExpectedValues.append(resultValuesForGroup(
        strGraphObjGroupNamePlusSigns, ptPosPlusSigns, physValRectPlusSigns));
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

    // Ungroup PlusSigns
    //------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " DrawingScene.ungroup(" + strGraphObjGroupNamePlusSigns + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + strGraphObjGroupNamePlusSigns + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", strGraphObjGroupNamePlusSigns);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNamePlusSigns));
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
    m_ptPosSmallPlusSign = QPointF(275.0, 325.0);
    ptTLSmallPlusSign = QPointF(260.0, bYAxisTopDown ? 310.0 : 290.0);
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
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(300.0, 300.0);
    ptTLBigPlusSign = QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0);
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

    // Move SmallPlusSign to Top Group
    //--------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameTopGroup + ".addToGroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".addToGroup(" + c_strGraphObjNameSmallPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("GraphObjChildName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("GraphObjChildKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSign));
    pTestStep->setConfigValue("Method", "addToGroup");
    strlstExpectedValues.clear();
    // TopGroup
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // Checkmark
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // SmallRect
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallRect, m_ptPosSmallRect, *m_pPhysValRectSmallRect));
    // Moved SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(-25.0, 25.0);
    ptTLSmallPlusSign = QPointF(10.0, bYAxisTopDown ? 60.0 : 40.0);
    *m_pPhysValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, m_sizeSmallPlusSign);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Show Labels
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameBigPlusSign + ".showLabel(Name)",
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
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameBigPlusSign + ".showGeometryLabels(TopLeft, Angle)",
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

    m_physValAngleBigPlusSign.setVal(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
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

    // Show Geometry Labels
    //---------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameBigPlusSign + ".showGeometryLabels(TopLeft, Angle)",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".showGeometryLabels(TopLeft, Angle)",
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

    m_physValAngleBigPlusSign.setVal(135.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* strOperation    */ c_strGraphObjNameBigPlusSign + ".setRotationAngle(" + m_physValAngleBigPlusSign.toString() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
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

    // Move BigPlusSign to Top Group
    //--------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameTopGroup + ".addToGroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".addToGroup(" + c_strGraphObjNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("GraphObjChildName", c_strGraphObjNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjChildKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSign));
    pTestStep->setConfigValue("Method", "addToGroup");
    strlstExpectedValues.clear();
    // TopGroup
    m_sizeTopGroup = QSizeF(Math::sqrt(2.0) * 100.0, Math::sqrt(2.0) * 100.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(QPointF(300.0, 300.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // Checkmark
    m_pPhysValRectCheckmark->setCenter(QPointF(71.0, 71.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // SmallRect
    m_pPhysValRectSmallRect->setCenter(QPointF(96.0, 46.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallRect, m_ptPosSmallRect, *m_pPhysValRectSmallRect));
    // SmallPlusSign
    m_pPhysValRectSmallPlusSign->setCenter(QPointF(46.0, 96.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // Moved BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(71.0, 71.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Modify SmallRect
    //=================

    ZS::Test::CTestStepGroup* pGrpModifySmallRect = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Small Rect Lines",
        /* pTSGrpParent */ pGrpModifyShapes );

    // Rotate SmallRect
    //-----------------

    m_physValAngleSmallRect.setVal(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameSmallRect + ".setRotationAngle(" + m_physValAngleSmallRect.toString() + ")",
        /* strOperation    */ c_strGraphObjNameSmallRect + ".setRotationAngle(" + m_physValAngleSmallRect.toString() + ")",
        /* pGrpParent      */ pGrpModifySmallRect,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
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
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Top Group",
        /* pTSGrpParent */ pGrpModifyShapes );

    // Resize top group
    //-----------------

    QPoint ptBRTopGroup(450.0, bYAxisTopDown ? 450.0 : 150.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRTopGroup) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setBottomRight(" + qPoint2Str(ptBRTopGroup) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyTopGroup,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBRTopGroup);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    // Group Top
    m_ptPosTopGroup = QPointF(339.6, 339.6);
    m_sizeTopGroup = QSizeF(221.0, 221.0);
    m_pPhysValRectTopGroup->setSize(m_sizeTopGroup);
    m_pPhysValRectTopGroup->setCenter(QPointF(339.6, 339.6));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // Checkmark
    m_ptPosCheckmark = QPointF(0.0, 0.0);
    m_sizeCheckmark = QSizeF(78.0, 78.0);
    m_pPhysValRectCheckmark->setSize(m_sizeCheckmark);
    m_pPhysValRectCheckmark->setCenter(QPointF(110.0, 110.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // SmallRect
    m_ptPosSmallRect = QPointF(39.0, -39.0);
    m_sizeSmallRect = QSizeF(16.0, 16.0);
    m_pPhysValRectSmallRect->setSize(m_sizeSmallRect);
    m_pPhysValRectSmallRect->setCenter(QPointF(149.0, 71.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallRect, m_ptPosSmallRect, *m_pPhysValRectSmallRect));
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(-39.0, 39.0);
    m_sizeSmallPlusSign = QSizeF(47.0, 47.0);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    m_pPhysValRectSmallPlusSign->setCenter(QPointF(71.0, 149.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = QSizeF(156.0, 156.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(110.0, 110.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate top group
    //-----------------

    m_physValAngleTopGroup.setVal(45.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameTopGroup + ".setRotationAngle(" + m_physValAngleTopGroup.toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setRotationAngle(" + m_physValAngleTopGroup.toString() + ")",
        /* pGrpParent      */ pGrpModifyTopGroup,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", m_physValAngleTopGroup.toString());
    strlstExpectedValues.clear();
    // Group Top
    m_pPhysValRectTopGroup->setAngle(m_physValAngleTopGroup);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // Checkmark
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // SmallRect
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallRect, m_ptPosSmallRect, *m_pPhysValRectSmallRect));
    // SmallPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // BigPlusSign
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize top group (setWidth)
    //----------------------------

    m_sizeTopGroup.setWidth(300.0);
    m_pPhysValRectTopGroup->setWidth(300.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + c_strGraphObjNameTopGroup + ".setWidth(" + m_pPhysValRectTopGroup->width().toString() + ")",
        /* strOperation    */ c_strGraphObjNameTopGroup + ".setWidth(" + m_pPhysValRectTopGroup->width().toString() + ")",
        /* pGrpParent      */ pGrpModifyTopGroup,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", c_strGraphObjNameTopGroup);
    pTestStep->setConfigValue("GroupKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameTopGroup));
    pTestStep->setConfigValue("Method", "setWidth");
    pTestStep->setConfigValue("Width", m_pPhysValRectTopGroup->width().toString());
    strlstExpectedValues.clear();
    // Group Top
    m_ptPosTopGroup = QPointF(367.4, 367.7);
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameTopGroup, m_ptPosTopGroup, *m_pPhysValRectTopGroup));
    // Checkmark
    m_ptPosCheckmark = QPointF(0.0, 0.0);
    m_sizeCheckmark = QSizeF(150.0, 100.0);
    m_pPhysValRectCheckmark->setSize(m_sizeCheckmark);
    m_pPhysValRectCheckmark->setCenter(QPointF(150.0, 110.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameCheckmark, m_ptPosCheckmark, *m_pPhysValRectCheckmark));
    // SmallRect
    m_ptPosSmallRect = QPointF(75.0, -50.0);
    m_sizeSmallRect = QSizeF(30.0, 20.0);
    m_pPhysValRectSmallRect->setSize(m_sizeSmallRect);
    m_pPhysValRectSmallRect->setCenter(QPointF(203.0, 71.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallRect, m_ptPosSmallRect, *m_pPhysValRectSmallRect));
    // SmallPlusSign
    m_ptPosSmallPlusSign = QPointF(-75.0, 50.0);
    m_sizeSmallPlusSign = QSizeF(90.0, 60.0);
    m_pPhysValRectSmallPlusSign->setSize(m_sizeSmallPlusSign);
    m_pPhysValRectSmallPlusSign->setCenter(QPointF(97.0, 149.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameSmallPlusSign, m_ptPosSmallPlusSign, *m_pPhysValRectSmallPlusSign));
    // BigPlusSign
    m_ptPosBigPlusSign = QPointF(0.0, 0.0);
    m_sizeBigPlusSign = QSizeF(300.0, 200.0);
    m_pPhysValRectBigPlusSign->setSize(m_sizeBigPlusSign);
    m_pPhysValRectBigPlusSign->setCenter(QPointF(150.0, 110.0));
    strlstExpectedValues.append(resultValuesForGroup(
        c_strGraphObjNameBigPlusSign, m_ptPosBigPlusSign, *m_pPhysValRectBigPlusSign));
    pTestStep->setExpectedValues(strlstExpectedValues);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpModifyShapes->path());
    }
    return pGrpModifyShapes;
}
