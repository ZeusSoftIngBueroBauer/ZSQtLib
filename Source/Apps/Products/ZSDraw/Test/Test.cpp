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
#include "MainWindow.h"
#include "WidgetCentral.h"

#include "Electricity/ZSDrawGraphObjElectricityResistor.h"
#include "Electricity/ZSDrawGraphObjElectricityCapacitor.h"
#include "Electricity/ZSDrawGraphObjElectricityInductor.h"
#include "Electricity/ZSDrawGraphObjElectricitySwitch.h"
#include "Electricity/ZSDrawGraphObjElectricityTransistor.h"
#include "QtWidgets/ZSDrawGraphObjWdgtCheckBox.h"
#include "QtWidgets/ZSDrawGraphObjWdgtComboBox.h"
#include "QtWidgets/ZSDrawGraphObjWdgtGroupBox.h"
#include "QtWidgets/ZSDrawGraphObjWdgtLabel.h"
#include "QtWidgets/ZSDrawGraphObjWdgtLineEdit.h"
#include "QtWidgets/ZSDrawGraphObjWdgtPushButton.h"

#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingView.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjEllipse.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjImage.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPoint.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPolygon.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPolyline.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjRect.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjText.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtCore/qdir.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qstandardpaths.h>
#include <QtCore/qtimer.h>

#include <QtGui/qevent.h>

#if QT_VERSION < 0x050000
#include <QtGui/qaction.h>
#else
#include <QtWidgets/qaction.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Draw::Electricity;
using namespace ZS::Draw::QtWidgets;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Products::Draw;
using namespace ZS::Apps::Test::Draw;


/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest() :
//------------------------------------------------------------------------------
    ZS::Test::CTest(NameSpace(), "theInst"),
    m_pMainWindow(nullptr),
    m_pDrawingView(nullptr),
    m_pDrawingScene(nullptr)
    //m_ptDrawingSceneMousePos(),
    //m_ptDrawingViewMousePos(),
    //m_pGraphObjCreating(nullptr),
    //m_pGraphObjSelected(nullptr),
    //m_iTestStepLev1(0),
    //m_iTestStepLev2(0),
    //m_iTestStepLev3(0),
    //m_iTestStepLev4(0),
    //m_fAngle_degree(0.0),
    //m_fxOffsHor(0.0),
    //m_fyOffsHor(0.0),
    //m_iPointsCount(0),
    //m_iLinesCount(0),
    //m_iRectanglesCount(0),
    //m_iEllipsesCount(0),
    //m_iPolylinesCount(0),
    //m_iPolygonsCount(0),
    //m_iTextsCount(0),
    //m_iImagesCount(0),
    //m_iConnectionPointsCount(0),
    //m_iConnectionLinesCount(0),
    //m_iGroupsCount(0),
    //m_iElectricityResistorsCount(0),
    //m_iElectricityCapacitorsCount(0),
    //m_iElectricityInductorsCount(0),
    //m_iElectricityTransistorsCount(0),
    //m_iElectricitySwitchesCount(0),
    //m_iWdgtCheckBoxesCount(0),
    //m_iWdgtComboBoxesCount(0),
    //m_iWdgtGroupBoxesCount(0),
    //m_iWdgtLabelsCount(0),
    //m_iWdgtLineEditsCount(0),
    //m_iWdgtPushButtonsCount(0)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    int idxGroup = 0;

    // Mouse Events
    //-------------

    createTestGroupDrawStandardShapes(idxGroup);

    // Recall test step settings
    //--------------------------

    recallTestSteps();

} // ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    SErrResultInfo errResultInfo = saveTestSteps();
    if (errResultInfo.isErrorResult()) {
        if(CErrLog::GetInstance() != nullptr) {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    m_pMainWindow = nullptr;
    m_pDrawingView = nullptr;
    m_pDrawingScene = nullptr;
    //m_ptDrawingSceneMousePos;
    //m_ptDrawingViewMousePos;
    //m_pGraphObjCreating = nullptr;
    //m_pGraphObjSelected = nullptr;
    //m_iTestStepLev1 = 0;
    //m_iTestStepLev2 = 0;
    //m_iTestStepLev3 = 0;
    //m_iTestStepLev4 = 0;
    //m_fAngle_degree = 0.0;
    //m_fxOffsHor = 0.0;
    //m_fyOffsHor = 0.0;
    //m_iPointsCount = 0;
    //m_iLinesCount = 0;
    //m_iRectanglesCount = 0;
    //m_iEllipsesCount = 0;
    //m_iPolylinesCount = 0;
    //m_iPolygonsCount = 0;
    //m_iTextsCount = 0;
    //m_iImagesCount = 0;
    //m_iConnectionPointsCount = 0;
    //m_iConnectionLinesCount = 0;
    //m_iGroupsCount = 0;
    //m_iElectricityResistorsCount = 0;
    //m_iElectricityCapacitorsCount = 0;
    //m_iElectricityInductorsCount = 0;
    //m_iElectricityTransistorsCount = 0;
    //m_iElectricitySwitchesCount = 0;
    //m_iWdgtCheckBoxesCount = 0;
    //m_iWdgtComboBoxesCount = 0;
    //m_iWdgtGroupBoxesCount = 0;
    //m_iWdgtLabelsCount = 0;
    //m_iWdgtLineEditsCount = 0;
    //m_iWdgtPushButtonsCount = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::setMainWindow( CMainWindow* i_pMainWindow )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pMainWindow == nullptr ? "null" : i_pMainWindow->objectName());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMainWindow",
        /* strAddInfo   */ strMthInArgs );

    m_pMainWindow = i_pMainWindow;
    m_pDrawingView = CWidgetCentral::GetInstance()->drawingView();
    m_pDrawingScene = CWidgetCentral::GetInstance()->drawingScene();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapes( int& io_idxGroup )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "IdxGroup:" + QString::number(io_idxGroup);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapes",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpDrawStandardShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Draw Standard Shapes",
        /* pTSGrpParent */ nullptr );

    createTestGroupDrawStandardShapesLines(pGrpDrawStandardShapes, io_idxGroup);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
    }
} // createTestGroupDrawStandardShapes

