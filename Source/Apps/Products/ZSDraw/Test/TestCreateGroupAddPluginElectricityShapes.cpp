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
void CTest::createTestGroupAddPluginElectricityShapes(ZS::Test::CTestStepGroup* i_pTestStepGroupParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDrawTestSteps, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDrawTestSteps,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupAddPluginElectricityShapes",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpAddPluginElectricityShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(ZS::Test::CTestStepGroup::testGroupCount()) + " Electricity",
        /* pTSGrpParent */ i_pTestStepGroupParent);

#if TEST_ADD_OBJECTS_PLUGINS_ELECTRICITY_RESISTORS == 1
    createTestGroupAddPluginElectricityShapesResistor(pGrpAddPluginElectricityShapes);
#endif
}

//------------------------------------------------------------------------------
void CTest::createTestGroupAddPluginElectricityShapesResistor(
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
        /* strMethod    */ "createTestGroupAddPluginElectricityShapesResistor",
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
    strMethod = "addGraphObj";
    strMthArgs = c_strGraphObjNameElectricityResistor1;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpResistorShapes,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)));
    strObjName = c_strGraphObjNameElectricityResistor1;
    m_hshGraphObjNameToKeys.insert(strObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strObjName));
    m_ptPosElectricityResistor1 = QPointF(200.0, 200.0);
    *m_pPhysValRectElectricityResistor1 = CGraphObjResistor::initialSize(m_pDrawingScene, unit);
    m_pPhysValRectElectricityResistor1->setCenter(CPhysValPoint(*m_pDrawingScene, m_ptPosElectricityResistor1, Units.Length.px));
    pTestStep->setConfigValue("GraphObjFactoryGroup", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("Rect", m_pPhysValRectElectricityResistor1->toQRectF());
    pTestStep->setConfigValue("Rect.Unit", strUnit);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForGroup(
        strObjName, m_ptPosElectricityResistor1, *m_pPhysValRectElectricityResistor1,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);

    // R2
    //-----

    strObjName = "DrawingScene";
    strMethod = "addGraphObj";
    strMthArgs = c_strGraphObjNameElectricityResistor2;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(ZS::Test::CTestStep::testStepCount()) + " " + strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* strOperation    */ strObjName + "." + strMethod + "(" + strMthArgs + ")",
        /* pGrpParent      */ pGrpResistorShapes,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)));
    strObjName = c_strGraphObjNameElectricityResistor2;
    m_hshGraphObjNameToKeys.insert(strObjName, pIdxTree->buildKeyInTreeStr(strEntryType, strObjName));
    m_ptPosElectricityResistor1 = QPointF(400.0, 200.0);
    *m_pPhysValRectElectricityResistor1 = CGraphObjResistor::initialSize(m_pDrawingScene, unit);
    m_pPhysValRectElectricityResistor1->setCenter(CPhysValPoint(*m_pDrawingScene, m_ptPosElectricityResistor1, Units.Length.px));
    pTestStep->setConfigValue("GraphObjFactoryGroup", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strObjName);
    pTestStep->setConfigValue("Rect", m_pPhysValRectElectricityResistor1->toQRectF());
    pTestStep->setConfigValue("Rect.Unit", strUnit);
    pTestStep->setConfigValue("ResultValuesPrecision", iResultValuesPrecision);
    strlstExpectedValues.clear();
    strlstExpectedValues.append(resultValuesForGroup(
        strObjName, m_ptPosElectricityResistor1, *m_pPhysValRectElectricityResistor1,
        false, iResultValuesPrecision));
    pTestStep->setExpectedValues(strlstExpectedValues);
}
