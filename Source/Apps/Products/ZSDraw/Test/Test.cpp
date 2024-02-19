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
#include "ZSSys/ZSSysAux.h"
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

static QHash<QString, int> s_hshGraphObjsInstCounts = {
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypePoint), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypeLine), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypeRect), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypeEllipse), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypePolygon), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypePolyline), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypeText), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypeImage), 0},
    { CObjFactory::c_strGroupNameStandardShapes + "::" + graphObjType2Str(EGraphObjTypeGroup), 0},
    { CObjFactory::c_strGroupNameConnections + "::" + graphObjType2Str(EGraphObjTypeConnectionLine), 0},
    { CObjFactory::c_strGroupNameConnections + "::" + graphObjType2Str(EGraphObjTypeConnectionPoint), 0}
};

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

    // Start with reasonable drawing size
    CDrawingSize drawingSize("DrawingScene");
    drawingSize.setImageSize(CPhysVal(800.0, Units.Length.px, 1.0), CPhysVal(600.0, Units.Length.px, 1.0));
    m_pDrawingScene->setDrawingSize(drawingSize);

    int idxGroup = 0;

    createTestGroupImageSizeAndObjectCoordinates(nullptr, idxGroup);
    createTestGroupPixelsDrawing(nullptr, idxGroup);
    createTestGroupMetricsDrawing(nullptr, idxGroup);

    // Recall test step settings
    //--------------------------

    recallTestSteps();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupPrepareScene(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    const CDrawingSize& i_drawingSize, const CDrawGridSettings& i_gridSettings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
                       ", DrawingSize {" + i_drawingSize.toString() + "}",
                       ", GridSettings {" + i_gridSettings.toString() + "}";
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
    pTestStep->setConfigValue("Width", 1800);
    pTestStep->setConfigValue("Height", 840);

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
        CDrawingScene::c_strXmlAttrDimensionUnit, i_drawingSize.dimensionUnit().toString());
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrScreenResolutionPxPerMilliMeter, i_drawingSize.screenResolutionInPxPerMM());
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
            CDrawingScene::c_strXmlAttrMetricImageCoorsDecimals, i_drawingSize.metricImageCoorsDecimals());
        pTestStep->setConfigValue(
            CDrawingScene::c_strXmlAttrWidth, i_drawingSize.metricImageWidth().getVal());
        pTestStep->setConfigValue(
            CDrawingScene::c_strXmlAttrHeight, i_drawingSize.metricImageHeight().getVal());
        pTestStep->setConfigValue(
            CDrawingScene::c_strXmlAttrYScaleAxisOrientation, i_drawingSize.yScaleAxisOrientation().toString());
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetGridSettings",
        /* strOperation    */ "DrawingScene.setGridSettings",
        /* pGrpParent      */ pGrpPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepSetGridSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrGridLinesVisible, i_gridSettings.areLinesVisible());
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrGridLinesDistMin, i_gridSettings.linesDistMin());
    pTestStep->setConfigValue(
        CDrawingScene::c_strXmlAttrGridLabelsVisible, i_gridSettings.areLabelsVisible());

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
ZS::Test::CTestStepGroup* CTest::createTestGroupImageSizeAndObjectCoordinates(
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
        /* strMethod    */ "createTestGroupImageSizeAndObjectCoordinates",
        /* strAddInfo   */ strMthInArgs );

    QStringList strlstExpectedValues;
    ZS::Test::CTestStep* pTestStep = nullptr;
    int idxStep = 0;

    ZS::Test::CTestStepGroup* pGrpImageSizeAndObjectCoordinates = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Image Size and Object Coordinates",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Pixels Drawing",
        /* pTSGrpParent */ pGrpImageSizeAndObjectCoordinates );

    {
        ZS::Test::CTestStepGroup* pGrpPixelsDrawing100x100 = new ZS::Test::CTestStepGroup(
            /* pTest        */ this,
            /* strName      */ "Group " + QString::number(++io_idxGroup) + " Pixels Drawing (100 * 100) px",
            /* pTSGrpParent */ pGrpPixelsDrawing );

        CDrawingSize drawingSize("Test-PixelsDrawing");
        drawingSize.setDimensionUnit(EScaleDimensionUnit::Pixels);
        drawingSize.setImageSize(CPhysVal(100, Units.Length.px), CPhysVal(100, Units.Length.px));

        CDrawGridSettings gridSettings("Test-PixelsDrawing");
        gridSettings.setLinesVisible(true);
        gridSettings.setLinesDistMin(20);
        gridSettings.setLabelsVisible(true);

        createTestGroupPrepareScene(pGrpPixelsDrawing100x100, io_idxGroup, drawingSize, gridSettings);
        createTestGroupImageSizeAndObjectCoordinatesPixelsDrawingAddLines(pGrpPixelsDrawing100x100, io_idxGroup);
    }

    {
        ZS::Test::CTestStepGroup* pGrpPixelsDrawing101x101 = new ZS::Test::CTestStepGroup(
            /* pTest        */ this,
            /* strName      */ "Group " + QString::number(++io_idxGroup) + " Pixels Drawing (101 * 101) px",
            /* pTSGrpParent */ pGrpPixelsDrawing );

        CDrawingSize drawingSize("Test-PixelsDrawing");
        drawingSize.setDimensionUnit(EScaleDimensionUnit::Pixels);
        drawingSize.setImageSize(CPhysVal(101, Units.Length.px), CPhysVal(101, Units.Length.px));

        CDrawGridSettings gridSettings("Test-PixelsDrawing");
        gridSettings.setLinesVisible(true);
        gridSettings.setLinesDistMin(20);
        gridSettings.setLabelsVisible(true);

        createTestGroupPrepareScene(pGrpPixelsDrawing101x101, io_idxGroup, drawingSize, gridSettings);
        createTestGroupImageSizeAndObjectCoordinatesPixelsDrawingAddLines(pGrpPixelsDrawing101x101, io_idxGroup);
    }

    ZS::Test::CTestStepGroup* pGrpMetricsDrawing = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Metrics Drawing",
        /* pTSGrpParent */ pGrpImageSizeAndObjectCoordinates );
    {
        ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown = new ZS::Test::CTestStepGroup(
            /* pTest        */ this,
            /* strName      */ "Group " + QString::number(++io_idxGroup) + " YScaleTopDown (100 * 100) mm",
            /* pTSGrpParent */ pGrpMetricsDrawing );

        CDrawingSize drawingSize("Test-MetricsDrawing-YScaleTopDown");
        drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
        drawingSize.setMetricUnit(Units.Length.mm);
        drawingSize.setMetricImageCoorsDecimals(1);
        drawingSize.setScreenResolutionInPxPerMM(3.5);
        drawingSize.setImageSize(CPhysVal(100, Units.Length.mm), CPhysVal(100, Units.Length.mm));
        drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::TopDown);

        CDrawGridSettings gridSettings("Test-MetricsDrawing-YScaleTopDown");
        gridSettings.setLinesVisible(true);
        gridSettings.setLinesDistMin(20);
        gridSettings.setLabelsVisible(true);

        createTestGroupPrepareScene(pGrpMetricsDrawingYScaleTopDown, io_idxGroup, drawingSize, gridSettings);
        createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConversionFunctions(pGrpMetricsDrawingYScaleTopDown, io_idxGroup, drawingSize);
        createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingAddLines(pGrpMetricsDrawingYScaleTopDown, io_idxGroup, drawingSize);
    }
    {
        ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp = new ZS::Test::CTestStepGroup(
            /* pTest        */ this,
            /* strName      */ "Group " + QString::number(++io_idxGroup) + " YScaleBottomUp (100 * 100) mm",
            /* pTSGrpParent */ pGrpMetricsDrawing );

        CDrawingSize drawingSize("Test-MetricsDrawing-YScaleBottomUp");
        drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
        drawingSize.setMetricUnit(Units.Length.mm);
        drawingSize.setMetricImageCoorsDecimals(1);
        drawingSize.setScreenResolutionInPxPerMM(3.5);
        drawingSize.setImageSize(CPhysVal(100, Units.Length.mm), CPhysVal(100, Units.Length.mm));
        drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::BottomUp);

        CDrawGridSettings gridSettings("Test-MetricsDrawing-YScaleBottomUp");
        gridSettings.setLinesVisible(true);
        gridSettings.setLinesDistMin(20);
        gridSettings.setLabelsVisible(true);

        createTestGroupPrepareScene(pGrpMetricsDrawingYScaleBottomUp, io_idxGroup, drawingSize, gridSettings);
        createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConversionFunctions(pGrpMetricsDrawingYScaleBottomUp, io_idxGroup, drawingSize);
        createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingAddLines(pGrpMetricsDrawingYScaleBottomUp, io_idxGroup, drawingSize);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpImageSizeAndObjectCoordinates->path());
    }
    return pGrpImageSizeAndObjectCoordinates;

} // createTestGroupImageSizeAndObjectCoordinates

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupImageSizeAndObjectCoordinatesPixelsDrawingAddLines(
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
        /* strMethod    */ "createTestGroupImageSizeAndObjectCoordinatesPixelsDrawingAddLines",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpAddLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add Lines",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    for (const QString& strKey : s_hshGraphObjsInstCounts.keys()) {
        s_hshGraphObjsInstCounts[strKey] = 0;
    }

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;

    int idxStep = 0;

    // Line0 (top)
    //------------

    QString strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = m_pDrawingScene->getGraphObjsIdxTree()->buildKeyInTreeStr(strEntryType, strGraphObjName);

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(0, 0)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(100, 0)));

    QStringList strlstExpectedValues;
    strlstExpectedValues.append("line.p1 {-50.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {50.0, 0.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    strlstExpectedValues.append("line.pos {50.0, 0.0} px");
    strlstExpectedValues.append("getLine.p1.x: 0 px");
    strlstExpectedValues.append("getLine.p1.y: 0 px");
    strlstExpectedValues.append("getLine.p2.x: 100 px");
    strlstExpectedValues.append("getLine.p2.y: 0 px");
    strlstExpectedValues.append("getLength: 100.0 px");
    strlstExpectedValues.append("getAngle: 0.0 " + Math::c_strSymbolDegree);
    strlstExpectedValues.append("getPos {50.0, 0.0} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line1 (right)
    //--------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = m_pDrawingScene->getGraphObjsIdxTree()->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(100, 0)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(100, 100)));

    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {0.0, -50.0} px");
    strlstExpectedValues.append("line.p2 {0.0, 50.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    strlstExpectedValues.append("line.pos {100.0, 50.0} px");
    strlstExpectedValues.append("getLine.p1.x: 100 px");
    strlstExpectedValues.append("getLine.p1.y: 0 px");
    strlstExpectedValues.append("getLine.p2.x: 100 px");
    strlstExpectedValues.append("getLine.p2.y: 100 px");
    strlstExpectedValues.append("getLength: 100.0 px");
    strlstExpectedValues.append("getAngle: 270.0 " + Math::c_strSymbolDegree);
    strlstExpectedValues.append("getPos {100.0, 50.0} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line2 (bottom)
    //---------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = m_pDrawingScene->getGraphObjsIdxTree()->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(100, 100)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(0, 100)));

    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {50.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {-50.0, 0.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    strlstExpectedValues.append("line.pos {50.0, 100.0} px");
    strlstExpectedValues.append("getLine.p1.x: 100 px");
    strlstExpectedValues.append("getLine.p1.y: 100 px");
    strlstExpectedValues.append("getLine.p2.x: 0 px");
    strlstExpectedValues.append("getLine.p2.y: 100 px");
    strlstExpectedValues.append("getLength: 100.0 px");
    strlstExpectedValues.append("getAngle: 180.0 " + Math::c_strSymbolDegree);
    strlstExpectedValues.append("getPos {50.0, 100.0} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line3 (left)
    //-------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = m_pDrawingScene->getGraphObjsIdxTree()->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(0, 100)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(0, 0)));

    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {0.0, 50.0} px");
    strlstExpectedValues.append("line.p2 {0.0, -50.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    strlstExpectedValues.append("line.pos {0.0, 50.0} px");
    strlstExpectedValues.append("getLine.p1.x: 0 px");
    strlstExpectedValues.append("getLine.p1.y: 100 px");
    strlstExpectedValues.append("getLine.p2.x: 0 px");
    strlstExpectedValues.append("getLine.p2.y: 0 px");
    strlstExpectedValues.append("getLength: 100.0 px");
    strlstExpectedValues.append("getAngle: 90.0 " + Math::c_strSymbolDegree);
    strlstExpectedValues.append("getPos {0.0, 50.0} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpAddLines->path());
    }
    return pGrpAddLines;

} // createTestGroupImageSizeAndObjectCoordinatesPixelsDrawingAddLines

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingAddLines(
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
        /* strMethod    */ "createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingAddLines",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpAddLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add Lines",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    for (const QString& strKey : s_hshGraphObjsInstCounts.keys()) {
        s_hshGraphObjsInstCounts[strKey] = 0;
    }

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;

    int idxStep = 0;

    // Line0 (top)
    //------------

    QString strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = m_pDrawingScene->getGraphObjsIdxTree()->buildKeyInTreeStr(strEntryType, strGraphObjName);

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(0, 0)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(100, 0)));

    QStringList strlstExpectedValues;
    strlstExpectedValues.append("line.p1 {-175.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {175.0, 0.0} px");
    strlstExpectedValues.append("line.length: 350.0 px");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("line.pos {175.0, 0.0} px");
    } else {
        strlstExpectedValues.append("line.pos {175.0, 350.0} px");
    }
    strlstExpectedValues.append("getLine.p1.x: 0.0 mm");
    strlstExpectedValues.append("getLine.p1.y: 0.0 mm");
    strlstExpectedValues.append("getLine.p2.x: 100.0 mm");
    strlstExpectedValues.append("getLine.p2.y: 0.0 mm");
    strlstExpectedValues.append("getLength: 100.0 mm");
    strlstExpectedValues.append("getAngle: 0.0 " + Math::c_strSymbolDegree);
    strlstExpectedValues.append("getPos {50.0, 0.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line1 (right)
    //--------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = m_pDrawingScene->getGraphObjsIdxTree()->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(100, 0)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(100, 100)));

    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {0.0, -175.0} px");
    strlstExpectedValues.append("line.p2 {0.0, 175.0} px");
    strlstExpectedValues.append("line.length: 350.0 px");
    strlstExpectedValues.append("line.pos {350.0, 175.0} px");
    strlstExpectedValues.append("getLine.p1.x: 100.0 mm");
    strlstExpectedValues.append("getLine.p1.y: 0.0 mm");
    strlstExpectedValues.append("getLine.p2.x: 100.0 mm");
    strlstExpectedValues.append("getLine.p2.y: 100.0 mm");
    strlstExpectedValues.append("getLength: 100.0 mm");
    strlstExpectedValues.append("getAngle: 270.0 " + Math::c_strSymbolDegree);
    strlstExpectedValues.append("getPos {100.0, 50.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line2 (bottom)
    //---------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = m_pDrawingScene->getGraphObjsIdxTree()->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(100, 100)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(0, 100)));

    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {175.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {-175.0, 0.0} px");
    strlstExpectedValues.append("line.length: 350.0 px");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("line.pos {175.0, 350.0} px");
    } else {
        strlstExpectedValues.append("line.pos {175.0, 0.0} px");
    }
    strlstExpectedValues.append("getLine.p1.x: 100.0 mm");
    strlstExpectedValues.append("getLine.p1.y: 100.0 mm");
    strlstExpectedValues.append("getLine.p2.x: 0.0 mm");
    strlstExpectedValues.append("getLine.p2.y: 100.0 mm");
    strlstExpectedValues.append("getLength: 100.0 mm");
    strlstExpectedValues.append("getAngle: 180.0 " + Math::c_strSymbolDegree);
    strlstExpectedValues.append("getPos {50.0, 100.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line3 (left)
    //-------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = m_pDrawingScene->getGraphObjsIdxTree()->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(0, 100)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(0, 0)));

    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {0.0, 175.0} px");
    strlstExpectedValues.append("line.p2 {0.0, -175.0} px");
    strlstExpectedValues.append("line.length: 350.0 px");
    strlstExpectedValues.append("line.pos {0.0, 175.0} px");
    strlstExpectedValues.append("getLine.p1.x: 0.0 mm");
    strlstExpectedValues.append("getLine.p1.y: 100.0 mm");
    strlstExpectedValues.append("getLine.p2.x: 0.0 mm");
    strlstExpectedValues.append("getLine.p2.y: 0.0 mm");
    strlstExpectedValues.append("getLength: 100.0 mm");
    strlstExpectedValues.append("getAngle: 90.0 " + Math::c_strSymbolDegree);
    strlstExpectedValues.append("getPos {0.0, 50.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpAddLines->path());
    }
    return pGrpAddLines;

} // createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingAddLines

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConversionFunctions(
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
        /* strMethod    */ "createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConversionFunctions",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpDrawingSceneConvert = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " DrawingScene.Convert",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValPoint(pGrpDrawingSceneConvert, io_idxGroup, i_drawingSize);
    createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValSize(pGrpDrawingSceneConvert, io_idxGroup, i_drawingSize);
    createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValLine(pGrpDrawingSceneConvert, io_idxGroup, i_drawingSize);
    createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValRect(pGrpDrawingSceneConvert, io_idxGroup, i_drawingSize);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpDrawingSceneConvert->path());
    }
    return pGrpDrawingSceneConvert;
}

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValPoint(
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
        /* strMethod    */ "createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValPoint",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpConvertToPhysValPoint = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " convert(Point)",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    QStringList strlstExpectedValues;

    // px -> mm
    //---------

    ZS::Test::CTestStepGroup* pGrpConvertToPhysValPointPx2MM = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " convert(px, mm)",
        /* pTSGrpParent */ pGrpConvertToPhysValPoint );

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Point(0, 0) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Point(0, 0) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValPointPx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValPoint(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Point", QPointF(0.0, 0.0));
    pTestStep->setConfigValue("Point.Unit", "px");
    pTestStep->setConfigValue("Point.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{0.0, 0.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Point(100, 100) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Point(100, 100) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValPointPx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValPoint(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Point", QPointF(100.0, 100.0));
    pTestStep->setConfigValue("Point.Unit", "px");
    pTestStep->setConfigValue("Point.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{28.6, 28.6} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Point(200, 200) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Point(200, 200) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValPointPx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValPoint(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Point", QPointF(200.0, 200.0));
    pTestStep->setConfigValue("Point.Unit", "px");
    pTestStep->setConfigValue("Point.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{57.1, 57.1} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Point(350, 350) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Point(350, 350) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValPointPx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValPoint(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Point", QPointF(350.0, 350.0));
    pTestStep->setConfigValue("Point.Unit", "px");
    pTestStep->setConfigValue("Point.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{100.0, 100.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    // mm -> px
    //---------

    ZS::Test::CTestStepGroup* pGrpConvertToPhysValPointMM2Px = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " convert(mm, px)",
        /* pTSGrpParent */ pGrpConvertToPhysValPoint );

    idxStep = 0;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Point(0, 0) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Point(0, 0) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValPointMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValPoint(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Point", QPointF(0.0, 0.0));
    pTestStep->setConfigValue("Point.Unit", "mm");
    pTestStep->setConfigValue("Point.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{0, 0} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Point(20, 20) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Point(20, 20) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValPointMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValPoint(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Point", QPointF(20.0, 20.0));
    pTestStep->setConfigValue("Point.Unit", "mm");
    pTestStep->setConfigValue("Point.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{70, 70} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Point(50, 50) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Point(50, 50) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValPointMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValPoint(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Point", QPointF(50.0, 50.0));
    pTestStep->setConfigValue("Point.Unit", "mm");
    pTestStep->setConfigValue("Point.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{175, 175} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Point(100, 100) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Point(100, 100) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValPointMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValPoint(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Point", QPointF(100.0, 100.0));
    pTestStep->setConfigValue("Point.Unit", "mm");
    pTestStep->setConfigValue("Point.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{350, 350} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpConvertToPhysValPoint->path());
    }
    return pGrpConvertToPhysValPoint;

} // createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValPoint

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValSize(
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
        /* strMethod    */ "createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValSize",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpConvertToPhysValSize = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " convert(Size)",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    QStringList strlstExpectedValues;

    // px -> mm
    //---------

    ZS::Test::CTestStepGroup* pGrpConvertToPhysValSizePx2MM = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " convert(px, mm)",
        /* pTSGrpParent */ pGrpConvertToPhysValSize );

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(0, 0) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Size(0, 0) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValSizePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(0.0, 0.0));
    pTestStep->setConfigValue("Size.Unit", "px");
    pTestStep->setConfigValue("Size.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{0.0, 0.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(100, 0) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Size(100, 0) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValSizePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(100.0, 0.0));
    pTestStep->setConfigValue("Size.Unit", "px");
    pTestStep->setConfigValue("Size.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{28.6, 0.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(0, 100) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Size(0, 100) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValSizePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(0.0, 100.0));
    pTestStep->setConfigValue("Size.Unit", "px");
    pTestStep->setConfigValue("Size.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{0.0, 28.6} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(350, 0) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Size(350, 0) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValSizePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(350.0, 0.0));
    pTestStep->setConfigValue("Size.Unit", "px");
    pTestStep->setConfigValue("Size.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("(100.0, 0.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(0, 350) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Size(0, 350) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValSizePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(0.0, 350.0));
    pTestStep->setConfigValue("Size.Unit", "px");
    pTestStep->setConfigValue("Size.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{0.0, 100.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(1, 1) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Size(1, 1) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValSizePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(1.0, 1.0));
    pTestStep->setConfigValue("Size.Unit", "px");
    pTestStep->setConfigValue("Size.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{0.3, 0.3} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(10, 10) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Size(10, 10) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValSizePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(10.0, 10.0));
    pTestStep->setConfigValue("Size.Unit", "px");
    pTestStep->setConfigValue("Size.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{2.9, 2.9} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(100, 100) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Size(100, 100) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValSizePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(100.0, 100.0));
    pTestStep->setConfigValue("Size.Unit", "px");
    pTestStep->setConfigValue("Size.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{28.6, 28.6} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(350, 350) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Size(350, 350) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValSizePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(350.0, 350.0));
    pTestStep->setConfigValue("Size.Unit", "px");
    pTestStep->setConfigValue("Size.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{100.0, 100.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    // mm -> px
    //---------

    ZS::Test::CTestStepGroup* pGrpConvertToPhysValSizeMM2Px = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " convert(mm, px)",
        /* pTSGrpParent */ pGrpConvertToPhysValSize );

    idxStep = 0;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(0, 0) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Size(0, 0) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValSizeMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(0.0, 0.0));
    pTestStep->setConfigValue("Size.Unit", "mm");
    pTestStep->setConfigValue("Size.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{0, 0} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(1, 0) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Size(1, 0) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValSizeMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(1.0, 0.0));
    pTestStep->setConfigValue("Size.Unit", "mm");
    pTestStep->setConfigValue("Size.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{4, 0} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(0, 1) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Size(0, 1) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValSizeMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(0.0, 1.0));
    pTestStep->setConfigValue("Size.Unit", "mm");
    pTestStep->setConfigValue("Size.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{0, 4} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(10, 0) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Size(10, 0) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValSizeMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(10.0, 0.0));
    pTestStep->setConfigValue("Size.Unit", "mm");
    pTestStep->setConfigValue("Size.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{35, 0} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(0, 10) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Size(0, 10) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValSizeMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(0.0, 10.0));
    pTestStep->setConfigValue("Size.Unit", "mm");
    pTestStep->setConfigValue("Size.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{0, 35} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(20, 20) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Size(20, 20) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValSizeMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(20.0, 20.0));
    pTestStep->setConfigValue("Size.Unit", "mm");
    pTestStep->setConfigValue("Size.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{70, 70} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(50, 50) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Size(50, 50) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValSizeMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(50.0, 50.0));
    pTestStep->setConfigValue("Size.Unit", "mm");
    pTestStep->setConfigValue("Size.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{175, 175} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(100, 100) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Size(100, 100) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValSizeMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(100.0, 100.0));
    pTestStep->setConfigValue("Size.Unit", "mm");
    pTestStep->setConfigValue("Size.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{350, 350} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpConvertToPhysValSize->path());
    }
    return pGrpConvertToPhysValSize;

} // createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValSize

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValLine(
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
        /* strMethod    */ "createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValLine",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpConvertToPhysValLine = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " convert(Line)",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    QStringList strlstExpectedValues;

    // px -> mm
    //---------

    ZS::Test::CTestStepGroup* pGrpConvertToPhysValLinePx2MM = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " convert(px, mm)",
        /* pTSGrpParent */ pGrpConvertToPhysValLine );

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(0, 0) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Line(0, 0) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValLinePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Line", QLineF(0.0, 0.0, 100.0, 100.0));
    pTestStep->setConfigValue("Line.Unit", "px");
    pTestStep->setConfigValue("Line.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{0.0, 0.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(100, 100) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Line(100, 100) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValLinePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Line", QLineF(100.0, 100.0, 100.0, 100.0));
    pTestStep->setConfigValue("Line.Unit", "px");
    pTestStep->setConfigValue("Line.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{28.6, 28.6} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(200, 200) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Line(200, 200) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValLinePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Line", QLineF(200.0, 200.0, 100.0, 100.0));
    pTestStep->setConfigValue("Line.Unit", "px");
    pTestStep->setConfigValue("Line.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{57.1, 57.1} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(350, 350) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Line(350, 350) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValLinePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Line", QLineF(350.0, 350.0, 100.0, 100.0));
    pTestStep->setConfigValue("Line.Unit", "px");
    pTestStep->setConfigValue("Line.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{100.0, 100.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    // mm -> px
    //---------

    ZS::Test::CTestStepGroup* pGrpConvertToPhysValLineMM2Px = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " convert(mm, px)",
        /* pTSGrpParent */ pGrpConvertToPhysValLine );

    idxStep = 0;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(0, 0) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Line(0, 0) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValLineMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Line", QLineF(0.0, 0.0, 100.0, 100.0));
    pTestStep->setConfigValue("Line.Unit", "mm");
    pTestStep->setConfigValue("Line.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{0, 0} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(20, 20) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Line(20, 20) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValLineMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Line", QLineF(20.0, 20.0, 100.0, 100.0));
    pTestStep->setConfigValue("Line.Unit", "mm");
    pTestStep->setConfigValue("Line.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{70, 70} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(50, 50) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Line(50, 50) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValLineMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Line", QLineF(50.0, 50.0, 100.0, 100.0));
    pTestStep->setConfigValue("Line.Unit", "mm");
    pTestStep->setConfigValue("Line.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{175, 175} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(100, 100) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Line(100, 100) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValLineMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Line", QLineF(100.0, 100.0, 100.0, 100.0));
    pTestStep->setConfigValue("Line.Unit", "mm");
    pTestStep->setConfigValue("Line.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{350, 350} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpConvertToPhysValLine->path());
    }
    return pGrpConvertToPhysValLine;

} // createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValLine

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValRect(
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
        /* strMethod    */ "createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValRect",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpConvertToPhysValRect = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " convert(Rect)",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    QStringList strlstExpectedValues;

    // px -> mm
    //---------

    ZS::Test::CTestStepGroup* pGrpConvertToPhysValRectPx2MM = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " convert(px, mm)",
        /* pTSGrpParent */ pGrpConvertToPhysValRect );

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Rect(0, 0) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Rect(0, 0) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValRectPx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Rect", QRectF(0.0, 0.0, 100.0, 100.0));
    pTestStep->setConfigValue("Rect.Unit", "px");
    pTestStep->setConfigValue("Rect.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{0.0, 0.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Rect(100, 100) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Rect(100, 100) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValRectPx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Rect", QRectF(100.0, 100.0, 100.0, 100.0));
    pTestStep->setConfigValue("Rect.Unit", "px");
    pTestStep->setConfigValue("Rect.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{28.6, 28.6} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Rect(200, 200) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Rect(200, 200) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValRectPx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Rect", QRectF(200.0, 200.0, 100.0, 100.0));
    pTestStep->setConfigValue("Rect.Unit", "px");
    pTestStep->setConfigValue("Rect.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{57.1, 57.1} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Rect(350, 350) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Rect(350, 350) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValRectPx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Rect", QRectF(350.0, 350.0, 100.0, 100.0));
    pTestStep->setConfigValue("Rect.Unit", "px");
    pTestStep->setConfigValue("Rect.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{100.0, 100.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    // mm -> px
    //---------

    ZS::Test::CTestStepGroup* pGrpConvertToPhysValRectMM2Px = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " convert(mm, px)",
        /* pTSGrpParent */ pGrpConvertToPhysValRect );

    idxStep = 0;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Rect(0, 0) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Rect(0, 0) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValRectMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Rect", QRectF(0.0, 0.0, 100.0, 100.0));
    pTestStep->setConfigValue("Rect.Unit", "mm");
    pTestStep->setConfigValue("Rect.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{0, 0} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Rect(20, 20) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Rect(20, 20) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValRectMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Rect", QRectF(20.0, 20.0, 100.0, 100.0));
    pTestStep->setConfigValue("Rect.Unit", "mm");
    pTestStep->setConfigValue("Rect.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{70, 70} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Rect(50, 50) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Rect(50, 50) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValRectMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Rect", QRectF(50.0, 50.0, 100.0, 100.0));
    pTestStep->setConfigValue("Rect.Unit", "mm");
    pTestStep->setConfigValue("Rect.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{175, 175} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Rect(100, 100) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Rect(100, 100) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValRectMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Rect", QRectF(100.0, 100.0, 100.0, 100.0));
    pTestStep->setConfigValue("Rect.Unit", "mm");
    pTestStep->setConfigValue("Rect.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{350, 350} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpConvertToPhysValRect->path());
    }
    return pGrpConvertToPhysValRect;

} // createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValRect

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

    for (const QString& strKey : s_hshGraphObjsInstCounts.keys()) {
        s_hshGraphObjsInstCounts[strKey] = 0;
    }

    CDrawingSize drawingSize("Test-PixelsDrawing");
    drawingSize.setDimensionUnit(EScaleDimensionUnit::Pixels);
    drawingSize.setScreenResolutionInPxPerMM(1.0);
    drawingSize.setImageSize(CPhysVal(800, Units.Length.px), CPhysVal(600, Units.Length.px));

    CDrawGridSettings gridSettings("Test-PixelsDrawing");
    gridSettings.setLinesVisible(true);
    gridSettings.setLabelsVisible(true);

    createTestGroupPrepareScene(pGrpPixelsDrawing, io_idxGroup, drawingSize, gridSettings);
    createTestGroupAddStandardShapes(pGrpPixelsDrawing, io_idxGroup, drawingSize);
    createTestGroupDrawStandardShapes(pGrpPixelsDrawing, io_idxGroup, drawingSize);
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

    createTestGroupMetricsDrawingYScaleTopDown(pGrpMetricsDrawing, io_idxGroup);
    createTestGroupMetricsDrawingYScaleBottomUp(pGrpMetricsDrawing, io_idxGroup);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpMetricsDrawing->path());
    }
    return pGrpMetricsDrawing;

} // createTestGroupMetricsDrawing

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupMetricsDrawingYScaleTopDown(
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
        /* strMethod    */ "createTestGroupMetricsDrawingYScaleTopDown",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpMetricsDrawing = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Metrics Drawing Y Scale TopDown",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    for (const QString& strKey : s_hshGraphObjsInstCounts.keys()) {
        s_hshGraphObjsInstCounts[strKey] = 0;
    }

    CDrawingSize drawingSize("Test-MetricsDrawing-YScaleTopDown");
    drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
    drawingSize.setScreenResolutionInPxPerMM(1.0);
    drawingSize.setImageSize(CPhysVal(800, Units.Length.mm), CPhysVal(600, Units.Length.mm));
    drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::TopDown);

    CDrawGridSettings gridSettings("Test-PixelsDrawing");
    gridSettings.setLinesVisible(true);
    gridSettings.setLabelsVisible(true);

    createTestGroupPrepareScene(pGrpMetricsDrawing, io_idxGroup, drawingSize, gridSettings);
    createTestGroupAddStandardShapes(pGrpMetricsDrawing, io_idxGroup, drawingSize);
    createTestGroupDrawStandardShapes(pGrpMetricsDrawing, io_idxGroup, drawingSize);
    createTestGroupSaveLoadFile(pGrpMetricsDrawing, io_idxGroup, drawingSize);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpMetricsDrawing->path());
    }
    return pGrpMetricsDrawing;

} // createTestGroupMetricsDrawingYScaleTopDown

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupMetricsDrawingYScaleBottomUp(
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
        /* strMethod    */ "createTestGroupMetricsDrawingYScaleBottomUp",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpMetricsDrawing = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Metrics Drawing Y Scale BottomUp",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    for (const QString& strKey : s_hshGraphObjsInstCounts.keys()) {
        s_hshGraphObjsInstCounts[strKey] = 0;
    }

    CDrawingSize drawingSize("Test-MetricsDrawing-YScaleBottomUp");
    drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
    drawingSize.setScreenResolutionInPxPerMM(1.0);
    drawingSize.setImageSize(CPhysVal(800, Units.Length.mm), CPhysVal(600, Units.Length.mm));
    drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::BottomUp);

    CDrawGridSettings gridSettings("Test-PixelsDrawing");
    gridSettings.setLinesVisible(true);
    gridSettings.setLabelsVisible(true);

    createTestGroupPrepareScene(pGrpMetricsDrawing, io_idxGroup, drawingSize, gridSettings);
    createTestGroupAddStandardShapes(pGrpMetricsDrawing, io_idxGroup, drawingSize);
    createTestGroupDrawStandardShapes(pGrpMetricsDrawing, io_idxGroup, drawingSize);
    createTestGroupSaveLoadFile(pGrpMetricsDrawing, io_idxGroup, drawingSize);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpMetricsDrawing->path());
    }
    return pGrpMetricsDrawing;

} // createTestGroupMetricsDrawingYScaleBottomUp

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

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpAddStandardShapes->path());
    }
    return pGrpAddStandardShapes;

} // createTestGroupAddStandardShapes

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupDrawStandardShapes(
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
        /* strMethod    */ "createTestGroupDrawStandardShapes",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpDrawStandardShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Draw " + CObjFactory::c_strGroupNameStandardShapes,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    //createTestGroupDrawStandardShapesLines(pGrpDrawStandardShapes, io_idxGroup);
    //createTestGroupDrawStandardShapesGroups(pGrpDrawStandardShapes, io_idxGroup);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpDrawStandardShapes->path());
    }
    return pGrpDrawStandardShapes;

} // createTestGroupDrawStandardShapes

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
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Lines",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;

    // Line0 (vertical)
    //-----------------

    QString strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpLine0 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName,
        /* pTSGrpParent */ pGrpAddLines );

    ZS::Test::CTestStepGroup* pGrpAddLine0 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add " + strGraphObjName,
        /* pTSGrpParent */ pGrpLine0 );

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLine0,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(300, 250)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(300, 350)));

    ZS::Test::CTestStepGroup* pGrpLine0ShowAndMoveLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Show and Move Labels",
        /* pTSGrpParent */ pGrpLine0 );

    createTestGroupShowAndMoveTextLabel(
        pGrpLine0ShowAndMoveLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strLabelName,
        QPoint(295, 305), QPoint(320, 260));
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine0ShowAndMoveLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameDY,
        QPoint(295, 305), QPoint(270, 280));

    // Line1 (horizontal)
    //-------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpLine1 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName,
        /* pTSGrpParent */ pGrpAddLines );

    ZS::Test::CTestStepGroup* pGrpAddLine1 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add " + strGraphObjName,
        /* pTSGrpParent */ pGrpLine1 );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLine1,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(250, 300)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(350, 300)));

    ZS::Test::CTestStepGroup* pGrpLine1ShowAndMoveLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Show and Move Labels",
        /* pTSGrpParent */ pGrpLine1 );

    createTestGroupShowAndMoveTextLabel(
        pGrpLine1ShowAndMoveLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strLabelName,
        QPoint(295, 305), QPoint(340, 290));
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine1ShowAndMoveLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameDX,
        QPoint(295, 305), QPoint(280, 325));

    // Line2 (Checkmark line from left to bottom)
    //----------------------------------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpLine2 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName,
        /* pTSGrpParent */ pGrpAddLines );

    ZS::Test::CTestStepGroup* pGrpAddLine2 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add " + strGraphObjName,
        /* pTSGrpParent */ pGrpLine2 );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLine2,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(250, 250)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(325, 350)));

    // Line3 (Checkmark line from bottom to right)
    //---------------------------------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpLine3 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName,
        /* pTSGrpParent */ pGrpAddLines );

    ZS::Test::CTestStepGroup* pGrpAddLine3 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add " + strGraphObjName,
        /* pTSGrpParent */ pGrpLine3 );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLine3,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(325, 350)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(350, 325)));

    // Line4 (Top line of rectangle in second quadrant)
    //-------------------------------------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpLine4 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName,
        /* pTSGrpParent */ pGrpAddLines );

    ZS::Test::CTestStepGroup* pGrpAddLine4 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add " + strGraphObjName,
        /* pTSGrpParent */ pGrpLine4 );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLine4,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(320, 270)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(330, 270)));

    // Line5 (Right line of rectangle in second quadrant)
    //----------------------------------------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpLine5 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName,
        /* pTSGrpParent */ pGrpAddLines );

    ZS::Test::CTestStepGroup* pGrpAddLine5 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add " + strGraphObjName,
        /* pTSGrpParent */ pGrpLine5 );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLine5,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(330, 270)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(330, 280)));

    // Line6 (Bottom line of rectangle in second quadrant)
    //----------------------------------------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpLine6 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName,
        /* pTSGrpParent */ pGrpAddLines );

    ZS::Test::CTestStepGroup* pGrpAddLine6 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add " + strGraphObjName,
        /* pTSGrpParent */ pGrpLine6 );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLine6,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(330, 280)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(320, 280)));

    // Line7 (Left line of rectangle in second quadrant)
    //----------------------------------------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpLine7 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName,
        /* pTSGrpParent */ pGrpAddLines );

    ZS::Test::CTestStepGroup* pGrpAddLine7 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add " + strGraphObjName,
        /* pTSGrpParent */ pGrpLine7 );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLine7,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObj(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);
    pTestStep->setConfigValue("Point1", qPoint2Str(QPoint(320, 280)));
    pTestStep->setConfigValue("Point2", qPoint2Str(QPoint(320, 270)));

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
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Groups",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);
    QString strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " UnsetCurrentDrawingTool",
        /* strOperation    */ "DrawingScene.setCurrentDrawingTool(null)",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepUnsetCurrentDrawingTool(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", "");
    pTestStep->setConfigValue("GraphObjType", "");

    // Group0 (short lines which form a rectangle)
    //--------------------------------------------

    QString strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpGroup0 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName,
        /* pTSGrpParent */ pGrpAddGroups );

    ZS::Test::CTestStepGroup* pGrpAddGroup0 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add " + strGraphObjName,
        /* pTSGrpParent */ pGrpGroup0 );

    idxStep = 0;

    QPoint ptMouseStart = QPoint(325, 265);
    QPoint ptMouseStop = QPoint(325, 270);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptMouseStart.setY(i_drawingSize.imageHeightInPixels() - ptMouseStart.y() - 1);
        ptMouseStop.setY(i_drawingSize.imageHeightInPixels() - ptMouseStop.y() - 1);
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvents(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* pGrpParent      */ pGrpAddGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
    pTestStep->setConfigValue("MousePosStart", ptMouseStart);
    pTestStep->setConfigValue("MousePosStop", ptMouseStop);
    pTestStep->setConfigValue("MovesCount", 5);

    QPoint ptMousePos = QPoint(ptMouseStop);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MousePressEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpAddGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpAddGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);

    ptMouseStart = ptMouseStop;
    ptMouseStop = QPoint(330, 275);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptMouseStop.setY(i_drawingSize.imageHeightInPixels() - ptMouseStop.y() - 1);
    }
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvents(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* pGrpParent      */ pGrpAddGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
    pTestStep->setConfigValue("MousePosStart", ptMouseStart);
    pTestStep->setConfigValue("MousePosStop", ptMouseStop);
    pTestStep->setConfigValue("MovesCount", 5);

    ptMousePos = ptMouseStop;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MousePressEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpAddGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);
    pTestStep->setConfigValue("KeyboardModifiers", Qt::ControlModifier);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpAddGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);
    pTestStep->setConfigValue("KeyboardModifiers", Qt::ControlModifier);

    ptMouseStart = ptMouseStop;
    ptMouseStop = QPoint(325, 280);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptMouseStop.setY(i_drawingSize.imageHeightInPixels() - ptMouseStop.y() - 1);
    }
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvents(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* pGrpParent      */ pGrpAddGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
    pTestStep->setConfigValue("MousePosStart", ptMouseStart);
    pTestStep->setConfigValue("MousePosStop", ptMouseStop);
    pTestStep->setConfigValue("MovesCount", 5);

    ptMousePos = ptMouseStop;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MousePressEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpAddGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);
    pTestStep->setConfigValue("KeyboardModifiers", Qt::ControlModifier);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpAddGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);
    pTestStep->setConfigValue("KeyboardModifiers", Qt::ControlModifier);

    ptMouseStart = ptMouseStop;
    ptMouseStop = QPoint(320, 275);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptMouseStop.setY(i_drawingSize.imageHeightInPixels() - ptMouseStop.y() - 1);
    }
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvents(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* pGrpParent      */ pGrpAddGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
    pTestStep->setConfigValue("MousePosStart", ptMouseStart);
    pTestStep->setConfigValue("MousePosStop", ptMouseStop);
    pTestStep->setConfigValue("MovesCount", 5);

    ptMousePos = ptMouseStop;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MousePressEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpAddGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);
    pTestStep->setConfigValue("KeyboardModifiers", Qt::ControlModifier);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpAddGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);
    pTestStep->setConfigValue("KeyboardModifiers", Qt::ControlModifier);

    ptMouseStart = ptMouseStop;
    ptMouseStop = QPoint(315, 275);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptMouseStop.setY(i_drawingSize.imageHeightInPixels() - ptMouseStop.y() - 1);
    }
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvents(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* pGrpParent      */ pGrpAddGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
    pTestStep->setConfigValue("MousePosStart", ptMouseStart);
    pTestStep->setConfigValue("MousePosStop", ptMouseStop);
    pTestStep->setConfigValue("MovesCount", 5);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " GroupSelectedObjects",
        /* strOperation    */ "DrawingScene.groupGraphObjsSelected()",
        /* pGrpParent      */ pGrpAddGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepSetCurrentDrawingTool(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);

    // Group1 (checkmark)
    //-------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpGroup1 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName,
        /* pTSGrpParent */ pGrpAddGroups );

    ZS::Test::CTestStepGroup* pGrpAddGroup1 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add " + strGraphObjName,
        /* pTSGrpParent */ pGrpGroup1 );

    idxStep = 0;

    ptMouseStart = QPoint(280, 300);
    ptMouseStop = QPoint(287, 300);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptMouseStart.setY(i_drawingSize.imageHeightInPixels() - ptMouseStart.y() - 1);
        ptMouseStop.setY(i_drawingSize.imageHeightInPixels() - ptMouseStop.y() - 1);
    }
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvents(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* pGrpParent      */ pGrpAddGroup1,
        /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
    pTestStep->setConfigValue("MousePosStart", ptMouseStart);
    pTestStep->setConfigValue("MousePosStop", ptMouseStop);
    pTestStep->setConfigValue("MovesCount", 5);

    ptMousePos = ptMouseStop;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MousePressEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpAddGroup1,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpAddGroup1,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);

    ptMouseStart = ptMouseStop;
    ptMouseStop = QPoint(337, 337);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptMouseStop.setY(i_drawingSize.imageHeightInPixels() - ptMouseStop.y() - 1);
    }
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvents(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* pGrpParent      */ pGrpAddGroup1,
        /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
    pTestStep->setConfigValue("MousePosStart", ptMouseStart);
    pTestStep->setConfigValue("MousePosStop", ptMouseStop);
    pTestStep->setConfigValue("MovesCount", 5);

    ptMousePos = ptMouseStop;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MousePressEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpAddGroup1,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);
    pTestStep->setConfigValue("KeyboardModifiers", Qt::ControlModifier);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpAddGroup1,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);
    pTestStep->setConfigValue("KeyboardModifiers", Qt::ControlModifier);

    ptMouseStart = ptMouseStop;
    ptMouseStop = QPoint(347, 337);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptMouseStop.setY(i_drawingSize.imageHeightInPixels() - ptMouseStop.y() - 1);
    }
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseMoveEvents(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseStart) + ", " + qPoint2Str(ptMouseStop) + ")",
        /* pGrpParent      */ pGrpAddGroup1,
        /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
    pTestStep->setConfigValue("MousePosStart", ptMouseStart);
    pTestStep->setConfigValue("MousePosStop", ptMouseStop);
    pTestStep->setConfigValue("MovesCount", 5);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " GroupSelectedObjects",
        /* strOperation    */ "DrawingScene.groupGraphObjsSelected()",
        /* pGrpParent      */ pGrpAddGroup1,
        /* szDoTestStepFct */ SLOT(doTestStepSetCurrentDrawingTool(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);

    // Group2 (horizonal and vertical lines together with checkmark and rectangle group)
    //----------------------------------------------------------------------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    createTestGroupDrawMouseEventsCreateGroup(
        pGrpAddGroups, io_idxGroup,
        strGraphObjName,
        QPoint(120, 120), QPoint(480, 480), 10, QPoint(490, 490));

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpAddGroups->path());
    }
    return pGrpAddGroups;

} // createTestGroupAddStandardShapesGroups

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupDrawStandardShapesLines(
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
        /* strMethod    */ "createTestGroupDrawStandardShapesLines",
        /* strAddInfo   */ strMthInArgs );

    // Lines
    //------

    ZS::Test::CTestStepGroup* pGrpDrawLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Create Lines",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);

    // Line0
    //------

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;
    QString strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpDrawLine0 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Create, Resize, Move " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLines );

    createTestGroupDrawMouseEventsCreateObject(
        pGrpDrawLine0, io_idxGroup,
        CObjFactory::c_strGroupNameStandardShapes, EGraphObjTypeLine, strGraphObjName,
        QPoint(100, 100), QPoint(100, 150), QPoint(110, 160));

    ZS::Test::CTestStepGroup* pGrpLine0ShowAndMoveLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Show and Move Labels",
        /* pTSGrpParent */ pGrpDrawLine0 );

    ZS::Test::CTestStepGroup* pGrpLine0ShowAndMoveTextLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Text Labels",
        /* pTSGrpParent */ pGrpLine0ShowAndMoveLabels );
    createTestGroupShowAndMoveTextLabel(
        pGrpLine0ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strLabelName,
        QPoint(95, 125), QPoint(60, 100));
    createTestGroupShowAndMoveTextLabel(
        pGrpLine0ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameP1,
        QPoint(95, 100), QPoint(95, 90));
    createTestGroupShowAndMoveTextLabel(
        pGrpLine0ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameP2,
        QPoint(95, 150), QPoint(95, 160));

    ZS::Test::CTestStepGroup* pGrpLine0ShowAndMoveGeometryLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Geometry Labels",
        /* pTSGrpParent */ pGrpLine0ShowAndMoveLabels );
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine0ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameLength,
        QPoint(105, 125), QPoint(125, 125));
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine0ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameAngle,
        QPoint(90, 125), QPoint(70, 125));

    ZS::Test::CTestStepGroup* pGrpDrawResizeLine0 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Resize " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLine0 );

    int idxStep = 0;

    addTestStepDrawMouseEventsMove(
        pGrpDrawResizeLine0, idxStep, QPoint(90, 140), QPoint(100, 148));
    addTestStepsDrawMouseEventsMovePressMoveRelease(
        pGrpDrawResizeLine0, idxStep, QPoint(100, 148), QPoint(200, 200));
    addTestStepDrawMouseEventsMove(
        pGrpDrawResizeLine0, idxStep, QPoint(200, 200), QPoint(210, 210));

    ZS::Test::CTestStepGroup* pGrpDrawMoveLine0 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Move " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLine0 );

    idxStep = 0;

    addTestStepDrawMouseEventsMove(
        pGrpDrawMoveLine0, idxStep, QPoint(140, 140), QPoint(150, 150));
    addTestStepsDrawMouseEventsMovePressMoveRelease(
        pGrpDrawMoveLine0, idxStep, QPoint(150, 150), QPoint(200, 200));
    addTestStepDrawMouseEventsMove(
        pGrpDrawMoveLine0, idxStep, QPoint(200, 200), QPoint(210, 200)); // move sidewards away from line

    // Line1
    //------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpDrawLine1 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Create, Resize, Move " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLines );

    createTestGroupDrawMouseEventsCreateObject(
        pGrpDrawLine1, io_idxGroup,
        CObjFactory::c_strGroupNameStandardShapes, EGraphObjTypeLine, strGraphObjName,
        QPoint(500, 100), QPoint(500, 150), QPoint(510, 160));

    ZS::Test::CTestStepGroup* pGrpLine1ShowAndMoveLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Show and Move Labels",
        /* pTSGrpParent */ pGrpDrawLine1 );

    ZS::Test::CTestStepGroup* pGrpLine1ShowAndMoveTextLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Text Labels",
        /* pTSGrpParent */ pGrpLine1ShowAndMoveLabels );

    createTestGroupShowAndMoveTextLabel(
        pGrpLine1ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strLabelName,
        QPoint(495, 125), QPoint(440, 100));
    createTestGroupShowAndMoveTextLabel(
        pGrpLine1ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameP1,
        QPoint(495, 100), QPoint(495, 90));
    createTestGroupShowAndMoveTextLabel(
        pGrpLine1ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameP2,
        QPoint(495, 150), QPoint(495, 160));

    ZS::Test::CTestStepGroup* pGrpLine1ShowAndMoveGeometryLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Geometry Labels",
        /* pTSGrpParent */ pGrpLine1ShowAndMoveLabels );
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine1ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameLength,
        QPoint(505, 125), QPoint(525, 125));
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine1ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameAngle,
        QPoint(490, 125), QPoint(470, 125));

    ZS::Test::CTestStepGroup* pGrpDrawResizeLine1 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Resize " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLine1 );

    idxStep = 0;

    addTestStepDrawMouseEventsMove(
        pGrpDrawResizeLine1, idxStep, QPoint(490, 140), QPoint(500, 148));
    addTestStepsDrawMouseEventsMovePressMoveRelease(
        pGrpDrawResizeLine1, idxStep, QPoint(500, 148), QPoint(400, 200));
    addTestStepDrawMouseEventsMove(
        pGrpDrawResizeLine1, idxStep, QPoint(400, 200), QPoint(390, 210));

    ZS::Test::CTestStepGroup* pGrpDrawMoveLine1 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Move " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLine1 );

    idxStep = 0;

    addTestStepDrawMouseEventsMove(
        pGrpDrawMoveLine1, idxStep, QPoint(440, 140), QPoint(450, 150));
    addTestStepsDrawMouseEventsMovePressMoveRelease(
        pGrpDrawMoveLine1, idxStep, QPoint(450, 150), QPoint(400, 200));
    addTestStepDrawMouseEventsMove(
        pGrpDrawMoveLine1, idxStep, QPoint(400, 200), QPoint(390, 200)); // move sidewards away from line

    // Line2
    //------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpDrawLine2 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Create, Resize, Move " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLines );

    createTestGroupDrawMouseEventsCreateObject(
        pGrpDrawLine2, io_idxGroup,
        CObjFactory::c_strGroupNameStandardShapes, EGraphObjTypeLine, strGraphObjName,
        QPoint(500, 500), QPoint(500, 450), QPoint(510, 460));

    ZS::Test::CTestStepGroup* pGrpLine2ShowAndMoveLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Show and Move Labels",
        /* pTSGrpParent */ pGrpDrawLine2 );

    ZS::Test::CTestStepGroup* pGrpLine2ShowAndMoveTextLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Text Labels",
        /* pTSGrpParent */ pGrpLine2ShowAndMoveLabels );

    createTestGroupShowAndMoveTextLabel(
        pGrpLine2ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strLabelName,
        QPoint(495, 475), QPoint(460, 450));
    createTestGroupShowAndMoveTextLabel(
        pGrpLine2ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameP1,
        QPoint(495, 500), QPoint(495, 510));
    createTestGroupShowAndMoveTextLabel(
        pGrpLine2ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameP2,
        QPoint(495, 450), QPoint(495, 440));

    ZS::Test::CTestStepGroup* pGrpLine2ShowAndMoveGeometryLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Geometry Labels",
        /* pTSGrpParent */ pGrpLine2ShowAndMoveLabels );
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine2ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameLength,
        QPoint(505, 475), QPoint(525, 475));
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine2ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameAngle,
        QPoint(490, 475), QPoint(470, 475));

    ZS::Test::CTestStepGroup* pGrpDrawResizeLine2 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Resize " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLine2 );

    idxStep = 0;

    addTestStepDrawMouseEventsMove(
        pGrpDrawResizeLine2, idxStep, QPoint(490, 440), QPoint(500, 452));
    addTestStepsDrawMouseEventsMovePressMoveRelease(
        pGrpDrawResizeLine2, idxStep, QPoint(500, 452), QPoint(400, 400));
    addTestStepDrawMouseEventsMove(
        pGrpDrawResizeLine2, idxStep, QPoint(400, 400), QPoint(390, 390));

    ZS::Test::CTestStepGroup* pGrpDrawMoveLine2 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Move " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLine2 );

    idxStep = 0;

    addTestStepDrawMouseEventsMove(
        pGrpDrawMoveLine2, idxStep, QPoint(440, 440), QPoint(450, 450));
    addTestStepsDrawMouseEventsMovePressMoveRelease(
        pGrpDrawMoveLine2, idxStep, QPoint(450, 450), QPoint(400, 400));
    addTestStepDrawMouseEventsMove(
        pGrpDrawMoveLine2, idxStep, QPoint(400, 400), QPoint(390, 400)); // move sidewards away from line

    // Line3
    //------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpDrawLine3 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Create, Resize, Move " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLines );

    createTestGroupDrawMouseEventsCreateObject(
        pGrpDrawLine3, io_idxGroup,
        CObjFactory::c_strGroupNameStandardShapes, EGraphObjTypeLine, strGraphObjName,
        QPoint(100, 500), QPoint(100, 450), QPoint(110, 460));

    ZS::Test::CTestStepGroup* pGrpLine3ShowAndMoveLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Show and Move Labels",
        /* pTSGrpParent */ pGrpDrawLine3 );

    ZS::Test::CTestStepGroup* pGrpLine3ShowAndMoveTextLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Text Labels",
        /* pTSGrpParent */ pGrpLine3ShowAndMoveLabels );

    createTestGroupShowAndMoveTextLabel(
        pGrpLine3ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strLabelName,
        QPoint(95, 475), QPoint(60, 450));
    createTestGroupShowAndMoveTextLabel(
        pGrpLine3ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameP1,
        QPoint(95, 500), QPoint(95, 510));
    createTestGroupShowAndMoveTextLabel(
        pGrpLine3ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameP2,
        QPoint(95, 450), QPoint(95, 440));

    ZS::Test::CTestStepGroup* pGrpLine3ShowAndMoveGeometryLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Geometry Labels",
        /* pTSGrpParent */ pGrpLine3ShowAndMoveLabels );
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine3ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameLength,
        QPoint(105, 475), QPoint(125, 475));
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine3ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObj::c_strGeometryLabelNameAngle,
        QPoint(92, 475), QPoint(80, 475));

    ZS::Test::CTestStepGroup* pGrpDrawResizeLine3 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Resize " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLine3 );

    idxStep = 0;

    addTestStepDrawMouseEventsMove(
        pGrpDrawResizeLine3, idxStep, QPoint(90, 440), QPoint(100, 452));
    addTestStepsDrawMouseEventsMovePressMoveRelease(
        pGrpDrawResizeLine3, idxStep, QPoint(100, 452), QPoint(200, 400));
    addTestStepDrawMouseEventsMove(
        pGrpDrawResizeLine3, idxStep, QPoint(200, 400), QPoint(210, 390));

    ZS::Test::CTestStepGroup* pGrpDrawMoveLine3 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Move " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawLine3 );

    idxStep = 0;

    addTestStepDrawMouseEventsMove(
        pGrpDrawMoveLine3, idxStep, QPoint(140, 440), QPoint(150, 450));
    addTestStepsDrawMouseEventsMovePressMoveRelease(
        pGrpDrawMoveLine3, idxStep, QPoint(150, 450), QPoint(200, 400));
    addTestStepDrawMouseEventsMove(
        pGrpDrawMoveLine3, idxStep, QPoint(200, 400), QPoint(210, 400)); // move sidewards away from line

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpDrawLines->path());
    }
    return pGrpDrawLines;

} // createTestGroupDrawStandardShapesLines

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupDrawStandardShapesGroups(
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
        /* strMethod    */ "createTestGroupDrawStandardShapesGroups",
        /* strAddInfo   */ strMthInArgs );

    // Groups
    //-------

    ZS::Test::CTestStepGroup* pGrpDrawGroups = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Groups",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Group0
    //-------

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);
    QString strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;
    QString strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpDrawGroup0 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Create, Resize, Move " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawGroups );

    createTestGroupDrawMouseEventsCreateGroup(
        pGrpDrawGroup0, io_idxGroup,
        strGraphObjName,
        QPoint(120, 120), QPoint(480, 480), 10, QPoint(490, 490));

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpDrawGroups->path());
    }
    return pGrpDrawGroups;

} // createTestGroupDrawStandardShapesGroups

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupDrawMouseEventsCreateObject(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    const QString& i_strFactoryGroupName, EGraphObjType i_graphObjType, const QString& i_strGraphObjName,
    const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease, const QPoint& i_ptMouseAfterRelease)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
                       ", FactoryGroup:" + i_strFactoryGroupName +
                       ", GraphObjType:" + graphObjType2Str(i_graphObjType) +
                       ", GraphObjName: " + i_strGraphObjName +
                       ", MousePress {" + qPoint2Str(i_ptMousePress) + "}" +
                       ", MouseRelease {" + qPoint2Str(i_ptMouseRelease) + "}";
                       ", MouseAfterRelease {" + qPoint2Str(i_ptMouseAfterRelease) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawMouseEventsCreateObject",
        /* strAddInfo   */ strMthInArgs );

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = nullptr;

    ZS::Test::CTestStepGroup* pGrpDrawMouseEventsCreateObject = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Create " + i_strGraphObjName,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetCurrentDrawingTool(" + i_strFactoryGroupName + ", " + graphObjType2Str(i_graphObjType) + ")",
        /* strOperation    */ "DrawingScene.setCurrentDrawingTool(" + i_strFactoryGroupName + ", " + graphObjType2Str(i_graphObjType) + ")",
        /* pGrpParent      */ pGrpDrawMouseEventsCreateObject,
        /* szDoTestStepFct */ SLOT(doTestStepSetCurrentDrawingTool(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", i_strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", graphObjType2Str(i_graphObjType));

    addTestStepsDrawMouseEventsMovePressMoveRelease(
        pGrpDrawMouseEventsCreateObject, idxStep, i_ptMousePress, i_ptMouseRelease);
    addTestStepDrawMouseEventsMove(
        pGrpDrawMouseEventsCreateObject, idxStep, i_ptMouseRelease, i_ptMouseAfterRelease);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " UnsetCurrentDrawingTool",
        /* strOperation    */ "DrawingScene.setCurrentDrawingTool(null)",
        /* pGrpParent      */ pGrpDrawMouseEventsCreateObject,
        /* szDoTestStepFct */ SLOT(doTestStepUnsetCurrentDrawingTool(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", "");
    pTestStep->setConfigValue("GraphObjType", "");

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpDrawMouseEventsCreateObject->path());
    }
    return pGrpDrawMouseEventsCreateObject;

} // createTestGroupDrawMouseEventsCreateObject

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupDrawMouseEventsCreateGroup(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    const QString& i_strGraphObjName,
    const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease, int i_iMovesCount,
    const QPoint& i_ptMouseAfterRelease)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
                       ", GraphObjName: " + i_strGraphObjName +
                       ", MousePress {" + qPoint2Str(i_ptMousePress) + "}" +
                       ", MouseRelease {" + qPoint2Str(i_ptMouseRelease) + "}" +
                       ", MovesCount: " + QString::number(i_iMovesCount) +
                       ", MouseAfterRelease {" + qPoint2Str(i_ptMouseAfterRelease) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupDrawMouseEventsCreateGroup",
        /* strAddInfo   */ strMthInArgs );

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = nullptr;

    ZS::Test::CTestStepGroup* pGrpDrawMouseEventsCreateObject = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Create " + i_strGraphObjName,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);

    // To create a group:
    // - Set the factory group name to nullptr (groups are created via "groupGraphObjsSelected" call)
    // - Select the mode with EEditTool::Select, EEditMode::None, EEditResizeMode::None
    // - After the mode has been set the objects to be grouped are selected by mouse move events.
    // - After the objects have been selected invoke method "groupGraphObjsSelected"

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " UnsetCurrentDrawingTool()",
        /* strOperation    */ "DrawingScene.setCurrentDrawingTool(null)",
        /* pGrpParent      */ pGrpDrawMouseEventsCreateObject,
        /* szDoTestStepFct */ SLOT(doTestStepUnsetCurrentDrawingTool(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", "");
    pTestStep->setConfigValue("GraphObjType", "");

    addTestStepDrawMouseEventPress(
        pGrpDrawMouseEventsCreateObject, idxStep, i_ptMousePress);
    addTestStepsDrawMouseEventsMove(
        pGrpDrawMouseEventsCreateObject, idxStep, i_ptMousePress, i_ptMouseRelease, i_iMovesCount);
    addTestStepDrawMouseEventRelease(
        pGrpDrawMouseEventsCreateObject, idxStep, i_ptMouseRelease);
    addTestStepDrawMouseEventsMove(
        pGrpDrawMouseEventsCreateObject, idxStep, i_ptMouseRelease, i_ptMouseAfterRelease, 0);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " GroupSelectedObjects",
        /* strOperation    */ "DrawingScene.groupGraphObjsSelected()",
        /* pGrpParent      */ pGrpDrawMouseEventsCreateObject,
        /* szDoTestStepFct */ SLOT(doTestStepSetCurrentDrawingTool(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpDrawMouseEventsCreateObject->path());
    }
    return pGrpDrawMouseEventsCreateObject;

} // createTestGroupDrawMouseEventsCreateGroup

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupShowAndMoveTextLabel(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    const QString& i_strGraphObjName, const QString& i_strLabelName,
    const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
                       ", ObjName: " + i_strGraphObjName + ", Label: " + i_strLabelName +
                       ", MousePress {" + qPoint2Str(i_ptMousePress) + "}" +
                       ", MouseRelease {" + qPoint2Str(i_ptMouseRelease) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createTestGroupShowAndMoveTextLabel",
        /* strAddInfo   */ strMthInArgs );

    int idxStep = 0;

    ZS::Test::CTestStepGroup* pGrpShowAndMoveLabel = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + i_strGraphObjName + " Text Label " + i_strLabelName,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + i_strGraphObjName + ".showLabel(" + i_strLabelName + ")",
        /* strOperation    */ "GraphObj.showLabel(" + i_strGraphObjName + ", " + i_strLabelName + ")",
        /* pGrpParent      */ pGrpShowAndMoveLabel,
        /* szDoTestStepFct */ SLOT(doTestStepShowLabel(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", i_strGraphObjName);
    pTestStep->setConfigValue("LabelName", i_strLabelName);

    addTestStepsDrawMouseEventsMovePressMoveRelease(
        pGrpShowAndMoveLabel, idxStep, i_ptMousePress, i_ptMouseRelease);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpShowAndMoveLabel->path());
    }
    return pGrpShowAndMoveLabel;

} // createTestGroupShowAndMoveTextLabel

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupShowAndMoveGeometryLabel(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    const QString& i_strGraphObjName, const QString& i_strLabelName,
    const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", IdxGroup:" + QString::number(io_idxGroup) +
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

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = nullptr;

    ZS::Test::CTestStepGroup* pGrpShowAndMoveLabel = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + i_strGraphObjName + " Geometry Label " + i_strLabelName,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + i_strGraphObjName + ".showGeometryLabel(" + i_strLabelName + ")",
        /* strOperation    */ "GraphObj.showGeometryLabel(" + i_strGraphObjName + ", " + i_strLabelName + ")",
        /* pGrpParent      */ pGrpShowAndMoveLabel,
        /* szDoTestStepFct */ SLOT(doTestStepShowGeometryLabel(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", i_strGraphObjName);
    pTestStep->setConfigValue("LabelName", i_strLabelName);

    addTestStepsDrawMouseEventsMovePressMoveRelease(
        pGrpShowAndMoveLabel, idxStep, i_ptMousePress, i_ptMouseRelease);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpShowAndMoveLabel->path());
    }
    return pGrpShowAndMoveLabel;

} // createTestGroupShowAndMoveGeometryLabel

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::addTestStepDrawMouseEventPress(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxStep,
    const QPoint& i_ptMousePos)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", Step: " + QString::number(io_idxStep) +
                       ", MousePos {" + qPoint2Str(i_ptMousePos) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addTestStepDrawMouseEventPress",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStep* pTestStep = nullptr;

    // First move the cursor on the object so that the object is hit.
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++io_idxStep) + " MousePressEvent(" + qPoint2Str(i_ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mousePressEvent(" + qPoint2Str(i_ptMousePos) + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", i_ptMousePos);
}

