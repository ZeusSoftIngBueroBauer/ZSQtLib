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
    createTestGroupPixelsDrawing(nullptr, idxGroup);
    createTestGroupMetricsDrawing(nullptr, idxGroup);

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
ZS::Test::CTestStepGroup* CTest::createTestGroupPrepareScene(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
                       ", DrawingSize {" + i_drawingSize.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupPrepareScene",
        /* strAddInfo   */ strMthInArgs );

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = nullptr;

    ZS::Test::CTestStepGroup* pGrpPrepareScene = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Prepare Scene",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetGeometry",
        /* strOperation    */ "MainWindow.setGeometry",
        /* pGrpParent      */ pGrpPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepMainWindowSetGeometry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Width", 1600);
    pTestStep->setConfigValue("Height", 800);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetDrawingSize",
        /* strOperation    */ "DrawingScene.setDrawingSize",
        /* pGrpParent      */ pGrpPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepSetDrawingSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrScreenResolutionPxPerMilliMeter, i_drawingSize.screenResolutionInPxPerMM());
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrDimensionUnit, i_drawingSize.dimensionUnit().toString());
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        pTestStep->setConfigValue(
            CDrawingScene::c_strXmlAttrWidth, i_drawingSize.imageWidthInPixels());
        pTestStep->setConfigValue(
            CDrawingScene::c_strXmlAttrHeight, i_drawingSize.imageHeightInPixels());
    }
    else {
        pTestStep->setConfigValue(
            CDrawingScene::c_strXmlAttrUnit, i_drawingSize.metricUnit().symbol());
        pTestStep->setConfigValue(
            CDrawingScene::c_strXmlAttrWidth, i_drawingSize.metricImageWidth().getVal());
        pTestStep->setConfigValue(
            CDrawingScene::c_strXmlAttrHeight, i_drawingSize.metricImageHeight().getVal());
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetGridSettings",
        /* strOperation    */ "DrawingScene.setGridSettings",
        /* pGrpParent      */ pGrpPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepSetGridSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrGridLinesVisible, true);
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrGridLabelsVisible, true);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpPrepareScene->path());
    }
    return pGrpPrepareScene;

} // createTestGroupPrepareScene

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupSaveLoadFile(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    const ZS::Draw::CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
                       ", DrawingSize {" + i_drawingSize.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupSaveLoadFile",
        /* strAddInfo   */ strMthInArgs );

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = nullptr;

    ZS::Test::CTestStepGroup* pGrpSaveLoadFile = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Save/Load File",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    QString strAbsDirPath =
        QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator() + "Test";
    QString strFileName;
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strFileName = QString::number(i_drawingSize.imageWidthInPixels()) + "x" +
                      QString::number(i_drawingSize.imageHeightInPixels()) + "px.xml";
    }
    else {
        strFileName = QString::number(static_cast<int>(i_drawingSize.metricImageWidth().getVal())) + "x" +
                      QString::number(static_cast<int>(i_drawingSize.metricImageWidth().getVal())) +
                      i_drawingSize.metricUnit().symbol() + ".xml";
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SaveFile",
        /* strOperation    */ "DrawingScene.save",
        /* pGrpParent      */ pGrpSaveLoadFile,
        /* szDoTestStepFct */ SLOT(doTestStepSaveFile(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("AbsDirPath", strAbsDirPath);
    pTestStep->setConfigValue("FileName", strFileName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Clear Drawing",
        /* strOperation    */ "DrawingScene.clear",
        /* pGrpParent      */ pGrpSaveLoadFile,
        /* szDoTestStepFct */ SLOT(doTestStepClearDrawingScene(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LoadFile",
        /* strOperation    */ "DrawingScene.load",
        /* pGrpParent      */ pGrpSaveLoadFile,
        /* szDoTestStepFct */ SLOT(doTestStepLoadFile(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("AbsDirPath", strAbsDirPath);
    pTestStep->setConfigValue("FileName", strFileName);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpSaveLoadFile->path());
    }
    return pGrpSaveLoadFile;

} // createTestGroupLoadSaveFile

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupPixelsDrawing(
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
        /* strMethod    */ "createTestGroupPixelsDrawing",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Pixels Drawing",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    CDrawingSize drawingSize("Test-PixelsDrawing");
    drawingSize.setDimensionUnit(EScaleDimensionUnit::Pixels);
    drawingSize.setScreenResolutionInPxPerMM(1.0);
    drawingSize.setImageSize(CPhysVal(800, Units.Length.px), CPhysVal(600, Units.Length.px));
    createTestGroupPrepareScene(pGrpPixelsDrawing, io_idxGroup, drawingSize);
    createTestGroupDrawStandardShapes(pGrpPixelsDrawing, io_idxGroup);
    createTestGroupSaveLoadFile(pGrpPixelsDrawing, io_idxGroup, drawingSize);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpPixelsDrawing->path());
    }
    return pGrpPixelsDrawing;

} // createTestGroupPixelsDrawing

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupMetricsDrawing(
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
        /* strMethod    */ "createTestGroupMetricsDrawing",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpMetricsDrawing = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Metrics Drawing",
        /* pTSGrpParent */ nullptr );

    CDrawingSize drawingSize("Test-MetricsDrawing");
    drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
    drawingSize.setScreenResolutionInPxPerMM(1.0);
    drawingSize.setImageSize(CPhysVal(800, Units.Length.mm), CPhysVal(600, Units.Length.mm));
    createTestGroupPrepareScene(pGrpMetricsDrawing, io_idxGroup, drawingSize);
    createTestGroupDrawStandardShapes(pGrpMetricsDrawing, io_idxGroup);
    createTestGroupSaveLoadFile(pGrpMetricsDrawing, io_idxGroup, drawingSize);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpMetricsDrawing->path());
    }
    return pGrpMetricsDrawing;

} // createTestGroupMetricsDrawing

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupDrawStandardShapes(
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
        /* strMethod    */ "createTestGroupDrawStandardShapes",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpDrawStandardShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Draw Standard Shapes",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    createTestGroupDrawStandardShapesLines(pGrpDrawStandardShapes, io_idxGroup);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpDrawStandardShapes->path());
    }
    return pGrpDrawStandardShapes;

} // createTestGroupDrawStandardShapes

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupDrawStandardShapesLines(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup)
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
        /* strMethod    */ "createTestGroupDrawStandardShapesLines",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpDrawLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Lines",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    int iGraphObjCount = 0;
    QString strGraphObjName = graphObjType2Str(EGraphObjTypeLine) + QString::number(iGraphObjCount);

    ZS::Test::CTestStepGroup* pGrpDrawLine0 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLines );

    QStringList strlstLabelNames = { CGraphObj::c_strLabelName, "P1", "P2"};
    QStringList strlstGeometryLabelNames;
    QList<QPoint> arptMousePress;
    QList<QPoint> arptMouseRelease;

    createTestGroupDrawMouseEventsCreateObject(
        pGrpDrawLine0, io_idxGroup, EGraphObjTypeLine,
        QPoint(100, 100), QPoint(150, 150));
    createTestGroupDrawMouseEventsMovePressMoveRelease(
        pGrpDrawLine0, io_idxGroup, "Resize " + strGraphObjName,
        true, QPoint(150, 150), QPoint(200, 200));
    createTestGroupDrawMouseEventsMovePressMoveRelease(
        pGrpDrawLine0, io_idxGroup, "Move " + strGraphObjName,
        false, QPoint(150, 150), QPoint(200, 200));
    createTestGroupShowLabels(
        pGrpDrawLine0, io_idxGroup, EGraphObjTypeLine, strGraphObjName, strlstLabelNames);
    arptMousePress.clear();
    arptMousePress.append(QPoint(200, 200));
    arptMousePress.append(QPoint(150, 150));
    arptMousePress.append(QPoint(250, 250));
    arptMouseRelease.clear();
    arptMouseRelease.append(QPoint(210, 190));
    arptMouseRelease.append(QPoint(160, 140));
    arptMouseRelease.append(QPoint(260, 240));
    createTestGroupMoveLabels(
        pGrpDrawLine0, io_idxGroup, EGraphObjTypeLine, strGraphObjName,
        strlstLabelNames, arptMousePress, arptMouseRelease);
    strlstGeometryLabelNames.clear();
    strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameP1);
    strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameP2);
    strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameCenter);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameWidth);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameHeight);
    strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameLength);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameAngle);
    createTestGroupShowGeometryLabels(
        pGrpDrawLine0, io_idxGroup, EGraphObjTypeLine, strGraphObjName, strlstGeometryLabelNames);
    strlstGeometryLabelNames.clear();
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameAngle);
    strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameLength);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameHeight);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameWidth);
    arptMousePress.clear();
    //arptMousePress.append(QPoint(200, 200));    // Angle
    arptMousePress.append(QPoint(200, 200));    // Length
    //arptMousePress.append(QPoint(200, 200));    // Height
    //arptMousePress.append(QPoint(200, 200));    // Width
    arptMouseRelease.clear();
    //arptMouseRelease.append(QPoint(240, 180));  // Angle
    arptMouseRelease.append(QPoint(210, 170));  // Length
    //arptMouseRelease.append(QPoint(160, 210));  // Height
    //arptMouseRelease.append(QPoint(190, 220));  // Width
    createTestGroupMoveLabels(
        pGrpDrawLine0, io_idxGroup, EGraphObjTypeLine, strGraphObjName,
        strlstGeometryLabelNames, arptMousePress, arptMouseRelease);

    ++iGraphObjCount;
    strGraphObjName = graphObjType2Str(EGraphObjTypeLine) + QString::number(iGraphObjCount);

    ZS::Test::CTestStepGroup* pGrpDrawLine1 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLines );

    createTestGroupDrawMouseEventsCreateObject(
        pGrpDrawLine1, io_idxGroup, EGraphObjTypeLine, QPoint(420, 100), QPoint(370, 150));
    createTestGroupDrawMouseEventsMovePressMoveRelease(
        pGrpDrawLine1, io_idxGroup, "Resize " + strGraphObjName,
        true, QPoint(370, 150), QPoint(320, 200));
    createTestGroupDrawMouseEventsMovePressMoveRelease(
        pGrpDrawLine1, io_idxGroup, "Move " + strGraphObjName,
        false, QPoint(370, 150), QPoint(320, 200));
    createTestGroupShowLabels(
        pGrpDrawLine1, io_idxGroup, EGraphObjTypeLine, strGraphObjName, strlstLabelNames);
    arptMousePress.clear();
    arptMousePress.append(QPoint(320, 200));
    arptMousePress.append(QPoint(370, 150));
    arptMousePress.append(QPoint(270, 250));
    arptMouseRelease.clear();
    arptMouseRelease.append(QPoint(330, 210));
    arptMouseRelease.append(QPoint(380, 160));
    arptMouseRelease.append(QPoint(280, 260));
    createTestGroupMoveLabels(
        pGrpDrawLine1, io_idxGroup, EGraphObjTypeLine, strGraphObjName,
        strlstLabelNames, arptMousePress, arptMouseRelease);
    //strlstGeometryLabelNames.clear();
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameP1);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameP2);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameCenter);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameWidth);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameHeight);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameLength);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameAngle);
    //createTestGroupShowGeometryLabels(
    //    pGrpDrawLine1, io_idxGroup, EGraphObjTypeLine, strGraphObjName, strlstGeometryLabelNames);
    //strlstGeometryLabelNames.clear();
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameAngle);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameLength);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameHeight);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameWidth);
    //arptMousePress.clear();
    //arptMousePress.append(QPoint(320, 200));    // Angle
    //arptMousePress.append(QPoint(320, 200));    // Length
    //arptMousePress.append(QPoint(320, 200));    // Height
    //arptMousePress.append(QPoint(320, 200));    // Width
    //arptMouseRelease.clear();
    //arptMouseRelease.append(QPoint(360, 200));  // Angle
    //arptMouseRelease.append(QPoint(320, 240));  // Length
    //arptMouseRelease.append(QPoint(310, 180));  // Height
    //arptMouseRelease.append(QPoint(280, 210));  // Width
    //createTestGroupMoveLabels(
    //    pGrpDrawLine1, io_idxGroup, EGraphObjTypeLine, strGraphObjName,
    //    strlstGeometryLabelNames, arptMousePress, arptMouseRelease);

    ++iGraphObjCount;
    strGraphObjName = graphObjType2Str(EGraphObjTypeLine) + QString::number(iGraphObjCount);

    ZS::Test::CTestStepGroup* pGrpDrawLine2 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLines );

    createTestGroupDrawMouseEventsCreateObject(
        pGrpDrawLine2, io_idxGroup, EGraphObjTypeLine, QPoint(420, 420), QPoint(370, 370));
    createTestGroupDrawMouseEventsMovePressMoveRelease(
        pGrpDrawLine2, io_idxGroup, "Resize " + strGraphObjName,
        true, QPoint(370, 370), QPoint(320, 320));
    createTestGroupDrawMouseEventsMovePressMoveRelease(
        pGrpDrawLine2, io_idxGroup, "Move " + strGraphObjName,
        false, QPoint(370, 370), QPoint(320, 320));
    createTestGroupShowLabels(
        pGrpDrawLine2, io_idxGroup, EGraphObjTypeLine, strGraphObjName, strlstLabelNames);
    arptMousePress.clear();
    arptMousePress.append(QPoint(320, 320));
    arptMousePress.append(QPoint(370, 370));
    arptMousePress.append(QPoint(270, 270));
    arptMouseRelease.clear();
    arptMouseRelease.append(QPoint(310, 330));
    arptMouseRelease.append(QPoint(360, 390));
    arptMouseRelease.append(QPoint(260, 290));
    createTestGroupMoveLabels(
        pGrpDrawLine2, io_idxGroup, EGraphObjTypeLine, strGraphObjName,
        strlstLabelNames, arptMousePress, arptMouseRelease);
    //strlstGeometryLabelNames.clear();
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameP1);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameP2);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameCenter);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameWidth);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameHeight);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameLength);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameAngle);
    //createTestGroupShowGeometryLabels(
    //    pGrpDrawLine2, io_idxGroup, EGraphObjTypeLine, strGraphObjName, strlstGeometryLabelNames);
    //strlstGeometryLabelNames.clear();
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameAngle);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameLength);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameHeight);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameWidth);
    //arptMousePress.clear();
    //arptMousePress.append(QPoint(320, 320));    // Angle
    //arptMousePress.append(QPoint(320, 320));    // Length
    //arptMousePress.append(QPoint(320, 320));    // Height
    //arptMousePress.append(QPoint(320, 320));    // Width
    //arptMouseRelease.clear();
    //arptMouseRelease.append(QPoint(320, 350));  // Angle
    //arptMouseRelease.append(QPoint(280, 320));  // Length
    //arptMouseRelease.append(QPoint(350, 320));  // Height
    //arptMouseRelease.append(QPoint(320, 290));  // Width
    //createTestGroupMoveLabels(
    //    pGrpDrawLine2, io_idxGroup, EGraphObjTypeLine, strGraphObjName,
    //    strlstGeometryLabelNames, arptMousePress, arptMouseRelease);

    ++iGraphObjCount;
    strGraphObjName = graphObjType2Str(EGraphObjTypeLine) + QString::number(iGraphObjCount);

    ZS::Test::CTestStepGroup* pGrpDrawLine3 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLines );

    createTestGroupDrawMouseEventsCreateObject(
        pGrpDrawLine3, io_idxGroup, EGraphObjTypeLine, QPoint(100, 420), QPoint(150, 370));
    createTestGroupDrawMouseEventsMovePressMoveRelease(
        pGrpDrawLine3, io_idxGroup, "Resize " + strGraphObjName,
        true, QPoint(150, 370), QPoint(200, 320));
    createTestGroupDrawMouseEventsMovePressMoveRelease(
        pGrpDrawLine3, io_idxGroup, "Move " + strGraphObjName,
        false, QPoint(150, 370), QPoint(200, 320));
    createTestGroupShowLabels(
        pGrpDrawLine3, io_idxGroup, EGraphObjTypeLine, strGraphObjName, strlstLabelNames);
    arptMousePress.clear();
    arptMousePress.append(QPoint(200, 320));
    arptMousePress.append(QPoint(150, 370));
    arptMousePress.append(QPoint(250, 270));
    arptMouseRelease.clear();
    arptMouseRelease.append(QPoint(190, 310));
    arptMouseRelease.append(QPoint(140, 360));
    arptMouseRelease.append(QPoint(240, 260));
    createTestGroupMoveLabels(
        pGrpDrawLine3, io_idxGroup, EGraphObjTypeLine, strGraphObjName,
        strlstLabelNames, arptMousePress, arptMouseRelease);
    //strlstGeometryLabelNames.clear();
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameP1);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameP2);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameCenter);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameWidth);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameHeight);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameLength);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameAngle);
    //createTestGroupShowGeometryLabels(
    //    pGrpDrawLine3, io_idxGroup, EGraphObjTypeLine, strGraphObjName, strlstGeometryLabelNames);
    //strlstGeometryLabelNames.clear();
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameAngle);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameLength);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameHeight);
    //strlstGeometryLabelNames.append(CGraphObj::c_strGeometryLabelNameWidth);
    //arptMousePress.clear();
    //arptMousePress.append(QPoint(200, 320));    // Angle
    //arptMousePress.append(QPoint(200, 320));    // Length
    //arptMousePress.append(QPoint(200, 320));    // Height
    //arptMousePress.append(QPoint(200, 320));    // Width
    //arptMouseRelease.clear();
    //arptMouseRelease.append(QPoint(200, 290));  // Angle
    //arptMouseRelease.append(QPoint(160, 320));  // Length
    //arptMouseRelease.append(QPoint(210, 340));  // Height
    //arptMouseRelease.append(QPoint(230, 320));  // Width
    //createTestGroupMoveLabels(
    //    pGrpDrawLine3, io_idxGroup, EGraphObjTypeLine, strGraphObjName,
    //    strlstGeometryLabelNames, arptMousePress, arptMouseRelease);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpDrawLines->path());
    }
    return pGrpDrawLines;

} // createTestGroupDrawStandardShapesLines

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupDrawMouseEventsCreateObject(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    EGraphObjType i_graphObjType, const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
                       ", GraphObjType:" + graphObjType2Str(i_graphObjType) +
                       ", MousePress {" + qPoint2Str(i_ptMousePress) + "}" +
                       ", MouseRelease {" + qPoint2Str(i_ptMouseRelease) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawMouseEventsCreateObject",
        /* strAddInfo   */ strMthInArgs );

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = nullptr;

    QString strGroupName = "Standard Shapes";
    QString strGraphObjType = graphObjType2Str(i_graphObjType);

    ZS::Test::CTestStepGroup* pGrpDrawMouseEventsCreateObject = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Create " + strGraphObjType,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetCurrentDrawingTool(" + strGraphObjType + ")",
        /* strOperation    */ "DrawingScene.setCurrentDrawingTool(" + strGraphObjType + ")",
        /* pGrpParent      */ pGrpDrawMouseEventsCreateObject,
        /* szDoTestStepFct */ SLOT(doTestStepSetCurrentDrawingTool(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GroupName", strGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);

    createTestGroupDrawMouseEventsMovePressMoveRelease(
        pGrpDrawMouseEventsCreateObject, io_idxGroup, "Create " + strGraphObjType,
        true, i_ptMousePress, i_ptMouseRelease);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " UnsetCurrentDrawingTool",
        /* strOperation    */ "DrawingScene.setCurrentDrawingTool(null)",
        /* pGrpParent      */ pGrpDrawMouseEventsCreateObject,
        /* szDoTestStepFct */ SLOT(doTestStepUnsetCurrentDrawingTool(ZS::Test::CTestStep*)) );

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpDrawMouseEventsCreateObject->path());
    }
    return pGrpDrawMouseEventsCreateObject;

} // createTestGroupDrawMouseEventsCreateObject

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupShowLabels(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    ZS::Draw::EGraphObjType i_graphObjType, const QString& i_strObjName,
    const QStringList& i_strlstLabelNames)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
                       ", GraphObjType:" + graphObjType2Str(i_graphObjType) +
                       ", ObjName: " + i_strObjName +
                       ", Labels {" + i_strlstLabelNames.join(", ") + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupShowLabels",
        /* strAddInfo   */ strMthInArgs );

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = nullptr;

    ZS::Test::CTestStepGroup* pGrpShowLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Show Labels",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    for (const QString& strLabelName : i_strlstLabelNames) {
        pTestStep = new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " " + i_strObjName + ".showLabel(" + strLabelName + ")",
            /* strOperation    */ "GraphObj.showLabel(" + i_strObjName + ", " + strLabelName + ")",
            /* pGrpParent      */ pGrpShowLabels,
            /* szDoTestStepFct */ SLOT(doTestStepShowLabel(ZS::Test::CTestStep*)) );
        pTestStep->setConfigValue("GraphObjName", i_strObjName);
        pTestStep->setConfigValue("LabelName", strLabelName);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpShowLabels->path());
    }
    return pGrpShowLabels;

} // createTestGroupShowLabels

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupMoveLabels(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    ZS::Draw::EGraphObjType i_graphObjType, const QString& i_strObjName,
    const QStringList& i_strlstLabelNames,
    const QList<QPoint>& i_arptMousePress, const QList<QPoint>& i_arptMouseRelease)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
                       ", GraphObjType:" + graphObjType2Str(i_graphObjType) +
                       ", ObjName: " + i_strObjName +
                       ", Labels {" + i_strlstLabelNames.join(", ") + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupMoveLabels",
        /* strAddInfo   */ strMthInArgs );

    if (i_strlstLabelNames.size() != i_arptMousePress.size() || i_strlstLabelNames.size() != i_arptMouseRelease.size()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = nullptr;

    ZS::Test::CTestStepGroup* pGrpMoveLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Move Labels",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    for (int idxLabel = 0; idxLabel < i_strlstLabelNames.size(); idxLabel++) {
        const QString& strLabelName = i_strlstLabelNames[idxLabel];
        createTestGroupDrawMouseEventsMovePressMoveRelease(
            pGrpMoveLabels, io_idxGroup, "Move " + strLabelName,
            false, i_arptMousePress[idxLabel], i_arptMouseRelease[idxLabel]);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpMoveLabels->path());
    }
    return pGrpMoveLabels;

} // createTestGroupMoveLabels

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupShowGeometryLabels(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    ZS::Draw::EGraphObjType i_graphObjType, const QString& i_strObjName,
    const QStringList& i_strlstLabelNames)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
                       ", GraphObjType:" + graphObjType2Str(i_graphObjType) +
                       ", ObjName: " + i_strObjName +
                       ", Labels {" + i_strlstLabelNames.join(", ") + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupShowGeometryLabels",
        /* strAddInfo   */ strMthInArgs );

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = nullptr;

    ZS::Test::CTestStepGroup* pGrpShowLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Show Geometry Labels",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    for (const QString& strLabelName : i_strlstLabelNames) {
        pTestStep = new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " " + i_strObjName + ".showGeometryLabel(" + strLabelName + ")",
            /* strOperation    */ "GraphObj.showGeometryLabel(" + i_strObjName + ", " + strLabelName + ")",
            /* pGrpParent      */ pGrpShowLabels,
            /* szDoTestStepFct */ SLOT(doTestStepShowGeometryLabel(ZS::Test::CTestStep*)) );
        pTestStep->setConfigValue("GraphObjName", i_strObjName);
        pTestStep->setConfigValue("LabelName", strLabelName);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpShowLabels->path());
    }
    return pGrpShowLabels;

} // createTestGroupShowGeometryLabels

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupDrawMouseEventsMovePressMoveRelease(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    const QString& i_strGroupName, bool i_bSkipFirstMove,
    const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
                       ", GroupName: " + i_strGroupName +
                       ", SkipFirstMove: " + bool2Str(i_bSkipFirstMove) +
                       ", MousePress {" + qPoint2Str(i_ptMousePress) + "}" +
                       ", MouseRelease {" + qPoint2Str(i_ptMouseRelease) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawMouseEventsMovePressMoveRelease",
        /* strAddInfo   */ strMthInArgs );

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = nullptr;

    ZS::Test::CTestStepGroup* pGrpDrawMouseEventsMoveObject = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + i_strGroupName,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // First move the cursor on the object so that the object is hit.
    QPoint ptMouseMovePos = i_ptMousePress;
    //if (!i_bSkipFirstMove) {
        pTestStep = new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* pGrpParent      */ pGrpDrawMouseEventsMoveObject,
            /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
        pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
        pTestStep->setConfigValue("MousePos", ptMouseMovePos);
    //}

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MousePressEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* pGrpParent      */ pGrpDrawMouseEventsMoveObject,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMouseMovePos);

    bool bMoveRight = i_ptMouseRelease.x() > i_ptMousePress.x();
    bool bMoveLeft = !bMoveRight;
    bool bMoveDown = i_ptMouseRelease.y() > i_ptMousePress.y();
    bool bMoveUp = !bMoveDown;
    QSize sizeMouseMoveDist = QSize(
        (i_ptMouseRelease.x() - i_ptMousePress.x()) / 2,
        (i_ptMouseRelease.y() - i_ptMousePress.y()) / 2);
    int iMoves = 0;
    while ((ptMouseMovePos.x() != i_ptMouseRelease.x()) && iMoves < 10) {
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
            /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* pGrpParent      */ pGrpDrawMouseEventsMoveObject,
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
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(i_ptMouseRelease) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" + qPoint2Str(i_ptMouseRelease) + ")",
        /* pGrpParent      */ pGrpDrawMouseEventsMoveObject,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", i_ptMouseRelease);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpDrawMouseEventsMoveObject->path());
    }
    return pGrpDrawMouseEventsMoveObject;

} // createTestGroupDrawMouseEventsMovePressMoveRelease

