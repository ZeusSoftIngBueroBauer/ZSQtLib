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
void CTest::createTestGroupDrawObjects(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawObjects",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpDrawStandardShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Draw " + CObjFactory::c_strGroupNameStandardShapes,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    createTestGroupDrawStandardShapes(pGrpDrawStandardShapes);
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapes(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapes",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStep* pTestStep = nullptr;

    // Lines
    //======

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
    createTestGroupDrawStandardShapesLinesSmallPlusSign(pGrpLinesSmallPlusSign);

    // BigPlusSign
    //------------

    ZS::Test::CTestStepGroup* pGrpLinesBigPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameBigPlusSign,
        /* pTSGrpParent */ pGrpLines );
    createTestGroupDrawStandardShapesLinesBigPlusSign(pGrpLinesBigPlusSign);

    // Checkmark
    //----------

    ZS::Test::CTestStepGroup* pGrpLinesCheckmark = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameCheckmark,
        /* pTSGrpParent */ pGrpLines );
    createTestGroupDrawStandardShapesLinesCheckmark(pGrpLinesCheckmark);

    // SmallRect
    //----------

    ZS::Test::CTestStepGroup* pGrpLinesSmallRect = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + c_strGraphObjNameSmallRect,
        /* pTSGrpParent */ pGrpLines );
    createTestGroupDrawStandardShapesLinesSmallRect(pGrpLinesSmallRect);

    createTestStepSaveLoadFile(pGrpLines);

    // Groups
    //=======

    ZS::Test::CTestStepGroup* pGrpGroups = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Groups",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // SmallPlusSign
    //--------------

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
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Add Shapes",
        /* pTSGrpParent */ pGrpGroupsSmallPlusSign );

    createTestGroupDrawStandardShapesGroupSmallPlusSign(pGrpGroupsSmallPlusSignDrawShapes);
    createTestGroupDrawStandardShapesGroupSmallPlusSignResize(pGrpGroupsSmallPlusSign);
    createTestStepSaveLoadFile(pGrpGroupsSmallPlusSign);

}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesLinesSmallPlusSign(
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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameSmallPlusSignVerticalLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallPlusSignVerticalLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallPlusSignVerticalLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSignVerticalLine));
    QPoint ptP1SmallPlusSignVerticalLine(275, 310);
    QPoint ptP2SmallPlusSignVerticalLine(275, 340);
    m_ptPosSmallPlusSignVerticalLine = QPointF(275.0, 325.0);
    m_lineSmallPlusSignVerticalLine = QLineF(QPointF(0.0, -15.0), QPointF(0.0, 15.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP1(QPointF(275.0, bYAxisTopDown ? 310.0 : 290.0));
    m_pPhysValLineSmallPlusSignVerticalLine->setP2(QPointF(275.0, bYAxisTopDown ? 340.0 : 260.0));
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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameSmallPlusSignHorizontalLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallPlusSignHorizontalLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallPlusSignHorizontalLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallPlusSignHorizontalLine));
    QPoint ptP1SmallPlusSignHorizontalLine(260, 325);
    QPoint ptP2SmallPlusSignHorizontalLine(290, 325);
    m_ptPosSmallPlusSignHorizontalLine = QPointF(275.0, 325.0);
    m_lineSmallPlusSignHorizontalLine = QLineF(QPointF(-15.0, 0.0), QPointF(15.0, 0.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP1(QPointF(260.0, bYAxisTopDown ? 325.0 : 275.0));
    m_pPhysValLineSmallPlusSignHorizontalLine->setP2(QPointF(290.0, bYAxisTopDown ? 325.0 : 275.0));
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
void CTest::createTestGroupDrawStandardShapesLinesBigPlusSign(
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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameBigPlusSignVerticalLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameBigPlusSignVerticalLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameBigPlusSignVerticalLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSignVerticalLine));
    QPoint ptP1BigPlusSignVerticalLine(300, 250);
    QPoint ptP2BigPlusSignVerticalLine(300, 350);
    m_ptPosBigPlusSignVerticalLine = QPointF(300.0, 300.0);
    m_lineBigPlusSignVerticalLine = QLineF(QPointF(0.0, -50.0), QPointF(0.0, 50.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP1(QPointF(300.0, bYAxisTopDown ? 250.0 : 350.0));
    m_pPhysValLineBigPlusSignVerticalLine->setP2(QPointF(300.0, bYAxisTopDown ? 350.0 : 250.0));
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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameBigPlusSignHorizontalLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameBigPlusSignHorizontalLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameBigPlusSignHorizontalLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameBigPlusSignHorizontalLine));
    QPoint ptP1BigPlusSignHorizontalLine(250, 300);
    QPoint ptP2BigPlusSignHorizontalLine(350, 300);
    m_ptPosBigPlusSignHorizontalLine = QPointF(300.0, 300.0);
    m_lineBigPlusSignHorizontalLine = QLineF(QPointF(-50.0, 0.0), QPointF(50.0, 0.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP1(QPointF(250.0, bYAxisTopDown ? 300.0 : 300.0));
    m_pPhysValLineBigPlusSignHorizontalLine->setP2(QPointF(350.0, bYAxisTopDown ? 300.0 : 300.0));
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
void CTest::createTestGroupDrawStandardShapesLinesCheckmark(
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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameCheckmarkLeftLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameCheckmarkLeftLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameCheckmarkLeftLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameCheckmarkLeftLine));
    QPoint ptP1CheckmarkLeftLine(250, 250);
    QPoint ptP2CheckmarkLeftLine(325, 350);
    m_ptPosCheckmarkLeftLine = QPointF(287.5, 300.0);
    m_lineCheckmarkLeftLine = QLineF(QPointF(-37.5, -50.0), QPointF(37.5, 50.0));
    m_pPhysValLineCheckmarkLeftLine->setP1(QPointF(250.0, bYAxisTopDown ? 250.0 : 350.0));
    m_pPhysValLineCheckmarkLeftLine->setP2(QPointF(325.0, bYAxisTopDown ? 350.0 : 250.0));
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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameCheckmarkRightLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameCheckmarkRightLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameCheckmarkRightLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameCheckmarkRightLine));
    QPoint ptP1CheckmarkRightLine(325, 350);
    QPoint ptP2CheckmarkRightLine(350, 325);
    m_ptPosCheckmarkRightLine = QPointF(337.5, 337.5);
    m_lineCheckmarkRightLine = QLineF(QPointF(-12.5, 12.5), QPointF(12.5, -12.5));
    m_pPhysValLineCheckmarkRightLine->setP1(QPointF(325.0, bYAxisTopDown ? 350.0 : 250.0));
    m_pPhysValLineCheckmarkRightLine->setP2(QPointF(350.0, bYAxisTopDown ? 325.0 : 275.0));
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
void CTest::createTestGroupDrawStandardShapesLinesSmallRect(
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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameSmallRectTopLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectTopLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallRectTopLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallRectTopLine));
    QPoint ptP1SmallRectTopLine(320, 270);
    QPoint ptP2SmallRectTopLine(330, 270);
    m_ptPosSmallRectTopLine = QPointF(325.0, 270.0);
    m_lineSmallRectTopLine = QLineF(QPointF(-5.0, 0.0), QPointF(5.0, 0.0));
    m_pPhysValLineSmallRectTopLine->setP1(QPointF(320.0, bYAxisTopDown ? 270.0 : 330.0));
    m_pPhysValLineSmallRectTopLine->setP2(QPointF(330.0, bYAxisTopDown ? 270.0 : 330.0));
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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameSmallRectRightLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectRightLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallRectRightLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallRectRightLine));
    QPoint ptP1SmallRectRightLine(330, 270);
    QPoint ptP2SmallRectRightLine(330, 280);
    m_ptPosSmallRectRightLine = QPointF(330.0, 275.0);
    m_lineSmallRectRightLine = QLineF(QPointF(0.0, -5.0), QPointF(0.0, 5.0));
    m_pPhysValLineSmallRectRightLine->setP1(QPointF(330.0, bYAxisTopDown ? 270.0 : 330.0));
    m_pPhysValLineSmallRectRightLine->setP2(QPointF(330.0, bYAxisTopDown ? 280.0 : 320.0));
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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameSmallRectBottomLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectBottomLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallRectBottomLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallRectBottomLine));
    QPoint ptP1SmallRectBottomLine(330, 280);
    QPoint ptP2SmallRectBottomLine(320, 280);
    m_ptPosSmallRectBottomLine = QPointF(325.0, 280.0);
    m_lineSmallRectBottomLine = QLineF(QPointF(5.0, 0.0), QPointF(-5.0, 0.0));
    m_pPhysValLineSmallRectBottomLine->setP1(QPointF(330.0, bYAxisTopDown ? 280.0 : 320.0));
    m_pPhysValLineSmallRectBottomLine->setP2(QPointF(320.0, bYAxisTopDown ? 280.0 : 320.0));
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
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Draw(" + c_strGraphObjNameSmallRectLeftLine + ")",
        /* strOperation    */ "DrawingScene.drawGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ", " + c_strGraphObjNameSmallRectLeftLine + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjLine(ZS::Test::CTestStep*)) );
    m_hshGraphObjNameToKeys.insert(c_strGraphObjNameSmallRectLeftLine, pIdxTree->buildKeyInTreeStr(strEntryType, c_strGraphObjNameSmallRectLeftLine));
    QPoint ptP1SmallRectLeftLine(320, 280);
    QPoint ptP2SmallRectLeftLine(320, 270);
    m_ptPosSmallRectLeftLine = QPointF(320.0, 275.0);
    m_lineSmallRectLeftLine = QLineF(QPointF(0.0, 5.0), QPointF(0.0, -5.0));
    m_pPhysValLineSmallRectLeftLine->setP1(QPointF(320.0, bYAxisTopDown ? 280.0 : 320.0));
    m_pPhysValLineSmallRectLeftLine->setP2(QPointF(320.0, bYAxisTopDown ? 270.0 : 330.0));
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
void CTest::createTestGroupDrawStandardShapesGroupSmallPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesGroupSmallPlusSign",
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
    QStringList strlstGraphObjsKeyInTreeGetResultValues;

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
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("P1", QPoint(258, 308));
    pTestStep->setConfigValue("P2", QPoint(292, 342));
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
void CTest::createTestGroupDrawStandardShapesGroupSmallPlusSignResize(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesGroupSmallPlusSignResize",
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
    CEnumSelectionPoint eSelPt;
    QPoint pt1SelPt;
    QPoint pt2SelPt;

    ZS::Test::CTestStepGroup* pGrpModifySmallPlusSign = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Modify " + c_strGraphObjNameSmallPlusSign,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Resize SmallPlusSign
    //---------------------

    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomRight : ESelectionPoint::TopRight;
    QSize sizeNewSmallPlusSign(30, 60);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameSmallPlusSign + ".setSize(" + qSize2Str(m_sizeSmallPlusSign) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameSmallPlusSign + ".setSize(" + qSize2Str(m_sizeSmallPlusSign) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifySmallPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjBySceneMouseEvents(ZS::Test::CTestStep*)) );
    int dx = sizeNewSmallPlusSign.width() - m_sizeSmallPlusSign.width();
    int dy = sizeNewSmallPlusSign.height() - m_sizeSmallPlusSign.height();
    if (bYAxisTopDown) {
        pt1SelPt = QPoint(m_ptPosSmallPlusSign.x() + m_sizeSmallPlusSign.width()/2, m_ptPosSmallPlusSign.y() + m_sizeSmallPlusSign.height()/2);
        pt2SelPt = QPoint(pt1SelPt.x() + dx, pt1SelPt.y() + dy);
    }
    else {
        pt1SelPt = QPoint(m_ptPosSmallPlusSign.x() + m_sizeSmallPlusSign.width()/2, m_ptPosSmallPlusSign.y() - m_sizeSmallPlusSign.height()/2);
        pt2SelPt = QPoint(pt1SelPt.x() + dx, pt1SelPt.y() - dy);
    }
    m_sizeSmallPlusSign = sizeNewSmallPlusSign;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign]);
    pTestStep->setConfigValue("Method", "moveSelectionPoint");
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
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

    eSelPt = bYAxisTopDown ? ESelectionPoint::BottomCenter : ESelectionPoint::TopCenter;
    sizeNewSmallPlusSign.setHeight(120.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + c_strGraphObjNameSmallPlusSign + ".setHeight(" + QString::number(m_sizeSmallPlusSign.height()) + " " + unit.symbol() + ")",
        /* strOperation    */ c_strGraphObjNameSmallPlusSign + ".setHeight(" + QString::number(m_sizeSmallPlusSign.height()) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpModifySmallPlusSign,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjBySceneMouseEvents(ZS::Test::CTestStep*)) );
    dx = sizeNewSmallPlusSign.width() - m_sizeSmallPlusSign.width();
    if (dx == 0) dx += 4; // simulate not exactly moved vertically (if width would be changed)
    dy = sizeNewSmallPlusSign.height() - m_sizeSmallPlusSign.height();
    if (dy == 0) dy -= 5; // simulate not exactly moved horizontally (if height would be changed)
    if (bYAxisTopDown) {
        pt1SelPt = QPoint(m_ptPosSmallPlusSign.x(), m_ptPosSmallPlusSign.y() + m_sizeSmallPlusSign.height()/2);
        pt2SelPt = QPoint(pt1SelPt.x() + dx, pt1SelPt.y() + dy);
    }
    else {
        pt1SelPt = QPoint(m_ptPosSmallPlusSign.x(), m_ptPosSmallPlusSign.y() - m_sizeSmallPlusSign.height()/2);
        pt2SelPt = QPoint(pt1SelPt.x() + dx, pt1SelPt.y() - dy);
    }
    m_sizeSmallPlusSign = sizeNewSmallPlusSign;
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeGroup));
    pTestStep->setConfigValue("GraphObjName", c_strGraphObjNameSmallPlusSign);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[c_strGraphObjNameSmallPlusSign]);
    pTestStep->setConfigValue("Method", "moveSelectionPoint");
    pTestStep->setConfigValue("SelectionPoint", eSelPt.toString());
    pTestStep->setConfigValue("P1", pt1SelPt);
    pTestStep->setConfigValue("P2", pt2SelPt);
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