//------------------------------------------------------------------------------
void CTest::addTestStepDrawMouseEventRelease(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxStep,
    const QPoint& i_ptMousePos)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", Step: " + QString::number(io_idxStep) +
                       ", MousePos {" + qPoint2Str(i_ptMousePos) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addTestStepDrawMouseEventRelease",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStep* pTestStep = nullptr;

    // First move the cursor on the object so that the object is hit.
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++io_idxStep) + " MouseReleaseEvent(" + qPoint2Str(i_ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" + qPoint2Str(i_ptMousePos) + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", i_ptMousePos);
}

//------------------------------------------------------------------------------
void CTest::addTestStepDrawMouseEventsMove(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxStep,
    const QPoint& i_ptMouseStart, const QPoint& i_ptMouseStop, int i_iMovesCount)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", Step: " + QString::number(io_idxStep) +
                       ", MouseStart {" + qPoint2Str(i_ptMouseStart) + "}" +
                       ", MouseStop {" + qPoint2Str(i_ptMouseStop) + "}" +
                       ", MovesCount: " + QString::number(i_iMovesCount) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addTestStepDrawMouseEventsMove",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStep* pTestStep = nullptr;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++io_idxStep) + " MouseMoveEvents(" + qPoint2Str(i_ptMouseStart) + ", " + qPoint2Str(i_ptMouseStop) + ")",
        /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(i_ptMouseStart) + ", " + qPoint2Str(i_ptMouseStop) + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
    pTestStep->setConfigValue("MousePosStart", i_ptMouseStart);
    pTestStep->setConfigValue("MousePosStop", i_ptMouseStop);
    pTestStep->setConfigValue("MovesCount", i_iMovesCount);
}