//------------------------------------------------------------------------------
void CTest::createTestGroupDrawStandardShapesLines(ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs =
            "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
            ", IdxGroup:" + QString::number(io_idxGroup);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawStandardShapesLines",
        /* strAddInfo   */ strMthInArgs );

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = nullptr;

    ZS::Test::CTestStepGroup* pGrpDrawLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Lines",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStepGroup* pGrpDrawLinesLoadSave = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Load/Save",
        /* pTSGrpParent */ pGrpDrawLines );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetGeometry",
        /* strOperation    */ "MainWindow.setGeometry",
        /* pGrpParent      */ pGrpDrawLinesLoadSave,
        /* szDoTestStepFct */ SLOT(doTestStepMainWindowSetGeometry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Width", 1600);
    pTestStep->setConfigValue("Height", 800);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Clear",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpDrawLinesLoadSave,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LoadFile",
        /* strOperation    */ "DrawingScene.load",
        /* pGrpParent      */ pGrpDrawLinesLoadSave,
        /* szDoTestStepFct */ SLOT(doTestStepLoadFile(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FileName", "600px400pxOneLine.xml");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SaveFile",
        /* strOperation    */ "DrawingScene.save",
        /* pGrpParent      */ pGrpDrawLinesLoadSave,
        /* szDoTestStepFct */ SLOT(doTestStepSaveFile(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FileName", "600px400pxOneLine.xml");

    ZS::Test::CTestStepGroup* pGrpDrawLinesMouseEvents = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " MouseEvents",
        /* pTSGrpParent */ pGrpDrawLines );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetGeometry",
        /* strOperation    */ "MainWindow.setGeometry",
        /* pGrpParent      */ pGrpDrawLinesMouseEvents,
        /* szDoTestStepFct */ SLOT(doTestStepMainWindowSetGeometry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Width", 1600);
    pTestStep->setConfigValue("Height", 800);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Clear",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpDrawLinesMouseEvents,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetDrawingSize",
        /* strOperation    */ "DrawingScene.setDrawingSize",
        /* pGrpParent      */ pGrpDrawLinesMouseEvents,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawingSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrDimensionUnit, CEnumDrawingDimensionUnit(EDrawingDimensionUnit::Pixels).toString());
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrWidth, 600);
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrHeight, 400);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetGridSettings",
        /* strOperation    */ "DrawingScene.setGridSettings",
        /* pGrpParent      */ pGrpDrawLinesMouseEvents,
        /* szDoTestStepFct */ SLOT(doTestStepSetGridSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrGridLinesVisible, true);
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrGridLabelsVisible, true);

    QString strGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strTypeName = graphObjType2Str(EGraphObjTypeLine);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetCurrentDrawingTool(" + strGroupName + ", " + strTypeName + ")",
        /* strOperation    */ "DrawingScene.setCurrentDrawingTool(" + strGroupName + ", " + strTypeName + ")",
        /* pGrpParent      */ pGrpDrawLinesMouseEvents,
        /* szDoTestStepFct */ SLOT(doTestStepSetCurrentDrawingTool(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", strGroupName);
    pTestStep->setConfigValue("GraphObjType", strTypeName);

    QPoint ptMousePressPos(100, 100);
    //QPoint ptMouseReleasePos(200, 200);
    QPoint ptMouseReleasePos(110, 110);
    QSize sizeMouseMoveDist(10, 10);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MousePressEvent(" + qPoint2Str(ptMousePressPos) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" + qPoint2Str(ptMousePressPos) + ")",
        /* pGrpParent      */ pGrpDrawLinesMouseEvents,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePressPos);

    QPoint ptMouseMovePos = ptMousePressPos;
    while (ptMouseMovePos.x() < ptMouseReleasePos.x()) {
        ptMouseMovePos.setX(ptMouseMovePos.x() + sizeMouseMoveDist.width());
        ptMouseMovePos.setY(ptMouseMovePos.y() + sizeMouseMoveDist.height());
        pTestStep = new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* pGrpParent      */ pGrpDrawLinesMouseEvents,
            /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
        pTestStep->setConfigValue("MousePos", ptMouseMovePos);
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMouseReleasePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" + qPoint2Str(ptMouseReleasePos) + ")",
        /* pGrpParent      */ pGrpDrawLinesMouseEvents,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMouseReleasePos);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " UnsetCurrentDrawingTool",
        /* strOperation    */ "DrawingScene.setCurrentDrawingTool(null)",
        /* pGrpParent      */ pGrpDrawLinesMouseEvents,
        /* szDoTestStepFct */ SLOT(doTestStepUnsetCurrentDrawingTool(ZS::Test::CTestStep*)) );

    ptMousePressPos = ptMouseReleasePos;
    ptMouseReleasePos = QPoint(ptMousePressPos.x() + 10, ptMousePressPos.y() + 10);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MousePressEvent(" + qPoint2Str(ptMousePressPos) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" + qPoint2Str(ptMousePressPos) + ")",
        /* pGrpParent      */ pGrpDrawLinesMouseEvents,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePressPos);

    ptMouseMovePos = ptMousePressPos;
    while (ptMouseMovePos.x() < ptMouseReleasePos.x()) {
        ptMouseMovePos.setX(ptMouseMovePos.x() + sizeMouseMoveDist.width());
        ptMouseMovePos.setY(ptMouseMovePos.y() + sizeMouseMoveDist.height());
        pTestStep = new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* pGrpParent      */ pGrpDrawLinesMouseEvents,
            /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
        pTestStep->setConfigValue("MousePos", ptMouseMovePos);
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMouseReleasePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" + qPoint2Str(ptMouseReleasePos) + ")",
        /* pGrpParent      */ pGrpDrawLinesMouseEvents,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMouseReleasePos);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SaveFile",
        /* strOperation    */ "DrawingScene.save",
        /* pGrpParent      */ pGrpDrawLinesMouseEvents,
        /* szDoTestStepFct */ SLOT(doTestStepSaveFile(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FileName", "New600px400px.xml");

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
    }
} // createTestGroupDrawStandardShapesLines

