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
#include "Test/TestConfig.h"

#include "Electricity/ZSDrawGraphObjElectricityResistor.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"
#include <QtCore/qdir.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Products::Draw;
using namespace ZS::Apps::Test::Draw;
using namespace ZS::Draw::Plugins::Electricity;


/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawPluginElectricityShapes(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawPluginElectricityShapes",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpAddPluginElectricityShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Electricity",
        /* pTSGrpParent */ i_pTestStepGroupParent);

#if TEST_ADD_OBJECTS_PLUGINS_ELECTRICITY_RESISTORS == 1
    createTestGroupDrawPluginElectricityShapesResistor(pGrpAddPluginElectricityShapes);
#endif
}

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawPluginElectricityShapesResistor(
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
        /* strMethod    */ "createTestGroupDrawPluginElectricityShapesResistor",
        /* strAddInfo   */ strMthInArgs );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = "Electricity";
    QString strGraphObjType = "Resistor";
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    bool bYAxisTopDown = (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    CUnit unit = drawingSize.unit();
    QString strUnit = unit.symbol();
    int iResultValuesPrecision = bUnitPixel ? 0 : drawingSize.metricImageCoorsDecimals();
    QFontMetricsF fontMetrics(m_pDrawingScene->font());

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

    ZS::Test::CTestStepGroup* pGrpResistorShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Resistors",
        /* pTSGrpParent */ i_pTestStepGroupParent);

    // Clear Drawing
    //--------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpResistorShapes,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)));
    pTestStep->setExpectedValue("");

    initInstCounts();
    initObjectCoors();

    // R1
    //-----

    strObjName = "DrawingScene";
    strMethod = "dropGraphObj";
    strMthArgs = c_strGraphObjNameElectricityResistor1;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpResistorShapes,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjByDropEvent(ZS::Test::CTestStep*)));
    strObjName = c_strGraphObjNameElectricityResistor1;
    m_hshGraphObjNameToKeys.insert(strObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strObjName));
    m_ptPosElectricityResistor1 = QPointF(200.0, 200.0);
    *m_pPhysValRectElectricityResistor1 = CGraphObjResistor::initialSize(m_pDrawingScene, unit);
    m_pPhysValRectElectricityResistor1->setCenter(CPhysValPoint(*m_pDrawingScene, m_ptPosElectricityResistor1, Units.Length.px));
    pTestStep->setConfigValue("GraphObjFactoryGroup", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("P1", m_ptPosElectricityResistor1);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForGroup(
        strObjName, m_ptPosElectricityResistor1, *m_pPhysValRectElectricityResistor1,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // R2
    //-----

    strObjName = "DrawingScene";
    strMethod = "dropGraphObj";
    strMthArgs = c_strGraphObjNameElectricityResistor2;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpResistorShapes,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjByDropEvent(ZS::Test::CTestStep*)));
    strObjName = c_strGraphObjNameElectricityResistor2;
    m_hshGraphObjNameToKeys.insert(strObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strObjName));
    m_ptPosElectricityResistor2 = QPointF(400.0, 200.0);
    *m_pPhysValRectElectricityResistor2 = CGraphObjResistor::initialSize(m_pDrawingScene, unit);
    m_pPhysValRectElectricityResistor2->setCenter(CPhysValPoint(*m_pDrawingScene, m_ptPosElectricityResistor2, Units.Length.px));
    pTestStep->setConfigValue("GraphObjFactoryGroup", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("P1", m_ptPosElectricityResistor2);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForGroup(
        strObjName, m_ptPosElectricityResistor2, *m_pPhysValRectElectricityResistor2,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: R1.P2 -> R2.P1
    //--------------------------------

    strGraphObjType = graphObjType2Str(EGraphObjTypeConnectionLine);
    strObjName = "DrawingScene";
    strMethod = "drawGraphObj";
    strMthArgs = c_strGraphObjNameConnectionLineR1P1R2P1;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpResistorShapes,
        /* szDoTestStepFct */ SLOT(doTestStepDrawGraphObjConnectionLine(ZS::Test::CTestStep*)));
    strObjName = c_strGraphObjNameConnectionLineR1P1R2P1;
    m_hshGraphObjNameToKeys.insert(strObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strObjName));
    QPointF ptScenePosCnctPtR1P2 = m_pPhysValRectElectricityResistor1->rightCenter().toQPointF(Units.Length.px);
    QPointF ptScenePosCnctPtR2P1 = m_pPhysValRectElectricityResistor2->leftCenter().toQPointF(Units.Length.px);
    QLineF lineR1P2R2P1(ptScenePosCnctPtR1P2, ptScenePosCnctPtR2P1);
    m_polygonConnectionLineR1P2R2P1 = QPolygonF({
        { ptScenePosCnctPtR1P2 - lineR1P2R2P1.center() },
        { ptScenePosCnctPtR2P1 - lineR1P2R2P1.center() },
        });
    *m_pPhysValPolygonConnectionLineR1P2R2P1 = CPhysValPolygon(*m_pDrawingScene, {
        {m_pPhysValRectElectricityResistor1->rightCenter()},
        {m_pPhysValRectElectricityResistor2->leftCenter()},
        });
    // We need a start position, from which we move to the first connection point.
    // The we add the position of the two connection points.
    // If more than 3 points are defined, the points from index 2 to the penultimate
    // index are additional polygon points.
    QPoint ptMouseStart = m_pPhysValRectElectricityResistor1->rightCenter().toQPointF(Units.Length.px).toPoint();
    ptMouseStart.setX(ptMouseStart.x() + 20);
    ptMouseStart.setY(ptMouseStart.y() - 20);
    QPolygon points({
        ptMouseStart,
        m_pPhysValRectElectricityResistor1->rightCenter().toQPointF(Units.Length.px).toPoint(),
        m_pPhysValRectElectricityResistor2->leftCenter().toQPointF(Units.Length.px).toPoint(),
        });
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("Points", points);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForConnectionLine(
        strObjName, "R1.P2", "R2.P1",
        m_polygonConnectionLineR1P2R2P1, *m_pPhysValPolygonConnectionLineR1P2R2P1,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Connection Line: CnctPt1 -> CnctPt2: setLineStyle(Color, SolidLine)
    //--------------------------------------------------------------------

    strMethod = "setDrawSettings";
    strMthArgs = "{LineStyle, blue, Solid}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpResistorShapes,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
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

    strMethod = "setDrawSettings";
    strMthArgs = "{LineEndPoint, Arrow}";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpResistorShapes,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawSettings(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(EGraphObjTypeConnectionLine));
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", m_hshGraphObjNameToKeys[strObjName]);
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
}