//------------------------------------------------------------------------------
void CTest::addTestStepsDrawMouseEventsMove(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxStep,
    const QPoint& i_ptMouseStart, const QPoint& i_ptMouseStop, int i_iMovesCount)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", Step: " + QString::number(io_idxStep) +
                       ", MouseStart {" + qPoint2Str(i_ptMouseStart) + "}" +
                       ", MouseStop {" + qPoint2Str(i_ptMouseStop) + "}" +
                       ", MovesCount: " + QString::number(i_iMovesCount) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addTestStepsDrawMouseEventsMove",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStep* pTestStep = nullptr;

    int iMovesCount = i_iMovesCount;
    bool bMoveRight = i_ptMouseStop.x() > i_ptMouseStart.x();
    bool bMoveLeft = i_ptMouseStop.x() < i_ptMouseStart.x();
    bool bMoveDown = i_ptMouseStop.y() > i_ptMouseStart.y();
    bool bMoveUp = i_ptMouseStop.y() < i_ptMouseStart.y();

    int xDist_px = abs(i_ptMouseStop.x() - i_ptMouseStart.x());
    int yDist_px = abs(i_ptMouseStop.y() - i_ptMouseStart.y());
    if (iMovesCount == 0) {
        iMovesCount = xDist_px;
        if (xDist_px < yDist_px) {
            iMovesCount = yDist_px;
        }
    }

    double fXStep_px = (static_cast<double>(xDist_px) / static_cast<double>(iMovesCount));
    double fYStep_px = (static_cast<double>(yDist_px) / static_cast<double>(iMovesCount));

    QPoint ptMouseMovePos = i_ptMouseStart;
    for (int iMoves = 0; iMoves < iMovesCount; ++iMoves) {
        ptMouseMovePos.setX(ptMouseMovePos.x() + Math::round2Nearest(fXStep_px));
        ptMouseMovePos.setY(ptMouseMovePos.y() + Math::round2Nearest(fYStep_px));
        if (bMoveRight && ptMouseMovePos.x() > (i_ptMouseStop.x()-2)) {
            ptMouseMovePos.setX(i_ptMouseStop.x());
        }
        else if (bMoveLeft && ptMouseMovePos.x() < (i_ptMouseStop.x()+2)) {
            ptMouseMovePos.setX(i_ptMouseStop.x());
        }
        if (bMoveDown && ptMouseMovePos.y() > (i_ptMouseStop.y()-2)) {
            ptMouseMovePos.setY(i_ptMouseStop.y());
        }
        else if (bMoveUp && ptMouseMovePos.y() < (i_ptMouseStop.y()+2)) {
            ptMouseMovePos.setY(i_ptMouseStop.y());
        }
        pTestStep = new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++io_idxStep) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
            /* pGrpParent      */ i_pTestStepGroupParent,
            /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
        pTestStep->setConfigValue("MousePos", ptMouseMovePos);
        if (bMoveRight && ptMouseMovePos.x() >= i_ptMouseStop.x()) {
            break;
        }
        else if (bMoveLeft && ptMouseMovePos.x() <= i_ptMouseStop.x()) {
            break;
        }
        if (bMoveDown && ptMouseMovePos.y() >= i_ptMouseStop.y()) {
            break;
        }
        else if (bMoveUp && ptMouseMovePos.y() <= i_ptMouseStop.y()) {
            break;
        }
    }

} // addTestStepsDrawMouseEventsMove