//------------------------------------------------------------------------------
void CTest::doTestStepMainWindowSetGeometry( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepMainWindowSetGeometry",
        /* strAddInfo   */ strMthInArgs );

    int cxWidth = i_pTestStep->getConfigValue("Width").toInt();
    int cyHeight = i_pTestStep->getConfigValue("Height").toInt();

    QStringList strlstExpectedValues;
    strlstExpectedValues.append(qSize2Str(QSize(cxWidth, cyHeight)));
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    m_pMainWindow->setFixedSize(cxWidth, cyHeight);

    QStringList strlstResultValues;
    strlstResultValues.append(qSize2Str(m_pMainWindow->size()));
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepClearDrawingScene( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepClearDrawingScene",
        /* strAddInfo   */ strMthInArgs );

    i_pTestStep->setExpectedValue("");

    m_pDrawingScene->clear();

    i_pTestStep->setResultValue("");
}

//------------------------------------------------------------------------------
void CTest::doTestStepSetDrawingSize( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepSetDrawingSize",
        /* strAddInfo   */ strMthInArgs );

    CDrawingSize drawingSize("Test");
    drawingSize.setDimensionUnit(
        i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrDimensionUnit).toString());
    if (drawingSize.dimensionUnit() == EDrawingDimensionUnit::Pixels) {
        int cxWidth = i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrWidth).toInt();
        int cyHeight = i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrHeight).toInt();
        drawingSize.setImageSize(CPhysVal(cxWidth, Units.Length.pxX), CPhysVal(cyHeight, Units.Length.pxY));
    }
    else {
        CUnit unit = i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrUnit).toString();
        double fxWidth = i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrWidth).toDouble();
        double fyHeight = i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrHeight).toDouble();
        drawingSize.setImageSize(CPhysVal(fxWidth, unit), CPhysVal(fyHeight, unit));
    }

    i_pTestStep->setExpectedValue("");

    m_pDrawingScene->setDrawingSize(drawingSize);

    i_pTestStep->setResultValue("");
}