/*==============================================================================
protected slots:
==============================================================================*/

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

    CGraphObjConnectionLine::s_iInstCount = 0;
    CGraphObjConnectionPoint::s_iInstCount = 0;
    CGraphObjEllipse::s_iInstCount = 0;
    CGraphObjGroup::s_iInstCount = 0;
    CGraphObjImage::s_iInstCount = 0;
    CGraphObjLine::s_iInstCount = 0;
    CGraphObjPoint::s_iInstCount = 0;
    CGraphObjPolygon::s_iInstCount = 0;
    CGraphObjPolyline::s_iInstCount = 0;
    CGraphObjRect::s_iInstCount = 0;
    CGraphObjText::s_iInstCount = 0;

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

    CDrawingSize drawingSize("Test");
    drawingSize.setDimensionUnit(
        i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrDimensionUnit).toString());
    if (i_pTestStep->hasConfigValue(CDrawingScene::c_strXmlAttrScreenResolutionPxPerMilliMeter)) {
        double fResPxPerMM = i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrScreenResolutionPxPerMilliMeter).toDouble();
        drawingSize.setScreenResolutionInPxPerMM(fResPxPerMM);
    }
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

    QString strAbsDirPath = i_pTestStep->getConfigValue("AbsDirPath").toString();
    QString strFileName = i_pTestStep->getConfigValue("FileName").toString();
    QString strAbsFilePath = strAbsDirPath + QDir::separator() + strFileName;

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

    QString strAbsDirPath = i_pTestStep->getConfigValue("AbsDirPath").toString();
    QString strFileName = i_pTestStep->getConfigValue("FileName").toString();
    QString strAbsFilePath = strAbsDirPath + QDir::separator() + strFileName;

    SErrResultInfo errResultInfo;
    i_pTestStep->setExpectedValue(errResultInfo.toString());

    errResultInfo = m_pDrawingScene->load(strAbsFilePath);
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
void CTest::doTestStepShowLabel( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepShowLabel",
        /* strAddInfo   */ strMthInArgs );

    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strLabelName = i_pTestStep->getConfigValue("LabelName").toString();

    QString strKeyInTree = "B:" + strGraphObjName;
    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strKeyInTree);
    if (pGraphObj != nullptr) {
        pGraphObj->showLabel(strLabelName);
        pGraphObj->showLabelAnchorLine(strLabelName);
    }

    QStringList strlstExpectedValues;
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    QStringList strlstResultValues;
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepShowGeometryLabel( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepShowGeometryLabel",
        /* strAddInfo   */ strMthInArgs );

    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strLabelName = i_pTestStep->getConfigValue("LabelName").toString();

    QString strKeyInTree = "B:" + strGraphObjName;
    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strKeyInTree);
    if (pGraphObj != nullptr) {
        pGraphObj->showGeometryLabel(strLabelName);
        pGraphObj->showGeometryLabelAnchorLine(strLabelName);
    }

    QStringList strlstExpectedValues;
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    QStringList strlstResultValues;
    i_pTestStep->setResultValues(strlstResultValues);
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
