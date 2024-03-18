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

#include "ZSDraw/Common/ZSDrawAux.h"
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
#include <QtCore/qfile.h>
#include <QtCore/qstandardpaths.h>
#include <QtCore/qtextstream.h>
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

    createTestGroupDrawingSize(nullptr, idxGroup);
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
ZS::Test::CTestStepGroup* CTest::createTestGroupDrawingSize(
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
        /* strMethod    */ "createTestGroupDrawingSize",
        /* strAddInfo   */ strMthInArgs );

    int idxStep = 0;

    ZS::Test::CTestStepGroup* pGrpDrawingSize = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Drawing Size",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " DrawingSize",
        /* strOperation    */ "DrawingSize",
        /* pGrpParent      */ pGrpDrawingSize,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("setScreenResolutionInPxPerMM", 3.5);

    QStringList strlstExpectedValues;
    strlstExpectedValues.append("Test, IsValid: false, DimensionUnit: Pixels, YScale: TopDown, "
                                "ScreenResolution: 3.5 px/mm, Size/mm {0.00, 0.00}, Size/px {0, 0}, "
                                "Scale: 1:1, PaperSize: ?, Orientation: ?");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " DrawingSize",
        /* strOperation    */ "DrawingSize",
        /* pGrpParent      */ pGrpDrawingSize,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("1: setScreenResolutionInPxPerMM", 3.5);
    pTestStep->setConfigValue("2: setDimensionUnit", CEnumScaleDimensionUnit(EScaleDimensionUnit::Pixels).toString());
    pTestStep->setConfigValue("3: setImageSize", "700, 350, 1.0, px");

    strlstExpectedValues.clear();
    strlstExpectedValues.append("Test, IsValid: true, DimensionUnit: Pixels, YScale: TopDown, "
                                "ScreenResolution: 3.5 px/mm, Size/mm {200.00, 100.00}, Size/px {700, 350}, "
                                "Scale: 1:1, PaperSize: ?, Orientation: ?");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " DrawingSize",
        /* strOperation    */ "DrawingSize",
        /* pGrpParent      */ pGrpDrawingSize,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("1: setScreenResolutionInPxPerMM", 3.5);
    pTestStep->setConfigValue("2: setDimensionUnit", CEnumScaleDimensionUnit(EScaleDimensionUnit::Metric).toString());
    pTestStep->setConfigValue("3: setMetricImageCoorsDecimals", 2);
    pTestStep->setConfigValue("4: setMetricUnit", "mm");
    pTestStep->setConfigValue("5: setScaleFactor", "1:1");
    pTestStep->setConfigValue("6: setYScaleAxisOrientation", CEnumYScaleAxisOrientation(EYScaleAxisOrientation::TopDown).toString());
    pTestStep->setConfigValue("7: setImageSize", "100, 100, 1.0, mm");

    strlstExpectedValues.clear();
    strlstExpectedValues.append("Test, IsValid: true, DimensionUnit: Metric, YScale: TopDown, "
                                "ScreenResolution: 3.5 px/mm, Size/mm {100.00, 100.00}, Size/px {351, 351}, "
                                "Scale: 1:1, PaperSize: ?, Orientation: ?");
    pTestStep->setExpectedValues(strlstExpectedValues);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpDrawingSize->path());
    }
    return pGrpDrawingSize;

} // createTestGroupDrawingSize

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
        XmlStreamParser::c_strXmlAttrDimensionUnit, i_drawingSize.dimensionUnit().toString());
    pTestStep->setConfigValue(
        XmlStreamParser::c_strXmlAttrScreenResolutionPxPerMilliMeter, i_drawingSize.screenResolutionInPxPerMM());
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        pTestStep->setConfigValue(
            XmlStreamParser::c_strXmlAttrWidth, i_drawingSize.imageWidthInPixels());
        pTestStep->setConfigValue(
            XmlStreamParser::c_strXmlAttrHeight, i_drawingSize.imageHeightInPixels());
    }
    else {
        pTestStep->setConfigValue(
            XmlStreamParser::c_strXmlAttrUnit, i_drawingSize.metricUnit().symbol());
        pTestStep->setConfigValue(
            XmlStreamParser::c_strXmlAttrMetricImageCoorsDecimals, i_drawingSize.metricImageCoorsDecimals());
        pTestStep->setConfigValue(
            XmlStreamParser::c_strXmlAttrWidth, i_drawingSize.metricImageWidth().getVal());
        pTestStep->setConfigValue(
            XmlStreamParser::c_strXmlAttrHeight, i_drawingSize.metricImageHeight().getVal());
        pTestStep->setConfigValue(
            XmlStreamParser::c_strXmlAttrYScaleAxisOrientation, i_drawingSize.yScaleAxisOrientation().toString());
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SetGridSettings",
        /* strOperation    */ "DrawingScene.setGridSettings",
        /* pGrpParent      */ pGrpPrepareScene,
        /* szDoTestStepFct */ SLOT(doTestStepSetGridSettings(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue(
        XmlStreamParser::c_strXmlAttrLinesVisible, i_gridSettings.areLinesVisible());
    pTestStep->setConfigValue(
        XmlStreamParser::c_strXmlAttrLinesDistMin, i_gridSettings.linesDistMin());
    pTestStep->setConfigValue(
        XmlStreamParser::c_strXmlAttrLabelsVisible, i_gridSettings.areLabelsVisible());

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
                      i_drawingSize.metricUnit().symbol() + "-" + i_drawingSize.yScaleAxisOrientation().toString() + ".xml";
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

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;

    int idxStep = 0;

    // Line0 (top)
    //------------

    QString strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("P1", QPointF(0.0, 0.0));
    pTestStep->setConfigValue("P2", QPointF(100.0, 0.0));

    QStringList strlstExpectedValues;
    strlstExpectedValues.append("line.p1 {-50.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {50.0, 0.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    strlstExpectedValues.append("line.pos {50.0, 0.0} px");
    strlstExpectedValues.append("getLine.p1.x: 0 px");
    strlstExpectedValues.append("getLine.p1.y: 0 px");
    strlstExpectedValues.append("getLine.p2.x: 100 px");
    strlstExpectedValues.append("getLine.p2.y: 0 px");
    strlstExpectedValues.append("getLength: 100 px");
    strlstExpectedValues.append("getAngle: 0.0 " + Math::c_strSymbolDegree);
    strlstExpectedValues.append("getPos {50, 0} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line1 (right)
    //--------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("P1", QPointF(100.0, 0.0));
    pTestStep->setConfigValue("P2", QPointF(100.0, 100.0));

    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {0.0, -50.0} px");
    strlstExpectedValues.append("line.p2 {0.0, 50.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    strlstExpectedValues.append("line.pos {100.0, 50.0} px");
    strlstExpectedValues.append("getLine.p1.x: 100 px");
    strlstExpectedValues.append("getLine.p1.y: 0 px");
    strlstExpectedValues.append("getLine.p2.x: 100 px");
    strlstExpectedValues.append("getLine.p2.y: 100 px");
    strlstExpectedValues.append("getLength: 100 px");
    strlstExpectedValues.append("getAngle: 270.0 " + Math::c_strSymbolDegree);
    strlstExpectedValues.append("getPos {100, 50} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line2 (bottom)
    //---------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("P1", QPointF(100.0, 100.0));
    pTestStep->setConfigValue("P2", QPointF(0.0, 100.0));

    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {50.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {-50.0, 0.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    strlstExpectedValues.append("line.pos {50.0, 100.0} px");
    strlstExpectedValues.append("getLine.p1.x: 100 px");
    strlstExpectedValues.append("getLine.p1.y: 100 px");
    strlstExpectedValues.append("getLine.p2.x: 0 px");
    strlstExpectedValues.append("getLine.p2.y: 100 px");
    strlstExpectedValues.append("getLength: 100 px");
    strlstExpectedValues.append("getAngle: 180.0 " + Math::c_strSymbolDegree);
    strlstExpectedValues.append("getPos {50, 100} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line3 (left)
    //-------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("P1", QPointF(0.0, 100.0));
    pTestStep->setConfigValue("P2", QPointF(0.0, 0.0));

    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {0.0, 50.0} px");
    strlstExpectedValues.append("line.p2 {0.0, -50.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    strlstExpectedValues.append("line.pos {0.0, 50.0} px");
    strlstExpectedValues.append("getLine.p1.x: 0 px");
    strlstExpectedValues.append("getLine.p1.y: 100 px");
    strlstExpectedValues.append("getLine.p2.x: 0 px");
    strlstExpectedValues.append("getLine.p2.y: 0 px");
    strlstExpectedValues.append("getLength: 100 px");
    strlstExpectedValues.append("getAngle: 90.0 " + Math::c_strSymbolDegree);
    strlstExpectedValues.append("getPos {0, 50} px");
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

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;

    int idxStep = 0;

    // Line0 (top)
    //------------

    QString strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        pTestStep->setConfigValue("P1", QPointF(0.0, 0.0));
        pTestStep->setConfigValue("P2", QPointF(100.0, 0.0));
    }
    else {
        pTestStep->setConfigValue("P1", QPointF(0.0, 100.0));
        pTestStep->setConfigValue("P2", QPointF(100.0, 100.0));
    }
    QStringList strlstExpectedValues;
    strlstExpectedValues.append("line.p1 {-175.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {175.0, 0.0} px");
    strlstExpectedValues.append("line.length: 350.0 px");
    strlstExpectedValues.append("line.pos {175.0, 0.0} px");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("getLine.p1.x: 0.0 mm");
        strlstExpectedValues.append("getLine.p1.y: 0.0 mm");
        strlstExpectedValues.append("getLine.p2.x: 100.0 mm");
        strlstExpectedValues.append("getLine.p2.y: 0.0 mm");
    }
    else {
        strlstExpectedValues.append("getLine.p1.x: 0.0 mm");
        strlstExpectedValues.append("getLine.p1.y: 100.0 mm");
        strlstExpectedValues.append("getLine.p2.x: 100.0 mm");
        strlstExpectedValues.append("getLine.p2.y: 100.0 mm");
    }
    strlstExpectedValues.append("getLength: 100.0 mm");
    strlstExpectedValues.append("getAngle: 0.0 " + Math::c_strSymbolDegree);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("getPos {50.0, 0.0} mm");
    }
    else {
        strlstExpectedValues.append("getPos {50.0, 100.0} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line1 (right)
    //--------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        pTestStep->setConfigValue("P1", QPointF(100.0, 0.0));
        pTestStep->setConfigValue("P2", QPointF(100.0, 100.0));
    }
    else {
        pTestStep->setConfigValue("P1", QPointF(100.0, 100.0));
        pTestStep->setConfigValue("P2", QPointF(100.0, 0.0));
    }
    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {0.0, -175.0} px");
    strlstExpectedValues.append("line.p2 {0.0, 175.0} px");
    strlstExpectedValues.append("line.length: 350.0 px");
    strlstExpectedValues.append("line.pos {350.0, 175.0} px");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("getLine.p1.x: 100.0 mm");
        strlstExpectedValues.append("getLine.p1.y: 0.0 mm");
        strlstExpectedValues.append("getLine.p2.x: 100.0 mm");
        strlstExpectedValues.append("getLine.p2.y: 100.0 mm");
    }
    else {
        strlstExpectedValues.append("getLine.p1.x: 100.0 mm");
        strlstExpectedValues.append("getLine.p1.y: 100.0 mm");
        strlstExpectedValues.append("getLine.p2.x: 100.0 mm");
        strlstExpectedValues.append("getLine.p2.y: 0.0 mm");
    }
    strlstExpectedValues.append("getLength: 100.0 mm");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("getAngle: 270.0 " + Math::c_strSymbolDegree);
    }
    else {
        strlstExpectedValues.append("getAngle: 90.0 " + Math::c_strSymbolDegree);
    }
    strlstExpectedValues.append("getPos {100.0, 50.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line2 (bottom)
    //---------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        pTestStep->setConfigValue("P1", QPointF(100.0, 100.0));
        pTestStep->setConfigValue("P2", QPointF(0.0, 100.0));
    }
    else {
        pTestStep->setConfigValue("P1", QPointF(100.0, 0.0));
        pTestStep->setConfigValue("P2", QPointF(0.0, 0.0));
    }
    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {175.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {-175.0, 0.0} px");
    strlstExpectedValues.append("line.length: 350.0 px");
    strlstExpectedValues.append("line.pos {175.0, 350.0} px");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("getLine.p1.x: 100.0 mm");
        strlstExpectedValues.append("getLine.p1.y: 100.0 mm");
        strlstExpectedValues.append("getLine.p2.x: 0.0 mm");
        strlstExpectedValues.append("getLine.p2.y: 100.0 mm");
    }
    else {
        strlstExpectedValues.append("getLine.p1.x: 100.0 mm");
        strlstExpectedValues.append("getLine.p1.y: 0.0 mm");
        strlstExpectedValues.append("getLine.p2.x: 0.0 mm");
        strlstExpectedValues.append("getLine.p2.y: 0.0 mm");
    }
    strlstExpectedValues.append("getLength: 100.0 mm");
    strlstExpectedValues.append("getAngle: 180.0 " + Math::c_strSymbolDegree);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("getPos {50.0, 100.0} mm");
    }
    else {
        strlstExpectedValues.append("getPos {50.0, 0.0} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line3 (left)
    //-------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        pTestStep->setConfigValue("P1", QPointF(0.0, 100.0));
        pTestStep->setConfigValue("P2", QPointF(0.0, 0.0));
    }
    else {
        pTestStep->setConfigValue("P1", QPointF(0.0, 0.0));
        pTestStep->setConfigValue("P2", QPointF(0.0, 100.0));
    }
    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {0.0, 175.0} px");
    strlstExpectedValues.append("line.p2 {0.0, -175.0} px");
    strlstExpectedValues.append("line.length: 350.0 px");
    strlstExpectedValues.append("line.pos {0.0, 175.0} px");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("getLine.p1.x: 0.0 mm");
        strlstExpectedValues.append("getLine.p1.y: 100.0 mm");
        strlstExpectedValues.append("getLine.p2.x: 0.0 mm");
        strlstExpectedValues.append("getLine.p2.y: 0.0 mm");
    }
    else {
        strlstExpectedValues.append("getLine.p1.x: 0.0 mm");
        strlstExpectedValues.append("getLine.p1.y: 0.0 mm");
        strlstExpectedValues.append("getLine.p2.x: 0.0 mm");
        strlstExpectedValues.append("getLine.p2.y: 100.0 mm");
    }
    strlstExpectedValues.append("getLength: 100.0 mm");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("getAngle: 90.0 " + Math::c_strSymbolDegree);
    }
    else {
        strlstExpectedValues.append("getAngle: 270.0 " + Math::c_strSymbolDegree);
    }
    strlstExpectedValues.append("getPos {0.0, 50.0} mm");
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line4
    //------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        pTestStep->setConfigValue("P1", QPointF(28.57143, 28.57143));
        pTestStep->setConfigValue("P2", QPointF(71.42857, 71.42857));
    }
    else {
        pTestStep->setConfigValue("P1", QPointF(28.57143, 71.42857));
        pTestStep->setConfigValue("P2", QPointF(71.42857, 28.57143));
    }
    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {-75.0, -75.0} px");
    strlstExpectedValues.append("line.p2 {75.0, 75.0} px");
    strlstExpectedValues.append("line.length: 212.1 px");
    strlstExpectedValues.append("line.pos {175.0, 175.0} px");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("getLine.p1.x: 28.6 mm");
        strlstExpectedValues.append("getLine.p1.y: 28.6 mm");
        strlstExpectedValues.append("getLine.p2.x: 71.4 mm");
        strlstExpectedValues.append("getLine.p2.y: 71.4 mm");
    }
    else {
        strlstExpectedValues.append("getLine.p1.x: 28.6 mm");
        strlstExpectedValues.append("getLine.p1.y: 71.4 mm");
        strlstExpectedValues.append("getLine.p2.x: 71.4 mm");
        strlstExpectedValues.append("getLine.p2.y: 28.6 mm");
    }
    strlstExpectedValues.append("getLength: 60.6 mm");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("getAngle: 315.0 " + Math::c_strSymbolDegree);
    }
    else {
        strlstExpectedValues.append("getAngle: 45.0 " + Math::c_strSymbolDegree);
    }
    strlstExpectedValues.append("getPos {50.0, 50.0} mm");
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
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("{0.0, 0.0} mm");
    }
    else {
        strlstExpectedValues.append("{0.0, 100.0} mm");
    }
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
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("{28.6, 28.6} mm");
    }
    else {
        strlstExpectedValues.append("{28.6, 71.4} mm");
    }
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
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("{57.1, 57.1} mm");
    }
    else {
        strlstExpectedValues.append("{57.1, 42.9} mm");
    }
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
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("{100.0, 100.0} mm");
    }
    else {
        strlstExpectedValues.append("{100.0, 0.0} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    // mm -> px
    //---------

    ZS::Test::CTestStepGroup* pGrpConvertToPhysValPointMM2Px = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " convert(mm, px)",
        /* pTSGrpParent */ pGrpConvertToPhysValPoint );

    idxStep = 0;

    double fYPos_mm = 0.0;
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        fYPos_mm = 100.0;
    }
    QString strYPos = QString::number(static_cast<int>(fYPos_mm));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Point(0, " + strYPos + ") mm, px)",
        /* strOperation    */ "DrawingScene.convert(Point(0, " + strYPos + ") mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValPointMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValPoint(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Point", QPointF(0.0, fYPos_mm));
    pTestStep->setConfigValue("Point.Unit", "mm");
    pTestStep->setConfigValue("Point.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{0, 0} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    fYPos_mm = 20.0;
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        fYPos_mm = 80.0;
    }
    strYPos = QString::number(static_cast<int>(fYPos_mm));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Point(20, " + strYPos + ") mm, px)",
        /* strOperation    */ "DrawingScene.convert(Point(20, " + strYPos + ") mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValPointMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValPoint(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Point", QPointF(20.0, fYPos_mm));
    pTestStep->setConfigValue("Point.Unit", "mm");
    pTestStep->setConfigValue("Point.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{70, 70} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    fYPos_mm = 50.0;
    strYPos = QString::number(static_cast<int>(fYPos_mm));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Point(50, " + strYPos + ") mm, px)",
        /* strOperation    */ "DrawingScene.convert(Point(50, " + strYPos + ") mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValPointMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValPoint(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Point", QPointF(50.0, fYPos_mm));
    pTestStep->setConfigValue("Point.Unit", "mm");
    pTestStep->setConfigValue("Point.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{175, 175} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    fYPos_mm = 100.0;
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        fYPos_mm = 0.0;
    }
    strYPos = QString::number(static_cast<int>(fYPos_mm));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Point(100, " + strYPos + ") mm, px)",
        /* strOperation    */ "DrawingScene.convert(Point(100, " + strYPos + ") mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValPointMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValPoint(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Point", QPointF(100.0, fYPos_mm));
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
    strlstExpectedValues.append("{100.0, 0.0} mm");
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
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(1, 1) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Size(1, 1) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValSizeMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(1.0, 1.0));
    pTestStep->setConfigValue("Size.Unit", "mm");
    pTestStep->setConfigValue("Size.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{4, 4} px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(10, 10) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Size(10, 10) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValSizeMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(10.0, 10.0));
    pTestStep->setConfigValue("Size.Unit", "mm");
    pTestStep->setConfigValue("Size.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{35, 35} px");
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
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Size(90, 90) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Size(90, 90) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValSizeMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Size", QSizeF(90.0, 90.0));
    pTestStep->setConfigValue("Size.Unit", "mm");
    pTestStep->setConfigValue("Size.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("{315, 315} px");
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
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(P1(0, 0), P2(350, 0)) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Line(P1(0, 0), P2(350, 0)) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValLinePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Line", QLineF(0.0, 0.0, 350.0, 0.0));
    pTestStep->setConfigValue("Line.Unit", "px");
    pTestStep->setConfigValue("Line.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("P1 {0.0, 0.0} mm");
        strlstExpectedValues.append("P2 {100.0, 0.0} mm");
        strlstExpectedValues.append("Center {50.0, 0.0} mm");
        strlstExpectedValues.append("dx: 100.0 mm");
        strlstExpectedValues.append("dy: 0.0 mm");
    }
    else {
        strlstExpectedValues.append("P1 {0.0, 100.0} mm");
        strlstExpectedValues.append("P2 {100.0, 100.0} mm");
        strlstExpectedValues.append("Center {50.0, 100.0} mm");
        strlstExpectedValues.append("dx: 100.0 mm");
        strlstExpectedValues.append("dy: 0.0 mm");
    }
    strlstExpectedValues.append("Length: 100.0 mm");
    strlstExpectedValues.append("Angle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(P1(350, 0), P2(350, 350)) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Line(P1(350, 0), P2(350, 350)) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValLinePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Line", QLineF(350.0, 0.0, 350.0, 350.0));
    pTestStep->setConfigValue("Line.Unit", "px");
    pTestStep->setConfigValue("Line.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("P1 {100.0, 0.0} mm");
        strlstExpectedValues.append("P2 {100.0, 100.0} mm");
        strlstExpectedValues.append("Center {100.0, 50.0} mm");
        strlstExpectedValues.append("dx: 0.0 mm");
        strlstExpectedValues.append("dy: 100.0 mm");
        strlstExpectedValues.append("Length: 100.0 mm");
        strlstExpectedValues.append("Angle: 270.0 " + Math::c_strSymbolDegree);
    }
    else {
        strlstExpectedValues.append("P1 {100.0, 100.0} mm");
        strlstExpectedValues.append("P2 {100.0, 0.0} mm");
        strlstExpectedValues.append("Center {100.0, 50.0} mm");
        strlstExpectedValues.append("dx: 0.0 mm");
        strlstExpectedValues.append("dy: -100.0 mm");
        strlstExpectedValues.append("Length: 100.0 mm");
        strlstExpectedValues.append("Angle: 90.0 " + Math::c_strSymbolDegree);
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(P1(350, 350), P2(0, 350)) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Line(P1(350, 350), P2(0, 350)) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValLinePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("Line", QLineF(350.0, 350.0, 0.0, 350.0));
    pTestStep->setConfigValue("Line.Unit", "px");
    pTestStep->setConfigValue("Line.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("P1 {100.0, 100.0} mm");
        strlstExpectedValues.append("P2 {0.0, 100.0} mm");
        strlstExpectedValues.append("Center {50.0, 100.0} mm");
        strlstExpectedValues.append("dx: -100.0 mm");
        strlstExpectedValues.append("dy: 0.0 mm");
        strlstExpectedValues.append("Length: 100.0 mm");
        strlstExpectedValues.append("Angle: 180.0 " + Math::c_strSymbolDegree);
    }
    else {
        strlstExpectedValues.append("P1 {100.0, 0.0} mm");
        strlstExpectedValues.append("P2 {0.0, 0.0} mm");
        strlstExpectedValues.append("Center {50.0, 0.0} mm");
        strlstExpectedValues.append("dx: -100.0 mm");
        strlstExpectedValues.append("dy: 0.0 mm");
        strlstExpectedValues.append("Length: 100.0 mm");
        strlstExpectedValues.append("Angle: 180.0 " + Math::c_strSymbolDegree);
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(P1(0, 350), P2(0, 0)) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Line(P1(0, 350), P2(0, 0)) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValLinePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("Line", QLineF(0.0, 350.0, 0.0, 0.0));
    pTestStep->setConfigValue("Line.Unit", "px");
    pTestStep->setConfigValue("Line.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("P1 {0.0, 100.0} mm");
        strlstExpectedValues.append("P2 {0.0, 0.0} mm");
        strlstExpectedValues.append("Center {0.0, 50.0} mm");
        strlstExpectedValues.append("dx: 0.0 mm");
        strlstExpectedValues.append("dy: -100.0 mm");
        strlstExpectedValues.append("Length: 100.0 mm");
        strlstExpectedValues.append("Angle: 90.0 " + Math::c_strSymbolDegree);
    }
    else {
        strlstExpectedValues.append("P1 {0.0, 0.0} mm");
        strlstExpectedValues.append("P2 {0.0, 100.0} mm");
        strlstExpectedValues.append("Center {0.0, 50.0} mm");
        strlstExpectedValues.append("dx: 0.0 mm");
        strlstExpectedValues.append("dy: 100.0 mm");
        strlstExpectedValues.append("Length: 100.0 mm");
        strlstExpectedValues.append("Angle: 270.0 " + Math::c_strSymbolDegree);
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(P1(100, 100), P2(250, 250)) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Line(P1(100, 100), P2(250, 250)) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValLinePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("Line", QLineF(100.0, 100.0, 250.0, 250.0));
    pTestStep->setConfigValue("Line.Unit", "px");
    pTestStep->setConfigValue("Line.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("P1 {28.6, 28.6} mm");
        strlstExpectedValues.append("P2 {71.4, 71.4} mm");
        strlstExpectedValues.append("Center {50.0, 50.0} mm");
        strlstExpectedValues.append("dx: 42.9 mm");
        strlstExpectedValues.append("dy: 42.9 mm");
        strlstExpectedValues.append("Length: 60.6 mm");
        strlstExpectedValues.append("Angle: 315.0 " + Math::c_strSymbolDegree);
    }
    else {
        strlstExpectedValues.append("P1 {28.6, 71.4} mm");
        strlstExpectedValues.append("P2 {71.4, 28.6} mm");
        strlstExpectedValues.append("Center {50.0, 50.0} mm");
        strlstExpectedValues.append("dx: 42.9 mm");
        strlstExpectedValues.append("dy: -42.9 mm");
        strlstExpectedValues.append("Length: 60.6 mm");
        strlstExpectedValues.append("Angle: 45.0 " + Math::c_strSymbolDegree);
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    // mm -> px
    //---------

    ZS::Test::CTestStepGroup* pGrpConvertToPhysValLineMM2Px = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " convert(mm, px)",
        /* pTSGrpParent */ pGrpConvertToPhysValLine );

    idxStep = 0;

    double fP1YPos_mm = 0.0;
    double fP2YPos_mm = 0.0;
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        fP1YPos_mm = 100.0;
        fP2YPos_mm = 100.0;
    }
    QString strP1YPos = QString::number(static_cast<int>(fP1YPos_mm));
    QString strP2YPos = QString::number(static_cast<int>(fP2YPos_mm));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(P1(0, " + strP1YPos + "), P2(100, " + strP2YPos + ")) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Line(P1(0, " + strP1YPos + "), P2(100, " + strP2YPos + ")) mm, px))",
        /* pGrpParent      */ pGrpConvertToPhysValLineMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("Line", QLineF(0.0, fP1YPos_mm, 100.0, fP2YPos_mm));
    pTestStep->setConfigValue("Line.Unit", "mm");
    pTestStep->setConfigValue("Line.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("P1 {0, 0} px");
    strlstExpectedValues.append("P2 {350, 0} px");
    strlstExpectedValues.append("Center {175, 0} px");
    strlstExpectedValues.append("dx: 350 px");
    strlstExpectedValues.append("dy: 0 px");
    strlstExpectedValues.append("Length: 350 px");
    strlstExpectedValues.append("Angle: 0.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    fP1YPos_mm = 0.0;
    fP2YPos_mm = 100.0;
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        fP1YPos_mm = 100.0;
        fP2YPos_mm = 0.0;
    }
    strP1YPos = QString::number(static_cast<int>(fP1YPos_mm));
    strP2YPos = QString::number(static_cast<int>(fP2YPos_mm));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(P1(100, " + strP1YPos + "), P2(100, " + strP2YPos + ")) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Line(P1(100, " + strP1YPos + "), P2(100, " + strP2YPos + ")) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValLineMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("Line", QLineF(100.0, fP1YPos_mm, 100.0, fP2YPos_mm));
    pTestStep->setConfigValue("Line.Unit", "mm");
    pTestStep->setConfigValue("Line.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("P1 {350, 0} px");
    strlstExpectedValues.append("P2 {350, 350} px");
    strlstExpectedValues.append("Center {350, 175} px");
    strlstExpectedValues.append("dx: 0 px");
    strlstExpectedValues.append("dy: 350 px");
    strlstExpectedValues.append("Length: 350 px");
    strlstExpectedValues.append("Angle: 270.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    fP1YPos_mm = 100.0;
    fP2YPos_mm = 100.0;
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        fP1YPos_mm = 0.0;
        fP2YPos_mm = 0.0;
    }
    strP1YPos = QString::number(static_cast<int>(fP1YPos_mm));
    strP2YPos = QString::number(static_cast<int>(fP2YPos_mm));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(P1(100, " + strP1YPos + "), P2(0, " + strP2YPos + ")) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Line(P1(100, " + strP1YPos + "), P2(0, " + strP2YPos + ")) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValLineMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("Line", QLineF(100.0, fP1YPos_mm, 0.0, fP2YPos_mm));
    pTestStep->setConfigValue("Line.Unit", "mm");
    pTestStep->setConfigValue("Line.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("P1 {350, 350} px");
    strlstExpectedValues.append("P2 {0, 350} px");
    strlstExpectedValues.append("Center {175, 350} px");
    strlstExpectedValues.append("dx: -350 px");
    strlstExpectedValues.append("dy: 0 px");
    strlstExpectedValues.append("Length: 350 px");
    strlstExpectedValues.append("Angle: 180.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    fP1YPos_mm = 100.0;
    fP2YPos_mm = 0.0;
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        fP1YPos_mm = 0.0;
        fP2YPos_mm = 100.0;
    }
    strP1YPos = QString::number(static_cast<int>(fP1YPos_mm));
    strP2YPos = QString::number(static_cast<int>(fP2YPos_mm));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(P1(" + strP1YPos + ", 100), P2(0, " + strP2YPos + ")) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Line(P1(" + strP1YPos + ", 100), P2(0, " + strP2YPos + ")) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValLineMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("Line", QLineF(0.0, fP1YPos_mm, 0.0, fP2YPos_mm));
    pTestStep->setConfigValue("Line.Unit", "mm");
    pTestStep->setConfigValue("Line.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("P1 {0, 350} px");
    strlstExpectedValues.append("P2 {0, 0} px");
    strlstExpectedValues.append("Center {0, 175} px");
    strlstExpectedValues.append("dx: 0 px");
    strlstExpectedValues.append("dy: -350 px");
    strlstExpectedValues.append("Length: 350 px");
    strlstExpectedValues.append("Angle: 90.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    fP1YPos_mm = 28.6;
    fP2YPos_mm = 71.4;
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        fP1YPos_mm = 71.4;
        fP2YPos_mm = 28.6;
    }
    strP1YPos = QString::number(static_cast<int>(fP1YPos_mm));
    strP2YPos = QString::number(static_cast<int>(fP2YPos_mm));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(P1(28.6, " + strP1YPos + "), P2(71.4, " + strP2YPos + ")) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Line(P1(28.6, " + strP1YPos + "), P2(71.4, " + strP2YPos + ")) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValLineMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("Line", QLineF(28.6, fP1YPos_mm, 71.4, fP2YPos_mm));
    pTestStep->setConfigValue("Line.Unit", "mm");
    pTestStep->setConfigValue("Line.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("P1 {100, 100} px");
    strlstExpectedValues.append("P2 {250, 250} px");
    strlstExpectedValues.append("Center {175, 175} px");
    strlstExpectedValues.append("dx: 150 px");
    strlstExpectedValues.append("dy: 150 px");
    strlstExpectedValues.append("Length: 212 px");
    strlstExpectedValues.append("Angle: 315.0 " + Math::c_strSymbolDegree);
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
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Rect((0, 0), (350, 350)) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Rect((0, 0), (350, 350)) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValRectPx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Rect", QRectF(QPointF(0.0, 0.0), QPointF(350.0, 350.0)));
    pTestStep->setConfigValue("Rect.Unit", "px");
    pTestStep->setConfigValue("Rect.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("Top: 0.0 mm");
        strlstExpectedValues.append("Bottom: 100.0 mm");
        strlstExpectedValues.append("Left: 0.0 mm");
        strlstExpectedValues.append("Right: 100.0 mm");
        strlstExpectedValues.append("TopLeft {0.0, 0.0} mm");
        strlstExpectedValues.append("TopRight {100.0, 0.0} mm");
        strlstExpectedValues.append("BottomRight {100.0, 100.0} mm");
        strlstExpectedValues.append("BottomLeft {0.0, 100.0} mm");
        strlstExpectedValues.append("Center {50.0, 50.0} mm");
        strlstExpectedValues.append("Width: 100.0 mm");
        strlstExpectedValues.append("Height: 100.0 mm");
    }
    else {
        strlstExpectedValues.append("Top: 100.0 mm");
        strlstExpectedValues.append("Bottom: 0.0 mm");
        strlstExpectedValues.append("Left: 0.0 mm");
        strlstExpectedValues.append("Right: 100.0 mm");
        strlstExpectedValues.append("TopLeft {0.0, 100.0} mm");
        strlstExpectedValues.append("TopRight {100.0, 100.0} mm");
        strlstExpectedValues.append("BottomRight {100.0, 0.0} mm");
        strlstExpectedValues.append("BottomLeft {0.0, 0.0} mm");
        strlstExpectedValues.append("Center {50.0, 50.0} mm");
        strlstExpectedValues.append("Width: 100.0 mm");
        strlstExpectedValues.append("Height: -100.0 mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Rect((70, 180), (130, 320)) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Rect((70, 180), (130, 320)) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValRectPx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValRect(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("Rect", QRectF(QPointF(70.0, 180.0), QPointF(130.0, 320.0)));
    pTestStep->setConfigValue("Rect.Unit", "px");
    pTestStep->setConfigValue("Rect.Resolution", 1.0);
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("Top: 51.4 mm");
        strlstExpectedValues.append("Bottom: 91.4 mm");
        strlstExpectedValues.append("Left: 20.0 mm");
        strlstExpectedValues.append("Right: 37.1 mm");
        strlstExpectedValues.append("TopLeft {20.0, 51.4} mm");
        strlstExpectedValues.append("TopRight {37.1, 51.4} mm");
        strlstExpectedValues.append("BottomRight {37.1, 91.4} mm");
        strlstExpectedValues.append("BottomLeft {20.0, 91.4} mm");
        strlstExpectedValues.append("Center {28.6, 71.4} mm");
        strlstExpectedValues.append("Width: 17.1 mm");
        strlstExpectedValues.append("Height: 40.0 mm");
    }
    else {
        strlstExpectedValues.append("Top: 48.6 mm");
        strlstExpectedValues.append("Bottom: 8.6 mm");
        strlstExpectedValues.append("Left: 20.0 mm");
        strlstExpectedValues.append("Right: 37.1 mm");
        strlstExpectedValues.append("TopLeft {20.0, 48.6} mm");
        strlstExpectedValues.append("TopRight {37.1, 48.6} mm");
        strlstExpectedValues.append("BottomRight {37.1, 8.6} mm");
        strlstExpectedValues.append("BottomLeft {20.0, 8.6} mm");
        strlstExpectedValues.append("Center {28.6, 28.6} mm");
        strlstExpectedValues.append("Width: 17.1 mm");
        strlstExpectedValues.append("Height: -40.0 mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    // mm -> px
    //---------

    ZS::Test::CTestStepGroup* pGrpConvertToPhysValRectMM2Px = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " convert(mm, px)",
        /* pTSGrpParent */ pGrpConvertToPhysValRect );

    idxStep = 0;

    double fTLYPos_mm = 0.0;
    double fBRYPos_mm = 100.0;
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        fTLYPos_mm = 100.0;
        fBRYPos_mm = 0.0;
    }
    QString strTLYPos = QString::number(static_cast<int>(fTLYPos_mm));
    QString strBRYPos = QString::number(static_cast<int>(fBRYPos_mm));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Rect((0, " + strTLYPos + "), (100, " + strBRYPos + ")) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Rect((0, " + strTLYPos + "), (100, " + strBRYPos + ")) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValRectMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("Rect", QRectF(QPointF(0.0, fTLYPos_mm), QPointF(100.0, fBRYPos_mm)));
    pTestStep->setConfigValue("Rect.Unit", "mm");
    pTestStep->setConfigValue("Rect.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Top: 0 px");
    strlstExpectedValues.append("Bottom: 350 px");
    strlstExpectedValues.append("Left: 0 px");
    strlstExpectedValues.append("Right: 350 px");
    strlstExpectedValues.append("TopLeft {0, 0} px");
    strlstExpectedValues.append("TopRight {350, 0} px");
    strlstExpectedValues.append("BottomRight {350, 350} px");
    strlstExpectedValues.append("BottomLeft {0, 350} px");
    strlstExpectedValues.append("Center {175, 175} px");
    strlstExpectedValues.append("Width: 350 px");
    strlstExpectedValues.append("Height: 350 px");
    pTestStep->setExpectedValues(strlstExpectedValues);

    fTLYPos_mm = 51.4;
    fBRYPos_mm = 91.4;
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        fTLYPos_mm = 48.6;
        fBRYPos_mm = 8.6;
    }
    strTLYPos = QString::number(static_cast<int>(fTLYPos_mm));
    strBRYPos = QString::number(static_cast<int>(fBRYPos_mm));
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Rect((20.0, " + strTLYPos + "), (37.1, " + strBRYPos + ")) mm, px)",
        /* strOperation    */ "DrawingScene.convert(Rect((20.0, " + strTLYPos + "), (37.1, " + strBRYPos + ")) mm, px)",
        /* pGrpParent      */ pGrpConvertToPhysValRectMM2Px,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValRect(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("Rect", QRectF(QPointF(20.0, fTLYPos_mm), QPointF(37.1, fBRYPos_mm)));
    pTestStep->setConfigValue("Rect.Unit", "mm");
    pTestStep->setConfigValue("Rect.Resolution", 0.1);
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Top: 180 px");
    strlstExpectedValues.append("Bottom: 320 px");
    strlstExpectedValues.append("Left: 70 px");
    strlstExpectedValues.append("Right: 130 px");
    strlstExpectedValues.append("TopLeft {70, 180} px");
    strlstExpectedValues.append("TopRight {130, 180} px");
    strlstExpectedValues.append("BottomRight {130, 320} px");
    strlstExpectedValues.append("BottomLeft {70, 320} px");
    strlstExpectedValues.append("Center {100, 250} px");
    strlstExpectedValues.append("Width: 60 px");
    strlstExpectedValues.append("Height: 140 px");
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
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add Lines",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;

    int idxStep = 0;

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    // Line0 (vertical)
    //-----------------

    QString strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        pTestStep->setConfigValue("P1", QPointF(300.0, 250.0));
        pTestStep->setConfigValue("P2", QPointF(300.0, 350.0));
    }
    else {
        pTestStep->setConfigValue("P1", QPointF(300.0, 350.0));
        pTestStep->setConfigValue("P2", QPointF(300.0, 250.0));
    }
    QStringList strlstExpectedValues;
    strlstExpectedValues.append("line.p1 {0.0, -50.0} px");
    strlstExpectedValues.append("line.p2 {0.0, 50.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    strlstExpectedValues.append("line.pos {300.0, 300.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("getLine.p1.x: 300 px");
        strlstExpectedValues.append("getLine.p1.y: 250 px");
        strlstExpectedValues.append("getLine.p2.x: 300 px");
        strlstExpectedValues.append("getLine.p2.y: 350 px");
        strlstExpectedValues.append("getLength: 100 px");
        strlstExpectedValues.append("getAngle: 270.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("getPos {300, 300} px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("getLine.p1.x: 300.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 250.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 300.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 350.00 mm");
            strlstExpectedValues.append("getLength: 100.00 mm");
            strlstExpectedValues.append("getAngle: 270.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {300.00, 300.00} mm");
        }
        else {
            strlstExpectedValues.append("getLine.p1.x: 300.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 350.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 300.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 250.00 mm");
            strlstExpectedValues.append("getLength: 100.00 mm");
            strlstExpectedValues.append("getAngle: 90.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {300.00, 300.00} mm");
        }
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line1 (horizontal)
    //-------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        pTestStep->setConfigValue("P1", QPointF(250.0, 300.0));
        pTestStep->setConfigValue("P2", QPointF(350.0, 300.0));
    }
    else {
        pTestStep->setConfigValue("P1", QPointF(250.0, 300.0));
        pTestStep->setConfigValue("P2", QPointF(350.0, 300.0));
    }
    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {-50.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {50.0, 0.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    strlstExpectedValues.append("line.pos {300.0, 300.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("getLine.p1.x: 250 px");
        strlstExpectedValues.append("getLine.p1.y: 300 px");
        strlstExpectedValues.append("getLine.p2.x: 350 px");
        strlstExpectedValues.append("getLine.p2.y: 300 px");
        strlstExpectedValues.append("getLength: 100 px");
        strlstExpectedValues.append("getAngle: 0.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("getPos {300, 300} px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("getLine.p1.x: 250.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 300.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 350.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 300.00 mm");
            strlstExpectedValues.append("getLength: 100.00 mm");
            strlstExpectedValues.append("getAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {300.00, 300.00} mm");
        }
        else {
            strlstExpectedValues.append("getLine.p1.x: 250.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 300.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 350.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 300.00 mm");
            strlstExpectedValues.append("getLength: 100.00 mm");
            strlstExpectedValues.append("getAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {300.00, 300.00} mm");
        }
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line2 (Checkmark line from left to bottom)
    //----------------------------------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        pTestStep->setConfigValue("P1", QPointF(250.0, 250.0));
        pTestStep->setConfigValue("P2", QPointF(325.0, 350.0));
    }
    else {
        pTestStep->setConfigValue("P1", QPointF(250.0, 350.0));
        pTestStep->setConfigValue("P2", QPointF(325.0, 250.0));
    }
    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {-37.5, -50.0} px");
    strlstExpectedValues.append("line.p2 {37.5, 50.0} px");
    strlstExpectedValues.append("line.length: 125.0 px");
    strlstExpectedValues.append("line.pos {287.5, 300.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("getLine.p1.x: 250 px");
        strlstExpectedValues.append("getLine.p1.y: 250 px");
        strlstExpectedValues.append("getLine.p2.x: 325 px");
        strlstExpectedValues.append("getLine.p2.y: 350 px");
        strlstExpectedValues.append("getLength: 125 px");
        strlstExpectedValues.append("getAngle: 306.9 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("getPos {288, 300} px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("getLine.p1.x: 250.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 250.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 325.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 350.00 mm");
            strlstExpectedValues.append("getLength: 125.00 mm");
            strlstExpectedValues.append("getAngle: 306.9 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {287.50, 300.00} mm");
        }
        else {
            strlstExpectedValues.append("getLine.p1.x: 250.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 350.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 325.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 250.00 mm");
            strlstExpectedValues.append("getLength: 125.00 mm");
            strlstExpectedValues.append("getAngle: 53.1 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {287.50, 300.00} mm");
        }
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line3 (Checkmark line from bottom to right)
    //---------------------------------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        pTestStep->setConfigValue("P1", QPointF(325.0, 350.0));
        pTestStep->setConfigValue("P2", QPointF(350.0, 325.0));
    }
    else {
        pTestStep->setConfigValue("P1", QPointF(325.0, 250.0));
        pTestStep->setConfigValue("P2", QPointF(350.0, 275.0));
    }
    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {-12.5, 12.5} px");
    strlstExpectedValues.append("line.p2 {12.5, -12.5} px");
    strlstExpectedValues.append("line.length: 35.4 px");
    strlstExpectedValues.append("line.pos {337.5, 337.5} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("getLine.p1.x: 325 px");
        strlstExpectedValues.append("getLine.p1.y: 350 px");
        strlstExpectedValues.append("getLine.p2.x: 350 px");
        strlstExpectedValues.append("getLine.p2.y: 325 px");
        strlstExpectedValues.append("getLength: 35 px");
        strlstExpectedValues.append("getAngle: 45.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("getPos {338, 338} px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("getLine.p1.x: 325.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 350.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 350.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 325.00 mm");
            strlstExpectedValues.append("getLength: 35.36 mm");
            strlstExpectedValues.append("getAngle: 45.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {337.50, 337.50} mm");
        }
        else {
            strlstExpectedValues.append("getLine.p1.x: 325.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 250.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 350.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 275.00 mm");
            strlstExpectedValues.append("getLength: 35.36 mm");
            strlstExpectedValues.append("getAngle: 315.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {337.50, 262.50} mm");
        }
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line4 (Top line of rectangle in second quadrant)
    //-------------------------------------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        pTestStep->setConfigValue("P1", QPointF(320.0, 270.0));
        pTestStep->setConfigValue("P2", QPointF(330.0, 270.0));
    }
    else {
        pTestStep->setConfigValue("P1", QPointF(320.0, 330.0));
        pTestStep->setConfigValue("P2", QPointF(330.0, 330.0));
    }
    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {-5.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {5.0, 0.0} px");
    strlstExpectedValues.append("line.length: 10.0 px");
    strlstExpectedValues.append("line.pos {325.0, 270.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("getLine.p1.x: 320 px");
        strlstExpectedValues.append("getLine.p1.y: 270 px");
        strlstExpectedValues.append("getLine.p2.x: 330 px");
        strlstExpectedValues.append("getLine.p2.y: 270 px");
        strlstExpectedValues.append("getLength: 10 px");
        strlstExpectedValues.append("getAngle: 0.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("getPos {325, 270} px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("getLine.p1.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 270.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 270.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
            strlstExpectedValues.append("getAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {325.00, 270.00} mm");
        }
        else {
            strlstExpectedValues.append("getLine.p1.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 330.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 330.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
            strlstExpectedValues.append("getAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {325.00, 330.00} mm");
        }
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line5 (Right line of rectangle in second quadrant)
    //----------------------------------------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        pTestStep->setConfigValue("P1", QPointF(330.0, 270.0));
        pTestStep->setConfigValue("P2", QPointF(330.0, 280.0));
    }
    else {
        pTestStep->setConfigValue("P1", QPointF(330.0, 330.0));
        pTestStep->setConfigValue("P2", QPointF(330.0, 320.0));
    }
    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {0.0, -5.0} px");
    strlstExpectedValues.append("line.p2 {0.0, 5.0} px");
    strlstExpectedValues.append("line.length: 10.0 px");
    strlstExpectedValues.append("line.pos {330.0, 275.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("getLine.p1.x: 330 px");
        strlstExpectedValues.append("getLine.p1.y: 270 px");
        strlstExpectedValues.append("getLine.p2.x: 330 px");
        strlstExpectedValues.append("getLine.p2.y: 280 px");
        strlstExpectedValues.append("getLength: 10 px");
        strlstExpectedValues.append("getAngle: 270.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("getPos {330, 275} px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("getLine.p1.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 270.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 280.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
            strlstExpectedValues.append("getAngle: 270.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {330.00, 275.00} mm");
        }
        else {
            strlstExpectedValues.append("getLine.p1.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 330.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 320.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
            strlstExpectedValues.append("getAngle: 90.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {330.00, 325.00} mm");
        }
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line6 (Bottom line of rectangle in second quadrant)
    //----------------------------------------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        pTestStep->setConfigValue("P1", QPointF(330.0, 280.0));
        pTestStep->setConfigValue("P2", QPointF(320.0, 280.0));
    }
    else {
        pTestStep->setConfigValue("P1", QPointF(330.0, 320.0));
        pTestStep->setConfigValue("P2", QPointF(320.0, 320.0));
    }
    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {5.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {-5.0, 0.0} px");
    strlstExpectedValues.append("line.length: 10.0 px");
    strlstExpectedValues.append("line.pos {325.0, 280.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("getLine.p1.x: 330 px");
        strlstExpectedValues.append("getLine.p1.y: 280 px");
        strlstExpectedValues.append("getLine.p2.x: 320 px");
        strlstExpectedValues.append("getLine.p2.y: 280 px");
        strlstExpectedValues.append("getLength: 10 px");
        strlstExpectedValues.append("getAngle: 180.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("getPos {325, 280} px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("getLine.p1.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 280.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 280.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
            strlstExpectedValues.append("getAngle: 180.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {325.00, 280.00} mm");
        }
        else {
            strlstExpectedValues.append("getLine.p1.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 320.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 320.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
            strlstExpectedValues.append("getAngle: 180.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {325.00, 320.00} mm");
        }
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line7 (Left line of rectangle in second quadrant)
    //----------------------------------------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddLines,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        pTestStep->setConfigValue("P1", QPointF(320.0, 280.0));
        pTestStep->setConfigValue("P2", QPointF(320.0, 270.0));
    }
    else {
        pTestStep->setConfigValue("P1", QPointF(320.0, 320.0));
        pTestStep->setConfigValue("P2", QPointF(320.0, 330.0));
    }
    strlstExpectedValues.clear();
    strlstExpectedValues.append("line.p1 {0.0, 5.0} px");
    strlstExpectedValues.append("line.p2 {0.0, -5.0} px");
    strlstExpectedValues.append("line.length: 10.0 px");
    strlstExpectedValues.append("line.pos {320.0, 275.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("getLine.p1.x: 320 px");
        strlstExpectedValues.append("getLine.p1.y: 280 px");
        strlstExpectedValues.append("getLine.p2.x: 320 px");
        strlstExpectedValues.append("getLine.p2.y: 270 px");
        strlstExpectedValues.append("getLength: 10 px");
        strlstExpectedValues.append("getAngle: 90.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("getPos {320, 275} px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("getLine.p1.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 280.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 270.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
            strlstExpectedValues.append("getAngle: 90.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {320.00, 275.00} mm");
        }
        else {
            strlstExpectedValues.append("getLine.p1.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 320.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 330.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
            strlstExpectedValues.append("getAngle: 270.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("getPos {320.00, 325.00} mm");
        }
    }
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

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);
    QString strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;
    QStringList strlstGraphObjsAddToGroup;

    int idxStep = 0;

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    // Group0 (short lines which form a rectangle)
    //--------------------------------------------

    QString strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(pIdxTree->buildKeyInTreeStr(strEntryType, "Line4"));
    strlstGraphObjsAddToGroup.append(pIdxTree->buildKeyInTreeStr(strEntryType, "Line5"));
    strlstGraphObjsAddToGroup.append(pIdxTree->buildKeyInTreeStr(strEntryType, "Line6"));
    strlstGraphObjsAddToGroup.append(pIdxTree->buildKeyInTreeStr(strEntryType, "Line7"));
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);

    QStringList strlstExpectedValues;
    strlstExpectedValues.append("Group0.topLeft {-5.0, -5.0} px");
    strlstExpectedValues.append("Group0.bottomRight {5.0, 5.0} px");
    strlstExpectedValues.append("Group0.size {10.0, 10.0} px");
    strlstExpectedValues.append("Group0.pos {325.0, 275.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Group0.getRect.topLeft.x: 320 px");
        strlstExpectedValues.append("Group0.getRect.topLeft.y: 270 px");
        strlstExpectedValues.append("Group0.getRect.bottomRight.x: 330 px");
        strlstExpectedValues.append("Group0.getRect.bottomRight.y: 280 px");
        strlstExpectedValues.append("Group0.getRect.size.width: 10 px");
        strlstExpectedValues.append("Group0.getRect.size.height: 10 px");
        strlstExpectedValues.append("Group0.getPos {325, 275} px");
        strlstExpectedValues.append("Line4.pos {0.0, -5.0} px");
        strlstExpectedValues.append("Line4.getLine.p1.x: 0 px");
        strlstExpectedValues.append("Line4.getLine.p1.y: 0 px");
        strlstExpectedValues.append("Line4.getLine.p2.x: 10 px");
        strlstExpectedValues.append("Line4.getLine.p2.y: 0 px");
        strlstExpectedValues.append("Line4.getLength: 10 px");
        strlstExpectedValues.append("Line4.getAngle: 0.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line4.getPos {5, 0} px");
        strlstExpectedValues.append("Line5.pos {5.0, 0.0} px");
        strlstExpectedValues.append("Line5.getLine.p1.x: 10 px");
        strlstExpectedValues.append("Line5.getLine.p1.y: 0 px");
        strlstExpectedValues.append("Line5.getLine.p2.x: 10 px");
        strlstExpectedValues.append("Line5.getLine.p2.y: 10 px");
        strlstExpectedValues.append("Line5.getLength: 10 px");
        strlstExpectedValues.append("Line5.getAngle: 270.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line5.getPos {10, 5} px");
        strlstExpectedValues.append("Line6.pos {0.0, 5.0} px");
        strlstExpectedValues.append("Line6.getLine.p1.x: 10 px");
        strlstExpectedValues.append("Line6.getLine.p1.y: 10 px");
        strlstExpectedValues.append("Line6.getLine.p2.x: 0 px");
        strlstExpectedValues.append("Line6.getLine.p2.y: 10 px");
        strlstExpectedValues.append("Line6.getLength: 10 px");
        strlstExpectedValues.append("Line6.getAngle: 180.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line6.getPos {5, 10} px");
        strlstExpectedValues.append("Line7.pos {-5.0, 0.0} px");
        strlstExpectedValues.append("Line7.getLine.p1.x: 0 px");
        strlstExpectedValues.append("Line7.getLine.p1.y: 10 px");
        strlstExpectedValues.append("Line7.getLine.p2.x: 0 px");
        strlstExpectedValues.append("Line7.getLine.p2.y: 0 px");
        strlstExpectedValues.append("Line7.getLength: 10 px");
        strlstExpectedValues.append("Line7.getAngle: 90.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line7.getPos {0, 5} px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("Group0.getRect.topLeft.x: 320.00 mm");
            strlstExpectedValues.append("Group0.getRect.topLeft.y: 270.00 mm");
            strlstExpectedValues.append("Group0.getRect.bottomRight.x: 330.00 mm");
            strlstExpectedValues.append("Group0.getRect.bottomRight.y: 280.00 mm");
            strlstExpectedValues.append("Group0.getRect.size.width: 10.00 mm");
            strlstExpectedValues.append("Group0.getRect.size.height: 10.00 mm");
            strlstExpectedValues.append("Group0.getPos {325.00, 275.00} mm");
            strlstExpectedValues.append("Line4.pos {0.0, -5.0} px");
            strlstExpectedValues.append("Line4.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line4.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line4.getLine.p2.x: 10.00 mm");
            strlstExpectedValues.append("Line4.getLine.p2.y: 0.00 mm");
            strlstExpectedValues.append("Line4.getLength: 10.00 mm");
            strlstExpectedValues.append("Line4.getAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line4.getPos {5.00, 0.00} mm");
            strlstExpectedValues.append("Line5.pos {5.0, 0.0} px");
            strlstExpectedValues.append("Line5.getLine.p1.x: 10.00 mm");
            strlstExpectedValues.append("Line5.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line5.getLine.p2.x: 10.00 mm");
            strlstExpectedValues.append("Line5.getLine.p2.y: 10.00 mm");
            strlstExpectedValues.append("Line5.getLength: 10.00 mm");
            strlstExpectedValues.append("Line5.getAngle: 270.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line5.getPos {10.00, 5.00} mm");
            strlstExpectedValues.append("Line6.pos {0.0, 5.0} px");
            strlstExpectedValues.append("Line6.getLine.p1.x: 10.00 mm");
            strlstExpectedValues.append("Line6.getLine.p1.y: 10.00 mm");
            strlstExpectedValues.append("Line6.getLine.p2.x: 0.00 mm");
            strlstExpectedValues.append("Line6.getLine.p2.y: 10.00 mm");
            strlstExpectedValues.append("Line6.getLength: 10.00 mm");
            strlstExpectedValues.append("Line6.getAngle: 180.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line6.getPos {5.00, 10.00} mm");
            strlstExpectedValues.append("Line7.pos {-5.0, 0.0} px");
            strlstExpectedValues.append("Line7.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line7.getLine.p1.y: 10.00 mm");
            strlstExpectedValues.append("Line7.getLine.p2.x: 0.00 mm");
            strlstExpectedValues.append("Line7.getLine.p2.y: 0.00 mm");
            strlstExpectedValues.append("Line7.getLength: 10.00 mm");
            strlstExpectedValues.append("Line7.getAngle: 90.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line7.getPos {0.00, 5.00} mm");
        }
        else {
            strlstExpectedValues.append("Group0.getRect.topLeft.x: 320.00 mm");
            strlstExpectedValues.append("Group0.getRect.topLeft.y: 330.00 mm");
            strlstExpectedValues.append("Group0.getRect.bottomRight.x: 330.00 mm");
            strlstExpectedValues.append("Group0.getRect.bottomRight.y: 320.00 mm");
            strlstExpectedValues.append("Group0.getRect.size.width: 10.00 mm");
            strlstExpectedValues.append("Group0.getRect.size.height: -10.00 mm");
            strlstExpectedValues.append("Group0.getPos {325.00, 325.00} mm");
            strlstExpectedValues.append("Line4.pos {0.0, -5.0} px");
            strlstExpectedValues.append("Line4.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line4.getLine.p1.y: 10.00 mm");
            strlstExpectedValues.append("Line4.getLine.p2.x: 10.00 mm");
            strlstExpectedValues.append("Line4.getLine.p2.y: 10.00 mm");
            strlstExpectedValues.append("Line4.getLength: 10.00 mm");
            strlstExpectedValues.append("Line4.getAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line4.getPos {5.00, 10.00} mm");
            strlstExpectedValues.append("Line5.pos {5.0, 0.0} px");
            strlstExpectedValues.append("Line5.getLine.p1.x: 10.00 mm");
            strlstExpectedValues.append("Line5.getLine.p1.y: 10.00 mm");
            strlstExpectedValues.append("Line5.getLine.p2.x: 10.00 mm");
            strlstExpectedValues.append("Line5.getLine.p2.y: 0.00 mm");
            strlstExpectedValues.append("Line5.getLength: 10.00 mm");
            strlstExpectedValues.append("Line5.getAngle: 90.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line5.getPos {10.00, 5.00} mm");
            strlstExpectedValues.append("Line6.pos {0.0, 5.0} px");
            strlstExpectedValues.append("Line6.getLine.p1.x: 10.00 mm");
            strlstExpectedValues.append("Line6.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line6.getLine.p2.x: 0.00 mm");
            strlstExpectedValues.append("Line6.getLine.p2.y: 0.00 mm");
            strlstExpectedValues.append("Line6.getLength: 10.00 mm");
            strlstExpectedValues.append("Line6.getAngle: 180.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line6.getPos {5.00, 0.00} mm");
            strlstExpectedValues.append("Line7.pos {-5.0, 0.0} px");
            strlstExpectedValues.append("Line7.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line7.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line7.getLine.p2.x: 0.00 mm");
            strlstExpectedValues.append("Line7.getLine.p2.y: 10.00 mm");
            strlstExpectedValues.append("Line7.getLength: 10.00 mm");
            strlstExpectedValues.append("Line7.getAngle: 270.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line7.getPos {0.00, 5.00} mm");
        }
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Group1 (checkmark)
    //-------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(pIdxTree->buildKeyInTreeStr(strEntryType, "Line2"));
    strlstGraphObjsAddToGroup.append(pIdxTree->buildKeyInTreeStr(strEntryType, "Line3"));
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);

    strlstExpectedValues.clear();
    strlstExpectedValues.append("Group1.topLeft {-50.0, -50.0} px");
    strlstExpectedValues.append("Group1.bottomRight {50.0, 50.0} px");
    strlstExpectedValues.append("Group1.size {100.0, 100.0} px");
    strlstExpectedValues.append("Group1.pos {300.0, 300.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Group1.getRect.topLeft.x: 250 px");
        strlstExpectedValues.append("Group1.getRect.topLeft.y: 250 px");
        strlstExpectedValues.append("Group1.getRect.bottomRight.x: 350 px");
        strlstExpectedValues.append("Group1.getRect.bottomRight.y: 350 px");
        strlstExpectedValues.append("Group1.getRect.size.width: 100 px");
        strlstExpectedValues.append("Group1.getRect.size.height: 100 px");
        strlstExpectedValues.append("Group1.getPos {300, 300} px");
        strlstExpectedValues.append("Line2.pos {-12.5, 0.0} px");
        strlstExpectedValues.append("Line2.getLine.p1.x: 0 px");
        strlstExpectedValues.append("Line2.getLine.p1.y: 0 px");
        strlstExpectedValues.append("Line2.getLine.p2.x: 75 px");
        strlstExpectedValues.append("Line2.getLine.p2.y: 100 px");
        strlstExpectedValues.append("Line2.getLength: 125 px");
        strlstExpectedValues.append("Line2.getAngle: 306.9 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line2.getPos {38, 50} px");
        strlstExpectedValues.append("Line3.pos {37.5, 37.5} px");
        strlstExpectedValues.append("Line3.getLine.p1.x: 75 px");
        strlstExpectedValues.append("Line3.getLine.p1.y: 100 px");
        strlstExpectedValues.append("Line3.getLine.p2.x: 100 px");
        strlstExpectedValues.append("Line3.getLine.p2.y: 75 px");
        strlstExpectedValues.append("Line3.getLength: 35 px");
        strlstExpectedValues.append("Line3.getAngle: 45.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line3.getPos {88, 88} px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("Group1.getRect.topLeft.x: 250.00 mm");
            strlstExpectedValues.append("Group1.getRect.topLeft.y: 250.00 mm");
            strlstExpectedValues.append("Group1.getRect.bottomRight.x: 350.00 mm");
            strlstExpectedValues.append("Group1.getRect.bottomRight.y: 350.00 mm");
            strlstExpectedValues.append("Group1.getRect.size.width: 100.00 mm");
            strlstExpectedValues.append("Group1.getRect.size.height: 100.00 mm");
            strlstExpectedValues.append("Group1.getPos {300.00, 300.00} mm");
            strlstExpectedValues.append("Line2.pos {-12.5, 0.0} px");
            strlstExpectedValues.append("Line2.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line2.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line2.getLine.p2.x: 75.00 mm");
            strlstExpectedValues.append("Line2.getLine.p2.y: 100.00 mm");
            strlstExpectedValues.append("Line2.getLength: 125.00 mm");
            strlstExpectedValues.append("Line2.getAngle: 306.9 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line2.getPos {37.50, 50.00} mm");
            strlstExpectedValues.append("Line3.pos {37.5, 37.5} px");
            strlstExpectedValues.append("Line3.getLine.p1.x: 75.00 mm");
            strlstExpectedValues.append("Line3.getLine.p1.y: 100.00 mm");
            strlstExpectedValues.append("Line3.getLine.p2.x: 100.00 mm");
            strlstExpectedValues.append("Line3.getLine.p2.y: 75.00 mm");
            strlstExpectedValues.append("Line3.getLength: 35.36 mm");
            strlstExpectedValues.append("Line3.getAngle: 45.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line3.getPos {87.50, 87.50} mm");
        }
        else {
            strlstExpectedValues.append("Group1.getRect.topLeft.x: 250.00 mm");
            strlstExpectedValues.append("Group1.getRect.topLeft.y: 350.00 mm");
            strlstExpectedValues.append("Group1.getRect.bottomRight.x: 350.00 mm");
            strlstExpectedValues.append("Group1.getRect.bottomRight.y: 250.00 mm");
            strlstExpectedValues.append("Group1.getRect.size.width: 100.00 mm");
            strlstExpectedValues.append("Group1.getRect.size.height: -100.00 mm");
            strlstExpectedValues.append("Group1.getPos {300.00, 300.00} mm");
            strlstExpectedValues.append("Line2.pos {-12.5, 0.0} px");
            strlstExpectedValues.append("Line2.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line2.getLine.p1.y: 100.00 mm");
            strlstExpectedValues.append("Line2.getLine.p2.x: 75.00 mm");
            strlstExpectedValues.append("Line2.getLine.p2.y: 0.00 mm");
            strlstExpectedValues.append("Line2.getLength: 125.00 mm");
            strlstExpectedValues.append("Line2.getAngle: 53.1 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line2.getPos {37.50, 50.00} mm");
            strlstExpectedValues.append("Line3.pos {37.5, 37.5} px");
            strlstExpectedValues.append("Line3.getLine.p1.x: 75.00 mm");
            strlstExpectedValues.append("Line3.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line3.getLine.p2.x: 100.00 mm");
            strlstExpectedValues.append("Line3.getLine.p2.y: 25.00 mm");
            strlstExpectedValues.append("Line3.getLength: 35.36 mm");
            strlstExpectedValues.append("Line3.getAngle: 315.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line3.getPos {87.50, 12.50} mm");
        }
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Group2 (horizonal and vertical lines together with checkmark and rectangle group)
    //----------------------------------------------------------------------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add(" + strFactoryGroupName + ", " + strGraphObjName + ")",
        /* strOperation    */ "DrawingScene.addGraphObj(" + strFactoryGroupName + ", " + strGraphObjType + ")",
        /* pGrpParent      */ pGrpAddGroups,
        /* szDoTestStepFct */ SLOT(doTestStepAddGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    strlstGraphObjsAddToGroup.clear();
    strlstGraphObjsAddToGroup.append(pIdxTree->buildKeyInTreeStr(strEntryType, "Line0"));
    strlstGraphObjsAddToGroup.append(pIdxTree->buildKeyInTreeStr(strEntryType, "Line1"));
    strlstGraphObjsAddToGroup.append(pIdxTree->buildKeyInTreeStr(strEntryType, "Group0"));
    strlstGraphObjsAddToGroup.append(pIdxTree->buildKeyInTreeStr(strEntryType, "Group1"));
    pTestStep->setConfigValue("AddToGroup", strlstGraphObjsAddToGroup);

    strlstExpectedValues.clear();
    strlstExpectedValues.append("Group2.topLeft {-50.0, -50.0} px");
    strlstExpectedValues.append("Group2.bottomRight {50.0, 50.0} px");
    strlstExpectedValues.append("Group2.size {100.0, 100.0} px");
    strlstExpectedValues.append("Group2.pos {300.0, 300.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Group2.getRect.topLeft.x: 250 px");
        strlstExpectedValues.append("Group2.getRect.topLeft.y: 250 px");
        strlstExpectedValues.append("Group2.getRect.bottomRight.x: 350 px");
        strlstExpectedValues.append("Group2.getRect.bottomRight.y: 350 px");
        strlstExpectedValues.append("Group2.getRect.size.width: 100 px");
        strlstExpectedValues.append("Group2.getRect.size.height: 100 px");
        strlstExpectedValues.append("Group2.getPos {300, 300} px");
        strlstExpectedValues.append("Line0.pos {0.0, 0.0} px");
        strlstExpectedValues.append("Line0.getLine.p1.x: 50 px");
        strlstExpectedValues.append("Line0.getLine.p1.y: 0 px");
        strlstExpectedValues.append("Line0.getLine.p2.x: 50 px");
        strlstExpectedValues.append("Line0.getLine.p2.y: 100 px");
        strlstExpectedValues.append("Line0.getLength: 100 px");
        strlstExpectedValues.append("Line0.getAngle: 270.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line0.getPos {50, 50} px");
        strlstExpectedValues.append("Line1.pos {0.0, 0.0} px");
        strlstExpectedValues.append("Line1.getLine.p1.x: 0 px");
        strlstExpectedValues.append("Line1.getLine.p1.y: 50 px");
        strlstExpectedValues.append("Line1.getLine.p2.x: 100 px");
        strlstExpectedValues.append("Line1.getLine.p2.y: 50 px");
        strlstExpectedValues.append("Line1.getLength: 100 px");
        strlstExpectedValues.append("Line1.getAngle: 0.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line1.getPos {50, 50} px");
        strlstExpectedValues.append("Group0.pos {25.0, -25.0} px");
        strlstExpectedValues.append("Group0.getRect.topLeft.x: 70 px");
        strlstExpectedValues.append("Group0.getRect.topLeft.y: 20 px");
        strlstExpectedValues.append("Group0.getRect.bottomRight.x: 80 px");
        strlstExpectedValues.append("Group0.getRect.bottomRight.y: 30 px");
        strlstExpectedValues.append("Group0.getRect.size.width: 10 px");
        strlstExpectedValues.append("Group0.getRect.size.height: 10 px");
        strlstExpectedValues.append("Group0.getPos {75, 25} px");
        strlstExpectedValues.append("Group1.pos {0.0, 0.0} px");
        strlstExpectedValues.append("Group1.getRect.topLeft.x: 0 px");
        strlstExpectedValues.append("Group1.getRect.topLeft.y: 0 px");
        strlstExpectedValues.append("Group1.getRect.bottomRight.x: 100 px");
        strlstExpectedValues.append("Group1.getRect.bottomRight.y: 100 px");
        strlstExpectedValues.append("Group1.getRect.size.width: 100 px");
        strlstExpectedValues.append("Group1.getRect.size.height: 100 px");
        strlstExpectedValues.append("Group1.getPos {50, 50} px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("Group2.getRect.topLeft.x: 250.00 mm");
            strlstExpectedValues.append("Group2.getRect.topLeft.y: 250.00 mm");
            strlstExpectedValues.append("Group2.getRect.bottomRight.x: 350.00 mm");
            strlstExpectedValues.append("Group2.getRect.bottomRight.y: 350.00 mm");
            strlstExpectedValues.append("Group2.getRect.size.width: 100.00 mm");
            strlstExpectedValues.append("Group2.getRect.size.height: 100.00 mm");
            strlstExpectedValues.append("Group2.getPos {300.00, 300.00} mm");
            strlstExpectedValues.append("Line0.pos {0.0, 0.0} px");
            strlstExpectedValues.append("Line0.getLine.p1.x: 50.00 mm");
            strlstExpectedValues.append("Line0.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line0.getLine.p2.x: 50.00 mm");
            strlstExpectedValues.append("Line0.getLine.p2.y: 100.00 mm");
            strlstExpectedValues.append("Line0.getLength: 100.00 mm");
            strlstExpectedValues.append("Line0.getAngle: 270.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line0.getPos {50.00, 50.00} mm");
            strlstExpectedValues.append("Line1.pos {0.0, 0.0} px");
            strlstExpectedValues.append("Line1.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line1.getLine.p1.y: 50.00 mm");
            strlstExpectedValues.append("Line1.getLine.p2.x: 100.00 mm");
            strlstExpectedValues.append("Line1.getLine.p2.y: 50.00 mm");
            strlstExpectedValues.append("Line1.getLength: 100.00 mm");
            strlstExpectedValues.append("Line1.getAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line1.getPos {50.00, 50.00} mm");
            strlstExpectedValues.append("Group0.pos {25.0, -25.0} px");
            strlstExpectedValues.append("Group0.getRect.topLeft.x: 70.00 mm");
            strlstExpectedValues.append("Group0.getRect.topLeft.y: 20.00 mm");
            strlstExpectedValues.append("Group0.getRect.bottomRight.x: 80.00 mm");
            strlstExpectedValues.append("Group0.getRect.bottomRight.y: 30.00 mm");
            strlstExpectedValues.append("Group0.getRect.size.width: 10.00 mm");
            strlstExpectedValues.append("Group0.getRect.size.height: 10.00 mm");
            strlstExpectedValues.append("Group0.getPos {75.00, 25.00} mm");
            strlstExpectedValues.append("Group1.pos {0.0, 0.0} px");
            strlstExpectedValues.append("Group1.getRect.topLeft.x: 0.00 mm");
            strlstExpectedValues.append("Group1.getRect.topLeft.y: 0.00 mm");
            strlstExpectedValues.append("Group1.getRect.bottomRight.x: 100.00 mm");
            strlstExpectedValues.append("Group1.getRect.bottomRight.y: 100.00 mm");
            strlstExpectedValues.append("Group1.getRect.size.width: 100.00 mm");
            strlstExpectedValues.append("Group1.getRect.size.height: 100.00 mm");
            strlstExpectedValues.append("Group1.getPos {50.00, 50.00} mm");
        }
        else {
            strlstExpectedValues.append("Group2.getRect.topLeft.x: 250.00 mm");
            strlstExpectedValues.append("Group2.getRect.topLeft.y: 350.00 mm");
            strlstExpectedValues.append("Group2.getRect.bottomRight.x: 350.00 mm");
            strlstExpectedValues.append("Group2.getRect.bottomRight.y: 250.00 mm");
            strlstExpectedValues.append("Group2.getRect.size.width: 100.00 mm");
            strlstExpectedValues.append("Group2.getRect.size.height: -100.00 mm");
            strlstExpectedValues.append("Group2.getPos {300.00, 300.00} mm");
            strlstExpectedValues.append("Line0.pos {0.0, 0.0} px");
            strlstExpectedValues.append("Line0.getLine.p1.x: 50.00 mm");
            strlstExpectedValues.append("Line0.getLine.p1.y: 100.00 mm");
            strlstExpectedValues.append("Line0.getLine.p2.x: 50.00 mm");
            strlstExpectedValues.append("Line0.getLine.p2.y: 0.00 mm");
            strlstExpectedValues.append("Line0.getLength: 100.00 mm");
            strlstExpectedValues.append("Line0.getAngle: 90.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line0.getPos {50.00, 50.00} mm");
            strlstExpectedValues.append("Line1.pos {0.0, 0.0} px");
            strlstExpectedValues.append("Line1.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line1.getLine.p1.y: 50.00 mm");
            strlstExpectedValues.append("Line1.getLine.p2.x: 100.00 mm");
            strlstExpectedValues.append("Line1.getLine.p2.y: 50.00 mm");
            strlstExpectedValues.append("Line1.getLength: 100.00 mm");
            strlstExpectedValues.append("Line1.getAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line1.getPos {50.00, 50.00} mm");
            strlstExpectedValues.append("Group0.pos {25.0, -25.0} px");
            strlstExpectedValues.append("Group0.getRect.topLeft.x: 70.00 mm");
            strlstExpectedValues.append("Group0.getRect.topLeft.y: 80.00 mm");
            strlstExpectedValues.append("Group0.getRect.bottomRight.x: 80.00 mm");
            strlstExpectedValues.append("Group0.getRect.bottomRight.y: 70.00 mm");
            strlstExpectedValues.append("Group0.getRect.size.width: 10.00 mm");
            strlstExpectedValues.append("Group0.getRect.size.height: -10.00 mm");
            strlstExpectedValues.append("Group0.getPos {75.00, 75.00} mm");
            strlstExpectedValues.append("Group1.pos {0.0, 0.0} px");
            strlstExpectedValues.append("Group1.getRect.topLeft.x: 0.00 mm");
            strlstExpectedValues.append("Group1.getRect.topLeft.y: 100.00 mm");
            strlstExpectedValues.append("Group1.getRect.bottomRight.x: 100.00 mm");
            strlstExpectedValues.append("Group1.getRect.bottomRight.y: 0.00 mm");
            strlstExpectedValues.append("Group1.getRect.size.width: 100.00 mm");
            strlstExpectedValues.append("Group1.getRect.size.height: -100.00 mm");
            strlstExpectedValues.append("Group1.getPos {50.00, 50.00} mm");
        }
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

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
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Draw Lines",
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
        pGrpLine0ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameP1,
        QPoint(95, 100), QPoint(95, 90));
    createTestGroupShowAndMoveTextLabel(
        pGrpLine0ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameP2,
        QPoint(95, 150), QPoint(95, 160));

    ZS::Test::CTestStepGroup* pGrpLine0ShowAndMoveGeometryLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Geometry Labels",
        /* pTSGrpParent */ pGrpLine0ShowAndMoveLabels );
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine0ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameLength,
        QPoint(105, 125), QPoint(125, 125));
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine0ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameAngle,
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
        pGrpLine1ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameP1,
        QPoint(495, 100), QPoint(495, 90));
    createTestGroupShowAndMoveTextLabel(
        pGrpLine1ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameP2,
        QPoint(495, 150), QPoint(495, 160));

    ZS::Test::CTestStepGroup* pGrpLine1ShowAndMoveGeometryLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Geometry Labels",
        /* pTSGrpParent */ pGrpLine1ShowAndMoveLabels );
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine1ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameLength,
        QPoint(505, 125), QPoint(525, 125));
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine1ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameAngle,
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
        pGrpLine2ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameP1,
        QPoint(495, 500), QPoint(495, 510));
    createTestGroupShowAndMoveTextLabel(
        pGrpLine2ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameP2,
        QPoint(495, 450), QPoint(495, 440));

    ZS::Test::CTestStepGroup* pGrpLine2ShowAndMoveGeometryLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Geometry Labels",
        /* pTSGrpParent */ pGrpLine2ShowAndMoveLabels );
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine2ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameLength,
        QPoint(505, 475), QPoint(525, 475));
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine2ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameAngle,
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
        pGrpLine3ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameP1,
        QPoint(95, 500), QPoint(95, 510));
    createTestGroupShowAndMoveTextLabel(
        pGrpLine3ShowAndMoveTextLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameP2,
        QPoint(95, 450), QPoint(95, 440));

    ZS::Test::CTestStepGroup* pGrpLine3ShowAndMoveGeometryLabels = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " " + strGraphObjName + " Geometry Labels",
        /* pTSGrpParent */ pGrpLine3ShowAndMoveLabels );
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine3ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameLength,
        QPoint(105, 475), QPoint(125, 475));
    createTestGroupShowAndMoveGeometryLabel(
        pGrpLine3ShowAndMoveGeometryLabels, io_idxGroup, strGraphObjName, CGraphObjLine::c_strGeometryLabelNameAngle,
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
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Draw Groups",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);
    QString strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;

    int idxStep = 0;

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " UnsetCurrentDrawingTool",
        /* strOperation    */ "DrawingScene.setCurrentDrawingTool(null)",
        /* pGrpParent      */ pGrpDrawGroups,
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
        /* pTSGrpParent */ pGrpDrawGroups );

    ZS::Test::CTestStepGroup* pGrpDrawGroup0 = new ZS::Test::CTestStepGroup(
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
        /* pGrpParent      */ pGrpDrawGroup0,
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
        /* pGrpParent      */ pGrpDrawGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpDrawGroup0,
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
        /* pGrpParent      */ pGrpDrawGroup0,
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
        /* pGrpParent      */ pGrpDrawGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);
    pTestStep->setConfigValue("KeyboardModifiers", Qt::ControlModifier);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpDrawGroup0,
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
        /* pGrpParent      */ pGrpDrawGroup0,
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
        /* pGrpParent      */ pGrpDrawGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);
    pTestStep->setConfigValue("KeyboardModifiers", Qt::ControlModifier);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpDrawGroup0,
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
        /* pGrpParent      */ pGrpDrawGroup0,
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
        /* pGrpParent      */ pGrpDrawGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);
    pTestStep->setConfigValue("KeyboardModifiers", Qt::ControlModifier);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpDrawGroup0,
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
        /* pGrpParent      */ pGrpDrawGroup0,
        /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
    pTestStep->setConfigValue("MousePosStart", ptMouseStart);
    pTestStep->setConfigValue("MousePosStop", ptMouseStop);
    pTestStep->setConfigValue("MovesCount", 5);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " GroupSelectedObjects",
        /* strOperation    */ "DrawingScene.groupGraphObjsSelected()",
        /* pGrpParent      */ pGrpDrawGroup0,
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
        /* pTSGrpParent */ pGrpDrawGroups );

    ZS::Test::CTestStepGroup* pGrpDrawGroup1 = new ZS::Test::CTestStepGroup(
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
        /* pGrpParent      */ pGrpDrawGroup1,
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
        /* pGrpParent      */ pGrpDrawGroup1,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpDrawGroup1,
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
        /* pGrpParent      */ pGrpDrawGroup1,
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
        /* pGrpParent      */ pGrpDrawGroup1,
        /* szDoTestStepFct */ SLOT(doTestStepMousePressEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MousePos", ptMousePos);
    pTestStep->setConfigValue("KeyboardModifiers", Qt::ControlModifier);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MouseReleaseEvent(" + qPoint2Str(ptMousePos) + ")",
        /* strOperation    */ "DrawingView.mouseReleaseEvent(" +qPoint2Str(ptMousePos) + ")",
        /* pGrpParent      */ pGrpDrawGroup1,
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
        /* pGrpParent      */ pGrpDrawGroup1,
        /* szDoTestStepFct */ SLOT(doTestStepMouseMoveEvent(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("MouseButtons", Qt::NoButton);
    pTestStep->setConfigValue("MousePosStart", ptMouseStart);
    pTestStep->setConfigValue("MousePosStop", ptMouseStop);
    pTestStep->setConfigValue("MovesCount", 5);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " GroupSelectedObjects",
        /* strOperation    */ "DrawingScene.groupGraphObjsSelected()",
        /* pGrpParent      */ pGrpDrawGroup1,
        /* szDoTestStepFct */ SLOT(doTestStepSetCurrentDrawingTool(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("FactoryGroupName", strFactoryGroupName);
    pTestStep->setConfigValue("GraphObjType", strGraphObjType);

    // Group2 (horizonal and vertical lines together with checkmark and rectangle group)
    //----------------------------------------------------------------------------------

    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    createTestGroupDrawMouseEventsCreateGroup(
        pGrpDrawGroups, io_idxGroup,
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
void CTest::doTestStepDrawingSize( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepDrawingSize",
        /* strAddInfo   */ strMthInArgs );

    CDrawingSize drawingSize("Test");

    QStringList strlstConfigValueKeys = i_pTestStep->getConfigValueKeys();
    QStringList strlstMethodCallsSorted;

    for (const QString& strKey : strlstConfigValueKeys) {
        QStringList strlst = strKey.split(":");
        if (strlst.size() == 2) {
            bool bOk = false;
            int iNr = strlst[0].toInt(&bOk);
            if (bOk && iNr >= 0) {
                while (strlstMethodCallsSorted.size() < (iNr+1)) {
                    strlstMethodCallsSorted.append("");
                }
                strlstMethodCallsSorted[iNr] = strKey;
            }
        }
        else {
            strlstMethodCallsSorted.append(strKey);
        }
    }
    for (const QString& strKey : strlstMethodCallsSorted) {
        QString strConfigValue = i_pTestStep->getConfigValue(strKey).toString();
        if (strKey.contains("setDimensionUnit", Qt::CaseInsensitive)) {
            CEnumScaleDimensionUnit dimensionUnit = strConfigValue;
            drawingSize.setDimensionUnit(dimensionUnit);
        }
        if (strKey.contains("setScreenResolutionInPxPerMM", Qt::CaseInsensitive)) {
            double fRes_px_mm = strConfigValue.toDouble();
            drawingSize.setScreenResolutionInPxPerMM(fRes_px_mm);
        }
        if (strKey.contains("setMetricImageCoorsDecimals", Qt::CaseInsensitive)) {
            int iDecimals = strConfigValue.toInt();
            drawingSize.setMetricImageCoorsDecimals(iDecimals);
        }
        if (strKey.contains("setMetricUnit", Qt::CaseInsensitive)) {
            CUnit unit = strConfigValue.simplified();
            drawingSize.setMetricUnit(unit);
        }
        if (strKey.contains("setNormedPaperSize", Qt::CaseInsensitive)) {
            CEnumNormedPaperSize paperSize = strConfigValue;
            drawingSize.setNormedPaperSize(paperSize);
        }
        if (strKey.contains("setNormedPaperOrientation", Qt::CaseInsensitive)) {
            CEnumOrientation orientation = strConfigValue;
            drawingSize.setNormedPaperOrientation(orientation);
        }
        if (strKey.contains("setScaleFactor", Qt::CaseInsensitive)) {
            QStringList strlstFactor = strConfigValue.split(":");
            if (strlstFactor.size() == 2) {
                bool bDividendOk = false;
                bool bDivisorOk = false;
                int iDividend = strlstFactor[0].toInt(&bDividendOk);
                int iDivisor = strlstFactor[1].toInt(&bDivisorOk);
                if (bDividendOk && iDividend > 0 && bDivisorOk && iDivisor > 0) {
                    drawingSize.setScaleFactor(iDividend, iDivisor);
                }
            }
        }
        if (strKey.contains("setYScaleAxisOrientation", Qt::CaseInsensitive)) {
            CEnumYScaleAxisOrientation yScaleAxisOrientation = strConfigValue;
            drawingSize.setYScaleAxisOrientation(yScaleAxisOrientation);
        }
        if (strKey.contains("setImageSize", Qt::CaseInsensitive)) {
            QStringList strlstImageSize = strConfigValue.split(",");
            if (strlstImageSize.size() == 4) {
                bool bWidthOk = false;
                double fWidth = strlstImageSize[0].toDouble(&bWidthOk);
                bool bHeightOk = false;
                double fHeight = strlstImageSize[1].toDouble(&bHeightOk);
                bool bResolutionOk = false;
                double fResolution = strlstImageSize[2].toDouble(&bResolutionOk);
                CUnit unit = strlstImageSize[3].simplified();
                if (bWidthOk && bHeightOk && bResolutionOk) {
                    drawingSize.setImageSize(CPhysValSize(fWidth, fHeight, fResolution, unit));
                }
            }
        }
    }

    QStringList strlstResultValues;
    strlstResultValues.append(drawingSize.toString());
    i_pTestStep->setResultValues(strlstResultValues);
}

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
        i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlAttrDimensionUnit).toString());
    if (i_pTestStep->hasConfigValue(XmlStreamParser::c_strXmlAttrScreenResolutionPxPerMilliMeter)) {
        double fResPxPerMM = i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlAttrScreenResolutionPxPerMilliMeter).toDouble();
        drawingSize.setScreenResolutionInPxPerMM(fResPxPerMM);
    }
    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        int cxWidth = i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlAttrWidth).toInt();
        int cyHeight = i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlAttrHeight).toInt();
        drawingSize.setImageSize(CPhysVal(cxWidth, Units.Length.px), CPhysVal(cyHeight, Units.Length.px));
    }
    else {
        CUnit unit = drawingSize.metricUnit();
        if (i_pTestStep->hasConfigValue(XmlStreamParser::c_strXmlAttrUnit)) {
            unit = i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlAttrUnit).toString();
            drawingSize.setMetricUnit(unit);
        }
        if (i_pTestStep->hasConfigValue(XmlStreamParser::c_strXmlAttrMetricImageCoorsDecimals)) {
            int iDecimals = i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlAttrMetricImageCoorsDecimals).toInt();
            drawingSize.setMetricImageCoorsDecimals(iDecimals);
        }
        double fxWidth = i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlAttrWidth).toDouble();
        double fyHeight = i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlAttrHeight).toDouble();
        drawingSize.setImageSize(CPhysVal(fxWidth, unit), CPhysVal(fyHeight, unit));
        bool bOk = false;
        if (i_pTestStep->hasConfigValue(XmlStreamParser::c_strXmlAttrYScaleAxisOrientation)) {
            CEnumYScaleAxisOrientation eYScaleOrientation = CEnumYScaleAxisOrientation::fromString(
                i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlAttrYScaleAxisOrientation).toString(), &bOk);
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
        i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlAttrLinesVisible).toBool());
    gridSettings.setLinesDistMin(
        i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlAttrLinesDistMin).toInt());
    gridSettings.setLabelsVisible(
        i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlAttrLabelsVisible).toBool());

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
    CUnit unitSource = Units.Length.px;
    CUnit unitDest = drawingSize.unit();
    if (i_pTestStep->hasConfigValue("Point.Unit")) {
        QString strUnitPoint = i_pTestStep->getConfigValue("Point.Unit").toString();
        unitSource = strUnitPoint;
    }
    if (i_pTestStep->hasConfigValue("UnitDest")) {
        QString strUnitDest = i_pTestStep->getConfigValue("UnitDest").toString();
        unitDest = strUnitDest;
    }
    double fResolution = drawingSize.imageCoorsResolutionInPx();
    if (Units.Length.isMetricUnit(unitSource)) {
        fResolution = drawingSize.imageCoorsResolution(unitSource).getVal();
    }
    // Resolution will be ignored if the line is given in pixels (defaults to 1.0 pixels).
    if (Units.Length.isMetricUnit(unitSource) && i_pTestStep->hasConfigValue("Point.Resolution")) {
        fResolution = i_pTestStep->getConfigValue("Point.Resolution").toDouble();
    }
    QPointF pt = i_pTestStep->getConfigValue("Point").toPointF();
    CPhysValPoint physValPointResult;
    if (Units.Length.isMetricUnit(unitSource)) {
        CPhysValPoint physValPoint(pt, fResolution, unitSource);
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
    CUnit unitSource = Units.Length.px;
    CUnit unitDest = drawingSize.unit();
    if (i_pTestStep->hasConfigValue("Size.Unit")) {
        QString strUnitPoint = i_pTestStep->getConfigValue("Size.Unit").toString();
        unitSource = strUnitPoint;
    }
    if (i_pTestStep->hasConfigValue("UnitDest")) {
        QString strUnitDest = i_pTestStep->getConfigValue("UnitDest").toString();
        unitDest = strUnitDest;
    }
    double fResolution = drawingSize.imageCoorsResolutionInPx();
    if (Units.Length.isMetricUnit(unitSource)) {
        fResolution = drawingSize.imageCoorsResolution(unitSource).getVal();
    }
    // Resolution will be ignored if the line is given in pixels (defaults to 1.0 pixels).
    if (Units.Length.isMetricUnit(unitSource) && i_pTestStep->hasConfigValue("Size.Resolution")) {
        fResolution = i_pTestStep->getConfigValue("Size.Resolution").toDouble();
    }
    QSizeF sizeF = i_pTestStep->getConfigValue("Size").toSizeF();
    CPhysValSize physValSizeResult;
    if (Units.Length.isMetricUnit(unitSource)) {
        CPhysValSize physValSize(sizeF, fResolution, unitSource);
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
    CUnit unitSource = Units.Length.px;
    CUnit unitDest = drawingSize.unit();
    if (i_pTestStep->hasConfigValue("Line.Unit")) {
        QString strUnitPoint = i_pTestStep->getConfigValue("Line.Unit").toString();
        unitSource = strUnitPoint;
    }
    if (i_pTestStep->hasConfigValue("UnitDest")) {
        QString strUnitDest = i_pTestStep->getConfigValue("UnitDest").toString();
        unitDest = strUnitDest;
    }
    double fResolution = drawingSize.imageCoorsResolutionInPx();
    if (Units.Length.isMetricUnit(unitSource)) {
        fResolution = drawingSize.imageCoorsResolution(unitSource).getVal();
    }
    // Resolution will be ignored if the line is given in pixels (defaults to 1.0 pixels).
    if (Units.Length.isMetricUnit(unitSource) && i_pTestStep->hasConfigValue("Line.Resolution")) {
        fResolution = i_pTestStep->getConfigValue("Line.Resolution").toDouble();
    }
    QLineF lineF = i_pTestStep->getConfigValue("Line").toLineF();
    CPhysValLine physValLineResult;
    if (Units.Length.isMetricUnit(unitSource)) {
        CPhysValLine physValLine(lineF, fResolution, unitSource);
        physValLineResult = m_pDrawingScene->convert(physValLine, unitDest);
    }
    else {
        physValLineResult = m_pDrawingScene->convert(lineF, unitDest);
    }
    QStringList strlstResultValues;
    strlstResultValues.append("P1 {" + physValLineResult.p1().toString() + "} " + physValLineResult.unit().symbol());
    strlstResultValues.append("P2 {" + physValLineResult.p2().toString() + "} " + physValLineResult.unit().symbol());
    strlstResultValues.append("Center {" + physValLineResult.center().toString() + "} " + physValLineResult.unit().symbol());
    strlstResultValues.append("dx: " + physValLineResult.dx().toString());
    strlstResultValues.append("dy: " + physValLineResult.dy().toString());
    strlstResultValues.append("Length: " + physValLineResult.length().toString());
    strlstResultValues.append("Angle: " + physValLineResult.angle().toString());
    i_pTestStep->setResultValues(strlstResultValues);
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
    CUnit unitSource = Units.Length.px;
    CUnit unitDest = drawingSize.unit();
    if (i_pTestStep->hasConfigValue("Rect.Unit")) {
        QString strUnitPoint = i_pTestStep->getConfigValue("Rect.Unit").toString();
        unitSource = strUnitPoint;
    }
    if (i_pTestStep->hasConfigValue("UnitDest")) {
        QString strUnitDest = i_pTestStep->getConfigValue("UnitDest").toString();
        unitDest = strUnitDest;
    }
    double fResolution = drawingSize.imageCoorsResolutionInPx();
    if (Units.Length.isMetricUnit(unitSource)) {
        fResolution = drawingSize.imageCoorsResolution(unitSource).getVal();
    }
    // Resolution will be ignored if the line is given in pixels (defaults to 1.0 pixels).
    if (Units.Length.isMetricUnit(unitSource) && i_pTestStep->hasConfigValue("Rect.Resolution")) {
        fResolution = i_pTestStep->getConfigValue("Rect.Resolution").toDouble();
    }
    QRectF rectF = i_pTestStep->getConfigValue("Rect").toRectF();
    CPhysValRect physValRectResult;
    if (Units.Length.isMetricUnit(unitSource)) {
        CPhysValRect physValRect(rectF, fResolution, unitSource);
        physValRectResult = m_pDrawingScene->convert(physValRect, unitDest);
    }
    else {
        physValRectResult = m_pDrawingScene->convert(rectF, unitDest);
    }
    QStringList strlstResultValues;
    strlstResultValues.append("Top: " + physValRectResult.top().toString());
    strlstResultValues.append("Bottom: " + physValRectResult.bottom().toString());
    strlstResultValues.append("Left: " + physValRectResult.left().toString());
    strlstResultValues.append("Right: " + physValRectResult.right().toString());
    strlstResultValues.append("TopLeft {" + physValRectResult.topLeft().toString() + "} " + physValRectResult.unit().symbol());
    strlstResultValues.append("TopRight {" + physValRectResult.topRight().toString() + "} " + physValRectResult.unit().symbol());
    strlstResultValues.append("BottomRight {" + physValRectResult.bottomRight().toString() + "} " + physValRectResult.unit().symbol());
    strlstResultValues.append("BottomLeft {" + physValRectResult.bottomLeft().toString() + "} " + physValRectResult.unit().symbol());
    strlstResultValues.append("Center {" + physValRectResult.center().toString() + "} " + physValRectResult.unit().symbol());
    strlstResultValues.append("Width: " + physValRectResult.width().toString());
    strlstResultValues.append("Height: " + physValRectResult.height().toString());
    i_pTestStep->setResultValues(strlstResultValues);
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

    QStringList strlstResultValues;

    QStringList strlstSavedFile;
    errResultInfo = readFile(strAbsFilePath, strlstSavedFile);
    if (errResultInfo.isErrorResult()) {
        CErrLog::GetInstance()->addEntry(errResultInfo);
        strlstResultValues.append(errResultInfo.toString());
    }

    QString strExpectedResultsAbsFilePath = c_strTestExpectedResultFilesAbsDirPath + QDir::separator() + strFileName;
    if (!strExpectedResultsAbsFilePath.endsWith(".xml")) {
        strExpectedResultsAbsFilePath += ".xml";
    }
    QStringList strlstExpectedFile;
    errResultInfo = readFile(strExpectedResultsAbsFilePath, strlstExpectedFile);
    if (errResultInfo.isErrorResult()) {
        CErrLog::GetInstance()->addEntry(errResultInfo);
        strlstResultValues.append(errResultInfo.toString());
    }

    for (int iLine = 0; (iLine < strlstSavedFile.size() && iLine < strlstExpectedFile.size()); ++iLine) {
        if (strlstSavedFile[iLine] != strlstExpectedFile[iLine]) {
            strlstResultValues.append("Saved file is different from expected result file");
            strlstResultValues.append("Saved file: " + strAbsFilePath);
            strlstResultValues.append("Expected file: " + strExpectedResultsAbsFilePath);
            strlstResultValues.append("First difference recognized in line " + QString::number(iLine+1));
            break;
        }
    }

    if (!strlstResultValues.isEmpty()) {
        i_pTestStep->setResultValues(strlstResultValues);
    }
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

    QStringList strlstResultValues;

    QStringList strlstLoadedFile;
    errResultInfo = readFile(strAbsFilePath, strlstLoadedFile);
    if (errResultInfo.isErrorResult()) {
        CErrLog::GetInstance()->addEntry(errResultInfo);
        strlstResultValues.append(errResultInfo.toString());
    }

    errResultInfo = m_pDrawingScene->save(strAbsFilePath + ".bak");
    if (errResultInfo.isErrorResult()) {
        CErrLog::GetInstance()->addEntry(errResultInfo);
        strlstResultValues.append(errResultInfo.toString());
    }

    QStringList strlstSavedFile;
    errResultInfo = readFile(strAbsFilePath + ".bak", strlstSavedFile);
    if (errResultInfo.isErrorResult()) {
        CErrLog::GetInstance()->addEntry(errResultInfo);
        strlstResultValues.append(errResultInfo.toString());
    }

    for (int iLine = 0; (iLine < strlstLoadedFile.size() && iLine < strlstSavedFile.size()); ++iLine) {
        if (strlstLoadedFile[iLine] != strlstSavedFile[iLine]) {
            strlstResultValues.append("Saved file is different from loaded file");
            strlstResultValues.append("Loaded file: " + strAbsFilePath);
            strlstResultValues.append("Saved file: " + strAbsFilePath + ".bak");
            strlstResultValues.append("First difference recognized in line " + QString::number(iLine+1));
            break;
        }
    }

    if (!strlstResultValues.isEmpty()) {
        i_pTestStep->setResultValues(strlstResultValues);
    }
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
void CTest::doTestStepAddGraphObjLine( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepAddGraphObjLine",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);

    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();

    CPhysValPoint physValPoint1(*m_pDrawingScene);
    physValPoint1 = i_pTestStep->getConfigValue("P1").toPointF();
    CPhysValPoint physValPoint2(*m_pDrawingScene);
    physValPoint2 = i_pTestStep->getConfigValue("P2").toPointF();

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);

    if (pObjFactory != nullptr) {
        CDrawSettings drawSettings(EGraphObjTypeLine);
        CGraphObj* pGraphObj = pObjFactory->createGraphObj(m_pDrawingScene, physValPoint1, drawSettings);
        m_pDrawingScene->addGraphObj(pGraphObj);
        CGraphObjLine* pGraphObjLine = dynamic_cast<CGraphObjLine*>(pGraphObj);
        if (pGraphObjLine != nullptr) {
            pGraphObjLine->setP2(physValPoint2);
        }
    }

    QStringList strlstResultValues;
    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
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
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepAddGraphObjGroup( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepAddGraphObjGroup",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);

    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();
    QStringList strlstGraphObjsAddToGroup = i_pTestStep->getConfigValue("AddToGroup").toStringList();

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);
    if (pObjFactory != nullptr) {
        CDrawSettings drawSettings(EGraphObjTypeGroup);
        CGraphObj* pGraphObj = pObjFactory->createGraphObj(m_pDrawingScene, CPhysValPoint(*m_pDrawingScene), drawSettings);
        m_pDrawingScene->addGraphObj(pGraphObj);
        CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphObj);
        if (pGraphObjGroup != nullptr) {
            for (const QString& strKeyInTree : strlstGraphObjsAddToGroup) {
                CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strKeyInTree);
                if (pGraphObj != nullptr) {
                    pGraphObjGroup->addToGroup(pGraphObj);
                }
            }
        }
    }

    QStringList strlstResultValues;
    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
    CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphObj);
    if (pGraphObjGroup != nullptr) {
        QRectF rectF = pGraphObjGroup->getBoundingRect();
        strlstResultValues.append(pGraphObjGroup->name() + ".topLeft {" + qPoint2Str(rectF.topLeft()) + "} px");
        strlstResultValues.append(pGraphObjGroup->name() + ".bottomRight {" + qPoint2Str(rectF.bottomRight()) + "} px");
        strlstResultValues.append(pGraphObjGroup->name() + ".size {" + qSize2Str(rectF.size()) + "} px");
        strlstResultValues.append(pGraphObjGroup->name() + ".pos {" + qPoint2Str(pGraphObjGroup->pos()) + "} px");
        CPhysValRect physValRect = pGraphObjGroup->getRect();
        strlstResultValues.append(pGraphObjGroup->name() + ".getRect.topLeft.x: " + physValRect.topLeft().x().toString());
        strlstResultValues.append(pGraphObjGroup->name() + ".getRect.topLeft.y: " + physValRect.topLeft().y().toString());
        strlstResultValues.append(pGraphObjGroup->name() + ".getRect.bottomRight.x: " + physValRect.bottomRight().x().toString());
        strlstResultValues.append(pGraphObjGroup->name() + ".getRect.bottomRight.y: " + physValRect.bottomRight().y().toString());
        CPhysValSize physValSize = pGraphObjGroup->getSize();
        strlstResultValues.append(pGraphObjGroup->name() + ".getRect.size.width: " + physValSize.width().toString());
        strlstResultValues.append(pGraphObjGroup->name() + ".getRect.size.height: " + physValSize.height().toString());
        CPhysValPoint physValPointPos = pGraphObjGroup->getPos();
        strlstResultValues.append(pGraphObjGroup->name() + ".getPos {" + physValPointPos.toString() + "} " + physValPointPos.unit().symbol());
        for (CGraphObj* pGraphObjChild : pGraphObjGroup->childs()) {
            QGraphicsItem* pGraphicsItemChild = dynamic_cast<QGraphicsItem*>(pGraphObjChild);
            strlstResultValues.append(pGraphObjChild->name() + ".pos {" + qPoint2Str(pGraphicsItemChild->pos()) + "} px");
            if (pGraphObjChild->isLine()) {
                CGraphObjLine* pGraphObjChildLine = dynamic_cast<CGraphObjLine*>(pGraphObjChild);
                CPhysValLine physValLine = pGraphObjChildLine->getLine();
                strlstResultValues.append(pGraphObjChild->name() + ".getLine.p1.x: " + physValLine.p1().x().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getLine.p1.y: " + physValLine.p1().y().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getLine.p2.x: " + physValLine.p2().x().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getLine.p2.y: " + physValLine.p2().y().toString());
                CPhysVal physValLength = pGraphObjChildLine->getLength();
                strlstResultValues.append(pGraphObjChild->name() + ".getLength: " + physValLength.toString());
                CPhysVal physValAngle = pGraphObjChildLine->getAngle(Units.Angle.Degree);
                strlstResultValues.append(pGraphObjChild->name() + ".getAngle: " + physValAngle.toString());
                CPhysValPoint physValPointPos = pGraphObjChildLine->getPos();
                strlstResultValues.append(pGraphObjChild->name() + ".getPos {" + physValPointPos.toString() + "} " + physValPointPos.unit().symbol());
            }
            else if (pGraphObjChild->isGroup()) {
                CGraphObjGroup* pGraphObjChildGroup = dynamic_cast<CGraphObjGroup*>(pGraphObjChild);
                CPhysValRect physValRect = pGraphObjChildGroup->getRect();
                strlstResultValues.append(pGraphObjChild->name() + ".getRect.topLeft.x: " + physValRect.topLeft().x().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getRect.topLeft.y: " + physValRect.topLeft().y().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getRect.bottomRight.x: " + physValRect.bottomRight().x().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getRect.bottomRight.y: " + physValRect.bottomRight().y().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getRect.size.width: " + physValRect.size().width().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getRect.size.height: " + physValRect.size().height().toString());
                CPhysValPoint physValPointPos = pGraphObjChildGroup->getPos();
                strlstResultValues.append(pGraphObjChild->name() + ".getPos {" + physValPointPos.toString() + "} " + physValPointPos.unit().symbol());
            }
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

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CTest::readFile(const QString& i_strAbsFilePath, QStringList& o_strlstLines) const
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), "readFile(" + i_strAbsFilePath + ")");
    QFile file;
    if (i_strAbsFilePath.isEmpty()) {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidFileName);
    }
    else {
        QFileInfo fileInfo(i_strAbsFilePath);
        file.setFileName(i_strAbsFilePath);
        if (!file.open(QIODevice::ReadOnly)) {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileOpenForRead);
            errResultInfo.setAddErrInfoDscr(i_strAbsFilePath);
        }
    }
    if (!errResultInfo.isErrorResult()) {
        QTextStream in(&file);
        QString line;
        while (in.readLineInto(&line)) {
            o_strlstLines << line;
        }
    }
    return errResultInfo;
}