//------------------------------------------------------------------------------
void CTest::addTestStepsDrawMouseEventsMovePressMoveRelease(
    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxStep,
    const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent: " + QString(i_pTestStepGroupParent == nullptr ? "nullptr" : i_pTestStepGroupParent->path()) +
                       ", Step: " + QString::number(io_idxStep) +
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
        /* strName         */ "Step " + QString::number(++io_idxStep) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* strOperation    */ "DrawingView.mouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
    pTestStep->setConfigValue("MousePos", ptMouseMovePos);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++io_idxStep) + " MousePressEvent(" + qPoint2Str(ptMouseMovePos) + ")",
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
            /* strName         */ "Step " + QString::number(++io_idxStep) + " MouseMoveEvent(" + qPoint2Str(ptMouseMovePos) + ")",
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
        /* strName         */ "Step " + QString::number(++io_idxStep) + " MouseReleaseEvent(" + qPoint2Str(i_ptMouseRelease) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" + qPoint2Str(i_ptMouseRelease) + ")",
        /* pGrpParent      */ i_pTestStepGroupParent,
        /* szDoTestStepFct */ SLOT(doTestStepMouseReleaseEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", i_ptMouseRelease);

} // addTestStepsDrawMouseEventsMovePressMoveRelease

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

    CGraphObjPoint::s_iInstCount = 0;
    CGraphObjLine::s_iInstCount = 0;
    CGraphObjRect::s_iInstCount = 0;
    CGraphObjEllipse::s_iInstCount = 0;
    CGraphObjPolygon::s_iInstCount = 0;
    CGraphObjPolyline::s_iInstCount = 0;
    CGraphObjText::s_iInstCount = 0;
    CGraphObjImage::s_iInstCount = 0;
    CGraphObjConnectionPoint::s_iInstCount = 0;
    CGraphObjConnectionLine::s_iInstCount = 0;
    CGraphObjGroup::s_iInstCount = 0;

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
        CUnit unit = drawingSize.metricUnit();
        if (i_pTestStep->hasConfigValue(CDrawingScene::c_strXmlAttrUnit)) {
            unit = i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrUnit).toString();
            drawingSize.setMetricUnit(unit);
        }
        if (i_pTestStep->hasConfigValue(CDrawingScene::c_strXmlAttrMetricImageCoorsDecimals)) {
            int iDecimals = i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrMetricImageCoorsDecimals).toInt();
            drawingSize.setMetricImageCoorsDecimals(iDecimals);
        }
        double fxWidth = i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrWidth).toDouble();
        double fyHeight = i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrHeight).toDouble();
        drawingSize.setImageSize(CPhysVal(fxWidth, unit), CPhysVal(fyHeight, unit));
        bool bOk = false;
        if (i_pTestStep->hasConfigValue(CDrawingScene::c_strXmlAttrYScaleAxisOrientation)) {
            CEnumYScaleAxisOrientation eYScaleOrientation = CEnumYScaleAxisOrientation::fromString(
                i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrYScaleAxisOrientation).toString(), &bOk);
            if (bOk) {
                drawingSize.setYScaleAxisOrientation(eYScaleOrientation);
            }
        }
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
    gridSettings.setLinesDistMin(
        i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrGridLinesDistMin).toInt());
    gridSettings.setLabelsVisible(
        i_pTestStep->getConfigValue(CDrawingScene::c_strXmlAttrGridLabelsVisible).toBool());

    m_pDrawingScene->setGridSettings(gridSettings);

    i_pTestStep->setResultValue("");
}