//------------------------------------------------------------------------------
void CTest::doTestStepSetGridSettings( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepSetGridSettings",
        /* strAddInfo   */ strMthInArgs );

    i_pTestStep->setExpectedValue("");

    CDrawGridSettings gridSettings("Test");
    gridSettings.setLinesVisible(
        i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrGridLinesVisible).toBool());
    gridSettings.setLabelsVisible(
        i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrGridLabelsVisible).toBool());

    m_pDrawingScene->setGridSettings(gridSettings);

    i_pTestStep->setResultValue("");
}

//------------------------------------------------------------------------------
void CTest::doTestStepLoadFile( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepLoadFile",
        /* strAddInfo   */ strMthInArgs );

    QString strFileName = i_pTestStep->getConfigValue("FileName").toString();
    QString strAbsFilePath = c_strTestInputFilesAbsDirPath + QDir::separator() + strFileName;

    SErrResultInfo errResultInfo;
    i_pTestStep->setExpectedValue(errResultInfo.toString());

    errResultInfo = m_pDrawingScene->load(strAbsFilePath);
    if (errResultInfo.isErrorResult()) {
        CErrLog::GetInstance()->addEntry(errResultInfo);
    }
    i_pTestStep->setResultValue(errResultInfo.toString());
}

//------------------------------------------------------------------------------
void CTest::doTestStepSaveFile( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepSaveFile",
        /* strAddInfo   */ strMthInArgs );

    QString strFileName = i_pTestStep->getConfigValue("FileName").toString();
    QString strTestFilesSaveAbsDirPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QString strAbsFilePath =
        strTestFilesSaveAbsDirPath +
        QDir::separator() + "Test" +
        QDir::separator() + strFileName;
    QFileInfo fileInfo(strAbsFilePath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }

    SErrResultInfo errResultInfo;
    i_pTestStep->setExpectedValue(errResultInfo.toString());

    errResultInfo = m_pDrawingScene->save(strAbsFilePath);
    if (errResultInfo.isErrorResult()) {
        CErrLog::GetInstance()->addEntry(errResultInfo);
    }
    i_pTestStep->setResultValue(errResultInfo.toString());
}

