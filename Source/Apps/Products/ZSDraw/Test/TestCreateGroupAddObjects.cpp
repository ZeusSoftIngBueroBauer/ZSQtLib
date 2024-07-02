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

    //for (const QString& strKey : s_hshGraphObjsInstCounts.keys()) {
    //    s_hshGraphObjsInstCounts[strKey] = 0;
    //}

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
#if 0
    createTestGroupModifyStandardShapesLines(pGrpAddStandardShapes, io_idxGroup, i_drawingSize);
#endif
    createTestGroupModifyStandardShapesGroups(pGrpAddStandardShapes, io_idxGroup, i_drawingSize);

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

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;

    bool bYAxisTopDown = (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : i_drawingSize.metricImageCoorsDecimals();

    ZS::Test::CTestStep* pTestStep = nullptr;
    int idxStep = 0;
    QString strGraphObjName;
    QStringList strlstExpectedValues;

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    // Small Plus Sign
    //--------------

    ZS::Test::CTestStepGroup* pGrpAddSmallPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " SmallPlusSign",
        /* pTSGrpParent */ pGrpAddLines );

    // Vertical Line
    //--------------

    strGraphObjName = "SmallPlusSign-VerticalLine";
    //s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    QPointF ptP1SmallPlusSignVerticalLine(275.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptP2SmallPlusSignVerticalLine(275.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptPosSmallPlusSignVerticalLine(275.0, 275.0);
    QPointF ptP1PosSmallPlusSignVerticalLine(0.0, -25.0);
    QPointF ptP2PosSmallPlusSignVerticalLine(0.0, 25.0);
    double fLengthSmallPlusSignVerticalLine = 50.0;
    CPhysValLine physValLineSmallPlusSignVerticalLine(*m_pDrawingScene, ptP1SmallPlusSignVerticalLine, ptP2SmallPlusSignVerticalLine);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjName + ")",
        /* pGrpParent      */ pGrpAddSmallPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", ptP1SmallPlusSignVerticalLine);
    pTestStep->setConfigValue("P2", ptP2SmallPlusSignVerticalLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p1 {" + qPoint2Str(ptP1PosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p2 {" + qPoint2Str(ptP2PosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".length: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + qPoint2Str(ptPosSmallPlusSignVerticalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Horizontal Line
    //----------------

    strGraphObjName = "SmallPlusSign-HorizontalLine";
    //s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    QPointF ptP1SmallPlusSignHorizontalLine(250.0, bYAxisTopDown ? 275.0 : 325.0);
    QPointF ptP2SmallPlusSignHorizontalLine(300.0, bYAxisTopDown ? 275.0 : 325.0);
    QPointF ptPosSmallPlusSignHorizontalLine(275.0, 275.0);
    QPointF ptP1PosSmallPlusSignHorizontalLine(-25.0, 0.0);
    QPointF ptP2PosSmallPlusSignHorizontalLine(25.0, 0.0);
    double fLengthSmallPlusSignHorizontalLine = 50.0;
    CPhysValLine physValLineSmallPlusSignHorizontalLine(*m_pDrawingScene, ptP1SmallPlusSignHorizontalLine, ptP2SmallPlusSignHorizontalLine);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjName + ")",
        /* pGrpParent      */ pGrpAddSmallPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", ptP1SmallPlusSignHorizontalLine);
    pTestStep->setConfigValue("P2", ptP2SmallPlusSignHorizontalLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p1 {" + qPoint2Str(ptP1PosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p2 {" + qPoint2Str(ptP2PosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".length: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Big Plus Sign
    //--------------

    ZS::Test::CTestStepGroup* pGrpAddBigPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " BigPlusSign",
        /* pTSGrpParent */ pGrpAddLines );

    // Vertical Line
    //--------------

    strGraphObjName = "BigPlusSign-VerticalLine";
    //s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    QPointF ptP1BigPlusSignVerticalLine(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptP2BigPlusSignVerticalLine(300.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptPosBigPlusSignVerticalLine(300.0, 300.0);
    QPointF ptP1PosBigPlusSignVerticalLine(0.0, -50.0);
    QPointF ptP2PosBigPlusSignVerticalLine(0.0, 50.0);
    double fLengthBigPlusSignVerticalLine = 100.0;
    CPhysValLine physValLineBigPlusSignVerticalLine(*m_pDrawingScene, ptP1BigPlusSignVerticalLine, ptP2BigPlusSignVerticalLine);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjName + ")",
        /* pGrpParent      */ pGrpAddBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", ptP1BigPlusSignVerticalLine);
    pTestStep->setConfigValue("P2", ptP2BigPlusSignVerticalLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p1 {" + qPoint2Str(ptP1PosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p2 {" + qPoint2Str(ptP2PosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".length: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + qPoint2Str(ptPosBigPlusSignVerticalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Horizontal Line
    //----------------

    strGraphObjName = "BigPlusSign-HorizontalLine";
    //s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    QPointF ptP1BigPlusSignHorizontalLine(250.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptP2BigPlusSignHorizontalLine(350.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptPosBigPlusSignHorizontalLine(300.0, 300.0);
    QPointF ptP1PosBigPlusSignHorizontalLine(-50.0, 0.0);
    QPointF ptP2PosBigPlusSignHorizontalLine(50.0, 0.0);
    double fLengthBigPlusSignHorizontalLine = 100.0;
    CPhysValLine physValLineBigPlusSignHorizontalLine(*m_pDrawingScene, ptP1BigPlusSignHorizontalLine, ptP2BigPlusSignHorizontalLine);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjName + ")",
        /* pGrpParent      */ pGrpAddBigPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", ptP1BigPlusSignHorizontalLine);
    pTestStep->setConfigValue("P2", ptP2BigPlusSignHorizontalLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p1 {" + qPoint2Str(ptP1PosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p2 {" + qPoint2Str(ptP2PosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".length: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + qPoint2Str(ptPosBigPlusSignHorizontalLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Checkmark
    //----------
#if 0
    ZS::Test::CTestStepGroup* pGrpAddCheckmark = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Checkmark",
        /* pTSGrpParent */ pGrpAddLines );

    // Left Line
    //----------

    strGraphObjName = "Checkmark-LeftLine";
    //s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    QPointF ptP1CheckmarkLeftLine(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptP2CheckmarkLeftLine(325.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptPosCheckmarkLeftLine(287.5, 300.0);
    QPointF ptP1PosCheckmarkLeftLine(-37.5, -50.0);
    QPointF ptP2PosCheckmarkLeftLine(37.5, 50.0);
    double fLengthCheckmarkLeftLine = 125.0;
    CPhysValLine physValLineCheckmarkLeftLine(*m_pDrawingScene, ptP1CheckmarkLeftLine, ptP2CheckmarkLeftLine);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjName + ")",
        /* pGrpParent      */ pGrpAddCheckmark,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", ptP1CheckmarkLeftLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkLeftLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p1 {" + qPoint2Str(ptP1PosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p2 {" + qPoint2Str(ptP2PosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".length: " + QString::number(Math::round2Nearest(fLengthCheckmarkLeftLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + qPoint2Str(ptPosCheckmarkLeftLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLine {" + physValLineCheckmarkLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkLeftLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 53.1 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Right Line
    //-----------

    strGraphObjName = "Checkmark-RightLine";
    //s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    QPointF ptP1CheckmarkRightLine(325.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptP2CheckmarkRightLine(350.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptPosCheckmarkRightLine(337.5, 337.5);
    QPointF ptP1PosCheckmarkRightLine(-12.5, 12.5);
    QPointF ptP2PosCheckmarkRightLine(12.5, -12.5);
    double fLengthCheckmarkRightLine = 35.4;
    CPhysValLine physValLineCheckmarkRightLine(*m_pDrawingScene, ptP1CheckmarkRightLine, ptP2CheckmarkRightLine);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjName + ")",
        /* pGrpParent      */ pGrpAddCheckmark,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", ptP1CheckmarkRightLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkRightLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p1 {" + qPoint2Str(ptP1PosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p2 {" + qPoint2Str(ptP2PosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".length: " + QString::number(Math::round2Nearest(fLengthCheckmarkRightLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + qPoint2Str(ptPosCheckmarkRightLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLine {" + physValLineCheckmarkRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkRightLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 315.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Small Rectangle
    //----------------

    ZS::Test::CTestStepGroup* pGrpAddSmallRect = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " SmallRect",
        /* pTSGrpParent */ pGrpAddLines );

    // Top Line
    //---------

    strGraphObjName = "SmallRect-TopLine";
    //s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    QPointF ptP1SmallRectTopLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectTopLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectTopLine(325.0, 270.0);
    QPointF ptP1PosSmallRectTopLine(-5.0, 0.0);
    QPointF ptP2PosSmallRectTopLine(5.0, 0.0);
    double fLengthSmallRectTopLine = 10.0;
    CPhysValLine physValLineSmallRectTopLine(*m_pDrawingScene, ptP1SmallRectTopLine, ptP2SmallRectTopLine);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjName + ")",
        /* pGrpParent      */ pGrpAddSmallRect,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", ptP1SmallRectTopLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectTopLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosSmallRectTopLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p1 {" + qPoint2Str(ptP1PosSmallRectTopLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p2 {" + qPoint2Str(ptP2PosSmallRectTopLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".length: " + QString::number(Math::round2Nearest(fLengthSmallRectTopLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + qPoint2Str(ptPosSmallRectTopLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLine {" + physValLineSmallRectTopLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectTopLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Right Line
    //-----------

    strGraphObjName = "SmallRect-RightLine";
    //s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    QPointF ptP1SmallRectRightLine(330.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptP2SmallRectRightLine(330.0, bYAxisTopDown ? 280.0 : 320.0);
    QPointF ptPosSmallRectRightLine(330.0, 275.0);
    QPointF ptP1PosSmallRectRightLine(0.0, -5.0);
    QPointF ptP2PosSmallRectRightLine(0.0, 5.0);
    double fLengthSmallRectRightLine = 10.0;
    CPhysValLine physValLineSmallRectRightLine(*m_pDrawingScene, ptP1SmallRectRightLine, ptP2SmallRectRightLine);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjName + ")",
        /* pGrpParent      */ pGrpAddSmallRect,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", ptP1SmallRectRightLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectRightLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosSmallRectRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p1 {" + qPoint2Str(ptP1PosSmallRectRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p2 {" + qPoint2Str(ptP2PosSmallRectRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".length: " + QString::number(Math::round2Nearest(fLengthSmallRectRightLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + qPoint2Str(ptPosSmallRectRightLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLine {" + physValLineSmallRectRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectRightLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Bottom Line
    //------------

    strGraphObjName = "SmallRect-BottomLine";
    //s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    QPointF ptP1SmallRectBottomLine(330.0, bYAxisTopDown ? 280.0 : 320.0);
    QPointF ptP2SmallRectBottomLine(320.0, bYAxisTopDown ? 280.0 : 320.0);
    QPointF ptPosSmallRectBottomLine(325.0, 280.0);
    QPointF ptP1PosSmallRectBottomLine(5.0, 0.0);
    QPointF ptP2PosSmallRectBottomLine(-5.0, 0.0);
    double fLengthSmallRectBottomLine = 10.0;
    CPhysValLine physValLineSmallRectBottomLine(*m_pDrawingScene, ptP1SmallRectBottomLine, ptP2SmallRectBottomLine);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjName + ")",
        /* pGrpParent      */ pGrpAddSmallRect,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", ptP1SmallRectBottomLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectBottomLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosSmallRectBottomLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p1 {" + qPoint2Str(ptP1PosSmallRectBottomLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p2 {" + qPoint2Str(ptP2PosSmallRectBottomLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".length: " + QString::number(Math::round2Nearest(fLengthSmallRectBottomLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + qPoint2Str(ptPosSmallRectBottomLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLine {" + physValLineSmallRectBottomLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectBottomLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 180.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Left Line
    //----------

    strGraphObjName = "SmallRect-LeftLine";
    //s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    QPointF ptP1SmallRectLeftLine(320.0, bYAxisTopDown ? 280.0 : 320.0);
    QPointF ptP2SmallRectLeftLine(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QPointF ptPosSmallRectLeftLine(320.0, 275.0);
    QPointF ptP1PosSmallRectLeftLine(0.0, 5.0);
    QPointF ptP2PosSmallRectLeftLine(0.0, -5.0);
    double fLengthSmallRectLeftLine = 10.0;
    CPhysValLine physValLineSmallRectLeftLine(*m_pDrawingScene, ptP1SmallRectLeftLine, ptP2SmallRectLeftLine);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjName + ")",
        /* pGrpParent      */ pGrpAddSmallRect,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("P1", ptP1SmallRectLeftLine);
    pTestStep->setConfigValue("P2", ptP2SmallRectLeftLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosSmallRectLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p1 {" + qPoint2Str(ptP1PosSmallRectLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p2 {" + qPoint2Str(ptP2PosSmallRectLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".length: " + QString::number(Math::round2Nearest(fLengthSmallRectLeftLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + qPoint2Str(ptPosSmallRectLeftLine, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLine {" + physValLineSmallRectLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallRectLeftLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 270.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
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

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);
    //QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    //QString strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;

    bool bYAxisTopDown = (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : i_drawingSize.metricImageCoorsDecimals();

    ZS::Test::CTestStep* pTestStep = nullptr;
    int idxStep = 0;

    QStringList strlstGraphObjsAddToGroup;
    QStringList strlstExpectedValues;

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    // Small Rectangle
    //----------------

    QString strGraphObjGroupName = "SmallRect";
#if 0
    //s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjGroupName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjGroupName + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    QString strGraphObjNameSmallRectTopLine = "SmallRect-TopLine";
    QString strGraphObjNameSmallRectRightLine = "SmallRect-RightLine";
    QString strGraphObjNameSmallRectBottomLine = "SmallRect-BottomLine";
    QString strGraphObjNameSmallRectLeftLine = "SmallRect-LeftLine";
    QStringList strlstGraphObjsAddToGroup;
    strlstGraphObjsAddToGroup.append(strGraphObjNameSmallRectTopLine);
    strlstGraphObjsAddToGroup.append(strGraphObjNameSmallRectRightLine);
    strlstGraphObjsAddToGroup.append(strGraphObjNameSmallRectBottomLine);
    strlstGraphObjsAddToGroup.append(strGraphObjNameSmallRectLeftLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupName);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    // Group
    QPointF ptPosSmallRect(325.0, 275.0);
    QPointF ptTLSmallRect(320.0, bYAxisTopDown ? 270.0 : 330.0);
    QSizeF sizeSmallRect(10.0, 10.0);
    QRectF rectBoundingSmallRect(QPointF(-5.0, -5.0), sizeSmallRect);
    CPhysValRect physValRectSmallRect(*m_pDrawingScene, ptTLSmallRect, sizeSmallRect);
    QStringList strlstExpectedValues;
    strlstExpectedValues.append(strGraphObjGroupName + ".pos {" + qPoint2Str(ptPosSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjGroupName + ".boundingRect {" + qRect2Str(rectBoundingSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjGroupName + ".position {" + qPoint2Str(ptPosSmallRect, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupName + ".getRect {" + physValRectSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupName + ".getSize {" + physValRectSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
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
#endif

    // Checkmark
    //----------
#if 0
    strGraphObjGroupName = "Checkmark";
    //s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjGroupName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjGroupName + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    QString strGraphObjNameCheckmarkLeftLine = "Checkmark-LeftLine";
    QString strGraphObjNameCheckmarkRightLine = "Checkmark-RightLine";
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(strGraphObjNameCheckmarkLeftLine);
    strlstGraphObjsAddToGroup.append(strGraphObjNameCheckmarkRightLine);
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupName);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    // Group
    QPointF ptPosCheckmark(300.0, 300.0);
    QPointF ptTLCheckmark(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QSizeF sizeCheckmark(100.0, 100.0);
    QRectF rectBoundingCheckmark(QPointF(-50.0, -50.0), sizeCheckmark);
    CPhysValRect physValRectCheckmark(*m_pDrawingScene, ptTLCheckmark, sizeCheckmark);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupName + ".pos {" + qPoint2Str(ptPosCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjGroupName + ".boundingRect {" + qRect2Str(rectBoundingCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjGroupName + ".position {" + qPoint2Str(ptPosCheckmark, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupName + ".getRect {" + physValRectCheckmark.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupName + ".getSize {" + physValRectCheckmark.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
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
#endif
    // PlusSign Lines, SmallRect, Checkmark
    //--------------------------------------
#if 0
    strGraphObjGroupName = "PlusSign-Checkmark-SmallRect";
    //s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjGroupName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjGroupName + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
#endif
    QString strGraphObjNameSmallPlusSignVerticalLine = "SmallPlusSign-VerticalLine";
    QString strGraphObjNameSmallPlusSignHorizontalLine = "SmallPlusSign-HorizontalLine";
    QString strGraphObjNameBigPlusSignVerticalLine = "BigPlusSign-VerticalLine";
    QString strGraphObjNameBigPlusSignHorizontalLine = "BigPlusSign-HorizontalLine";
#if 0
    QString strGraphObjNameSmallRect = "SmallRect";
    QString strGraphObjNameCheckmark = "Checkmark";
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(strGraphObjNamePlusSignVerticalLine);
    strlstGraphObjsAddToGroup.append(strGraphObjNamePlusSignHorizontalLine);
    strlstGraphObjsAddToGroup.append(strGraphObjNameSmallRect);
    strlstGraphObjsAddToGroup.append(strGraphObjNameCheckmark);
    pTestStep->setConfigValue("GraphObjName", strGraphObjGroupName);
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);
    // Group
    QPointF ptPosPlusSignLinesCheckmarkSmallRect(300.0, 300.0);
    QPointF ptTLPlusSignLinesCheckmarkSmallRect(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QSizeF sizePlusSignLinesCheckmarkSmallRect(100.0, 100.0);
    QRectF rectBoundingPlusSignLinesCheckmarkSmallRect(QPointF(-50.0, -50.0), sizePlusSignLinesCheckmarkSmallRect);
    CPhysValRect physValRectPlusSignLinesCheckmarkSmallRect(*m_pDrawingScene, ptTLPlusSignLinesCheckmarkSmallRect, sizePlusSignLinesCheckmarkSmallRect);
    QStringList strlstExpectedValues;
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjGroupName + ".pos {" + qPoint2Str(ptPosPlusSignLinesCheckmarkSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjGroupName + ".boundingRect {" + qRect2Str(rectBoundingPlusSignLinesCheckmarkSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjGroupName + ".position {" + qPoint2Str(ptPosPlusSignLinesCheckmarkSmallRect, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupName + ".getRect {" + physValRectPlusSignLinesCheckmarkSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupName + ".getSize {" + physValRectPlusSignLinesCheckmarkSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjGroupName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
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
    // SmallRect
    ptPosSmallRect = QPointF(25.0, -25.0);
    ptTLSmallRect = QPointF(70.0, bYAxisTopDown ? 20.0 : 80.0);
    sizeSmallRect = QSizeF(10.0, 10.0);
    rectBoundingSmallRect = QRectF(QPointF(-5.0, -5.0), sizeSmallRect);
    physValRectSmallRect = CPhysValRect(*m_pDrawingScene, ptTLSmallRect, sizeSmallRect);
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".pos {" + qPoint2Str(ptPosSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".boundingRect {" + qRect2Str(rectBoundingSmallRect) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".position {" + physValRectSmallRect.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".getRect {" + physValRectSmallRect.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".getSize {" + physValRectSmallRect.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallRect + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // Checkmark
    ptPosCheckmark = QPointF(0.0, 0.0);
    ptTLCheckmark = QPointF(0.0, bYAxisTopDown ? 0.0 : 100.0);
    sizeCheckmark = QSizeF(100.0, 100.0);
    rectBoundingCheckmark = QRectF(QPointF(-50.0, -50.0), sizeCheckmark);
    physValRectCheckmark = CPhysValRect(*m_pDrawingScene, ptTLCheckmark, sizeCheckmark);
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".pos {" + qPoint2Str(ptPosCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".boundingRect {" + qRect2Str(rectBoundingCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".position {" + physValRectCheckmark.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".getRect {" + physValRectCheckmark.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".getSize {" + physValRectCheckmark.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmark + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif
    // PlusSign (moving Vertical Line and Horizontal Line to new group)
    //-----------------------------------------------------------------

#if 0
    strGraphObjGroupName = "PlusSign";
#else
    strGraphObjGroupName = "PlusSigns";
#endif
    //s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strGraphObjGroupName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + strGraphObjGroupName + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
#if 0
    QString strGraphObjPathNameBigPlusSignVerticalLine = pIdxTree->buildPathStr("PlusSign-Checkmark-SmallRect", strGraphObjNameBigPlusSignVerticalLine);
    QString strGraphObjPathNameBigPlusSignHorizontalLine = pIdxTree->buildPathStr("PlusSign-Checkmark-SmallRect", strGraphObjNameBigPlusSignHorizontalLine);
#else
    QString strGraphObjPathNameSmallPlusSignVerticalLine = strGraphObjNameSmallPlusSignVerticalLine;
    QString strGraphObjPathNameSmallPlusSignHorizontalLine = strGraphObjNameSmallPlusSignHorizontalLine;
    QString strGraphObjPathNameBigPlusSignVerticalLine = strGraphObjNameBigPlusSignVerticalLine;
    QString strGraphObjPathNameBigPlusSignHorizontalLine = strGraphObjNameBigPlusSignHorizontalLine;
#endif
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

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpAddGroups->path());
    }
    return pGrpAddGroups;

} // createTestGroupAddStandardShapesGroups

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupModifyStandardShapesLines(
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
        /* strMethod    */ "createTestGroupModifyStandardShapesLines",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    bool bYAxisTopDown = (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : i_drawingSize.metricImageCoorsDecimals();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpModifyLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Modify Lines",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Checkmark
    //----------

    ZS::Test::CTestStepGroup* pGrpModifyCheckmarkLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Checkmark Lines",
        /* pTSGrpParent */ pGrpModifyLines );

    int idxStep = 0;

    // Left Line
    //----------

    QString strGroupNameTop = "PlusSign-Checkmark-SmallRect";
    QString strGroupNameCheckmark = "Checkmark";
    QString strGraphObjName = "Checkmark-LeftLine";
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameTop, strGroupNameCheckmark, strGraphObjName);

    QPointF ptP1CheckmarkLeftLine(25.0, bYAxisTopDown ? 25.0 : 75.0);
    QPointF ptP2CheckmarkLeftLine(60.0, bYAxisTopDown ? 75.0 : 25.0);
    QPointF ptPosCheckmarkLeftLine(-7.5, 0.0);
    QPointF ptP1PosCheckmarkLeftLine(-17.5, -25.0);
    QPointF ptP2PosCheckmarkLeftLine(17.5, 25.0);
    CPhysValPoint physValPointCenterCheckmarkLeftLine(*m_pDrawingScene, 43.0, 50.0);
    double fLengthCheckmarkLeftLine = 61.0;
    CPhysValLine physValLineCheckmarkLeftLine(*m_pDrawingScene, ptP1CheckmarkLeftLine, ptP2CheckmarkLeftLine);
    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjName + ".setLine()",
        /* strOperation    */ strGraphObjName + ".setLine()",
        /* pGrpParent      */ pGrpModifyCheckmarkLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("Method", "setLine");
    pTestStep->setConfigValue("P1", ptP1CheckmarkLeftLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkLeftLine);
    QStringList strlstExpectedValues;
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p1 {" + qPoint2Str(ptP1PosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p2 {" + qPoint2Str(ptP2PosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".length: " + QString::number(Math::round2Nearest(fLengthCheckmarkLeftLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + physValPointCenterCheckmarkLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLine {" + physValLineCheckmarkLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkLeftLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 55.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Right Line
    //-----------

    strGraphObjName = "Checkmark-RightLine";
    strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameTop, strGroupNameCheckmark, strGraphObjName);

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
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjName + ".setLine()",
        /* strOperation    */ strGraphObjName + ".setLine()",
        /* pGrpParent      */ pGrpModifyCheckmarkLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("Method", "setLine");
    pTestStep->setConfigValue("P1", ptP1CheckmarkRightLine);
    pTestStep->setConfigValue("P2", ptP2CheckmarkRightLine);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p1 {" + qPoint2Str(ptP1PosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".p2 {" + qPoint2Str(ptP2PosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".length: " + QString::number(Math::round2Nearest(fLengthCheckmarkRightLine, 1), 'f', 1) + " px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + physValPointCenterCheckmarkRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLine {" + physValLineCheckmarkRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkRightLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 315.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpModifyLines->path());
    }
    return pGrpModifyLines;
}

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupModifyStandardShapesGroups(
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
        /* strMethod    */ "createTestGroupModifyStandardShapesGroups",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    CUnit unit = i_drawingSize.unit();
    QString strUnit = unit.symbol();
    bool bUnitPixel = (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    int iDigits = bUnitPixel ? 0 : i_drawingSize.metricImageCoorsDecimals();
    bool bYAxisTopDown = (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    QString strGraphObjNameSmallRect = "SmallRect";
    QString strGraphObjNameCheckmark = "Checkmark";
    QString strGraphObjNamePlusSign = "PlusSign";

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    ZS::Test::CTestStepGroup* pGrpModifyGroups = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Modify Groups",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = nullptr;
    int idxStep = 0;

    QString strGraphObjName;
    QString strGraphObjKeyInTree;
    CPhysVal physValAngle(45.0, Units.Angle.Degree, 0.1);
    QStringList strlstExpectedValues;

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
        {"GraphObjName", "PlusSign-Checkmark-SmallRect"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "PlusSign-Checkmark-SmallRect")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"setPos", QPointF(225.0, 235.0)},
        {"ExpectedText", physValRectPlusSignLinesCheckmarkSmallRect.topLeft().toString()},
    });
    pTestStep->addDataRow({
        {"GraphObjName", "PlusSign-Checkmark-SmallRect"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "PlusSign-Checkmark-SmallRect")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"setPos", QPointF(380.0, 300.0)},
        {"ExpectedText", physValRectPlusSignLinesCheckmarkSmallRect.angle().toString()}
    });
    pTestStep->addDataRow({
        {"GraphObjName", "Checkmark"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "PlusSign-Checkmark-SmallRect", "Checkmark")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"setPos", QPointF(250.0, 260.0)},
        {"ExpectedText", physValRectCheckmark.topLeft().toString()}
    });
    pTestStep->addDataRow({
        {"GraphObjName", "Checkmark"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "PlusSign-Checkmark-SmallRect", "Checkmark")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"setPos", QPointF(340.0, 300.0)},
        {"ExpectedText", physValRectCheckmark.angle().toString()}
    });
    pTestStep->addDataRow({
        {"GraphObjName", "SmallRect"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "PlusSign-Checkmark-SmallRect", "SmallRect")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"setPos", QPointF(305.0, 255.0)},
        {"ExpectedText", physValRectSmallRect.topLeft().toString()}
    });
    pTestStep->addDataRow({
        {"GraphObjName", "SmallRect"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "PlusSign-Checkmark-SmallRect", "SmallRect")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"setPos", QPointF(350.0, 270.0)},
        {"ExpectedText", physValRectSmallRect.angle().toString()}
    });
#endif
    // Rotate SmallRect
    //-----------------
#if 0
    QString strGroupNameParent = "PlusSign-Checkmark-SmallRect";
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

    // Checkmark - resizeToContent
    //----------------------------

    strGroupNameParent = "PlusSign-Checkmark-SmallRect";
    QString strGroupNameParentKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameParent);
    strGraphObjName = "Checkmark";
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameParent, strGraphObjName);
    QString strGraphObjNameCheckmarkLeftLine = "Checkmark-LeftLine";
    QString strGraphObjNameCheckmarkRightLine = "Checkmark-RightLine";

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjName + ".resizeToContent()",
        /* strOperation    */ strGraphObjName + ".resizeToContent()",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "resizeToContent");
    // Group
    QPointF ptPosCheckmark(0.0, 0.0);
    ptTLCheckmark = QPointF(25.0, bYAxisTopDown ? 25.0 : 75.0);
    sizeCheckmark = QSizeF(50.0, 50.0);
    QRectF rectBoundingCheckmark(QPointF(-25.0, -25.0), sizeCheckmark);
    physValRectCheckmark = CPhysValRect(*m_pDrawingScene, ptTLCheckmark, sizeCheckmark);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".boundingRect {" + qRect2Str(rectBoundingCheckmark) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + physValRectCheckmark.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getRect {" + physValRectCheckmark.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getSize {" + physValRectCheckmark.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // Checkmark-LeftLine
    QPointF ptPosCheckmarkLeftLine(-7.5, 0.0);
    QRectF rectBoundingCheckmarkLeftLine(QPointF(-17.5, -25.0), QSizeF(35.0, 50.0));
    double fLengthCheckmarkLeftLine = 61.0;
    CPhysValLine physValLineCheckmarkLeftLine(*m_pDrawingScene, QPointF(0.0, 0.0), QPointF(35.0, 50.0));
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".pos {" + qPoint2Str(ptPosCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".boundingRect {" + qRect2Str(rectBoundingCheckmarkLeftLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".position {" + physValLineCheckmarkLeftLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".getLine {" + physValLineCheckmarkLeftLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkLeftLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkLeftLine + ".rotationAngle: 55.0 " + Math::c_strSymbolDegree);
    // Checkmark-RightLine
    QPointF ptPosCheckmarkRightLine(17.5, 17.5);
    QRectF rectBoundingCheckmarkRightLine(QPointF(-7.5, -7.5), QSizeF(15.0, 15.0));
    double fLengthCheckmarkRightLine = 21.0;
    CPhysValLine physValLineCheckmarkRightLine(*m_pDrawingScene, QPointF(35.0, 50.0), QPointF(50.0, 35.0));
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".pos {" + qPoint2Str(ptPosCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".boundingRect {" + qRect2Str(rectBoundingCheckmarkRightLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".position {" + physValLineCheckmarkRightLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".getLine {" + physValLineCheckmarkRightLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthCheckmarkRightLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameCheckmarkRightLine + ".rotationAngle: 315.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Move PlusSign Group to Group with SmallRect and Checkmark
    //----------------------------------------------------------

    strGroupNameParent = "PlusSign-Checkmark-SmallRect";
    strGroupNameParentKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameParent);
    strGraphObjName = "PlusSign";
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);
#endif
    QString strGraphObjNameSmallPlusSignVerticalLine = "SmallPlusSign-VerticalLine";
    QString strGraphObjNameSmallPlusSignHorizontalLine = "SmallPlusSign-HorizontalLine";
    QString strGraphObjNameBigPlusSignVerticalLine = "BigPlusSign-VerticalLine";
    QString strGraphObjNameBigPlusSignHorizontalLine = "BigPlusSign-HorizontalLine";
#if 0
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
        {"GraphObjName", "PlusSign-Checkmark-SmallRect"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "PlusSign-Checkmark-SmallRect")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"ExpectedValue", "PlusSign-Checkmark-SmallRect." + CGraphObj::c_strGeometryLabelNameTopLeft + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", "PlusSign-Checkmark-SmallRect"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "PlusSign-Checkmark-SmallRect")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"ExpectedValue", "PlusSign-Checkmark-SmallRect." + CGraphObj::c_strGeometryLabelNameAngle + " not found"}
    });
    pTestStep->addDataRow({
        {"GraphObjName", "Checkmark"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "PlusSign-Checkmark-SmallRect", "Checkmark")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"ExpectedValue", "Checkmark." + CGraphObj::c_strGeometryLabelNameTopLeft + " not found"},
    });
    pTestStep->addDataRow({
        {"GraphObjName", "Checkmark"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "PlusSign-Checkmark-SmallRect", "Checkmark")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"ExpectedValue", "Checkmark." + CGraphObj::c_strGeometryLabelNameAngle + " not found"},
    });
    pTestStep->addDataRow({
        {"GraphObjName", "SmallRect"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "PlusSign-Checkmark-SmallRect", "SmallRect")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameTopLeft},
        {"ExpectedValue", "SmallRect." + CGraphObj::c_strGeometryLabelNameTopLeft + " not found"},
    });
    pTestStep->addDataRow({
        {"GraphObjName", "SmallRect"},
        {"GraphObjKeyInTree", pIdxTree->buildKeyInTreeStr(strEntryType, "PlusSign-Checkmark-SmallRect", "SmallRect")},
        {"LabelName", CGraphObj::c_strGeometryLabelNameAngle},
        {"ExpectedValue", "SmallRect." + CGraphObj::c_strGeometryLabelNameAngle + " not found"},
    });
#endif

    // Resize top group PlusSign-Checkmark-SmallRect
    //----------------------------------------------

#if 0
    strGraphObjName = "PlusSign-Checkmark-SmallRect";
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
    // Group PlusSign-Checkmark-SmallRect
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

    // Resize PlusSigns
    //-----------------
#else
    strGraphObjName = "PlusSigns";
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    QPointF ptTLPlusSigns = QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QSizeF sizePlusSigns = QSizeF(400.0, 200.0);
    QPointF ptBRPlusSigns(650.0, bYAxisTopDown ? 450.0 : 150.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjName + ".setBottomRight(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* strOperation    */ strGraphObjName + ".setTopLeft(" + qPoint2Str(ptBRPlusSigns) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "setBottomRight");
    pTestStep->setConfigValue("BottomRight", ptBRPlusSigns);
    pTestStep->setConfigValue("BottomRight.unit", unit.symbol());
    // Group PlusSign-Checkmark-SmallRect
    QPointF ptPosPlusSign(450.0, 350.0);
    QRectF rectBoundingPlusSign(QPointF(-200.0, -100.0), sizePlusSigns);
    CPhysValRect physValRectPlusSign = CPhysValRect(*m_pDrawingScene, ptTLPlusSigns, sizePlusSigns);
    physValAngle.setVal(0.0);
    physValRectPlusSign.setAngle(physValAngle);
    strlstExpectedValues.clear();
    // PlusSigns
    strlstExpectedValues.append(strGraphObjName + ".pos {" + qPoint2Str(ptPosPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".boundingRect {" + qRect2Str(rectBoundingPlusSign) + "} px");
    strlstExpectedValues.append(strGraphObjName + ".position {" + physValRectPlusSign.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getRect {" + physValRectPlusSign.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".getSize {" + physValRectPlusSign.size().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-VerticalLine
    QPointF ptPosSmallPlusSignVerticalLine(-100.0, -50.0);
    QRectF rectBoundingSmallPlusSignVerticalLine(QPointF(0.0, -50.0), QSizeF(0.0, 100.0));
    double fLengthSmallPlusSignVerticalLine = 100.0;
    CPhysValLine physValLineSmallPlusSignVerticalLine(*m_pDrawingScene, QPointF(100.0, 0.0), QPointF(100.0, 100.0));
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".position {" + physValLineSmallPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLine {" + physValLineSmallPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // SmallPlusSign-HorizontalLine
    QPointF ptPosSmallPlusSignHorizontalLine(-100.0, -50.0);
    QRectF rectBoundingSmallPlusSignHorizontalLine(QPointF(-100.0, 0.0), QSizeF(200.0, 0.0));
    double fLengthSmallPlusSignHorizontalLine = 200.0;
    CPhysValLine physValLineSmallPlusSignHorizontalLine(*m_pDrawingScene, QPointF(0.0, 50.0), QPointF(200.0, 50.0));
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingSmallPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".position {" + physValLineSmallPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLine {" + physValLineSmallPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthSmallPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameSmallPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-VerticalLine
    QPointF ptPosBigPlusSignVerticalLine(0.0, 0.0);
    QRectF rectBoundingBigPlusSignVerticalLine(QPointF(0.0, -100.0), QSizeF(0.0, 200.0));
    double fLengthBigPlusSignVerticalLine = 200.0;
    CPhysValLine physValLineBigPlusSignVerticalLine(*m_pDrawingScene, QPointF(200.0, 0.0), QPointF(200.0, 200.0));
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignVerticalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".position {" + physValLineBigPlusSignVerticalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLine {" + physValLineBigPlusSignVerticalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignVerticalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignVerticalLine + ".rotationAngle: 90.0 " + Math::c_strSymbolDegree);
    // BigPlusSign-HorizontalLine
    QPointF ptPosBigPlusSignHorizontalLine(0.0, 0.0);
    QRectF rectBoundingBigPlusSignHorizontalLine(QPointF(-200.0, 0.0), QSizeF(400.0, 0.0));
    double fLengthBigPlusSignHorizontalLine = 400.0;
    CPhysValLine physValLineBigPlusSignHorizontalLine(*m_pDrawingScene, QPointF(0.0, 100.0), QPointF(400.0, 100.0));
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".pos {" + qPoint2Str(ptPosBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".boundingRect {" + qRect2Str(rectBoundingBigPlusSignHorizontalLine) + "} px");
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".position {" + physValLineBigPlusSignHorizontalLine.center().toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLine {" + physValLineBigPlusSignHorizontalLine.toString() + "} " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".getLength: " + QString::number(Math::round2Nearest(fLengthBigPlusSignHorizontalLine, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append(strGraphObjNameBigPlusSignHorizontalLine + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);
#endif

    // Rotate top group PlusSign-Checkmark-SmallRect
    //----------------------------------------------
#if 0
    strGraphObjName = "PlusSign-Checkmark-SmallRect";
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
    // Group PlusSign-Checkmark-SmallRect
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
#endif
    // Resize top group PlusSign-Checkmark-SmallRect
    //----------------------------------------------
#if 0
    strGraphObjName = "PlusSign-Checkmark-SmallRect";
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
    // Group PlusSign-Checkmark-SmallRect
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
#endif
    // Group2 (horizonal and vertical lines together with checkmark and rectangle group)
    //----------------------------------------------------------------------------------
#if 0
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
        mthTracer.setMethodReturn(pGrpModifyGroups->path());
    }
    return pGrpModifyGroups;
}