//------------------------------------------------------------------------------
void CTest::doTestStepDrawingSceneConvertToPhysValPoint( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepDrawingSceneConvertToPhysValPoint",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unitPoint = Units.Length.px;
    CUnit unitDest = drawingSize.unit();
    if (i_pTestStep->hasConfigValue("Point.Unit")) {
        QString strUnitPoint = i_pTestStep->getConfigValue("Point.Unit").toString();
        unitPoint = strUnitPoint;
    }
    if (i_pTestStep->hasConfigValue("UnitDest")) {
        QString strUnitDest = i_pTestStep->getConfigValue("UnitDest").toString();
        unitDest = strUnitDest;
    }
    double fResolution = drawingSize.imageCoorsResolutionInPx();
    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
        fResolution = drawingSize.imageCoorsResolution(unitDest).getVal();
    }
    if (i_pTestStep->hasConfigValue("Point.Resolution")) {
        fResolution = drawingSize.imageCoorsResolution(unitDest).getVal();
    }
    QPointF pt = i_pTestStep->getConfigValue("Point").toPointF();
    CPhysValPoint physValPointResult;
    if (Units.Length.isMetricUnit(unitPoint)) {
        CPhysValPoint physValPoint(pt, fResolution, unitPoint);
        physValPointResult = m_pDrawingScene->convert(physValPoint, unitDest);
    }
    else {
        physValPointResult = m_pDrawingScene->convert(pt, unitDest);
    }
    QString strResultValue = "{" + physValPointResult.toString() + "} " + physValPointResult.unit().symbol();
    i_pTestStep->setResultValue(strResultValue);
}

