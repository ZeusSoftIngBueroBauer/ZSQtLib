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

    QString strFileName;

    QString strGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strTypeName = graphObjType2Str(EGraphObjTypeLine);

    QPoint ptGraphObjLineP1;
    QPoint ptGraphObjLineP2;
    QPoint ptGraphObjLineCenter;

    QPoint ptMouseMovePos;
    QSize sizeMouseMoveDist;

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = nullptr;

    ZS::Test::CTestStepGroup* pGrpDrawLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Lines",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Pixels Drawing
    //===============

    ZS::Test::CTestStepGroup* pGrpDrawLinesPixelsDrawing = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " PixelsDrawing",
        /* pTSGrpParent */ pGrpDrawLines );

    // Load/Save
    //----------

    ZS::Test::CTestStepGroup* pGrpDrawLinesPixelsDrawingLoadSave = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Load/Save",
        /* pTSGrpParent */ pGrpDrawLinesPixelsDrawing );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetGeometry",
        /* strOperation    */ "MainWindow.setGeometry",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingLoadSave,
        /* szDoTestStepFct */ SLOT(doTestStepMainWindowSetGeometry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Width", 1600);
    pTestStep->setConfigValue("Height", 800);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Clear",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingLoadSave,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LoadFile",
        /* strOperation    */ "DrawingScene.load",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingLoadSave,
        /* szDoTestStepFct */ SLOT(doTestStepLoadFile(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FileName", "600px400pxOneLine.xml");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SaveFile",
        /* strOperation    */ "DrawingScene.save",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingLoadSave,
        /* szDoTestStepFct */ SLOT(doTestStepSaveFile(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FileName", "600px400pxOneLine.xml");

    // Mouse Events
    //-------------

    ZS::Test::CTestStepGroup* pGrpDrawLinesPixelsDrawingMouseEvents = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " MouseEvents",
        /* pTSGrpParent */ pGrpDrawLinesPixelsDrawing );

    // Prepare drawing scene
    //----------------------

    ZS::Test::CTestStepGroup* pGrpDrawLinesPixelsDrawingMouseEventsPrepareScene = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " PrepareScene",
        /* pTSGrpParent */ pGrpDrawLinesPixelsDrawingMouseEvents );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetGeometry",
        /* strOperation    */ "MainWindow.setGeometry",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepMainWindowSetGeometry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Width", 1600);
    pTestStep->setConfigValue("Height", 800);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Clear",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );

    int cxDrawingWidth_px = 600;
    int cyDrawingHeight_px = 400;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetDrawingSize",
        /* strOperation    */ "DrawingScene.setDrawingSize",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawingSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrScreenResolutionPxPerMilliMeter, 1.0);
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrDimensionUnit, CEnumScaleDimensionUnit(EScaleDimensionUnit::Pixels).toString());
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrWidth, cxDrawingWidth_px);
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrHeight, cyDrawingHeight_px);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetGridSettings",
        /* strOperation    */ "DrawingScene.setGridSettings",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepSetGridSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrGridLinesVisible, true);
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrGridLabelsVisible, true);

    // Create object
    //--------------

    ZS::Test::CTestStepGroup* pGrpDrawLinesPixelsDrawingMouseEventsCreateObject = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " CreateObject",
        /* pTSGrpParent */ pGrpDrawLinesPixelsDrawingMouseEvents );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetCurrentDrawingTool(" + strGroupName + ", " + strTypeName + ")",
        /* strOperation    */ "DrawingScene.setCurrentDrawingTool(" + strGroupName + ", " + strTypeName + ")",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsCreateObject,
        /* szDoTestStepFct */ SLOT(doTestStepSetCurrentDrawingTool(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", strGroupName);
    pTestStep->setConfigValue("GraphObjType", strTypeName);

    ptGraphObjLineP1 = QPoint(100, 100);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MousePressEvent(" + qPoint2Str(ptGraphObjLineP1) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" + qPoint2Str(ptGraphObjLineP1) + ")",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsCreateObject,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptGraphObjLineP1);

    ptGraphObjLineP2 = QPoint(110, 110);
    //ptGraphObjLineP2 = QPoint(200, 200);
    ptMouseMovePos = ptGraphObjLineP1;
    sizeMouseMoveDist = QSize(10, 10);
    while (ptMouseMovePos.x() < ptGraphObjLineP2.x()) {
        ptMouseMovePos.setX(ptMouseMovePos.x() + sizeMouseMoveDist.width());
        ptMouseMovePos.setY(ptMouseMovePos.y() + sizeMouseMoveDist.height());
        pTestStep = new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsCreateObject,
            /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
        pTestStep->setConfigValue("MousePos", ptMouseMovePos);
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsCreateObject,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMouseMovePos);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " UnsetCurrentDrawingTool",
        /* strOperation    */ "DrawingScene.setCurrentDrawingTool(null)",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsCreateObject,
        /* szDoTestStepFct */ SLOT(doTestStepUnsetCurrentDrawingTool(ZS::Test::CTestStep*)) );

    // Resize object
    //--------------

    ZS::Test::CTestStepGroup* pGrpDrawLinesPixelsDrawingMouseEventsResizeObject = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " ResizeObject",
        /* pTSGrpParent */ pGrpDrawLinesPixelsDrawingMouseEvents );

    ptGraphObjLineP2 = ptMouseMovePos;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MousePressEvent(" + qPoint2Str(ptGraphObjLineP2) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" + qPoint2Str(ptGraphObjLineP2) + ")",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsResizeObject,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptGraphObjLineP2);

    ptGraphObjLineP2 = QPoint(ptGraphObjLineP2.x() + 10, ptGraphObjLineP2.y() + 10);
    //ptGraphObjLineP2 = QPoint(ptGraphObjLineP2.x() + 50, ptGraphObjLineP2.y() + 50);
    sizeMouseMoveDist = QSize(10, 10);
    while (ptMouseMovePos.x() < ptGraphObjLineP2.x()) {
        ptMouseMovePos.setX(ptMouseMovePos.x() + sizeMouseMoveDist.width());
        ptMouseMovePos.setY(ptMouseMovePos.y() + sizeMouseMoveDist.height());
        pTestStep = new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsResizeObject,
            /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
        pTestStep->setConfigValue("MousePos", ptMouseMovePos);
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsResizeObject,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMouseMovePos);

    // Move object
    //------------

    ZS::Test::CTestStepGroup* pGrpDrawLinesPixelsDrawingMouseEventsMoveObject = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " MoveObject",
        /* pTSGrpParent */ pGrpDrawLinesPixelsDrawingMouseEvents );

    // First move the cursor on the line so that the line is hit.
    ptGraphObjLineCenter = QLine(ptGraphObjLineP1, ptGraphObjLineP2).center();
    ptMouseMovePos = ptGraphObjLineCenter;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsMoveObject,
        /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
    pTestStep->setConfigValue("MousePos", ptMouseMovePos);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MousePressEvent(" + qPoint2Str(ptGraphObjLineCenter) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" + qPoint2Str(ptGraphObjLineCenter) + ")",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsMoveObject,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptGraphObjLineCenter);

    ptGraphObjLineCenter = QPoint(ptGraphObjLineCenter.x() + 10, ptGraphObjLineCenter.y() + 10);
    //ptGraphObjLineCenter = QPoint(ptGraphObjLineCenter.x() + 50, ptGraphObjLineCenter.y() + 50);
    sizeMouseMoveDist = QSize(10, 10);
    while (ptMouseMovePos.x() < ptGraphObjLineCenter.x()) {
        ptMouseMovePos.setX(ptMouseMovePos.x() + sizeMouseMoveDist.width());
        ptMouseMovePos.setY(ptMouseMovePos.y() + sizeMouseMoveDist.height());
        pTestStep = new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsMoveObject,
            /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
        pTestStep->setConfigValue("MousePos", ptMouseMovePos);
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsMoveObject,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMouseMovePos);

    // Save changes
    //-------------

    ZS::Test::CTestStepGroup* pGrpDrawLinesPixelsDrawingMouseEventsSaveScene = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " SaveScene",
        /* pTSGrpParent */ pGrpDrawLinesPixelsDrawingMouseEvents );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SaveFile",
        /* strOperation    */ "DrawingScene.save",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsSaveScene,
        /* szDoTestStepFct */ SLOT(doTestStepSaveFile(ZS::Test::CTestStep*)) );
    strFileName = "TestStandardShapesLinesPixelsDrawing-" + QString::number(cxDrawingWidth_px) + "px" + QString::number(cyDrawingHeight_px) + "px.xml";
    pTestStep->setConfigValue("FileName", strFileName);

    // Metrics Drawing
    //==================

    ZS::Test::CTestStepGroup* pGrpDrawLinesMetricsDrawing = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " MetricsDrawing",
        /* pTSGrpParent */ pGrpDrawLines );

    // Load/Save
    //----------

    ZS::Test::CTestStepGroup* pGrpDrawLinesMetricsDrawingLoadSave = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Load/Save",
        /* pTSGrpParent */ pGrpDrawLinesMetricsDrawing );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetGeometry",
        /* strOperation    */ "MainWindow.setGeometry",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingLoadSave,
        /* szDoTestStepFct */ SLOT(doTestStepMainWindowSetGeometry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Width", 1600);
    pTestStep->setConfigValue("Height", 800);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Clear",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingLoadSave,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LoadFile",
        /* strOperation    */ "DrawingScene.load",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingLoadSave,
        /* szDoTestStepFct */ SLOT(doTestStepLoadFile(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FileName", "600mm400mmOneLine.xml");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SaveFile",
        /* strOperation    */ "DrawingScene.save",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingLoadSave,
        /* szDoTestStepFct */ SLOT(doTestStepSaveFile(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FileName", "600mm400mmOneLine.xml");

    // Mouse Events
    //-------------

    ZS::Test::CTestStepGroup* pGrpDrawLinesMetricsDrawingMouseEvents = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " MouseEvents",
        /* pTSGrpParent */ pGrpDrawLinesMetricsDrawing );

    // Prepare drawing scene
    //----------------------

    ZS::Test::CTestStepGroup* pGrpDrawLinesMetricsDrawingMouseEventsPrepareScene = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " PrepareScene",
        /* pTSGrpParent */ pGrpDrawLinesMetricsDrawingMouseEvents );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetGeometry",
        /* strOperation    */ "MainWindow.setGeometry",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepMainWindowSetGeometry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Width", 1600);
    pTestStep->setConfigValue("Height", 800);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Clear",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );

    int cxDrawingWidth_mm = 600;
    int cyDrawingHeight_mm = 400;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetDrawingSize",
        /* strOperation    */ "DrawingScene.setDrawingSize",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawingSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrScreenResolutionPxPerMilliMeter, 1.0);
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrDimensionUnit, CEnumScaleDimensionUnit(EScaleDimensionUnit::Metric).toString());
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrUnit, "mm");
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrWidth, cxDrawingWidth_mm);
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrHeight, cyDrawingHeight_mm);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetGridSettings",
        /* strOperation    */ "DrawingScene.setGridSettings",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepSetGridSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrGridLinesVisible, true);
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrGridLabelsVisible, true);

    // Create object
    //--------------

    ZS::Test::CTestStepGroup* pGrpDrawLinesMetricsDrawingMouseEventsCreateObject = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " CreateObject",
        /* pTSGrpParent */ pGrpDrawLinesMetricsDrawingMouseEvents );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetCurrentDrawingTool(" + strGroupName + ", " + strTypeName + ")",
        /* strOperation    */ "DrawingScene.setCurrentDrawingTool(" + strGroupName + ", " + strTypeName + ")",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsCreateObject,
        /* szDoTestStepFct */ SLOT(doTestStepSetCurrentDrawingTool(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", strGroupName);
    pTestStep->setConfigValue("GraphObjType", strTypeName);

    ptGraphObjLineP1 = QPoint(100, 100);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MousePressEvent(" + qPoint2Str(ptGraphObjLineP1) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" + qPoint2Str(ptGraphObjLineP1) + ")",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsCreateObject,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptGraphObjLineP1);

    ptGraphObjLineP2 = QPoint(110, 110);
    //ptGraphObjLineP2 = QPoint(200, 200);
    ptMouseMovePos = ptGraphObjLineP1;
    sizeMouseMoveDist = QSize(10, 10);
    while (ptMouseMovePos.x() < ptGraphObjLineP2.x()) {
        ptMouseMovePos.setX(ptMouseMovePos.x() + sizeMouseMoveDist.width());
        ptMouseMovePos.setY(ptMouseMovePos.y() + sizeMouseMoveDist.height());
        pTestStep = new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsCreateObject,
            /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
        pTestStep->setConfigValue("MousePos", ptMouseMovePos);
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsCreateObject,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMouseMovePos);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " UnsetCurrentDrawingTool",
        /* strOperation    */ "DrawingScene.setCurrentDrawingTool(null)",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsCreateObject,
        /* szDoTestStepFct */ SLOT(doTestStepUnsetCurrentDrawingTool(ZS::Test::CTestStep*)) );

    // Resize object
    //--------------

    ZS::Test::CTestStepGroup* pGrpDrawLinesMetricsDrawingMouseEventsResizeObject = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " ResizeObject",
        /* pTSGrpParent */ pGrpDrawLinesMetricsDrawingMouseEvents );

    ptGraphObjLineP2 = ptMouseMovePos;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MousePressEvent(" + qPoint2Str(ptGraphObjLineP2) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" + qPoint2Str(ptGraphObjLineP2) + ")",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsResizeObject,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptGraphObjLineP2);

    ptGraphObjLineP2 = QPoint(ptGraphObjLineP2.x() + 10, ptGraphObjLineP2.y() + 10);
    //ptGraphObjLineP2 = QPoint(ptGraphObjLineP2.x() + 50, ptGraphObjLineP2.y() + 50);
    sizeMouseMoveDist = QSize(10, 10);
    while (ptMouseMovePos.x() < ptGraphObjLineP2.x()) {
        ptMouseMovePos.setX(ptMouseMovePos.x() + sizeMouseMoveDist.width());
        ptMouseMovePos.setY(ptMouseMovePos.y() + sizeMouseMoveDist.height());
        pTestStep = new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsResizeObject,
            /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
        pTestStep->setConfigValue("MousePos", ptMouseMovePos);
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsResizeObject,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMouseMovePos);

    // Move object
    //------------

    ZS::Test::CTestStepGroup* pGrpDrawLinesMetricsDrawingMouseEventsMoveObject = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " MoveObject",
        /* pTSGrpParent */ pGrpDrawLinesMetricsDrawingMouseEvents );

    // First move the cursor on the line so that the line is hit.
    ptGraphObjLineCenter = QLine(ptGraphObjLineP1, ptGraphObjLineP2).center();
    ptMouseMovePos = ptGraphObjLineCenter;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* pGrpParent      */ pGrpDrawLinesPixelsDrawingMouseEventsMoveObject,
        /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
    pTestStep->setConfigValue("MousePos", ptMouseMovePos);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MousePressEvent(" + qPoint2Str(ptGraphObjLineCenter) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" + qPoint2Str(ptGraphObjLineCenter) + ")",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsMoveObject,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptGraphObjLineCenter);

    ptGraphObjLineCenter = QPoint(ptGraphObjLineCenter.x() + 10, ptGraphObjLineCenter.y() + 10);
    //ptGraphObjLineCenter = QPoint(ptGraphObjLineCenter.x() + 50, ptGraphObjLineCenter.y() + 50);
    sizeMouseMoveDist = QSize(10, 10);
    while (ptMouseMovePos.x() < ptGraphObjLineCenter.x()) {
        ptMouseMovePos.setX(ptMouseMovePos.x() + sizeMouseMoveDist.width());
        ptMouseMovePos.setY(ptMouseMovePos.y() + sizeMouseMoveDist.height());
        pTestStep = new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsMoveObject,
            /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
        pTestStep->setConfigValue("MousePos", ptMouseMovePos);
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsMoveObject,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMouseMovePos);

    // Save changes
    //-------------

    ZS::Test::CTestStepGroup* pGrpDrawLinesMetricsDrawingMouseEventsSaveScene = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " SaveScene",
        /* pTSGrpParent */ pGrpDrawLinesMetricsDrawingMouseEvents );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SaveFile",
        /* strOperation    */ "DrawingScene.save",
        /* pGrpParent      */ pGrpDrawLinesMetricsDrawingMouseEventsSaveScene,
        /* szDoTestStepFct */ SLOT(doTestStepSaveFile(ZS::Test::CTestStep*)) );
    strFileName = "TestStandardShapesLinesMetricDrawing-" + QString::number(cxDrawingWidth_mm) + "mm" + QString::number(cyDrawingHeight_mm) + "mm.xml";
    pTestStep->setConfigValue("FileName", strFileName);

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

    QRect rectMainWindow = m_pMainWindow->geometry();
    rectMainWindow.setWidth(cxWidth);
    rectMainWindow.setHeight(cyHeight);
    m_pMainWindow->setGeometry(rectMainWindow);

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

#pragma message(__TODO__"Test with different scale factors")
#pragma message(__TODO__"Test with different Y Scale Axis orientations")

    if (i_pTestStep->hasConfigValue(CDrawingScene::c_strXmlAttrScreenResolutionPxPerMilliMeter)) {
        double fResPxPerMM = i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrScreenResolutionPxPerMilliMeter).toDouble();
        Units.Length.setScreenResolutionInPxPerMM(fResPxPerMM);
    }
    CDrawingSize drawingSize("Test");
    drawingSize.setDimensionUnit(
        i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrDimensionUnit).toString());
    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        int cxWidth = i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrWidth).toInt();
        int cyHeight = i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrHeight).toInt();
        drawingSize.setImageSize(CPhysVal(cxWidth, Units.Length.px), CPhysVal(cyHeight, Units.Length.px));
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
        /* button    */ Qt::LeftButton,
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

    Qt::MouseButton mouseBtn = Qt::LeftButton;
    if (i_pTestStep->hasConfigValue("MouseButtons")) {
        mouseBtn = static_cast<Qt::MouseButton>(i_pTestStep->getConfigValue("MouseButtons").toInt());
    }
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
        /* buttons   */ mouseBtn,
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
