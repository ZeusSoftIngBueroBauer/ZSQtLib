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
    QRectF rectF(QPointF(200.0, 250.0), QSizeF(200.0, 100.0));
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        rectF.setTop(350.0);
        rectF.setHeight(-100.0);
    }

    ZS::Test::CTestStepGroup* pGrpTransformRect = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " PhysValRect(" + qRect2Str(rectF) + " " + unit.symbol() + ")",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    //--------------------------------------------------------------------------
    // Rotation Angle: 0.0°
    //--------------------------------------------------------------------------

    CPhysVal physValAngle(0.0, Units.Angle.Degree, 0.1);

    ZS::Test::CTestStepGroup* pGrpTransformRectAngle_0_Degree = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Angle(" + physValAngle.toString() + ")",
        /* pTSGrpParent */ pGrpTransformRect );

    /* setAngle
    ------------
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
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    // Center, Width and Height must remain the same for all following setAngle test steps
    double fxCenterSetAngle0 = rectF.center().x();
    double fyCenterSetAngle0 = rectF.center().y();
    double fWidthSetAngle0 = rectF.width();
    double fHeightSetAngle0 = i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown ? rectF.height() : -rectF.height();
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
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetAngle0)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetAngle0)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetAngle0.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetAngle0, 'f', 1) + " mm");
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

    /* setCenter
    ------------
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
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setCenter", ptCenter);
    pTestStep->setConfigValue("setCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetCenterAngle0)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetCenterAngle0)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetCenterAngle0.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetCenterAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetCenterAngle0, 'f', 1) + " mm");
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

    /* setSize
    ----------
        200   250   300   350   400          200   250   300   350   400   450   600  BottomUp
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
    QSizeF sizeF(300.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        sizeF.setHeight(-200.0);
    }
    double fxCenterSetSizeAngle0 = fxCenterSetAngle0 + (sizeF.width() - fWidthSetAngle0) / 2.0;
    double fyCenterSetSizeAngle0 = fyCenterSetAngle0 + (sizeF.height() - fHeightSetAngle0) / 2.0;
    double fWidthSetSizeAngle0 = sizeF.width();
    double fHeightSetSizeAngle0 = sizeF.height();
    double fLeftSetSizeAngle0 = fxCenterSetSizeAngle0 - fWidthSetSizeAngle0 / 2.0;
    double fRightSetSizeAngle0 = fyCenterSetSizeAngle0 + fWidthSetSizeAngle0 / 2.0;
    double fTopSetSizeAngle0 = fyCenterSetSizeAngle0 - fHeightSetSizeAngle0 / 2.0;
    double fBottomSetSizeAngle0 = fyCenterSetSizeAngle0 + fHeightSetSizeAngle0 / 2.0;
    QPointF ptCenterSetSizeAngle0(fxCenterSetSizeAngle0, fyCenterSetSizeAngle0);
    QPointF ptTopLeftSetSizeAngle0(fLeftSetSizeAngle0, fTopSetSizeAngle0);
    QPointF ptTopRightSetSizeAngle0(fRightSetSizeAngle0, fTopSetSizeAngle0);
    QPointF ptBottomRightSetSizeAngle0(fRightSetSizeAngle0, fBottomSetSizeAngle0);
    QPointF ptBottomLeftSetSizeAngle0(fLeftSetSizeAngle0, fBottomSetSizeAngle0);
    QPointF ptTopCenterSetSizeAngle0(fxCenterSetSizeAngle0, fTopSetSizeAngle0);
    QPointF ptRightCenterSetSizeAngle0(fRightSetSizeAngle0, fyCenterSetSizeAngle0);
    QPointF ptBottomCenterSetSizeAngle0(fxCenterSetSizeAngle0, fBottomSetSizeAngle0);
    QPointF ptLeftCenterSetSizeAngle0(fLeftSetSizeAngle0, fyCenterSetSizeAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setSize(" + qSize2Str(sizeF) + " " + unit.symbol() + ")",
        /* strOperation    */ "setSize(" + qSize2Str(sizeF) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setSize", sizeF);
    pTestStep->setConfigValue("setSize.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetSizeAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetSizeAngle0)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetSizeAngle0)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetSizeAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetSizeAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetSizeAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetSizeAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetSizeAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetSizeAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetSizeAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetSizeAngle0.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetSizeAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetSizeAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetSizeAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetSizeAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetSizeAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetSizeAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetSizeAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetSizeAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidth
    -----------
        200   250   300   350   400          200   250   300   350   400   450   500    BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+-----+-----+-----+-----+       350
         |                       |            |                                   |
     300 +           X           +   =>   300 x                 X                 x       300
         |                       |            |                                   |
     350 +-----+-----+-----+-----+        350 +-----+-----+-----+-----+-----+-----+       250
    */
    CPhysVal physValWidth(300.0, unit, i_drawingSize.imageCoorsResolution());
    double fxCenterSetWidthAngle0 = fxCenterSetAngle0 + (physValWidth.getVal() - fWidthSetAngle0) / 2.0;
    double fyCenterSetWidthAngle0 = fyCenterSetAngle0;
    double fWidthSetWidthAngle0 = physValWidth.getVal();
    double fHeightSetWidthAngle0 = fHeightSetAngle0;
    double fLeftSetWidthAngle0 = fxCenterSetWidthAngle0 - fWidthSetWidthAngle0 / 2.0;
    double fRightSetWidthAngle0 = fxCenterSetWidthAngle0 + fWidthSetWidthAngle0 / 2.0;
    double fTopSetWidthAngle0 = fyCenterSetWidthAngle0 - fHeightSetWidthAngle0 / 2.0;
    double fBottomSetWidthAngle0 = fyCenterSetWidthAngle0 + fHeightSetWidthAngle0 / 2.0;
    QPointF ptCenterSetWidthAngle0(fxCenterSetWidthAngle0, fyCenterSetWidthAngle0);
    QPointF ptTopLeftSetWidthAngle0(fLeftSetWidthAngle0, fTopSetWidthAngle0);
    QPointF ptTopRightSetWidthAngle0(fRightSetWidthAngle0, fTopSetWidthAngle0);
    QPointF ptBottomRightSetWidthAngle0(fRightSetWidthAngle0, fBottomSetWidthAngle0);
    QPointF ptBottomLeftSetWidthAngle0(fLeftSetWidthAngle0, fBottomSetWidthAngle0);
    QPointF ptTopCenterSetWidthAngle0(fxCenterSetWidthAngle0, fTopSetWidthAngle0);
    QPointF ptRightCenterSetWidthAngle0(fRightSetWidthAngle0, fyCenterSetWidthAngle0);
    QPointF ptBottomCenterSetWidthAngle0(fxCenterSetWidthAngle0, fBottomSetWidthAngle0);
    QPointF ptLeftCenterSetWidthAngle0(fLeftSetWidthAngle0, fyCenterSetWidthAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidth(" + physValWidth.toString() + ")",
        /* strOperation    */ "setWidth(" + physValWidth.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidth", physValWidth.toString());
    pTestStep->setConfigValue("setWidth.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetWidthAngle0)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetWidthAngle0)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthAngle0.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetWidthAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetWidthAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingLeftCenter
    -----------------------------
        200   250   300   350   400          300   350   400  BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 +-----+-----+-----+-----+        350 +-----+-----+      250
    */
    QPointF ptMoved = ptTopCenterSetAngle0;
    double fxCenterMoveLeftCenterAngle0 = fRightSetAngle0 - (fRightSetAngle0 - ptMoved.x()) / 2.0;
    double fyCenterMoveLeftCenterAngle0 = fyCenterSetAngle0;
    double fWidthMoveLeftCenterAngle0 = fRightSetAngle0 - ptMoved.x();
    double fHeightMoveLeftCenterAngle0 = fHeightSetAngle0;
    double fLeftMoveLeftCenterAngle0 = fxCenterMoveLeftCenterAngle0 - fWidthMoveLeftCenterAngle0 / 2.0;
    double fRightMoveLeftCenterAngle0 = fxCenterMoveLeftCenterAngle0 + fWidthMoveLeftCenterAngle0 / 2.0;
    double fTopMoveLeftCenterAngle0 = fyCenterMoveLeftCenterAngle0 - fHeightMoveLeftCenterAngle0 / 2.0;
    double fBottomMoveLeftCenterAngle0 = fyCenterMoveLeftCenterAngle0 + fHeightMoveLeftCenterAngle0 / 2.0;
    QPointF ptCenterMoveLeftCenterAngle0(fxCenterMoveLeftCenterAngle0, fyCenterMoveLeftCenterAngle0);
    QPointF ptTopLeftMoveLeftCenterAngle0(fLeftMoveLeftCenterAngle0, fTopMoveLeftCenterAngle0);
    QPointF ptTopRightMoveLeftCenterAngle0(fRightMoveLeftCenterAngle0, fTopMoveLeftCenterAngle0);
    QPointF ptBottomRightMoveLeftCenterAngle0(fRightMoveLeftCenterAngle0, fBottomMoveLeftCenterAngle0);
    QPointF ptBottomLeftMoveLeftCenterAngle0(fLeftMoveLeftCenterAngle0, fBottomMoveLeftCenterAngle0);
    QPointF ptTopCenterMoveLeftCenterAngle0(fxCenterMoveLeftCenterAngle0, fTopMoveLeftCenterAngle0);
    QPointF ptRightCenterMoveLeftCenterAngle0(fRightMoveLeftCenterAngle0, fyCenterMoveLeftCenterAngle0);
    QPointF ptBottomCenterMoveLeftCenterAngle0(fxCenterMoveLeftCenterAngle0, fBottomMoveLeftCenterAngle0);
    QPointF ptLeftCenterMoveLeftCenterAngle0(fLeftMoveLeftCenterAngle0, fyCenterMoveLeftCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveLeftCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthMoveLeftCenterAngle0)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightMoveLeftCenterAngle0)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveLeftCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveLeftCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveLeftCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveLeftCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveLeftCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveLeftCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveLeftCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveLeftCenterAngle0.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthMoveLeftCenterAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightMoveLeftCenterAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveLeftCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingRightCenter
    ------------------------------
        200   250   300   350   400          200   250   300  BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 +-----+-----+-----+-----+        350 +-----+-----+      250
    */
    ptMoved = ptTopCenterSetAngle0;
    double fxCenterMoveRightCenterAngle0 = fLeftSetAngle0 + (ptMoved.x() - fLeftSetAngle0) / 2.0;
    double fyCenterMoveRightCenterAngle0 = fyCenterSetAngle0;
    double fWidthMoveRightCenterAngle0 = ptMoved.x() - fLeftSetAngle0;
    double fHeightMoveRightCenterAngle0 = fHeightSetAngle0;
    double fLeftMoveRightCenterAngle0 = fxCenterMoveRightCenterAngle0 - fWidthMoveRightCenterAngle0 / 2.0;
    double fRightMoveRightCenterAngle0 = fxCenterMoveRightCenterAngle0 + fWidthMoveRightCenterAngle0 / 2.0;
    double fTopMoveRightCenterAngle0 = fyCenterMoveRightCenterAngle0 - fHeightMoveRightCenterAngle0 / 2.0;
    double fBottomMoveRightCenterAngle0 = fyCenterMoveRightCenterAngle0 + fHeightMoveRightCenterAngle0 / 2.0;
    QPointF ptCenterMoveRightCenterAngle0(fxCenterMoveRightCenterAngle0, fyCenterMoveRightCenterAngle0);
    QPointF ptTopLeftMoveRightCenterAngle0(fLeftMoveRightCenterAngle0, fTopMoveRightCenterAngle0);
    QPointF ptTopRightMoveRightCenterAngle0(fRightMoveRightCenterAngle0, fTopMoveRightCenterAngle0);
    QPointF ptBottomRightMoveRightCenterAngle0(fRightMoveRightCenterAngle0, fBottomMoveRightCenterAngle0);
    QPointF ptBottomLeftMoveRightCenterAngle0(fLeftMoveRightCenterAngle0, fBottomMoveRightCenterAngle0);
    QPointF ptTopCenterMoveRightCenterAngle0(fxCenterMoveRightCenterAngle0, fTopMoveRightCenterAngle0);
    QPointF ptRightCenterMoveRightCenterAngle0(fRightMoveRightCenterAngle0, fyCenterMoveRightCenterAngle0);
    QPointF ptBottomCenterMoveRightCenterAngle0(fxCenterMoveRightCenterAngle0, fBottomMoveRightCenterAngle0);
    QPointF ptLeftCenterMoveRightCenterAngle0(fLeftMoveRightCenterAngle0, fyCenterMoveRightCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveRightCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthMoveRightCenterAngle0)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightMoveRightCenterAngle0)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveRightCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveRightCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveRightCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveRightCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveRightCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveRightCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveRightCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveRightCenterAngle0.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthMoveRightCenterAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightMoveRightCenterAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveRightCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeight
    ------------
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
    CPhysVal physValHeight(200.0, i_drawingSize.unit(), i_drawingSize.imageCoorsResolution());
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        physValHeight.setVal(-200.0);
    }
    double fxCenterSetHeightAngle0 = fxCenterSetAngle0;
    double fyCenterSetHeightAngle0 = fyCenterSetAngle0 + (physValHeight.getVal() - fHeightSetAngle0) / 2.0;
    double fWidthSetHeightAngle0 = fWidthSetAngle0;
    double fHeightSetHeightAngle0 = physValHeight.getVal();
    double fLeftSetHeightAngle0 = fxCenterSetHeightAngle0 - fWidthSetHeightAngle0 / 2.0;
    double fRightSetHeightAngle0 = fxCenterSetHeightAngle0 + fWidthSetHeightAngle0 / 2.0;
    double fTopSetHeightAngle0 = fyCenterSetHeightAngle0 - fHeightSetHeightAngle0 / 2.0;
    double fBottomSetHeightAngle0 = fyCenterSetHeightAngle0 + fHeightSetHeightAngle0 / 2.0;
    QPointF ptCenterSetHeightAngle0(fxCenterSetHeightAngle0, fyCenterSetHeightAngle0);
    QPointF ptTopLeftSetHeightAngle0(fLeftSetHeightAngle0, fTopSetHeightAngle0);
    QPointF ptTopRightSetHeightAngle0(fRightSetHeightAngle0, fTopSetHeightAngle0);
    QPointF ptBottomRightSetHeightAngle0(fRightSetHeightAngle0, fBottomSetHeightAngle0);
    QPointF ptBottomLeftSetHeightAngle0(fLeftSetHeightAngle0, fBottomSetHeightAngle0);
    QPointF ptTopCenterSetHeightAngle0(fxCenterSetHeightAngle0, fTopSetHeightAngle0);
    QPointF ptRightCenterSetHeightAngle0(fRightSetHeightAngle0, fyCenterSetHeightAngle0);
    QPointF ptBottomCenterSetHeightAngle0(fxCenterSetHeightAngle0, fBottomSetHeightAngle0);
    QPointF ptLeftCenterSetHeightAngle0(fLeftSetHeightAngle0, fyCenterSetHeightAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeight(" + physValHeight.toString() + ")",
        /* strOperation    */ "setHeight(" + physValHeight.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeight", physValHeight.toString());
    pTestStep->setConfigValue("setHeight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetHeightAngle0)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetHeightAngle0)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightAngle0.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetHeightAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetHeightAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingTopCenter
    -----------------------------
        200   250   300   350   400          200   250   300   350   400   BottomUp
     250 +-----+-----+-----+-----+        250                                350
         |                       |
     300 +           X           +        300 +-----+-----+-----+-----+      300
         |                       |   =>       |           X           |
     350 +-----+-----+-----+-----+        350 +-----+-----+-----+-----+
    */
    ptMoved = ptRightCenterSetAngle0;
    double fxCenterMoveTopCenterAngle0 = fxCenterSetAngle0;
    double fyCenterMoveTopCenterAngle0 = fBottomSetAngle0 - (fBottomSetAngle0 - ptMoved.y()) / 2.0;
    double fWidthMoveTopCenterAngle0 = fWidthSetAngle0;
    double fHeightMoveTopCenterAngle0 = fBottomSetAngle0 - ptMoved.y();
    double fLeftMoveTopCenterAngle0 = fxCenterMoveTopCenterAngle0 - fWidthMoveTopCenterAngle0 / 2.0;
    double fRightMoveTopCenterAngle0 = fxCenterMoveTopCenterAngle0 + fWidthMoveTopCenterAngle0 / 2.0;
    double fTopMoveTopCenterAngle0 = fyCenterMoveTopCenterAngle0 - fHeightMoveTopCenterAngle0 / 2.0;
    double fBottomMoveTopCenterAngle0 = fyCenterMoveTopCenterAngle0 + fHeightMoveTopCenterAngle0 / 2.0;
    QPointF ptCenterMoveTopCenterAngle0(fxCenterMoveTopCenterAngle0, fyCenterMoveTopCenterAngle0);
    QPointF ptTopLeftMoveTopCenterAngle0(fLeftMoveTopCenterAngle0, fTopMoveTopCenterAngle0);
    QPointF ptTopRightMoveTopCenterAngle0(fRightMoveTopCenterAngle0, fTopMoveTopCenterAngle0);
    QPointF ptBottomRightMoveTopCenterAngle0(fRightMoveTopCenterAngle0, fBottomMoveTopCenterAngle0);
    QPointF ptBottomLeftMoveTopCenterAngle0(fLeftMoveTopCenterAngle0, fBottomMoveTopCenterAngle0);
    QPointF ptTopCenterMoveTopCenterAngle0(fxCenterMoveTopCenterAngle0, fTopMoveTopCenterAngle0);
    QPointF ptRightCenterMoveTopCenterAngle0(fRightMoveTopCenterAngle0, fyCenterMoveTopCenterAngle0);
    QPointF ptBottomCenterMoveTopCenterAngle0(fxCenterMoveTopCenterAngle0, fBottomMoveTopCenterAngle0);
    QPointF ptLeftCenterMoveTopCenterAngle0(fLeftMoveTopCenterAngle0, fyCenterMoveTopCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveTopCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthMoveTopCenterAngle0)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightMoveTopCenterAngle0)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveTopCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveTopCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveTopCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveTopCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveTopCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveTopCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveTopCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveTopCenterAngle0.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthMoveTopCenterAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightMoveTopCenterAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveTopCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingBottomCenter
    --------------------------------
        200   250   300   350   400          200   250   300   350   400   BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+-----+-----+      350
         |                       |   =>       |           X           |
     300 +           X           +        300 +-----+-----+-----+-----+      300
         |                       |
     350 +-----+-----+-----+-----+
    */
    ptMoved = ptRightCenterSetAngle0;
    double fxCenterMoveBottomCenterAngle0 = fxCenterSetAngle0;
    double fyCenterMoveBottomCenterAngle0 = fTopSetAngle0 + (ptMoved.y() - fTopSetAngle0) / 2.0;
    double fWidthMoveBottomCenterAngle0 = fWidthSetAngle0;
    double fHeightMoveBottomCenterAngle0 = ptMoved.y() - fTopSetAngle0;
    double fLeftMoveBottomCenterAngle0 = fxCenterMoveBottomCenterAngle0 - fWidthMoveBottomCenterAngle0 / 2.0;
    double fRightMoveBottomCenterAngle0 = fxCenterMoveBottomCenterAngle0 + fWidthMoveBottomCenterAngle0 / 2.0;
    double fTopMoveBottomCenterAngle0 = fyCenterMoveBottomCenterAngle0 - fHeightMoveBottomCenterAngle0 / 2.0;
    double fBottomMoveBottomCenterAngle0 = fyCenterMoveBottomCenterAngle0 + fHeightMoveBottomCenterAngle0 / 2.0;
    QPointF ptCenterMoveBottomCenterAngle0(fxCenterMoveBottomCenterAngle0, fyCenterMoveBottomCenterAngle0);
    QPointF ptTopLeftMoveBottomCenterAngle0(fLeftMoveBottomCenterAngle0, fTopMoveBottomCenterAngle0);
    QPointF ptTopRightMoveBottomCenterAngle0(fRightMoveBottomCenterAngle0, fTopMoveBottomCenterAngle0);
    QPointF ptBottomRightMoveBottomCenterAngle0(fRightMoveBottomCenterAngle0, fBottomMoveBottomCenterAngle0);
    QPointF ptBottomLeftMoveBottomCenterAngle0(fLeftMoveBottomCenterAngle0, fBottomMoveBottomCenterAngle0);
    QPointF ptTopCenterMoveBottomCenterAngle0(fxCenterMoveBottomCenterAngle0, fTopMoveBottomCenterAngle0);
    QPointF ptRightCenterMoveBottomCenterAngle0(fRightMoveBottomCenterAngle0, fyCenterMoveBottomCenterAngle0);
    QPointF ptBottomCenterMoveBottomCenterAngle0(fxCenterMoveBottomCenterAngle0, fBottomMoveBottomCenterAngle0);
    QPointF ptLeftCenterMoveBottomCenterAngle0(fLeftMoveBottomCenterAngle0, fyCenterMoveBottomCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveBottomCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthMoveBottomCenterAngle0)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightMoveBottomCenterAngle0)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveBottomCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveBottomCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveBottomCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveBottomCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveBottomCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveBottomCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveBottomCenterAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveBottomCenterAngle0.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthMoveBottomCenterAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightMoveBottomCenterAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveBottomCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft
    -------------
                                              300   350   400     BottomUp
                                           200 +-----+-----+        400
        200   250   300   350   400            |           |
     250 +-----+-----+-----+-----+         250 |           |        350
         |                       |             x     X     x
     300 x           X           x    =>   300 |           |        300
         |                       |             |           |
     350 +-----+-----x-----+-----+         350 +-----x-----+        250
    */
    QPointF ptTopLeft(300.0, 200.0);
    double fxCenterSetTopLeftAngle0 = ptTopLeft.x() + (fRightSetAngle0 - ptTopLeft.x()) / 2.0;
    double fyCenterSetTopLeftAngle0 = ptTopLeft.y() + (fBottomSetAngle0 - ptTopLeft.y()) / 2.0;
    double fWidthSetTopLeftAngle0 = fRightSetAngle0 - ptTopLeft.x();
    double fHeightSetTopLeftAngle0 = fBottomSetAngle0 - ptTopLeft.y();
    double fLeftSetTopLeftAngle0 = fxCenterSetTopLeftAngle0 - fWidthSetTopLeftAngle0 / 2.0;
    double fRightSetTopLeftAngle0 = fxCenterSetTopLeftAngle0 + fWidthSetTopLeftAngle0 / 2.0;
    double fTopSetTopLeftAngle0 = fyCenterSetTopLeftAngle0 - fHeightSetTopLeftAngle0 / 2.0;
    double fBottomSetTopLeftAngle0 = fyCenterSetTopLeftAngle0 + fHeightSetTopLeftAngle0 / 2.0;
    QPointF ptCenterSetTopLeftAngle0(fxCenterSetTopLeftAngle0, fyCenterSetTopLeftAngle0);
    QPointF ptTopLeftSetTopLeftAngle0(fLeftSetTopLeftAngle0, fTopSetTopLeftAngle0);
    QPointF ptTopRightSetTopLeftAngle0(fRightSetTopLeftAngle0, fTopSetTopLeftAngle0);
    QPointF ptBottomRightSetTopLeftAngle0(fRightSetTopLeftAngle0, fBottomSetTopLeftAngle0);
    QPointF ptBottomLeftSetTopLeftAngle0(fLeftSetTopLeftAngle0, fBottomSetTopLeftAngle0);
    QPointF ptTopCenterSetTopLeftAngle0(fxCenterSetTopLeftAngle0, fTopSetTopLeftAngle0);
    QPointF ptRightCenterSetTopLeftAngle0(fRightSetTopLeftAngle0, fyCenterSetTopLeftAngle0);
    QPointF ptBottomCenterSetTopLeftAngle0(fxCenterSetTopLeftAngle0, fBottomSetTopLeftAngle0);
    QPointF ptLeftCenterSetTopLeftAngle0(fLeftSetTopLeftAngle0, fyCenterSetTopLeftAngle0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopLeft.setY(400.0);
    }
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(" + qPoint2Str(ptTopLeft) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptTopLeft) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptTopLeft);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetTopLeftAngle0)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetTopLeftAngle0)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftAngle0.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetTopLeftAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetTopLeftAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight
    --------------
                                              200   250   300     BottomUp
                                           200 +-----+-----+        400
        200   250   300   350   400            |           |
     250 +-----+-----+-----+-----+         250 |           |        350
         |                       |             x     X     x
     300 x           X           x    =>   300 |           |        300
         |                       |             |           |
     350 +-----+-----x-----+-----+         350 +-----x-----+        250
    */
    QPointF ptTopRight(300.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopRight.setY(400.0);
    }
    double fxCenterSetTopRightAngle0 = fLeftSetAngle0 + (ptTopRight.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetTopRightAngle0 = ptTopRight.y() + (fBottomSetAngle0 - ptTopRight.y()) / 2.0;
    double fWidthSetTopRightAngle0 = ptTopRight.x() - fLeftSetAngle0;
    double fHeightSetTopRightAngle0 = fBottomSetAngle0 - ptTopRight.y();
    double fLeftSetTopRightAngle0 = fxCenterSetTopRightAngle0 - fWidthSetTopRightAngle0 / 2.0;
    double fRightSetTopRightAngle0 = fxCenterSetTopRightAngle0 + fWidthSetTopRightAngle0 / 2.0;
    double fTopSetTopRightAngle0 = fyCenterSetTopRightAngle0 - fHeightSetTopRightAngle0 / 2.0;
    double fBottomSetTopRightAngle0 = fyCenterSetTopRightAngle0 + fHeightSetTopRightAngle0 / 2.0;
    QPointF ptCenterSetTopRightAngle0(fxCenterSetTopRightAngle0, fyCenterSetTopRightAngle0);
    QPointF ptTopLeftSetTopRightAngle0(fLeftSetTopRightAngle0, fTopSetTopRightAngle0);
    QPointF ptTopRightSetTopRightAngle0(fRightSetTopRightAngle0, fTopSetTopRightAngle0);
    QPointF ptBottomRightSetTopRightAngle0(fRightSetTopRightAngle0, fBottomSetTopRightAngle0);
    QPointF ptBottomLeftSetTopRightAngle0(fLeftSetTopRightAngle0, fBottomSetTopRightAngle0);
    QPointF ptTopCenterSetTopRightAngle0(fxCenterSetTopRightAngle0, fTopSetTopRightAngle0);
    QPointF ptRightCenterSetTopRightAngle0(fRightSetTopRightAngle0, fyCenterSetTopRightAngle0);
    QPointF ptBottomCenterSetTopRightAngle0(fxCenterSetTopRightAngle0, fBottomSetTopRightAngle0);
    QPointF ptLeftCenterSetTopRightAngle0(fLeftSetTopRightAngle0, fyCenterSetTopRightAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(" + qPoint2Str(ptTopRight) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptTopRight) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptTopRight);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetTopRightAngle0)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetTopRightAngle0)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightAngle0.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetTopRightAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetTopRightAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight
    -----------------
        200   250   300   350   400           200   250   300      BottomUp
     250 +-----+-----+-----+-----+         250 +-----+-----+        350
         |                       |             |           |
     300 x           X           x    =>   300 +           +        300
         |                       |             |     X     |
     350 +-----+-----x-----+-----+         350 +           +        250
                                               |           |
                                           400 +-----+-----+        200
    */
    QPointF ptBottomRight(300.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomRight.setY(200.0);
    }
    double fxCenterSetBottomRightAngle0 = fLeftSetAngle0 + (ptBottomRight.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetBottomRightAngle0 = fTopSetAngle0 + (ptBottomRight.y() - fTopSetAngle0) / 2.0;
    double fWidthSetBottomRightAngle0 = ptBottomRight.x() - fLeftSetAngle0;
    double fHeightSetBottomRightAngle0 = ptBottomRight.y() - fTopSetAngle0;
    double fLeftSetBottomRightAngle0 = fxCenterSetBottomRightAngle0 - fWidthSetBottomRightAngle0 / 2.0;
    double fRightSetBottomRightAngle0 = fxCenterSetBottomRightAngle0 + fWidthSetBottomRightAngle0 / 2.0;
    double fTopSetBottomRightAngle0 = fyCenterSetBottomRightAngle0 - fHeightSetBottomRightAngle0 / 2.0;
    double fBottomSetBottomRightAngle0 = fyCenterSetBottomRightAngle0 + fHeightSetBottomRightAngle0 / 2.0;
    QPointF ptCenterSetBottomRightAngle0(fxCenterSetBottomRightAngle0, fyCenterSetBottomRightAngle0);
    QPointF ptTopLeftSetBottomRightAngle0(fLeftSetBottomRightAngle0, fTopSetBottomRightAngle0);
    QPointF ptTopRightSetBottomRightAngle0(fRightSetBottomRightAngle0, fTopSetBottomRightAngle0);
    QPointF ptBottomRightSetBottomRightAngle0(fRightSetBottomRightAngle0, fBottomSetBottomRightAngle0);
    QPointF ptBottomLeftSetBottomRightAngle0(fLeftSetBottomRightAngle0, fBottomSetBottomRightAngle0);
    QPointF ptTopCenterSetBottomRightAngle0(fxCenterSetBottomRightAngle0, fTopSetBottomRightAngle0);
    QPointF ptRightCenterSetBottomRightAngle0(fRightSetBottomRightAngle0, fyCenterSetBottomRightAngle0);
    QPointF ptBottomCenterSetBottomRightAngle0(fxCenterSetBottomRightAngle0, fBottomSetBottomRightAngle0);
    QPointF ptLeftCenterSetBottomRightAngle0(fLeftSetBottomRightAngle0, fyCenterSetBottomRightAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(" + qPoint2Str(ptBottomRight) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptBottomRight) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptBottomRight);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetBottomRightAngle0)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetBottomRightAngle0)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightAngle0.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetBottomRightAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetBottomRightAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft
    ----------------
        200   250   300   350   400           300   350   400      BottomUp
     250 +-----+-----+-----+-----+         250 +-----+-----+        350
         |                       |             |           |
     300 x           X           x    =>   300 +           +        300
         |                       |             |     X     |
     350 +-----+-----x-----+-----+         350 +           +        250
                                               |           |
                                           400 +-----+-----+        200
    */
    QPointF ptBottomLeft(300.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomLeft.setY(200.0);
    }
    double fxCenterSetBottomLeftAngle0 = ptBottomLeft.x() + (fRightSetAngle0 - ptBottomLeft.x()) / 2.0;
    double fyCenterSetBottomLeftAngle0 = fTopSetAngle0 + (ptBottomLeft.y() - fTopSetAngle0) / 2.0;
    double fWidthSetBottomLeftAngle0 = fRightSetAngle0 - ptBottomLeft.x();
    double fHeightSetBottomLeftAngle0 = ptBottomLeft.y() - fTopSetAngle0;
    double fLeftSetBottomLeftAngle0 = fxCenterSetBottomLeftAngle0 - fWidthSetBottomLeftAngle0 / 2.0;
    double fRightSetBottomLeftAngle0 = fxCenterSetBottomLeftAngle0 + fWidthSetBottomLeftAngle0 / 2.0;
    double fTopSetBottomLeftAngle0 = fyCenterSetBottomLeftAngle0 - fHeightSetBottomLeftAngle0 / 2.0;
    double fBottomSetBottomLeftAngle0 = fyCenterSetBottomLeftAngle0 + fHeightSetBottomLeftAngle0 / 2.0;
    QPointF ptCenterSetBottomLeftAngle0(fxCenterSetBottomLeftAngle0, fyCenterSetBottomLeftAngle0);
    QPointF ptTopLeftSetBottomLeftAngle0(fLeftSetBottomLeftAngle0, fTopSetBottomLeftAngle0);
    QPointF ptTopRightSetBottomLeftAngle0(fRightSetBottomLeftAngle0, fTopSetBottomLeftAngle0);
    QPointF ptBottomRightSetBottomLeftAngle0(fRightSetBottomLeftAngle0, fBottomSetBottomLeftAngle0);
    QPointF ptBottomLeftSetBottomLeftAngle0(fLeftSetBottomLeftAngle0, fBottomSetBottomLeftAngle0);
    QPointF ptTopCenterSetBottomLeftAngle0(fxCenterSetBottomLeftAngle0, fTopSetBottomLeftAngle0);
    QPointF ptRightCenterSetBottomLeftAngle0(fRightSetBottomLeftAngle0, fyCenterSetBottomLeftAngle0);
    QPointF ptBottomCenterSetBottomLeftAngle0(fxCenterSetBottomLeftAngle0, fBottomSetBottomLeftAngle0);
    QPointF ptLeftCenterSetBottomLeftAngle0(fLeftSetBottomLeftAngle0, fyCenterSetBottomLeftAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(" + qPoint2Str(ptBottomLeft) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptBottomLeft) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptBottomLeft);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetBottomLeftAngle0)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetBottomLeftAngle0)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftAngle0.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftAngle0.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetBottomLeftAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetBottomLeftAngle0, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftAngle0) + "} mm");
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

    /* setAngle
    ------------
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
    pTestStep->setConfigValue("create", rectF);
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
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetAngle30)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetAngle30)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetAngle30.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetAngle30, 'f', 1) + " mm");
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

    /* setCenter
    ------------
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
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setCenter", ptCenter);
    pTestStep->setConfigValue("setCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetCenterAngle30)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetCenterAngle30)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetCenterAngle30.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetCenterAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetCenterAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetCenterAngle30, 'f', 1) + " mm");
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

    /* setSize
    ----------
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
    sizeF = QSizeF(300.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        sizeF.setHeight(-200.0);
    }
    double fWidthSetSizeAngle30 = sizeF.width();
    double fHeightSetSizeAngle30 = sizeF.height();
    QPointF ptCenterSetSizeAngle30(318.3, 368.3);
    QPointF ptTopLeftSetSizeAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetSizeAngle30(498.2, 356.7);
    QPointF ptBottomRightSetSizeAngle30(398.2, 529.9);
    QPointF ptBottomLeftSetSizeAngle30(138.4, 379.9);
    QPointF ptTopCenterSetSizeAngle30 = QPointF((ptTopRightSetSizeAngle30.x() + ptTopLeftSetSizeAngle30.x()) / 2.0,
                                                (ptTopRightSetSizeAngle30.y() + ptTopLeftSetSizeAngle30.y()) / 2.0);
    QPointF ptRightCenterSetSizeAngle30 = QPointF((ptTopRightSetSizeAngle30.x() + ptBottomRightSetSizeAngle30.x()) / 2.0,
                                                  (ptBottomRightSetSizeAngle30.y() + ptTopRightSetSizeAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetSizeAngle30 = QPointF((ptBottomRightSetSizeAngle30.x() + ptBottomLeftSetSizeAngle30.x()) / 2.0,
                                                   (ptBottomRightSetSizeAngle30.y() + ptBottomLeftSetSizeAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetSizeAngle30 = QPointF((ptTopLeftSetSizeAngle30.x() + ptBottomLeftSetSizeAngle30.x()) / 2.0,
                                                 (ptBottomLeftSetSizeAngle30.y() + ptTopLeftSetSizeAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setSize(" + qSize2Str(sizeF) + " " + unit.symbol() + ")",
        /* strOperation    */ "setSize(" + qSize2Str(sizeF) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setSize", sizeF);
    pTestStep->setConfigValue("setSize.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetSizeAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetSizeAngle30)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetSizeAngle30)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetSizeAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetSizeAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetSizeAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetSizeAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetSizeAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetSizeAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetSizeAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetSizeAngle30.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetSizeAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetSizeAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetSizeAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetSizeAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetSizeAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetSizeAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetSizeAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetSizeAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidth
    -----------
        200   250   300   350   400          200   250   300   350   400   450   500    BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+-----+-----+-----+-----+       350
         |                       |            |                                   |
     300 +           X           +   =>   300 x                 X                 x       300
         |                       |            |                                   |
     350 +-----+-----+-----+-----+        350 +-----+-----+-----+-----+-----+-----+       250
    */
    physValWidth.setVal(300.0);
    double fWidthSetWidthAngle30 = physValWidth.getVal();
    double fHeightSetWidthAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetWidthAngle30(343.3, 325.0);
    QPointF ptTopLeftSetWidthAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetWidthAngle30(498.2, 356.7);
    QPointF ptBottomRightSetWidthAngle30(448.2, 443.3);
    QPointF ptBottomLeftSetWidthAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetWidthAngle30 = QPointF((ptTopRightSetWidthAngle30.x() + ptTopLeftSetWidthAngle30.x()) / 2.0,
                                                 (ptTopRightSetWidthAngle30.y() + ptTopLeftSetWidthAngle30.y()) / 2.0);
    QPointF ptRightCenterSetWidthAngle30 = QPointF((ptTopRightSetWidthAngle30.x() + ptBottomRightSetWidthAngle30.x()) / 2.0,
                                                   (ptBottomRightSetWidthAngle30.y() + ptTopRightSetWidthAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetWidthAngle30 = QPointF((ptBottomRightSetWidthAngle30.x() + ptBottomLeftSetWidthAngle30.x()) / 2.0,
                                                    (ptBottomRightSetWidthAngle30.y() + ptBottomLeftSetWidthAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetWidthAngle30 = ptLeftCenterSetAngle30;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidth(" + physValWidth.toString() + ")",
        /* strOperation    */ "setWidth(" + physValWidth.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidth", physValWidth.toString());
    pTestStep->setConfigValue("setWidth.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetWidthAngle30)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetWidthAngle30)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthAngle30.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetWidthAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetWidthAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingLeftCenter
    -----------------------------
        200   250   300   350   400         300   350   400    BottomUp
     250 +-----+-----+-----+-----+       250 +-----x-----+       350
         |                       |           |           |
     300 +           X           +  =>   300 x     X     x       300
         |                       |           |           |
     350 +-----+-----+-----+-----+       350 +-----x-----+       250
    */
    // Set width to half of original width. Thats been done by moving to the top center point of the original rectangle.
    // The resulting center point, when resizing the rectangle by moving the left edge to a new width of 100.0, is the
    // same as setting the width to 300.0 by moving the right edge of the rectangle. But only in this special case.
    ptMoved = ptTopCenterSetAngle30;
    double fWidthMoveLeftCenterAngle30 = fWidthSetAngle30 / 2.0;
    double fHeightMoveLeftCenterAngle30 = fHeightSetAngle30;
    QPointF ptCenterMoveLeftCenterAngle30 = ptCenterSetWidthAngle30;
    QPointF ptTopLeftMoveLeftCenterAngle30 = ptTopCenterSetAngle30;
    QPointF ptTopRightMoveLeftCenterAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightMoveLeftCenterAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftMoveLeftCenterAngle30 = ptBottomCenterSetAngle30;
    QPointF ptTopCenterMoveLeftCenterAngle30 = QPointF((ptTopRightMoveLeftCenterAngle30.x() + ptTopLeftMoveLeftCenterAngle30.x()) / 2.0,
                                                       (ptTopRightMoveLeftCenterAngle30.y() + ptTopLeftMoveLeftCenterAngle30.y()) / 2.0);
    QPointF ptRightCenterMoveLeftCenterAngle30 = ptRightCenterSetAngle30;
    QPointF ptBottomCenterMoveLeftCenterAngle30 = QPointF((ptBottomRightMoveLeftCenterAngle30.x() + ptBottomLeftMoveLeftCenterAngle30.x()) / 2.0,
                                                          (ptBottomRightMoveLeftCenterAngle30.y() + ptBottomLeftMoveLeftCenterAngle30.y()) / 2.0);
    QPointF ptLeftCenterMoveLeftCenterAngle30 = QPointF((ptTopLeftMoveLeftCenterAngle30.x() + ptBottomLeftMoveLeftCenterAngle30.x()) / 2.0,
                                                        (ptBottomLeftMoveLeftCenterAngle30.y() + ptTopLeftMoveLeftCenterAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveLeftCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthMoveLeftCenterAngle30)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightMoveLeftCenterAngle30)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveLeftCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveLeftCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveLeftCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveLeftCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveLeftCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveLeftCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveLeftCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveLeftCenterAngle30.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthMoveLeftCenterAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightMoveLeftCenterAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveLeftCenterAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingRightCenter
    ------------------------------
        200   250   300   350   400          200   250   300   BottomUp
     250 +-----+-----+-----+-----+        250 +-----x-----+      350
         |                       |            |           |
     300 +           X           +   =>   300 x     X     x      300
         |                       |            |           |
     350 +-----+-----+-----+-----+        350 +-----x-----+      250
    */
    // Same width chosen by moving right center as with setWidth call above.
    ptMoved = ptTopCenterSetAngle30;
    double fWidthMoveRightCenterAngle30 = fWidthSetAngle30 / 2.0;
    double fHeightMoveRightCenterAngle30 = fHeightSetAngle30;
    QPointF ptCenterMoveRightCenterAngle30(257.0, 275.0);
    QPointF ptTopLeftMoveRightCenterAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightMoveRightCenterAngle30 = ptTopCenterSetAngle30;
    QPointF ptBottomRightMoveRightCenterAngle30 = ptBottomCenterSetAngle30;
    QPointF ptBottomLeftMoveRightCenterAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterMoveRightCenterAngle30 = QPointF((ptTopRightMoveRightCenterAngle30.x() + ptTopLeftMoveRightCenterAngle30.x()) / 2.0,
                                                        (ptTopRightMoveRightCenterAngle30.y() + ptTopLeftMoveRightCenterAngle30.y()) / 2.0);
    QPointF ptRightCenterMoveRightCenterAngle30 = QPointF((ptTopRightMoveRightCenterAngle30.x() + ptBottomRightMoveRightCenterAngle30.x()) / 2.0,
                                                          (ptTopRightMoveRightCenterAngle30.y() + ptBottomRightMoveRightCenterAngle30.y()) / 2.0);
    QPointF ptBottomCenterMoveRightCenterAngle30 = QPointF((ptBottomRightMoveRightCenterAngle30.x() + ptBottomLeftMoveRightCenterAngle30.x()) / 2.0,
                                                           (ptBottomRightMoveRightCenterAngle30.y() + ptBottomLeftMoveRightCenterAngle30.y()) / 2.0);
    QPointF ptLeftCenterMoveRightCenterAngle30 = ptLeftCenterSetAngle30;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveRightCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthMoveRightCenterAngle30)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightMoveRightCenterAngle30)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveRightCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveRightCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveRightCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveRightCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveRightCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveRightCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveRightCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveRightCenterAngle30.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthMoveRightCenterAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightMoveRightCenterAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveRightCenterAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeight
    ------------
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
    physValHeight.setVal(200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        physValHeight.setVal(-200.0);
    }
    double fWidthSetHeightAngle30 = fWidthSetAngle30;
    double fHeightSetHeightAngle30 = physValHeight.getVal();
    QPointF ptCenterSetHeightAngle30(275.0, 343.3);
    QPointF ptTopLeftSetHeightAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetHeightAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetHeightAngle30(311.6, 479.9);
    QPointF ptBottomLeftSetHeightAngle30(138.4, 379.9);
    QPointF ptTopCenterSetHeightAngle30 = ptTopCenterSetAngle30;
    QPointF ptRightCenterSetHeightAngle30 = QPointF((ptTopRightSetHeightAngle30.x() + ptBottomRightSetHeightAngle30.x()) / 2.0,
                                                    (ptBottomRightSetHeightAngle30.y() + ptTopRightSetHeightAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetHeightAngle30 = QPointF((ptBottomRightSetHeightAngle30.x() + ptBottomLeftSetHeightAngle30.x()) / 2.0,
                                                     (ptBottomRightSetHeightAngle30.y() + ptBottomLeftSetHeightAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetHeightAngle30 = QPointF((ptTopLeftSetHeightAngle30.x() + ptBottomLeftSetHeightAngle30.x()) / 2.0,
                                                   (ptBottomLeftSetHeightAngle30.y() + ptTopLeftSetHeightAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeight(" + physValHeight.toString() + ")",
        /* strOperation    */ "setHeight(" + physValHeight.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeight", physValHeight.toString());
    pTestStep->setConfigValue("setHeight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetHeightAngle30)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetHeightAngle30)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightAngle30.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetHeightAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetHeightAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingTopCenter
    -----------------------------
        200   250   300   350   400         200   250   300   350   400   BottomUp
     250 +-----+-----+-----+-----+       250                                350
         |                       |
     300 +           X           +       300 +-----+-----+-----+-----+      300
         |                       |   =>      |           X           |
     350 +-----+-----+-----+-----+       350 +-----+-----+-----+-----+      250
    */
    ptMoved = ptRightCenterSetAngle30;
    double fWidthMoveTopCenterAngle30 = fWidthSetAngle30;
    double fHeightMoveTopCenterAngle30 = fHeightSetAngle30 / 2.0;
    QPointF ptCenterMoveTopCenterAngle30(287.0, 322.0);
    QPointF ptTopLeftMoveTopCenterAngle30 = ptLeftCenterSetAngle30;
    QPointF ptTopRightMoveTopCenterAngle30 = ptRightCenterSetAngle30;
    QPointF ptBottomRightMoveTopCenterAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftMoveTopCenterAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterMoveTopCenterAngle30 = QPointF((ptTopRightMoveTopCenterAngle30.x() + ptTopLeftMoveTopCenterAngle30.x()) / 2.0,
                                                      (ptTopRightMoveTopCenterAngle30.y() + ptTopLeftMoveTopCenterAngle30.y()) / 2.0);
    QPointF ptRightCenterMoveTopCenterAngle30 = QPointF((ptTopRightMoveTopCenterAngle30.x() + ptBottomRightMoveTopCenterAngle30.x()) / 2.0,
                                                        (ptTopRightMoveTopCenterAngle30.y() + ptBottomRightMoveTopCenterAngle30.y()) / 2.0);
    QPointF ptBottomCenterMoveTopCenterAngle30 = ptBottomCenterSetAngle30;
    QPointF ptLeftCenterMoveTopCenterAngle30 = QPointF((ptTopLeftMoveTopCenterAngle30.x() + ptBottomLeftMoveTopCenterAngle30.x()) / 2.0,
                                                       (ptBottomLeftMoveTopCenterAngle30.y() + ptTopLeftMoveTopCenterAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveTopCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthMoveTopCenterAngle30)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightMoveTopCenterAngle30)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveTopCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveTopCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveTopCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveTopCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveTopCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveTopCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveTopCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveTopCenterAngle30.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthMoveTopCenterAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightMoveTopCenterAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveTopCenterAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingBottomCenter
    --------------------------------
        200   250   300   350   400         200   250   300   350   400   BottomUp
     250 +-----+-----+-----+-----+       250 +-----+-----+-----+-----+      350
         |                       |   =>      |           X           |
     300 +           X           +       300 +-----+-----+-----+-----+      300
         |                       |
     350 +-----+-----+-----+-----+                                          250
    */
    ptMoved = ptRightCenterSetAngle30;
    double fWidthMoveBottomCenterAngle30 = fWidthSetHeightAngle30;
    double fHeightMoveBottomCenterAngle30 = fHeightSetAngle30 / 2.0;
    QPointF ptCenterMoveBottomCenterAngle30(312.0, 278.0);
    QPointF ptTopLeftMoveBottomCenterAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightMoveBottomCenterAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightMoveBottomCenterAngle30 = ptRightCenterSetAngle30;
    QPointF ptBottomLeftMoveBottomCenterAngle30 = ptLeftCenterSetAngle30;
    QPointF ptTopCenterMoveBottomCenterAngle30 = ptTopCenterSetAngle30;
    QPointF ptRightCenterMoveBottomCenterAngle30 = QPointF((ptTopRightMoveBottomCenterAngle30.x() + ptBottomRightMoveBottomCenterAngle30.x()) / 2.0,
                                                           (ptTopRightMoveBottomCenterAngle30.y() + ptBottomRightMoveBottomCenterAngle30.y()) / 2.0);
    QPointF ptBottomCenterMoveBottomCenterAngle30 = QPointF((ptBottomLeftMoveBottomCenterAngle30.x() + ptBottomRightMoveBottomCenterAngle30.x()) / 2.0,
                                                            (ptBottomLeftMoveBottomCenterAngle30.y() + ptBottomRightMoveBottomCenterAngle30.y()) / 2.0);;
    QPointF ptLeftCenterMoveBottomCenterAngle30 = QPointF((ptTopLeftMoveBottomCenterAngle30.x() + ptBottomLeftMoveBottomCenterAngle30.x()) / 2.0,
                                                          (ptBottomLeftMoveBottomCenterAngle30.y() + ptTopLeftMoveBottomCenterAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveBottomCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthMoveBottomCenterAngle30)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightMoveBottomCenterAngle30)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveBottomCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveBottomCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveBottomCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveBottomCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveBottomCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveBottomCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveBottomCenterAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveBottomCenterAngle30.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterMoveBottomCenterAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthMoveBottomCenterAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightMoveBottomCenterAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftMoveBottomCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightMoveBottomCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightMoveBottomCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftMoveBottomCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterMoveBottomCenterAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterMoveBottomCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterMoveBottomCenterAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterMoveBottomCenterAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft
    -------------
    */
    ptTopLeft = QPointF(200.0, 100.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopLeft.setY(500.0);
    }
    double fWidthSetTopLeftAngle30 = sizeF.width();
    double fHeightSetTopLeftAngle30 = sizeF.height();
    QPointF ptCenterSetTopLeftAngle30(318, 368);
    QPointF ptTopLeftSetTopLeftAngle30 = ptTopLeft;
    QPointF ptTopRightSetTopLeftAngle30(498, 357);
    QPointF ptBottomRightSetTopLeftAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetTopLeftAngle30(138, 380);
    QPointF ptTopCenterSetTopLeftAngle30 = QPointF((ptTopRightSetTopLeftAngle30.x() + ptTopLeftSetTopLeftAngle30.x()) / 2.0,
                                                (ptTopRightSetTopLeftAngle30.y() + ptTopLeftSetTopLeftAngle30.y()) / 2.0);
    QPointF ptRightCenterSetTopLeftAngle30 = QPointF((ptTopRightSetTopLeftAngle30.x() + ptBottomRightSetTopLeftAngle30.x()) / 2.0,
                                                  (ptBottomRightSetTopLeftAngle30.y() + ptTopRightSetTopLeftAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetTopLeftAngle30 = QPointF((ptBottomRightSetTopLeftAngle30.x() + ptBottomLeftSetTopLeftAngle30.x()) / 2.0,
                                                   (ptBottomRightSetTopLeftAngle30.y() + ptBottomLeftSetTopLeftAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetTopLeftAngle30 = QPointF((ptTopLeftSetTopLeftAngle30.x() + ptBottomLeftSetTopLeftAngle30.x()) / 2.0,
                                                 (ptBottomLeftSetTopLeftAngle30.y() + ptTopLeftSetTopLeftAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(" + qPoint2Str(ptTopLeft) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptTopLeft) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptTopLeft);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetTopLeftAngle30)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetTopLeftAngle30)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftAngle30.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetTopLeftAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetTopLeftAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight
    --------------
    */
    ptTopRight = QPointF(550.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopRight.setY(400.0);
    }
    double fWidthSetTopRightAngle30 = sizeF.width();
    double fHeightSetTopRightAngle30 = sizeF.height();
    QPointF ptCenterSetTopRightAngle30(318, 368);
    QPointF ptTopLeftSetTopRightAngle30(498, 357);
    QPointF ptTopRightSetTopRightAngle30(498, 357);
    QPointF ptBottomRightSetTopRightAngle30(398, 530);
    QPointF ptBottomLeftSetTopRightAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetTopRightAngle30 = QPointF((ptTopRightSetTopRightAngle30.x() + ptTopLeftSetTopRightAngle30.x()) / 2.0,
                                                (ptTopRightSetTopRightAngle30.y() + ptTopLeftSetTopRightAngle30.y()) / 2.0);
    QPointF ptRightCenterSetTopRightAngle30 = QPointF((ptTopRightSetTopRightAngle30.x() + ptBottomRightSetTopRightAngle30.x()) / 2.0,
                                                  (ptBottomRightSetTopRightAngle30.y() + ptTopRightSetTopRightAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetTopRightAngle30 = QPointF((ptBottomRightSetTopRightAngle30.x() + ptBottomLeftSetTopRightAngle30.x()) / 2.0,
                                                   (ptBottomRightSetTopRightAngle30.y() + ptBottomLeftSetTopRightAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetTopRightAngle30 = QPointF((ptTopLeftSetTopRightAngle30.x() + ptBottomLeftSetTopRightAngle30.x()) / 2.0,
                                                 (ptBottomLeftSetTopRightAngle30.y() + ptTopLeftSetTopRightAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(" + qPoint2Str(ptTopRight) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptTopRight) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptTopRight);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetTopRightAngle30)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetTopRightAngle30)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightAngle30.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetTopRightAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetTopRightAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight
    -----------------
    */
    ptBottomRight = QPointF(400.0, 500.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomRight.setY(100.0);
    }
    double fWidthSetBottomRightAngle30 = sizeF.width();
    double fHeightSetBottomRightAngle30 = sizeF.height();
    QPointF ptCenterSetBottomRightAngle30(318, 368);
    QPointF ptTopLeftSetBottomRightAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetBottomRightAngle30(498, 357);
    QPointF ptBottomRightSetBottomRightAngle30(398, 530);
    QPointF ptBottomLeftSetBottomRightAngle30(138, 380);
    QPointF ptTopCenterSetBottomRightAngle30 = QPointF((ptTopRightSetBottomRightAngle30.x() + ptTopLeftSetBottomRightAngle30.x()) / 2.0,
                                                (ptTopRightSetBottomRightAngle30.y() + ptTopLeftSetBottomRightAngle30.y()) / 2.0);
    QPointF ptRightCenterSetBottomRightAngle30 = QPointF((ptTopRightSetBottomRightAngle30.x() + ptBottomRightSetBottomRightAngle30.x()) / 2.0,
                                                  (ptBottomRightSetBottomRightAngle30.y() + ptTopRightSetBottomRightAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetBottomRightAngle30 = QPointF((ptBottomRightSetBottomRightAngle30.x() + ptBottomLeftSetBottomRightAngle30.x()) / 2.0,
                                                   (ptBottomRightSetBottomRightAngle30.y() + ptBottomLeftSetBottomRightAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetBottomRightAngle30 = QPointF((ptTopLeftSetBottomRightAngle30.x() + ptBottomLeftSetBottomRightAngle30.x()) / 2.0,
                                                 (ptBottomLeftSetBottomRightAngle30.y() + ptTopLeftSetBottomRightAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(" + qPoint2Str(ptBottomRight) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptBottomRight) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptBottomRight);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetBottomRightAngle30)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetBottomRightAngle30)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightAngle30.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetBottomRightAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetBottomRightAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft
    ----------------
        200   250   300   350   400          150   200   250   300   350   400    BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+-----+-----+-----+        350
         |                       |            |                             |
     300 +           X           +        300 |                             |        300
         |                       |            x              X              x
     350 +-----+-----+-----+-----+   =>   350 |                             |        250
                                              |                             |
                                          400 +-----+-----+-----+-----+-----+        200
    */
    ptBottomLeft = QPointF(100.0, 350.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomLeft.setY(250.0);
    }
    double fWidthSetBottomLeftAngle30 = sizeF.width();
    double fHeightSetBottomLeftAngle30 = sizeF.height();
    QPointF ptCenterSetBottomLeftAngle30(318, 368);
    QPointF ptTopLeftSetBottomLeftAngle30(398, 530);
    QPointF ptTopRightSetBottomLeftAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetBottomLeftAngle30(398, 530);
    QPointF ptBottomLeftSetBottomLeftAngle30(138, 380);
    QPointF ptTopCenterSetBottomLeftAngle30 = QPointF((ptTopRightSetBottomLeftAngle30.x() + ptTopLeftSetBottomLeftAngle30.x()) / 2.0,
                                                (ptTopRightSetBottomLeftAngle30.y() + ptTopLeftSetBottomLeftAngle30.y()) / 2.0);
    QPointF ptRightCenterSetBottomLeftAngle30 = QPointF((ptTopRightSetBottomLeftAngle30.x() + ptBottomRightSetBottomLeftAngle30.x()) / 2.0,
                                                  (ptBottomRightSetBottomLeftAngle30.y() + ptTopRightSetBottomLeftAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetBottomLeftAngle30 = QPointF((ptBottomRightSetBottomLeftAngle30.x() + ptBottomLeftSetBottomLeftAngle30.x()) / 2.0,
                                                   (ptBottomRightSetBottomLeftAngle30.y() + ptBottomLeftSetBottomLeftAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetBottomLeftAngle30 = QPointF((ptTopLeftSetBottomLeftAngle30.x() + ptBottomLeftSetBottomLeftAngle30.x()) / 2.0,
                                                 (ptBottomLeftSetBottomLeftAngle30.y() + ptTopLeftSetBottomLeftAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(" + qPoint2Str(ptBottomLeft) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptBottomLeft) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("create", rectF);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptBottomLeft);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(static_cast<int>(fWidthSetBottomLeftAngle30)) + " px");
        strlstExpectedValues.append("Height: " + QString::number(static_cast<int>(fHeightSetBottomLeftAngle30)) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftAngle30.toPoint()) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftAngle30.toPoint()) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(fWidthSetBottomLeftAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(fHeightSetBottomLeftAngle30, 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftAngle30) + "} mm");
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