//------------------------------------------------------------------------------
void CTest::doTestStepDrawingSceneConvertToPhysValSize( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepDrawingSceneConvertToPhysValSize",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unitPoint = Units.Length.px;
    CUnit unitDest = drawingSize.unit();
    if (i_pTestStep->hasConfigValue("Size.Unit")) {
        QString strUnitPoint = i_pTestStep->getConfigValue("Size.Unit").toString();
        unitPoint = strUnitPoint;
    }
    if (i_pTestStep->hasConfigValue("UnitDest")) {
        QString strUnitDest = i_pTestStep->getConfigValue("UnitDest").toString();
        unitDest = strUnitDest;
    }
    double fResolution = drawingSize.imageCoorsResolutionInPx();
    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
        fResolution = drawingSize.imageCoorsResolution(unitDest).getVal();
    }
    if (i_pTestStep->hasConfigValue("Size.Resolution")) {
        fResolution = drawingSize.imageCoorsResolution(unitDest).getVal();
    }
    QSizeF sizeF = i_pTestStep->getConfigValue("Size").toSizeF();
    CPhysValSize physValSizeResult;
    if (Units.Length.isMetricUnit(unitPoint)) {
        CPhysValSize physValSize(sizeF, fResolution, unitPoint);
        physValSizeResult = m_pDrawingScene->convert(physValSize, unitDest);
    }
    else {
        physValSizeResult = m_pDrawingScene->convert(sizeF, unitDest);
    }
    QString strResultValue = "{" + physValSizeResult.toString() + "} " + physValSizeResult.unit().symbol();
    i_pTestStep->setResultValue(strResultValue);
}