//------------------------------------------------------------------------------
void CTest::doTestStepSetCurrentDrawingTool( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepSetCurrentDrawingTool",
        /* strAddInfo   */ strMthInArgs );

    QString strGroupName = i_pTestStep->getConfigValue("GroupName").toString();
    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();

    m_pDrawingScene->setCurrentDrawingTool(strGroupName, strGraphObjType);

    QStringList strlstExpectedValues;
    strlstExpectedValues.append(strGroupName + "::" + strGraphObjType);
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    QStringList strlstResultValues;

    CObjFactory* pObjFactory = m_pDrawingScene->getCurrentDrawingTool();
    if (pObjFactory != nullptr) {
        strGroupName = pObjFactory->getGroupName();
        strGraphObjType = pObjFactory->getGraphObjTypeAsString();
        strlstResultValues.append(strGroupName + "::" + strGraphObjType);
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepUnsetCurrentDrawingTool( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepUnsetCurrentDrawingTool",
        /* strAddInfo   */ strMthInArgs );

    i_pTestStep->setExpectedValue("");

    m_pDrawingScene->setCurrentDrawingTool(nullptr);

    i_pTestStep->setResultValue("");
}

//------------------------------------------------------------------------------
void CTest::doTestStepMousePressEvent( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepMousePressEvent",
        /* strAddInfo   */ strMthInArgs );

    QPoint ptMousePos = i_pTestStep->getConfigValue("MousePos").toPoint();
    QPoint ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptMousePos);
    QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptDrawingViewMousePos);
    ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
    ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

    QMouseEvent* pMouseEv = new QMouseEvent(
        /* type      */ QEvent::MouseButtonPress,
        /* pos       */ ptDrawingViewMousePos,
        /* globalPos */ ptMousePosGlobal,
        /* button    */ Qt::LeftButton,
        /* buttons   */ Qt::NoButton,
        /* modifiers */ Qt::NoModifier );
    m_pDrawingView->mousePressEvent(pMouseEv);
    delete pMouseEv;
    pMouseEv = nullptr;

    QStringList strlstExpectedValues;
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    QStringList strlstResultValues;
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepMouseMoveEvent( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepMouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    QPoint ptMousePos = i_pTestStep->getConfigValue("MousePos").toPoint();
    QPoint ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptMousePos);
    QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptDrawingViewMousePos);
    ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
    ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

    QMouseEvent* pMouseEv = new QMouseEvent(
        /* type      */ QEvent::MouseMove,
        /* pos       */ ptDrawingViewMousePos,
        /* globalPos */ ptMousePosGlobal,
        /* button    */ Qt::NoButton,
        /* buttons   */ Qt::LeftButton,
        /* modifiers */ Qt::NoModifier );
    m_pDrawingView->mouseMoveEvent(pMouseEv);
    delete pMouseEv;
    pMouseEv = nullptr;

    QStringList strlstExpectedValues;
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    QStringList strlstResultValues;
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepMouseReleaseEvent( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepMouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    QPoint ptMousePos = i_pTestStep->getConfigValue("MousePos").toPoint();
    QPoint ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptMousePos);
    QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptDrawingViewMousePos);
    ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
    ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

    QMouseEvent* pMouseEv = new QMouseEvent(
        /* type      */ QEvent::MouseButtonRelease,
        /* pos       */ ptDrawingViewMousePos,
        /* globalPos */ ptMousePosGlobal,
        /* button    */ Qt::LeftButton,
        /* buttons   */ Qt::NoButton,
        /* modifiers */ Qt::NoModifier );
    m_pDrawingView->mouseReleaseEvent(pMouseEv);
    delete pMouseEv;
    pMouseEv = nullptr;

    QStringList strlstExpectedValues;
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    QStringList strlstResultValues;
    i_pTestStep->setResultValues(strlstResultValues);
}
