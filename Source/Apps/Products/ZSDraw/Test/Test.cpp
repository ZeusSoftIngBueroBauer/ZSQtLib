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
    pTestStep->setConfigValue(
        XmlStreamParser::c_strXmlAttrFont, i_gridSettings.labelsFont().family());

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
                      QString::number(static_cast<int>(i_drawingSize.metricImageHeight().getVal())) +
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

} // createTestGroupSaveLoadFile

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

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing100x100 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Pixels Drawing (100 * 100) px",
        /* pTSGrpParent */ pGrpPixelsDrawing );
    {
        CDrawingSize drawingSize("Test-PixelsDrawing");
        drawingSize.setDimensionUnit(EScaleDimensionUnit::Pixels);
        drawingSize.setImageSize(CPhysVal(100, Units.Length.px), CPhysVal(100, Units.Length.px));

        CDrawGridSettings gridSettings("Test-PixelsDrawing");
        gridSettings.setLinesVisible(true);
        gridSettings.setLinesDistMin(20);
        gridSettings.setLabelsVisible(true);
        gridSettings.setLabelsFont(QFont("Terminal"));

        createTestGroupPrepareScene(pGrpPixelsDrawing100x100, io_idxGroup, drawingSize, gridSettings);
        createTestGroupImageSizeAndObjectCoordinatesPixelsDrawingAddLines(pGrpPixelsDrawing100x100, io_idxGroup);
    }

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing101x101 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Pixels Drawing (101 * 101) px",
        /* pTSGrpParent */ pGrpPixelsDrawing );
    {
        CDrawingSize drawingSize("Test-PixelsDrawing");
        drawingSize.setDimensionUnit(EScaleDimensionUnit::Pixels);
        drawingSize.setImageSize(CPhysVal(101, Units.Length.px), CPhysVal(101, Units.Length.px));

        CDrawGridSettings gridSettings("Test-PixelsDrawing");
        gridSettings.setLinesVisible(true);
        gridSettings.setLinesDistMin(20);
        gridSettings.setLabelsVisible(true);
        gridSettings.setLabelsFont(QFont("Terminal"));

        createTestGroupPrepareScene(pGrpPixelsDrawing101x101, io_idxGroup, drawingSize, gridSettings);
        createTestGroupImageSizeAndObjectCoordinatesPixelsDrawingAddLines(pGrpPixelsDrawing101x101, io_idxGroup);
    }

    ZS::Test::CTestStepGroup* pGrpPixelsDrawing800x600 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Pixels Drawing (800 * 600) px",
        /* pTSGrpParent */ pGrpPixelsDrawing );
    {
        CDrawingSize drawingSize("Test-PixelsDrawing");
        drawingSize.setDimensionUnit(EScaleDimensionUnit::Pixels);
        drawingSize.setImageSize(CPhysVal(800, Units.Length.px), CPhysVal(600, Units.Length.px));

        CDrawGridSettings gridSettings("Test-PixelsDrawing");
        gridSettings.setLinesVisible(true);
        gridSettings.setLinesDistMin(20);
        gridSettings.setLabelsVisible(true);
        gridSettings.setLabelsFont(QFont("Terminal"));

        createTestGroupPrepareScene(pGrpPixelsDrawing800x600, io_idxGroup, drawingSize, gridSettings);
        createTestGroupImageSizeAndObjectCoordinatesTransformPhysValShapes(pGrpPixelsDrawing800x600, io_idxGroup, drawingSize);
    }

    ZS::Test::CTestStepGroup* pGrpMetricsDrawing = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Metrics Drawing",
        /* pTSGrpParent */ pGrpImageSizeAndObjectCoordinates );

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " YScaleTopDown",
        /* pTSGrpParent */ pGrpMetricsDrawing );
    {
        ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown100x100 = new ZS::Test::CTestStepGroup(
            /* pTest        */ this,
            /* strName      */ "Group " + QString::number(++io_idxGroup) + " YScaleTopDown (100 * 100) mm",
            /* pTSGrpParent */ pGrpMetricsDrawingYScaleTopDown );
        {
            CDrawingSize drawingSize("Test-MetricsDrawing-YScaleTopDown-100x100");
            drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
            drawingSize.setMetricUnit(Units.Length.mm);
            drawingSize.setMetricImageCoorsDecimals(1);
            drawingSize.setScreenResolutionInPxPerMM(3.5);
            drawingSize.setImageSize(CPhysVal(100, Units.Length.mm), CPhysVal(100, Units.Length.mm));
            drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::TopDown);

            CDrawGridSettings gridSettings("Test-MetricsDrawing-YScaleTopDown-100x100");
            gridSettings.setLinesVisible(true);
            gridSettings.setLinesDistMin(20);
            gridSettings.setLabelsVisible(true);
            gridSettings.setLabelsFont(QFont("Terminal"));

            createTestGroupPrepareScene(pGrpMetricsDrawingYScaleTopDown100x100, io_idxGroup, drawingSize, gridSettings);
            createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConversionFunctions(pGrpMetricsDrawingYScaleTopDown100x100, io_idxGroup, drawingSize);
            createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingAddLines(pGrpMetricsDrawingYScaleTopDown100x100, io_idxGroup, drawingSize);
        }

        ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleTopDown800x600 = new ZS::Test::CTestStepGroup(
            /* pTest        */ this,
            /* strName      */ "Group " + QString::number(++io_idxGroup) + " YScaleTopDown (800 * 600) mm",
            /* pTSGrpParent */ pGrpMetricsDrawingYScaleTopDown );
        {
            CDrawingSize drawingSize("Test-MetricsDrawing-YScaleTopDown-800x600");
            drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
            drawingSize.setMetricUnit(Units.Length.mm);
            drawingSize.setMetricImageCoorsDecimals(1);
            drawingSize.setScreenResolutionInPxPerMM(1.0);
            drawingSize.setImageSize(CPhysVal(800, Units.Length.mm), CPhysVal(600, Units.Length.mm));
            drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::TopDown);

            CDrawGridSettings gridSettings("Test-MetricsDrawing-YScaleTopDown-800x600");
            gridSettings.setLinesVisible(true);
            gridSettings.setLinesDistMin(20);
            gridSettings.setLabelsVisible(true);
            gridSettings.setLabelsFont(QFont("Terminal"));

            createTestGroupPrepareScene(pGrpMetricsDrawingYScaleTopDown800x600, io_idxGroup, drawingSize, gridSettings);
            createTestGroupImageSizeAndObjectCoordinatesTransformPhysValShapes(pGrpMetricsDrawingYScaleTopDown800x600, io_idxGroup, drawingSize);
        }
    }

    ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " YScaleBottomUp",
        /* pTSGrpParent */ pGrpMetricsDrawing );
    {
        ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp100x100 = new ZS::Test::CTestStepGroup(
            /* pTest        */ this,
            /* strName      */ "Group " + QString::number(++io_idxGroup) + " YScaleBottomUp (100 * 100) mm",
            /* pTSGrpParent */ pGrpMetricsDrawingYScaleBottomUp );
        {
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
            gridSettings.setLabelsFont(QFont("Terminal"));

            createTestGroupPrepareScene(pGrpMetricsDrawingYScaleBottomUp100x100, io_idxGroup, drawingSize, gridSettings);
            createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConversionFunctions(pGrpMetricsDrawingYScaleBottomUp100x100, io_idxGroup, drawingSize);
            createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingAddLines(pGrpMetricsDrawingYScaleBottomUp100x100, io_idxGroup, drawingSize);
        }

        ZS::Test::CTestStepGroup* pGrpMetricsDrawingYScaleBottomUp800x600 = new ZS::Test::CTestStepGroup(
            /* pTest        */ this,
            /* strName      */ "Group " + QString::number(++io_idxGroup) + " YScaleBottomUp (800 * 600) mm",
            /* pTSGrpParent */ pGrpMetricsDrawingYScaleBottomUp );
        {
            CDrawingSize drawingSize("Test-MetricsDrawing-YScaleBottomUp-800x600");
            drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
            drawingSize.setMetricUnit(Units.Length.mm);
            drawingSize.setMetricImageCoorsDecimals(1);
            drawingSize.setScreenResolutionInPxPerMM(1.0);
            drawingSize.setImageSize(CPhysVal(800, Units.Length.mm), CPhysVal(600, Units.Length.mm));
            drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::BottomUp);

            CDrawGridSettings gridSettings("Test-MetricsDrawing-YScaleBottomUp-800x600");
            gridSettings.setLinesVisible(true);
            gridSettings.setLinesDistMin(20);
            gridSettings.setLabelsVisible(true);
            gridSettings.setLabelsFont(QFont("Terminal"));

            createTestGroupPrepareScene(pGrpMetricsDrawingYScaleBottomUp800x600, io_idxGroup, drawingSize, gridSettings);
            createTestGroupImageSizeAndObjectCoordinatesTransformPhysValShapes(pGrpMetricsDrawingYScaleBottomUp800x600, io_idxGroup, drawingSize);
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpImageSizeAndObjectCoordinates->path());
    }
    return pGrpImageSizeAndObjectCoordinates;

} // createTestGroupImageSizeAndObjectCoordinates

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupImageSizeAndObjectCoordinatesTransformPhysValShapes(
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
        /* strMethod    */ "createTestGroupImageSizeAndObjectCoordinatesTransformPhysValShapes",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpPhysValShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " PhysValShapeTransformations",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    createTestGroupImageSizeAndObjectCoordinatesTransformPhysValRect(pGrpPhysValShapes, io_idxGroup, i_drawingSize);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpPhysValShapes->path());
    }
    return pGrpPhysValShapes;

} // createTestGroupImageSizeAndObjectCoordinatesTransformPhysValShapes