//------------------------------------------------------------------------------
void CTest::doTestStepDrawingSceneConvertToPhysValLine( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepDrawingSceneConvertToPhysValLine",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unitPoint = Units.Length.px;
    CUnit unitDest = drawingSize.unit();
    if (i_pTestStep->hasConfigValue("Line.Unit")) {
        QString strUnitPoint = i_pTestStep->getConfigValue("Line.Unit").toString();
        unitPoint = strUnitPoint;
    }
    if (i_pTestStep->hasConfigValue("UnitDest")) {
        QString strUnitDest = i_pTestStep->getConfigValue("UnitDest").toString();
        unitDest = strUnitDest;
    }
    double fResolution = drawingSize.imageCoorsResolutionInPx();
    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
        fResolution = drawingSize.imageCoorsResolution(unitDest).getVal();
    }
    if (i_pTestStep->hasConfigValue("Line.Resolution")) {
        fResolution = drawingSize.imageCoorsResolution(unitDest).getVal();
    }
    QLineF lineF = i_pTestStep->getConfigValue("Line").toLineF();
    CPhysValLine physValLineResult;
    if (Units.Length.isMetricUnit(unitPoint)) {
        CPhysValLine physValLine(lineF, fResolution, unitPoint);
        physValLineResult = m_pDrawingScene->convert(physValLine, unitDest);
    }
    else {
        physValLineResult = m_pDrawingScene->convert(lineF, unitDest);
    }
    QString strResultValue = "{" + physValLineResult.toString() + "} " + physValLineResult.unit().symbol();
    i_pTestStep->setResultValue(strResultValue);
}