#if 0
//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupShowAndMoveGeometryLabel(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent,
    const QString& i_strGraphObjName, const QString& i_strLabelName,
    const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", ObjName: " + i_strGraphObjName +
                       ", Label: " + i_strLabelName +
                       ", MousePress {" + qPoint2Str(i_ptMousePress) + "}" +
                       ", MouseRelease {" + qPoint2Str(i_ptMouseRelease) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupShowAndMoveGeometryLabel",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStep* pTestStep = nullptr;

    ZS::Test::CTestStepGroup* pGrpShowAndMoveLabel = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " " + i_strGraphObjName + " Geometry Label " + i_strLabelName,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + i_strGraphObjName + ".showGeometryLabel(" + i_strLabelName + ")",
        /* strOperation    */ "GraphObj.showGeometryLabel(" + i_strGraphObjName + ", " + i_strLabelName + ")",
        /* pGrpParent      */ pGrpShowAndMoveLabel,
        /* szDoTestStepFct */ SLOT(doTestStepShowGeometryLabel(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", i_strGraphObjName);
    pTestStep->setConfigValue("LabelName", i_strLabelName);

    addTestStepsDrawMouseEventsMovePressMoveRelease(
        pGrpShowAndMoveLabel, i_ptMousePress, i_ptMouseRelease);
}
#endif

/*==============================================================================
protected: // instance methods
==============================================================================*/

#if 0
//------------------------------------------------------------------------------
void CTest::addTestStepsDrawMouseEventsMovePressMoveRelease(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent,
    const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", MousePress {" + qPoint2Str(i_ptMousePress) + "}" +
                       ", MouseRelease {" + qPoint2Str(i_ptMouseRelease) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addTestStepsDrawMouseEventsMovePressMoveRelease",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStep* pTestStep = nullptr;

    // First move the cursor on the object so that the object is hit.
    QPoint ptMouseMovePos = i_ptMousePress;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
    pTestStep->setConfigValue("MousePos", ptMouseMovePos);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " MousePressEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMouseMovePos);

    bool bMoveRight = i_ptMouseRelease.x() > i_ptMousePress.x();
    bool bMoveLeft = i_ptMouseRelease.x() < i_ptMousePress.x();
    bool bMoveDown = i_ptMouseRelease.y() > i_ptMousePress.y();
    bool bMoveUp = i_ptMouseRelease.y() < i_ptMousePress.y();
    QSize sizeMouseMoveDist = QSize(
        (i_ptMouseRelease.x() - i_ptMousePress.x()) / 2,
        (i_ptMouseRelease.y() - i_ptMousePress.y()) / 2);
    int iMoves = 0;
    while (iMoves < 10) {
        ptMouseMovePos.setX(ptMouseMovePos.x() + sizeMouseMoveDist.width());
        ptMouseMovePos.setY(ptMouseMovePos.y() + sizeMouseMoveDist.height());
        if (bMoveRight && ptMouseMovePos.x() > (i_ptMouseRelease.x()-2)) {
            ptMouseMovePos.setX(i_ptMouseRelease.x());
        }
        else if (bMoveLeft && ptMouseMovePos.x() < (i_ptMouseRelease.x()+2)) {
            ptMouseMovePos.setX(i_ptMouseRelease.x());
        }
        if (bMoveDown && ptMouseMovePos.y() > (i_ptMouseRelease.y()-2)) {
            ptMouseMovePos.setY(i_ptMouseRelease.y());
        }
        else if (bMoveUp && ptMouseMovePos.y() < (i_ptMouseRelease.y()+2)) {
            ptMouseMovePos.setY(i_ptMouseRelease.y());
        }
        pTestStep = new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* pGrpParent      */ i_pTestStepGroupParent,
            /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
        pTestStep->setConfigValue("MousePos", ptMouseMovePos);
        if (bMoveRight && ptMouseMovePos.x() >= i_ptMouseRelease.x()) {
            break;
        }
        else if (bMoveLeft && ptMouseMovePos.x() <= i_ptMouseRelease.x()) {
            break;
        }
        if (bMoveDown && ptMouseMovePos.y() >= i_ptMouseRelease.y()) {
            break;
        }
        else if (bMoveUp && ptMouseMovePos.y() <= i_ptMouseRelease.y()) {
            break;
        }
        ++iMoves;
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " MouseReleaseEvent(" + qPoint2Str(i_ptMouseRelease) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" + qPoint2Str(i_ptMouseRelease) + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", i_ptMouseRelease);

} // addTestStepsDrawMouseEventsMovePressMoveRelease
#endif
