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

    QString strGraphObjNameSmallPlusSignVerticalLine = "SmallPlusSign-VerticalLine";
    QString strGraphObjNameSmallPlusSignHorizontalLine = "SmallPlusSign-HorizontalLine";
    QString strGraphObjNameBigPlusSignVerticalLine = "BigPlusSign-VerticalLine";
    QString strGraphObjNameBigPlusSignHorizontalLine = "BigPlusSign-HorizontalLine";
    QString strGraphObjNameCheckmarkLeftLine = "Checkmark-LeftLine";
    QString strGraphObjNameCheckmarkRightLine = "Checkmark-RightLine";
    QString strGraphObjNameSmallRectTopLine = "SmallRect-TopLine";
    QString strGraphObjNameSmallRectRightLine = "SmallRect-RightLine";
    QString strGraphObjNameSmallRectBottomLine = "SmallRect-BottomLine";
    QString strGraphObjNameSmallRectLeftLine = "SmallRect-LeftLine";

    QString strGraphObjKeyInTree;

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
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjNameSmallPlusSignVerticalLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjNameSmallPlusSignVerticalLine + ")",
        /* pGrpParent      */ pGrpAddSmallPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1SmallPlusSignVerticalLine(275.0, bYAxisTopDown ? 310.0 : 290.0);
    QPointF ptP2SmallPlusSignVerticalLine(275.0, bYAxisTopDown ? 340.0 : 260.0);
    QPointF ptPosSmallPlusSignVerticalLine(275.0, 325.0);
    QPointF ptP1PosSmallPlusSignVerticalLine(0.0, -15.0);
    QPointF ptP2PosSmallPlusSignVerticalLine(0.0, 15.0);
    double fLengthSmallPlusSignVerticalLine = 30.0;
    CPhysValLine physValLineSmallPlusSignVerticalLine(*m_pDrawingScene, ptP1SmallPlusSignVerticalLine, ptP2SmallPlusSignVerticalLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjNameSmallPlusSignVerticalLine);
    pTestStep->setConfigValue("P1", ptP1SmallPlusSignVerticalLine);
    pTestStep->setConfigValue("P2", ptP2SmallPlusSignVerticalLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".p1 {" + qPoint2Str(ptP1PosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".p2 {" + qPoint2Str(ptP2PosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".length: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".position {" + qPoint2Str(ptPosSmallPlusSignVerticalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Horizontal Line
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjNameSmallPlusSignHorizontalLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjNameSmallPlusSignHorizontalLine + ")",
        /* pGrpParent      */ pGrpAddSmallPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1SmallPlusSignHorizontalLine(260.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptP2SmallPlusSignHorizontalLine(290.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptPosSmallPlusSignHorizontalLine(275.0, 325.0);
    QPointF ptP1PosSmallPlusSignHorizontalLine(-15.0, 0.0);
    QPointF ptP2PosSmallPlusSignHorizontalLine(15.0, 0.0);
    double fLengthSmallPlusSignHorizontalLine = 30.0;
    CPhysValLine physValLineSmallPlusSignHorizontalLine(*m_pDrawingScene, ptP1SmallPlusSignHorizontalLine, ptP2SmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjNameSmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("P1", ptP1SmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("P2", ptP2SmallPlusSignHorizontalLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".p1 {" + qPoint2Str(ptP1PosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".p2 {" + qPoint2Str(ptP2PosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".length: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
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
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjNameBigPlusSignVerticalLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjNameBigPlusSignVerticalLine + ")",
        /* pGrpParent      */ pGrpAddBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1BigPlusSignVerticalLine(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptP2BigPlusSignVerticalLine(300.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptPosBigPlusSignVerticalLine(300.0, 300.0);
    QPointF ptP1PosBigPlusSignVerticalLine(0.0, -50.0);
    QPointF ptP2PosBigPlusSignVerticalLine(0.0, 50.0);
    double fLengthBigPlusSignVerticalLine = 100.0;
    CPhysValLine physValLineBigPlusSignVerticalLine(*m_pDrawingScene, ptP1BigPlusSignVerticalLine, ptP2BigPlusSignVerticalLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjNameBigPlusSignVerticalLine);
    pTestStep->setConfigValue("P1", ptP1BigPlusSignVerticalLine);
    pTestStep->setConfigValue("P2", ptP2BigPlusSignVerticalLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".p1 {" + qPoint2Str(ptP1PosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".p2 {" + qPoint2Str(ptP2PosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".length: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".position {" + qPoint2Str(ptPosBigPlusSignVerticalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Horizontal Line
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjNameBigPlusSignHorizontalLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjNameBigPlusSignHorizontalLine + ")",
        /* pGrpParent      */ pGrpAddBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1BigPlusSignHorizontalLine(250.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptP2BigPlusSignHorizontalLine(350.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptPosBigPlusSignHorizontalLine(300.0, 300.0);
    QPointF ptP1PosBigPlusSignHorizontalLine(-50.0, 0.0);
    QPointF ptP2PosBigPlusSignHorizontalLine(50.0, 0.0);
    double fLengthBigPlusSignHorizontalLine = 100.0;
    CPhysValLine physValLineBigPlusSignHorizontalLine(*m_pDrawingScene, ptP1BigPlusSignHorizontalLine, ptP2BigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("P1", ptP1BigPlusSignHorizontalLine);
    pTestStep->setConfigValue("P2", ptP2BigPlusSignHorizontalLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".p1 {" + qPoint2Str(ptP1PosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".p2 {" + qPoint2Str(ptP2PosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".length: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".position {" + qPoint2Str(ptPosBigPlusSignHorizontalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
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
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjNameCheckmarkLeftLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjNameCheckmarkLeftLine + ")",
        /* pGrpParent      */ pGrpAddCheckmark,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1CheckmarkLeftLine(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptP2CheckmarkLeftLine(325.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptPosCheckmarkLeftLine(287.5, 300.0);
    QPointF ptP1PosCheckmarkLeftLine(-37.5, -50.0);
    QPointF ptP2PosCheckmarkLeftLine(37.5, 50.0);
    double fLengthCheckmarkLeftLine = 125.0;
    CPhysValLine physValLineCheckmarkLeftLine(*m_pDrawingScene, ptP1CheckmarkLeftLine, ptP2CheckmarkLeftLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjNameCheckmarkLeftLine);
    pTestStep->setConfigValue("P1", ptP1CheckmarkLeftLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkLeftLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".pos {" + qPoint2Str(ptPosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".p1 {" + qPoint2Str(ptP1PosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".p2 {" + qPoint2Str(ptP2PosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".length: " + QString::number(Math::round2Nearest(fLengthCheckmarkLeftLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".position {" + qPoint2Str(ptPosCheckmarkLeftLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".getLine {" + physValLineCheckmarkLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkLeftLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".rotationAngle: 53.1 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Right Line
    //-----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjNameCheckmarkRightLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjNameCheckmarkRightLine + ")",
        /* pGrpParent      */ pGrpAddCheckmark,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1CheckmarkRightLine(325.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptP2CheckmarkRightLine(350.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptPosCheckmarkRightLine(337.5, 337.5);
    QPointF ptP1PosCheckmarkRightLine(-12.5, 12.5);
    QPointF ptP2PosCheckmarkRightLine(12.5, -12.5);
    double fLengthCheckmarkRightLine = 35.4;
    CPhysValLine physValLineCheckmarkRightLine(*m_pDrawingScene, ptP1CheckmarkRightLine, ptP2CheckmarkRightLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjNameCheckmarkRightLine);
    pTestStep->setConfigValue("P1", ptP1CheckmarkRightLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkRightLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".pos {" + qPoint2Str(ptPosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".p1 {" + qPoint2Str(ptP1PosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".p2 {" + qPoint2Str(ptP2PosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".length: " + QString::number(Math::round2Nearest(fLengthCheckmarkRightLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".position {" + qPoint2Str(ptPosCheckmarkRightLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".getLine {" + physValLineCheckmarkRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkRightLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".rotationAngle: 315.0 " + Math::c_strSymbolDegree);
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
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjNameSmallRectTopLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjNameSmallRectTopLine + ")",
        /* pGrpParent      */ pGrpAddSmallRect,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1SmallRectTopLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectTopLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectTopLine(325.0, 270.0);
    QPointF ptP1PosSmallRectTopLine(-5.0, 0.0);
    QPointF ptP2PosSmallRectTopLine(5.0, 0.0);
    double fLengthSmallRectTopLine = 10.0;
    CPhysValLine physValLineSmallRectTopLine(*m_pDrawingScene, ptP1SmallRectTopLine, ptP2SmallRectTopLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjNameSmallRectTopLine);
    pTestStep->setConfigValue("P1", ptP1SmallRectTopLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectTopLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".pos {" + qPoint2Str(ptPosSmallRectTopLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".p1 {" + qPoint2Str(ptP1PosSmallRectTopLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".p2 {" + qPoint2Str(ptP2PosSmallRectTopLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".length: " + QString::number(Math::round2Nearest(fLengthSmallRectTopLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".position {" + qPoint2Str(ptPosSmallRectTopLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".getLine {" + physValLineSmallRectTopLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectTopLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Right Line
    //-----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjNameSmallRectRightLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjNameSmallRectRightLine + ")",
        /* pGrpParent      */ pGrpAddSmallRect,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1SmallRectRightLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectRightLine(330.0, bYAxisTopDown ? 280.0 : 320.0);
    QPointF ptPosSmallRectRightLine(330.0, 275.0);
    QPointF ptP1PosSmallRectRightLine(0.0, -5.0);
    QPointF ptP2PosSmallRectRightLine(0.0, 5.0);
    double fLengthSmallRectRightLine = 10.0;
    CPhysValLine physValLineSmallRectRightLine(*m_pDrawingScene, ptP1SmallRectRightLine, ptP2SmallRectRightLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjNameSmallRectRightLine);
    pTestStep->setConfigValue("P1", ptP1SmallRectRightLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectRightLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".pos {" + qPoint2Str(ptPosSmallRectRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".p1 {" + qPoint2Str(ptP1PosSmallRectRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".p2 {" + qPoint2Str(ptP2PosSmallRectRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".length: " + QString::number(Math::round2Nearest(fLengthSmallRectRightLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".position {" + qPoint2Str(ptPosSmallRectRightLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".getLine {" + physValLineSmallRectRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectRightLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Bottom Line
    //------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjNameSmallRectBottomLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjNameSmallRectBottomLine + ")",
        /* pGrpParent      */ pGrpAddSmallRect,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1SmallRectBottomLine(330.0, bYAxisTopDown ? 280.0 : 320.0);
    QPointF ptP2SmallRectBottomLine(320.0, bYAxisTopDown ? 280.0 : 320.0);
    QPointF ptPosSmallRectBottomLine(325.0, 280.0);
    QPointF ptP1PosSmallRectBottomLine(5.0, 0.0);
    QPointF ptP2PosSmallRectBottomLine(-5.0, 0.0);
    double fLengthSmallRectBottomLine = 10.0;
    CPhysValLine physValLineSmallRectBottomLine(*m_pDrawingScene, ptP1SmallRectBottomLine, ptP2SmallRectBottomLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjNameSmallRectBottomLine);
    pTestStep->setConfigValue("P1", ptP1SmallRectBottomLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectBottomLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".pos {" + qPoint2Str(ptPosSmallRectBottomLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".p1 {" + qPoint2Str(ptP1PosSmallRectBottomLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".p2 {" + qPoint2Str(ptP2PosSmallRectBottomLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".length: " + QString::number(Math::round2Nearest(fLengthSmallRectBottomLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".position {" + qPoint2Str(ptPosSmallRectBottomLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".getLine {" + physValLineSmallRectBottomLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectBottomLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".rotationAngle: 180.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Left Line
    //----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjNameSmallRectLeftLine + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjNameSmallRectLeftLine + ")",
        /* pGrpParent      */ pGrpAddSmallRect,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    QPointF ptP1SmallRectLeftLine(320.0, bYAxisTopDown ? 280.0 : 320.0);
    QPointF ptP2SmallRectLeftLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectLeftLine(320.0, 275.0);
    QPointF ptP1PosSmallRectLeftLine(0.0, 5.0);
    QPointF ptP2PosSmallRectLeftLine(0.0, -5.0);
    double fLengthSmallRectLeftLine = 10.0;
    CPhysValLine physValLineSmallRectLeftLine(*m_pDrawingScene, ptP1SmallRectLeftLine, ptP2SmallRectLeftLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjNameSmallRectLeftLine);
    pTestStep->setConfigValue("P1", ptP1SmallRectLeftLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectLeftLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".pos {" + qPoint2Str(ptPosSmallRectLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".p1 {" + qPoint2Str(ptP1PosSmallRectLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".p2 {" + qPoint2Str(ptP2PosSmallRectLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".length: " + QString::number(Math::round2Nearest(fLengthSmallRectLeftLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".position {" + qPoint2Str(ptPosSmallRectLeftLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".getLine {" + physValLineSmallRectLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectLeftLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".rotationAngle: 270.0 " + Math::c_strSymbolDegree);
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

    QString strGraphObjGroupNameTopGroup = "Top";
    QString strGraphObjGroupNameSmallPlusSign = "SmallPlusSign";
    QString strGraphObjNameSmallPlusSignVerticalLine = "SmallPlusSign-VerticalLine";
    QString strGraphObjNameSmallPlusSignHorizontalLine = "SmallPlusSign-HorizontalLine";
    QString strGraphObjGroupNameBigPlusSign = "BigPlusSign";
    QString strGraphObjNameBigPlusSignVerticalLine = "BigPlusSign-VerticalLine";
    QString strGraphObjNameBigPlusSignHorizontalLine = "BigPlusSign-HorizontalLine";
    QString strGraphObjGroupNameCheckmark = "Checkmark";
    QString strGraphObjNameCheckmarkLeftLine = "Checkmark-LeftLine";
    QString strGraphObjNameCheckmarkRightLine = "Checkmark-RightLine";
    QString strGraphObjGroupNameSmallRect = "SmallRect";
    QString strGraphObjNameSmallRectTopLine = "SmallRect-TopLine";
    QString strGraphObjNameSmallRectRightLine = "SmallRect-RightLine";
    QString strGraphObjNameSmallRectBottomLine = "SmallRect-BottomLine";
    QString strGraphObjNameSmallRectLeftLine = "SmallRect-LeftLine";

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
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjGroupNameSmallPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjGroupNameSmallPlusSign + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(strGraphObjNameSmallPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(strGraphObjNameSmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNameSmallPlusSign);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    // Group
    QPointF ptPosSmallPlusSign(275.0, 325.0);
    QPointF ptTLSmallPlusSign(260.0, bYAxisTopDown ? 310.0 : 290.0);
    QSizeF sizeSmallPlusSign(30.0, 30.0);
    QRectF rectBoundingSmallPlusSign(QPointF(-15.0, -15.0), sizeSmallPlusSign);
    CPhysValRect physValRectSmallPlusSign(*m_pDrawingScene, ptTLSmallPlusSign, sizeSmallPlusSign);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".pos {" + qPoint2Str(ptPosSmallPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".position {" + qPoint2Str(ptPosSmallPlusSign, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".getRect {" + physValRectSmallPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".getSize {" + physValRectSmallPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-VerticalLine
    QPointF ptPosSmallPlusSignVerticalLine = QPointF(0.0, 0.0);
    QRectF rectBoundingSmallPlusSignVerticalLine = QRectF(QPointF(0.0, -15.0), QSizeF(0.0, 30.0));
    double fLengthSmallPlusSignVerticalLine = 30.0;
    CPhysValLine physValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(15.0, 0.0), QPointF(15.0, 30.0));
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    QPointF ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    QRectF rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-15.0, 0.0), QSizeF(30.0, 0.0));
    double fLengthSmallPlusSignHorizontalLine = 30.0;
    CPhysValLine physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 15.0), QPointF(30.0, 15.0));
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Big Plus Sign
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjGroupNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjGroupNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(strGraphObjNameBigPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(strGraphObjNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNameBigPlusSign);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    // Group
    QPointF ptPosBigPlusSign(300.0, 300.0);
    QPointF ptTLBigPlusSign(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QSizeF sizeBigPlusSign(100.0, 100.0);
    QRectF rectBoundingBigPlusSign(QPointF(-50.0, -50.0), sizeBigPlusSign);
    CPhysValRect physValRectBigPlusSign(*m_pDrawingScene, ptTLBigPlusSign, sizeBigPlusSign);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".pos {" + qPoint2Str(ptPosBigPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".position {" + qPoint2Str(ptPosBigPlusSign, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".getRect {" + physValRectBigPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".getSize {" + physValRectBigPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    QPointF ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    QRectF rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -50.0), QSizeF(0.0, 100.0));
    double fLengthBigPlusSignVerticalLine = 100.0;
    CPhysValLine physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, 0.0), QPointF(50.0, 100.0));
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    QPointF ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    QRectF rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-50.0, 0.0), QSizeF(100.0, 0.0));
    double fLengthBigPlusSignHorizontalLine = 100.0;
    CPhysValLine physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Checkmark
    //----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjGroupNameCheckmark + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjGroupNameCheckmark + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(strGraphObjNameCheckmarkLeftLine);
    strlstGraphObjsAddToGroup.append(strGraphObjNameCheckmarkRightLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNameCheckmark);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    // Group
    QPointF ptPosCheckmark(300.0, 300.0);
    QPointF ptTLCheckmark(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QSizeF sizeCheckmark(100.0, 100.0);
    QRectF rectBoundingCheckmark(QPointF(-50.0, -50.0), sizeCheckmark);
    CPhysValRect physValRectCheckmark(*m_pDrawingScene, ptTLCheckmark, sizeCheckmark);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".pos {" + qPoint2Str(ptPosCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".boundingRect {" + qRect2Str(rectBoundingCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".position {" + qPoint2Str(ptPosCheckmark, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".getRect {" + physValRectCheckmark.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".getSize {" + physValRectCheckmark.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // Checkmark-LeftLine
    QPointF ptP1CheckmarkLeftLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2CheckmarkLeftLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosCheckmarkLeftLine(-12.5, 0.0);
    QRectF rectBoundingCheckmarkLeftLine(QPointF(-37.5, -50.0), QSizeF(75.0, 100.0));
    double fLengthCheckmarkLeftLine = 125.0;
    CPhysValLine physValLineCheckmarkLeftLine(*m_pDrawingScene, QPointF(0.0, 0.0), QPointF(75.0, 100.0));
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".pos {" + qPoint2Str(ptPosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".boundingRect {" + qRect2Str(rectBoundingCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".position {" + physValLineCheckmarkLeftLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".getLine {" + physValLineCheckmarkLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkLeftLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".rotationAngle: 53.1 " + Math::c_strSymbolDegree);
    // Checkmark-RightLine
    QPointF ptP1CheckmarkRightLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2CheckmarkRightLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosCheckmarkRightLine(37.5, 37.5);
    QRectF rectBoundingCheckmarkRightLine(QPointF(-12.5, -12.5), QSizeF(25.0, 25.0));
    double fLengthCheckmarkRightLine = 35.0;
    CPhysValLine physValLineCheckmarkRightLine(*m_pDrawingScene, QPointF(75.0, 100.0), QPointF(100.0, 75.0));
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".pos {" + qPoint2Str(ptPosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".boundingRect {" + qRect2Str(rectBoundingCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".position {" + physValLineCheckmarkRightLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".getLine {" + physValLineCheckmarkRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkRightLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".rotationAngle: 315.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Small Rectangle
    //----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjGroupNameSmallRect + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjGroupNameSmallRect + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(strGraphObjNameSmallRectTopLine);
    strlstGraphObjsAddToGroup.append(strGraphObjNameSmallRectRightLine);
    strlstGraphObjsAddToGroup.append(strGraphObjNameSmallRectBottomLine);
    strlstGraphObjsAddToGroup.append(strGraphObjNameSmallRectLeftLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNameSmallRect);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    // Group
    QPointF ptPosSmallRect(325.0, 275.0);
    QPointF ptTLSmallRect(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QSizeF sizeSmallRect(10.0, 10.0);
    QRectF rectBoundingSmallRect(QPointF(-5.0, -5.0), sizeSmallRect);
    CPhysValRect physValRectSmallRect(*m_pDrawingScene, ptTLSmallRect, sizeSmallRect);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupNameSmallRect + ".pos {" + qPoint2Str(ptPosSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameSmallRect + ".boundingRect {" + qRect2Str(rectBoundingSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameSmallRect + ".position {" + qPoint2Str(ptPosSmallRect, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameSmallRect + ".getRect {" + physValRectSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameSmallRect + ".getSize {" + physValRectSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameSmallRect + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallRect-TopLine
    QPointF ptP1SmallRectTopLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectTopLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectTopLine(0.0, -5.0);
    QRectF rectBoundingSmallRectTopLine(QPointF(-5.0, 0.0), QSizeF(10.0, 0.0));
    double fLengthSmallRectTopLine = 10.0;
    CPhysValLine physValLineSmallRectTopLine(*m_pDrawingScene, QPointF(0.0, 0.0), QPointF(10.0, 0.0));
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".pos {" + qPoint2Str(ptPosSmallRectTopLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".boundingRect {" + qRect2Str(rectBoundingSmallRectTopLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".position {" + physValLineSmallRectTopLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".getLine {" + physValLineSmallRectTopLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectTopLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallRect-RightLine
    QPointF ptP1SmallRectRightLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectRightLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectRightLine(5.0, 0.0);
    QRectF rectBoundingSmallRectRightLine(QPointF(0.0, -5.0), QSizeF(0.0, 10.0));
    double fLengthSmallRectRightLine = 10.0;
    CPhysValLine physValLineSmallRectRightLine(*m_pDrawingScene, QPointF(10.0, 0.0), QPointF(10.0, 10.0));
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".pos {" + qPoint2Str(ptPosSmallRectRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".boundingRect {" + qRect2Str(rectBoundingSmallRectRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".position {" + physValLineSmallRectRightLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".getLine {" + physValLineSmallRectRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectRightLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallRect-BottomLine
    QPointF ptP1SmallRectBottomLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectBottomLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectBottomLine(0.0, 5.0);
    QRectF rectBoundingSmallRectBottomLine(QPointF(-5.0, 0.0), QSizeF(10.0, 0.0));
    double fLengthSmallRectBottomLine = 10.0;
    CPhysValLine physValLineSmallRectBottomLine(*m_pDrawingScene, QPointF(10.0, 10.0), QPointF(0.0, 10.0));
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".pos {" + qPoint2Str(ptPosSmallRectBottomLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".boundingRect {" + qRect2Str(rectBoundingSmallRectBottomLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".position {" + physValLineSmallRectBottomLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".getLine {" + physValLineSmallRectBottomLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectBottomLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".rotationAngle: 180.0 " + Math::c_strSymbolDegree);
    // SmallRect-LeftLine
    QPointF ptP1SmallRectLeftLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectLeftLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectLeftLine(-5.0, 0.0);
    QRectF rectBoundingSmallRectLeftLine(QPointF(0.0, -5.0), QSizeF(0.0, 10.0));
    double fLengthSmallRectLeftLine = 10.0;
    CPhysValLine physValLineSmallRectLeftLine(*m_pDrawingScene, QPointF(0.0, 10.0), QPointF(0.0, 0.0));
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".pos {" + qPoint2Str(ptPosSmallRectLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".boundingRect {" + qRect2Str(rectBoundingSmallRectLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".position {" + physValLineSmallRectLeftLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".getLine {" + physValLineSmallRectLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectLeftLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".rotationAngle: 270.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Top Group
    //----------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjGroupNameTopGroup + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjGroupNameTopGroup + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );

    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(strGraphObjGroupNameSmallPlusSign);
    strlstGraphObjsAddToGroup.append(strGraphObjGroupNameBigPlusSign);
    strlstGraphObjsAddToGroup.append(strGraphObjGroupNameCheckmark);
    strlstGraphObjsAddToGroup.append(strGraphObjGroupNameSmallRect);
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNameTopGroup);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    // Group
    QPointF ptPosPlusSignLinesCheckmarkSmallRect(300.0, 300.0);
    QPointF ptTLPlusSignLinesCheckmarkSmallRect(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QSizeF sizePlusSignLinesCheckmarkSmallRect(100.0, 100.0);
    QRectF rectBoundingPlusSignLinesCheckmarkSmallRect(QPointF(-50.0, -50.0), sizePlusSignLinesCheckmarkSmallRect);
    CPhysValRect physValRectPlusSignLinesCheckmarkSmallRect(*m_pDrawingScene, ptTLPlusSignLinesCheckmarkSmallRect, sizePlusSignLinesCheckmarkSmallRect);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupNameTopGroup + ".pos {" + qPoint2Str(ptPosPlusSignLinesCheckmarkSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameTopGroup + ".boundingRect {" + qRect2Str(rectBoundingPlusSignLinesCheckmarkSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameTopGroup + ".position {" + qPoint2Str(ptPosPlusSignLinesCheckmarkSmallRect, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameTopGroup + ".getRect {" + physValRectPlusSignLinesCheckmarkSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameTopGroup + ".getSize {" + physValRectPlusSignLinesCheckmarkSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameTopGroup + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign
    ptPosSmallPlusSign = QPointF(-25.0, 25.0);
    ptTLSmallPlusSign = QPointF(10.0, bYAxisTopDown ? 60.0 : 40.0);
    sizeSmallPlusSign = QSizeF(30.0, 30.0);
    rectBoundingSmallPlusSign = QRectF(QPointF(-15.0, -15.0), sizeSmallPlusSign);
    physValRectSmallPlusSign = CPhysValRect(*m_pDrawingScene, ptTLSmallPlusSign, sizeSmallPlusSign);
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".pos {" + qPoint2Str(ptPosSmallPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".position {" + physValRectSmallPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".getRect {" + physValRectSmallPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".getSize {" + physValRectSmallPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign
    ptPosBigPlusSign = QPointF(0.0, 0.0);
    ptTLBigPlusSign = QPointF(0.0, bYAxisTopDown ? 0.0 : 100.0);
    sizeBigPlusSign = QSizeF(100.0, 100.0);
    rectBoundingBigPlusSign = QRectF(QPointF(-50.0, -50.0), sizeBigPlusSign);
    physValRectBigPlusSign = CPhysValRect(*m_pDrawingScene, ptTLBigPlusSign, sizeBigPlusSign);
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".pos {" + qPoint2Str(ptPosBigPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".position {" + physValRectBigPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".getRect {" + physValRectBigPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".getSize {" + physValRectBigPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // Checkmark
    ptPosCheckmark = QPointF(0.0, 0.0);
    ptTLCheckmark = QPointF(0.0, bYAxisTopDown ? 0.0 : 100.0);
    sizeCheckmark = QSizeF(100.0, 100.0);
    rectBoundingCheckmark = QRectF(QPointF(-50.0, -50.0), sizeCheckmark);
    physValRectCheckmark = CPhysValRect(*m_pDrawingScene, ptTLCheckmark, sizeCheckmark);
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".pos {" + qPoint2Str(ptPosCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".boundingRect {" + qRect2Str(rectBoundingCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".position {" + physValRectCheckmark.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".getRect {" + physValRectCheckmark.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".getSize {" + physValRectCheckmark.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallRect
    ptPosSmallRect = QPointF(25.0, -25.0);
    ptTLSmallRect = QPointF(70.0, bYAxisTopDown ? 20.0 : 80.0);
    sizeSmallRect = QSizeF(10.0, 10.0);
    rectBoundingSmallRect = QRectF(QPointF(-5.0, -5.0), sizeSmallRect);
    physValRectSmallRect = CPhysValRect(*m_pDrawingScene, ptTLSmallRect, sizeSmallRect);
    strlstExpectedValues.append(strGraphObjGroupNameSmallRect + ".pos {" + qPoint2Str(ptPosSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameSmallRect + ".boundingRect {" + qRect2Str(rectBoundingSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameSmallRect + ".position {" + physValRectSmallRect.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameSmallRect + ".getRect {" + physValRectSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameSmallRect + ".getSize {" + physValRectSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameSmallRect + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
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
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    CUnit unit = i_drawingSize.unit();
    QString strUnit = unit.symbol();
    bool bUnitPixel = (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    int iDigits = bUnitPixel ? 0 : i_drawingSize.metricImageCoorsDecimals();
    bool bYAxisTopDown = (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);

    QString strGraphObjGroupNameTopGroup = "Top";
    QString strGraphObjGroupNamePlusSigns = "PlusSigns";
    QString strGraphObjGroupNameSmallPlusSign = "SmallPlusSign";
    QString strGraphObjNameSmallPlusSignVerticalLine = "SmallPlusSign-VerticalLine";
    QString strGraphObjNameSmallPlusSignHorizontalLine = "SmallPlusSign-HorizontalLine";
    QString strGraphObjGroupNameBigPlusSign = "BigPlusSign";
    QString strGraphObjNameBigPlusSignVerticalLine = "BigPlusSign-VerticalLine";
    QString strGraphObjNameBigPlusSignHorizontalLine = "BigPlusSign-HorizontalLine";
    QString strGraphObjGroupNameCheckmark = "Checkmark";
    QString strGraphObjNameCheckmarkLeftLine = "Checkmark-LeftLine";
    QString strGraphObjNameCheckmarkRightLine = "Checkmark-RightLine";
    QString strGraphObjGroupNameSmallRect = "SmallRect";
    QString strGraphObjNameSmallRectTopLine = "SmallRect-TopLine";
    QString strGraphObjNameSmallRectRightLine = "SmallRect-RightLine";
    QString strGraphObjNameSmallRectBottomLine = "SmallRect-BottomLine";
    QString strGraphObjNameSmallRectLeftLine = "SmallRect-LeftLine";

    QString strGraphObjKeyInTree;
    QString strGraphObjParentKeyInTree;

    CPhysVal physValAngle(0.0, Units.Angle.Degree, 0.1);

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

    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNameTopGroup, strGraphObjGroupNameCheckmark, strGraphObjNameCheckmarkLeftLine);

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
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjNameCheckmarkLeftLine + ".setLine()",
        /* strOperation    */ strGraphObjNameCheckmarkLeftLine + ".setLine()",
        /* pGrpParent      */ pGrpModifyCheckmarkLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjNameCheckmarkLeftLine);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "setLine");
    pTestStep->setConfigValue("P1", ptP1CheckmarkLeftLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkLeftLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".pos {" + qPoint2Str(ptPosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".p1 {" + qPoint2Str(ptP1PosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".p2 {" + qPoint2Str(ptP2PosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".length: " + QString::number(Math::round2Nearest(fLengthCheckmarkLeftLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".position {" + physValPointCenterCheckmarkLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".getLine {" + physValLineCheckmarkLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkLeftLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".rotationAngle: 55.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Right Line
    //-----------

    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(
        strEntryType, strGraphObjGroupNameTopGroup, strGraphObjGroupNameCheckmark, strGraphObjNameCheckmarkRightLine);

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
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjNameCheckmarkRightLine + ".setLine()",
        /* strOperation    */ strGraphObjNameCheckmarkRightLine + ".setLine()",
        /* pGrpParent      */ pGrpModifyCheckmarkLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjNameCheckmarkRightLine);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "setLine");
    pTestStep->setConfigValue("P1", ptP1CheckmarkRightLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkRightLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".pos {" + qPoint2Str(ptPosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".p1 {" + qPoint2Str(ptP1PosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".p2 {" + qPoint2Str(ptP2PosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".length: " + QString::number(Math::round2Nearest(fLengthCheckmarkRightLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".position {" + physValPointCenterCheckmarkRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".getLine {" + physValLineCheckmarkRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkRightLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".rotationAngle: 315.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Checkmark - resizeToContent
    //----------------------------

    strGraphObjParentKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNameTopGroup);
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNameTopGroup, strGraphObjGroupNameCheckmark);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjGroupNameCheckmark + ".resizeToContent()",
        /* strOperation    */ strGraphObjGroupNameCheckmark + ".resizeToContent()",
        /* pGrpParent      */ pGrpModifyCheckmarkLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNameCheckmark);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "resizeToContent");
    // Group
    QPointF ptPosCheckmark(0.0, 0.0);
    QPointF ptTLCheckmark(25.0, bYAxisTopDown ? 25.0 : 75.0);
    QSizeF sizeCheckmark(50.0, 50.0);
    QRectF rectBoundingCheckmark(QPointF(-25.0, -25.0), sizeCheckmark);
    CPhysValRect physValRectCheckmark(*m_pDrawingScene, ptTLCheckmark, sizeCheckmark);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".pos {" + qPoint2Str(ptPosCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".boundingRect {" + qRect2Str(rectBoundingCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".position {" + physValRectCheckmark.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".getRect {" + physValRectCheckmark.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".getSize {" + physValRectCheckmark.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameCheckmark + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // Checkmark-LeftLine
    ptPosCheckmarkLeftLine = QPointF(-7.5, 0.0);
    QRectF rectBoundingCheckmarkLeftLine(QPointF(-17.5, -25.0), QSizeF(35.0, 50.0));
    fLengthCheckmarkLeftLine = 61.0;
    physValLineCheckmarkLeftLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 0.0), QPointF(35.0, 50.0));
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".pos {" + qPoint2Str(ptPosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".boundingRect {" + qRect2Str(rectBoundingCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".position {" + physValLineCheckmarkLeftLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".getLine {" + physValLineCheckmarkLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkLeftLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".rotationAngle: 55.0 " + Math::c_strSymbolDegree);
    // Checkmark-RightLine
    ptPosCheckmarkRightLine = QPointF(17.5, 17.5);
    QRectF rectBoundingCheckmarkRightLine(QPointF(-7.5, -7.5), QSizeF(15.0, 15.0));
    fLengthCheckmarkRightLine = 21.0;
    physValLineCheckmarkRightLine = CPhysValLine(*m_pDrawingScene, QPointF(35.0, 50.0), QPointF(50.0, 35.0));
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".pos {" + qPoint2Str(ptPosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".boundingRect {" + qRect2Str(rectBoundingCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".position {" + physValLineCheckmarkRightLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".getLine {" + physValLineCheckmarkRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkRightLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".rotationAngle: 315.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Modify PlusSigns
    //=================

    ZS::Test::CTestStepGroup* pGrpModifyPlusSigns = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Plus Lines",
        /* pTSGrpParent */ pGrpModifyShapes );

    // Remove SmallPlusSign from TopGroup
    //-----------------------------------

    strGraphObjParentKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNameTopGroup);
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNameTopGroup, strGraphObjGroupNameSmallPlusSign);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjGroupNameTopGroup + ".removeFromGroup(" + strGraphObjGroupNameSmallPlusSign + ")",
        /* strOperation    */ strGraphObjGroupNameTopGroup + ".removeFromGroup(" + strGraphObjGroupNameSmallPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupNameParent", strGraphObjGroupNameTopGroup);
    pTestStep->setConfigValue("GroupNameParentKeyInTree", strGraphObjParentKeyInTree);
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNameSmallPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "removeFromGroup");
    // Group
    QPointF ptPosSmallPlusSign(275.0, 325.0);
    QPointF ptTLSmallPlusSign(260.0, bYAxisTopDown ? 310.0 : 290.0);
    QSizeF sizeSmallPlusSign(30.0, 30.0);
    QRectF rectBoundingSmallPlusSign(QPointF(-15.0, -15.0), sizeSmallPlusSign);
    CPhysValRect physValRectSmallPlusSign(*m_pDrawingScene, ptTLSmallPlusSign, sizeSmallPlusSign);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".pos {" + qPoint2Str(ptPosSmallPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".position {" + qPoint2Str(ptPosSmallPlusSign, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".getRect {" + physValRectSmallPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".getSize {" + physValRectSmallPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameSmallPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-VerticalLine
    QPointF ptPosSmallPlusSignVerticalLine(0.0, 0.0);
    QRectF rectBoundingSmallPlusSignVerticalLine(QPointF(0.0, -15.0), QSizeF(0.0, 30.0));
    double fLengthSmallPlusSignVerticalLine = 30.0;
    CPhysValLine physValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(15.0, 0.0), QPointF(15.0, 30.0));
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    QPointF ptPosSmallPlusSignHorizontalLine = QPointF(0.0, 0.0);
    QRectF rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-15.0, 0.0), QSizeF(30.0, 0.0));
    double fLengthSmallPlusSignHorizontalLine = 30.0;
    CPhysValLine physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 15.0), QPointF(30.0, 15.0));
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Remove BigPlusSign from TopGroup
    //-----------------------------------

    strGraphObjParentKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNameTopGroup);
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNameTopGroup, strGraphObjGroupNameBigPlusSign);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjGroupNameTopGroup + ".removeFromGroup(" + strGraphObjGroupNameBigPlusSign + ")",
        /* strOperation    */ strGraphObjGroupNameTopGroup + ".removeFromGroup(" + strGraphObjGroupNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupNameParent", strGraphObjGroupNameTopGroup);
    pTestStep->setConfigValue("GroupNameParentKeyInTree", strGraphObjParentKeyInTree);
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "removeFromGroup");
    // Group
    QPointF ptPosBigPlusSign(300.0, 300.0);
    QPointF ptTLBigPlusSign(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QSizeF sizeBigPlusSign(100.0, 100.0);
    QRectF rectBoundingBigPlusSign(QPointF(-50.0, -50.0), sizeBigPlusSign);
    CPhysValRect physValRectBigPlusSign(*m_pDrawingScene, ptTLBigPlusSign, sizeBigPlusSign);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".pos {" + qPoint2Str(ptPosBigPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".position {" + qPoint2Str(ptPosBigPlusSign, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".getRect {" + physValRectBigPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".getSize {" + physValRectBigPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNameBigPlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    QPointF ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    QRectF rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -50.0), QSizeF(0.0, 100.0));
    double fLengthBigPlusSignVerticalLine = 100.0;
    CPhysValLine physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, 0.0), QPointF(50.0, 100.0));
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    QPointF ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    QRectF rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-50.0, 0.0), QSizeF(100.0, 0.0));
    double fLengthBigPlusSignHorizontalLine = 100.0;
    CPhysValLine physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Ungroup SmallPlusSign
    //----------------------

    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNameSmallPlusSign);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " DrawingScene.ungroup(" + strGraphObjGroupNameSmallPlusSign + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + strGraphObjGroupNameSmallPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNameSmallPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "ungroup");
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjNameSmallPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjNameSmallPlusSignHorizontalLine));
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
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".position {" + qPoint2Str(ptPosSmallPlusSignVerticalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // Horizontal Line
    QPointF ptP1SmallPlusSignHorizontalLine(260.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptP2SmallPlusSignHorizontalLine(290.0, bYAxisTopDown ? 325.0 : 275.0);
    ptPosSmallPlusSignHorizontalLine = QPointF(275.0, 325.0);
    rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-15.0, 0.0), QSizeF(30.0, 0.0));
    QPointF ptP1PosSmallPlusSignHorizontalLine(-15.0, 0.0);
    QPointF ptP2PosSmallPlusSignHorizontalLine(15.0, 0.0);
    fLengthSmallPlusSignHorizontalLine = 30.0;
    physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, ptP1SmallPlusSignHorizontalLine, ptP2SmallPlusSignHorizontalLine);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Ungroup BigPlusSign
    //----------------------

    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNameBigPlusSign);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " DrawingScene.ungroup(" + strGraphObjGroupNameBigPlusSign + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + strGraphObjGroupNameBigPlusSign + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNameBigPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "ungroup");
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjNameBigPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjNameBigPlusSignHorizontalLine));
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
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".position {" + qPoint2Str(ptPosBigPlusSignVerticalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // Horizontal Line
    QPointF ptP1BigPlusSignHorizontalLine(250.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptP2BigPlusSignHorizontalLine(350.0, bYAxisTopDown ? 300.0 : 300.0);
    ptPosBigPlusSignHorizontalLine = QPointF(300.0, 300.0);
    rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-50.0, 0.0), QSizeF(100.0, 0.0));
    QPointF ptP1PosBigPlusSignHorizontalLine(-50.0, 0.0);
    QPointF ptP2PosBigPlusSignHorizontalLine(50.0, 0.0);
    fLengthBigPlusSignHorizontalLine = 100.0;
    physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, ptP1BigPlusSignHorizontalLine, ptP2BigPlusSignHorizontalLine);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".position {" + qPoint2Str(ptPosBigPlusSignHorizontalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Group Plus Signs
    //-----------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjGroupNamePlusSigns + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjGroupNamePlusSigns + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(strGraphObjNameSmallPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(strGraphObjNameSmallPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.append(strGraphObjNameBigPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(strGraphObjNameBigPlusSignHorizontalLine);
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
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    ptPosSmallPlusSignHorizontalLine = QPointF(-25.0, 25.0);
    rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-15.0, 0.0), QSizeF(30.0, 0.0));
    fLengthSmallPlusSignHorizontalLine = 30.0;
    physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(10.0, 75.0), QPointF(40.0, 75.0));
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -50.0), QSizeF(0.0, 100.0));
    fLengthBigPlusSignVerticalLine = 100.0;
    physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, 0.0), QPointF(50.0, 100.0));
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-50.0, 0.0), QSizeF(100.0, 0.0));
    fLengthBigPlusSignHorizontalLine = 100.0;
    physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize PlusSigns
    //-----------------

    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNamePlusSigns);

    ptTLPlusSigns = QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0);
    sizePlusSigns = QSizeF(400.0, 200.0);
    QPointF ptBRPlusSigns(650.0, bYAxisTopDown ? 450.0 : 150.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjGroupNamePlusSigns + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* strOperation    */ strGraphObjGroupNamePlusSigns + ".setTopLeft(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNamePlusSigns);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBRPlusSigns);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    // Group PlusSigns
    QPointF ptPosPlusSign(450.0, 350.0);
    QRectF rectBoundingPlusSign(QPointF(-200.0, -100.0), sizePlusSigns);
    CPhysValRect physValRectPlusSign = CPhysValRect(*m_pDrawingScene, ptTLPlusSigns, sizePlusSigns);
    physValAngle.setVal(0.0);
    physValRectPlusSign.setAngle(physValAngle);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".pos {" + qPoint2Str(ptPosPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".boundingRect {" + qRect2Str(rectBoundingPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".position {" + physValRectPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".getRect {" + physValRectPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".getSize {" + physValRectPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupNamePlusSigns + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-VerticalLine
    ptPosSmallPlusSignVerticalLine = QPointF(-100.0, 50.0);
    rectBoundingSmallPlusSignVerticalLine = QRectF(QPointF(0.0, -30.0), QSizeF(0.0, 60.0));
    fLengthSmallPlusSignVerticalLine = 60.0;
    physValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(100.0, 120.0), QPointF(100.0, 180.0));
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    ptPosSmallPlusSignHorizontalLine = QPointF(-100.0, 50.0);
    rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-60.0, 0.0), QSizeF(120.0, 0.0));
    fLengthSmallPlusSignHorizontalLine = 120.0;
    physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(40.0, 150.0), QPointF(160.0, 150.0));
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -100.0), QSizeF(0.0, 200.0));
    fLengthBigPlusSignVerticalLine = 200.0;
    physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(200.0, 0.0), QPointF(200.0, 200.0));
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-200.0, 0.0), QSizeF(400.0, 0.0));
    fLengthBigPlusSignHorizontalLine = 400.0;
    physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 100.0), QPointF(400.0, 100.0));
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Ungroup PlusSigns
    //------------------

    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjGroupNamePlusSigns);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " DrawingScene.ungroup(" + strGraphObjGroupNamePlusSigns + ")",
        /* strOperation    */ "DrawingScene.ungroup(" + strGraphObjGroupNamePlusSigns + ")",
        /* pGrpParent      */ pGrpModifyPlusSigns,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupNamePlusSigns);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "ungroup");
    strlstGraphObjsKeyInTreeGetResultValues.clear();
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjNameSmallPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjNameSmallPlusSignHorizontalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjNameBigPlusSignVerticalLine));
    strlstGraphObjsKeyInTreeGetResultValues.append(pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjNameBigPlusSignHorizontalLine));
    pTestStep->setConfigValue("GraphObjsKeyInTreeGetResultValues", strlstGraphObjsKeyInTreeGetResultValues);
    strlstExpectedValues.clear();
    // SmallPlusSign-VerticalLine
    ptPosSmallPlusSignVerticalLine = QPointF(350.0, 400.0);
    rectBoundingSmallPlusSignVerticalLine = QRectF(QPointF(0.0, -30.0), QSizeF(0.0, 60.0));
    fLengthSmallPlusSignVerticalLine = 60.0;
    physValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(350.0, 370.0), QPointF(350.0, 430.0));
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    ptPosSmallPlusSignHorizontalLine = QPointF(350.0, 400.0);
    rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-60.0, 0.0), QSizeF(120.0, 0.0));
    fLengthSmallPlusSignHorizontalLine = 120.0;
    physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(290.0, 400.0), QPointF(410.0, 400.0));
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    ptPosBigPlusSignVerticalLine = QPointF(450.0, 350.0);
    rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -100.0), QSizeF(0.0, 200.0));
    fLengthBigPlusSignVerticalLine = 200.0;
    physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(450.0, 250.0), QPointF(450.0, 450.0));
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    ptPosBigPlusSignHorizontalLine = QPointF(450.0, 350.0);
    rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-200.0, 0.0), QSizeF(400.0, 0.0));
    fLengthBigPlusSignHorizontalLine = 400.0;
    physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(250.0, 350.0), QPointF(650.0, 350.0));
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

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
    // Rotate SmallRect
    //-----------------
#if 0
    QString strGroupNameParent = "Top";
    QString strGraphObjName = "SmallRect";
    QString strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameParent, strGraphObjName);

    CPhysVal physValAngle(45.0, Units.Angle.Degree, 0.1);

    QString strGraphObjNameSmallRectTopLine = "SmallRect-TopLine";
    QString strGraphObjNameSmallRectRightLine = "SmallRect-RightLine";
    QString strGraphObjNameSmallRectBottomLine = "SmallRect-BottomLine";
    QString strGraphObjNameSmallRectLeftLine = "SmallRect-LeftLine";

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjName + ".setRotationAngle(" + physValAngle.toString() + ")",
        /* strOperation    */ strGraphObjName + ".setRotationAngle(" + physValAngle.toString() + ")",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", physValAngle.toString());
    QPointF ptPosSmallRect(25.0, -25.0);
    sizeSmallRect = QSizeF(10.0, 10.0);
    QRectF rectBoundingSmallRect(QPointF(-5.0, -5.0), sizeSmallRect);
    physValRectSmallRect = CPhysValRect(*m_pDrawingScene, QPointF(70.0, 20.0), sizeSmallRect);
    physValRectSmallRect.setAngle(physValAngle);
    QStringList strlstExpectedValues;
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".boundingRect {" + qRect2Str(rectBoundingSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + physValRectSmallRect.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getRect {" + physValRectSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getSize {" + physValRectSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: " + physValAngle.toString());
    // SmallRect-TopLine
    QPointF ptP1SmallRectTopLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectTopLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectTopLine(0.0, -5.0);
    QRectF rectBoundingSmallRectTopLine(QPointF(-5.0, 0.0), QSizeF(10.0, 0.0));
    double fLengthSmallRectTopLine = 10.0;
    CPhysValLine physValLineSmallRectTopLine(*m_pDrawingScene, QPointF(0.0, 0.0), QPointF(10.0, 0.0));
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".pos {" + qPoint2Str(ptPosSmallRectTopLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".boundingRect {" + qRect2Str(rectBoundingSmallRectTopLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".position {" + physValLineSmallRectTopLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".getLine {" + physValLineSmallRectTopLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectTopLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectTopLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallRect-RightLine
    QPointF ptP1SmallRectRightLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectRightLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectRightLine(5.0, 0.0);
    QRectF rectBoundingSmallRectRightLine(QPointF(0.0, -5.0), QSizeF(0.0, 10.0));
    double fLengthSmallRectRightLine = 10.0;
    CPhysValLine physValLineSmallRectRightLine(*m_pDrawingScene, QPointF(10.0, 0.0), QPointF(10.0, 10.0));
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".pos {" + qPoint2Str(ptPosSmallRectRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".boundingRect {" + qRect2Str(rectBoundingSmallRectRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".position {" + physValLineSmallRectRightLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".getLine {" + physValLineSmallRectRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectRightLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectRightLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallRect-BottomLine
    QPointF ptP1SmallRectBottomLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectBottomLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectBottomLine(0.0, 5.0);
    QRectF rectBoundingSmallRectBottomLine(QPointF(-5.0, 0.0), QSizeF(10.0, 0.0));
    double fLengthSmallRectBottomLine = 10.0;
    CPhysValLine physValLineSmallRectBottomLine(*m_pDrawingScene, QPointF(10.0, 10.0), QPointF(0.0, 10.0));
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".pos {" + qPoint2Str(ptPosSmallRectBottomLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".boundingRect {" + qRect2Str(rectBoundingSmallRectBottomLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".position {" + physValLineSmallRectBottomLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".getLine {" + physValLineSmallRectBottomLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectBottomLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectBottomLine + ".rotationAngle: 180.0 " + Math::c_strSymbolDegree);
    // SmallRect-LeftLine
    QPointF ptP1SmallRectLeftLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectLeftLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectLeftLine(-5.0, 0.0);
    QRectF rectBoundingSmallRectLeftLine(QPointF(0.0, -5.0), QSizeF(0.0, 10.0));
    double fLengthSmallRectLeftLine = 10.0;
    CPhysValLine physValLineSmallRectLeftLine(*m_pDrawingScene, QPointF(0.0, 10.0), QPointF(0.0, 0.0));
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".pos {" + qPoint2Str(ptPosSmallRectLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".boundingRect {" + qRect2Str(rectBoundingSmallRectLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".position {" + physValLineSmallRectLeftLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".getLine {" + physValLineSmallRectLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectLeftLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRectLeftLine + ".rotationAngle: 270.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // PlusSign (moving Vertical Line and Horizontal Line to new group)
    //-----------------------------------------------------------------

    strGraphObjGroupName = "PlusSign";

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjGroupName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjGroupName + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    QString strGraphObjPathNameBigPlusSignVerticalLine = pIdxTree->buildPathStr("Top", strGraphObjNameBigPlusSignVerticalLine);
    QString strGraphObjPathNameBigPlusSignHorizontalLine = pIdxTree->buildPathStr("Top", strGraphObjNameBigPlusSignHorizontalLine);
    QString strGraphObjPathNameSmallPlusSignVerticalLine = strGraphObjNameSmallPlusSignVerticalLine;
    QString strGraphObjPathNameSmallPlusSignHorizontalLine = strGraphObjNameSmallPlusSignHorizontalLine;
    QString strGraphObjPathNameBigPlusSignVerticalLine = strGraphObjNameBigPlusSignVerticalLine;
    QString strGraphObjPathNameBigPlusSignHorizontalLine = strGraphObjNameBigPlusSignHorizontalLine;
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(strGraphObjPathNameSmallPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(strGraphObjPathNameSmallPlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.append(strGraphObjPathNameBigPlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(strGraphObjPathNameBigPlusSignHorizontalLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupName);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    // Group
    QPointF ptPosPlusSign(300.0, 300.0);
    QPointF ptTLPlusSign(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QSizeF sizePlusSign(100.0, 100.0);
    QRectF rectBoundingPlusSign(QPointF(-50.0, -50.0), sizePlusSign);
    CPhysValRect physValRectPlusSign(*m_pDrawingScene, ptTLPlusSign, sizePlusSign);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupName + ".pos {" + qPoint2Str(ptPosPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupName + ".boundingRect {" + qRect2Str(rectBoundingPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjGroupName + ".position {" + qPoint2Str(ptPosPlusSign, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupName + ".getRect {" + physValRectPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupName + ".getSize {" + physValRectPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-VerticalLine
    QPointF ptPosSmallPlusSignVerticalLine = QPointF(-25.0, -25.0);
    QRectF rectBoundingSmallPlusSignVerticalLine = QRectF(QPointF(0.0, -25.0), QSizeF(0.0, 50.0));
    double fLengthSmallPlusSignVerticalLine = 50.0;
    CPhysValLine physValLineSmallPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(25.0, 0.0), QPointF(25.0, 50.0));
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    QPointF ptPosSmallPlusSignHorizontalLine = QPointF(-25.0, -25.0);
    QRectF rectBoundingSmallPlusSignHorizontalLine = QRectF(QPointF(-25.0, 0.0), QSizeF(50.0, 0.0));
    double fLengthSmallPlusSignHorizontalLine = 50.0;
    CPhysValLine physValLineSmallPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 25.0), QPointF(50.0, 25.0));
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    QPointF ptPosBigPlusSignVerticalLine = QPointF(0.0, 0.0);
    QRectF rectBoundingBigPlusSignVerticalLine = QRectF(QPointF(0.0, -50.0), QSizeF(0.0, 100.0));
    double fLengthBigPlusSignVerticalLine = 100.0;
    CPhysValLine physValLineBigPlusSignVerticalLine = CPhysValLine(*m_pDrawingScene, QPointF(50.0, 0.0), QPointF(50.0, 100.0));
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    QPointF ptPosBigPlusSignHorizontalLine = QPointF(0.0, 0.0);
    QRectF rectBoundingBigPlusSignHorizontalLine = QRectF(QPointF(-50.0, 0.0), QSizeF(100.0, 0.0));
    double fLengthBigPlusSignHorizontalLine = 100.0;
    CPhysValLine physValLineBigPlusSignHorizontalLine = CPhysValLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Move PlusSign Group to Group with SmallRect and Checkmark
    //----------------------------------------------------------

    strGroupNameParent = "Top";
    strGroupNameParentKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameParent);
    strGraphObjName = "PlusSign";
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGroupNameParent + ".addToGroup(" + strGraphObjName + ")",
        /* strOperation    */ strGroupNameParent + ".addToGroup(" + strGraphObjName + ")",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupNameParent", strGroupNameParent);
    pTestStep->setConfigValue("GroupNameParentKeyInTree", strGroupNameParentKeyInTree);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "addToGroup");
    // Moved PlusSign Group
    QPointF ptPosPlusSign(0.0, 0.0);
    QPointF ptTLPlusSign(0.0, bYAxisTopDown ? 0.0 : 100.0);
    QSizeF sizePlusSign(100.0, 100.0);
    QRectF rectBoundingPlusSign(QPointF(-50.0, -50.0), sizePlusSign);
    CPhysValRect physValRectPlusSign(*m_pDrawingScene, ptTLPlusSign, sizePlusSign);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".boundingRect {" + qRect2Str(rectBoundingPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + physValRectPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getRect {" + physValRectPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getSize {" + physValRectPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    QPointF ptPosBigPlusSignVerticalLine(0.0, 0.0);
    QRectF rectBoundingPlusSignVerticalLine(QPointF(0.0, -50.0), QSizeF(0.0, 100.0));
    double fLengthBigPlusSignVerticalLine = 100.0;
    CPhysValLine physValLineBigPlusSignVerticalLine(*m_pDrawingScene, QPointF(50.0, 0.0), QPointF(50.0, 100.0));
    strlstExpectedValues.append(strGraphObjNamePlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNamePlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNamePlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNamePlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNamePlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNamePlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    QPointF ptPosBigPlusSignHorizontalLine(0.0, 0.0);
    QRectF rectBoundingPlusSignHorizontalLine(QPointF(-50.0, 0.0), QSizeF(100.0, 0.0));
    double fLengthBigPlusSignHorizontalLine = 100.0;
    CPhysValLine physValLineBigPlusSignHorizontalLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(100.0, 50.0));
    strlstExpectedValues.append(strGraphObjNamePlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNamePlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNamePlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNamePlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNamePlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNamePlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
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

    // Resize top group Top
    //----------------------------------------------

#if 0
    strGraphObjName = "Top";
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    QPoint ptBR(450.0, bYAxisTopDown ? 450.0 : 150.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjName + ".setBottomRight(" + qPoint2Str(ptBR) + " " + unit.symbol() + ")",
        /* strOperation    */ strGraphObjName + ".setTopLeft(" + qPoint2Str(ptBR) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBR);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    // Group Top
    QPointF ptPosPlusSignLinesCheckmarkSmallRect(350.0, 350.0);
    QPointF ptTLPlusSignLinesCheckmarkSmallRect = QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QSizeF sizePlusSignLinesCheckmarkSmallRect = QSizeF(200.0, 200.0);
    QRectF rectBoundingPlusSignLinesCheckmarkSmallRect(QPointF(-100.0, -100.0), sizePlusSignLinesCheckmarkSmallRect);
    CPhysValRect physValRectPlusSignLinesCheckmarkSmallRect = CPhysValRect(*m_pDrawingScene, ptTLPlusSignLinesCheckmarkSmallRect, sizePlusSignLinesCheckmarkSmallRect);
    physValAngle.setVal(0.0);
    physValRectPlusSignLinesCheckmarkSmallRect.setAngle(physValAngle);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosPlusSignLinesCheckmarkSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".boundingRect {" + qRect2Str(rectBoundingPlusSignLinesCheckmarkSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + physValRectPlusSignLinesCheckmarkSmallRect.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getRect {" + physValRectPlusSignLinesCheckmarkSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getSize {" + physValRectPlusSignLinesCheckmarkSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: " + physValAngle.toString());
    // SmallRect
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".pos {" + qPoint2Str(ptPosSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".boundingRect {" + qRect2Str(rectBoundingSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".position {" + physValRectSmallRect.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".getRect {" + physValRectSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".getSize {" + physValRectSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".rotationAngle: 45.0 " + Math::c_strSymbolDegree);
    // Checkmark
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".pos {" + qPoint2Str(ptPosCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".boundingRect {" + qRect2Str(rectBoundingCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".position {" + physValRectCheckmark.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".getRect {" + physValRectCheckmark.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".getSize {" + physValRectCheckmark.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
    // PlusSign
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".pos {" + qPoint2Str(ptPosPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".boundingRect {" + qRect2Str(rectBoundingPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".position {" + physValRectPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".getRect {" + physValRectPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".getSize {" + physValRectPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Rotate top group Top
    //----------------------------------------------

    strGraphObjName = "Top";
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    physValAngle.setVal(45.0);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjName + ".setRotationAngle(" + physValAngle.toString() + ")",
        /* strOperation    */ strGraphObjName + ".setRotationAngle(" + physValAngle.toString() + ")",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", physValAngle.toString());
    // Group Top
    ptPosPlusSignLinesCheckmarkSmallRect = QPointF(300.0, 300.0);
    ptTLPlusSignLinesCheckmarkSmallRect = QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0);
    sizePlusSignLinesCheckmarkSmallRect = QSizeF(100.0, 100.0);
    rectBoundingPlusSignLinesCheckmarkSmallRect = QRectF(QPointF(-50.0, -50.0), sizePlusSignLinesCheckmarkSmallRect);
    physValRectPlusSignLinesCheckmarkSmallRect = CPhysValRect(*m_pDrawingScene, ptTLPlusSignLinesCheckmarkSmallRect, sizePlusSignLinesCheckmarkSmallRect);
    physValRectPlusSignLinesCheckmarkSmallRect.setAngle(physValAngle);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosPlusSignLinesCheckmarkSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".boundingRect {" + qRect2Str(rectBoundingPlusSignLinesCheckmarkSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + physValRectPlusSignLinesCheckmarkSmallRect.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getRect {" + physValRectPlusSignLinesCheckmarkSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getSize {" + physValRectPlusSignLinesCheckmarkSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: " + physValAngle.toString());
    // SmallRect
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".pos {" + qPoint2Str(ptPosSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".boundingRect {" + qRect2Str(rectBoundingSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".position {" + physValRectSmallRect.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".getRect {" + physValRectSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".getSize {" + physValRectSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".rotationAngle: 45.0 " + Math::c_strSymbolDegree);
    // Checkmark
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".pos {" + qPoint2Str(ptPosCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".boundingRect {" + qRect2Str(rectBoundingCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".position {" + physValRectCheckmark.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".getRect {" + physValRectCheckmark.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".getSize {" + physValRectCheckmark.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
    // PlusSign
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".pos {" + qPoint2Str(ptPosPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".boundingRect {" + qRect2Str(rectBoundingPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".position {" + physValRectPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".getRect {" + physValRectPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".getSize {" + physValRectPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Resize top group Top
    //----------------------------------------------

    strGraphObjName = "Top";
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    QPoint ptTopLeft(300.0, bYAxisTopDown ? 200.0 : 400.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjName + ".setTopLeft(" + qPoint2Str(ptTopLeft) + " " + unit.symbol() + ")",
        /* strOperation    */ strGraphObjName + ".setTopLeft(" + qPoint2Str(ptTopLeft) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "setTopLeft");
    pTestStep->setConfigValue("TopLeft", ptTopLeft);
    pTestStep->setConfigValue("TopLeft.unit", unit.symbol());
    // Group Top
    ptPosPlusSignLinesCheckmarkSmallRect = QPointF(300.0, 300.0);
    ptTLPlusSignLinesCheckmarkSmallRect = QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0);
    sizePlusSignLinesCheckmarkSmallRect = QSizeF(100.0, 100.0);
    rectBoundingPlusSignLinesCheckmarkSmallRect = QRectF(QPointF(-50.0, -50.0), sizePlusSignLinesCheckmarkSmallRect);
    physValRectPlusSignLinesCheckmarkSmallRect = CPhysValRect(*m_pDrawingScene, ptTLPlusSignLinesCheckmarkSmallRect, sizePlusSignLinesCheckmarkSmallRect);
    physValRectPlusSignLinesCheckmarkSmallRect.setAngle(physValAngle);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosPlusSignLinesCheckmarkSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".boundingRect {" + qRect2Str(rectBoundingPlusSignLinesCheckmarkSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + physValRectPlusSignLinesCheckmarkSmallRect.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getRect {" + physValRectPlusSignLinesCheckmarkSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getSize {" + physValRectPlusSignLinesCheckmarkSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: " + physValAngle.toString());
    // SmallRect
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".pos {" + qPoint2Str(ptPosSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".boundingRect {" + qRect2Str(rectBoundingSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".position {" + physValRectSmallRect.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".getRect {" + physValRectSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".getSize {" + physValRectSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".rotationAngle: 45.0 " + Math::c_strSymbolDegree);
    // Checkmark
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".pos {" + qPoint2Str(ptPosCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".boundingRect {" + qRect2Str(rectBoundingCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".position {" + physValRectCheckmark.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".getRect {" + physValRectCheckmark.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".getSize {" + physValRectCheckmark.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
    // PlusSign
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".pos {" + qPoint2Str(ptPosPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".boundingRect {" + qRect2Str(rectBoundingPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".position {" + physValRectPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".getRect {" + physValRectPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".getSize {" + physValRectPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNamePlusSign + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Group2 (horizonal and vertical lines together with checkmark and rectangle group)
    //----------------------------------------------------------------------------------

    // Group2.removeFromGroup(Group0)
    strGroupNameParent = "Group2";
    strGroupNameParentKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameParent);
    strGraphObjName = "Group0";
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameParent, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGroupNameParent + ".removeFromGroup(" + strGraphObjName + ")",
        /* strOperation    */ strGroupNameParent + ".removeFromGroup(" + strGraphObjName + ")",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupNameParent", strGroupNameParent);
    pTestStep->setConfigValue("GroupNameParentKeyInTree", strGroupNameParentKeyInTree);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "removeFromGroup");

    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {325.0, 275.0} px");
    strlstExpectedValues.append(strGraphObjName + ".topLeft {-5.0, -5.0} px");
    strlstExpectedValues.append(strGraphObjName + ".bottomRight {5.0, 5.0} px");
    strlstExpectedValues.append(strGraphObjName + ".size {10.0, 10.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append(strGraphObjName + ".position {325, 275} px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 320 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 270 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 330 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 280 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.size.width: 10 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.size.height: 10 px");
        strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line4.pos {0.0, -5.0} px");
        strlstExpectedValues.append("Line4.position {5, 0} px");
        strlstExpectedValues.append("Line4.getLine.p1.x: 0 px");
        strlstExpectedValues.append("Line4.getLine.p1.y: 0 px");
        strlstExpectedValues.append("Line4.getLine.p2.x: 10 px");
        strlstExpectedValues.append("Line4.getLine.p2.y: 0 px");
        strlstExpectedValues.append("Line4.getLength: 10 px");
        strlstExpectedValues.append("Line4.rotationAngle: 0.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line5.pos {5.0, 0.0} px");
        strlstExpectedValues.append("Line5.position {10, 5} px");
        strlstExpectedValues.append("Line5.getLine.p1.x: 10 px");
        strlstExpectedValues.append("Line5.getLine.p1.y: 0 px");
        strlstExpectedValues.append("Line5.getLine.p2.x: 10 px");
        strlstExpectedValues.append("Line5.getLine.p2.y: 10 px");
        strlstExpectedValues.append("Line5.getLength: 10 px");
        strlstExpectedValues.append("Line5.rotationAngle: 90.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line6.pos {0.0, 5.0} px");
        strlstExpectedValues.append("Line6.position {5, 10} px");
        strlstExpectedValues.append("Line6.getLine.p1.x: 10 px");
        strlstExpectedValues.append("Line6.getLine.p1.y: 10 px");
        strlstExpectedValues.append("Line6.getLine.p2.x: 0 px");
        strlstExpectedValues.append("Line6.getLine.p2.y: 10 px");
        strlstExpectedValues.append("Line6.getLength: 10 px");
        strlstExpectedValues.append("Line6.rotationAngle: 180.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line7.pos {-5.0, 0.0} px");
        strlstExpectedValues.append("Line7.position {0, 5} px");
        strlstExpectedValues.append("Line7.getLine.p1.x: 0 px");
        strlstExpectedValues.append("Line7.getLine.p1.y: 10 px");
        strlstExpectedValues.append("Line7.getLine.p2.x: 0 px");
        strlstExpectedValues.append("Line7.getLine.p2.y: 0 px");
        strlstExpectedValues.append("Line7.getLength: 10 px");
        strlstExpectedValues.append("Line7.rotationAngle: 270.0 " + Math::c_strSymbolDegree);
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append(strGraphObjName + ".position {325.00, 275.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 320.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 270.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 330.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 280.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.width: 10.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.height: 10.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line4.pos {0.0, -5.0} px");
            strlstExpectedValues.append("Line4.position {5.00, 0.00} mm");
            strlstExpectedValues.append("Line4.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line4.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line4.getLine.p2.x: 10.00 mm");
            strlstExpectedValues.append("Line4.getLine.p2.y: 0.00 mm");
            strlstExpectedValues.append("Line4.getLength: 10.00 mm");
            strlstExpectedValues.append("Line4.rotationAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line5.pos {5.0, 0.0} px");
            strlstExpectedValues.append("Line5.position {10.00, 5.00} mm");
            strlstExpectedValues.append("Line5.getLine.p1.x: 10.00 mm");
            strlstExpectedValues.append("Line5.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line5.getLine.p2.x: 10.00 mm");
            strlstExpectedValues.append("Line5.getLine.p2.y: 10.00 mm");
            strlstExpectedValues.append("Line5.getLength: 10.00 mm");
            strlstExpectedValues.append("Line5.rotationAngle: 90.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line6.pos {0.0, 5.0} px");
            strlstExpectedValues.append("Line6.position {5.00, 10.00} mm");
            strlstExpectedValues.append("Line6.getLine.p1.x: 10.00 mm");
            strlstExpectedValues.append("Line6.getLine.p1.y: 10.00 mm");
            strlstExpectedValues.append("Line6.getLine.p2.x: 0.00 mm");
            strlstExpectedValues.append("Line6.getLine.p2.y: 10.00 mm");
            strlstExpectedValues.append("Line6.getLength: 10.00 mm");
            strlstExpectedValues.append("Line6.rotationAngle: 180.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line7.pos {-5.0, 0.0} px");
            strlstExpectedValues.append("Line7.position {0.00, 5.00} mm");
            strlstExpectedValues.append("Line7.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line7.getLine.p1.y: 10.00 mm");
            strlstExpectedValues.append("Line7.getLine.p2.x: 0.00 mm");
            strlstExpectedValues.append("Line7.getLine.p2.y: 0.00 mm");
            strlstExpectedValues.append("Line7.getLength: 10.00 mm");
            strlstExpectedValues.append("Line7.rotationAngle: 270.0 " + Math::c_strSymbolDegree);
        }
        else {
            strlstExpectedValues.append(strGraphObjName + ".position {325.00, 325.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 320.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 330.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 330.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 320.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.width: 10.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.height: -10.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line4.pos {0.0, -5.0} px");
            strlstExpectedValues.append("Line4.position {5.00, 10.00} mm");
            strlstExpectedValues.append("Line4.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line4.getLine.p1.y: 10.00 mm");
            strlstExpectedValues.append("Line4.getLine.p2.x: 10.00 mm");
            strlstExpectedValues.append("Line4.getLine.p2.y: 10.00 mm");
            strlstExpectedValues.append("Line4.getLength: 10.00 mm");
            strlstExpectedValues.append("Line4.rotationAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line5.pos {5.0, 0.0} px");
            strlstExpectedValues.append("Line5.position {10.00, 5.00} mm");
            strlstExpectedValues.append("Line5.getLine.p1.x: 10.00 mm");
            strlstExpectedValues.append("Line5.getLine.p1.y: 10.00 mm");
            strlstExpectedValues.append("Line5.getLine.p2.x: 10.00 mm");
            strlstExpectedValues.append("Line5.getLine.p2.y: 0.00 mm");
            strlstExpectedValues.append("Line5.getLength: 10.00 mm");
            strlstExpectedValues.append("Line5.rotationAngle: 90.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line6.pos {0.0, 5.0} px");
            strlstExpectedValues.append("Line6.position {5.00, 0.00} mm");
            strlstExpectedValues.append("Line6.getLine.p1.x: 10.00 mm");
            strlstExpectedValues.append("Line6.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line6.getLine.p2.x: 0.00 mm");
            strlstExpectedValues.append("Line6.getLine.p2.y: 0.00 mm");
            strlstExpectedValues.append("Line6.getLength: 10.00 mm");
            strlstExpectedValues.append("Line6.rotationAngle: 180.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line7.pos {-5.0, 0.0} px");
            strlstExpectedValues.append("Line7.position {0.00, 5.00} mm");
            strlstExpectedValues.append("Line7.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line7.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line7.getLine.p2.x: 0.00 mm");
            strlstExpectedValues.append("Line7.getLine.p2.y: 10.00 mm");
            strlstExpectedValues.append("Line7.getLength: 10.00 mm");
            strlstExpectedValues.append("Line7.rotationAngle: 270.0 " + Math::c_strSymbolDegree);
        }
    }
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif

    // Group2 (horizonal and vertical lines together with checkmark and rectangle group)
    //----------------------------------------------------------------------------------
#if 0
    // Group2.removeFromGroup(Group1)
    strGroupNameParent = "Group2";
    strGroupNameParentKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameParent);
    strGraphObjName = "Group1";
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameParent, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGroupNameParent + ".removeFromGroup(" + strGraphObjName + ")",
        /* strOperation    */ strGroupNameParent + ".removeFromGroup(" + strGraphObjName + ")",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupNameParent", strGroupNameParent);
    pTestStep->setConfigValue("GroupNameParentKeyInTree", strGroupNameParentKeyInTree);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "removeFromGroup");

    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {300.0, 300.0} px");
    strlstExpectedValues.append(strGraphObjName + ".topLeft {-25.0, -25.0} px");
    strlstExpectedValues.append(strGraphObjName + ".bottomRight {25.0, 25.0} px");
    strlstExpectedValues.append(strGraphObjName + ".size {50.0, 50.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append(strGraphObjName + ".position {300, 300} px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 275 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 275 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 325 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 325 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.size.width: 50 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.size.height: 50 px");
        strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line2.pos {-7.5, 0.0} px");
        strlstExpectedValues.append("Line2.position {18, 25} px");
        strlstExpectedValues.append("Line2.getLine.p1.x: 0 px");
        strlstExpectedValues.append("Line2.getLine.p1.y: 0 px");
        strlstExpectedValues.append("Line2.getLine.p2.x: 35 px");
        strlstExpectedValues.append("Line2.getLine.p2.y: 50 px");
        strlstExpectedValues.append("Line2.getLength: 61 px");
        strlstExpectedValues.append("Line2.rotationAngle: 55.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line3.pos {17.5, 17.5} px");
        strlstExpectedValues.append("Line3.position {43, 43} px");
        strlstExpectedValues.append("Line3.getLine.p1.x: 35 px");
        strlstExpectedValues.append("Line3.getLine.p1.y: 50 px");
        strlstExpectedValues.append("Line3.getLine.p2.x: 50 px");
        strlstExpectedValues.append("Line3.getLine.p2.y: 35 px");
        strlstExpectedValues.append("Line3.getLength: 21 px");
        strlstExpectedValues.append("Line3.rotationAngle: 315.0 " + Math::c_strSymbolDegree);
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append(strGraphObjName + ".position {300.00, 300.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 275.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 275.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 325.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 325.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.width: 50.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.height: 50.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line2.pos {-7.5, 0.0} px");
            strlstExpectedValues.append("Line2.position {17.50, 25.00} mm");
            strlstExpectedValues.append("Line2.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line2.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line2.getLine.p2.x: 35.00 mm");
            strlstExpectedValues.append("Line2.getLine.p2.y: 50.00 mm");
            strlstExpectedValues.append("Line2.getLength: 61.03 mm");
            strlstExpectedValues.append("Line2.rotationAngle: 55.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line3.pos {17.5, 17.5} px");
            strlstExpectedValues.append("Line3.position {42.50, 42.50} mm");
            strlstExpectedValues.append("Line3.getLine.p1.x: 35.00 mm");
            strlstExpectedValues.append("Line3.getLine.p1.y: 50.00 mm");
            strlstExpectedValues.append("Line3.getLine.p2.x: 50.00 mm");
            strlstExpectedValues.append("Line3.getLine.p2.y: 35.00 mm");
            strlstExpectedValues.append("Line3.getLength: 21.21 mm");
            strlstExpectedValues.append("Line3.rotationAngle: 315.0 " + Math::c_strSymbolDegree);
        }
        else {
            strlstExpectedValues.append(strGraphObjName + ".position {300.00, 300.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 275.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 325.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 325.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 275.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.width: 50.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.height: -50.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line2.pos {-7.5, 0.0} px");
            strlstExpectedValues.append("Line2.position {17.50, 25.00} mm");
            strlstExpectedValues.append("Line2.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line2.getLine.p1.y: 50.00 mm");
            strlstExpectedValues.append("Line2.getLine.p2.x: 35.00 mm");
            strlstExpectedValues.append("Line2.getLine.p2.y: 0.00 mm");
            strlstExpectedValues.append("Line2.getLength: 61.03 mm");
            strlstExpectedValues.append("Line2.rotationAngle: 55.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line3.pos {17.5, 17.5} px");
            strlstExpectedValues.append("Line3.position {42.50, 7.50} mm");
            strlstExpectedValues.append("Line3.getLine.p1.x: 35.00 mm");
            strlstExpectedValues.append("Line3.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line3.getLine.p2.x: 50.00 mm");
            strlstExpectedValues.append("Line3.getLine.p2.y: 15.00 mm");
            strlstExpectedValues.append("Line3.getLength: 21.21 mm");
            strlstExpectedValues.append("Line3.rotationAngle: 315.0 " + Math::c_strSymbolDegree);
        }
    }
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif

    // Group2 (horizonal and vertical lines)
    //--------------------------------------
#if 0
    // Group2.setPosition()
    strGraphObjName = "Group2";
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    ptPos = QPointF(400.0, 300.0);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjName + ".setPosition(" + qPoint2Str(ptPos) + " " + i_drawingSize.unit().symbol() + ")",
        /* strOperation    */ strGraphObjName + ".setPosition(" + qPoint2Str(ptPos) + " " + i_drawingSize.unit().symbol() + ")",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "setPosition");
    pTestStep->setConfigValue("Pos", ptPos);

    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {400.0, 300.0} px");
    strlstExpectedValues.append(strGraphObjName + ".topLeft {-50.0, -50.0} px");
    strlstExpectedValues.append(strGraphObjName + ".bottomRight {50.0, 50.0} px");
    strlstExpectedValues.append(strGraphObjName + ".size {100.0, 100.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append(strGraphObjName + ".position {400, 300} px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 350 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 250 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 450 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 350 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.size.width: 100 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.size.height: 100 px");
        strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 45.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line0.pos {0.0, 0.0} px");
        strlstExpectedValues.append("Line0.position {50, 50} px");
        strlstExpectedValues.append("Line0.getLine.p1.x: 50 px");
        strlstExpectedValues.append("Line0.getLine.p1.y: 0 px");
        strlstExpectedValues.append("Line0.getLine.p2.x: 50 px");
        strlstExpectedValues.append("Line0.getLine.p2.y: 100 px");
        strlstExpectedValues.append("Line0.getLength: 100 px");
        strlstExpectedValues.append("Line0.rotationAngle: 90.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line1.pos {0.0, 0.0} px");
        strlstExpectedValues.append("Line1.position {50, 50} px");
        strlstExpectedValues.append("Line1.getLine.p1.x: 0 px");
        strlstExpectedValues.append("Line1.getLine.p1.y: 50 px");
        strlstExpectedValues.append("Line1.getLine.p2.x: 100 px");
        strlstExpectedValues.append("Line1.getLine.p2.y: 50 px");
        strlstExpectedValues.append("Line1.getLength: 100 px");
        strlstExpectedValues.append("Line1.rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append(strGraphObjName + ".position {400.00, 300.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 350.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 250.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 450.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 350.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.width: 100.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.height: 100.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 45.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line0.pos {0.0, 0.0} px");
            strlstExpectedValues.append("Line0.position {50.00, 50.00} mm");
            strlstExpectedValues.append("Line0.getLine.p1.x: 50.00 mm");
            strlstExpectedValues.append("Line0.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line0.getLine.p2.x: 50.00 mm");
            strlstExpectedValues.append("Line0.getLine.p2.y: 100.00 mm");
            strlstExpectedValues.append("Line0.getLength: 100.00 mm");
            strlstExpectedValues.append("Line0.rotationAngle: 90.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line1.pos {0.0, 0.0} px");
            strlstExpectedValues.append("Line1.position {50.00, 50.00} mm");
            strlstExpectedValues.append("Line1.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line1.getLine.p1.y: 50.00 mm");
            strlstExpectedValues.append("Line1.getLine.p2.x: 100.00 mm");
            strlstExpectedValues.append("Line1.getLine.p2.y: 50.00 mm");
            strlstExpectedValues.append("Line1.getLength: 100.00 mm");
            strlstExpectedValues.append("Line1.rotationAngle: 0.0 " + Math::c_strSymbolDegree);
        }
        else {
            strlstExpectedValues.append(strGraphObjName + ".position {400.00, 300.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 350.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 350.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 450.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 250.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.width: 100.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.height: -100.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 45.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line0.pos {0.0, 0.0} px");
            strlstExpectedValues.append("Line0.position {50.00, 50.00} mm");
            strlstExpectedValues.append("Line0.getLine.p1.x: 50.00 mm");
            strlstExpectedValues.append("Line0.getLine.p1.y: 100.00 mm");
            strlstExpectedValues.append("Line0.getLine.p2.x: 50.00 mm");
            strlstExpectedValues.append("Line0.getLine.p2.y: 0.00 mm");
            strlstExpectedValues.append("Line0.getLength: 100.00 mm");
            strlstExpectedValues.append("Line0.rotationAngle: 90.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line1.pos {0.0, 0.0} px");
            strlstExpectedValues.append("Line1.position {50.00, 50.00} mm");
            strlstExpectedValues.append("Line1.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line1.getLine.p1.y: 50.00 mm");
            strlstExpectedValues.append("Line1.getLine.p2.x: 100.00 mm");
            strlstExpectedValues.append("Line1.getLine.p2.y: 50.00 mm");
            strlstExpectedValues.append("Line1.getLength: 100.00 mm");
            strlstExpectedValues.append("Line1.rotationAngle: 0.0 " + Math::c_strSymbolDegree);
        }
    }
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpModifyShapes->path());
    }
    return pGrpModifyShapes;
}