//------------------------------------------------------------------------------
void CTest::doTestStepDrawingSceneConvertToPhysValRect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepDrawingSceneConvertToPhysValRect",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unitPoint = Units.Length.px;
    CUnit unitDest = drawingSize.unit();
    if (i_pTestStep->hasConfigValue("Rect.Unit")) {
        QString strUnitPoint = i_pTestStep->getConfigValue("Rect.Unit").toString();
        unitPoint = strUnitPoint;
    }
    if (i_pTestStep->hasConfigValue("UnitDest")) {
        QString strUnitDest = i_pTestStep->getConfigValue("UnitDest").toString();
        unitDest = strUnitDest;
    }
    double fResolution = drawingSize.imageCoorsResolutionInPx();
    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
        fResolution = drawingSize.imageCoorsResolution(unitDest).getVal();
    }
    if (i_pTestStep->hasConfigValue("Rect.Resolution")) {
        fResolution = drawingSize.imageCoorsResolution(unitDest).getVal();
    }
    QRectF rectF = i_pTestStep->getConfigValue("Rect").toRectF();
    CPhysValRect physValRectResult;
    if (Units.Length.isMetricUnit(unitPoint)) {
        CPhysValRect physValRect(rectF, fResolution, unitPoint);
        physValRectResult = m_pDrawingScene->convert(physValRect, unitDest);
    }
    else {
        physValRectResult = m_pDrawingScene->convert(rectF, unitDest);
    }
    QString strResultValue = "{" + physValRectResult.toString() + "} " + physValRectResult.unit().symbol();
    i_pTestStep->setResultValue(strResultValue);
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
void CTest::doTestStepSetMode( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepSetMode",
        /* strAddInfo   */ strMthInArgs );

    QString strMode = i_pTestStep->getConfigValue("Mode").toString();
    CEnumMode eMode = strMode;

    i_pTestStep->setExpectedValue("");

    m_pDrawingScene->setMode(eMode);

    i_pTestStep->setResultValue("");
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

    QStringList strlstExpectedValues;
    QString strFactoryGroupName = i_pTestStep->getConfigValue("FactoryGroupName").toString();
    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();

    if (!strFactoryGroupName.isEmpty() && !strGraphObjType.isEmpty()) {
        if (strFactoryGroupName == CObjFactory::c_strGroupNameStandardShapes && strGraphObjType == graphObjType2Str(EGraphObjTypeGroup)) {
            m_pDrawingScene->groupGraphObjsSelected();
        }
        else {
            m_pDrawingScene->setCurrentDrawingTool(strFactoryGroupName, strGraphObjType);
            strlstExpectedValues.append(strFactoryGroupName + "::" + strGraphObjType);
        }
    }
    else {
        m_pDrawingScene->setCurrentDrawingTool(nullptr);
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    QStringList strlstResultValues;

    if (!strFactoryGroupName.isEmpty() && !strGraphObjType.isEmpty()) {
        if (strFactoryGroupName == CObjFactory::c_strGroupNameStandardShapes && strGraphObjType == graphObjType2Str(EGraphObjTypeGroup)) {
        }
        else {
            CObjFactory* pObjFactory = m_pDrawingScene->getCurrentDrawingTool();
            if (pObjFactory != nullptr) {
                strFactoryGroupName = pObjFactory->getGroupName();
                strGraphObjType = pObjFactory->getGraphObjTypeAsString();
                strlstResultValues.append(strFactoryGroupName + "::" + strGraphObjType);
            }
        }
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
void CTest::doTestStepAddGraphObj( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepAddGraphObj",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = i_pTestStep->getConfigValue("FactoryGroupName").toString();
    QString strGraphObjType = i_pTestStep->getConfigValue("GraphObjType").toString();
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();

    EGraphObjType graphObjType = str2GraphObjType(strGraphObjType);

    CPhysValPoint physValPoint1(*m_pDrawingScene);
    physValPoint1 = i_pTestStep->getConfigValue("Point1").toString();
    CPhysValPoint physValPoint2(*m_pDrawingScene);
    physValPoint2 = i_pTestStep->getConfigValue("Point2").toString();

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);

    if (pObjFactory != nullptr) {
        CDrawSettings drawSettings(graphObjType);
        CGraphObj* pGraphObj = pObjFactory->createGraphObj(m_pDrawingScene, physValPoint1, drawSettings);
        m_pDrawingScene->addGraphObj(pGraphObj);
        if (graphObjType == EGraphObjTypeLine) {
            CGraphObjLine* pGraphObjLine = dynamic_cast<CGraphObjLine*>(pGraphObj);
            if (pGraphObjLine != nullptr) {
                pGraphObjLine->setP2(physValPoint2);
            }
        }
    }

    QStringList strlstExpectedValues = i_pTestStep->getExpectedValues();
    QStringList strlstResultValues;
    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
    if (graphObjType == EGraphObjTypeLine) {
        CGraphObjLine* pGraphObjLine = dynamic_cast<CGraphObjLine*>(pGraphObj);
        if (pGraphObjLine != nullptr) {
            QLineF lineF = pGraphObjLine->line();
            strlstResultValues.append("line.p1 {" + qPoint2Str(lineF.p1()) + "} px");
            strlstResultValues.append("line.p2 {" + qPoint2Str(lineF.p2()) + "} px");
            strlstResultValues.append("line.length: " + QString::number(lineF.length(), 'f', 1) + " px");
            strlstResultValues.append("line.pos {" + qPoint2Str(pGraphObjLine->pos()) + "} px");
            CPhysValLine physValLine = pGraphObjLine->getLine();
            strlstResultValues.append("getLine.p1.x: " + physValLine.p1().x().toString());
            strlstResultValues.append("getLine.p1.y: " + physValLine.p1().y().toString());
            strlstResultValues.append("getLine.p2.x: " + physValLine.p2().x().toString());
            strlstResultValues.append("getLine.p2.y: " + physValLine.p2().y().toString());
            CPhysVal physValLength = pGraphObjLine->getLength();
            strlstResultValues.append("getLength: " + physValLength.toString());
            CPhysVal physValAngle = pGraphObjLine->getAngle(Units.Angle.Degree);
            strlstResultValues.append("getAngle: " + physValAngle.toString());
            CPhysValPoint physValPointPos = pGraphObjLine->getPos();
            strlstResultValues.append("getPos {" + physValPointPos.toString() + "} " + physValPointPos.unit().symbol());
        }
    }
    i_pTestStep->setResultValues(strlstResultValues);
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
        //pGraphObj->showLabel(strLabelName);
        //pGraphObj->showLabelAnchorLine(strLabelName);
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
        //pGraphObj->showGeometryLabel(strLabelName);
        //pGraphObj->showGeometryLabelAnchorLine(strLabelName);
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
    Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
    if (i_pTestStep->hasConfigValue("KeyboardModifiers")) {
        keyboardModifiers = static_cast<Qt::KeyboardModifiers>(i_pTestStep->getConfigValue("KeyboardModifiers").toInt());
    }
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
        /* modifiers */ keyboardModifiers );
    m_pDrawingView->mousePressEvent(pMouseEv);
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
    Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
    if (i_pTestStep->hasConfigValue("KeyboardModifiers")) {
        keyboardModifiers = static_cast<Qt::KeyboardModifiers>(i_pTestStep->getConfigValue("KeyboardModifiers").toInt());
    }
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
        /* modifiers */ keyboardModifiers );
    m_pDrawingView->mouseReleaseEvent(pMouseEv);
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
void CTest::doTestStepMouseMoveEvents( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepMouseMoveEvents",
        /* strAddInfo   */ strMthInArgs );

    int iMovesCount = 0;
    Qt::MouseButton mouseBtn = Qt::LeftButton;
    if (i_pTestStep->hasConfigValue("MouseButtons")) {
        mouseBtn = static_cast<Qt::MouseButton>(i_pTestStep->getConfigValue("MouseButtons").toInt());
    }
    QPoint ptMousePosStart = i_pTestStep->getConfigValue("MousePosStart").toPoint();
    QPoint ptMousePosStop = i_pTestStep->getConfigValue("MousePosStop").toPoint();
    if (i_pTestStep->hasConfigValue("MovesCount")) {
        iMovesCount = i_pTestStep->getConfigValue("MovesCount").toInt();
    }

    bool bMoveRight = ptMousePosStop.x() > ptMousePosStart.x();
    bool bMoveLeft = ptMousePosStop.x() < ptMousePosStart.x();
    bool bMoveDown = ptMousePosStop.y() > ptMousePosStart.y();
    bool bMoveUp = ptMousePosStop.y() < ptMousePosStart.y();

    int xDist_px = abs(ptMousePosStop.x() - ptMousePosStart.x());
    int yDist_px = abs(ptMousePosStop.y() - ptMousePosStart.y());
    if (iMovesCount == 0) {
        iMovesCount = xDist_px;
        if (xDist_px < yDist_px) {
            iMovesCount = yDist_px;
        }
    }

    double fXStep_px = (static_cast<double>(xDist_px) / static_cast<double>(iMovesCount));
    double fYStep_px = (static_cast<double>(yDist_px) / static_cast<double>(iMovesCount));

    QPoint ptMouseMovePos = ptMousePosStart;
    for (int iMoves = 0; iMoves < iMovesCount; ++iMoves) {
        ptMouseMovePos.setX(ptMouseMovePos.x() + Math::round2Nearest(fXStep_px));
        ptMouseMovePos.setY(ptMouseMovePos.y() + Math::round2Nearest(fYStep_px));
        if (bMoveRight && ptMouseMovePos.x() > ptMousePosStop.x()) {
            ptMouseMovePos.setX(ptMousePosStop.x());
        }
        else if (bMoveLeft && ptMouseMovePos.x() < ptMousePosStop.x()) {
            ptMouseMovePos.setX(ptMousePosStop.x());
        }
        if (bMoveDown && ptMouseMovePos.y() > ptMousePosStop.y()) {
            ptMouseMovePos.setY(ptMousePosStop.y());
        }
        else if (bMoveUp && ptMouseMovePos.y() < ptMousePosStop.y()) {
            ptMouseMovePos.setY(ptMousePosStop.y());
        }

        QPoint ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptMouseMovePos);
        QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptDrawingViewMousePos);

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
        if (bMoveRight && ptMouseMovePos.x() >= ptMousePosStop.x()) {
            break;
        }
        else if (bMoveLeft && ptMouseMovePos.x() <= ptMousePosStop.x()) {
            break;
        }
        if (bMoveDown && ptMouseMovePos.y() >= ptMousePosStop.y()) {
            break;
        }
        else if (bMoveUp && ptMouseMovePos.y() <= ptMousePosStop.y()) {
            break;
        }
    }

    QStringList strlstExpectedValues;
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    QStringList strlstResultValues;
    i_pTestStep->setResultValues(strlstResultValues);
}