//------------------------------------------------------------------------------
ZS::Test::CTestStepGroup* CTest::createTestGroupImageSizeAndObjectCoordinatesTransformPhysValRect(
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
        /* strMethod    */ "createTestGroupImageSizeAndObjectCoordinatesTransformPhysValRect",
        /* strAddInfo   */ strMthInArgs );

    QStringList strlstExpectedValues;

    int idxStep = 0;

    CUnit unit = i_drawingSize.unit();

    /* create
    ---------
        200   250   300   350   400      BottomUp
     250 +-----+-----+-----+-----+         350
         |                       |
     300 +           X           +         300
         |                       |
     350 +-----+-----+-----+-----+         250
    */
    QRectF rectFOrig(QPointF(200.0, 250.0), QSizeF(200.0, 100.0));
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        rectFOrig.setTop(350.0);
    }

    ZS::Test::CTestStepGroup* pGrpTransformRect = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " PhysValRect(" + qRect2Str(rectFOrig) + " " + unit.symbol() + ")",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    //--------------------------------------------------------------------------
    // Rotation Angle: 0.0°
    //--------------------------------------------------------------------------

    CPhysVal physValAngle(0.0, Units.Angle.Degree, 0.1);

    ZS::Test::CTestStepGroup* pGrpTransformRectAngle_0_Degree = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Angle(" + physValAngle.toString() + ")",
        /* pTSGrpParent */ pGrpTransformRect );

    /* setAngle(0°)
    ----------------
        200   250   300   350   400      BottomUp
     250 +-----+-----+-----+-----+         350
         |                       |
     300 +           X           +         300
         |                       |
     350 +-----+-----+-----+-----+         250
    */
    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setAngle(" + physValAngle.toString() + ")",
        /* strOperation    */ "setAngle(" + physValAngle.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    // Center, Width and Height must remain the same for all following setAngle test steps
    double fxCenterSetAngle0 = rectFOrig.center().x();
    double fyCenterSetAngle0 = rectFOrig.center().y();
    double fWidthSetAngle0 = rectFOrig.width();
    double fHeightSetAngle0 = rectFOrig.height();
    // When setting the width the left edge must remain the same.
    double fLeftSetAngle0 = fxCenterSetAngle0 - fWidthSetAngle0 / 2.0;
    // When setting the width by moving the left center point the right edge must remain the same.
    double fRightSetAngle0 = fxCenterSetAngle0 + fWidthSetAngle0 / 2.0;
    // When setting the height the top edge must remain the same.
    double fTopSetAngle0 = fyCenterSetAngle0 - fHeightSetAngle0 / 2.0;
    // When setting the height by moving the top center point the bottom edge must remain the same.
    double fBottomSetAngle0 = fyCenterSetAngle0 + fHeightSetAngle0 / 2.0;
    QPointF ptCenterSetAngle0(fxCenterSetAngle0, fyCenterSetAngle0);
    QPointF ptTopLeftSetAngle0(fLeftSetAngle0, fTopSetAngle0);
    QPointF ptTopRightSetAngle0(fRightSetAngle0, fTopSetAngle0);
    QPointF ptBottomRightSetAngle0(fRightSetAngle0, fBottomSetAngle0);
    QPointF ptBottomLeftSetAngle0(fLeftSetAngle0, fBottomSetAngle0);
    QPointF ptTopCenterSetAngle0(fxCenterSetAngle0, fTopSetAngle0);
    QPointF ptRightCenterSetAngle0(fRightSetAngle0, fyCenterSetAngle0);
    QPointF ptBottomCenterSetAngle0(fxCenterSetAngle0, fBottomSetAngle0);
    QPointF ptLeftCenterSetAngle0(fLeftSetAngle0, fyCenterSetAngle0);
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setCenter(400, 400)
    ----------------------
        200   250   300   350   400                BottomUp
     250 +-----+-----+-----+-----+                   350
         |                       |
     300 +           X           +                   300
         |                       |
     350 +-----+-----+-----+-----+-----+-----+       250
                     |                       |
      400      =>    x           X           x       200
                     |                       |
      450            +-----+-----x-----+-----+       150
    */
    QPointF ptCenter(400.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptCenter.setY(200.0);
    }
    double fdx = ptCenter.x() - ptCenterSetAngle0.x();
    double fdy = ptCenter.y() - ptCenterSetAngle0.y();
    double fWidthSetCenterAngle0 = fWidthSetAngle0;
    double fHeightSetCenterAngle0 = fHeightSetAngle0;
    QPointF ptCenterSetCenterAngle0 = ptCenter;
    QPointF ptTopLeftSetCenterAngle0(ptTopLeftSetAngle0.x() + fdx, ptTopLeftSetAngle0.y() + fdy);
    QPointF ptTopRightSetCenterAngle0(ptTopRightSetAngle0.x() + fdx, ptTopRightSetAngle0.y() + fdy);
    QPointF ptBottomRightSetCenterAngle0(ptBottomRightSetAngle0.x() + fdx, ptBottomRightSetAngle0.y() + fdy);
    QPointF ptBottomLeftSetCenterAngle0(ptBottomLeftSetAngle0.x() + fdx, ptBottomLeftSetAngle0.y() + fdy);
    QPointF ptTopCenterSetCenterAngle0(ptTopCenterSetAngle0.x() + fdx, ptTopCenterSetAngle0.y() + fdy);
    QPointF ptRightCenterSetCenterAngle0(ptRightCenterSetAngle0.x() + fdx, ptRightCenterSetAngle0.y() + fdy);
    QPointF ptBottomCenterSetCenterAngle0(ptBottomCenterSetAngle0.x() + fdx, ptBottomCenterSetAngle0.y() + fdy);
    QPointF ptLeftCenterSetCenterAngle0(ptLeftCenterSetAngle0.x() + fdx, ptLeftCenterSetAngle0.y() + fdy);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setCenter(" + qPoint2Str(ptCenter) + " " + unit.symbol() + ")",
        /* strOperation    */ "setCenter(" + qPoint2Str(ptCenter) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setCenter", ptCenter);
    pTestStep->setConfigValue("setCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetCenterAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setSize(300, 200)
    --------------------
        200   250   300   350   400          200   250   300   350   400   450   500  BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+-----x-----+-----+-----+      350
         |                       |            |                                   |
     300 +           X           +        300 +                                   +      300
         |                       |            |                                   |
     350 +-----+-----+-----+-----+   =>   350 x                 X                 x      250
                                              |                                   |
                                          400 +                                   +      200
                                              |                                   |
                                          450 +-----+-----+-----x-----+-----+-----+      150
    */
    QSizeF size300x200(300.0, 200.0);
    double fxCenterSetValidSizeAngle0 = fLeftSetAngle0 + size300x200.width() / 2.0;
    double fyCenterSetValidSizeAngle0 = fTopSetAngle0 + size300x200.height() / 2.0;
    double fWidthSetValidSizeAngle0 = size300x200.width();
    double fHeightSetValidSizeAngle0 = size300x200.height();
    double fLeftSetValidSizeAngle0 = fxCenterSetValidSizeAngle0 - fWidthSetValidSizeAngle0 / 2.0;
    double fRightSetValidSizeAngle0 = fxCenterSetValidSizeAngle0 + fWidthSetValidSizeAngle0 / 2.0;
    double fTopSetValidSizeAngle0 = fyCenterSetValidSizeAngle0 - fHeightSetValidSizeAngle0 / 2.0;
    double fBottomSetValidSizeAngle0 = fyCenterSetValidSizeAngle0 + fHeightSetValidSizeAngle0 / 2.0;
    QPointF ptCenterSetValidSizeAngle0(fxCenterSetValidSizeAngle0, fyCenterSetValidSizeAngle0);
    QPointF ptTopLeftSetValidSizeAngle0(fLeftSetValidSizeAngle0, fTopSetValidSizeAngle0);
    QPointF ptTopRightSetValidSizeAngle0(fRightSetValidSizeAngle0, fTopSetValidSizeAngle0);
    QPointF ptBottomRightSetValidSizeAngle0(fRightSetValidSizeAngle0, fBottomSetValidSizeAngle0);
    QPointF ptBottomLeftSetValidSizeAngle0(fLeftSetValidSizeAngle0, fBottomSetValidSizeAngle0);
    QPointF ptTopCenterSetValidSizeAngle0(fxCenterSetValidSizeAngle0, fTopSetValidSizeAngle0);
    QPointF ptRightCenterSetValidSizeAngle0(fRightSetValidSizeAngle0, fyCenterSetValidSizeAngle0);
    QPointF ptBottomCenterSetValidSizeAngle0(fxCenterSetValidSizeAngle0, fBottomSetValidSizeAngle0);
    QPointF ptLeftCenterSetValidSizeAngle0(fLeftSetValidSizeAngle0, fyCenterSetValidSizeAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setSize(" + qSize2Str(size300x200) + " " + unit.symbol() + ")",
        /* strOperation    */ "setSize(" + qSize2Str(size300x200) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setSize", size300x200);
    pTestStep->setConfigValue("setSize.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidSizeAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidSizeAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setSize(-100, -50)
    ---------------------
        000   050   100   150   200   250   300   350   400   BottomUp
    200             BR-----+-----BL                              400
                     |     X     |          <=
    250             TR-----+-----TL----+-----+-----+-----TR      350
                                 |                       |
    300                          +           X           +       300
                                 |                       |
    350                         BL-----+-----+-----+-----BR      250
    */
    QSize size_100x_50(-100.0, -50.0);
    double fxCenterSetInvalidSizeAngle0 = fLeftSetAngle0 + size_100x_50.width() / 2.0;
    double fyCenterSetInvalidSizeAngle0 = fTopSetAngle0 + size_100x_50.height() / 2.0;
    double fWidthSetInvalidSizeAngle0 = size_100x_50.width();
    double fHeightSetInvalidSizeAngle0 = size_100x_50.height();
    double fLeftSetInvalidSizeAngle0 = fxCenterSetInvalidSizeAngle0 - fWidthSetInvalidSizeAngle0 / 2.0;
    double fRightSetInvalidSizeAngle0 = fxCenterSetInvalidSizeAngle0 + fWidthSetInvalidSizeAngle0 / 2.0;
    double fTopSetInvalidSizeAngle0 = fyCenterSetInvalidSizeAngle0 - fHeightSetInvalidSizeAngle0 / 2.0;
    double fBottomSetInvalidSizeAngle0 = fyCenterSetInvalidSizeAngle0 + fHeightSetInvalidSizeAngle0 / 2.0;
    QPointF ptCenterSetInvalidSizeAngle0(fxCenterSetInvalidSizeAngle0, fyCenterSetInvalidSizeAngle0);
    QPointF ptTopLeftSetInvalidSizeAngle0(fLeftSetInvalidSizeAngle0, fTopSetInvalidSizeAngle0);
    QPointF ptTopRightSetInvalidSizeAngle0(fRightSetInvalidSizeAngle0, fTopSetInvalidSizeAngle0);
    QPointF ptBottomRightSetInvalidSizeAngle0(fRightSetInvalidSizeAngle0, fBottomSetInvalidSizeAngle0);
    QPointF ptBottomLeftSetInvalidSizeAngle0(fLeftSetInvalidSizeAngle0, fBottomSetInvalidSizeAngle0);
    QPointF ptTopCenterSetInvalidSizeAngle0(fxCenterSetInvalidSizeAngle0, fTopSetInvalidSizeAngle0);
    QPointF ptRightCenterSetInvalidSizeAngle0(fRightSetInvalidSizeAngle0, fyCenterSetInvalidSizeAngle0);
    QPointF ptBottomCenterSetInvalidSizeAngle0(fxCenterSetInvalidSizeAngle0, fBottomSetInvalidSizeAngle0);
    QPointF ptLeftCenterSetInvalidSizeAngle0(fLeftSetInvalidSizeAngle0, fyCenterSetInvalidSizeAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setSize(" + qSize2Str(size_100x_50) + " " + unit.symbol() + ")",
        /* strOperation    */ "setSize(" + qSize2Str(size_100x_50) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setSize", size_100x_50);
    pTestStep->setConfigValue("setSize.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidSizeAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidSizeAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidth(300)
    ----------------
        200   250   300   350   400          200   250   300   350   400   450   500    BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+-----+-----+-----+-----+       350
         |                       |            |                                   |
     300 +           X           +   =>   300 x                 X                 x       300
         |                       |            |                                   |
     350 +-----+-----+-----+-----+        350 +-----+-----+-----+-----+-----+-----+       250
    */
    CPhysVal physValWidth300(300.0, unit, i_drawingSize.imageCoorsResolution());
    double fxCenterSetValidWidthAngle0 = fLeftSetAngle0 + physValWidth300.getVal() / 2.0;
    double fyCenterSetValidWidthAngle0 = fyCenterSetAngle0;
    double fWidthSetValidWidthAngle0 = physValWidth300.getVal();
    double fHeightSetValidWidthAngle0 = fHeightSetAngle0;
    double fLeftSetValidWidthAngle0 = fxCenterSetValidWidthAngle0 - fWidthSetValidWidthAngle0 / 2.0;
    double fRightSetValidWidthAngle0 = fxCenterSetValidWidthAngle0 + fWidthSetValidWidthAngle0 / 2.0;
    double fTopSetValidWidthAngle0 = fyCenterSetValidWidthAngle0 - fHeightSetValidWidthAngle0 / 2.0;
    double fBottomSetValidWidthAngle0 = fyCenterSetValidWidthAngle0 + fHeightSetValidWidthAngle0 / 2.0;
    QPointF ptCenterSetValidWidthAngle0(fxCenterSetValidWidthAngle0, fyCenterSetValidWidthAngle0);
    QPointF ptTopLeftSetValidWidthAngle0(fLeftSetValidWidthAngle0, fTopSetValidWidthAngle0);
    QPointF ptTopRightSetValidWidthAngle0(fRightSetValidWidthAngle0, fTopSetValidWidthAngle0);
    QPointF ptBottomRightSetValidWidthAngle0(fRightSetValidWidthAngle0, fBottomSetValidWidthAngle0);
    QPointF ptBottomLeftSetValidWidthAngle0(fLeftSetValidWidthAngle0, fBottomSetValidWidthAngle0);
    QPointF ptTopCenterSetValidWidthAngle0(fxCenterSetValidWidthAngle0, fTopSetValidWidthAngle0);
    QPointF ptRightCenterSetValidWidthAngle0(fRightSetValidWidthAngle0, fyCenterSetValidWidthAngle0);
    QPointF ptBottomCenterSetValidWidthAngle0(fxCenterSetValidWidthAngle0, fBottomSetValidWidthAngle0);
    QPointF ptLeftCenterSetValidWidthAngle0(fLeftSetValidWidthAngle0, fyCenterSetValidWidthAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidth(" + physValWidth300.toString() + ")",
        /* strOperation    */ "setWidth(" + physValWidth300.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidth", physValWidth300.toString());
    pTestStep->setConfigValue("setWidth.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidWidthAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidWidthAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidWidthAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidWidthAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidWidthAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidWidthAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidWidthAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidWidthAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidWidthAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidWidthAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidWidthAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidWidthAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidWidthAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidWidthAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidth(-100)
    -----------------
        200   250   300   350   400          100   150   200    BottomUp
     250 TL----+-----+-----+----TR        250 TR----+----TL       350
         |                       |            |           |
     300 +           X           +   =>   300 x     X     x       300
         |                       |            |           |
     350 BL----+-----+-----+----BR        350 BR----+----BL       250
    */
    CPhysVal physValWidth_100(-100.0, unit, i_drawingSize.imageCoorsResolution());
    double fxCenterSetInvalidWidthAngle0 = fLeftSetAngle0 + physValWidth_100.getVal() / 2.0;
    double fyCenterSetInvalidWidthAngle0 = fyCenterSetAngle0;
    double fWidthSetInvalidWidthAngle0 = physValWidth_100.getVal();
    double fHeightSetInvalidWidthAngle0 = fHeightSetAngle0;
    double fLeftSetInvalidWidthAngle0 = fxCenterSetInvalidWidthAngle0 - fWidthSetInvalidWidthAngle0 / 2.0;
    double fRightSetInvalidWidthAngle0 = fxCenterSetInvalidWidthAngle0 + fWidthSetInvalidWidthAngle0 / 2.0;
    double fTopSetInvalidWidthAngle0 = fyCenterSetInvalidWidthAngle0 - fHeightSetInvalidWidthAngle0 / 2.0;
    double fBottomSetInvalidWidthAngle0 = fyCenterSetInvalidWidthAngle0 + fHeightSetInvalidWidthAngle0 / 2.0;
    QPointF ptCenterSetInvalidWidthAngle0(fxCenterSetInvalidWidthAngle0, fyCenterSetInvalidWidthAngle0);
    QPointF ptTopLeftSetInvalidWidthAngle0(fLeftSetInvalidWidthAngle0, fTopSetInvalidWidthAngle0);
    QPointF ptTopRightSetInvalidWidthAngle0(fRightSetInvalidWidthAngle0, fTopSetInvalidWidthAngle0);
    QPointF ptBottomRightSetInvalidWidthAngle0(fRightSetInvalidWidthAngle0, fBottomSetInvalidWidthAngle0);
    QPointF ptBottomLeftSetInvalidWidthAngle0(fLeftSetInvalidWidthAngle0, fBottomSetInvalidWidthAngle0);
    QPointF ptTopCenterSetInvalidWidthAngle0(fxCenterSetInvalidWidthAngle0, fTopSetInvalidWidthAngle0);
    QPointF ptRightCenterSetInvalidWidthAngle0(fRightSetInvalidWidthAngle0, fyCenterSetInvalidWidthAngle0);
    QPointF ptBottomCenterSetInvalidWidthAngle0(fxCenterSetInvalidWidthAngle0, fBottomSetInvalidWidthAngle0);
    QPointF ptLeftCenterSetInvalidWidthAngle0(fLeftSetInvalidWidthAngle0, fyCenterSetInvalidWidthAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidth(" + physValWidth_100.toString() + ")",
        /* strOperation    */ "setWidth(" + physValWidth_100.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidth", physValWidth_100.toString());
    pTestStep->setConfigValue("setWidth.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidWidthAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidWidthAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidWidthAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidWidthAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidWidthAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidWidthAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidWidthAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidWidthAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidWidthAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidWidthAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidWidthAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidWidthAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidWidthAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidWidthAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingLeftCenter(300, 250)
    ---------------------------------------
        200   250   300   350   400          300   350   400  BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 +-----+-----+-----+-----+        350 +-----+-----+      250
    */
    QPointF ptMoved = ptTopCenterSetAngle0;
    double fxCenterSetValidWidthMoveLeftCenterAngle0 = fRightSetAngle0 - (fRightSetAngle0 - ptMoved.x()) / 2.0;
    double fyCenterSetValidWidthMoveLeftCenterAngle0 = fyCenterSetAngle0;
    double fWidthSetValidWidthMoveLeftCenterAngle0 = fRightSetAngle0 - ptMoved.x();
    double fHeightSetValidWidthMoveLeftCenterAngle0 = fHeightSetAngle0;
    double fLeftSetValidWidthMoveLeftCenterAngle0 = fxCenterSetValidWidthMoveLeftCenterAngle0 - fWidthSetValidWidthMoveLeftCenterAngle0 / 2.0;
    double fRightSetValidWidthMoveLeftCenterAngle0 = fxCenterSetValidWidthMoveLeftCenterAngle0 + fWidthSetValidWidthMoveLeftCenterAngle0 / 2.0;
    double fTopSetValidWidthMoveLeftCenterAngle0 = fyCenterSetValidWidthMoveLeftCenterAngle0 - fHeightSetValidWidthMoveLeftCenterAngle0 / 2.0;
    double fBottomSetValidWidthMoveLeftCenterAngle0 = fyCenterSetValidWidthMoveLeftCenterAngle0 + fHeightSetValidWidthMoveLeftCenterAngle0 / 2.0;
    QPointF ptCenterSetValidWidthMoveLeftCenterAngle0(fxCenterSetValidWidthMoveLeftCenterAngle0, fyCenterSetValidWidthMoveLeftCenterAngle0);
    QPointF ptTopLeftSetValidWidthMoveLeftCenterAngle0(fLeftSetValidWidthMoveLeftCenterAngle0, fTopSetValidWidthMoveLeftCenterAngle0);
    QPointF ptTopRightSetValidWidthMoveLeftCenterAngle0(fRightSetValidWidthMoveLeftCenterAngle0, fTopSetValidWidthMoveLeftCenterAngle0);
    QPointF ptBottomRightSetValidWidthMoveLeftCenterAngle0(fRightSetValidWidthMoveLeftCenterAngle0, fBottomSetValidWidthMoveLeftCenterAngle0);
    QPointF ptBottomLeftSetValidWidthMoveLeftCenterAngle0(fLeftSetValidWidthMoveLeftCenterAngle0, fBottomSetValidWidthMoveLeftCenterAngle0);
    QPointF ptTopCenterSetValidWidthMoveLeftCenterAngle0(fxCenterSetValidWidthMoveLeftCenterAngle0, fTopSetValidWidthMoveLeftCenterAngle0);
    QPointF ptRightCenterSetValidWidthMoveLeftCenterAngle0(fRightSetValidWidthMoveLeftCenterAngle0, fyCenterSetValidWidthMoveLeftCenterAngle0);
    QPointF ptBottomCenterSetValidWidthMoveLeftCenterAngle0(fxCenterSetValidWidthMoveLeftCenterAngle0, fBottomSetValidWidthMoveLeftCenterAngle0);
    QPointF ptLeftCenterSetValidWidthMoveLeftCenterAngle0(fLeftSetValidWidthMoveLeftCenterAngle0, fyCenterSetValidWidthMoveLeftCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidWidthMoveLeftCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidWidthMoveLeftCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidWidthMoveLeftCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidWidthMoveLeftCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidWidthMoveLeftCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingLeftCenter(500, 250)
    ---------------------------------------
        200   250   300   350   400          400   450   500  BottomUp
     250 TL----+-----+-----+----TR        250 TR----+----TL      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 BL----+-----+-----+----BR        350 BR----+----BL      250
    */
    ptMoved = QPointF(500, 250);
    double fxCenterSetInvalidWidthMoveLeftCenterAngle0 = fRightSetAngle0 + (ptMoved.x()- fRightSetAngle0) / 2.0;
    double fyCenterSetInvalidWidthMoveLeftCenterAngle0 = fyCenterSetAngle0;
    double fWidthSetInvalidWidthMoveLeftCenterAngle0 = fRightSetAngle0 - ptMoved.x();
    double fHeightSetInvalidWidthMoveLeftCenterAngle0 = fHeightSetAngle0;
    double fLeftSetInvalidWidthMoveLeftCenterAngle0 = fxCenterSetInvalidWidthMoveLeftCenterAngle0 - fWidthSetInvalidWidthMoveLeftCenterAngle0 / 2.0;
    double fRightSetInvalidWidthMoveLeftCenterAngle0 = fxCenterSetInvalidWidthMoveLeftCenterAngle0 + fWidthSetInvalidWidthMoveLeftCenterAngle0 / 2.0;
    double fTopSetInvalidWidthMoveLeftCenterAngle0 = fyCenterSetInvalidWidthMoveLeftCenterAngle0 - fHeightSetInvalidWidthMoveLeftCenterAngle0 / 2.0;
    double fBottomSetInvalidWidthMoveLeftCenterAngle0 = fyCenterSetInvalidWidthMoveLeftCenterAngle0 + fHeightSetInvalidWidthMoveLeftCenterAngle0 / 2.0;
    QPointF ptCenterSetInvalidWidthMoveLeftCenterAngle0(fxCenterSetInvalidWidthMoveLeftCenterAngle0, fyCenterSetInvalidWidthMoveLeftCenterAngle0);
    QPointF ptTopLeftSetInvalidWidthMoveLeftCenterAngle0(fLeftSetInvalidWidthMoveLeftCenterAngle0, fTopSetInvalidWidthMoveLeftCenterAngle0);
    QPointF ptTopRightSetInvalidWidthMoveLeftCenterAngle0(fRightSetInvalidWidthMoveLeftCenterAngle0, fTopSetInvalidWidthMoveLeftCenterAngle0);
    QPointF ptBottomRightSetInvalidWidthMoveLeftCenterAngle0(fRightSetInvalidWidthMoveLeftCenterAngle0, fBottomSetInvalidWidthMoveLeftCenterAngle0);
    QPointF ptBottomLeftSetInvalidWidthMoveLeftCenterAngle0(fLeftSetInvalidWidthMoveLeftCenterAngle0, fBottomSetInvalidWidthMoveLeftCenterAngle0);
    QPointF ptTopCenterSetInvalidWidthMoveLeftCenterAngle0(fxCenterSetInvalidWidthMoveLeftCenterAngle0, fTopSetInvalidWidthMoveLeftCenterAngle0);
    QPointF ptRightCenterSetInvalidWidthMoveLeftCenterAngle0(fRightSetInvalidWidthMoveLeftCenterAngle0, fyCenterSetInvalidWidthMoveLeftCenterAngle0);
    QPointF ptBottomCenterSetInvalidWidthMoveLeftCenterAngle0(fxCenterSetInvalidWidthMoveLeftCenterAngle0, fBottomSetInvalidWidthMoveLeftCenterAngle0);
    QPointF ptLeftCenterSetInvalidWidthMoveLeftCenterAngle0(fLeftSetInvalidWidthMoveLeftCenterAngle0, fyCenterSetInvalidWidthMoveLeftCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidWidthMoveLeftCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidWidthMoveLeftCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidWidthMoveLeftCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidWidthMoveLeftCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidWidthMoveLeftCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingRightCenter(300, 250)
    ----------------------------------------
        200   250   300   350   400          200   250   300  BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 +-----+-----+-----+-----+        350 +-----+-----+      250
    */
    ptMoved = ptTopCenterSetAngle0;
    double fxCenterSetValidWidthMoveRightCenterAngle0 = fLeftSetAngle0 + (ptMoved.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetValidWidthMoveRightCenterAngle0 = fyCenterSetAngle0;
    double fWidthSetValidWidthMoveRightCenterAngle0 = ptMoved.x() - fLeftSetAngle0;
    double fHeightSetValidWidthMoveRightCenterAngle0 = fHeightSetAngle0;
    double fLeftSetValidWidthMoveRightCenterAngle0 = fxCenterSetValidWidthMoveRightCenterAngle0 - fWidthSetValidWidthMoveRightCenterAngle0 / 2.0;
    double fRightSetValidWidthMoveRightCenterAngle0 = fxCenterSetValidWidthMoveRightCenterAngle0 + fWidthSetValidWidthMoveRightCenterAngle0 / 2.0;
    double fTopSetValidWidthMoveRightCenterAngle0 = fyCenterSetValidWidthMoveRightCenterAngle0 - fHeightSetValidWidthMoveRightCenterAngle0 / 2.0;
    double fBottomSetValidWidthMoveRightCenterAngle0 = fyCenterSetValidWidthMoveRightCenterAngle0 + fHeightSetValidWidthMoveRightCenterAngle0 / 2.0;
    QPointF ptCenterSetValidWidthMoveRightCenterAngle0(fxCenterSetValidWidthMoveRightCenterAngle0, fyCenterSetValidWidthMoveRightCenterAngle0);
    QPointF ptTopLeftSetValidWidthMoveRightCenterAngle0(fLeftSetValidWidthMoveRightCenterAngle0, fTopSetValidWidthMoveRightCenterAngle0);
    QPointF ptTopRightSetValidWidthMoveRightCenterAngle0(fRightSetValidWidthMoveRightCenterAngle0, fTopSetValidWidthMoveRightCenterAngle0);
    QPointF ptBottomRightSetValidWidthMoveRightCenterAngle0(fRightSetValidWidthMoveRightCenterAngle0, fBottomSetValidWidthMoveRightCenterAngle0);
    QPointF ptBottomLeftSetValidWidthMoveRightCenterAngle0(fLeftSetValidWidthMoveRightCenterAngle0, fBottomSetValidWidthMoveRightCenterAngle0);
    QPointF ptTopCenterSetValidWidthMoveRightCenterAngle0(fxCenterSetValidWidthMoveRightCenterAngle0, fTopSetValidWidthMoveRightCenterAngle0);
    QPointF ptRightCenterSetValidWidthMoveRightCenterAngle0(fRightSetValidWidthMoveRightCenterAngle0, fyCenterSetValidWidthMoveRightCenterAngle0);
    QPointF ptBottomCenterSetValidWidthMoveRightCenterAngle0(fxCenterSetValidWidthMoveRightCenterAngle0, fBottomSetValidWidthMoveRightCenterAngle0);
    QPointF ptLeftCenterSetValidWidthMoveRightCenterAngle0(fLeftSetValidWidthMoveRightCenterAngle0, fyCenterSetValidWidthMoveRightCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidWidthMoveRightCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidWidthMoveRightCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidWidthMoveRightCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidWidthMoveRightCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidWidthMoveRightCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingRightCenter(100, 250)
    ----------------------------------------
        200   250   300   350   400          100   150   200  BottomUp
     250 TL----+-----+-----+----TR        250 TR----+----TL      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 BL----+-----+-----+----BR        350 BR----+----BL      250
    */
    ptMoved = QPointF(100.0, 250.0);
    double fxCenterSetInvalidWidthMoveRightCenterAngle0 = fLeftSetAngle0 + (ptMoved.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetInvalidWidthMoveRightCenterAngle0 = fyCenterSetAngle0;
    double fWidthSetInvalidWidthMoveRightCenterAngle0 = ptMoved.x() - fLeftSetAngle0;
    double fHeightSetInvalidWidthMoveRightCenterAngle0 = fHeightSetAngle0;
    double fLeftSetInvalidWidthMoveRightCenterAngle0 = fxCenterSetInvalidWidthMoveRightCenterAngle0 - fWidthSetInvalidWidthMoveRightCenterAngle0 / 2.0;
    double fRightSetInvalidWidthMoveRightCenterAngle0 = fxCenterSetInvalidWidthMoveRightCenterAngle0 + fWidthSetInvalidWidthMoveRightCenterAngle0 / 2.0;
    double fTopSetInvalidWidthMoveRightCenterAngle0 = fyCenterSetInvalidWidthMoveRightCenterAngle0 - fHeightSetInvalidWidthMoveRightCenterAngle0 / 2.0;
    double fBottomSetInvalidWidthMoveRightCenterAngle0 = fyCenterSetInvalidWidthMoveRightCenterAngle0 + fHeightSetInvalidWidthMoveRightCenterAngle0 / 2.0;
    QPointF ptCenterSetInvalidWidthMoveRightCenterAngle0(fxCenterSetInvalidWidthMoveRightCenterAngle0, fyCenterSetInvalidWidthMoveRightCenterAngle0);
    QPointF ptTopLeftSetInvalidWidthMoveRightCenterAngle0(fLeftSetInvalidWidthMoveRightCenterAngle0, fTopSetInvalidWidthMoveRightCenterAngle0);
    QPointF ptTopRightSetInvalidWidthMoveRightCenterAngle0(fRightSetInvalidWidthMoveRightCenterAngle0, fTopSetInvalidWidthMoveRightCenterAngle0);
    QPointF ptBottomRightSetInvalidWidthMoveRightCenterAngle0(fRightSetInvalidWidthMoveRightCenterAngle0, fBottomSetInvalidWidthMoveRightCenterAngle0);
    QPointF ptBottomLeftSetInvalidWidthMoveRightCenterAngle0(fLeftSetInvalidWidthMoveRightCenterAngle0, fBottomSetInvalidWidthMoveRightCenterAngle0);
    QPointF ptTopCenterSetInvalidWidthMoveRightCenterAngle0(fxCenterSetInvalidWidthMoveRightCenterAngle0, fTopSetInvalidWidthMoveRightCenterAngle0);
    QPointF ptRightCenterSetInvalidWidthMoveRightCenterAngle0(fRightSetInvalidWidthMoveRightCenterAngle0, fyCenterSetInvalidWidthMoveRightCenterAngle0);
    QPointF ptBottomCenterSetInvalidWidthMoveRightCenterAngle0(fxCenterSetInvalidWidthMoveRightCenterAngle0, fBottomSetInvalidWidthMoveRightCenterAngle0);
    QPointF ptLeftCenterSetInvalidWidthMoveRightCenterAngle0(fLeftSetInvalidWidthMoveRightCenterAngle0, fyCenterSetInvalidWidthMoveRightCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidWidthMoveRightCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidWidthMoveRightCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidWidthMoveRightCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidWidthMoveRightCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidWidthMoveRightCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeight(200)
    -----------------
        200   250   300   350   400         200   250   300   350   400   BottomUp
     250 +-----+-----+-----+-----+       250 +-----+-----x-----+-----+      350
         |                       |           |                       |
     300 +           X           +       300 +                       +      300
         |                       |           |                       |
     350 +-----+-----+-----+-----+   =>  350 x           X           x      250
                                             |                       |
                                         400 +                       +      200
                                             |                       |
                                         450 +-----+-----x-----+-----+      150
    */
    CPhysVal physValHeight200(200.0, i_drawingSize.unit(), i_drawingSize.imageCoorsResolution());
    double fxCenterSetValidHeightAngle0 = fxCenterSetAngle0;
    double fyCenterSetValidHeightAngle0 = fyCenterSetAngle0 + (physValHeight200.getVal() - fHeightSetAngle0) / 2.0;
    double fWidthSetValidHeightAngle0 = fWidthSetAngle0;
    double fHeightSetValidHeightAngle0 = physValHeight200.getVal();
    double fLeftSetValidHeightAngle0 = fxCenterSetValidHeightAngle0 - fWidthSetValidHeightAngle0 / 2.0;
    double fRightSetValidHeightAngle0 = fxCenterSetValidHeightAngle0 + fWidthSetValidHeightAngle0 / 2.0;
    double fTopSetValidHeightAngle0 = fyCenterSetValidHeightAngle0 - fHeightSetValidHeightAngle0 / 2.0;
    double fBottomSetValidHeightAngle0 = fyCenterSetValidHeightAngle0 + fHeightSetValidHeightAngle0 / 2.0;
    QPointF ptCenterSetValidHeightAngle0(fxCenterSetValidHeightAngle0, fyCenterSetValidHeightAngle0);
    QPointF ptTopLeftSetValidHeightAngle0(fLeftSetValidHeightAngle0, fTopSetValidHeightAngle0);
    QPointF ptTopRightSetValidHeightAngle0(fRightSetValidHeightAngle0, fTopSetValidHeightAngle0);
    QPointF ptBottomRightSetValidHeightAngle0(fRightSetValidHeightAngle0, fBottomSetValidHeightAngle0);
    QPointF ptBottomLeftSetValidHeightAngle0(fLeftSetValidHeightAngle0, fBottomSetValidHeightAngle0);
    QPointF ptTopCenterSetValidHeightAngle0(fxCenterSetValidHeightAngle0, fTopSetValidHeightAngle0);
    QPointF ptRightCenterSetValidHeightAngle0(fRightSetValidHeightAngle0, fyCenterSetValidHeightAngle0);
    QPointF ptBottomCenterSetValidHeightAngle0(fxCenterSetValidHeightAngle0, fBottomSetValidHeightAngle0);
    QPointF ptLeftCenterSetValidHeightAngle0(fLeftSetValidHeightAngle0, fyCenterSetValidHeightAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeight(" + physValHeight200.toString() + ")",
        /* strOperation    */ "setHeight(" + physValHeight200.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeight", physValHeight200.toString());
    pTestStep->setConfigValue("setHeight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidHeightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidHeightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidHeightAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidHeightAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidHeightAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidHeightAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidHeightAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidHeightAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidHeightAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidHeightAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidHeightAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidHeightAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidHeightAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidHeightAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeight(-100)
    -----------------
        200   250   300   350   400         200   250   300   350   400   BottomUp
                                         150 +-----+-----x-----+-----+      450
                                             |                       |
                                         200 |           X           |      400
                                             |                       |
     250 +-----+-----+-----+-----+       250 +-----+-----x-----+-----+      350
         |                       |
     300 +           X           +
         |                       |
     350 +-----+-----+-----+-----+
    */
    CPhysVal physValHeight_100(-100.0, i_drawingSize.unit(), i_drawingSize.imageCoorsResolution());
    double fxCenterSetInvalidHeightAngle0 = fxCenterSetAngle0;
    double fyCenterSetInvalidHeightAngle0 = fyCenterSetAngle0 + (physValHeight_100.getVal() - fHeightSetAngle0) / 2.0;
    double fWidthSetInvalidHeightAngle0 = fWidthSetAngle0;
    double fHeightSetInvalidHeightAngle0 = physValHeight_100.getVal();
    double fLeftSetInvalidHeightAngle0 = fxCenterSetInvalidHeightAngle0 - fWidthSetInvalidHeightAngle0 / 2.0;
    double fRightSetInvalidHeightAngle0 = fxCenterSetInvalidHeightAngle0 + fWidthSetInvalidHeightAngle0 / 2.0;
    double fTopSetInvalidHeightAngle0 = fyCenterSetInvalidHeightAngle0 - fHeightSetInvalidHeightAngle0 / 2.0;
    double fBottomSetInvalidHeightAngle0 = fyCenterSetInvalidHeightAngle0 + fHeightSetInvalidHeightAngle0 / 2.0;
    QPointF ptCenterSetInvalidHeightAngle0(fxCenterSetInvalidHeightAngle0, fyCenterSetInvalidHeightAngle0);
    QPointF ptTopLeftSetInvalidHeightAngle0(fLeftSetInvalidHeightAngle0, fTopSetInvalidHeightAngle0);
    QPointF ptTopRightSetInvalidHeightAngle0(fRightSetInvalidHeightAngle0, fTopSetInvalidHeightAngle0);
    QPointF ptBottomRightSetInvalidHeightAngle0(fRightSetInvalidHeightAngle0, fBottomSetInvalidHeightAngle0);
    QPointF ptBottomLeftSetInvalidHeightAngle0(fLeftSetInvalidHeightAngle0, fBottomSetInvalidHeightAngle0);
    QPointF ptTopCenterSetInvalidHeightAngle0(fxCenterSetInvalidHeightAngle0, fTopSetInvalidHeightAngle0);
    QPointF ptRightCenterSetInvalidHeightAngle0(fRightSetInvalidHeightAngle0, fyCenterSetInvalidHeightAngle0);
    QPointF ptBottomCenterSetInvalidHeightAngle0(fxCenterSetInvalidHeightAngle0, fBottomSetInvalidHeightAngle0);
    QPointF ptLeftCenterSetInvalidHeightAngle0(fLeftSetInvalidHeightAngle0, fyCenterSetInvalidHeightAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeight(" + physValHeight_100.toString() + ")",
        /* strOperation    */ "setHeight(" + physValHeight_100.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeight", physValHeight_100.toString());
    pTestStep->setConfigValue("setHeight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidHeightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidHeightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidHeightAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidHeightAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidHeightAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidHeightAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidHeightAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidHeightAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidHeightAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidHeightAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidHeightAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidHeightAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidHeightAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidHeightAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingTopCenter(400, 300)
    ---------------------------------------
        200   250   300   350   400          200   250   300   350   400   BottomUp
     250 +-----+-----+-----+-----+        250                                350
         |                       |
     300 +           X           +        300 +-----+-----+-----+-----+      300
         |                       |   =>       |           X           |
     350 +-----+-----+-----+-----+        350 +-----+-----+-----+-----+
    */
    ptMoved = ptRightCenterSetAngle0;
    double fxCenterSetValidHeightMoveTopCenterAngle0 = fxCenterSetAngle0;
    double fyCenterSetValidHeightMoveTopCenterAngle0 = fBottomSetAngle0 - (fBottomSetAngle0 - ptMoved.y()) / 2.0;
    double fWidthSetValidHeightMoveTopCenterAngle0 = fWidthSetAngle0;
    double fHeightSetValidHeightMoveTopCenterAngle0 = fBottomSetAngle0 - ptMoved.y();
    double fLeftSetValidHeightMoveTopCenterAngle0 = fxCenterSetValidHeightMoveTopCenterAngle0 - fWidthSetValidHeightMoveTopCenterAngle0 / 2.0;
    double fRightSetValidHeightMoveTopCenterAngle0 = fxCenterSetValidHeightMoveTopCenterAngle0 + fWidthSetValidHeightMoveTopCenterAngle0 / 2.0;
    double fTopSetValidHeightMoveTopCenterAngle0 = fyCenterSetValidHeightMoveTopCenterAngle0 - fHeightSetValidHeightMoveTopCenterAngle0 / 2.0;
    double fBottomSetValidHeightMoveTopCenterAngle0 = fyCenterSetValidHeightMoveTopCenterAngle0 + fHeightSetValidHeightMoveTopCenterAngle0 / 2.0;
    QPointF ptCenterSetValidHeightMoveTopCenterAngle0(fxCenterSetValidHeightMoveTopCenterAngle0, fyCenterSetValidHeightMoveTopCenterAngle0);
    QPointF ptTopLeftSetValidHeightMoveTopCenterAngle0(fLeftSetValidHeightMoveTopCenterAngle0, fTopSetValidHeightMoveTopCenterAngle0);
    QPointF ptTopRightSetValidHeightMoveTopCenterAngle0(fRightSetValidHeightMoveTopCenterAngle0, fTopSetValidHeightMoveTopCenterAngle0);
    QPointF ptBottomRightSetValidHeightMoveTopCenterAngle0(fRightSetValidHeightMoveTopCenterAngle0, fBottomSetValidHeightMoveTopCenterAngle0);
    QPointF ptBottomLeftSetValidHeightMoveTopCenterAngle0(fLeftSetValidHeightMoveTopCenterAngle0, fBottomSetValidHeightMoveTopCenterAngle0);
    QPointF ptTopCenterSetValidHeightMoveTopCenterAngle0(fxCenterSetValidHeightMoveTopCenterAngle0, fTopSetValidHeightMoveTopCenterAngle0);
    QPointF ptRightCenterSetValidHeightMoveTopCenterAngle0(fRightSetValidHeightMoveTopCenterAngle0, fyCenterSetValidHeightMoveTopCenterAngle0);
    QPointF ptBottomCenterSetValidHeightMoveTopCenterAngle0(fxCenterSetValidHeightMoveTopCenterAngle0, fBottomSetValidHeightMoveTopCenterAngle0);
    QPointF ptLeftCenterSetValidHeightMoveTopCenterAngle0(fLeftSetValidHeightMoveTopCenterAngle0, fyCenterSetValidHeightMoveTopCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidHeightMoveTopCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidHeightMoveTopCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidHeightMoveTopCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidHeightMoveTopCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidHeightMoveTopCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingTopCenter(400, 400)
    ---------------------------------------
        200   250   300   350   400          200   250   300   350   400   BottomUp
     250 TL----+-----+-----+----TR                                           350
         |                       |
     300 +           X           +                                           300
         |                       |
     350 BL----+-----+-----+----BR        350 BL----+-----+-----+----BR      250
                                              |           X           |
                                          400 TL----+-----+-----+----TR      200
    */
    ptMoved = QPointF(400, 400);
    double fxCenterSetInvalidHeightMoveTopCenterAngle0 = fxCenterSetAngle0;
    double fyCenterSetInvalidHeightMoveTopCenterAngle0 = fBottomSetAngle0 - (fBottomSetAngle0 - ptMoved.y()) / 2.0;
    double fWidthSetInvalidHeightMoveTopCenterAngle0 = fWidthSetAngle0;
    double fHeightSetInvalidHeightMoveTopCenterAngle0 = fBottomSetAngle0 - ptMoved.y();
    double fLeftSetInvalidHeightMoveTopCenterAngle0 = fxCenterSetInvalidHeightMoveTopCenterAngle0 - fWidthSetInvalidHeightMoveTopCenterAngle0 / 2.0;
    double fRightSetInvalidHeightMoveTopCenterAngle0 = fxCenterSetInvalidHeightMoveTopCenterAngle0 + fWidthSetInvalidHeightMoveTopCenterAngle0 / 2.0;
    double fTopSetInvalidHeightMoveTopCenterAngle0 = fyCenterSetInvalidHeightMoveTopCenterAngle0 - fHeightSetInvalidHeightMoveTopCenterAngle0 / 2.0;
    double fBottomSetInvalidHeightMoveTopCenterAngle0 = fyCenterSetInvalidHeightMoveTopCenterAngle0 + fHeightSetInvalidHeightMoveTopCenterAngle0 / 2.0;
    QPointF ptCenterSetInvalidHeightMoveTopCenterAngle0(fxCenterSetInvalidHeightMoveTopCenterAngle0, fyCenterSetInvalidHeightMoveTopCenterAngle0);
    QPointF ptTopLeftSetInvalidHeightMoveTopCenterAngle0(fLeftSetInvalidHeightMoveTopCenterAngle0, fTopSetInvalidHeightMoveTopCenterAngle0);
    QPointF ptTopRightSetInvalidHeightMoveTopCenterAngle0(fRightSetInvalidHeightMoveTopCenterAngle0, fTopSetInvalidHeightMoveTopCenterAngle0);
    QPointF ptBottomRightSetInvalidHeightMoveTopCenterAngle0(fRightSetInvalidHeightMoveTopCenterAngle0, fBottomSetInvalidHeightMoveTopCenterAngle0);
    QPointF ptBottomLeftSetInvalidHeightMoveTopCenterAngle0(fLeftSetInvalidHeightMoveTopCenterAngle0, fBottomSetInvalidHeightMoveTopCenterAngle0);
    QPointF ptTopCenterSetInvalidHeightMoveTopCenterAngle0(fxCenterSetInvalidHeightMoveTopCenterAngle0, fTopSetInvalidHeightMoveTopCenterAngle0);
    QPointF ptRightCenterSetInvalidHeightMoveTopCenterAngle0(fRightSetInvalidHeightMoveTopCenterAngle0, fyCenterSetInvalidHeightMoveTopCenterAngle0);
    QPointF ptBottomCenterSetInvalidHeightMoveTopCenterAngle0(fxCenterSetInvalidHeightMoveTopCenterAngle0, fBottomSetInvalidHeightMoveTopCenterAngle0);
    QPointF ptLeftCenterSetInvalidHeightMoveTopCenterAngle0(fLeftSetInvalidHeightMoveTopCenterAngle0, fyCenterSetInvalidHeightMoveTopCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidHeightMoveTopCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidHeightMoveTopCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidHeightMoveTopCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidHeightMoveTopCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidHeightMoveTopCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingBottomCenter(400, 300)
    ------------------------------------------
        200   250   300   350   400          200   250   300   350   400   BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+-----+-----+      350
         |                       |   =>       |           X           |
     300 +           X           +        300 +-----+-----+-----+-----+      300
         |                       |
     350 +-----+-----+-----+-----+
    */
    ptMoved = ptRightCenterSetAngle0;
    double fxCenterSetValidHeightMoveBottomCenterAngle0 = fxCenterSetAngle0;
    double fyCenterSetValidHeightMoveBottomCenterAngle0 = fTopSetAngle0 + (ptMoved.y() - fTopSetAngle0) / 2.0;
    double fWidthSetValidHeightMoveBottomCenterAngle0 = fWidthSetAngle0;
    double fHeightSetValidHeightMoveBottomCenterAngle0 = ptMoved.y() - fTopSetAngle0;
    double fLeftSetValidHeightMoveBottomCenterAngle0 = fxCenterSetValidHeightMoveBottomCenterAngle0 - fWidthSetValidHeightMoveBottomCenterAngle0 / 2.0;
    double fRightSetValidHeightMoveBottomCenterAngle0 = fxCenterSetValidHeightMoveBottomCenterAngle0 + fWidthSetValidHeightMoveBottomCenterAngle0 / 2.0;
    double fTopSetValidHeightMoveBottomCenterAngle0 = fyCenterSetValidHeightMoveBottomCenterAngle0 - fHeightSetValidHeightMoveBottomCenterAngle0 / 2.0;
    double fBottomSetValidHeightMoveBottomCenterAngle0 = fyCenterSetValidHeightMoveBottomCenterAngle0 + fHeightSetValidHeightMoveBottomCenterAngle0 / 2.0;
    QPointF ptCenterSetValidHeightMoveBottomCenterAngle0(fxCenterSetValidHeightMoveBottomCenterAngle0, fyCenterSetValidHeightMoveBottomCenterAngle0);
    QPointF ptTopLeftSetValidHeightMoveBottomCenterAngle0(fLeftSetValidHeightMoveBottomCenterAngle0, fTopSetValidHeightMoveBottomCenterAngle0);
    QPointF ptTopRightSetValidHeightMoveBottomCenterAngle0(fRightSetValidHeightMoveBottomCenterAngle0, fTopSetValidHeightMoveBottomCenterAngle0);
    QPointF ptBottomRightSetValidHeightMoveBottomCenterAngle0(fRightSetValidHeightMoveBottomCenterAngle0, fBottomSetValidHeightMoveBottomCenterAngle0);
    QPointF ptBottomLeftSetValidHeightMoveBottomCenterAngle0(fLeftSetValidHeightMoveBottomCenterAngle0, fBottomSetValidHeightMoveBottomCenterAngle0);
    QPointF ptTopCenterSetValidHeightMoveBottomCenterAngle0(fxCenterSetValidHeightMoveBottomCenterAngle0, fTopSetValidHeightMoveBottomCenterAngle0);
    QPointF ptRightCenterSetValidHeightMoveBottomCenterAngle0(fRightSetValidHeightMoveBottomCenterAngle0, fyCenterSetValidHeightMoveBottomCenterAngle0);
    QPointF ptBottomCenterSetValidHeightMoveBottomCenterAngle0(fxCenterSetValidHeightMoveBottomCenterAngle0, fBottomSetValidHeightMoveBottomCenterAngle0);
    QPointF ptLeftCenterSetValidHeightMoveBottomCenterAngle0(fLeftSetValidHeightMoveBottomCenterAngle0, fyCenterSetValidHeightMoveBottomCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidHeightMoveBottomCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidHeightMoveBottomCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidHeightMoveBottomCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidHeightMoveBottomCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidHeightMoveBottomCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingBottomCenter(400, 200)
    ------------------------------------------


        200   250   300   350   400          200   250   300   350   400   BottomUp
                                          200 +-----+-----+-----+-----+      400
                                              |           X           |
     250 +-----+-----+-----+-----+        250 +-----+-----+-----+-----+      350
         |                       |
     300 +           X           +                                           300
         |                       |
     350 +-----+-----+-----+-----+
    */
    ptMoved = QPointF(400.0, 200.0);
    double fxCenterSetInvalidHeightMoveBottomCenterAngle0 = fxCenterSetAngle0;
    double fyCenterSetInvalidHeightMoveBottomCenterAngle0 = fTopSetAngle0 + (ptMoved.y() - fTopSetAngle0) / 2.0;
    double fWidthSetInvalidHeightMoveBottomCenterAngle0 = fWidthSetAngle0;
    double fHeightSetInvalidHeightMoveBottomCenterAngle0 = ptMoved.y() - fTopSetAngle0;
    double fLeftSetInvalidHeightMoveBottomCenterAngle0 = fxCenterSetInvalidHeightMoveBottomCenterAngle0 - fWidthSetInvalidHeightMoveBottomCenterAngle0 / 2.0;
    double fRightSetInvalidHeightMoveBottomCenterAngle0 = fxCenterSetInvalidHeightMoveBottomCenterAngle0 + fWidthSetInvalidHeightMoveBottomCenterAngle0 / 2.0;
    double fTopSetInvalidHeightMoveBottomCenterAngle0 = fyCenterSetInvalidHeightMoveBottomCenterAngle0 - fHeightSetInvalidHeightMoveBottomCenterAngle0 / 2.0;
    double fBottomSetInvalidHeightMoveBottomCenterAngle0 = fyCenterSetInvalidHeightMoveBottomCenterAngle0 + fHeightSetInvalidHeightMoveBottomCenterAngle0 / 2.0;
    QPointF ptCenterSetInvalidHeightMoveBottomCenterAngle0(fxCenterSetInvalidHeightMoveBottomCenterAngle0, fyCenterSetInvalidHeightMoveBottomCenterAngle0);
    QPointF ptTopLeftSetInvalidHeightMoveBottomCenterAngle0(fLeftSetInvalidHeightMoveBottomCenterAngle0, fTopSetInvalidHeightMoveBottomCenterAngle0);
    QPointF ptTopRightSetInvalidHeightMoveBottomCenterAngle0(fRightSetInvalidHeightMoveBottomCenterAngle0, fTopSetInvalidHeightMoveBottomCenterAngle0);
    QPointF ptBottomRightSetInvalidHeightMoveBottomCenterAngle0(fRightSetInvalidHeightMoveBottomCenterAngle0, fBottomSetInvalidHeightMoveBottomCenterAngle0);
    QPointF ptBottomLeftSetInvalidHeightMoveBottomCenterAngle0(fLeftSetInvalidHeightMoveBottomCenterAngle0, fBottomSetInvalidHeightMoveBottomCenterAngle0);
    QPointF ptTopCenterSetInvalidHeightMoveBottomCenterAngle0(fxCenterSetInvalidHeightMoveBottomCenterAngle0, fTopSetInvalidHeightMoveBottomCenterAngle0);
    QPointF ptRightCenterSetInvalidHeightMoveBottomCenterAngle0(fRightSetInvalidHeightMoveBottomCenterAngle0, fyCenterSetInvalidHeightMoveBottomCenterAngle0);
    QPointF ptBottomCenterSetInvalidHeightMoveBottomCenterAngle0(fxCenterSetInvalidHeightMoveBottomCenterAngle0, fBottomSetInvalidHeightMoveBottomCenterAngle0);
    QPointF ptLeftCenterSetInvalidHeightMoveBottomCenterAngle0(fLeftSetInvalidHeightMoveBottomCenterAngle0, fyCenterSetInvalidHeightMoveBottomCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidHeightMoveBottomCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidHeightMoveBottomCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidHeightMoveBottomCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidHeightMoveBottomCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidHeightMoveBottomCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(300, 200)
    -----------------------
                                              300   350   400     BottomUp
                                           200 +-----+-----+        400
        200   250   300   350   400            |           |
     250 +-----+-----+-----+-----+         250 |           |        350
         |                       |             x     X     x
     300 x           X           x    =>   300 |           |        300
         |                       |             |           |
     350 +-----+-----x-----+-----+         350 +-----x-----+        250
    */
    QPointF ptTopLeftValidSizeAngle0(300.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopLeftValidSizeAngle0.setY(400.0);
    }
    double fxCenterSetTopLeftValidSizeAngle0 = ptTopLeftValidSizeAngle0.x() + (fRightSetAngle0 - ptTopLeftValidSizeAngle0.x()) / 2.0;
    double fyCenterSetTopLeftValidSizeAngle0 = ptTopLeftValidSizeAngle0.y() + (fBottomSetAngle0 - ptTopLeftValidSizeAngle0.y()) / 2.0;
    double fWidthSetTopLeftValidSizeAngle0 = fRightSetAngle0 - ptTopLeftValidSizeAngle0.x();
    double fHeightSetTopLeftValidSizeAngle0 = fBottomSetAngle0 - ptTopLeftValidSizeAngle0.y();
    double fLeftSetTopLeftValidSizeAngle0 = fxCenterSetTopLeftValidSizeAngle0 - fWidthSetTopLeftValidSizeAngle0 / 2.0;
    double fRightSetTopLeftValidSizeAngle0 = fxCenterSetTopLeftValidSizeAngle0 + fWidthSetTopLeftValidSizeAngle0 / 2.0;
    double fTopSetTopLeftValidSizeAngle0 = fyCenterSetTopLeftValidSizeAngle0 - fHeightSetTopLeftValidSizeAngle0 / 2.0;
    double fBottomSetTopLeftValidSizeAngle0 = fyCenterSetTopLeftValidSizeAngle0 + fHeightSetTopLeftValidSizeAngle0 / 2.0;
    QPointF ptCenterSetTopLeftValidSizeAngle0(fxCenterSetTopLeftValidSizeAngle0, fyCenterSetTopLeftValidSizeAngle0);
    QPointF ptTopLeftSetTopLeftValidSizeAngle0(fLeftSetTopLeftValidSizeAngle0, fTopSetTopLeftValidSizeAngle0);
    QPointF ptTopRightSetTopLeftValidSizeAngle0(fRightSetTopLeftValidSizeAngle0, fTopSetTopLeftValidSizeAngle0);
    QPointF ptBottomRightSetTopLeftValidSizeAngle0(fRightSetTopLeftValidSizeAngle0, fBottomSetTopLeftValidSizeAngle0);
    QPointF ptBottomLeftSetTopLeftValidSizeAngle0(fLeftSetTopLeftValidSizeAngle0, fBottomSetTopLeftValidSizeAngle0);
    QPointF ptTopCenterSetTopLeftValidSizeAngle0(fxCenterSetTopLeftValidSizeAngle0, fTopSetTopLeftValidSizeAngle0);
    QPointF ptRightCenterSetTopLeftValidSizeAngle0(fRightSetTopLeftValidSizeAngle0, fyCenterSetTopLeftValidSizeAngle0);
    QPointF ptBottomCenterSetTopLeftValidSizeAngle0(fxCenterSetTopLeftValidSizeAngle0, fBottomSetTopLeftValidSizeAngle0);
    QPointF ptLeftCenterSetTopLeftValidSizeAngle0(fLeftSetTopLeftValidSizeAngle0, fyCenterSetTopLeftValidSizeAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(" + qPoint2Str(ptTopLeftValidSizeAngle0) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptTopLeftValidSizeAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptTopLeftValidSizeAngle0);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftValidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftValidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftValidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftValidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftValidSizeAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(500, 400)
    -----------------------
        200   250   300   350   400   450   500       BottomUp
     250 TL----+-----+-----+----TR                       350
         |                       |
     300 x           X           x                       300
         |                       |
     350 BL----+-----x-----+----BR-----x----BL           250
                                 |           |
     400                         TR----x----TL           200
    */
    QPointF ptTopLeftInvalidSizeAngle0(500.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopLeftInvalidSizeAngle0.setY(200.0);
    }
    double fxCenterSetTopLeftInvalidSizeAngle0 = ptTopLeftInvalidSizeAngle0.x() + (fRightSetAngle0 - ptTopLeftInvalidSizeAngle0.x()) / 2.0;
    double fyCenterSetTopLeftInvalidSizeAngle0 = ptTopLeftInvalidSizeAngle0.y() + (fBottomSetAngle0 - ptTopLeftInvalidSizeAngle0.y()) / 2.0;
    double fWidthSetTopLeftInvalidSizeAngle0 = fRightSetAngle0 - ptTopLeftInvalidSizeAngle0.x();
    double fHeightSetTopLeftInvalidSizeAngle0 = fBottomSetAngle0 - ptTopLeftInvalidSizeAngle0.y();
    double fLeftSetTopLeftInvalidSizeAngle0 = fxCenterSetTopLeftInvalidSizeAngle0 - fWidthSetTopLeftInvalidSizeAngle0 / 2.0;
    double fRightSetTopLeftInvalidSizeAngle0 = fxCenterSetTopLeftInvalidSizeAngle0 + fWidthSetTopLeftInvalidSizeAngle0 / 2.0;
    double fTopSetTopLeftInvalidSizeAngle0 = fyCenterSetTopLeftInvalidSizeAngle0 - fHeightSetTopLeftInvalidSizeAngle0 / 2.0;
    double fBottomSetTopLeftInvalidSizeAngle0 = fyCenterSetTopLeftInvalidSizeAngle0 + fHeightSetTopLeftInvalidSizeAngle0 / 2.0;
    QPointF ptCenterSetTopLeftInvalidSizeAngle0(fxCenterSetTopLeftInvalidSizeAngle0, fyCenterSetTopLeftInvalidSizeAngle0);
    QPointF ptTopLeftSetTopLeftInvalidSizeAngle0(fLeftSetTopLeftInvalidSizeAngle0, fTopSetTopLeftInvalidSizeAngle0);
    QPointF ptTopRightSetTopLeftInvalidSizeAngle0(fRightSetTopLeftInvalidSizeAngle0, fTopSetTopLeftInvalidSizeAngle0);
    QPointF ptBottomRightSetTopLeftInvalidSizeAngle0(fRightSetTopLeftInvalidSizeAngle0, fBottomSetTopLeftInvalidSizeAngle0);
    QPointF ptBottomLeftSetTopLeftInvalidSizeAngle0(fLeftSetTopLeftInvalidSizeAngle0, fBottomSetTopLeftInvalidSizeAngle0);
    QPointF ptTopCenterSetTopLeftInvalidSizeAngle0(fxCenterSetTopLeftInvalidSizeAngle0, fTopSetTopLeftInvalidSizeAngle0);
    QPointF ptRightCenterSetTopLeftInvalidSizeAngle0(fRightSetTopLeftInvalidSizeAngle0, fyCenterSetTopLeftInvalidSizeAngle0);
    QPointF ptBottomCenterSetTopLeftInvalidSizeAngle0(fxCenterSetTopLeftInvalidSizeAngle0, fBottomSetTopLeftInvalidSizeAngle0);
    QPointF ptLeftCenterSetTopLeftInvalidSizeAngle0(fLeftSetTopLeftInvalidSizeAngle0, fyCenterSetTopLeftInvalidSizeAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(" + qPoint2Str(ptTopLeftInvalidSizeAngle0) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptTopLeftInvalidSizeAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptTopLeftInvalidSizeAngle0);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftInvalidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftInvalidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftInvalidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftInvalidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftInvalidSizeAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(300, 200)
    ------------------------
                                              200   250   300     BottomUp
                                           200 +-----+-----+        400
        200   250   300   350   400            |           |
     250 +-----+-----+-----+-----+         250 |           |        350
         |                       |             x     X     x
     300 x           X           x    =>   300 |           |        300
         |                       |             |           |
     350 +-----+-----x-----+-----+         350 +-----x-----+        250
    */
    QPointF ptTopRightValidSizeAngle0(300.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopRightValidSizeAngle0.setY(400.0);
    }
    double fxCenterSetTopRightValidSizeAngle0 = fLeftSetAngle0 + (ptTopRightValidSizeAngle0.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetTopRightValidSizeAngle0 = ptTopRightValidSizeAngle0.y() + (fBottomSetAngle0 - ptTopRightValidSizeAngle0.y()) / 2.0;
    double fWidthSetTopRightValidSizeAngle0 = ptTopRightValidSizeAngle0.x() - fLeftSetAngle0;
    double fHeightSetTopRightValidSizeAngle0 = fBottomSetAngle0 - ptTopRightValidSizeAngle0.y();
    double fLeftSetTopRightValidSizeAngle0 = fxCenterSetTopRightValidSizeAngle0 - fWidthSetTopRightValidSizeAngle0 / 2.0;
    double fRightSetTopRightValidSizeAngle0 = fxCenterSetTopRightValidSizeAngle0 + fWidthSetTopRightValidSizeAngle0 / 2.0;
    double fTopSetTopRightValidSizeAngle0 = fyCenterSetTopRightValidSizeAngle0 - fHeightSetTopRightValidSizeAngle0 / 2.0;
    double fBottomSetTopRightValidSizeAngle0 = fyCenterSetTopRightValidSizeAngle0 + fHeightSetTopRightValidSizeAngle0 / 2.0;
    QPointF ptCenterSetTopRightValidSizeAngle0(fxCenterSetTopRightValidSizeAngle0, fyCenterSetTopRightValidSizeAngle0);
    QPointF ptTopLeftSetTopRightValidSizeAngle0(fLeftSetTopRightValidSizeAngle0, fTopSetTopRightValidSizeAngle0);
    QPointF ptTopRightSetTopRightValidSizeAngle0(fRightSetTopRightValidSizeAngle0, fTopSetTopRightValidSizeAngle0);
    QPointF ptBottomRightSetTopRightValidSizeAngle0(fRightSetTopRightValidSizeAngle0, fBottomSetTopRightValidSizeAngle0);
    QPointF ptBottomLeftSetTopRightValidSizeAngle0(fLeftSetTopRightValidSizeAngle0, fBottomSetTopRightValidSizeAngle0);
    QPointF ptTopCenterSetTopRightValidSizeAngle0(fxCenterSetTopRightValidSizeAngle0, fTopSetTopRightValidSizeAngle0);
    QPointF ptRightCenterSetTopRightValidSizeAngle0(fRightSetTopRightValidSizeAngle0, fyCenterSetTopRightValidSizeAngle0);
    QPointF ptBottomCenterSetTopRightValidSizeAngle0(fxCenterSetTopRightValidSizeAngle0, fBottomSetTopRightValidSizeAngle0);
    QPointF ptLeftCenterSetTopRightValidSizeAngle0(fLeftSetTopRightValidSizeAngle0, fyCenterSetTopRightValidSizeAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(" + qPoint2Str(ptTopRightValidSizeAngle0) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptTopRightValidSizeAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptTopRightValidSizeAngle0);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightValidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightValidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightValidSizeAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightValidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightValidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightValidSizeAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(100, 400)
    ------------------------
        100   150   200   250   300   350   400    BottomUp
     250             TL----+-----+-----+----TR       350
                     |                       |
     300             x           X           x       300
                     |                       |
     350 BR----+-----BL----+-----x-----+----BR       250
         |           |
     400 TR----x----TL                               200
    */
    QPointF ptTopRightInvalidSizeAngle0(100.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopRightInvalidSizeAngle0.setY(200.0);
    }
    double fxCenterSetTopRightInvalidSizeAngle0 = fLeftSetAngle0 + (ptTopRightInvalidSizeAngle0.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetTopRightInvalidSizeAngle0 = ptTopRightInvalidSizeAngle0.y() + (fBottomSetAngle0 - ptTopRightInvalidSizeAngle0.y()) / 2.0;
    double fWidthSetTopRightInvalidSizeAngle0 = ptTopRightInvalidSizeAngle0.x() - fLeftSetAngle0;
    double fHeightSetTopRightInvalidSizeAngle0 = fBottomSetAngle0 - ptTopRightInvalidSizeAngle0.y();
    double fLeftSetTopRightInvalidSizeAngle0 = fxCenterSetTopRightInvalidSizeAngle0 - fWidthSetTopRightInvalidSizeAngle0 / 2.0;
    double fRightSetTopRightInvalidSizeAngle0 = fxCenterSetTopRightInvalidSizeAngle0 + fWidthSetTopRightInvalidSizeAngle0 / 2.0;
    double fTopSetTopRightInvalidSizeAngle0 = fyCenterSetTopRightInvalidSizeAngle0 - fHeightSetTopRightInvalidSizeAngle0 / 2.0;
    double fBottomSetTopRightInvalidSizeAngle0 = fyCenterSetTopRightInvalidSizeAngle0 + fHeightSetTopRightInvalidSizeAngle0 / 2.0;
    QPointF ptCenterSetTopRightInvalidSizeAngle0(fxCenterSetTopRightInvalidSizeAngle0, fyCenterSetTopRightInvalidSizeAngle0);
    QPointF ptTopLeftSetTopRightInvalidSizeAngle0(fLeftSetTopRightInvalidSizeAngle0, fTopSetTopRightInvalidSizeAngle0);
    QPointF ptTopRightSetTopRightInvalidSizeAngle0(fRightSetTopRightInvalidSizeAngle0, fTopSetTopRightInvalidSizeAngle0);
    QPointF ptBottomRightSetTopRightInvalidSizeAngle0(fRightSetTopRightInvalidSizeAngle0, fBottomSetTopRightInvalidSizeAngle0);
    QPointF ptBottomLeftSetTopRightInvalidSizeAngle0(fLeftSetTopRightInvalidSizeAngle0, fBottomSetTopRightInvalidSizeAngle0);
    QPointF ptTopCenterSetTopRightInvalidSizeAngle0(fxCenterSetTopRightInvalidSizeAngle0, fTopSetTopRightInvalidSizeAngle0);
    QPointF ptRightCenterSetTopRightInvalidSizeAngle0(fRightSetTopRightInvalidSizeAngle0, fyCenterSetTopRightInvalidSizeAngle0);
    QPointF ptBottomCenterSetTopRightInvalidSizeAngle0(fxCenterSetTopRightInvalidSizeAngle0, fBottomSetTopRightInvalidSizeAngle0);
    QPointF ptLeftCenterSetTopRightInvalidSizeAngle0(fLeftSetTopRightInvalidSizeAngle0, fyCenterSetTopRightInvalidSizeAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(" + qPoint2Str(ptTopRightInvalidSizeAngle0) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptTopRightInvalidSizeAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptTopRightInvalidSizeAngle0);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightInvalidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightInvalidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightInvalidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightInvalidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightInvalidSizeAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight(300, 400)
    ---------------------------
        200   250   300   350   400           200   250   300      BottomUp
     250 +-----+-----+-----+-----+         250 +-----+-----+        350
         |                       |             |           |
     300 x           X           x    =>   300 +           +        300
         |                       |             |     X     |
     350 +-----+-----x-----+-----+         350 +           +        250
                                               |           |
                                           400 +-----+-----+        200
    */
    QPointF ptBottomRightValidSize(300.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomRightValidSize.setY(200.0);
    }
    double fxCenterSetBottomRightValidSizeAngle0 = fLeftSetAngle0 + (ptBottomRightValidSize.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetBottomRightValidSizeAngle0 = fTopSetAngle0 + (ptBottomRightValidSize.y() - fTopSetAngle0) / 2.0;
    double fWidthSetBottomRightValidSizeAngle0 = ptBottomRightValidSize.x() - fLeftSetAngle0;
    double fHeightSetBottomRightValidSizeAngle0 = ptBottomRightValidSize.y() - fTopSetAngle0;
    double fLeftSetBottomRightValidSizeAngle0 = fxCenterSetBottomRightValidSizeAngle0 - fWidthSetBottomRightValidSizeAngle0 / 2.0;
    double fRightSetBottomRightValidSizeAngle0 = fxCenterSetBottomRightValidSizeAngle0 + fWidthSetBottomRightValidSizeAngle0 / 2.0;
    double fTopSetBottomRightValidSizeAngle0 = fyCenterSetBottomRightValidSizeAngle0 - fHeightSetBottomRightValidSizeAngle0 / 2.0;
    double fBottomSetBottomRightValidSizeAngle0 = fyCenterSetBottomRightValidSizeAngle0 + fHeightSetBottomRightValidSizeAngle0 / 2.0;
    QPointF ptCenterSetBottomRightValidSizeAngle0(fxCenterSetBottomRightValidSizeAngle0, fyCenterSetBottomRightValidSizeAngle0);
    QPointF ptTopLeftSetBottomRightValidSizeAngle0(fLeftSetBottomRightValidSizeAngle0, fTopSetBottomRightValidSizeAngle0);
    QPointF ptTopRightSetBottomRightValidSizeAngle0(fRightSetBottomRightValidSizeAngle0, fTopSetBottomRightValidSizeAngle0);
    QPointF ptBottomRightSetBottomRightValidSizeAngle0(fRightSetBottomRightValidSizeAngle0, fBottomSetBottomRightValidSizeAngle0);
    QPointF ptBottomLeftSetBottomRightValidSizeAngle0(fLeftSetBottomRightValidSizeAngle0, fBottomSetBottomRightValidSizeAngle0);
    QPointF ptTopCenterSetBottomRightValidSizeAngle0(fxCenterSetBottomRightValidSizeAngle0, fTopSetBottomRightValidSizeAngle0);
    QPointF ptRightCenterSetBottomRightValidSizeAngle0(fRightSetBottomRightValidSizeAngle0, fyCenterSetBottomRightValidSizeAngle0);
    QPointF ptBottomCenterSetBottomRightValidSizeAngle0(fxCenterSetBottomRightValidSizeAngle0, fBottomSetBottomRightValidSizeAngle0);
    QPointF ptLeftCenterSetBottomRightValidSizeAngle0(fLeftSetBottomRightValidSizeAngle0, fyCenterSetBottomRightValidSizeAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(" + qPoint2Str(ptBottomRightValidSize) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptBottomRightValidSize) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptBottomRightValidSize);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightValidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightValidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightValidSizeAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightValidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightValidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightValidSizeAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight(100, 200)
    ---------------------------

         100   150   200   250   300   350   400     BottomUp
    200   BR----+----BL                                 400
          |           |
    250   TR----+-----TL----+-----+-----+----TR         350
                      |                       |
    300               x           X           x         300
                      |                       |
    350               BL----+-----x-----+----BR         250
    */
    QPointF ptBottomRightInvalidSize(100.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomRightInvalidSize.setY(400.0);
    }
    double fxCenterSetBottomRightInvalidSizeAngle0 = fLeftSetAngle0 + (ptBottomRightInvalidSize.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetBottomRightInvalidSizeAngle0 = fTopSetAngle0 + (ptBottomRightInvalidSize.y() - fTopSetAngle0) / 2.0;
    double fWidthSetBottomRightInvalidSizeAngle0 = ptBottomRightInvalidSize.x() - fLeftSetAngle0;
    double fHeightSetBottomRightInvalidSizeAngle0 = ptBottomRightInvalidSize.y() - fTopSetAngle0;
    double fLeftSetBottomRightInvalidSizeAngle0 = fxCenterSetBottomRightInvalidSizeAngle0 - fWidthSetBottomRightInvalidSizeAngle0 / 2.0;
    double fRightSetBottomRightInvalidSizeAngle0 = fxCenterSetBottomRightInvalidSizeAngle0 + fWidthSetBottomRightInvalidSizeAngle0 / 2.0;
    double fTopSetBottomRightInvalidSizeAngle0 = fyCenterSetBottomRightInvalidSizeAngle0 - fHeightSetBottomRightInvalidSizeAngle0 / 2.0;
    double fBottomSetBottomRightInvalidSizeAngle0 = fyCenterSetBottomRightInvalidSizeAngle0 + fHeightSetBottomRightInvalidSizeAngle0 / 2.0;
    QPointF ptCenterSetBottomRightInvalidSizeAngle0(fxCenterSetBottomRightInvalidSizeAngle0, fyCenterSetBottomRightInvalidSizeAngle0);
    QPointF ptTopLeftSetBottomRightInvalidSizeAngle0(fLeftSetBottomRightInvalidSizeAngle0, fTopSetBottomRightInvalidSizeAngle0);
    QPointF ptTopRightSetBottomRightInvalidSizeAngle0(fRightSetBottomRightInvalidSizeAngle0, fTopSetBottomRightInvalidSizeAngle0);
    QPointF ptBottomRightSetBottomRightInvalidSizeAngle0(fRightSetBottomRightInvalidSizeAngle0, fBottomSetBottomRightInvalidSizeAngle0);
    QPointF ptBottomLeftSetBottomRightInvalidSizeAngle0(fLeftSetBottomRightInvalidSizeAngle0, fBottomSetBottomRightInvalidSizeAngle0);
    QPointF ptTopCenterSetBottomRightInvalidSizeAngle0(fxCenterSetBottomRightInvalidSizeAngle0, fTopSetBottomRightInvalidSizeAngle0);
    QPointF ptRightCenterSetBottomRightInvalidSizeAngle0(fRightSetBottomRightInvalidSizeAngle0, fyCenterSetBottomRightInvalidSizeAngle0);
    QPointF ptBottomCenterSetBottomRightInvalidSizeAngle0(fxCenterSetBottomRightInvalidSizeAngle0, fBottomSetBottomRightInvalidSizeAngle0);
    QPointF ptLeftCenterSetBottomRightInvalidSizeAngle0(fLeftSetBottomRightInvalidSizeAngle0, fyCenterSetBottomRightInvalidSizeAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(" + qPoint2Str(ptBottomRightInvalidSize) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptBottomRightInvalidSize) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptBottomRightInvalidSize);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightInvalidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightInvalidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightInvalidSizeAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightInvalidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightInvalidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightInvalidSizeAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft(300, 400)
    --------------------------
        200   250   300   350   400           300   350   400      BottomUp
     250 +-----+-----+-----+-----+         250 +-----+-----+        350
         |                       |             |           |
     300 x           X           x    =>   300 +           +        300
         |                       |             |     X     |
     350 +-----+-----x-----+-----+         350 +           +        250
                                               |           |
                                           400 +-----+-----+        200
    */
    QPointF ptBottomLeftValidSize(300.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomLeftValidSize.setY(200.0);
    }
    double fxCenterSetBottomLeftValidSizeAngle0 = ptBottomLeftValidSize.x() + (fRightSetAngle0 - ptBottomLeftValidSize.x()) / 2.0;
    double fyCenterSetBottomLeftValidSizeAngle0 = fTopSetAngle0 + (ptBottomLeftValidSize.y() - fTopSetAngle0) / 2.0;
    double fWidthSetBottomLeftValidSizeAngle0 = fRightSetAngle0 - ptBottomLeftValidSize.x();
    double fHeightSetBottomLeftValidSizeAngle0 = ptBottomLeftValidSize.y() - fTopSetAngle0;
    double fLeftSetBottomLeftValidSizeAngle0 = fxCenterSetBottomLeftValidSizeAngle0 - fWidthSetBottomLeftValidSizeAngle0 / 2.0;
    double fRightSetBottomLeftValidSizeAngle0 = fxCenterSetBottomLeftValidSizeAngle0 + fWidthSetBottomLeftValidSizeAngle0 / 2.0;
    double fTopSetBottomLeftValidSizeAngle0 = fyCenterSetBottomLeftValidSizeAngle0 - fHeightSetBottomLeftValidSizeAngle0 / 2.0;
    double fBottomSetBottomLeftValidSizeAngle0 = fyCenterSetBottomLeftValidSizeAngle0 + fHeightSetBottomLeftValidSizeAngle0 / 2.0;
    QPointF ptCenterSetBottomLeftValidSizeAngle0(fxCenterSetBottomLeftValidSizeAngle0, fyCenterSetBottomLeftValidSizeAngle0);
    QPointF ptTopLeftSetBottomLeftValidSizeAngle0(fLeftSetBottomLeftValidSizeAngle0, fTopSetBottomLeftValidSizeAngle0);
    QPointF ptTopRightSetBottomLeftValidSizeAngle0(fRightSetBottomLeftValidSizeAngle0, fTopSetBottomLeftValidSizeAngle0);
    QPointF ptBottomRightSetBottomLeftValidSizeAngle0(fRightSetBottomLeftValidSizeAngle0, fBottomSetBottomLeftValidSizeAngle0);
    QPointF ptBottomLeftSetBottomLeftValidSizeAngle0(fLeftSetBottomLeftValidSizeAngle0, fBottomSetBottomLeftValidSizeAngle0);
    QPointF ptTopCenterSetBottomLeftValidSizeAngle0(fxCenterSetBottomLeftValidSizeAngle0, fTopSetBottomLeftValidSizeAngle0);
    QPointF ptRightCenterSetBottomLeftValidSizeAngle0(fRightSetBottomLeftValidSizeAngle0, fyCenterSetBottomLeftValidSizeAngle0);
    QPointF ptBottomCenterSetBottomLeftValidSizeAngle0(fxCenterSetBottomLeftValidSizeAngle0, fBottomSetBottomLeftValidSizeAngle0);
    QPointF ptLeftCenterSetBottomLeftValidSizeAngle0(fLeftSetBottomLeftValidSizeAngle0, fyCenterSetBottomLeftValidSizeAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(" + qPoint2Str(ptBottomLeftValidSize) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptBottomLeftValidSize) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptBottomLeftValidSize);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftValidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftValidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftValidSizeAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftValidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftValidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftValidSizeAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftValidSizeAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft(500, 200)
    --------------------------
        200   250   300   350   400   450   500     BottomUp
     200                        BR-----+----BL         400
                                 |     X     |
     250 TL----+-----+-----+----TR-----+----TL         350
         |                       |
     300 x           X           x                     300
         |                       |
     350 BL----+-----x-----+----BR                     250
    */
    QPointF ptBottomLeftInvalidSize(500.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomLeftInvalidSize.setY(400.0);
    }
    double fxCenterSetBottomLeftInvalidSizeAngle0 = ptBottomLeftInvalidSize.x() + (fRightSetAngle0 - ptBottomLeftInvalidSize.x()) / 2.0;
    double fyCenterSetBottomLeftInvalidSizeAngle0 = fTopSetAngle0 + (ptBottomLeftInvalidSize.y() - fTopSetAngle0) / 2.0;
    double fWidthSetBottomLeftInvalidSizeAngle0 = fRightSetAngle0 - ptBottomLeftInvalidSize.x();
    double fHeightSetBottomLeftInvalidSizeAngle0 = ptBottomLeftInvalidSize.y() - fTopSetAngle0;
    double fLeftSetBottomLeftInvalidSizeAngle0 = fxCenterSetBottomLeftInvalidSizeAngle0 - fWidthSetBottomLeftInvalidSizeAngle0 / 2.0;
    double fRightSetBottomLeftInvalidSizeAngle0 = fxCenterSetBottomLeftInvalidSizeAngle0 + fWidthSetBottomLeftInvalidSizeAngle0 / 2.0;
    double fTopSetBottomLeftInvalidSizeAngle0 = fyCenterSetBottomLeftInvalidSizeAngle0 - fHeightSetBottomLeftInvalidSizeAngle0 / 2.0;
    double fBottomSetBottomLeftInvalidSizeAngle0 = fyCenterSetBottomLeftInvalidSizeAngle0 + fHeightSetBottomLeftInvalidSizeAngle0 / 2.0;
    QPointF ptCenterSetBottomLeftInvalidSizeAngle0(fxCenterSetBottomLeftInvalidSizeAngle0, fyCenterSetBottomLeftInvalidSizeAngle0);
    QPointF ptTopLeftSetBottomLeftInvalidSizeAngle0(fLeftSetBottomLeftInvalidSizeAngle0, fTopSetBottomLeftInvalidSizeAngle0);
    QPointF ptTopRightSetBottomLeftInvalidSizeAngle0(fRightSetBottomLeftInvalidSizeAngle0, fTopSetBottomLeftInvalidSizeAngle0);
    QPointF ptBottomRightSetBottomLeftInvalidSizeAngle0(fRightSetBottomLeftInvalidSizeAngle0, fBottomSetBottomLeftInvalidSizeAngle0);
    QPointF ptBottomLeftSetBottomLeftInvalidSizeAngle0(fLeftSetBottomLeftInvalidSizeAngle0, fBottomSetBottomLeftInvalidSizeAngle0);
    QPointF ptTopCenterSetBottomLeftInvalidSizeAngle0(fxCenterSetBottomLeftInvalidSizeAngle0, fTopSetBottomLeftInvalidSizeAngle0);
    QPointF ptRightCenterSetBottomLeftInvalidSizeAngle0(fRightSetBottomLeftInvalidSizeAngle0, fyCenterSetBottomLeftInvalidSizeAngle0);
    QPointF ptBottomCenterSetBottomLeftInvalidSizeAngle0(fxCenterSetBottomLeftInvalidSizeAngle0, fBottomSetBottomLeftInvalidSizeAngle0);
    QPointF ptLeftCenterSetBottomLeftInvalidSizeAngle0(fLeftSetBottomLeftInvalidSizeAngle0, fyCenterSetBottomLeftInvalidSizeAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(" + qPoint2Str(ptBottomLeftInvalidSize) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptBottomLeftInvalidSize) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptBottomLeftInvalidSize);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftInvalidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftInvalidSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftInvalidSizeAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftInvalidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftInvalidSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftInvalidSizeAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftInvalidSizeAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    //--------------------------------------------------------------------------
    // Rotation Angle: 30.0°
    //--------------------------------------------------------------------------

    physValAngle.setVal(30.0);

    ZS::Test::CTestStepGroup* pGrpTransformRectAngle_30_Degree = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Angle(" + physValAngle.toString() + ")",
        /* pTSGrpParent */ pGrpTransformRect );

    /* setAngle(30.0°)
    ------------------
        200   250   300   350   400      BottomUp
     250 +-----+-----+-----+-----+         350
         |                       |
     300 +           X           +         300
         |                       |
     350 +-----+-----+-----+-----+         250
    */
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setAngle(" + physValAngle.toString() + ")",
        /* strOperation    */ "setAngle(" + physValAngle.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    double fWidthSetAngle30 = fWidthSetAngle0;
    double fHeightSetAngle30 = fHeightSetAngle0;
    QPointF ptCenterSetAngle30 = ptCenterSetAngle0;
    QPointF ptTopLeftSetAngle30(238.4, 206.7);
    QPointF ptTopRightSetAngle30(411.6, 306.7);
    QPointF ptBottomRightSetAngle30(361.6, 393.3);
    QPointF ptBottomLeftSetAngle30(188.4, 293.3);
    QPointF ptTopCenterSetAngle30 = QPointF((ptTopRightSetAngle30.x() + ptTopLeftSetAngle30.x()) / 2.0,
                                            (ptTopRightSetAngle30.y() + ptTopLeftSetAngle30.y()) / 2.0);
    QPointF ptRightCenterSetAngle30 = QPointF((ptTopRightSetAngle30.x() + ptBottomRightSetAngle30.x()) / 2.0,
                                              (ptBottomRightSetAngle30.y() + ptTopRightSetAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetAngle30 = QPointF((ptBottomRightSetAngle30.x() + ptBottomLeftSetAngle30.x()) / 2.0,
                                               (ptBottomRightSetAngle30.y() + ptBottomLeftSetAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetAngle30 = QPointF((ptTopLeftSetAngle30.x() + ptBottomLeftSetAngle30.x()) / 2.0,
                                             (ptBottomLeftSetAngle30.y() + ptTopLeftSetAngle30.y()) / 2.0);
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setCenter(400, 400)
    ----------------------
        200   250   300   350   400   450   500    BottomUp
     250 +-----+-----+-----+-----+                   350
         |                       |
     300 +           X           +                   300
         |                       |
     350 +-----+-----+-----+-----+-----+-----+       250
                     |                       |
     400      =>     x           X           x       200
                     |                       |
     450             +-----+-----x-----+-----+       150
    */
    ptCenter = QPointF(400.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptCenter.setY(200.0);
    }
    fdx = ptCenter.x() - ptCenterSetAngle30.x();
    fdy = ptCenter.y() - ptCenterSetAngle30.y();
    double fWidthSetCenterAngle30 = fWidthSetAngle30;
    double fHeightSetCenterAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetCenterAngle30 = ptCenter;
    QPointF ptTopLeftSetCenterAngle30(ptTopLeftSetAngle30.x() + fdx, ptTopLeftSetAngle30.y() + fdy);
    QPointF ptTopRightSetCenterAngle30(ptTopRightSetAngle30.x() + fdx, ptTopRightSetAngle30.y() + fdy);
    QPointF ptBottomRightSetCenterAngle30(ptBottomRightSetAngle30.x() + fdx, ptBottomRightSetAngle30.y() + fdy);
    QPointF ptBottomLeftSetCenterAngle30(ptBottomLeftSetAngle30.x() + fdx, ptBottomLeftSetAngle30.y() + fdy);
    QPointF ptTopCenterSetCenterAngle30 = QPointF((ptTopRightSetCenterAngle30.x() + ptTopLeftSetCenterAngle30.x()) / 2.0,
                                                  (ptTopRightSetCenterAngle30.y() + ptTopLeftSetCenterAngle30.y()) / 2.0);
    QPointF ptRightCenterSetCenterAngle30 = QPointF((ptTopRightSetCenterAngle30.x() + ptBottomRightSetCenterAngle30.x()) / 2.0,
                                                    (ptBottomRightSetCenterAngle30.y() + ptTopRightSetCenterAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetCenterAngle30 = QPointF((ptBottomRightSetCenterAngle30.x() + ptBottomLeftSetCenterAngle30.x()) / 2.0,
                                                     (ptBottomRightSetCenterAngle30.y() + ptBottomLeftSetCenterAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetCenterAngle30 = QPointF((ptTopLeftSetCenterAngle30.x() + ptBottomLeftSetCenterAngle30.x()) / 2.0,
                                                   (ptBottomLeftSetCenterAngle30.y() + ptTopLeftSetCenterAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setCenter(" + qPoint2Str(ptCenter) + " " + unit.symbol() + ")",
        /* strOperation    */ "setCenter(" + qPoint2Str(ptCenter) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setCenter", ptCenter);
    pTestStep->setConfigValue("setCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetCenterAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetCenterAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetCenterAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetCenterAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetCenterAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setSize(300, 200)
    --------------------
    */
    double fWidthSetValidSizeAngle30 = size300x200.width();
    double fHeightSetValidSizeAngle30 = size300x200.height();
    QPointF ptCenterSetValidSizeAngle30(318.3, 368.3);
    QPointF ptTopLeftSetValidSizeAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetValidSizeAngle30(498.2, 356.7);
    QPointF ptBottomRightSetValidSizeAngle30(398.2, 529.9);
    QPointF ptBottomLeftSetValidSizeAngle30(138.4, 379.9);
    QPointF ptTopCenterSetValidSizeAngle30 = QPointF((ptTopRightSetValidSizeAngle30.x() + ptTopLeftSetValidSizeAngle30.x()) / 2.0,
                                                (ptTopRightSetValidSizeAngle30.y() + ptTopLeftSetValidSizeAngle30.y()) / 2.0);
    QPointF ptRightCenterSetValidSizeAngle30 = QPointF((ptTopRightSetValidSizeAngle30.x() + ptBottomRightSetValidSizeAngle30.x()) / 2.0,
                                                  (ptBottomRightSetValidSizeAngle30.y() + ptTopRightSetValidSizeAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetValidSizeAngle30 = QPointF((ptBottomRightSetValidSizeAngle30.x() + ptBottomLeftSetValidSizeAngle30.x()) / 2.0,
                                                   (ptBottomRightSetValidSizeAngle30.y() + ptBottomLeftSetValidSizeAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetValidSizeAngle30 = QPointF((ptTopLeftSetValidSizeAngle30.x() + ptBottomLeftSetValidSizeAngle30.x()) / 2.0,
                                                 (ptBottomLeftSetValidSizeAngle30.y() + ptTopLeftSetValidSizeAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setSize(" + qSize2Str(size300x200) + " " + unit.symbol() + ")",
        /* strOperation    */ "setSize(" + qSize2Str(size300x200) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setSize", size300x200);
    pTestStep->setConfigValue("setSize.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidSizeAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidSizeAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setSize(-100, -50)
    ---------------------
    */
    double fWidthSetInvalidSizeAngle30 = size_100x_50.width();
    double fHeightSetInvalidSizeAngle30 = size_100x_50.height();
    QPointF ptCenterSetInvalidSizeAngle30(207.6, 160.0);
    QPointF ptTopLeftSetInvalidSizeAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetInvalidSizeAngle30(151.8, 156.7);
    QPointF ptBottomRightSetInvalidSizeAngle30(176.8, 113.4);
    QPointF ptBottomLeftSetInvalidSizeAngle30(263.4, 163.4);
    QPointF ptTopCenterSetInvalidSizeAngle30 = QPointF((ptTopRightSetInvalidSizeAngle30.x() + ptTopLeftSetInvalidSizeAngle30.x()) / 2.0,
                                                       (ptTopRightSetInvalidSizeAngle30.y() + ptTopLeftSetInvalidSizeAngle30.y()) / 2.0);
    QPointF ptRightCenterSetInvalidSizeAngle30(164.3, 135.0); // not calculated because of inaccuracy
    QPointF ptBottomCenterSetInvalidSizeAngle30 = QPointF((ptBottomRightSetInvalidSizeAngle30.x() + ptBottomLeftSetInvalidSizeAngle30.x()) / 2.0,
                                                          (ptBottomRightSetInvalidSizeAngle30.y() + ptBottomLeftSetInvalidSizeAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetInvalidSizeAngle30(250.9, 185.0); // not calculated because of inaccuracy
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setSize(" + qSize2Str(size_100x_50) + " " + unit.symbol() + ")",
        /* strOperation    */ "setSize(" + qSize2Str(size_100x_50) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setSize", size_100x_50);
    pTestStep->setConfigValue("setSize.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidSizeAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidSizeAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidth(300)
    ----------------
    */
    double fWidthSetValidWidthAngle30 = physValWidth300.getVal();
    double fHeightSetValidWidthAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetValidWidthAngle30(343.3, 325.0);
    QPointF ptTopLeftSetValidWidthAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetValidWidthAngle30(498.2, 356.7);
    QPointF ptBottomRightSetValidWidthAngle30(448.2, 443.3);
    QPointF ptBottomLeftSetValidWidthAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetValidWidthAngle30 = QPointF((ptTopRightSetValidWidthAngle30.x() + ptTopLeftSetValidWidthAngle30.x()) / 2.0,
                                                 (ptTopRightSetValidWidthAngle30.y() + ptTopLeftSetValidWidthAngle30.y()) / 2.0);
    QPointF ptRightCenterSetValidWidthAngle30 = QPointF((ptTopRightSetValidWidthAngle30.x() + ptBottomRightSetValidWidthAngle30.x()) / 2.0,
                                                   (ptBottomRightSetValidWidthAngle30.y() + ptTopRightSetValidWidthAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetValidWidthAngle30 = QPointF((ptBottomRightSetValidWidthAngle30.x() + ptBottomLeftSetValidWidthAngle30.x()) / 2.0,
                                                    (ptBottomRightSetValidWidthAngle30.y() + ptBottomLeftSetValidWidthAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetValidWidthAngle30 = ptLeftCenterSetAngle30;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidth(" + physValWidth300.toString() + ")",
        /* strOperation    */ "setWidth(" + physValWidth300.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidth", physValWidth300.toString());
    pTestStep->setConfigValue("setWidth.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidWidthAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidWidthAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidWidthAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidWidthAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidWidthAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidWidthAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidWidthAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidWidthAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidWidthAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidWidthAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidWidthAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidWidthAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidWidthAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidWidthAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidth(-100)
    ----------------
    */
    double fWidthSetInvalidWidthAngle30 = physValWidth_100.getVal();
    double fHeightSetInvalidWidthAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetInvalidWidthAngle30(343.3, 325.0);
    QPointF ptTopLeftSetInvalidWidthAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetInvalidWidthAngle30(498.2, 356.7);
    QPointF ptBottomRightSetInvalidWidthAngle30(448.2, 443.3);
    QPointF ptBottomLeftSetInvalidWidthAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetInvalidWidthAngle30 = QPointF((ptTopRightSetInvalidWidthAngle30.x() + ptTopLeftSetInvalidWidthAngle30.x()) / 2.0,
                                                 (ptTopRightSetInvalidWidthAngle30.y() + ptTopLeftSetInvalidWidthAngle30.y()) / 2.0);
    QPointF ptRightCenterSetInvalidWidthAngle30 = QPointF((ptTopRightSetInvalidWidthAngle30.x() + ptBottomRightSetInvalidWidthAngle30.x()) / 2.0,
                                                   (ptBottomRightSetInvalidWidthAngle30.y() + ptTopRightSetInvalidWidthAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetInvalidWidthAngle30 = QPointF((ptBottomRightSetInvalidWidthAngle30.x() + ptBottomLeftSetInvalidWidthAngle30.x()) / 2.0,
                                                    (ptBottomRightSetInvalidWidthAngle30.y() + ptBottomLeftSetInvalidWidthAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetInvalidWidthAngle30 = ptLeftCenterSetAngle30;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidth(" + physValWidth_100.toString() + ")",
        /* strOperation    */ "setWidth(" + physValWidth_100.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidth", physValWidth_100.toString());
    pTestStep->setConfigValue("setWidth.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidWidthAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidWidthAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidWidthAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidWidthAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidWidthAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidWidthAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidWidthAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidWidthAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidWidthAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidWidthAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidWidthAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidWidthAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidWidthAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidWidthAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingLeftCenter(300, 250)
    ---------------------------------------
    */
    // Set width to half of original width. Thats been done by moving to the top center point of the original rectangle.
    // The resulting center point, when resizing the rectangle by moving the left edge to a new width of 100.0, is the
    // same as setting the width to 300.0 by moving the right edge of the rectangle. But only in this special case.
    ptMoved = ptTopCenterSetAngle30;
    double fWidthSetValidWidthMoveLeftCenterAngle30 = fWidthSetAngle30 / 2.0;
    double fHeightSetValidWidthMoveLeftCenterAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetValidWidthMoveLeftCenterAngle30 = ptCenterSetValidWidthAngle30;
    QPointF ptTopLeftSetValidWidthMoveLeftCenterAngle30 = ptTopCenterSetAngle30;
    QPointF ptTopRightSetValidWidthMoveLeftCenterAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetValidWidthMoveLeftCenterAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetValidWidthMoveLeftCenterAngle30 = ptBottomCenterSetAngle30;
    QPointF ptTopCenterSetValidWidthMoveLeftCenterAngle30 = QPointF((ptTopRightSetValidWidthMoveLeftCenterAngle30.x() + ptTopLeftSetValidWidthMoveLeftCenterAngle30.x()) / 2.0,
                                                       (ptTopRightSetValidWidthMoveLeftCenterAngle30.y() + ptTopLeftSetValidWidthMoveLeftCenterAngle30.y()) / 2.0);
    QPointF ptRightCenterSetValidWidthMoveLeftCenterAngle30 = ptRightCenterSetAngle30;
    QPointF ptBottomCenterSetValidWidthMoveLeftCenterAngle30 = QPointF((ptBottomRightSetValidWidthMoveLeftCenterAngle30.x() + ptBottomLeftSetValidWidthMoveLeftCenterAngle30.x()) / 2.0,
                                                          (ptBottomRightSetValidWidthMoveLeftCenterAngle30.y() + ptBottomLeftSetValidWidthMoveLeftCenterAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetValidWidthMoveLeftCenterAngle30 = QPointF((ptTopLeftSetValidWidthMoveLeftCenterAngle30.x() + ptBottomLeftSetValidWidthMoveLeftCenterAngle30.x()) / 2.0,
                                                        (ptBottomLeftSetValidWidthMoveLeftCenterAngle30.y() + ptTopLeftSetValidWidthMoveLeftCenterAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidWidthMoveLeftCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidWidthMoveLeftCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidWidthMoveLeftCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidWidthMoveLeftCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidWidthMoveLeftCenterAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingLeftCenter(500, 250)
    ---------------------------------------
    */
    // Set width to half of original width. Thats been done by moving to the top center point of the original rectangle.
    // The resulting center point, when resizing the rectangle by moving the left edge to a new width of 100.0, is the
    // same as setting the width to 300.0 by moving the right edge of the rectangle. But only in this special case.
    ptMoved = QPointF(500, 250);
    double fWidthSetInvalidWidthMoveLeftCenterAngle30 = fWidthSetAngle30 / 2.0;
    double fHeightSetInvalidWidthMoveLeftCenterAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetInvalidWidthMoveLeftCenterAngle30 = ptCenterSetValidWidthAngle30;
    QPointF ptTopLeftSetInvalidWidthMoveLeftCenterAngle30 = ptTopCenterSetAngle30;
    QPointF ptTopRightSetInvalidWidthMoveLeftCenterAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetInvalidWidthMoveLeftCenterAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetInvalidWidthMoveLeftCenterAngle30 = ptBottomCenterSetAngle30;
    QPointF ptTopCenterSetInvalidWidthMoveLeftCenterAngle30 = QPointF((ptTopRightSetInvalidWidthMoveLeftCenterAngle30.x() + ptTopLeftSetInvalidWidthMoveLeftCenterAngle30.x()) / 2.0,
                                                       (ptTopRightSetInvalidWidthMoveLeftCenterAngle30.y() + ptTopLeftSetInvalidWidthMoveLeftCenterAngle30.y()) / 2.0);
    QPointF ptRightCenterSetInvalidWidthMoveLeftCenterAngle30 = ptRightCenterSetAngle30;
    QPointF ptBottomCenterSetInvalidWidthMoveLeftCenterAngle30 = QPointF((ptBottomRightSetInvalidWidthMoveLeftCenterAngle30.x() + ptBottomLeftSetInvalidWidthMoveLeftCenterAngle30.x()) / 2.0,
                                                          (ptBottomRightSetInvalidWidthMoveLeftCenterAngle30.y() + ptBottomLeftSetInvalidWidthMoveLeftCenterAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetInvalidWidthMoveLeftCenterAngle30 = QPointF((ptTopLeftSetInvalidWidthMoveLeftCenterAngle30.x() + ptBottomLeftSetInvalidWidthMoveLeftCenterAngle30.x()) / 2.0,
                                                        (ptBottomLeftSetInvalidWidthMoveLeftCenterAngle30.y() + ptTopLeftSetInvalidWidthMoveLeftCenterAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidWidthMoveLeftCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidWidthMoveLeftCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidWidthMoveLeftCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidWidthMoveLeftCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidWidthMoveLeftCenterAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingRightCenter(300, 250)
    ----------------------------------------
    */
    // Same width chosen by moving right center as with setWidth call above.
    ptMoved = ptTopCenterSetAngle30;
    double fWidthSetValidWidthMoveRightCenterAngle30 = fWidthSetAngle30 / 2.0;
    double fHeightSetValidWidthMoveRightCenterAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetValidWidthMoveRightCenterAngle30(256.7, 275.0);
    QPointF ptTopLeftSetValidWidthMoveRightCenterAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetValidWidthMoveRightCenterAngle30 = ptTopCenterSetAngle30;
    QPointF ptBottomRightSetValidWidthMoveRightCenterAngle30 = ptBottomCenterSetAngle30;
    QPointF ptBottomLeftSetValidWidthMoveRightCenterAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetValidWidthMoveRightCenterAngle30 = QPointF((ptTopRightSetValidWidthMoveRightCenterAngle30.x() + ptTopLeftSetValidWidthMoveRightCenterAngle30.x()) / 2.0,
                                                        (ptTopRightSetValidWidthMoveRightCenterAngle30.y() + ptTopLeftSetValidWidthMoveRightCenterAngle30.y()) / 2.0);
    QPointF ptRightCenterSetValidWidthMoveRightCenterAngle30 = QPointF((ptTopRightSetValidWidthMoveRightCenterAngle30.x() + ptBottomRightSetValidWidthMoveRightCenterAngle30.x()) / 2.0,
                                                          (ptTopRightSetValidWidthMoveRightCenterAngle30.y() + ptBottomRightSetValidWidthMoveRightCenterAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetValidWidthMoveRightCenterAngle30 = QPointF((ptBottomRightSetValidWidthMoveRightCenterAngle30.x() + ptBottomLeftSetValidWidthMoveRightCenterAngle30.x()) / 2.0,
                                                           (ptBottomRightSetValidWidthMoveRightCenterAngle30.y() + ptBottomLeftSetValidWidthMoveRightCenterAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetValidWidthMoveRightCenterAngle30 = ptLeftCenterSetAngle30;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidWidthMoveRightCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidWidthMoveRightCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidWidthMoveRightCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidWidthMoveRightCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidWidthMoveRightCenterAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingRightCenter(100, 250)
    ----------------------------------------
    */
    ptMoved = QPointF(100.0, 250.0);
    double fWidthSetInvalidWidthMoveRightCenterAngle30 = fWidthSetAngle30 / 2.0;
    double fHeightSetInvalidWidthMoveRightCenterAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetInvalidWidthMoveRightCenterAngle30(256.7, 275.0);
    QPointF ptTopLeftSetInvalidWidthMoveRightCenterAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetInvalidWidthMoveRightCenterAngle30 = ptTopCenterSetAngle30;
    QPointF ptBottomRightSetInvalidWidthMoveRightCenterAngle30 = ptBottomCenterSetAngle30;
    QPointF ptBottomLeftSetInvalidWidthMoveRightCenterAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetInvalidWidthMoveRightCenterAngle30 = QPointF((ptTopRightSetInvalidWidthMoveRightCenterAngle30.x() + ptTopLeftSetInvalidWidthMoveRightCenterAngle30.x()) / 2.0,
                                                        (ptTopRightSetInvalidWidthMoveRightCenterAngle30.y() + ptTopLeftSetInvalidWidthMoveRightCenterAngle30.y()) / 2.0);
    QPointF ptRightCenterSetInvalidWidthMoveRightCenterAngle30 = QPointF((ptTopRightSetInvalidWidthMoveRightCenterAngle30.x() + ptBottomRightSetInvalidWidthMoveRightCenterAngle30.x()) / 2.0,
                                                          (ptTopRightSetInvalidWidthMoveRightCenterAngle30.y() + ptBottomRightSetInvalidWidthMoveRightCenterAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetInvalidWidthMoveRightCenterAngle30 = QPointF((ptBottomRightSetInvalidWidthMoveRightCenterAngle30.x() + ptBottomLeftSetInvalidWidthMoveRightCenterAngle30.x()) / 2.0,
                                                           (ptBottomRightSetInvalidWidthMoveRightCenterAngle30.y() + ptBottomLeftSetInvalidWidthMoveRightCenterAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetInvalidWidthMoveRightCenterAngle30 = ptLeftCenterSetAngle30;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidWidthMoveRightCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidWidthMoveRightCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidWidthMoveRightCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidWidthMoveRightCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidWidthMoveRightCenterAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeight(200)
    -----------------
    */
    double fWidthSetValidHeightAngle30 = fWidthSetAngle30;
    double fHeightSetValidHeightAngle30 = physValHeight200.getVal();
    QPointF ptCenterSetValidHeightAngle30(275.0, 343.3);
    QPointF ptTopLeftSetValidHeightAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetValidHeightAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetValidHeightAngle30(311.6, 479.9);
    QPointF ptBottomLeftSetValidHeightAngle30(138.4, 379.9);
    QPointF ptTopCenterSetValidHeightAngle30 = ptTopCenterSetAngle30;
    QPointF ptRightCenterSetValidHeightAngle30 = QPointF((ptTopRightSetValidHeightAngle30.x() + ptBottomRightSetValidHeightAngle30.x()) / 2.0,
                                                    (ptBottomRightSetValidHeightAngle30.y() + ptTopRightSetValidHeightAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetValidHeightAngle30 = QPointF((ptBottomRightSetValidHeightAngle30.x() + ptBottomLeftSetValidHeightAngle30.x()) / 2.0,
                                                     (ptBottomRightSetValidHeightAngle30.y() + ptBottomLeftSetValidHeightAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetValidHeightAngle30 = QPointF((ptTopLeftSetValidHeightAngle30.x() + ptBottomLeftSetValidHeightAngle30.x()) / 2.0,
                                                   (ptBottomLeftSetValidHeightAngle30.y() + ptTopLeftSetValidHeightAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeight(" + physValHeight200.toString() + ")",
        /* strOperation    */ "setHeight(" + physValHeight200.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeight", physValHeight200.toString());
    pTestStep->setConfigValue("setHeight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidHeightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidHeightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidHeightAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidHeightAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidHeightAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidHeightAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidHeightAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidHeightAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidHeightAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidHeightAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidHeightAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidHeightAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidHeightAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidHeightAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeight(-100)
    -----------------
    */
    double fWidthSetInvalidHeightAngle30 = fWidthSetAngle30;
    double fHeightSetInvalidHeightAngle30 = physValHeight_100.getVal();
    QPointF ptCenterSetInvalidHeightAngle30(275.0, 343.3);
    QPointF ptTopLeftSetInvalidHeightAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetInvalidHeightAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetInvalidHeightAngle30(311.6, 479.9);
    QPointF ptBottomLeftSetInvalidHeightAngle30(138.4, 379.9);
    QPointF ptTopCenterSetInvalidHeightAngle30 = ptTopCenterSetAngle30;
    QPointF ptRightCenterSetInvalidHeightAngle30 = QPointF((ptTopRightSetInvalidHeightAngle30.x() + ptBottomRightSetInvalidHeightAngle30.x()) / 2.0,
                                                    (ptBottomRightSetInvalidHeightAngle30.y() + ptTopRightSetInvalidHeightAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetInvalidHeightAngle30 = QPointF((ptBottomRightSetInvalidHeightAngle30.x() + ptBottomLeftSetInvalidHeightAngle30.x()) / 2.0,
                                                     (ptBottomRightSetInvalidHeightAngle30.y() + ptBottomLeftSetInvalidHeightAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetInvalidHeightAngle30 = QPointF((ptTopLeftSetInvalidHeightAngle30.x() + ptBottomLeftSetInvalidHeightAngle30.x()) / 2.0,
                                                   (ptBottomLeftSetInvalidHeightAngle30.y() + ptTopLeftSetInvalidHeightAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeight(" + physValHeight200.toString() + ")",
        /* strOperation    */ "setHeight(" + physValHeight200.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeight", physValHeight200.toString());
    pTestStep->setConfigValue("setHeight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidHeightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidHeightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidHeightAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidHeightAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidHeightAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidHeightAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidHeightAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidHeightAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidHeightAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidHeightAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidHeightAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidHeightAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidHeightAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidHeightAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingTopCenter(400, 300)
    ---------------------------------------
    */
    ptMoved = ptRightCenterSetAngle30;
    double fWidthSetValidHeightMoveTopCenterAngle30 = fWidthSetAngle30;
    double fHeightSetValidHeightMoveTopCenterAngle30 = fHeightSetAngle30 / 2.0;
    QPointF ptCenterSetValidHeightMoveTopCenterAngle30(287.49, 321.7);
    QPointF ptTopLeftSetValidHeightMoveTopCenterAngle30 = ptLeftCenterSetAngle30;
    QPointF ptTopRightSetValidHeightMoveTopCenterAngle30 = ptRightCenterSetAngle30;
    QPointF ptBottomRightSetValidHeightMoveTopCenterAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetValidHeightMoveTopCenterAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetValidHeightMoveTopCenterAngle30 = QPointF((ptTopRightSetValidHeightMoveTopCenterAngle30.x() + ptTopLeftSetValidHeightMoveTopCenterAngle30.x()) / 2.0,
                                                      (ptTopRightSetValidHeightMoveTopCenterAngle30.y() + ptTopLeftSetValidHeightMoveTopCenterAngle30.y()) / 2.0);
    QPointF ptRightCenterSetValidHeightMoveTopCenterAngle30 = QPointF((ptTopRightSetValidHeightMoveTopCenterAngle30.x() + ptBottomRightSetValidHeightMoveTopCenterAngle30.x()) / 2.0,
                                                        (ptTopRightSetValidHeightMoveTopCenterAngle30.y() + ptBottomRightSetValidHeightMoveTopCenterAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetValidHeightMoveTopCenterAngle30 = ptBottomCenterSetAngle30;
    QPointF ptLeftCenterSetValidHeightMoveTopCenterAngle30 = QPointF((ptTopLeftSetValidHeightMoveTopCenterAngle30.x() + ptBottomLeftSetValidHeightMoveTopCenterAngle30.x()) / 2.0,
                                                       (ptBottomLeftSetValidHeightMoveTopCenterAngle30.y() + ptTopLeftSetValidHeightMoveTopCenterAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidHeightMoveTopCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidHeightMoveTopCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidHeightMoveTopCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidHeightMoveTopCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidHeightMoveTopCenterAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingTopCenter(400, 400)
    ---------------------------------------
    */
    ptMoved = QPointF(400, 400);
    double fWidthSetInvalidHeightMoveTopCenterAngle30 = fWidthSetAngle30;
    double fHeightSetInvalidHeightMoveTopCenterAngle30 = fHeightSetAngle30 / 2.0;
    QPointF ptCenterSetInvalidHeightMoveTopCenterAngle30(287.49, 321.7);
    QPointF ptTopLeftSetInvalidHeightMoveTopCenterAngle30 = ptLeftCenterSetAngle30;
    QPointF ptTopRightSetInvalidHeightMoveTopCenterAngle30 = ptRightCenterSetAngle30;
    QPointF ptBottomRightSetInvalidHeightMoveTopCenterAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetInvalidHeightMoveTopCenterAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetInvalidHeightMoveTopCenterAngle30 = QPointF((ptTopRightSetInvalidHeightMoveTopCenterAngle30.x() + ptTopLeftSetInvalidHeightMoveTopCenterAngle30.x()) / 2.0,
                                                      (ptTopRightSetInvalidHeightMoveTopCenterAngle30.y() + ptTopLeftSetInvalidHeightMoveTopCenterAngle30.y()) / 2.0);
    QPointF ptRightCenterSetInvalidHeightMoveTopCenterAngle30 = QPointF((ptTopRightSetInvalidHeightMoveTopCenterAngle30.x() + ptBottomRightSetInvalidHeightMoveTopCenterAngle30.x()) / 2.0,
                                                        (ptTopRightSetInvalidHeightMoveTopCenterAngle30.y() + ptBottomRightSetInvalidHeightMoveTopCenterAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetInvalidHeightMoveTopCenterAngle30 = ptBottomCenterSetAngle30;
    QPointF ptLeftCenterSetInvalidHeightMoveTopCenterAngle30 = QPointF((ptTopLeftSetInvalidHeightMoveTopCenterAngle30.x() + ptBottomLeftSetInvalidHeightMoveTopCenterAngle30.x()) / 2.0,
                                                       (ptBottomLeftSetInvalidHeightMoveTopCenterAngle30.y() + ptTopLeftSetInvalidHeightMoveTopCenterAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidHeightMoveTopCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidHeightMoveTopCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidHeightMoveTopCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidHeightMoveTopCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidHeightMoveTopCenterAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingBottomCenter(400, 300)
    ------------------------------------------
    */
    ptMoved = ptRightCenterSetAngle30;
    double fWidthSetValidHeightMoveBottomCenter30 = fWidthSetValidHeightAngle30;
    double fHeightSetValidHeightMoveBottomCenter30 = fHeightSetAngle30 / 2.0;
    QPointF ptCenterSetValidHeightMoveBottomCenter30(312.49, 278.4);
    QPointF ptTopLeftSetValidHeightMoveBottomCenter30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetValidHeightMoveBottomCenter30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetValidHeightMoveBottomCenter30 = ptRightCenterSetAngle30;
    QPointF ptBottomLeftSetValidHeightMoveBottomCenter30 = ptLeftCenterSetAngle30;
    QPointF ptTopCenterSetValidHeightMoveBottomCenter30 = ptTopCenterSetAngle30;
    QPointF ptRightCenterSetValidHeightMoveBottomCenter30 = QPointF((ptTopRightSetValidHeightMoveBottomCenter30.x() + ptBottomRightSetValidHeightMoveBottomCenter30.x()) / 2.0,
                                                           (ptTopRightSetValidHeightMoveBottomCenter30.y() + ptBottomRightSetValidHeightMoveBottomCenter30.y()) / 2.0);
    QPointF ptBottomCenterSetValidHeightMoveBottomCenter30 = QPointF((ptBottomLeftSetValidHeightMoveBottomCenter30.x() + ptBottomRightSetValidHeightMoveBottomCenter30.x()) / 2.0,
                                                            (ptBottomLeftSetValidHeightMoveBottomCenter30.y() + ptBottomRightSetValidHeightMoveBottomCenter30.y()) / 2.0);;
    QPointF ptLeftCenterSetValidHeightMoveBottomCenter30 = QPointF((ptTopLeftSetValidHeightMoveBottomCenter30.x() + ptBottomLeftSetValidHeightMoveBottomCenter30.x()) / 2.0,
                                                          (ptBottomLeftSetValidHeightMoveBottomCenter30.y() + ptTopLeftSetValidHeightMoveBottomCenter30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidHeightMoveBottomCenter30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidHeightMoveBottomCenter30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetValidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetValidHeightMoveBottomCenter30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetValidHeightMoveBottomCenter30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetValidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetValidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetValidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetValidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetValidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetValidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetValidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetValidHeightMoveBottomCenter30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingBottomCenter(400, 200)
    ------------------------------------------
    */
    ptMoved = QPointF(400, 200);
    double fWidthSetInvalidHeightMoveBottomCenter30 = fWidthSetValidHeightAngle30;
    double fHeightSetInvalidHeightMoveBottomCenter30 = fHeightSetAngle30 / 2.0;
    QPointF ptCenterSetInvalidHeightMoveBottomCenter30(312.49, 278.4);
    QPointF ptTopLeftSetInvalidHeightMoveBottomCenter30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetInvalidHeightMoveBottomCenter30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetInvalidHeightMoveBottomCenter30 = ptRightCenterSetAngle30;
    QPointF ptBottomLeftSetInvalidHeightMoveBottomCenter30 = ptLeftCenterSetAngle30;
    QPointF ptTopCenterSetInvalidHeightMoveBottomCenter30 = ptTopCenterSetAngle30;
    QPointF ptRightCenterSetInvalidHeightMoveBottomCenter30 = QPointF((ptTopRightSetInvalidHeightMoveBottomCenter30.x() + ptBottomRightSetInvalidHeightMoveBottomCenter30.x()) / 2.0,
                                                           (ptTopRightSetInvalidHeightMoveBottomCenter30.y() + ptBottomRightSetInvalidHeightMoveBottomCenter30.y()) / 2.0);
    QPointF ptBottomCenterSetInvalidHeightMoveBottomCenter30 = QPointF((ptBottomLeftSetInvalidHeightMoveBottomCenter30.x() + ptBottomRightSetInvalidHeightMoveBottomCenter30.x()) / 2.0,
                                                            (ptBottomLeftSetInvalidHeightMoveBottomCenter30.y() + ptBottomRightSetInvalidHeightMoveBottomCenter30.y()) / 2.0);;
    QPointF ptLeftCenterSetInvalidHeightMoveBottomCenter30 = QPointF((ptTopLeftSetInvalidHeightMoveBottomCenter30.x() + ptBottomLeftSetInvalidHeightMoveBottomCenter30.x()) / 2.0,
                                                          (ptBottomLeftSetInvalidHeightMoveBottomCenter30.y() + ptTopLeftSetInvalidHeightMoveBottomCenter30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidHeightMoveBottomCenter30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidHeightMoveBottomCenter30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetInvalidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetInvalidHeightMoveBottomCenter30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetInvalidHeightMoveBottomCenter30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetInvalidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetInvalidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetInvalidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetInvalidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetInvalidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetInvalidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetInvalidHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetInvalidHeightMoveBottomCenter30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(300, 200)
    -----------------------
    */
    QPointF ptTopLeftValidSizeAngle30 = QPointF(300.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopLeftValidSizeAngle30.setY(400.0);
    }
    double fWidthSetTopLeftValidSizeAngle30 = 286.6;
    double fHeightSetTopLeftValidSizeAngle30 = 173.2;
    QPointF ptCenterSetTopLeftValidSizeAngle30(280.8, 246.7);
    QPointF ptTopLeftSetTopLeftValidSizeAngle30 = ptTopLeftValidSizeAngle30;
    QPointF ptTopRightSetTopLeftValidSizeAngle30(448.2, 243.3);
    QPointF ptBottomRightSetTopLeftValidSizeAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetTopLeftValidSizeAngle30(113.4, 250.0);
    QPointF ptTopCenterSetTopLeftValidSizeAngle30 = QPointF((ptTopRightSetTopLeftValidSizeAngle30.x() + ptTopLeftSetTopLeftValidSizeAngle30.x()) / 2.0,
                                                (ptTopRightSetTopLeftValidSizeAngle30.y() + ptTopLeftSetTopLeftValidSizeAngle30.y()) / 2.0);
    QPointF ptRightCenterSetTopLeftValidSizeAngle30 = QPointF((ptTopRightSetTopLeftValidSizeAngle30.x() + ptBottomRightSetTopLeftValidSizeAngle30.x()) / 2.0,
                                                  (ptBottomRightSetTopLeftValidSizeAngle30.y() + ptTopRightSetTopLeftValidSizeAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetTopLeftValidSizeAngle30(237.49, 321.7); // not calucated because of inaccuracy
    QPointF ptLeftCenterSetTopLeftValidSizeAngle30 = QPointF((ptTopLeftSetTopLeftValidSizeAngle30.x() + ptBottomLeftSetTopLeftValidSizeAngle30.x()) / 2.0,
                                                 (ptBottomLeftSetTopLeftValidSizeAngle30.y() + ptTopLeftSetTopLeftValidSizeAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(" + qPoint2Str(ptTopLeftValidSizeAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptTopLeftValidSizeAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptTopLeftValidSizeAngle30);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftValidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftValidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftValidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftValidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftValidSizeAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(500, 400)
    -----------------------
    */
    QPointF ptTopLeftInvalidSizeAngle30 = QPointF(500.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopLeftInvalidSizeAngle30.setY(200.0);
    }
    double fWidthSetTopLeftInvalidSizeAngle30 = 286.6;
    double fHeightSetTopLeftInvalidSizeAngle30 = 173.2;
    QPointF ptCenterSetTopLeftInvalidSizeAngle30(280.8, 246.7);
    QPointF ptTopLeftSetTopLeftInvalidSizeAngle30 = ptTopLeftInvalidSizeAngle30;
    QPointF ptTopRightSetTopLeftInvalidSizeAngle30(448.2, 243.3);
    QPointF ptBottomRightSetTopLeftInvalidSizeAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetTopLeftInvalidSizeAngle30(113.4, 250.0);
    QPointF ptTopCenterSetTopLeftInvalidSizeAngle30 = QPointF((ptTopRightSetTopLeftInvalidSizeAngle30.x() + ptTopLeftSetTopLeftInvalidSizeAngle30.x()) / 2.0,
                                                (ptTopRightSetTopLeftInvalidSizeAngle30.y() + ptTopLeftSetTopLeftInvalidSizeAngle30.y()) / 2.0);
    QPointF ptRightCenterSetTopLeftInvalidSizeAngle30 = QPointF((ptTopRightSetTopLeftInvalidSizeAngle30.x() + ptBottomRightSetTopLeftInvalidSizeAngle30.x()) / 2.0,
                                                  (ptBottomRightSetTopLeftInvalidSizeAngle30.y() + ptTopRightSetTopLeftInvalidSizeAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetTopLeftInvalidSizeAngle30(237.49, 321.7); // not calucated because of inaccuracy
    QPointF ptLeftCenterSetTopLeftInvalidSizeAngle30 = QPointF((ptTopLeftSetTopLeftInvalidSizeAngle30.x() + ptBottomLeftSetTopLeftInvalidSizeAngle30.x()) / 2.0,
                                                 (ptBottomLeftSetTopLeftInvalidSizeAngle30.y() + ptTopLeftSetTopLeftInvalidSizeAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(" + qPoint2Str(ptTopLeftInvalidSizeAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptTopLeftInvalidSizeAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptTopLeftInvalidSizeAngle30);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftInvalidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftInvalidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftInvalidSizeAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftInvalidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftInvalidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftInvalidSizeAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(300, 200)
    ------------------------
    */
    QPointF ptTopRightValidSizeAngle30 = QPointF(300.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopRightValidSizeAngle30.setY(350.0);
    }
    double fWidthSetTopRightValidSizeAngle30 = 291.5;
    double fHeightSetTopRightValidSizeAngle30 = 218.3;
    QPointF ptCenterSetTopRightValidSizeAngle30(369.2, 271.7);
    QPointF ptTopLeftSetTopRightValidSizeAngle30(297.5, 104.2);
    QPointF ptTopRightSetTopRightValidSizeAngle30 = ptTopRightValidSizeAngle30;
    QPointF ptBottomRightSetTopRightValidSizeAngle30(440.9, 439.1);
    QPointF ptBottomLeftSetTopRightValidSizeAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetTopRightValidSizeAngle30 = QPointF((ptTopRightSetTopRightValidSizeAngle30.x() + ptTopLeftSetTopRightValidSizeAngle30.x()) / 2.0,
                                                (ptTopRightSetTopRightValidSizeAngle30.y() + ptTopLeftSetTopRightValidSizeAngle30.y()) / 2.0);
    QPointF ptRightCenterSetTopRightValidSizeAngle30(495.4, 344.5);  // not calculated because of inaccuracy
    QPointF ptBottomCenterSetTopRightValidSizeAngle30(314.6, 366.2); // not calculated because of inaccuracy
    QPointF ptLeftCenterSetTopRightValidSizeAngle30(243.0, 198.8);   // not calculated because of inaccuracy
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(" + qPoint2Str(ptTopRightValidSizeAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptTopRightValidSizeAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptTopRightValidSizeAngle30);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightValidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightValidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightValidSizeAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightValidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightValidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightValidSizeAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(100, 400)
    ------------------------
    */
    QPointF ptTopRightInvalidSizeAngle30 = QPointF(100.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopRightInvalidSizeAngle30.setY(200.0);
    }
    double fWidthSetTopRightInvalidSizeAngle30 = 291.5;
    double fHeightSetTopRightInvalidSizeAngle30 = 218.3;
    QPointF ptCenterSetTopRightInvalidSizeAngle30(369.2, 271.7);
    QPointF ptTopLeftSetTopRightInvalidSizeAngle30(297.5, 104.2);
    QPointF ptTopRightSetTopRightInvalidSizeAngle30 = ptTopRightInvalidSizeAngle30;
    QPointF ptBottomRightSetTopRightInvalidSizeAngle30(440.9, 439.1);
    QPointF ptBottomLeftSetTopRightInvalidSizeAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetTopRightInvalidSizeAngle30 = QPointF((ptTopRightSetTopRightInvalidSizeAngle30.x() + ptTopLeftSetTopRightInvalidSizeAngle30.x()) / 2.0,
                                                (ptTopRightSetTopRightInvalidSizeAngle30.y() + ptTopLeftSetTopRightInvalidSizeAngle30.y()) / 2.0);
    QPointF ptRightCenterSetTopRightInvalidSizeAngle30(495.4, 344.5);  // not calculated because of inaccuracy
    QPointF ptBottomCenterSetTopRightInvalidSizeAngle30(314.6, 366.2); // not calculated because of inaccuracy
    QPointF ptLeftCenterSetTopRightInvalidSizeAngle30(243.0, 198.8);   // not calculated because of inaccuracy
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(" + qPoint2Str(ptTopRightInvalidSizeAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptTopRightInvalidSizeAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptTopRightInvalidSizeAngle30);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightInvalidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightInvalidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightInvalidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightInvalidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightInvalidSizeAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight
    -----------------
    */
    QPointF ptBottomRightValidSizeAngle30 = QPointF(400.0, 500.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomRightValidSizeAngle30.setY(100.0);
    }
    double fWidthSetBottomRightValidSizeAngle30 = 286.6;
    double fHeightSetBottomRightValidSizeAngle30 = 173.2;
    QPointF ptCenterSetBottomRightValidSizeAngle30 (319.2, 353.4);
    QPointF ptTopLeftSetBottomRightValidSizeAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetBottomRightValidSizeAngle30(486.6, 350.0);
    QPointF ptBottomRightSetBottomRightValidSizeAngle30 = ptBottomRightValidSizeAngle30;
    QPointF ptBottomLeftSetBottomRightValidSizeAngle30(151.8, 356.7);
    QPointF ptTopCenterSetBottomRightValidSizeAngle30 = QPointF((ptTopRightSetBottomRightValidSizeAngle30.x() + ptTopLeftSetBottomRightValidSizeAngle30.x()) / 2.0,
                                                (ptTopRightSetBottomRightValidSizeAngle30.y() + ptTopLeftSetBottomRightValidSizeAngle30.y()) / 2.0);
    QPointF ptRightCenterSetBottomRightValidSizeAngle30 = QPointF((ptTopRightSetBottomRightValidSizeAngle30.x() + ptBottomRightSetBottomRightValidSizeAngle30.x()) / 2.0,
                                                  (ptBottomRightSetBottomRightValidSizeAngle30.y() + ptTopRightSetBottomRightValidSizeAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetBottomRightValidSizeAngle30 = QPointF((ptBottomRightSetBottomRightValidSizeAngle30.x() + ptBottomLeftSetBottomRightValidSizeAngle30.x()) / 2.0,
                                                   (ptBottomRightSetBottomRightValidSizeAngle30.y() + ptBottomLeftSetBottomRightValidSizeAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetBottomRightValidSizeAngle30 = QPointF((ptTopLeftSetBottomRightValidSizeAngle30.x() + ptBottomLeftSetBottomRightValidSizeAngle30.x()) / 2.0,
                                                 (ptBottomLeftSetBottomRightValidSizeAngle30.y() + ptTopLeftSetBottomRightValidSizeAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(" + qPoint2Str(ptBottomRightValidSizeAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptBottomRightValidSizeAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptBottomRightValidSizeAngle30);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightValidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightValidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightValidSizeAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightValidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightValidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightValidSizeAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight()
    -------------------
    */
    QPointF ptBottomRightInvalidSizeAngle30 = QPointF(400.0, 500.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomRightInvalidSizeAngle30.setY(100.0);
    }
    double fWidthSetBottomRightInvalidSizeAngle30 = 286.6;
    double fHeightSetBottomRightInvalidSizeAngle30 = 173.2;
    QPointF ptCenterSetBottomRightInvalidSizeAngle30 (319.2, 353.4);
    QPointF ptTopLeftSetBottomRightInvalidSizeAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetBottomRightInvalidSizeAngle30(486.6, 350.0);
    QPointF ptBottomRightSetBottomRightInvalidSizeAngle30 = ptBottomRightInvalidSizeAngle30;
    QPointF ptBottomLeftSetBottomRightInvalidSizeAngle30(151.8, 356.7);
    QPointF ptTopCenterSetBottomRightInvalidSizeAngle30 = QPointF((ptTopRightSetBottomRightInvalidSizeAngle30.x() + ptTopLeftSetBottomRightInvalidSizeAngle30.x()) / 2.0,
                                                (ptTopRightSetBottomRightInvalidSizeAngle30.y() + ptTopLeftSetBottomRightInvalidSizeAngle30.y()) / 2.0);
    QPointF ptRightCenterSetBottomRightInvalidSizeAngle30 = QPointF((ptTopRightSetBottomRightInvalidSizeAngle30.x() + ptBottomRightSetBottomRightInvalidSizeAngle30.x()) / 2.0,
                                                  (ptBottomRightSetBottomRightInvalidSizeAngle30.y() + ptTopRightSetBottomRightInvalidSizeAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetBottomRightInvalidSizeAngle30 = QPointF((ptBottomRightSetBottomRightInvalidSizeAngle30.x() + ptBottomLeftSetBottomRightInvalidSizeAngle30.x()) / 2.0,
                                                   (ptBottomRightSetBottomRightInvalidSizeAngle30.y() + ptBottomLeftSetBottomRightInvalidSizeAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetBottomRightInvalidSizeAngle30 = QPointF((ptTopLeftSetBottomRightInvalidSizeAngle30.x() + ptBottomLeftSetBottomRightInvalidSizeAngle30.x()) / 2.0,
                                                 (ptBottomLeftSetBottomRightInvalidSizeAngle30.y() + ptTopLeftSetBottomRightInvalidSizeAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(" + qPoint2Str(ptBottomRightInvalidSizeAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptBottomRightInvalidSizeAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptBottomRightInvalidSizeAngle30);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightInvalidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightInvalidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightInvalidSizeAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightInvalidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightInvalidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightInvalidSizeAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft
    ----------------
    */
    QPointF ptBottomLeftValidSizeAngle30 = QPointF(100.0, 350.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomLeftValidSizeAngle30.setY(250.0);
    }
    double fWidthSetBottomLeftValidSizeAngle30 = 248.2;
    double fHeightSetBottomLeftValidSizeAngle30 = 193.3;
    QPointF ptCenterSetBottomLeftValidSizeAngle30(255.8, 328.4);
    QPointF ptTopLeftSetBottomLeftValidSizeAngle30(196.7, 182.6);
    QPointF ptTopRightSetBottomLeftValidSizeAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetBottomLeftValidSizeAngle30(315.0, 474.1);
    QPointF ptBottomLeftSetBottomLeftValidSizeAngle30 = ptBottomLeftValidSizeAngle30;
    QPointF ptTopCenterSetBottomLeftValidSizeAngle30(304.1, 244.6); // because of calculation inaccurracy
    QPointF ptRightCenterSetBottomLeftValidSizeAngle30 = QPointF((ptTopRightSetBottomLeftValidSizeAngle30.x() + ptBottomRightSetBottomLeftValidSizeAngle30.x()) / 2.0,
                                                  (ptBottomRightSetBottomLeftValidSizeAngle30.y() + ptTopRightSetBottomLeftValidSizeAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetBottomLeftValidSizeAngle30(207.49, 412.1); // because of calculation inaccurracy
    QPointF ptLeftCenterSetBottomLeftValidSizeAngle30(148.3, 266.3); // because of calculation inaccurracy
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(" + qPoint2Str(ptBottomLeftValidSizeAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptBottomLeftValidSizeAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptBottomLeftValidSizeAngle30);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftValidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftValidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftValidSizeAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftValidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftValidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftValidSizeAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftValidSizeAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft
    ----------------
    */
    QPointF ptBottomLeftSizeInvalidSizeAngle30 = QPointF(100.0, 350.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomLeftSizeInvalidSizeAngle30.setY(250.0);
    }
    double fWidthSetBottomLeftSizeInvalidSizeAngle30 = 248.2;
    double fHeightSetBottomLeftSizeInvalidSizeAngle30 = 193.3;
    QPointF ptCenterSetBottomLeftSizeInvalidSizeAngle30(255.8, 328.4);
    QPointF ptTopLeftSetBottomLeftSizeInvalidSizeAngle30(196.7, 182.6);
    QPointF ptTopRightSetBottomLeftSizeInvalidSizeAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetBottomLeftSizeInvalidSizeAngle30(315.0, 474.1);
    QPointF ptBottomLeftSetBottomLeftSizeInvalidSizeAngle30 = ptBottomLeftSizeInvalidSizeAngle30;
    QPointF ptTopCenterSetBottomLeftSizeInvalidSizeAngle30(304.1, 244.6); // because of calculation inaccurracy
    QPointF ptRightCenterSetBottomLeftSizeInvalidSizeAngle30 = QPointF((ptTopRightSetBottomLeftSizeInvalidSizeAngle30.x() + ptBottomRightSetBottomLeftSizeInvalidSizeAngle30.x()) / 2.0,
                                                  (ptBottomRightSetBottomLeftSizeInvalidSizeAngle30.y() + ptTopRightSetBottomLeftSizeInvalidSizeAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetBottomLeftSizeInvalidSizeAngle30(207.49, 412.1); // because of calculation inaccurracy
    QPointF ptLeftCenterSetBottomLeftSizeInvalidSizeAngle30(148.3, 266.3); // because of calculation inaccurracy
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(" + qPoint2Str(ptBottomLeftSizeInvalidSizeAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptBottomLeftSizeInvalidSizeAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptBottomLeftSizeInvalidSizeAngle30);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftSizeInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftSizeInvalidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftSizeInvalidSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftSizeInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftSizeInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftSizeInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftSizeInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftSizeInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftSizeInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftSizeInvalidSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftSizeInvalidSizeAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftSizeInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftSizeInvalidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftSizeInvalidSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftSizeInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftSizeInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftSizeInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftSizeInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftSizeInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftSizeInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftSizeInvalidSizeAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftSizeInvalidSizeAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpTransformRect->path());
    }
    return pGrpTransformRect;

} // createTestGroupImageSizeAndObjectCoordinatesTransformPhysValRect

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
    strlstExpectedValues.append("line.pos {50.0, 0.0} px");
    strlstExpectedValues.append("line.p1 {-50.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {50.0, 0.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    strlstExpectedValues.append("position {50, 0} px");
    strlstExpectedValues.append("getLine.p1.x: 0 px");
    strlstExpectedValues.append("getLine.p1.y: 0 px");
    strlstExpectedValues.append("getLine.p2.x: 100 px");
    strlstExpectedValues.append("getLine.p2.y: 0 px");
    strlstExpectedValues.append("getLength: 100 px");
    strlstExpectedValues.append("rotationAngle: 0.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append("line.pos {100.0, 50.0} px");
    strlstExpectedValues.append("line.p1 {0.0, -50.0} px");
    strlstExpectedValues.append("line.p2 {0.0, 50.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    strlstExpectedValues.append("position {100, 50} px");
    strlstExpectedValues.append("getLine.p1.x: 100 px");
    strlstExpectedValues.append("getLine.p1.y: 0 px");
    strlstExpectedValues.append("getLine.p2.x: 100 px");
    strlstExpectedValues.append("getLine.p2.y: 100 px");
    strlstExpectedValues.append("getLength: 100 px");
    strlstExpectedValues.append("rotationAngle: 90.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append("line.pos {50.0, 100.0} px");
    strlstExpectedValues.append("line.p1 {50.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {-50.0, 0.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    strlstExpectedValues.append("position {50, 100} px");
    strlstExpectedValues.append("getLine.p1.x: 100 px");
    strlstExpectedValues.append("getLine.p1.y: 100 px");
    strlstExpectedValues.append("getLine.p2.x: 0 px");
    strlstExpectedValues.append("getLine.p2.y: 100 px");
    strlstExpectedValues.append("getLength: 100 px");
    strlstExpectedValues.append("rotationAngle: 180.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append("line.pos {0.0, 50.0} px");
    strlstExpectedValues.append("line.p1 {0.0, 50.0} px");
    strlstExpectedValues.append("line.p2 {0.0, -50.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    strlstExpectedValues.append("position {0, 50} px");
    strlstExpectedValues.append("getLine.p1.x: 0 px");
    strlstExpectedValues.append("getLine.p1.y: 100 px");
    strlstExpectedValues.append("getLine.p2.x: 0 px");
    strlstExpectedValues.append("getLine.p2.y: 0 px");
    strlstExpectedValues.append("getLength: 100 px");
    strlstExpectedValues.append("rotationAngle: 270.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append("line.pos {175.0, 0.0} px");
    strlstExpectedValues.append("line.p1 {-175.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {175.0, 0.0} px");
    strlstExpectedValues.append("line.length: 350.0 px");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("position {50.0, 0.0} mm");
        strlstExpectedValues.append("getLine.p1.x: 0.0 mm");
        strlstExpectedValues.append("getLine.p1.y: 0.0 mm");
        strlstExpectedValues.append("getLine.p2.x: 100.0 mm");
        strlstExpectedValues.append("getLine.p2.y: 0.0 mm");
    }
    else {
        strlstExpectedValues.append("position {50.0, 100.0} mm");
        strlstExpectedValues.append("getLine.p1.x: 0.0 mm");
        strlstExpectedValues.append("getLine.p1.y: 100.0 mm");
        strlstExpectedValues.append("getLine.p2.x: 100.0 mm");
        strlstExpectedValues.append("getLine.p2.y: 100.0 mm");
    }
    strlstExpectedValues.append("getLength: 100.0 mm");
    strlstExpectedValues.append("rotationAngle: 0.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append("line.pos {350.0, 175.0} px");
    strlstExpectedValues.append("line.p1 {0.0, -175.0} px");
    strlstExpectedValues.append("line.p2 {0.0, 175.0} px");
    strlstExpectedValues.append("line.length: 350.0 px");
    strlstExpectedValues.append("position {100.0, 50.0} mm");
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
    strlstExpectedValues.append("rotationAngle: 90.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append("line.pos {175.0, 350.0} px");
    strlstExpectedValues.append("line.p1 {175.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {-175.0, 0.0} px");
    strlstExpectedValues.append("line.length: 350.0 px");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("position {50.0, 100.0} mm");
        strlstExpectedValues.append("getLine.p1.x: 100.0 mm");
        strlstExpectedValues.append("getLine.p1.y: 100.0 mm");
        strlstExpectedValues.append("getLine.p2.x: 0.0 mm");
        strlstExpectedValues.append("getLine.p2.y: 100.0 mm");
    }
    else {
        strlstExpectedValues.append("position {50.0, 0.0} mm");
        strlstExpectedValues.append("getLine.p1.x: 100.0 mm");
        strlstExpectedValues.append("getLine.p1.y: 0.0 mm");
        strlstExpectedValues.append("getLine.p2.x: 0.0 mm");
        strlstExpectedValues.append("getLine.p2.y: 0.0 mm");
    }
    strlstExpectedValues.append("getLength: 100.0 mm");
    strlstExpectedValues.append("rotationAngle: 180.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append("line.pos {0.0, 175.0} px");
    strlstExpectedValues.append("line.p1 {0.0, 175.0} px");
    strlstExpectedValues.append("line.p2 {0.0, -175.0} px");
    strlstExpectedValues.append("line.length: 350.0 px");
    strlstExpectedValues.append("position {0.0, 50.0} mm");
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
    strlstExpectedValues.append("rotationAngle: 270.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append("line.pos {175.0, 175.0} px");
    strlstExpectedValues.append("line.p1 {-75.0, -75.0} px");
    strlstExpectedValues.append("line.p2 {75.0, 75.0} px");
    strlstExpectedValues.append("line.length: 212.1 px");
    strlstExpectedValues.append("position {50.0, 50.0} mm");
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
    strlstExpectedValues.append("rotationAngle: 45.0 " + Math::c_strSymbolDegree);
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
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("P1 {0.0, 0.0} mm");
        strlstExpectedValues.append("P2 {100.0, 0.0} mm");
        strlstExpectedValues.append("Center {50.0, 0.0} mm");
    }
    else {
        strlstExpectedValues.append("P1 {0.0, 100.0} mm");
        strlstExpectedValues.append("P2 {100.0, 100.0} mm");
        strlstExpectedValues.append("Center {50.0, 100.0} mm");
    }
    strlstExpectedValues.append("dx: 100.0 mm");
    strlstExpectedValues.append("dy: 0.0 mm");
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
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("P1 {100.0, 0.0} mm");
        strlstExpectedValues.append("P2 {100.0, 100.0} mm");
        strlstExpectedValues.append("Center {100.0, 50.0} mm");
        strlstExpectedValues.append("dx: 0.0 mm");
        strlstExpectedValues.append("dy: 100.0 mm");
    }
    else {
        strlstExpectedValues.append("P1 {100.0, 100.0} mm");
        strlstExpectedValues.append("P2 {100.0, 0.0} mm");
        strlstExpectedValues.append("Center {100.0, 50.0} mm");
        strlstExpectedValues.append("dx: 0.0 mm");
        strlstExpectedValues.append("dy: -100.0 mm");
    }
    strlstExpectedValues.append("Length: 100.0 mm");
    strlstExpectedValues.append("Angle: 90.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(P1(350, 350), P2(0, 350)) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Line(P1(350, 350), P2(0, 350)) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValLinePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("Line", QLineF(350.0, 350.0, 0.0, 350.0));
    pTestStep->setConfigValue("Line.Unit", "px");
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("P1 {100.0, 100.0} mm");
        strlstExpectedValues.append("P2 {0.0, 100.0} mm");
        strlstExpectedValues.append("Center {50.0, 100.0} mm");
    }
    else {
        strlstExpectedValues.append("P1 {100.0, 0.0} mm");
        strlstExpectedValues.append("P2 {0.0, 0.0} mm");
        strlstExpectedValues.append("Center {50.0, 0.0} mm");
    }
    strlstExpectedValues.append("dx: -100.0 mm");
    strlstExpectedValues.append("dy: 0.0 mm");
    strlstExpectedValues.append("Length: 100.0 mm");
    strlstExpectedValues.append("Angle: 180.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(P1(0, 350), P2(0, 0)) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Line(P1(0, 350), P2(0, 0)) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValLinePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("Line", QLineF(0.0, 350.0, 0.0, 0.0));
    pTestStep->setConfigValue("Line.Unit", "px");
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("P1 {0.0, 100.0} mm");
        strlstExpectedValues.append("P2 {0.0, 0.0} mm");
        strlstExpectedValues.append("Center {0.0, 50.0} mm");
        strlstExpectedValues.append("dx: 0.0 mm");
        strlstExpectedValues.append("dy: -100.0 mm");
    }
    else {
        strlstExpectedValues.append("P1 {0.0, 0.0} mm");
        strlstExpectedValues.append("P2 {0.0, 100.0} mm");
        strlstExpectedValues.append("Center {0.0, 50.0} mm");
        strlstExpectedValues.append("dx: 0.0 mm");
        strlstExpectedValues.append("dy: 100.0 mm");
    }
    strlstExpectedValues.append("Length: 100.0 mm");
    strlstExpectedValues.append("Angle: 270.0 " + Math::c_strSymbolDegree);
    pTestStep->setExpectedValues(strlstExpectedValues);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " convert(Line(P1(100, 100), P2(250, 250)) px, mm)",
        /* strOperation    */ "DrawingScene.convert(Line(P1(100, 100), P2(250, 250)) px, mm)",
        /* pGrpParent      */ pGrpConvertToPhysValLinePx2MM,
        /* szDoTestStepFct */ SLOT(doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep*)));
    pTestStep->setConfigValue("Line", QLineF(100.0, 100.0, 250.0, 250.0));
    pTestStep->setConfigValue("Line.Unit", "px");
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("P1 {28.6, 28.6} mm");
        strlstExpectedValues.append("P2 {71.4, 71.4} mm");
        strlstExpectedValues.append("Center {50.0, 50.0} mm");
        strlstExpectedValues.append("dx: 42.9 mm");
        strlstExpectedValues.append("dy: 42.9 mm");
    }
    else {
        strlstExpectedValues.append("P1 {28.6, 71.4} mm");
        strlstExpectedValues.append("P2 {71.4, 28.6} mm");
        strlstExpectedValues.append("Center {50.0, 50.0} mm");
        strlstExpectedValues.append("dx: 42.9 mm");
        strlstExpectedValues.append("dy: -42.9 mm");
    }
    strlstExpectedValues.append("Length: 60.6 mm");
    strlstExpectedValues.append("Angle: 45.0 " + Math::c_strSymbolDegree);
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
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("P1 {350, 0} px");
    strlstExpectedValues.append("P2 {350, 350} px");
    strlstExpectedValues.append("Center {350, 175} px");
    strlstExpectedValues.append("dx: 0 px");
    strlstExpectedValues.append("dy: 350 px");
    strlstExpectedValues.append("Length: 350 px");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("Angle: 90.0 " + Math::c_strSymbolDegree);
    }
    else {
        strlstExpectedValues.append("Angle: 270.0 " + Math::c_strSymbolDegree);
    }
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
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("P1 {0, 350} px");
    strlstExpectedValues.append("P2 {0, 0} px");
    strlstExpectedValues.append("Center {0, 175} px");
    strlstExpectedValues.append("dx: 0 px");
    strlstExpectedValues.append("dy: -350 px");
    strlstExpectedValues.append("Length: 350 px");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("Angle: 270.0 " + Math::c_strSymbolDegree);
    }
    else {
        strlstExpectedValues.append("Angle: 90.0 " + Math::c_strSymbolDegree);
    }
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
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("P1 {100, 100} px");
    strlstExpectedValues.append("P2 {250, 250} px");
    strlstExpectedValues.append("Center {175, 175} px");
    strlstExpectedValues.append("dx: 150 px");
    strlstExpectedValues.append("dy: 150 px");
    strlstExpectedValues.append("Length: 212 px");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("Angle: 45.0 " + Math::c_strSymbolDegree);
    }
    else {
        strlstExpectedValues.append("Angle: 315.0 " + Math::c_strSymbolDegree);
    }
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
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("TopLeft {0.0, 0.0} mm");
        strlstExpectedValues.append("TopRight {100.0, 0.0} mm");
        strlstExpectedValues.append("BottomRight {100.0, 100.0} mm");
        strlstExpectedValues.append("BottomLeft {0.0, 100.0} mm");
        strlstExpectedValues.append("Center {50.0, 50.0} mm");
        strlstExpectedValues.append("Width: 100.0 mm");
        strlstExpectedValues.append("Height: 100.0 mm");
    }
    else {
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
    pTestStep->setConfigValue("UnitDest", "mm");
    strlstExpectedValues.clear();
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        strlstExpectedValues.append("TopLeft {20.0, 51.4} mm");
        strlstExpectedValues.append("TopRight {37.1, 51.4} mm");
        strlstExpectedValues.append("BottomRight {37.1, 91.4} mm");
        strlstExpectedValues.append("BottomLeft {20.0, 91.4} mm");
        strlstExpectedValues.append("Center {28.6, 71.4} mm");
        strlstExpectedValues.append("Width: 17.1 mm");
        strlstExpectedValues.append("Height: 40.0 mm");
    }
    else {
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
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
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
    pTestStep->setConfigValue("UnitDest", "px");
    strlstExpectedValues.clear();
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
    gridSettings.setLinesDistMin(20);
    gridSettings.setLabelsVisible(true);
    gridSettings.setLabelsFont(QFont("Terminal"));

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
    gridSettings.setLinesDistMin(20);
    gridSettings.setLabelsVisible(true);
    gridSettings.setLabelsFont(QFont("Terminal"));

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
    gridSettings.setLinesDistMin(20);
    gridSettings.setLabelsVisible(true);
    gridSettings.setLabelsFont(QFont("Terminal"));

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
    createTestGroupModifyStandardShapesLines(pGrpAddStandardShapes, io_idxGroup, i_drawingSize);
    createTestGroupModifyStandardShapesGroups(pGrpAddStandardShapes, io_idxGroup, i_drawingSize);

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
    strlstExpectedValues.append("line.pos {300.0, 300.0} px");
    strlstExpectedValues.append("line.p1 {0.0, -50.0} px");
    strlstExpectedValues.append("line.p2 {0.0, 50.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("position {300, 300} px");
        strlstExpectedValues.append("getLine.p1.x: 300 px");
        strlstExpectedValues.append("getLine.p1.y: 250 px");
        strlstExpectedValues.append("getLine.p2.x: 300 px");
        strlstExpectedValues.append("getLine.p2.y: 350 px");
        strlstExpectedValues.append("getLength: 100 px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("position {300.00, 300.00} mm");
            strlstExpectedValues.append("getLine.p1.x: 300.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 250.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 300.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 350.00 mm");
            strlstExpectedValues.append("getLength: 100.00 mm");
        }
        else {
            strlstExpectedValues.append("position {300.00, 300.00} mm");
            strlstExpectedValues.append("getLine.p1.x: 300.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 350.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 300.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 250.00 mm");
            strlstExpectedValues.append("getLength: 100.00 mm");
        }
    }
    strlstExpectedValues.append("rotationAngle: 90.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append("line.pos {300.0, 300.0} px");
    strlstExpectedValues.append("line.p1 {-50.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {50.0, 0.0} px");
    strlstExpectedValues.append("line.length: 100.0 px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("position {300, 300} px");
        strlstExpectedValues.append("getLine.p1.x: 250 px");
        strlstExpectedValues.append("getLine.p1.y: 300 px");
        strlstExpectedValues.append("getLine.p2.x: 350 px");
        strlstExpectedValues.append("getLine.p2.y: 300 px");
        strlstExpectedValues.append("getLength: 100 px");
    }
    else {
        strlstExpectedValues.append("position {300.00, 300.00} mm");
        strlstExpectedValues.append("getLine.p1.x: 250.00 mm");
        strlstExpectedValues.append("getLine.p1.y: 300.00 mm");
        strlstExpectedValues.append("getLine.p2.x: 350.00 mm");
        strlstExpectedValues.append("getLine.p2.y: 300.00 mm");
        strlstExpectedValues.append("getLength: 100.00 mm");
    }
    strlstExpectedValues.append("rotationAngle: 0.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append("line.pos {287.5, 300.0} px");
    strlstExpectedValues.append("line.p1 {-37.5, -50.0} px");
    strlstExpectedValues.append("line.p2 {37.5, 50.0} px");
    strlstExpectedValues.append("line.length: 125.0 px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("position {288, 300} px");
        strlstExpectedValues.append("getLine.p1.x: 250 px");
        strlstExpectedValues.append("getLine.p1.y: 250 px");
        strlstExpectedValues.append("getLine.p2.x: 325 px");
        strlstExpectedValues.append("getLine.p2.y: 350 px");
        strlstExpectedValues.append("getLength: 125 px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("position {287.50, 300.00} mm");
            strlstExpectedValues.append("getLine.p1.x: 250.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 250.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 325.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 350.00 mm");
            strlstExpectedValues.append("getLength: 125.00 mm");
        }
        else {
            strlstExpectedValues.append("position {287.50, 300.00} mm");
            strlstExpectedValues.append("getLine.p1.x: 250.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 350.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 325.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 250.00 mm");
            strlstExpectedValues.append("getLength: 125.00 mm");
        }
    }
    strlstExpectedValues.append("rotationAngle: 53.1 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append("line.pos {337.5, 337.5} px");
    strlstExpectedValues.append("line.p1 {-12.5, 12.5} px");
    strlstExpectedValues.append("line.p2 {12.5, -12.5} px");
    strlstExpectedValues.append("line.length: 35.4 px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("position {338, 338} px");
        strlstExpectedValues.append("getLine.p1.x: 325 px");
        strlstExpectedValues.append("getLine.p1.y: 350 px");
        strlstExpectedValues.append("getLine.p2.x: 350 px");
        strlstExpectedValues.append("getLine.p2.y: 325 px");
        strlstExpectedValues.append("getLength: 35 px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("position {337.50, 337.50} mm");
            strlstExpectedValues.append("getLine.p1.x: 325.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 350.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 350.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 325.00 mm");
            strlstExpectedValues.append("getLength: 35.36 mm");
        }
        else {
            strlstExpectedValues.append("position {337.50, 262.50} mm");
            strlstExpectedValues.append("getLine.p1.x: 325.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 250.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 350.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 275.00 mm");
            strlstExpectedValues.append("getLength: 35.36 mm");
        }
    }
    strlstExpectedValues.append("rotationAngle: 315.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append("line.pos {325.0, 270.0} px");
    strlstExpectedValues.append("line.p1 {-5.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {5.0, 0.0} px");
    strlstExpectedValues.append("line.length: 10.0 px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("position {325, 270} px");
        strlstExpectedValues.append("getLine.p1.x: 320 px");
        strlstExpectedValues.append("getLine.p1.y: 270 px");
        strlstExpectedValues.append("getLine.p2.x: 330 px");
        strlstExpectedValues.append("getLine.p2.y: 270 px");
        strlstExpectedValues.append("getLength: 10 px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("position {325.00, 270.00} mm");
            strlstExpectedValues.append("getLine.p1.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 270.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 270.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
        }
        else {
            strlstExpectedValues.append("position {325.00, 330.00} mm");
            strlstExpectedValues.append("getLine.p1.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 330.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 330.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
        }
    }
    strlstExpectedValues.append("rotationAngle: 0.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append("line.pos {330.0, 275.0} px");
    strlstExpectedValues.append("line.p1 {0.0, -5.0} px");
    strlstExpectedValues.append("line.p2 {0.0, 5.0} px");
    strlstExpectedValues.append("line.length: 10.0 px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("position {330, 275} px");
        strlstExpectedValues.append("getLine.p1.x: 330 px");
        strlstExpectedValues.append("getLine.p1.y: 270 px");
        strlstExpectedValues.append("getLine.p2.x: 330 px");
        strlstExpectedValues.append("getLine.p2.y: 280 px");
        strlstExpectedValues.append("getLength: 10 px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("position {330.00, 275.00} mm");
            strlstExpectedValues.append("getLine.p1.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 270.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 280.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
        }
        else {
            strlstExpectedValues.append("position {330.00, 325.00} mm");
            strlstExpectedValues.append("getLine.p1.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 330.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 320.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
        }
    }
    strlstExpectedValues.append("rotationAngle: 90.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append("line.pos {325.0, 280.0} px");
    strlstExpectedValues.append("line.p1 {5.0, 0.0} px");
    strlstExpectedValues.append("line.p2 {-5.0, 0.0} px");
    strlstExpectedValues.append("line.length: 10.0 px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("position {325, 280} px");
        strlstExpectedValues.append("getLine.p1.x: 330 px");
        strlstExpectedValues.append("getLine.p1.y: 280 px");
        strlstExpectedValues.append("getLine.p2.x: 320 px");
        strlstExpectedValues.append("getLine.p2.y: 280 px");
        strlstExpectedValues.append("getLength: 10 px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("position {325.00, 280.00} mm");
            strlstExpectedValues.append("getLine.p1.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 280.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 280.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
        }
        else {
            strlstExpectedValues.append("position {325.00, 320.00} mm");
            strlstExpectedValues.append("getLine.p1.x: 330.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 320.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 320.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
        }
    }
    strlstExpectedValues.append("rotationAngle: 180.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append("line.pos {320.0, 275.0} px");
    strlstExpectedValues.append("line.p1 {0.0, 5.0} px");
    strlstExpectedValues.append("line.p2 {0.0, -5.0} px");
    strlstExpectedValues.append("line.length: 10.0 px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("position {320, 275} px");
        strlstExpectedValues.append("getLine.p1.x: 320 px");
        strlstExpectedValues.append("getLine.p1.y: 280 px");
        strlstExpectedValues.append("getLine.p2.x: 320 px");
        strlstExpectedValues.append("getLine.p2.y: 270 px");
        strlstExpectedValues.append("getLength: 10 px");
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append("position {320.00, 275.00} mm");
            strlstExpectedValues.append("getLine.p1.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 280.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 270.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
        }
        else {
            strlstExpectedValues.append("position {320.00, 325.00} mm");
            strlstExpectedValues.append("getLine.p1.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p1.y: 320.00 mm");
            strlstExpectedValues.append("getLine.p2.x: 320.00 mm");
            strlstExpectedValues.append("getLine.p2.y: 330.00 mm");
            strlstExpectedValues.append("getLength: 10.00 mm");
        }
    }
    strlstExpectedValues.append("rotationAngle: 270.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append(strGraphObjName + ".pos {300.0, 300.0} px");
    strlstExpectedValues.append(strGraphObjName + ".topLeft {-50.0, -50.0} px");
    strlstExpectedValues.append(strGraphObjName + ".bottomRight {50.0, 50.0} px");
    strlstExpectedValues.append(strGraphObjName + ".size {100.0, 100.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append(strGraphObjName + ".position {300, 300} px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 250 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 250 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 350 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 350 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.size.width: 100 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.size.height: 100 px");
        strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line2.pos {-12.5, 0.0} px");
        strlstExpectedValues.append("Line2.position {38, 50} px");
        strlstExpectedValues.append("Line2.getLine.p1.x: 0 px");
        strlstExpectedValues.append("Line2.getLine.p1.y: 0 px");
        strlstExpectedValues.append("Line2.getLine.p2.x: 75 px");
        strlstExpectedValues.append("Line2.getLine.p2.y: 100 px");
        strlstExpectedValues.append("Line2.getLength: 125 px");
        strlstExpectedValues.append("Line2.rotationAngle: 53.1 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Line3.pos {37.5, 37.5} px");
        strlstExpectedValues.append("Line3.position {88, 88} px");
        strlstExpectedValues.append("Line3.getLine.p1.x: 75 px");
        strlstExpectedValues.append("Line3.getLine.p1.y: 100 px");
        strlstExpectedValues.append("Line3.getLine.p2.x: 100 px");
        strlstExpectedValues.append("Line3.getLine.p2.y: 75 px");
        strlstExpectedValues.append("Line3.getLength: 35 px");
        strlstExpectedValues.append("Line3.rotationAngle: 315.0 " + Math::c_strSymbolDegree);
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append(strGraphObjName + ".position {300.00, 300.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 250.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 250.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 350.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 350.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.width: 100.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.height: 100.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line2.pos {-12.5, 0.0} px");
            strlstExpectedValues.append("Line2.position {37.50, 50.00} mm");
            strlstExpectedValues.append("Line2.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line2.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line2.getLine.p2.x: 75.00 mm");
            strlstExpectedValues.append("Line2.getLine.p2.y: 100.00 mm");
            strlstExpectedValues.append("Line2.getLength: 125.00 mm");
            strlstExpectedValues.append("Line2.rotationAngle: 53.1 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line3.pos {37.5, 37.5} px");
            strlstExpectedValues.append("Line3.position {87.50, 87.50} mm");
            strlstExpectedValues.append("Line3.getLine.p1.x: 75.00 mm");
            strlstExpectedValues.append("Line3.getLine.p1.y: 100.00 mm");
            strlstExpectedValues.append("Line3.getLine.p2.x: 100.00 mm");
            strlstExpectedValues.append("Line3.getLine.p2.y: 75.00 mm");
            strlstExpectedValues.append("Line3.getLength: 35.36 mm");
            strlstExpectedValues.append("Line3.rotationAngle: 315.0 " + Math::c_strSymbolDegree);
        }
        else {
            strlstExpectedValues.append(strGraphObjName + ".position {300.00, 300.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 250.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 350.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 350.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 250.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.width: 100.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.height: -100.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line2.pos {-12.5, 0.0} px");
            strlstExpectedValues.append("Line2.position {37.50, 50.00} mm");
            strlstExpectedValues.append("Line2.getLine.p1.x: 0.00 mm");
            strlstExpectedValues.append("Line2.getLine.p1.y: 100.00 mm");
            strlstExpectedValues.append("Line2.getLine.p2.x: 75.00 mm");
            strlstExpectedValues.append("Line2.getLine.p2.y: 0.00 mm");
            strlstExpectedValues.append("Line2.getLength: 125.00 mm");
            strlstExpectedValues.append("Line2.rotationAngle: 53.1 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Line3.pos {37.5, 37.5} px");
            strlstExpectedValues.append("Line3.position {87.50, 12.50} mm");
            strlstExpectedValues.append("Line3.getLine.p1.x: 75.00 mm");
            strlstExpectedValues.append("Line3.getLine.p1.y: 0.00 mm");
            strlstExpectedValues.append("Line3.getLine.p2.x: 100.00 mm");
            strlstExpectedValues.append("Line3.getLine.p2.y: 25.00 mm");
            strlstExpectedValues.append("Line3.getLength: 35.36 mm");
            strlstExpectedValues.append("Line3.rotationAngle: 315.0 " + Math::c_strSymbolDegree);
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
    strlstExpectedValues.append(strGraphObjName + ".pos {300.0, 300.0} px");
    strlstExpectedValues.append(strGraphObjName + ".topLeft {-50.0, -50.0} px");
    strlstExpectedValues.append(strGraphObjName + ".bottomRight {50.0, 50.0} px");
    strlstExpectedValues.append(strGraphObjName + ".size {100.0, 100.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append(strGraphObjName + ".position {300, 300} px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 250 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 250 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 350 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 350 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.size.width: 100 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.size.height: 100 px");
        strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
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
        strlstExpectedValues.append("Group0.pos {25.0, -25.0} px");
        strlstExpectedValues.append("Group0.position {75, 25} px");
        strlstExpectedValues.append("Group0.getRect.topLeft.x: 70 px");
        strlstExpectedValues.append("Group0.getRect.topLeft.y: 20 px");
        strlstExpectedValues.append("Group0.getRect.bottomRight.x: 80 px");
        strlstExpectedValues.append("Group0.getRect.bottomRight.y: 30 px");
        strlstExpectedValues.append("Group0.getRect.size.width: 10 px");
        strlstExpectedValues.append("Group0.getRect.size.height: 10 px");
        strlstExpectedValues.append("Group0.rotationAngle: 0.0 " + Math::c_strSymbolDegree);
        strlstExpectedValues.append("Group1.pos {0.0, 0.0} px");
        strlstExpectedValues.append("Group1.position {50, 50} px");
        strlstExpectedValues.append("Group1.getRect.topLeft.x: 0 px");
        strlstExpectedValues.append("Group1.getRect.topLeft.y: 0 px");
        strlstExpectedValues.append("Group1.getRect.bottomRight.x: 100 px");
        strlstExpectedValues.append("Group1.getRect.bottomRight.y: 100 px");
        strlstExpectedValues.append("Group1.getRect.size.width: 100 px");
        strlstExpectedValues.append("Group1.getRect.size.height: 100 px");
        strlstExpectedValues.append("Group1.rotationAngle: 0.0 " + Math::c_strSymbolDegree);
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append(strGraphObjName + ".position {300.00, 300.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 250.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 250.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 350.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 350.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.width: 100.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.height: 100.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
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
            strlstExpectedValues.append("Group0.pos {25.0, -25.0} px");
            strlstExpectedValues.append("Group0.position {75.00, 25.00} mm");
            strlstExpectedValues.append("Group0.getRect.topLeft.x: 70.00 mm");
            strlstExpectedValues.append("Group0.getRect.topLeft.y: 20.00 mm");
            strlstExpectedValues.append("Group0.getRect.bottomRight.x: 80.00 mm");
            strlstExpectedValues.append("Group0.getRect.bottomRight.y: 30.00 mm");
            strlstExpectedValues.append("Group0.getRect.size.width: 10.00 mm");
            strlstExpectedValues.append("Group0.getRect.size.height: 10.00 mm");
            strlstExpectedValues.append("Group0.rotationAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Group1.pos {0.0, 0.0} px");
            strlstExpectedValues.append("Group1.position {50.00, 50.00} mm");
            strlstExpectedValues.append("Group1.getRect.topLeft.x: 0.00 mm");
            strlstExpectedValues.append("Group1.getRect.topLeft.y: 0.00 mm");
            strlstExpectedValues.append("Group1.getRect.bottomRight.x: 100.00 mm");
            strlstExpectedValues.append("Group1.getRect.bottomRight.y: 100.00 mm");
            strlstExpectedValues.append("Group1.getRect.size.width: 100.00 mm");
            strlstExpectedValues.append("Group1.getRect.size.height: 100.00 mm");
            strlstExpectedValues.append("Group1.rotationAngle: 0.0 " + Math::c_strSymbolDegree);
        }
        else {
            strlstExpectedValues.append(strGraphObjName + ".position {300.00, 300.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 250.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 350.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 350.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 250.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.width: 100.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.size.height: -100.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 0.0 " + Math::c_strSymbolDegree);
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
            strlstExpectedValues.append("Group0.pos {25.0, -25.0} px");
            strlstExpectedValues.append("Group0.position {75.00, 75.00} mm");
            strlstExpectedValues.append("Group0.getRect.topLeft.x: 70.00 mm");
            strlstExpectedValues.append("Group0.getRect.topLeft.y: 80.00 mm");
            strlstExpectedValues.append("Group0.getRect.bottomRight.x: 80.00 mm");
            strlstExpectedValues.append("Group0.getRect.bottomRight.y: 70.00 mm");
            strlstExpectedValues.append("Group0.getRect.size.width: 10.00 mm");
            strlstExpectedValues.append("Group0.getRect.size.height: -10.00 mm");
            strlstExpectedValues.append("Group0.rotationAngle: 0.0 " + Math::c_strSymbolDegree);
            strlstExpectedValues.append("Group1.pos {0.0, 0.0} px");
            strlstExpectedValues.append("Group1.position {50.00, 50.00} mm");
            strlstExpectedValues.append("Group1.getRect.topLeft.x: 0.00 mm");
            strlstExpectedValues.append("Group1.getRect.topLeft.y: 100.00 mm");
            strlstExpectedValues.append("Group1.getRect.bottomRight.x: 100.00 mm");
            strlstExpectedValues.append("Group1.getRect.bottomRight.y: 0.00 mm");
            strlstExpectedValues.append("Group1.getRect.size.width: 100.00 mm");
            strlstExpectedValues.append("Group1.getRect.size.height: -100.00 mm");
            strlstExpectedValues.append("Group1.rotationAngle: 0.0 " + Math::c_strSymbolDegree);
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

    ZS::Test::CTestStepGroup* pGrpModifyLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Modify Lines",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);

    int idxStep = 0;

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    // Line2 (Checkmark line from left to bottom) in Group1
    //-----------------------------------------------------

    QString strGroupNameTop = "Group2";
    QString strGroupNameLine = "Group1";
    QString strGraphObjName = "Line2";
    QString strGraphObjPath = strGroupNameTop + pIdxTree->nodeSeparator() + strGroupNameLine + pIdxTree->nodeSeparator() + strGraphObjName;
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameTop, strGroupNameLine, strGraphObjName);

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjPath + ".setLine()",
        /* strOperation    */ strGraphObjPath + ".setLine()",
        /* pGrpParent      */ pGrpModifyLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("Method", "setLine");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        pTestStep->setConfigValue("P1", QPointF(25.0, 25.0));
        pTestStep->setConfigValue("P2", QPointF(60.0, 75.0));
    }
    else {
        pTestStep->setConfigValue("P1", QPointF(25.0, 75.0));
        pTestStep->setConfigValue("P2", QPointF(60.0, 25.0));
    }
    QStringList strlstExpectedValues;
    strlstExpectedValues.append(strGraphObjName + ".pos {-7.5, 0.0} px");
    strlstExpectedValues.append(strGraphObjName + ".line.p1 {-17.5, -25.0} px");
    strlstExpectedValues.append(strGraphObjName + ".line.p2 {17.5, 25.0} px");
    strlstExpectedValues.append(strGraphObjName + ".line.length: 61.0 px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append(strGraphObjName + ".position {43, 50} px");
        strlstExpectedValues.append(strGraphObjName + ".getLine.p1.x: 25 px");
        strlstExpectedValues.append(strGraphObjName + ".getLine.p1.y: 25 px");
        strlstExpectedValues.append(strGraphObjName + ".getLine.p2.x: 60 px");
        strlstExpectedValues.append(strGraphObjName + ".getLine.p2.y: 75 px");
        strlstExpectedValues.append(strGraphObjName + ".getLength: 61 px");
        strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 55.0 " + Math::c_strSymbolDegree);
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append(strGraphObjName + ".position {42.50, 50.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p1.x: 25.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p1.y: 25.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p2.x: 60.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p2.y: 75.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLength: 61.03 mm");
            strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 55.0 " + Math::c_strSymbolDegree);
        }
        else {
            strlstExpectedValues.append(strGraphObjName + ".position {42.50, 50.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p1.x: 25.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p1.y: 75.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p2.x: 60.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p2.y: 25.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLength: 61.03 mm");
            strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 55.0 " + Math::c_strSymbolDegree);
        }
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    // Line3 (Checkmark line from bottom to right) in Group1
    //------------------------------------------------------

    strGroupNameTop = "Group2";
    strGroupNameLine = "Group1";
    strGraphObjName = "Line3";
    strGraphObjPath = strGroupNameTop + pIdxTree->nodeSeparator() + strGroupNameLine + pIdxTree->nodeSeparator() + strGraphObjName;
    strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    strKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameTop, strGroupNameLine, strGraphObjName);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjPath + ".setLine()",
        /* strOperation    */ strGraphObjPath + ".setLine()",
        /* pGrpParent      */ pGrpModifyLines,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjLine(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strKeyInTree);
    pTestStep->setConfigValue("Method", "setLine");
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        pTestStep->setConfigValue("P1", QPointF(60.0, 75.0));
        pTestStep->setConfigValue("P2", QPointF(75.0, 60.0));
    }
    else {
        pTestStep->setConfigValue("P1", QPointF(60.0, 25.0));
        pTestStep->setConfigValue("P2", QPointF(75.0, 40.0));
    }
    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {17.5, 17.5} px");
    strlstExpectedValues.append(strGraphObjName + ".line.p1 {-7.5, 7.5} px");
    strlstExpectedValues.append(strGraphObjName + ".line.p2 {7.5, -7.5} px");
    strlstExpectedValues.append(strGraphObjName + ".line.length: 21.2 px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append(strGraphObjName + ".position {68, 68} px");
        strlstExpectedValues.append(strGraphObjName + ".getLine.p1.x: 60 px");
        strlstExpectedValues.append(strGraphObjName + ".getLine.p1.y: 75 px");
        strlstExpectedValues.append(strGraphObjName + ".getLine.p2.x: 75 px");
        strlstExpectedValues.append(strGraphObjName + ".getLine.p2.y: 60 px");
        strlstExpectedValues.append(strGraphObjName + ".getLength: 21 px");
        strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 315.0 " + Math::c_strSymbolDegree);
    }
    else {
        if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            strlstExpectedValues.append(strGraphObjName + ".position {67.50, 67.50} mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p1.x: 60.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p1.y: 75.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p2.x: 75.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p2.y: 60.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLength: 21.21 mm");
            strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 315.0 " + Math::c_strSymbolDegree);
        }
        else {
            strlstExpectedValues.append(strGraphObjName + ".position {67.50, 32.50} mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p1.x: 60.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p1.y: 25.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p2.x: 75.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLine.p2.y: 40.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getLength: 21.21 mm");
            strlstExpectedValues.append(strGraphObjName + ".rotationAngle: 315.0 " + Math::c_strSymbolDegree);
        }
    }
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

    ZS::Test::CTestStepGroup* pGrpModifyGroups = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Modify Groups",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    CIdxTree* pIdxTree = m_pDrawingScene->getGraphObjsIdxTree();

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);

    int idxStep = 0;

    /*-----------------------------------------------------------------------
    Pixels Drawing:
        Size: 800 * 600 Pixels
    Metrics Drawing:
        Size: 800 * 600 mm
        ScreenPixelResolution: 1.0 px/mm
        Decimals: 2
    -----------------------------------------------------------------------*/

    // Group1 (Checkmark)
    //-------------------

    // Group1.resizeToContent
    QString strEntryType = CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strGroupNameParent = "Group2";
    QString strGroupNameParentKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameParent);
    QString strGraphObjName = "Group1";
    QString strGraphObjPath = strGroupNameParent + pIdxTree->nodeSeparator() + strGraphObjName;
    QString strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameParent, strGraphObjName);

    ZS::Test::CTestStep* pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjPath + ".resizeToContent()",
        /* strOperation    */ strGraphObjPath + ".resizeToContent()",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "resizeToContent");
    QStringList strlstExpectedValues;
    strlstExpectedValues.append(strGraphObjName + ".pos {0.0, 0.0} px");
    strlstExpectedValues.append(strGraphObjName + ".topLeft {-25.0, -25.0} px");
    strlstExpectedValues.append(strGraphObjName + ".bottomRight {25.0, 25.0} px");
    strlstExpectedValues.append(strGraphObjName + ".size {50.0, 50.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append(strGraphObjName + ".position {50, 50} px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 25 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 25 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 75 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 75 px");
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
            strlstExpectedValues.append(strGraphObjName + ".position {50.00, 50.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 25.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 25.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 75.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 75.00 mm");
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
            strlstExpectedValues.append(strGraphObjName + ".position {50.00, 50.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 25.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 75.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 75.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 25.00 mm");
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

    // Group2 (horizonal and vertical lines together with checkmark and rectangle group)
    //----------------------------------------------------------------------------------

    // Group2.removeFromGroup(Group0)
    strGroupNameParent = "Group2";
    strGroupNameParentKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameParent);
    strGraphObjName = "Group0";
    strGraphObjPath = strGroupNameParent + pIdxTree->nodeSeparator() + strGraphObjName;
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

    // Group2 (horizonal and vertical lines together with checkmark and rectangle group)
    //----------------------------------------------------------------------------------

    // Group2.removeFromGroup(Group1)
    strGroupNameParent = "Group2";
    strGroupNameParentKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGroupNameParent);
    strGraphObjName = "Group1";
    strGraphObjPath = strGroupNameParent + pIdxTree->nodeSeparator() + strGraphObjName;
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

    // Group0 (short lines which form a rectangle)
    //--------------------------------------------

    // Group0.setPosition()
    strGraphObjName = "Group0";
    strGraphObjPath = strGraphObjName;
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    QPointF ptPos;
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        ptPos = QPointF(600.0, 100.0);
    }
    else {
        ptPos = QPointF(600.0, 500.0);
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjPath + ".setPosition(" + qPoint2Str(ptPos) + " " + i_drawingSize.unit().symbol() + ")",
        /* strOperation    */ strGraphObjPath + ".setPosition(" + qPoint2Str(ptPos) + " " + i_drawingSize.unit().symbol() + ")",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "setPosition");
    pTestStep->setConfigValue("Pos", ptPos);

    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {600.0, 100.0} px");
    strlstExpectedValues.append(strGraphObjName + ".topLeft {-5.0, -5.0} px");
    strlstExpectedValues.append(strGraphObjName + ".bottomRight {5.0, 5.0} px");
    strlstExpectedValues.append(strGraphObjName + ".size {10.0, 10.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append(strGraphObjName + ".position {600, 100} px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 595 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 95 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 605 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 105 px");
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
            strlstExpectedValues.append(strGraphObjName + ".position {600.00, 100.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 595.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 95.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 605.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 105.00 mm");
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
            strlstExpectedValues.append(strGraphObjName + ".position {600.00, 500.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 595.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 505.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 605.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 495.00 mm");
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

    // Group1 (checkmark)
    //-------------------

    // Group1.setPosition()
    strGraphObjName = "Group1";
    strGraphObjPath = strGraphObjName;
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        ptPos = QPointF(600.0, 200.0);
    }
    else {
        ptPos = QPointF(600.0, 400.0);
    }

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjPath + ".setPosition(" + qPoint2Str(ptPos) + " " + i_drawingSize.unit().symbol() + ")",
        /* strOperation    */ strGraphObjPath + ".setPosition(" + qPoint2Str(ptPos) + " " + i_drawingSize.unit().symbol() + ")",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "setPosition");
    pTestStep->setConfigValue("Pos", ptPos);

    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {600.0, 200.0} px");
    strlstExpectedValues.append(strGraphObjName + ".topLeft {-25.0, -25.0} px");
    strlstExpectedValues.append(strGraphObjName + ".bottomRight {25.0, 25.0} px");
    strlstExpectedValues.append(strGraphObjName + ".size {50.0, 50.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append(strGraphObjName + ".position {600, 200} px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 575 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 175 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 625 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 225 px");
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
            strlstExpectedValues.append(strGraphObjName + ".position {600.00, 200.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 575.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 175.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 625.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 225.00 mm");
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
            strlstExpectedValues.append(strGraphObjName + ".position {600.00, 400.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 575.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 425.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 625.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.y: 375.00 mm");
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

    // Group2 (horizonal and vertical lines)
    //--------------------------------------

    // Group2.setRotationAngle()
    strGraphObjName = "Group2";
    strGraphObjPath = strGraphObjName;
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    CPhysVal physValAngle(45.0, Units.Angle.Degree, 0.1);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjPath + ".setRotationAngle(" + physValAngle.toString() + ")",
        /* strOperation    */ strGraphObjPath + ".setRotationAngle(" + physValAngle.toString() + ")",
        /* pGrpParent      */ pGrpModifyGroups,
        /* szDoTestStepFct */ SLOT(doTestStepModifyGraphObjGroup(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("GraphObjName", strGraphObjName);
    pTestStep->setConfigValue("GraphObjKeyInTree", strGraphObjKeyInTree);
    pTestStep->setConfigValue("Method", "setRotationAngle");
    pTestStep->setConfigValue("Angle", physValAngle.toString());

    strlstExpectedValues.clear();
    strlstExpectedValues.append(strGraphObjName + ".pos {300.0, 300.0} px");
    strlstExpectedValues.append(strGraphObjName + ".topLeft {-50.0, -50.0} px");
    strlstExpectedValues.append(strGraphObjName + ".bottomRight {50.0, 50.0} px");
    strlstExpectedValues.append(strGraphObjName + ".size {100.0, 100.0} px");
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append(strGraphObjName + ".position {300, 300} px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 250 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 250 px");
        strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 350 px");
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
            strlstExpectedValues.append(strGraphObjName + ".position {300.00, 300.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 250.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 250.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 350.00 mm");
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
            strlstExpectedValues.append(strGraphObjName + ".position {300.00, 300.00} mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.x: 250.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.topLeft.y: 350.00 mm");
            strlstExpectedValues.append(strGraphObjName + ".getRect.bottomRight.x: 350.00 mm");
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

    // Group2 (horizonal and vertical lines)
    //--------------------------------------

    // Group2.setPosition()
    strGraphObjName = "Group2";
    strGraphObjPath = strGraphObjName;
    strGraphObjKeyInTree = pIdxTree->buildKeyInTreeStr(strEntryType, strGraphObjName);

    ptPos = QPointF(400.0, 300.0);

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strGraphObjPath + ".setPosition(" + qPoint2Str(ptPos) + " " + i_drawingSize.unit().symbol() + ")",
        /* strOperation    */ strGraphObjPath + ".setPosition(" + qPoint2Str(ptPos) + " " + i_drawingSize.unit().symbol() + ")",
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

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpModifyGroups->path());
    }
    return pGrpModifyGroups;
}

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
                    drawingSize.setImageSize(CPhysValSize(*m_pDrawingScene, fWidth, fHeight, unit));
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
    gridSettings.setLabelsFont(
        i_pTestStep->getConfigValue(XmlStreamParser::c_strXmlAttrFont).toString());

    m_pDrawingScene->setGridSettings(gridSettings);

    i_pTestStep->setResultValue("");
}

//------------------------------------------------------------------------------
void CTest::doTestStepTransformPhysValRect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepTransformPhysValRect",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CUnit unit = drawingSize.unit();

    QRectF rectF = i_pTestStep->getConfigValue("create").toRectF();
    if (i_pTestStep->hasConfigValue("create.unit")) {
        QString strUnitPoint = i_pTestStep->getConfigValue("create.unit").toString();
        unit = strUnitPoint;
    }

    CPhysValRect physValRectResult(*m_pDrawingScene, rectF, unit);

    if (i_pTestStep->hasConfigValue("setAngle")) {
        CPhysVal physValAngle = i_pTestStep->getConfigValue("setAngle").toString();
        physValAngle.setRes(0.1);
        physValRectResult.setAngle(physValAngle);
    }
    if (i_pTestStep->hasConfigValue("setCenter")) {
        QPointF pt = i_pTestStep->getConfigValue("setCenter").toPointF();
        if (i_pTestStep->hasConfigValue("setCenter.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setCenter.unit").toString();
            unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setCenter(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setCenter(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setSize")) {
        QSizeF sizeF = i_pTestStep->getConfigValue("setSize").toSizeF();
        if (i_pTestStep->hasConfigValue("setSize.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setSize.unit").toString();
            unit = strUnitPoint;
            CPhysValSize physValSize(*m_pDrawingScene, sizeF, unit);
            physValRectResult.setSize(physValSize);
        }
        else {
            CPhysValSize physValSize(*m_pDrawingScene, sizeF);
            physValRectResult.setSize(physValSize);
        }
    }
    if (i_pTestStep->hasConfigValue("setWidth")) {
        CPhysVal physValWidth = i_pTestStep->getConfigValue("setWidth").toString();
        physValRectResult.setWidth(physValWidth);
    }
    if (i_pTestStep->hasConfigValue("setWidthByMovingLeftCenter")) {
        QPointF pt = i_pTestStep->getConfigValue("setWidthByMovingLeftCenter").toPointF();
        if (i_pTestStep->hasConfigValue("setWidthByMovingLeftCenter.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setWidthByMovingLeftCenter.unit").toString();
            unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setWidthByMovingLeftCenter(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setWidthByMovingLeftCenter(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setWidthByMovingRightCenter")) {
        QPointF pt = i_pTestStep->getConfigValue("setWidthByMovingRightCenter").toPointF();
        if (i_pTestStep->hasConfigValue("setWidthByMovingRightCenter.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setWidthByMovingRightCenter.unit").toString();
            unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setWidthByMovingRightCenter(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setWidthByMovingRightCenter(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setHeight")) {
        CPhysVal physValHeight = i_pTestStep->getConfigValue("setHeight").toString();
        physValRectResult.setHeight(physValHeight);
    }
    if (i_pTestStep->hasConfigValue("setHeightByMovingTopCenter")) {
        QPointF pt = i_pTestStep->getConfigValue("setHeightByMovingTopCenter").toPointF();
        if (i_pTestStep->hasConfigValue("setTopCenter.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setHeightByMovingTopCenter.unit").toString();
            unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setHeightByMovingTopCenter(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setHeightByMovingTopCenter(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setHeightByMovingBottomCenter")) {
        QPointF pt = i_pTestStep->getConfigValue("setHeightByMovingBottomCenter").toPointF();
        if (i_pTestStep->hasConfigValue("setHeightByMovingBottomCenter.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setHeightByMovingBottomCenter.unit").toString();
            unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setHeightByMovingBottomCenter(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setHeightByMovingBottomCenter(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setTopLeft")) {
        QPointF pt = i_pTestStep->getConfigValue("setTopLeft").toPointF();
        if (i_pTestStep->hasConfigValue("setTopLeft.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setTopLeft.unit").toString();
            unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setTopLeft(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setTopLeft(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setTopRight")) {
        QPointF pt = i_pTestStep->getConfigValue("setTopRight").toPointF();
        if (i_pTestStep->hasConfigValue("setTopRight.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setTopRight.unit").toString();
            unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setTopRight(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setTopRight(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setBottomRight")) {
        QPointF pt = i_pTestStep->getConfigValue("setBottomRight").toPointF();
        if (i_pTestStep->hasConfigValue("setBottomRight.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setBottomRight.unit").toString();
            unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setBottomRight(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setBottomRight(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("setBottomLeft")) {
        QPointF pt = i_pTestStep->getConfigValue("setBottomLeft").toPointF();
        if (i_pTestStep->hasConfigValue("setBottomLeft.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setBottomLeft.unit").toString();
            unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setBottomLeft(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setBottomLeft(physValPt);
        }
    }

    QStringList strlstResultValues;
    strlstResultValues.append("Center {" + physValRectResult.center().toString() + "} " + physValRectResult.center().unit().symbol());
    strlstResultValues.append("Width: " + physValRectResult.width().toString());
    strlstResultValues.append("Height: " + physValRectResult.height().toString());
    strlstResultValues.append("Angle: " + physValRectResult.angle().toString());
    strlstResultValues.append("TopLeft {" + physValRectResult.topLeft().toString() + "} " + physValRectResult.topLeft().unit().symbol());
    strlstResultValues.append("TopRight {" + physValRectResult.topRight().toString() + "} " + physValRectResult.topRight().unit().symbol());
    strlstResultValues.append("BottomRight {" + physValRectResult.bottomRight().toString() + "} " + physValRectResult.bottomRight().unit().symbol());
    strlstResultValues.append("BottomLeft {" + physValRectResult.bottomLeft().toString() + "} " + physValRectResult.bottomLeft().unit().symbol());
    strlstResultValues.append("TopCenter {" + physValRectResult.topCenter().toString() + "} " + physValRectResult.topCenter().unit().symbol());
    strlstResultValues.append("RightCenter {" + physValRectResult.rightCenter().toString() + "} " + physValRectResult.rightCenter().unit().symbol());
    strlstResultValues.append("BottomCenter {" + physValRectResult.bottomCenter().toString() + "} " + physValRectResult.bottomCenter().unit().symbol());
    strlstResultValues.append("LeftCenter {" + physValRectResult.leftCenter().toString() + "} " + physValRectResult.leftCenter().unit().symbol());
    i_pTestStep->setResultValues(strlstResultValues);
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
    QPointF pt = i_pTestStep->getConfigValue("Point").toPointF();
    CPhysValPoint physValPointResult(*m_pDrawingScene);
    if (Units.Length.isMetricUnit(unitSource)) {
        CPhysValPoint physValPoint(*m_pDrawingScene, pt, unitSource);
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
    QSizeF sizeF = i_pTestStep->getConfigValue("Size").toSizeF();
    CPhysValSize physValSizeResult(*m_pDrawingScene);
    if (Units.Length.isMetricUnit(unitSource)) {
        CPhysValSize physValSize(*m_pDrawingScene, sizeF, unitSource);
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
    QLineF lineF = i_pTestStep->getConfigValue("Line").toLineF();
    CPhysValLine physValLineResult(*m_pDrawingScene);
    if (Units.Length.isMetricUnit(unitSource)) {
        CPhysValLine physValLine(*m_pDrawingScene, lineF, unitSource);
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
    QRectF rectF = i_pTestStep->getConfigValue("Rect").toRectF();
    CPhysValRect physValRectResult(*m_pDrawingScene);
    if (Units.Length.isMetricUnit(unitSource)) {
        CPhysValRect physValRect(*m_pDrawingScene, rectF, unitSource);
        physValRectResult = m_pDrawingScene->convert(physValRect, unitDest);
    }
    else {
        physValRectResult = m_pDrawingScene->convert(rectF, unitDest);
    }
    QStringList strlstResultValues;
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
        strlstResultValues.append("line.pos {" + qPoint2Str(pGraphObjLine->pos()) + "} px");
        strlstResultValues.append("line.p1 {" + qPoint2Str(lineF.p1()) + "} px");
        strlstResultValues.append("line.p2 {" + qPoint2Str(lineF.p2()) + "} px");
        strlstResultValues.append("line.length: " + QString::number(lineF.length(), 'f', 1) + " px");
        CPhysValPoint physValPointPos = pGraphObjLine->position();
        strlstResultValues.append("position {" + physValPointPos.toString() + "} " + physValPointPos.unit().symbol());
        CPhysValLine physValLine = pGraphObjLine->getLine();
        strlstResultValues.append("getLine.p1.x: " + physValLine.p1().x().toString());
        strlstResultValues.append("getLine.p1.y: " + physValLine.p1().y().toString());
        strlstResultValues.append("getLine.p2.x: " + physValLine.p2().x().toString());
        strlstResultValues.append("getLine.p2.y: " + physValLine.p2().y().toString());
        CPhysVal physValLength = pGraphObjLine->getLength();
        strlstResultValues.append("getLength: " + physValLength.toString());
        CPhysVal physValAngle = pGraphObjLine->rotationAngle(Units.Angle.Degree);
        strlstResultValues.append("rotationAngle: " + physValAngle.toString());
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
        strlstResultValues.append(pGraphObjGroup->name() + ".pos {" + qPoint2Str(pGraphObjGroup->pos()) + "} px");
        strlstResultValues.append(pGraphObjGroup->name() + ".topLeft {" + qPoint2Str(rectF.topLeft()) + "} px");
        strlstResultValues.append(pGraphObjGroup->name() + ".bottomRight {" + qPoint2Str(rectF.bottomRight()) + "} px");
        strlstResultValues.append(pGraphObjGroup->name() + ".size {" + qSize2Str(rectF.size()) + "} px");
        CPhysValPoint physValPointPos = pGraphObjGroup->position();
        strlstResultValues.append(pGraphObjGroup->name() + ".position {" + physValPointPos.toString() + "} " + physValPointPos.unit().symbol());
        CPhysValRect physValRect = pGraphObjGroup->getRect();
        strlstResultValues.append(pGraphObjGroup->name() + ".getRect.topLeft.x: " + physValRect.topLeft().x().toString());
        strlstResultValues.append(pGraphObjGroup->name() + ".getRect.topLeft.y: " + physValRect.topLeft().y().toString());
        strlstResultValues.append(pGraphObjGroup->name() + ".getRect.bottomRight.x: " + physValRect.bottomRight().x().toString());
        strlstResultValues.append(pGraphObjGroup->name() + ".getRect.bottomRight.y: " + physValRect.bottomRight().y().toString());
        CPhysValSize physValSize = pGraphObjGroup->getSize();
        strlstResultValues.append(pGraphObjGroup->name() + ".getRect.size.width: " + physValSize.width().toString());
        strlstResultValues.append(pGraphObjGroup->name() + ".getRect.size.height: " + physValSize.height().toString());
        CPhysVal physValAngle = pGraphObjGroup->rotationAngle(Units.Angle.Degree);
        strlstResultValues.append(pGraphObjGroup->name() + ".rotationAngle: " + physValAngle.toString());
        for (CGraphObj* pGraphObjChild : pGraphObjGroup->childs()) {
            QGraphicsItem* pGraphicsItemChild = dynamic_cast<QGraphicsItem*>(pGraphObjChild);
            strlstResultValues.append(pGraphObjChild->name() + ".pos {" + qPoint2Str(pGraphicsItemChild->pos()) + "} px");
            if (pGraphObjChild->isLine()) {
                CGraphObjLine* pGraphObjChildLine = dynamic_cast<CGraphObjLine*>(pGraphObjChild);
                physValPointPos = pGraphObjChildLine->position();
                strlstResultValues.append(pGraphObjChild->name() + ".position {" + physValPointPos.toString() + "} " + physValPointPos.unit().symbol());
                CPhysValLine physValLine = pGraphObjChildLine->getLine();
                strlstResultValues.append(pGraphObjChild->name() + ".getLine.p1.x: " + physValLine.p1().x().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getLine.p1.y: " + physValLine.p1().y().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getLine.p2.x: " + physValLine.p2().x().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getLine.p2.y: " + physValLine.p2().y().toString());
                CPhysVal physValLength = pGraphObjChildLine->getLength();
                strlstResultValues.append(pGraphObjChild->name() + ".getLength: " + physValLength.toString());
                physValAngle = pGraphObjChild->rotationAngle(Units.Angle.Degree);
                strlstResultValues.append(pGraphObjChild->name() + ".rotationAngle: " + physValAngle.toString());
            }
            else if (pGraphObjChild->isGroup()) {
                CGraphObjGroup* pGraphObjChildGroup = dynamic_cast<CGraphObjGroup*>(pGraphObjChild);
                physValPointPos = pGraphObjChildGroup->position();
                strlstResultValues.append(pGraphObjChild->name() + ".position {" + physValPointPos.toString() + "} " + physValPointPos.unit().symbol());
                CPhysValRect physValRect = pGraphObjChildGroup->getRect();
                strlstResultValues.append(pGraphObjChild->name() + ".getRect.topLeft.x: " + physValRect.topLeft().x().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getRect.topLeft.y: " + physValRect.topLeft().y().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getRect.bottomRight.x: " + physValRect.bottomRight().x().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getRect.bottomRight.y: " + physValRect.bottomRight().y().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getRect.size.width: " + physValRect.size().width().toString());
                strlstResultValues.append(pGraphObjChild->name() + ".getRect.size.height: " + physValRect.size().height().toString());
                physValAngle = pGraphObjChild->rotationAngle(Units.Angle.Degree);
                strlstResultValues.append(pGraphObjChild->name() + ".rotationAngle: " + physValAngle.toString());
            }
        }
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepModifyGraphObjLine( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepModifyGraphObjLine",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);

    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();
    QString strMethod = i_pTestStep->getConfigValue("Method").toString();

    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
    CGraphObjLine* pGraphObjLine = dynamic_cast<CGraphObjLine*>(pGraphObj);
    if (pGraphObjLine != nullptr) {
        if (strMethod.compare("setLine", Qt::CaseInsensitive) == 0) {
            if (i_pTestStep->hasConfigValue("P1")) {
                CPhysValPoint physValPoint1(*m_pDrawingScene);
                physValPoint1 = i_pTestStep->getConfigValue("P1").toPointF();
                pGraphObjLine->setP1(physValPoint1);
            }
            if (i_pTestStep->hasConfigValue("P2")) {
                CPhysValPoint physValPoint2(*m_pDrawingScene);
                physValPoint2 = i_pTestStep->getConfigValue("P2").toPointF();
                pGraphObjLine->setP2(physValPoint2);
            }
        }
        else if (strMethod.compare("setPosition", Qt::CaseInsensitive) == 0) {
            CPhysValPoint physValPos(*m_pDrawingScene);
            physValPos = i_pTestStep->getConfigValue("Pos").toPointF();
            pGraphObjLine->setPosition(physValPos);
        }
        else if (strMethod.compare("setRotationAngle", Qt::CaseInsensitive) == 0) {
            CPhysVal physValAngle(0.0, Units.Angle.Degree, 0.1);
            QString strAngle = i_pTestStep->getConfigValue("Angle").toString();
            physValAngle = strAngle;
            pGraphObjLine->setRotationAngle(physValAngle);
        }
    }

    QStringList strlstResultValues;
    if (pGraphObjLine != nullptr) {
        QLineF lineF = pGraphObjLine->line();
        strlstResultValues.append(strGraphObjName + ".pos {" + qPoint2Str(pGraphObjLine->pos()) + "} px");
        strlstResultValues.append(strGraphObjName + ".line.p1 {" + qPoint2Str(lineF.p1()) + "} px");
        strlstResultValues.append(strGraphObjName + ".line.p2 {" + qPoint2Str(lineF.p2()) + "} px");
        strlstResultValues.append(strGraphObjName + ".line.length: " + QString::number(lineF.length(), 'f', 1) + " px");
        CPhysValPoint physValPointPos = pGraphObjLine->position();
        strlstResultValues.append(strGraphObjName + ".position {" + physValPointPos.toString() + "} " + physValPointPos.unit().symbol());
        CPhysValLine physValLine = pGraphObjLine->getLine();
        strlstResultValues.append(strGraphObjName + ".getLine.p1.x: " + physValLine.p1().x().toString());
        strlstResultValues.append(strGraphObjName + ".getLine.p1.y: " + physValLine.p1().y().toString());
        strlstResultValues.append(strGraphObjName + ".getLine.p2.x: " + physValLine.p2().x().toString());
        strlstResultValues.append(strGraphObjName + ".getLine.p2.y: " + physValLine.p2().y().toString());
        CPhysVal physValLength = pGraphObjLine->getLength();
        strlstResultValues.append(strGraphObjName + ".getLength: " + physValLength.toString());
        CPhysVal physValAngle = pGraphObjLine->rotationAngle(Units.Angle.Degree);
        strlstResultValues.append(strGraphObjName + ".rotationAngle: " + physValAngle.toString());
    }
    i_pTestStep->setResultValues(strlstResultValues);
}

//------------------------------------------------------------------------------
void CTest::doTestStepModifyGraphObjGroup( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pTestStep->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "doTestStepModifyGraphObjGroup",
        /* strAddInfo   */ strMthInArgs );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);

    QString strGroupNameParent = i_pTestStep->getConfigValue("GroupNameParent").toString();
    QString strGroupNameParentKeyInTree = i_pTestStep->getConfigValue("GroupNameParentKeyInTree").toString();
    QString strGraphObjName = i_pTestStep->getConfigValue("GraphObjName").toString();
    QString strGraphObjKeyInTree = i_pTestStep->getConfigValue("GraphObjKeyInTree").toString();
    QString strMethod = i_pTestStep->getConfigValue("Method").toString();

    CGraphObj* pGraphObj = m_pDrawingScene->findGraphObj(strGraphObjKeyInTree);
    if (strMethod.compare("setPosition", Qt::CaseInsensitive) == 0) {
        CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphObj);
        if (pGraphObjGroup != nullptr) {
            CPhysValPoint physValPos(*m_pDrawingScene);
            physValPos = i_pTestStep->getConfigValue("Pos").toPointF();
            pGraphObjGroup->setPosition(physValPos);
        }
    }
    else if (strMethod.compare("setRotationAngle", Qt::CaseInsensitive) == 0) {
        CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphObj);
        if (pGraphObjGroup != nullptr) {
            CPhysVal physValAngle(0.0, Units.Angle.Degree, 0.1);
            QString strAngle = i_pTestStep->getConfigValue("Angle").toString();
            physValAngle = strAngle;
            pGraphObjGroup->setRotationAngle(physValAngle);
        }
    }
    else if (strMethod.compare("resizeToContent", Qt::CaseInsensitive) == 0) {
        CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphObj);
        if (pGraphObjGroup != nullptr) {
            pGraphObjGroup->resizeToContent();
        }
    }
    else if (strMethod.compare("removeFromGroup", Qt::CaseInsensitive) == 0) {
        CGraphObjGroup* pGraphObjGroupParent = dynamic_cast<CGraphObjGroup*>(m_pDrawingScene->findGraphObj(strGroupNameParentKeyInTree));
        if (pGraphObjGroupParent != nullptr && pGraphObj != nullptr) {
            pGraphObjGroupParent->removeFromGroup(pGraphObj);
        }
    }

    QStringList strlstResultValues;
    if (pGraphObj != nullptr) {
        if (pGraphObj->isGroup()) {
            CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphObj);
            strlstResultValues.append(pGraphObjGroup->name() + ".pos {" + qPoint2Str(pGraphObjGroup->pos()) + "} px");
            QRectF rectF = pGraphObjGroup->getBoundingRect();
            strlstResultValues.append(pGraphObjGroup->name() + ".topLeft {" + qPoint2Str(rectF.topLeft()) + "} px");
            strlstResultValues.append(pGraphObjGroup->name() + ".bottomRight {" + qPoint2Str(rectF.bottomRight()) + "} px");
            strlstResultValues.append(pGraphObjGroup->name() + ".size {" + qSize2Str(rectF.size()) + "} px");
            CPhysValPoint physValPointPos = pGraphObjGroup->position();
            strlstResultValues.append(pGraphObjGroup->name() + ".position {" + physValPointPos.toString() + "} " + physValPointPos.unit().symbol());
            CPhysValRect physValRect = pGraphObjGroup->getRect();
            strlstResultValues.append(pGraphObjGroup->name() + ".getRect.topLeft.x: " + physValRect.topLeft().x().toString());
            strlstResultValues.append(pGraphObjGroup->name() + ".getRect.topLeft.y: " + physValRect.topLeft().y().toString());
            strlstResultValues.append(pGraphObjGroup->name() + ".getRect.bottomRight.x: " + physValRect.bottomRight().x().toString());
            strlstResultValues.append(pGraphObjGroup->name() + ".getRect.bottomRight.y: " + physValRect.bottomRight().y().toString());
            CPhysValSize physValSize = pGraphObjGroup->getSize();
            strlstResultValues.append(pGraphObjGroup->name() + ".getRect.size.width: " + physValSize.width().toString());
            strlstResultValues.append(pGraphObjGroup->name() + ".getRect.size.height: " + physValSize.height().toString());
            CPhysVal physValAngle = pGraphObjGroup->rotationAngle(Units.Angle.Degree);
            strlstResultValues.append(pGraphObjGroup->name() + ".rotationAngle: " + physValAngle.toString());
            for (CGraphObj* pGraphObjChild : pGraphObjGroup->childs()) {
                QGraphicsItem* pGraphicsItemChild = dynamic_cast<QGraphicsItem*>(pGraphObjChild);
                strlstResultValues.append(pGraphObjChild->name() + ".pos {" + qPoint2Str(pGraphicsItemChild->pos()) + "} px");
                if (pGraphObjChild->isLine()) {
                    CGraphObjLine* pGraphObjChildLine = dynamic_cast<CGraphObjLine*>(pGraphObjChild);
                    physValPointPos = pGraphObjChildLine->position();
                    strlstResultValues.append(pGraphObjChild->name() + ".position {" + physValPointPos.toString() + "} " + physValPointPos.unit().symbol());
                    CPhysValLine physValLine = pGraphObjChildLine->getLine();
                    strlstResultValues.append(pGraphObjChild->name() + ".getLine.p1.x: " + physValLine.p1().x().toString());
                    strlstResultValues.append(pGraphObjChild->name() + ".getLine.p1.y: " + physValLine.p1().y().toString());
                    strlstResultValues.append(pGraphObjChild->name() + ".getLine.p2.x: " + physValLine.p2().x().toString());
                    strlstResultValues.append(pGraphObjChild->name() + ".getLine.p2.y: " + physValLine.p2().y().toString());
                    CPhysVal physValLength = pGraphObjChildLine->getLength();
                    strlstResultValues.append(pGraphObjChild->name() + ".getLength: " + physValLength.toString());
                    physValAngle = pGraphObjChild->rotationAngle(Units.Angle.Degree);
                    strlstResultValues.append(pGraphObjChild->name() + ".rotationAngle: " + physValAngle.toString());
                }
                else if (pGraphObjChild->isGroup()) {
                    CGraphObjGroup* pGraphObjChildGroup = dynamic_cast<CGraphObjGroup*>(pGraphObjChild);
                    physValPointPos = pGraphObjChildGroup->position();
                    strlstResultValues.append(pGraphObjChild->name() + ".position {" + physValPointPos.toString() + "} " + physValPointPos.unit().symbol());
                    CPhysValRect physValRect = pGraphObjChildGroup->getRect();
                    strlstResultValues.append(pGraphObjChild->name() + ".getRect.topLeft.x: " + physValRect.topLeft().x().toString());
                    strlstResultValues.append(pGraphObjChild->name() + ".getRect.topLeft.y: " + physValRect.topLeft().y().toString());
                    strlstResultValues.append(pGraphObjChild->name() + ".getRect.bottomRight.x: " + physValRect.bottomRight().x().toString());
                    strlstResultValues.append(pGraphObjChild->name() + ".getRect.bottomRight.y: " + physValRect.bottomRight().y().toString());
                    strlstResultValues.append(pGraphObjChild->name() + ".getRect.size.width: " + physValRect.size().width().toString());
                    strlstResultValues.append(pGraphObjChild->name() + ".getRect.size.height: " + physValRect.size().height().toString());
                    physValAngle = pGraphObjChild->rotationAngle(Units.Angle.Degree);
                    strlstResultValues.append(pGraphObjChild->name() + ".rotationAngle: " + physValAngle.toString());
                }
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
