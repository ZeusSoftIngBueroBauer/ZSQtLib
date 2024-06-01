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
    CDrawingSize drawingSize;
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
    strlstExpectedValues.append("IsValid: false, DimensionUnit: Pixels, YScale: TopDown, "
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
    strlstExpectedValues.append("IsValid: true, DimensionUnit: Pixels, YScale: TopDown, "
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
    strlstExpectedValues.append("IsValid: true, DimensionUnit: Metric, YScale: TopDown, "
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
        CDrawingSize drawingSize;
        drawingSize.setDimensionUnit(EScaleDimensionUnit::Pixels);
        drawingSize.setImageSize(CPhysVal(100, Units.Length.px), CPhysVal(100, Units.Length.px));

        CDrawGridSettings gridSettings;
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
        CDrawingSize drawingSize;
        drawingSize.setDimensionUnit(EScaleDimensionUnit::Pixels);
        drawingSize.setImageSize(CPhysVal(101, Units.Length.px), CPhysVal(101, Units.Length.px));

        CDrawGridSettings gridSettings;
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
        CDrawingSize drawingSize;
        drawingSize.setDimensionUnit(EScaleDimensionUnit::Pixels);
        drawingSize.setImageSize(CPhysVal(800, Units.Length.px), CPhysVal(600, Units.Length.px));

        CDrawGridSettings gridSettings;
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
            CDrawingSize drawingSize;
            drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
            drawingSize.setMetricUnit(Units.Length.mm);
            drawingSize.setMetricImageCoorsDecimals(1);
            drawingSize.setScreenResolutionInPxPerMM(3.5);
            drawingSize.setImageSize(CPhysVal(100, Units.Length.mm), CPhysVal(100, Units.Length.mm));
            drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::TopDown);

            CDrawGridSettings gridSettings;
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
            CDrawingSize drawingSize;
            drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
            drawingSize.setMetricUnit(Units.Length.mm);
            drawingSize.setMetricImageCoorsDecimals(1);
            drawingSize.setScreenResolutionInPxPerMM(1.0);
            drawingSize.setImageSize(CPhysVal(800, Units.Length.mm), CPhysVal(600, Units.Length.mm));
            drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::TopDown);

            CDrawGridSettings gridSettings;
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
            CDrawingSize drawingSize;
            drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
            drawingSize.setMetricUnit(Units.Length.mm);
            drawingSize.setMetricImageCoorsDecimals(1);
            drawingSize.setScreenResolutionInPxPerMM(3.5);
            drawingSize.setImageSize(CPhysVal(100, Units.Length.mm), CPhysVal(100, Units.Length.mm));
            drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::BottomUp);

            CDrawGridSettings gridSettings;
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
            CDrawingSize drawingSize;
            drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
            drawingSize.setMetricUnit(Units.Length.mm);
            drawingSize.setMetricImageCoorsDecimals(1);
            drawingSize.setScreenResolutionInPxPerMM(1.0);
            drawingSize.setImageSize(CPhysVal(800, Units.Length.mm), CPhysVal(600, Units.Length.mm));
            drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::BottomUp);

            CDrawGridSettings gridSettings;
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
    bool bYAxisTopDown = (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown);
    bool bUnitPixel = (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels);
    QString strUnit = bUnitPixel ? Units.Length.px.symbol() : Units.Length.mm.symbol();
    int iDigits = bUnitPixel ? 0 : i_drawingSize.metricImageCoorsDecimals();

    /* create
    ---------
        200   250   300   350   400      BottomUp
     250 +-----+-----+-----+-----+         350
         |                       |
     300 +           X           +         300
         |                       |
     350 +-----+-----+-----+-----+         250
    */
    QPointF ptTLOrig(200.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBROrig(400.0, bYAxisTopDown ? 350.0 : 250.0);
    QSizeF sizeOrig(200.0, 100.0);

    ZS::Test::CTestStepGroup* pGrpTransformRect = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " PhysValRect(" + qPoint2Str(ptTLOrig) + ", " + qSize2Str(sizeOrig) + " " + unit.symbol() + ")",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    //--------------------------------------------------------------------------
    // Rotation Angle: 0.0�
    //--------------------------------------------------------------------------

    CPhysVal physValAngle(0.0, Units.Angle.Degree, 0.1);
    CPhysVal physValAngleCorrected(0.0, Units.Angle.Degree, 0.1);

    ZS::Test::CTestStepGroup* pGrpTransformRectAngle_0_Degree = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Angle(" + physValAngle.toString() + ")",
        /* pTSGrpParent */ pGrpTransformRect );

    /* setAngle(0�)
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
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("addPhysValShape", "");
    double fWidthSetAngle0 = 200.0;
    double fHeightSetAngle0 = 100.0;
    QPointF ptCenterSetAngle0(300.0, 300.0);
    QPointF ptTopLeftSetAngle0(200.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopRightSetAngle0(400.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomRightSetAngle0(400.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomLeftSetAngle0(200.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopCenterSetAngle0(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptRightCenterSetAngle0(400.0, 300.0);
    QPointF ptBottomCenterSetAngle0(300.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptLeftCenterSetAngle0(200.0, 300.0);
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetAngle0, ", ", 'f', iDigits) + "} " + strUnit);
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
     400       =>    x           X           x       200
                     |                       |
     450             +-----+-----x-----+-----+       150
    */
    QPointF ptCenter(400.0, bYAxisTopDown ? 400.0 : 200.0);
    double fWidthSetCenterAngle0 = 200.0;
    double fHeightSetCenterAngle0 = 100.0;
    QPointF ptCenterSetCenterAngle0 = ptCenter;
    QPointF ptTopLeftSetCenterAngle0(300.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopRightSetCenterAngle0(500.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomRightSetCenterAngle0(500.0, bYAxisTopDown ? 450.0 : 150.0);
    QPointF ptBottomLeftSetCenterAngle0(300.0, bYAxisTopDown ? 450.0 : 150.0);
    QPointF ptTopCenterSetCenterAngle0(400.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptRightCenterSetCenterAngle0(500.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptBottomCenterSetCenterAngle0(400.0, bYAxisTopDown ? 450.0 : 150.0);
    QPointF ptLeftCenterSetCenterAngle0(300.0, bYAxisTopDown ? 400.0 : 200.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setCenter(" + qPoint2Str(ptCenter) + " " + unit.symbol() + ")",
        /* strOperation    */ "setCenter(" + qPoint2Str(ptCenter) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.bottomRight", ptBROrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setCenter", ptCenter);
    pTestStep->setConfigValue("setCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetCenterAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetCenterAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetCenterAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetCenterAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetCenterAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetCenterAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetCenterAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetCenterAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetCenterAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetCenterAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetCenterAngle0, ", ", 'f', iDigits) + "} " + strUnit);
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
    double fWidthSetSizeAngle0 = size300x200.width();
    double fHeightSetSizeAngle0 = size300x200.height();
    QPointF ptCenterSetSizeAngle0(350.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopLeftSetSizeAngle0(200.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopRightSetSizeAngle0(500.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomRightSetSizeAngle0(500.0, bYAxisTopDown ? 450.0 : 150.0);
    QPointF ptBottomLeftSetSizeAngle0(200.0, bYAxisTopDown ? 450.0 : 150.0);
    QPointF ptTopCenterSetSizeAngle0(350.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptRightCenterSetSizeAngle0(500.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomCenterSetSizeAngle0(350.0, bYAxisTopDown ? 450.0 : 150.0);
    QPointF ptLeftCenterSetSizeAngle0(200.0, bYAxisTopDown ? 350.0 : 250.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setSize(" + qSize2Str(size300x200) + " " + unit.symbol() + ")",
        /* strOperation    */ "setSize(" + qSize2Str(size300x200) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setSize", size300x200);
    pTestStep->setConfigValue("setSize.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetSizeAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetSizeAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetSizeAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetSizeAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetSizeAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetSizeAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetSizeAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetSizeAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetSizeAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetSizeAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetSizeAngle0, ", ", 'f', iDigits) + "} " + strUnit);
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
    double fWidthSetWidthAngle0 = physValWidth300.getVal();
    double fHeightSetWidthAngle0 = fHeightSetAngle0;
    QPointF ptCenterSetWidthAngle0(350.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptTopLeftSetWidthAngle0(200.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopRightSetWidthAngle0(500.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomRightSetWidthAngle0(500.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomLeftSetWidthAngle0(200.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopCenterSetWidthAngle0(350.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptRightCenterSetWidthAngle0(500.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptBottomCenterSetWidthAngle0(350.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptLeftCenterSetWidthAngle0(200.0, bYAxisTopDown ? 300.0 : 300.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidth(" + physValWidth300.toString() + ")",
        /* strOperation    */ "setWidth(" + physValWidth300.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidth", physValWidth300.toString());
    pTestStep->setConfigValue("setWidth.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingLeftCenter(BeforeRightBorder)
    ------------------------------------------------
        200   250   300   350   400          300   350   400  BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 +-----+-----+-----+-----+        350 +-----+-----+      250
    */
    QPointF ptMoved = ptTopCenterSetAngle0;
    double fWidthSetWidthMoveLeftCenterBeforeRightBorderAngle0 = 100.0;
    double fHeightSetWidthMoveLeftCenterBeforeRightBorderAngle0 = fHeightSetAngle0;
    QPointF ptCenterSetWidthMoveLeftCenterBeforeRightBorderAngle0(350.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptTopLeftSetWidthMoveLeftCenterBeforeRightBorderAngle0(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopRightSetWidthMoveLeftCenterBeforeRightBorderAngle0(400.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomRightSetWidthMoveLeftCenterBeforeRightBorderAngle0(400.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomLeftSetWidthMoveLeftCenterBeforeRightBorderAngle0(300.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopCenterSetWidthMoveLeftCenterBeforeRightBorderAngle0(350.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptRightCenterSetWidthMoveLeftCenterBeforeRightBorderAngle0(400.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptBottomCenterSetWidthMoveLeftCenterBeforeRightBorderAngle0(350.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptLeftCenterSetWidthMoveLeftCenterBeforeRightBorderAngle0(300.0, bYAxisTopDown ? 300.0 : 300.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(BeforeRightBorder)",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterBeforeRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterBeforeRightBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterBeforeRightBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterBeforeRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterBeforeRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterBeforeRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterBeforeRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterBeforeRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterBeforeRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterBeforeRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterBeforeRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingLeftCenter(BeyondRightBorder)
    ------------------------------------------------
        200   250   300   350   400          400   450   500  BottomUp
     250 TL----+-----+-----+----TR        250 BR----+----BL      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 BL----+-----+-----+----BR        350 TR----+----TL      250
    */
    ptMoved = QPointF(500, bYAxisTopDown ? 250 : 350.0);
    physValAngleCorrected.setVal(180.0);
    double fWidthSetWidthMoveLeftCenterBeyondRightBorderAngle0 = 100.0;
    double fHeightSetWidthMoveLeftCenterBeyondRightBorderAngle0 = fHeightSetAngle0;
    QPointF ptCenterSetWidthMoveLeftCenterBeyondRightBorderAngle0(450.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptTopLeftSetWidthMoveLeftCenterBeyondRightBorderAngle0(500.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopRightSetWidthMoveLeftCenterBeyondRightBorderAngle0(400.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomRightSetWidthMoveLeftCenterBeyondRightBorderAngle0(400.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomLeftSetWidthMoveLeftCenterBeyondRightBorderAngle0(500.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopCenterSetWidthMoveLeftCenterBeyondRightBorderAngle0(450.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptRightCenterSetWidthMoveLeftCenterBeyondRightBorderAngle0(400.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptBottomCenterSetWidthMoveLeftCenterBeyondRightBorderAngle0(450.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptLeftCenterSetWidthMoveLeftCenterBeyondRightBorderAngle0(500.0, bYAxisTopDown ? 300.0 : 300.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(BeyondRightBorder)",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterBeyondRightBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterBeyondRightBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingRightCenter(BeforeLeftBorder)
    ------------------------------------------------
        200   250   300   350   400          200   250   300  BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 +-----+-----+-----+-----+        350 +-----+-----+      250
    */
    ptMoved = ptTopCenterSetAngle0;
    double fWidthSetWidthMoveRightCenterBeforeLeftBorderAngle0 = 100.0;
    double fHeightSetWidthMoveRightCenterBeforeLeftBorderAngle0 = fHeightSetAngle0;
    QPointF ptCenterSetWidthMoveRightCenterBeforeLeftBorderAngle0(250.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptTopLeftSetWidthMoveRightCenterBeforeLeftBorderAngle0(200.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopRightSetWidthMoveRightCenterBeforeLeftBorderAngle0(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomRightSetWidthMoveRightCenterBeforeLeftBorderAngle0(300.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomLeftSetWidthMoveRightCenterBeforeLeftBorderAngle0(200.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopCenterSetWidthMoveRightCenterBeforeLeftBorderAngle0(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptRightCenterSetWidthMoveRightCenterBeforeLeftBorderAngle0(300.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptBottomCenterSetWidthMoveRightCenterBeforeLeftBorderAngle0(250.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptLeftCenterSetWidthMoveRightCenterBeforeLeftBorderAngle0(200.0, bYAxisTopDown ? 300.0 : 300.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(BeforeLeftBorder)",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterBeforeLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterBeforeLeftBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterBeforeLeftBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterBeforeLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterBeforeLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterBeforeLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterBeforeLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterBeforeLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterBeforeLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterBeforeLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterBeforeLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingRightCenter(BeyondLeftBorder)
    ------------------------------------------------
        200   250   300   350   400          100   150   200  BottomUp
     250 TL----+-----+-----+----TR        250 BR----+----BL      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 BL----+-----+-----+----BR        350 TR----+----TL      250
    */
    ptMoved = QPointF(100.0, bYAxisTopDown ? 250.0 : 350.0);
    physValAngleCorrected.setVal(180.0);
    double fWidthSetWidthMoveRightCenterBeyondLeftBorderAngle0 = 100;
    double fHeightSetWidthMoveRightCenterBeyondLeftBorderAngle0 = fHeightSetAngle0;
    QPointF ptCenterSetWidthMoveRightCenterBeyondLeftBorderAngle0(150.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptTopLeftSetWidthMoveRightCenterBeyondLeftBorderAngle0(200.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopRightSetWidthMoveRightCenterBeyondLeftBorderAngle0(100.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomRightSetWidthMoveRightCenterBeyondLeftBorderAngle0(100.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomLeftSetWidthMoveRightCenterBeyondLeftBorderAngle0(200.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopCenterSetWidthMoveRightCenterBeyondLeftBorderAngle0(150.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptRightCenterSetWidthMoveRightCenterBeyondLeftBorderAngle0(100.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptBottomCenterSetWidthMoveRightCenterBeyondLeftBorderAngle0(150.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptLeftCenterSetWidthMoveRightCenterBeyondLeftBorderAngle0(200.0, bYAxisTopDown ? 300.0 : 300.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(BeyondLeftBorder)",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterBeyondLeftBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterBeyondLeftBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
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
    double fWidthSetHeightAngle0 = fWidthSetAngle0;
    double fHeightSetHeightAngle0 = physValHeight200.getVal();
    QPointF ptCenterSetHeightAngle0(300.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopLeftSetHeightAngle0(200.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopRightSetHeightAngle0(400.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomRightSetHeightAngle0(400.0, bYAxisTopDown ? 450.0 : 150.0);
    QPointF ptBottomLeftSetHeightAngle0(200.0, bYAxisTopDown ? 450.0: 150.0);
    QPointF ptTopCenterSetHeightAngle0(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptRightCenterSetHeightAngle0(400.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomCenterSetHeightAngle0(300.0, bYAxisTopDown ? 450.0 : 150.0);
    QPointF ptLeftCenterSetHeightAngle0(200.0, bYAxisTopDown ? 350.0 : 250.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeight(" + physValHeight200.toString() + ")",
        /* strOperation    */ "setHeight(" + physValHeight200.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeight", physValHeight200.toString());
    pTestStep->setConfigValue("setHeight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingTopCenter(BeforeBottomBorder)
    -------------------------------------------------
        200   250   300   350   400          200   250   300   350   400   BottomUp
     250 +-----+-----+-----+-----+        250                                350
         |                       |
     300 +           X           +        300 +-----+-----+-----+-----+      300
         |                       |   =>       |           X           |
     350 +-----+-----+-----+-----+        350 +-----+-----+-----+-----+      250
    */
    ptMoved = ptRightCenterSetAngle0;
    double fWidthSetHeightMoveTopCenterBeforeBottomBorderAngle0 = fWidthSetAngle0;
    double fHeightSetHeightMoveTopCenterBeforeBottomBorderAngle0 = 50.0;
    QPointF ptCenterSetHeightMoveTopCenterBeforeBottomBorderAngle0(300.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptTopLeftSetHeightMoveTopCenterBeforeBottomBorderAngle0(200.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptTopRightSetHeightMoveTopCenterBeforeBottomBorderAngle0(400.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptBottomRightSetHeightMoveTopCenterBeforeBottomBorderAngle0(400.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomLeftSetHeightMoveTopCenterBeforeBottomBorderAngle0(200.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopCenterSetHeightMoveTopCenterBeforeBottomBorderAngle0(300.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptRightCenterSetHeightMoveTopCenterBeforeBottomBorderAngle0 (400.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptBottomCenterSetHeightMoveTopCenterBeforeBottomBorderAngle0(300.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptLeftCenterSetHeightMoveTopCenterBeforeBottomBorderAngle0(200.0, bYAxisTopDown ? 325.0 : 275.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(BeforeBottomBorder)",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterBeforeBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterBeforeBottomBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterBeforeBottomBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterBeforeBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterBeforeBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterBeforeBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterBeforeBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterBeforeBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterBeforeBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterBeforeBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterBeforeBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingTopCenter(BeyondBottomBorder)
    -------------------------------------------------
        200   250   300   350   400          200   250   300   350   400   BottomUp
     250 TL----+-----+-----+----TR                                           350
         |                       |
     300 +           X           +                                           300
         |                       |
     350 BL----+-----+-----+----BR        350 BR----+-----+-----+----BL      250
                                              |           X           |
                                          400 TR----+-----+-----+----TL      200
    */
    ptMoved = QPointF(400, bYAxisTopDown ? 400.0 : 200.0);
    physValAngleCorrected.setVal(180.0);
    double fWidthSetHeightMoveTopCenterBeyondBottomBorderAngle0 = fWidthSetAngle0;
    double fHeightSetHeightMoveTopCenterBeyondBottomBorderAngle0 = 50.0;
    QPointF ptCenterSetHeightMoveTopCenterBeyondBottomBorderAngle0(300.0, bYAxisTopDown ? 375.0 : 225.0);
    QPointF ptTopLeftSetHeightMoveTopCenterBeyondBottomBorderAngle0(400.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptTopRightSetHeightMoveTopCenterBeyondBottomBorderAngle0(200.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptBottomRightSetHeightMoveTopCenterBeyondBottomBorderAngle0(200.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomLeftSetHeightMoveTopCenterBeyondBottomBorderAngle0(400.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopCenterSetHeightMoveTopCenterBeyondBottomBorderAngle0(300.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptRightCenterSetHeightMoveTopCenterBeyondBottomBorderAngle0(200.0, bYAxisTopDown ? 375.0 : 225.0);
    QPointF ptBottomCenterSetHeightMoveTopCenterBeyondBottomBorderAngle0(300.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptLeftCenterSetHeightMoveTopCenterBeyondBottomBorderAngle0(400.0, bYAxisTopDown ? 375.0 : 225.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(BeyondBottomBorder)",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterBeyondBottomBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterBeyondBottomBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingBottomCenter(BeforeTopBorder)
    -------------------------------------------------
        200   250   300   350   400          200   250   300   350   400   BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+-----+-----+      350
         |                       |   =>       |           X           |
     300 +           X           +        300 +-----+-----+-----+-----+      300
         |                       |
     350 +-----+-----+-----+-----+
    */
    ptMoved = ptRightCenterSetAngle0;
    double fWidthSetHeightMoveBottomCenterBeforeTopBorderAngle0 = fWidthSetAngle0;
    double fHeightSetHeightMoveBottomCenterBeforeTopBorderAngle0 = 50.0;
    QPointF ptCenterSetHeightMoveBottomCenterBeforeTopBorderAngle0(300.0, bYAxisTopDown ? 275.0 : 325.0);
    QPointF ptTopLeftSetHeightMoveBottomCenterBeforeTopBorderAngle0(200.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopRightSetHeightMoveBottomCenterBeforeTopBorderAngle0(400.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomRightSetHeightMoveBottomCenterBeforeTopBorderAngle0(400.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptBottomLeftSetHeightMoveBottomCenterBeforeTopBorderAngle0(200.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptTopCenterSetHeightMoveBottomCenterBeforeTopBorderAngle0(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptRightCenterSetHeightMoveBottomCenterBeforeTopBorderAngle0(400.0, bYAxisTopDown ? 275.0 : 325.0);
    QPointF ptBottomCenterSetHeightMoveBottomCenterBeforeTopBorderAngle0(300.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptLeftCenterSetHeightMoveBottomCenterBeforeTopBorderAngle0(200.0, bYAxisTopDown ? 275.0 : 325.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(BeforeTopBorder)",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenterBeforeTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenterBeforeTopBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenterBeforeTopBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenterBeforeTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenterBeforeTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenterBeforeTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenterBeforeTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenterBeforeTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenterBeforeTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenterBeforeTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenterBeforeTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingBottomCenter(BeyondTopBorder)
    -------------------------------------------------


        200   250   300   350   400          200   250   300   350   400   BottomUp
                                          200 BR----+-----+-----+-----BL     400
                                              |           X           |
     250 +-----+-----+-----+-----+        250 TR----+-----+-----+-----TL     350
         |                       |
     300 +           X           +                                           300
         |                       |
     350 +-----+-----+-----+-----+
    */
    ptMoved = QPointF(400.0, bYAxisTopDown ? 200.0 : 400);
    physValAngleCorrected.setVal(180.0);
    double fWidthSetHeightMoveBottomCenterBeyondTopBorderAngle0 = fWidthSetAngle0;
    double fHeightSetHeightMoveBottomCenterBeyondTopBorderAngle0 = 50.0;
    QPointF ptCenterSetHeightMoveBottomCenterBeyondTopBorderAngle0(300.0, bYAxisTopDown ? 225.0 : 375.0);
    QPointF ptTopLeftSetHeightMoveBottomCenterBeyondTopBorderAngle0(400.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopRightSetHeightMoveBottomCenterBeyondTopBorderAngle0(200.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomRightSetHeightMoveBottomCenterBeyondTopBorderAngle0(200.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptBottomLeftSetHeightMoveBottomCenterBeyondTopBorderAngle0(400.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptTopCenterSetHeightMoveBottomCenterBeyondTopBorderAngle0(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptRightCenterSetHeightMoveBottomCenterBeyondTopBorderAngle0(200.0, bYAxisTopDown ? 225.0 : 375.0);
    QPointF ptBottomCenterSetHeightMoveBottomCenterBeyondTopBorderAngle0(300.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptLeftCenterSetHeightMoveBottomCenterBeyondTopBorderAngle0(400.0, bYAxisTopDown ? 225.0 : 375.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(BeyondTopBorder)",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenterBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenterBeyondTopBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenterBeyondTopBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenterBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenterBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenterBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenterBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenterBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenterBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenterBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenterBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(BeforeBottomRight)
    --------------------------------
            200   250   300   350   400
    TopDown                              BottomUp
      200               TL'---------TR'    400
                         |           |
      250    TL----------|----------TR     350
             |           |     X'    |
      300    |           |           |     300
             |           |           |
      350    BL---------BL'---------BR     250
    */
    QPointF ptSetTopLeftBeforeBottomRightAngle0(300.0, bYAxisTopDown ? 200.0 : 400);
    double fWidthSetTopLeftBeforeBottomRightAngle0 = 100.0;
    double fHeightSetTopLeftBeforeBottomRightAngle0 = 150.0;
    QPointF ptCenterSetTopLeftBeforeBottomRightAngle0(350.0, bYAxisTopDown ? 275.0 : 325.0);
    QPointF ptTopLeftSetTopLeftBeforeBottomRightAngle0 = ptSetTopLeftBeforeBottomRightAngle0;
    QPointF ptTopRightSetTopLeftBeforeBottomRightAngle0(400.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptBottomRightSetTopLeftBeforeBottomRightAngle0(400.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomLeftSetTopLeftBeforeBottomRightAngle0(300.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopCenterSetTopLeftBeforeBottomRightAngle0(350.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptRightCenterSetTopLeftBeforeBottomRightAngle0(400.0, bYAxisTopDown ? 275.0 : 325.0);
    QPointF ptBottomCenterSetTopLeftBeforeBottomRightAngle0(350.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptLeftCenterSetTopLeftBeforeBottomRightAngle0(300.0, bYAxisTopDown ? 275.0 : 325.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(BeforeBottomRight)",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptSetTopLeftBeforeBottomRightAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptSetTopLeftBeforeBottomRightAngle0);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeforeBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeforeBottomRightAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeforeBottomRightAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeforeBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeforeBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeforeBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeforeBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeforeBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeforeBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeforeBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeforeBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(BeyondRightBorder)
    --------------------------------
            200   250   300   350   400   450   500
    TopDown                                            BottomUp
      200                                                400

      250    TL---------------------TR                   350
             |                       |
      300    |           X          BL'---------TL'      300
             |                       |     X'    |
      350    BL---------------------BR----------TR'      250
    */
    QPointF ptSetTopLeftBeyondRightBorderAngle0(500.0, bYAxisTopDown ? 300.0 : 300.0);
    physValAngleCorrected.setVal(90.0);
    double fWidthSetTopLeftBeyondRightBorderAngle0 = 50.0;
    double fHeightSetTopLeftBeyondRightBorderAngle0 = 100.0;
    QPointF ptCenterSetTopLeftBeyondRightBorderAngle0(450.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptTopLeftSetTopLeftBeyondRightBorderAngle0 = ptSetTopLeftBeyondRightBorderAngle0;
    QPointF ptTopRightSetTopLeftBeyondRightBorderAngle0(500.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomRightSetTopLeftBeyondRightBorderAngle0(400.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomLeftSetTopLeftBeyondRightBorderAngle0(400.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptTopCenterSetTopLeftBeyondRightBorderAngle0(500.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptRightCenterSetTopLeftBeyondRightBorderAngle0(450.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomCenterSetTopLeftBeyondRightBorderAngle0(400.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptLeftCenterSetTopLeftBeyondRightBorderAngle0(450.0, bYAxisTopDown ? 300.0 : 300.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(BeyondRightBorder)",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptSetTopLeftBeyondRightBorderAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptSetTopLeftBeyondRightBorderAngle0);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondRightBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondRightBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(BeyondBottomBorder)
    ---------------------------------
            200   250   300   350   400   450   500
    TopDown                                            BottomUp
      200                                                400

      250    TL---------------------TR                   350
             |                       |
      300    |           X           |                   300
             |                       |
      350    BL---------TR'---------BR                   250
                         |     X'    |
      400               TL'---------BL'                  200
    */
    QPointF ptSetTopLeftBeyondBottomBorderAngle0(300.0, bYAxisTopDown ? 400.0 : 200.0);
    physValAngleCorrected.setVal(270.0);
    double fWidthSetTopLeftBeyondBottomBorderAngle0 = 50.0;
    double fHeightSetTopLeftBeyondBottomBorderAngle0 = 100.0;
    QPointF ptCenterSetTopLeftBeyondBottomBorderAngle0(350.0, bYAxisTopDown ? 375.0 : 225.0);
    QPointF ptTopLeftSetTopLeftBeyondBottomBorderAngle0 = ptSetTopLeftBeyondBottomBorderAngle0;
    QPointF ptTopRightSetTopLeftBeyondBottomBorderAngle0(300.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomRightSetTopLeftBeyondBottomBorderAngle0(400.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomLeftSetTopLeftBeyondBottomBorderAngle0(400.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptTopCenterSetTopLeftBeyondBottomBorderAngle0(300.0, bYAxisTopDown ? 375.0 : 225.0);
    QPointF ptRightCenterSetTopLeftBeyondBottomBorderAngle0(350.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomCenterSetTopLeftBeyondBottomBorderAngle0(400.0, bYAxisTopDown ? 375.0 : 225.0);
    QPointF ptLeftCenterSetTopLeftBeyondBottomBorderAngle0(350.0, bYAxisTopDown ? 400.0 : 200.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(BeyondBottomBorder)",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptSetTopLeftBeyondBottomBorderAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptSetTopLeftBeyondBottomBorderAngle0);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondBottomBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondBottomBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(BeyondBottomRight)
    --------------------------------
        200   250   300   350   400   450   500       BottomUp
     250 TL---------------------TR                       350
         |                       |
     300 |           X           |                       300
         |                       |
     350 BL---------------------BR----------BL'          250
                                 |           |
     400                         |     X'    |           200
                                 |           |
     450                        TR'---------TL'          150
    */
    QPointF ptTopLeftBeyondBottomRightAngle0(500.0, bYAxisTopDown ? 450.0 : 150.0);
    physValAngleCorrected.setVal(180.0);
    double fWidthSetTopLeftBeyondBottomRightAngle0 = 100.0;
    double fHeightSetTopLeftBeyondBottomRightAngle0 = 100.0;
    QPointF ptCenterSetTopLeftBeyondBottomRightAngle0(450.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptTopLeftSetTopLeftBeyondBottomRightAngle0 = ptTopLeftBeyondBottomRightAngle0;
    QPointF ptTopRightSetTopLeftBeyondBottomRightAngle0(400.0, bYAxisTopDown ? 450.0 : 150.0);
    QPointF ptBottomRightSetTopLeftBeyondBottomRightAngle0(400.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomLeftSetTopLeftBeyondBottomRightAngle0(500.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopCenterSetTopLeftBeyondBottomRightAngle0(450.0, bYAxisTopDown ? 450.0 : 150.0);
    QPointF ptRightCenterSetTopLeftBeyondBottomRightAngle0(400.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptBottomCenterSetTopLeftBeyondBottomRightAngle0(450.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptLeftCenterSetTopLeftBeyondBottomRightAngle0(500.0, bYAxisTopDown ? 400.0 : 200.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(BeyondBottomRight)",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptTopLeftBeyondBottomRightAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptTopLeftBeyondBottomRightAngle0);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondBottomRightAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondBottomRightAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondBottomRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(BeforeBottomLeft)
    --------------------------------
            200   250   300   350   400
    TopDown                              BottomUp
      200   TL'---------TR'                400
             |           |
      250   TL-----------|----------TR     350
             |     X'    |           |
      300    |           |     X     |     300
             |           |           |
      350   BL----------BR'---------BR     250
    */
    QPointF ptSetTopRightBeforeBottomLeftAngle0(300.0, bYAxisTopDown ? 200.0 : 400.0);
    double fWidthSetTopRightBeforeBottomLeftAngle0 = 100.0;
    double fHeightSetTopRightBeforeBottomLeftAngle0 = 150.0;
    QPointF ptCenterSetTopRightBeforeBottomLeftAngle0(250.0, bYAxisTopDown ? 275.0 : 325.0);
    QPointF ptTopLeftSetTopRightBeforeBottomLeftAngle0(200.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptTopRightSetTopRightBeforeBottomLeftAngle0 = ptSetTopRightBeforeBottomLeftAngle0;
    QPointF ptBottomRightSetTopRightBeforeBottomLeftAngle0(300.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomLeftSetTopRightBeforeBottomLeftAngle0(200.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopCenterSetTopRightBeforeBottomLeftAngle0(250.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptRightCenterSetTopRightBeforeBottomLeftAngle0(300.0, bYAxisTopDown ? 275.0 : 325.0);
    QPointF ptBottomCenterSetTopRightBeforeBottomLeftAngle0(250.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptLeftCenterSetTopRightBeforeBottomLeftAngle0(200.0, bYAxisTopDown ? 275.0 : 325.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(BeforeBottomLeft)",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptSetTopRightBeforeBottomLeftAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptSetTopRightBeforeBottomLeftAngle0);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeforeBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeforeBottomLeftAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeforeBottomLeftAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeforeBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeforeBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeforeBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeforeBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeforeBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeforeBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeforeBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeforeBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(BeyondLeftBorder)
    ---------------------------------
             100   150  200   250   300   350   400
    TopDown                                            BottomUp
     250                TL-----------------------TR      350
                         |                       |
     300    TR'---------BR'          X           |       300
             |     X'    |                       |
     350    TL'----------BL----------------------BR      250
    */
    QPointF ptSetTopRightBeyondLeftBorderAngle0(100.0, bYAxisTopDown ? 300.0 : 300.0);
    physValAngleCorrected.setVal(270.0);
    double fWidthSetTopRightBeyondLeftBorderAngle0 = 50.0;
    double fHeightSetTopRightBeyondLeftBorderAngle0 = 100.0;
    QPointF ptCenterSetTopRightBeyondLeftBorderAngle0(150.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptTopLeftSetTopRightBeyondLeftBorderAngle0(100.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopRightSetTopRightBeyondLeftBorderAngle0 = ptSetTopRightBeyondLeftBorderAngle0;
    QPointF ptBottomRightSetTopRightBeyondLeftBorderAngle0(200.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptBottomLeftSetTopRightBeyondLeftBorderAngle0(200.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopCenterSetTopRightBeyondLeftBorderAngle0(100.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptRightCenterSetTopRightBeyondLeftBorderAngle0(150.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptBottomCenterSetTopRightBeyondLeftBorderAngle0(200.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptLeftCenterSetTopRightBeyondLeftBorderAngle0(150.0, bYAxisTopDown ? 350.0 : 250.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(BeyondLeftBorder)",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptSetTopRightBeyondLeftBorderAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptSetTopRightBeyondLeftBorderAngle0);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondLeftBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondLeftBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(BeyondBottomBorder)
    ----------------------------------
             100   150  200   250   300   350   400
    TopDown                                           BottomUp
     250                TL-----------------------TR      350
                         |                       |
     300                 |           X           |       300
                         |                       |
     350                BL----------TL'----------BR      250
                         |     X'    |
     400                BR'---------TR'                  200
    */
    QPointF ptSetTopRightBeyondBottomBorderAngle0(300.0, bYAxisTopDown ? 400.0 : 200.0);
    physValAngleCorrected.setVal(90.0);
    double fWidthSetTopRightBeyondBottomBorderAngle0 = 50.0;
    double fHeightSetTopRightBeyondBottomBorderAngle0 = 100.0;
    QPointF ptCenterSetTopRightBeyondBottomBorderAngle0(250.0, bYAxisTopDown ? 375.0 : 225.0);
    QPointF ptTopLeftSetTopRightBeyondBottomBorderAngle0(300.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopRightSetTopRightBeyondBottomBorderAngle0 = ptSetTopRightBeyondBottomBorderAngle0;
    QPointF ptBottomRightSetTopRightBeyondBottomBorderAngle0(200.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptBottomLeftSetTopRightBeyondBottomBorderAngle0(200.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopCenterSetTopRightBeyondBottomBorderAngle0(300.0, bYAxisTopDown ? 375.0 : 225.0);
    QPointF ptRightCenterSetTopRightBeyondBottomBorderAngle0(250.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptBottomCenterSetTopRightBeyondBottomBorderAngle0(200.0, bYAxisTopDown ? 375.0 : 225.0);
    QPointF ptLeftCenterSetTopRightBeyondBottomBorderAngle0(250.0, bYAxisTopDown ? 350.0 : 250.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(BeyondBottomBorder)",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptSetTopRightBeyondBottomBorderAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptSetTopRightBeyondBottomBorderAngle0);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondBottomBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondBottomBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondBottomBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(BeyondBottomLeft)
    --------------------------------
             100   150  200   250   300   350   400
    TopDown                                            BottomUp
     250                TL-----------------------TR      350
                         |                       |
     300                 |           X           |       300
                         |                       |
     350    BR'----------BL----------------------BR      250
             |           |
     400     |     X'    |                               200
             |           |
     450    TR'---------TL'                              150
    */
    QPointF ptSetTopRightBeyondBottomLeftAngle0(100.0, bYAxisTopDown ? 450.0 : 150.0);
    physValAngleCorrected.setVal(180.0);
    double fWidthSetTopRightBeyondBottomLeftAngle0 = 100.0;
    double fHeightSetTopRightBeyondBottomLeftAngle0 = 100.0;
    QPointF ptCenterSetTopRightBeyondBottomLeftAngle0(150.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptTopLeftSetTopRightBeyondBottomLeftAngle0(200.0, bYAxisTopDown ? 450.0 : 150.0);
    QPointF ptTopRightSetTopRightBeyondBottomLeftAngle0 = ptSetTopRightBeyondBottomLeftAngle0;
    QPointF ptBottomRightSetTopRightBeyondBottomLeftAngle0(100.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomLeftSetTopRightBeyondBottomLeftAngle0(200.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopCenterSetTopRightBeyondBottomLeftAngle0(150.0, bYAxisTopDown ? 450.0 : 150.0);
    QPointF ptRightCenterSetTopRightBeyondBottomLeftAngle0(100.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptBottomCenterSetTopRightBeyondBottomLeftAngle0(150.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptLeftCenterSetTopRightBeyondBottomLeftAngle0(200.0, bYAxisTopDown ? 400.0 : 200.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(BeyondBottomLeft",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptSetTopRightBeyondBottomLeftAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptSetTopRightBeyondBottomLeftAngle0);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondBottomLeftAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondBottomLeftAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondBottomLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight(BeforeTopLeft)
    --------------------------------
            200   250   300   350   400
    TopDown                              BottomUp
      250   TL----------TR'----------TR    350
             |           |           |
      300    |           |     X     |     300
             |     X'    |           |
      350   BL-----------|-----------BR    250
             |           |
      400   BL'---------BR'                200
    */
    QPointF ptSetBottomRightBeforeTopLeftAngle0(300.0, bYAxisTopDown ? 400.0 : 200.0);
    double fWidthSetBottomRightBeforeTopLeftAngle0 = 100.0;
    double fHeightSetBottomRightBeforeTopLeftAngle0 = 150.0;
    QPointF ptCenterSetBottomRightBeforeTopLeftAngle0(250.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptTopLeftSetBottomRightBeforeTopLeftAngle0(200.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopRightSetBottomRightBeforeTopLeftAngle0(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomRightSetBottomRightBeforeTopLeftAngle0 = ptSetBottomRightBeforeTopLeftAngle0;
    QPointF ptBottomLeftSetBottomRightBeforeTopLeftAngle0(200.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptTopCenterSetBottomRightBeforeTopLeftAngle0(250.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptRightCenterSetBottomRightBeforeTopLeftAngle0(300.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptBottomCenterSetBottomRightBeforeTopLeftAngle0(250.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptLeftCenterSetBottomRightBeforeTopLeftAngle0(200.0, bYAxisTopDown ? 325.0 : 275.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(BeforeTopLeft)",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptSetBottomRightBeforeTopLeftAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptSetBottomRightBeforeTopLeftAngle0);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeforeTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeforeTopLeftAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeforeTopLeftAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeforeTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeforeTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeforeTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeforeTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeforeTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeforeTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeforeTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeforeTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight(BeyondTopBorder)
    ----------------------------------
             100   150  200   250   300   350   400
    TopDown                                           BottomUp
     200                TR'----------BR'                 400
                         |     X'    |
     250                TL-----------BL'---------TR      350
                         |                       |
     300                 |           X           |       300
                         |                       |
     350                BL-----------------------BR      250
    */
    QPointF ptSetBottomRightBeyondTopBorderAngle0(300.0, bYAxisTopDown ? 200.0 : 400.0);
    physValAngleCorrected.setVal(270.0);
    double fWidthSetBottomRightBeyondTopBorderAngle0 = 50.0;
    double fHeightSetBottomRightBeyondTopBorderAngle0 = 100.0;
    QPointF ptCenterSetBottomRightBeyondTopBorderAngle0(250.0, bYAxisTopDown ? 225.0 : 375.0);
    QPointF ptTopLeftSetBottomRightBeyondTopBorderAngle0(200.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopRightSetBottomRightBeyondTopBorderAngle0(200.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptBottomRightSetBottomRightBeyondTopBorderAngle0 = ptSetBottomRightBeyondTopBorderAngle0;
    QPointF ptBottomLeftSetBottomRightBeyondTopBorderAngle0(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopCenterSetBottomRightBeyondTopBorderAngle0(200.0, bYAxisTopDown ? 225.0 : 375.0);
    QPointF ptRightCenterSetBottomRightBeyondTopBorderAngle0(250.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptBottomCenterSetBottomRightBeyondTopBorderAngle0(300.0, bYAxisTopDown ? 225.0 : 375.0);
    QPointF ptLeftCenterSetBottomRightBeyondTopBorderAngle0(250.0, bYAxisTopDown ? 250.0 : 350.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(BeyondTopBorder)",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptSetBottomRightBeyondTopBorderAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptSetBottomRightBeyondTopBorderAngle0);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondTopBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondTopBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight(BeyondLeftBorder)
    -----------------------------------
             100   150   200   250   300   350   400
    TopDown                                           BottomUp
     250     BL'---------TL-----------------------TR      350
              |     X     |                       |
     300     BR'---------TR'          X           |       300
                          |                       |
     350                 BL-----------------------BR      250
    */
    QPointF ptSetBottomRightBeyondLeftBorderAngle0(100.0, bYAxisTopDown ? 300.0 : 300.0);
    physValAngleCorrected.setVal(90.0);
    double fWidthSetBottomRightBeyondLeftBorderAngle0 = 50.0;
    double fHeightSetBottomRightBeyondLeftBorderAngle0 = 100.0;
    QPointF ptCenterSetBottomRightBeyondLeftBorderAngle0(150.0, bYAxisTopDown ? 275.0 : 325.0);
    QPointF ptTopLeftSetBottomRightBeyondLeftBorderAngle0(200.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopRightSetBottomRightBeyondLeftBorderAngle0(200.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptBottomRightSetBottomRightBeyondLeftBorderAngle0 = ptSetBottomRightBeyondLeftBorderAngle0;
    QPointF ptBottomLeftSetBottomRightBeyondLeftBorderAngle0(100.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopCenterSetBottomRightBeyondLeftBorderAngle0(200.0, bYAxisTopDown ? 275.0 : 325.0);
    QPointF ptRightCenterSetBottomRightBeyondLeftBorderAngle0(150.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptBottomCenterSetBottomRightBeyondLeftBorderAngle0(100.0, bYAxisTopDown ? 275.0 : 325.0);
    QPointF ptLeftCenterSetBottomRightBeyondLeftBorderAngle0(150.0, bYAxisTopDown ? 250.0 : 350.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(BeyondLeftBorder)",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptSetBottomRightBeyondLeftBorderAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptSetBottomRightBeyondLeftBorderAngle0);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondLeftBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondLeftBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight(BeyondTopLeft)
    ---------------------------------
             100   150   200   250   300   350   400
    TopDown                                           BottomUp
     150     BR'---------BL'                              450
              |           |
     200      |     X'    |                               400
              |           |
     250     TR'---------TL-----------------------TR      350
                          |                       |
     300                  |           X           |       300
                          |                       |
     350                 BL-----------------------BR      250
    */
    QPointF ptSetBottomRightBeyondTopLeftAngle0(100.0, bYAxisTopDown ? 150.0 : 450.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptSetBottomRightBeyondTopLeftAngle0.setY(450.0);
    }
    physValAngleCorrected.setVal(180.0);
    double fWidthSetBottomRightBeyondTopLeftAngle0 = 100.0;
    double fHeightSetBottomRightBeyondTopLeftAngle0 = 100.0;
    QPointF ptCenterSetBottomRightBeyondTopLeftAngle0(150.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptTopLeftSetBottomRightBeyondTopLeftAngle0(200.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopRightSetBottomRightBeyondTopLeftAngle0(100.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomRightSetBottomRightBeyondTopLeftAngle0 = ptSetBottomRightBeyondTopLeftAngle0;
    QPointF ptBottomLeftSetBottomRightBeyondTopLeftAngle0(200.0, bYAxisTopDown ? 150.0 : 450.0);
    QPointF ptTopCenterSetBottomRightBeyondTopLeftAngle0(150.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptRightCenterSetBottomRightBeyondTopLeftAngle0(100.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptBottomCenterSetBottomRightBeyondTopLeftAngle0(150.0, bYAxisTopDown ? 150.0 : 450.0);
    QPointF ptLeftCenterSetBottomRightBeyondTopLeftAngle0(200.0, bYAxisTopDown ? 200.0 : 400.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(BeyondTopLeft)",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptSetBottomRightBeyondTopLeftAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptSetBottomRightBeyondTopLeftAngle0);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondTopLeftAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondTopLeftAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondTopLeftAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft(BeforeTopRight)
    --------------------------------
            200   250   300   350   400
    TopDown                              BottomUp
      250   TL----------TL'---------TR     350
             |           |           |
      300    |           |           |     300
             |           |     X'    |
      350   BL-----------|----------BR     250
                         |           |
      400               BL'---------BR'    200
    */
    QPointF ptSetBottomLeftBeforeTopRightAngle0(300.0, bYAxisTopDown ? 400.0 : 200.0);
    double fWidthSetBottomLeftBeforeTopRightAngle0 = 100.0;
    double fHeightSetBottomLeftBeforeTopRightAngle0 = 150.0;
    QPointF ptCenterSetBottomLeftBeforeTopRightAngle0(350.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptTopLeftSetBottomLeftBeforeTopRightAngle0(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopRightSetBottomLeftBeforeTopRightAngle0(400.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomRightSetBottomLeftBeforeTopRightAngle0(400.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptBottomLeftSetBottomLeftBeforeTopRightAngle0 = ptSetBottomLeftBeforeTopRightAngle0;
    QPointF ptTopCenterSetBottomLeftBeforeTopRightAngle0(350.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptRightCenterSetBottomLeftBeforeTopRightAngle0(400.0, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptBottomCenterSetBottomLeftBeforeTopRightAngle0(350.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptLeftCenterSetBottomLeftBeforeTopRightAngle0(300.0, bYAxisTopDown ? 325.0 : 275.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(BeforeTopRight)",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptSetBottomLeftBeforeTopRightAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptSetBottomLeftBeforeTopRightAngle0);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeforeTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeforeTopRightAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeforeTopRightAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeforeTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeforeTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeforeTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeforeTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeforeTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeforeTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeforeTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeforeTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft(BeyondTopBorder)
    ---------------------------------
            200   250   300   350   400
    TopDown                              BottomUp
      200               BL'---------TL'    400
                         |     X'    |
      250   TL----------BR'---------TR     350
             |                       |
      300    |           X           |     300
             |                       |
      350   BL----------------------BR     250
    */
    QPointF ptSetBottomLeftBeyondTopBorderAngle0(300.0, bYAxisTopDown ? 200.0 : 400.0);
    physValAngleCorrected.setVal(90.0);
    double fWidthSetBottomLeftBeyondTopBorderAngle0 = 50.0;
    double fHeightSetBottomLeftBeyondTopBorderAngle0 = 100.0;
    QPointF ptCenterSetBottomLeftBeyondTopBorderAngle0(350.0, bYAxisTopDown ? 225.0: 375.0);
    QPointF ptTopLeftSetBottomLeftBeyondTopBorderAngle0(400.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptTopRightSetBottomLeftBeyondTopBorderAngle0(400.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomRightSetBottomLeftBeyondTopBorderAngle0(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomLeftSetBottomLeftBeyondTopBorderAngle0 = ptSetBottomLeftBeyondTopBorderAngle0;
    QPointF ptTopCenterSetBottomLeftBeyondTopBorderAngle0(400.0, bYAxisTopDown ? 225.0 : 375.0);
    QPointF ptRightCenterSetBottomLeftBeyondTopBorderAngle0(350.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomCenterSetBottomLeftBeyondTopBorderAngle0(300.0, bYAxisTopDown ? 225.0 : 375.0);
    QPointF ptLeftCenterSetBottomLeftBeyondTopBorderAngle0(350.0, bYAxisTopDown ? 200.0 : 400.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(BeyondTopBorder)",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptSetBottomLeftBeyondTopBorderAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptSetBottomLeftBeyondTopBorderAngle0);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondTopBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondTopBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft(BeyondRightBorder)
    -----------------------------------
            200   250   300   350   400   450   500
    TopDown                                          BottomUp
      250   TL----------------------TR----------BR'    350
             |                       |     X'    |
      300    |           X'         TL'---------BL'    300
             |                       |
      350   BL----------------------BR                 250
    */
    QPointF ptSetBottomLeftBeyondRightBorderAngle0(500.0, bYAxisTopDown ? 300.0 : 300.0);
    physValAngleCorrected.setVal(270);
    double fWidthSetBottomLeftBeyondRightBorderAngle0 = 50.0;
    double fHeightSetBottomLeftBeyondRightBorderAngle0 = 100.0;
    QPointF ptCenterSetBottomLeftBeyondRightBorderAngle0(450.0, bYAxisTopDown ? 275.0 : 325.0);
    QPointF ptTopLeftSetBottomLeftBeyondRightBorderAngle0(400.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptTopRightSetBottomLeftBeyondRightBorderAngle0(400.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomRightSetBottomLeftBeyondRightBorderAngle0(500.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomLeftSetBottomLeftBeyondRightBorderAngle0 = ptSetBottomLeftBeyondRightBorderAngle0;
    QPointF ptTopCenterSetBottomLeftBeyondRightBorderAngle0(400.0, bYAxisTopDown ? 275.0 : 325.0);
    QPointF ptRightCenterSetBottomLeftBeyondRightBorderAngle0(450.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomCenterSetBottomLeftBeyondRightBorderAngle0(500.0, bYAxisTopDown ? 275.0 : 325.0);
    QPointF ptLeftCenterSetBottomLeftBeyondRightBorderAngle0(450.0, bYAxisTopDown ? 300.0 : 300.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(BeyondRightBorder)",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptSetBottomLeftBeyondRightBorderAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptSetBottomLeftBeyondRightBorderAngle0);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondRightBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondRightBorderAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondRightBorderAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft(BeyondTopRight)
    --------------------------------
            200   250   300   350   400   450   500
    TopDown                                         BottomUp
      150                           BR'---------BL'   450
                                     |           |  
      200                            |     X'    |    400
                                     |           |
      250   TL----------------------TR----------TL'   350
             |                       |              
      300    |           X           |                300
             |                       |              
      350   BL----------------------BR                250
    */
    QPointF ptSetBottomLeftBeyondTopRightAngle0(500.0, bYAxisTopDown ? 150.0 : 450.0);
    physValAngleCorrected.setVal(180.0);
    double fWidthSetBottomLeftBeyondTopRightAngle0 = 100.0;
    double fHeightSetBottomLeftBeyondTopRightAngle0 = 100.0;
    QPointF ptCenterSetBottomLeftBeyondTopRightAngle0(450.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptTopLeftSetBottomLeftBeyondTopRightAngle0(500.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopRightSetBottomLeftBeyondTopRightAngle0(400.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomRightSetBottomLeftBeyondTopRightAngle0(400.0, bYAxisTopDown ? 150.0 : 450.0);
    QPointF ptBottomLeftSetBottomLeftBeyondTopRightAngle0 = ptSetBottomLeftBeyondTopRightAngle0;
    QPointF ptTopCenterSetBottomLeftBeyondTopRightAngle0(450.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptRightCenterSetBottomLeftBeyondTopRightAngle0(400.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptBottomCenterSetBottomLeftBeyondTopRightAngle0(450.0, bYAxisTopDown ? 150.0 : 450.0);
    QPointF ptLeftCenterSetBottomLeftBeyondTopRightAngle0(500.0, bYAxisTopDown ? 200.0 : 400.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(BeyondTopRight)",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptSetBottomLeftBeyondTopRightAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptSetBottomLeftBeyondTopRightAngle0);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondTopRightAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondTopRightAngle0, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondTopRightAngle0, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    //--------------------------------------------------------------------------
    // Rotation Angle: 90.0�
    //--------------------------------------------------------------------------

    physValAngle.setVal(90.0);
    physValAngleCorrected.setVal(0.0);

    ZS::Test::CTestStepGroup* pGrpTransformRectAngle_90_Degree = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Angle(" + physValAngle.toString() + ")",
        /* pTSGrpParent */ pGrpTransformRect );

    /* setAngle(90�)
    ----------------
            200   250   300   350   400
    TopDown                              BottomUp
      200         BL'----+----TL'          400
                   |           |
      250          |           |           350
                   |           |
      300          +     X     +           300
                   |           |
      350          |           |           250
                   |           |
      400         BR'----+----TR'          200
    */
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setAngle(" + physValAngle.toString() + ")",
        /* strOperation    */ "setAngle(" + physValAngle.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_90_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("addPhysValShape", "");
    // Center, Width and Height must remain the same for all following setAngle test steps
    double fWidthSetAngle90 = 200.0;
    double fHeightSetAngle90 = 100.0;
    QPointF ptCenterSetAngle90(300.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptTopLeftSetAngle90(350.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptTopRightSetAngle90(350.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptBottomRightSetAngle90(250.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptBottomLeftSetAngle90(250.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptTopCenterSetAngle90(350.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptRightCenterSetAngle90(300.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptBottomCenterSetAngle90(250.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptLeftCenterSetAngle90(300.0, bYAxisTopDown ? 200.0 : 400.0);
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetAngle90, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetAngle90, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetAngle90, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetAngle90, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetAngle90, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetAngle90, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetAngle90, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetAngle90, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetAngle90, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetAngle90, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetAngle90, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    //--------------------------------------------------------------------------
    // Rotation Angle: 180.0�
    //--------------------------------------------------------------------------

    physValAngle.setVal(180.0);
    physValAngleCorrected.setVal(0.0);

    ZS::Test::CTestStepGroup* pGrpTransformRectAngle_180_Degree = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Angle(" + physValAngle.toString() + ")",
        /* pTSGrpParent */ pGrpTransformRect );

    /* setAngle(180�)
    ----------------
            200   250   300   350   400
    TopDown                              BottomUp
      250   BR'----------+----------BL'    350
             |                       |
      300    +           X           +     300
             |                       |
      350   TR'----------+----------TL'    250
    */
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setAngle(" + physValAngle.toString() + ")",
        /* strOperation    */ "setAngle(" + physValAngle.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("addPhysValShape", "");
    // Center, Width and Height must remain the same for all following setAngle test steps
    double fWidthSetAngle180 = 200.0;
    double fHeightSetAngle180 = 100.0;
    QPointF ptCenterSetAngle180(300.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptTopLeftSetAngle180(400.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptTopRightSetAngle180(200.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptBottomRightSetAngle180(200.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptBottomLeftSetAngle180(400.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptTopCenterSetAngle180(300.0, bYAxisTopDown ? 350.0 : 250.0);
    QPointF ptRightCenterSetAngle180(200.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptBottomCenterSetAngle180(300.0, bYAxisTopDown ? 250.0 : 350.0);
    QPointF ptLeftCenterSetAngle180(400.0, bYAxisTopDown ? 300.0 : 300.0);
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetAngle180, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetAngle180, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetAngle180, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetAngle180, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetAngle180, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetAngle180, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetAngle180, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetAngle180, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetAngle180, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetAngle180, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetAngle180, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    //--------------------------------------------------------------------------
    // Rotation Angle: 270.0�
    //--------------------------------------------------------------------------

    physValAngle.setVal(270.0);
    physValAngleCorrected.setVal(0.0);

    ZS::Test::CTestStepGroup* pGrpTransformRectAngle_270_Degree = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Angle(" + physValAngle.toString() + ")",
        /* pTSGrpParent */ pGrpTransformRect );

    /* setAngle(270�)
    -----------------
            200   250   300   350   400
    TopDown                              BottomUp
      200         TR'----+----BR'          400
                   |           |
      250          |           |           350
                   |           |
      300          +     X     +           300
                   |           |
      350          |           |           250
                   |           |
      400         TL'----+----BL'          200
    */
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setAngle(" + physValAngle.toString() + ")",
        /* strOperation    */ "setAngle(" + physValAngle.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_270_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("addPhysValShape", "");
    // Center, Width and Height must remain the same for all following setAngle test steps
    double fWidthSetAngle270 = 200.0;
    double fHeightSetAngle270 = 100.0;
    QPointF ptCenterSetAngle270(300.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptTopLeftSetAngle270(250.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptTopRightSetAngle270(250.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptBottomRightSetAngle270(350.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptBottomLeftSetAngle270(350.0, bYAxisTopDown ? 400.0 : 200.0);
    QPointF ptTopCenterSetAngle270(250.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptRightCenterSetAngle270(300.0, bYAxisTopDown ? 200.0 : 400.0);
    QPointF ptBottomCenterSetAngle270(350.0, bYAxisTopDown ? 300.0 : 300.0);
    QPointF ptLeftCenterSetAngle270(300.0, bYAxisTopDown ? 400.0 : 200.0);
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetAngle270, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetAngle270, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetAngle270, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetAngle270, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetAngle270, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetAngle270, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetAngle270, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetAngle270, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetAngle270, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetAngle270, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetAngle270, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    //--------------------------------------------------------------------------
    // Rotation Angle: 30.0�
    //--------------------------------------------------------------------------

    physValAngle.setVal(30.0);
    physValAngleCorrected.setVal(210.0);

    ZS::Test::CTestStepGroup* pGrpTransformRectAngle_30_Degree = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Angle(" + physValAngle.toString() + ")",
        /* pTSGrpParent */ pGrpTransformRect );

    /* setAngle(30.0�)
    ------------------
    */
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setAngle(" + physValAngle.toString() + ")",
        /* strOperation    */ "setAngle(" + physValAngle.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("addPhysValShape", "");
    double fWidthSetAngle30 = fWidthSetAngle0;
    double fHeightSetAngle30 = fHeightSetAngle0;
    QPointF ptCenterSetAngle30 = ptCenterSetAngle0;
    QPointF ptTopLeftSetAngle30(238.4, bYAxisTopDown ? 206.7 : 393.3);
    QPointF ptTopRightSetAngle30(411.6, bYAxisTopDown ? 306.7 : 293.3);
    QPointF ptBottomRightSetAngle30(361.6, bYAxisTopDown ? 393.3 : 206.7);
    QPointF ptBottomLeftSetAngle30(188.4, bYAxisTopDown ? 293.3 : 306.7);
    QPointF ptTopCenterSetAngle30 = QLineF(ptTopRightSetAngle30, ptTopLeftSetAngle30).center();
    QPointF ptRightCenterSetAngle30 = QLineF(ptTopRightSetAngle30, ptBottomRightSetAngle30).center();
    QPointF ptBottomCenterSetAngle30 = QLineF(ptBottomRightSetAngle30, ptBottomLeftSetAngle30).center();
    QPointF ptLeftCenterSetAngle30 = QLineF(ptTopLeftSetAngle30, ptBottomLeftSetAngle30).center();
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setCenter(400, 400)
    ----------------------
    */
    ptCenter = QPointF(400.0, bYAxisTopDown ? 400.0 : 200.0);
    double fdx = ptCenter.x() - ptCenterSetAngle30.x();
    double fdy = ptCenter.y() - ptCenterSetAngle30.y();
    double fWidthSetCenterAngle30 = fWidthSetAngle30;
    double fHeightSetCenterAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetCenterAngle30 = ptCenter;
    QPointF ptTopLeftSetCenterAngle30(ptTopLeftSetAngle30.x() + fdx, ptTopLeftSetAngle30.y() + fdy);
    QPointF ptTopRightSetCenterAngle30(ptTopRightSetAngle30.x() + fdx, ptTopRightSetAngle30.y() + fdy);
    QPointF ptBottomRightSetCenterAngle30(ptBottomRightSetAngle30.x() + fdx, ptBottomRightSetAngle30.y() + fdy);
    QPointF ptBottomLeftSetCenterAngle30(ptBottomLeftSetAngle30.x() + fdx, ptBottomLeftSetAngle30.y() + fdy);
    QPointF ptTopCenterSetCenterAngle30 = QLineF(ptTopRightSetCenterAngle30, ptTopLeftSetCenterAngle30).center();
    QPointF ptRightCenterSetCenterAngle30 = QLineF(ptTopRightSetCenterAngle30, ptBottomRightSetCenterAngle30).center();
    QPointF ptBottomCenterSetCenterAngle30 = QLineF(ptBottomRightSetCenterAngle30, ptBottomLeftSetCenterAngle30).center();
    QPointF ptLeftCenterSetCenterAngle30 = QLineF(ptTopLeftSetCenterAngle30, ptBottomLeftSetCenterAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setCenter(" + qPoint2Str(ptCenter) + " " + unit.symbol() + ")",
        /* strOperation    */ "setCenter(" + qPoint2Str(ptCenter) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setCenter", ptCenter);
    pTestStep->setConfigValue("setCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetCenterAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetCenterAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetCenterAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetCenterAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetCenterAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetCenterAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetCenterAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetCenterAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetCenterAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetCenterAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetCenterAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setSize(300, 200)
    --------------------
    */
    double fWidthSetSizeAngle30 = size300x200.width();
    double fHeightSetSizeAngle30 = size300x200.height();
    QPointF ptCenterSetSizeAngle30(318.3, bYAxisTopDown ? 368.3 : 231.7);
    QPointF ptTopLeftSetSizeAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetSizeAngle30(498.2, bYAxisTopDown ? 356.7 : 243.3);
    QPointF ptBottomRightSetSizeAngle30(398.2, bYAxisTopDown ? 529.9 : 70.1);
    QPointF ptBottomLeftSetSizeAngle30(138.4, bYAxisTopDown ? 379.9: 220.1);
    QPointF ptTopCenterSetSizeAngle30 = QLineF(ptTopRightSetSizeAngle30, ptTopLeftSetSizeAngle30).center();
    QPointF ptRightCenterSetSizeAngle30 = QLineF(ptTopRightSetSizeAngle30, ptBottomRightSetSizeAngle30).center();
    QPointF ptBottomCenterSetSizeAngle30 = QLineF(ptBottomRightSetSizeAngle30, ptBottomLeftSetSizeAngle30).center();
    QPointF ptLeftCenterSetSizeAngle30 = QLineF(ptTopLeftSetSizeAngle30, ptBottomLeftSetSizeAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setSize(" + qSize2Str(size300x200) + " " + unit.symbol() + ")",
        /* strOperation    */ "setSize(" + qSize2Str(size300x200) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setSize", size300x200);
    pTestStep->setConfigValue("setSize.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetSizeAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetSizeAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetSizeAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetSizeAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetSizeAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetSizeAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetSizeAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetSizeAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetSizeAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetSizeAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetSizeAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidth(300)
    ----------------
    */
    double fWidthSetWidthAngle30 = physValWidth300.getVal();
    double fHeightSetWidthAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetWidthAngle30(343.3, bYAxisTopDown ? 325.0 : 275.0);
    QPointF ptTopLeftSetWidthAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetWidthAngle30(498.2, bYAxisTopDown ? 356.7 : 243.3);
    QPointF ptBottomRightSetWidthAngle30(448.2, bYAxisTopDown ? 443.3 : 156.7);
    QPointF ptBottomLeftSetWidthAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetWidthAngle30 = QLineF(ptTopRightSetWidthAngle30, ptTopLeftSetWidthAngle30).center();
    QPointF ptRightCenterSetWidthAngle30 = QLineF(ptTopRightSetWidthAngle30, ptBottomRightSetWidthAngle30).center();
    QPointF ptBottomCenterSetWidthAngle30 = QLineF(ptBottomRightSetWidthAngle30, ptBottomLeftSetWidthAngle30).center();
    QPointF ptLeftCenterSetWidthAngle30 = ptLeftCenterSetAngle30;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidth(" + physValWidth300.toString() + ")",
        /* strOperation    */ "setWidth(" + physValWidth300.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidth", physValWidth300.toString());
    pTestStep->setConfigValue("setWidth.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingLeftCenter(BeforeRightBorder)
    -------------------------------------------------
    */
    // Set width to half of original width. Thats been done by moving to the top center point of the original rectangle.
    // The resulting center point, when resizing the rectangle by moving the left edge to a new width of 100.0, is the
    // same as setting the width to 300.0 by moving the right edge of the rectangle. But only in this special case.
    ptMoved = ptTopCenterSetAngle30;
    double fWidthSetWidthMoveLeftCenterBeforeRightBorderAngle30 = fWidthSetAngle30 / 2.0;
    double fHeightSetWidthMoveLeftCenterBeforeRightBorderAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetWidthMoveLeftCenterBeforeRightBorderAngle30 = ptCenterSetWidthAngle30;
    QPointF ptTopLeftSetWidthMoveLeftCenterBeforeRightBorderAngle30 = ptTopCenterSetAngle30;
    QPointF ptTopRightSetWidthMoveLeftCenterBeforeRightBorderAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetWidthMoveLeftCenterBeforeRightBorderAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetWidthMoveLeftCenterBeforeRightBorderAngle30 = ptBottomCenterSetAngle30;
    QPointF ptTopCenterSetWidthMoveLeftCenterBeforeRightBorderAngle30 =
        QLineF(ptTopRightSetWidthMoveLeftCenterBeforeRightBorderAngle30, ptTopLeftSetWidthMoveLeftCenterBeforeRightBorderAngle30).center();
    QPointF ptRightCenterSetWidthMoveLeftCenterBeforeRightBorderAngle30 = ptRightCenterSetAngle30;
    QPointF ptBottomCenterSetWidthMoveLeftCenterBeforeRightBorderAngle30 =
        QLineF(ptBottomRightSetWidthMoveLeftCenterBeforeRightBorderAngle30, ptBottomLeftSetWidthMoveLeftCenterBeforeRightBorderAngle30).center();
    QPointF ptLeftCenterSetWidthMoveLeftCenterBeforeRightBorderAngle30 =
        QLineF(ptTopLeftSetWidthMoveLeftCenterBeforeRightBorderAngle30, ptBottomLeftSetWidthMoveLeftCenterBeforeRightBorderAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(BeforeRightBorder)",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterBeforeRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterBeforeRightBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterBeforeRightBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterBeforeRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterBeforeRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterBeforeRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterBeforeRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterBeforeRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterBeforeRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterBeforeRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterBeforeRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingLeftCenter(BeyondRightBorder)
    ------------------------------------------------
    */
    // Set width to half of original width. Thats been done by moving to the top center point of the original rectangle.
    // The resulting center point, when resizing the rectangle by moving the left edge to a new width of 100.0, is the
    // same as setting the width to 300.0 by moving the right edge of the rectangle. But only in this special case.
    ptMoved = QPointF(ptTopRightSetAngle30.x() + (ptTopRightSetAngle30.x() - ptTopLeftSetAngle30.x()) / 2.0,
                      ptTopRightSetAngle30.y() + (ptTopRightSetAngle30.y() - ptTopLeftSetAngle30.y()) / 2.0);
    physValAngleCorrected.setVal(210.0);
    double fWidthSetWidthMoveLeftCenterBeyondRightBorderAngle30 = fWidthSetAngle30 / 2.0;
    double fHeightSetWidthMoveLeftCenterBeyondRightBorderAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetWidthMoveLeftCenterBeyondRightBorderAngle30(429.9, bYAxisTopDown ? 375.0 : 225.0);
    QPointF ptTopLeftSetWidthMoveLeftCenterBeyondRightBorderAngle30(448.2, bYAxisTopDown ? 443.3 : 156.7);
    QPointF ptTopRightSetWidthMoveLeftCenterBeyondRightBorderAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomRightSetWidthMoveLeftCenterBeyondRightBorderAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomLeftSetWidthMoveLeftCenterBeyondRightBorderAngle30(498.2, bYAxisTopDown ? 356.7 : 243.3);
    QPointF ptTopCenterSetWidthMoveLeftCenterBeyondRightBorderAngle30 =
        QLineF(ptTopRightSetWidthMoveLeftCenterBeyondRightBorderAngle30, ptTopLeftSetWidthMoveLeftCenterBeyondRightBorderAngle30).center();
    QPointF ptRightCenterSetWidthMoveLeftCenterBeyondRightBorderAngle30 = ptRightCenterSetAngle30;
    QPointF ptBottomCenterSetWidthMoveLeftCenterBeyondRightBorderAngle30 =
        QLineF(ptBottomRightSetWidthMoveLeftCenterBeyondRightBorderAngle30, ptBottomLeftSetWidthMoveLeftCenterBeyondRightBorderAngle30).center();
    QPointF ptLeftCenterSetWidthMoveLeftCenterBeyondRightBorderAngle30 =
        QLineF(ptTopLeftSetWidthMoveLeftCenterBeyondRightBorderAngle30, ptBottomLeftSetWidthMoveLeftCenterBeyondRightBorderAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(BeyondRightBorder)",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterBeyondRightBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterBeyondRightBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingRightCenter(BeforeLeftBorder)
    ------------------------------------------------
    */
    // Same width chosen by moving right center as with setWidth call above.
    ptMoved = ptTopCenterSetAngle30;
    double fWidthSetWidthMoveRightCenterBeforeLeftBorderAngle30 = fWidthSetAngle30 / 2.0;
    double fHeightSetWidthMoveRightCenterBeforeLeftBorderAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetWidthMoveRightCenterBeforeLeftBorderAngle30(256.7, bYAxisTopDown ? 275.0 : 325.0);
    QPointF ptTopLeftSetWidthMoveRightCenterBeforeLeftBorderAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetWidthMoveRightCenterBeforeLeftBorderAngle30 = ptTopCenterSetAngle30;
    QPointF ptBottomRightSetWidthMoveRightCenterBeforeLeftBorderAngle30 = ptBottomCenterSetAngle30;
    QPointF ptBottomLeftSetWidthMoveRightCenterBeforeLeftBorderAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetWidthMoveRightCenterBeforeLeftBorderAngle30 =
        QLineF(ptTopRightSetWidthMoveRightCenterBeforeLeftBorderAngle30, ptTopLeftSetWidthMoveRightCenterBeforeLeftBorderAngle30).center();
    QPointF ptRightCenterSetWidthMoveRightCenterBeforeLeftBorderAngle30 =
        QLineF(ptTopRightSetWidthMoveRightCenterBeforeLeftBorderAngle30, ptBottomRightSetWidthMoveRightCenterBeforeLeftBorderAngle30).center();
    QPointF ptBottomCenterSetWidthMoveRightCenterBeforeLeftBorderAngle30 =
        QLineF(ptBottomRightSetWidthMoveRightCenterBeforeLeftBorderAngle30, ptBottomLeftSetWidthMoveRightCenterBeforeLeftBorderAngle30).center();
    QPointF ptLeftCenterSetWidthMoveRightCenterBeforeLeftBorderAngle30 = ptLeftCenterSetAngle30;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(BeforeLeftBorder)",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterBeforeLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterBeforeLeftBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterBeforeLeftBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterBeforeLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterBeforeLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterBeforeLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterBeforeLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterBeforeLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterBeforeLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterBeforeLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterBeforeLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingRightCenter(BeyondLeftBorder)
    ------------------------------------------------
    */
    ptMoved = QPointF(ptTopLeftSetAngle30.x() - (ptTopRightSetAngle30.x() - ptTopLeftSetAngle30.x()) / 2.0,
                      ptTopLeftSetAngle30.y() - (ptTopRightSetAngle30.y() - ptTopLeftSetAngle30.y()) / 2.0);
    physValAngleCorrected.setVal(210.0);
    double fWidthSetWidthMoveRightCenterBeyondLeftBorderAngle30 = fWidthSetAngle30 / 2.0;
    double fHeightSetWidthMoveRightCenterBeyondLeftBorderAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetWidthMoveRightCenterBeyondLeftBorderAngle30(170.1, bYAxisTopDown ? 225.0 : 375.0);
    QPointF ptTopLeftSetWidthMoveRightCenterBeyondLeftBorderAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopRightSetWidthMoveRightCenterBeyondLeftBorderAngle30(101.8, bYAxisTopDown ? 243.3 : 356.7);
    QPointF ptBottomRightSetWidthMoveRightCenterBeyondLeftBorderAngle30 = ptMoved;
    QPointF ptBottomLeftSetWidthMoveRightCenterBeyondLeftBorderAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopCenterSetWidthMoveRightCenterBeyondLeftBorderAngle30 =
        QLineF(ptTopRightSetWidthMoveRightCenterBeyondLeftBorderAngle30, ptTopLeftSetWidthMoveRightCenterBeyondLeftBorderAngle30).center();
    QPointF ptRightCenterSetWidthMoveRightCenterBeyondLeftBorderAngle30 =
        QLineF(ptTopRightSetWidthMoveRightCenterBeyondLeftBorderAngle30, ptBottomRightSetWidthMoveRightCenterBeyondLeftBorderAngle30).center();
    QPointF ptBottomCenterSetWidthMoveRightCenterBeyondLeftBorderAngle30 =
        QLineF(ptBottomRightSetWidthMoveRightCenterBeyondLeftBorderAngle30, ptBottomLeftSetWidthMoveRightCenterBeyondLeftBorderAngle30).center();
    QPointF ptLeftCenterSetWidthMoveRightCenterBeyondLeftBorderAngle30 = ptLeftCenterSetAngle30;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(BeyondLeftBorder)",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterBeyondLeftBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterBeyondLeftBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeight(200)
    -----------------
    */
    double fWidthSetHeightAngle30 = fWidthSetAngle30;
    double fHeightSetHeightAngle30 = physValHeight200.getVal();
    QPointF ptCenterSetHeightAngle30(275.0, bYAxisTopDown ? 343.3 : 256.7);
    QPointF ptTopLeftSetHeightAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetHeightAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetHeightAngle30(311.6, bYAxisTopDown ? 479.9 : 120.1);
    QPointF ptBottomLeftSetHeightAngle30(138.4, bYAxisTopDown ? 379.9 : 220.1);
    QPointF ptTopCenterSetHeightAngle30 = ptTopCenterSetAngle30;
    QPointF ptRightCenterSetHeightAngle30 = QLineF(ptTopRightSetHeightAngle30, ptBottomRightSetHeightAngle30).center();
    QPointF ptBottomCenterSetHeightAngle30 = QLineF(ptBottomRightSetHeightAngle30, ptBottomLeftSetHeightAngle30).center();
    QPointF ptLeftCenterSetHeightAngle30 = QLineF(ptTopLeftSetHeightAngle30, ptBottomLeftSetHeightAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeight(" + physValHeight200.toString() + ")",
        /* strOperation    */ "setHeight(" + physValHeight200.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeight", physValHeight200.toString());
    pTestStep->setConfigValue("setHeight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingTopCenter(BeforeBottomBorder)
    -------------------------------------------------
    */
    ptMoved = ptRightCenterSetAngle30;
    double fWidthSetHeightMoveTopCenterBeforeBottomBorderAngle30 = fWidthSetAngle30;
    double fHeightSetHeightMoveTopCenterBeforeBottomBorderAngle30 = fHeightSetAngle30 / 2.0;
    QPointF ptCenterSetHeightMoveTopCenterBeforeBottomBorderAngle30(287.49, bYAxisTopDown ? 321.7 : 278.3);
    QPointF ptTopLeftSetHeightMoveTopCenterBeforeBottomBorderAngle30 = ptLeftCenterSetAngle30;
    QPointF ptTopRightSetHeightMoveTopCenterBeforeBottomBorderAngle30 = ptRightCenterSetAngle30;
    QPointF ptBottomRightSetHeightMoveTopCenterBeforeBottomBorderAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetHeightMoveTopCenterBeforeBottomBorderAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetHeightMoveTopCenterBeforeBottomBorderAngle30 =
        QLineF(ptTopLeftSetHeightMoveTopCenterBeforeBottomBorderAngle30, ptTopRightSetHeightMoveTopCenterBeforeBottomBorderAngle30).center();
    QPointF ptRightCenterSetHeightMoveTopCenterBeforeBottomBorderAngle30 =
        QLineF(ptTopRightSetHeightMoveTopCenterBeforeBottomBorderAngle30, ptBottomRightSetHeightMoveTopCenterBeforeBottomBorderAngle30).center();
    QPointF ptBottomCenterSetHeightMoveTopCenterBeforeBottomBorderAngle30 = ptBottomCenterSetAngle30;
    QPointF ptLeftCenterSetHeightMoveTopCenterBeforeBottomBorderAngle30 =
        QLineF(ptTopLeftSetHeightMoveTopCenterBeforeBottomBorderAngle30, ptBottomLeftSetHeightMoveTopCenterBeforeBottomBorderAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(BeforeBottomBorder)",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterBeforeBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterBeforeBottomBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterBeforeBottomBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterBeforeBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterBeforeBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterBeforeBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterBeforeBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterBeforeBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterBeforeBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterBeforeBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterBeforeBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingTopCenter(BeyondBottomBorder)
    -------------------------------------------------
    */
    ptMoved = QPointF(ptBottomRightSetAngle30.x() - (ptTopRightSetAngle30.x() - ptBottomRightSetAngle30.x()) / 2.0,
                      ptBottomRightSetAngle30.y() + (ptBottomRightSetAngle30.y() - ptTopRightSetAngle30.y()) / 2.0);
    physValAngleCorrected.setVal(210.0);
    double fWidthSetHeightMoveTopCenterBeyondBottomBorderAngle30 = fWidthSetAngle30;
    double fHeightSetHeightMoveTopCenterBeyondBottomBorderAngle30 = fHeightSetAngle30 / 2.0;
    QPointF ptCenterSetHeightMoveTopCenterBeyondBottomBorderAngle30(262.5, bYAxisTopDown ? 365.0 : 235.0);
    QPointF ptTopLeftSetHeightMoveTopCenterBeyondBottomBorderAngle30(336.6, bYAxisTopDown ? 436.6 : 163.4);
    QPointF ptTopRightSetHeightMoveTopCenterBeyondBottomBorderAngle30(163.4, bYAxisTopDown ? 336.6 : 263.4);
    QPointF ptBottomRightSetHeightMoveTopCenterBeyondBottomBorderAngle30 = ptBottomLeftSetAngle30;
    QPointF ptBottomLeftSetHeightMoveTopCenterBeyondBottomBorderAngle30 = ptBottomRightSetAngle30;
    QPointF ptTopCenterSetHeightMoveTopCenterBeyondBottomBorderAngle30 =
        QLineF(ptTopLeftSetHeightMoveTopCenterBeyondBottomBorderAngle30, ptTopRightSetHeightMoveTopCenterBeyondBottomBorderAngle30).center();
    QPointF ptRightCenterSetHeightMoveTopCenterBeyondBottomBorderAngle30 =
        QLineF(ptTopRightSetHeightMoveTopCenterBeyondBottomBorderAngle30, ptBottomRightSetHeightMoveTopCenterBeyondBottomBorderAngle30).center();
    //QPointF ptRightCenterSetHeightMoveTopCenterBeyondBottomBorderAngle30 = QPointF(175.9, bYAxisTopDown ? 315.0 : 285.0);
    QPointF ptBottomCenterSetHeightMoveTopCenterBeyondBottomBorderAngle30 = ptBottomCenterSetAngle30;
    QPointF ptLeftCenterSetHeightMoveTopCenterBeyondBottomBorderAngle30 =
        QLineF(ptTopLeftSetHeightMoveTopCenterBeyondBottomBorderAngle30, ptBottomLeftSetHeightMoveTopCenterBeyondBottomBorderAngle30).center();
    //QPointF ptLeftCenterSetHeightMoveTopCenterBeyondBottomBorderAngle30 = QPointF(349.1, bYAxisTopDown ? 415.0 : 185.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(BeyondBottomBorder)",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterBeyondBottomBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterBeyondBottomBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingBottomCenter(BeforeTopBorder)
    -------------------------------------------------
    */
    ptMoved = ptRightCenterSetAngle30;
    double fWidthSetHeightMoveBottomCenterBeforeTopBorderAngle30 = fWidthSetHeightAngle30;
    double fHeightSetHeightMoveBottomCenterBeforeTopBorderAngle30 = fHeightSetAngle30 / 2.0;
    QPointF ptCenterSetHeightMoveBottomCenterBeforeTopBorderAngle30(312.49, bYAxisTopDown ? 278.4 : 321.7);
    QPointF ptTopLeftSetHeightMoveBottomCenterBeforeTopBorderAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetHeightMoveBottomCenterBeforeTopBorderAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetHeightMoveBottomCenterBeforeTopBorderAngle30 = ptRightCenterSetAngle30;
    QPointF ptBottomLeftSetHeightMoveBottomCenterBeforeTopBorderAngle30 = ptLeftCenterSetAngle30;
    QPointF ptTopCenterSetHeightMoveBottomCenterBeforeTopBorderAngle30 = ptTopCenterSetAngle30;
    QPointF ptRightCenterSetHeightMoveBottomCenterBeforeTopBorderAngle30 =
        QLineF(ptTopRightSetHeightMoveBottomCenterBeforeTopBorderAngle30, ptBottomRightSetHeightMoveBottomCenterBeforeTopBorderAngle30).center();
    QPointF ptBottomCenterSetHeightMoveBottomCenterBeforeTopBorderAngle30 =
        QLineF(ptBottomLeftSetHeightMoveBottomCenterBeforeTopBorderAngle30, ptBottomRightSetHeightMoveBottomCenterBeforeTopBorderAngle30).center();
    QPointF ptLeftCenterSetHeightMoveBottomCenterBeforeTopBorderAngle30 =
        QLineF(ptTopLeftSetHeightMoveBottomCenterBeforeTopBorderAngle30, ptBottomLeftSetHeightMoveBottomCenterBeforeTopBorderAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(BeforeTopBorder)",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenterBeforeTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenterBeforeTopBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenterBeforeTopBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenterBeforeTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenterBeforeTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenterBeforeTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenterBeforeTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenterBeforeTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenterBeforeTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenterBeforeTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenterBeforeTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingBottomCenter(BeyondTopBorder)
    -------------------------------------------------
    */
    ptMoved = QPointF(ptTopRightSetAngle30.x() + (ptTopRightSetAngle30.x() - ptBottomRightSetAngle30.x()) / 2.0,
                      ptTopRightSetAngle30.y() - (ptBottomRightSetAngle30.y() - ptTopRightSetAngle30.y()) / 2.0);
    physValAngleCorrected.setVal(210.0);
    double fWidthSetHeightMoveBottomCenterBeyondTopBorderAngle30 = fWidthSetHeightAngle30;
    double fHeightSetHeightMoveBottomCenterBeyondTopBorderAngle30 = fHeightSetAngle30 / 2.0;
    QPointF ptCenterSetHeightMoveBottomCenterBeyondTopBorderAngle30(337.499, bYAxisTopDown ? 235.1 : 365.0);
    QPointF ptTopLeftSetHeightMoveBottomCenterBeyondTopBorderAngle30 = ptTopRightSetAngle30;
    QPointF ptTopRightSetHeightMoveBottomCenterBeyondTopBorderAngle30 = ptTopLeftSetAngle30;
    QPointF ptBottomRightSetHeightMoveBottomCenterBeyondTopBorderAngle30(263.4, bYAxisTopDown ? 163.4 : 436.6);
    QPointF ptBottomLeftSetHeightMoveBottomCenterBeyondTopBorderAngle30(436.6, bYAxisTopDown ? 263.4 : 336.6);
    QPointF ptTopCenterSetHeightMoveBottomCenterBeyondTopBorderAngle30 = ptTopCenterSetAngle30;
    QPointF ptRightCenterSetHeightMoveBottomCenterBeyondTopBorderAngle30 =
        QLineF(ptTopRightSetHeightMoveBottomCenterBeyondTopBorderAngle30, ptBottomRightSetHeightMoveBottomCenterBeyondTopBorderAngle30).center();
    QPointF ptBottomCenterSetHeightMoveBottomCenterBeyondTopBorderAngle30 =
        QLineF(ptBottomLeftSetHeightMoveBottomCenterBeyondTopBorderAngle30, ptBottomRightSetHeightMoveBottomCenterBeyondTopBorderAngle30).center();
    QPointF ptLeftCenterSetHeightMoveBottomCenterBeyondTopBorderAngle30 =
        QLineF(ptTopLeftSetHeightMoveBottomCenterBeyondTopBorderAngle30, ptBottomLeftSetHeightMoveBottomCenterBeyondTopBorderAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(BeyondTopBorder)",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenterBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenterBeyondTopBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenterBeyondTopBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenterBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenterBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenterBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenterBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenterBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenterBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenterBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenterBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(BeforeBottomRight)
    --------------------------------
    */
    QPointF ptSetTopLeftBeforeBottomRightAngle30 = QPointF(350.0, bYAxisTopDown ? 210.0 : 390.0);
    double fWidthSetTopLeftBeforeBottomRightAngle30 = 101.7;
    double fHeightSetTopLeftBeforeBottomRightAngle30 = 152.9;
    QPointF ptCenterSetTopLeftBeforeBottomRightAngle30(355.8, 301.7);
    QPointF ptTopLeftSetTopLeftBeforeBottomRightAngle30 = ptSetTopLeftBeforeBottomRightAngle30;
    QPointF ptTopRightSetTopLeftBeforeBottomRightAngle30(438.1, 260.9);
    QPointF ptBottomRightSetTopLeftBeforeBottomRightAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetTopLeftBeforeBottomRightAngle30(273.5, 342.499);
    QPointF ptTopCenterSetTopLeftBeforeBottomRightAngle30 =
        QLineF(ptTopLeftSetTopLeftBeforeBottomRightAngle30, ptTopRightSetTopLeftBeforeBottomRightAngle30).center();
    QPointF ptRightCenterSetTopLeftBeforeBottomRightAngle30 =
        QLineF(ptTopRightSetTopLeftBeforeBottomRightAngle30, ptBottomRightSetTopLeftBeforeBottomRightAngle30).center();
    QPointF ptBottomCenterSetTopLeftBeforeBottomRightAngle30 =
        QLineF(ptBottomLeftSetTopLeftBeforeBottomRightAngle30, ptBottomRightSetTopLeftBeforeBottomRightAngle30).center();
    QPointF ptLeftCenterSetTopLeftBeforeBottomRightAngle30 =
        QLineF(ptTopLeftSetTopLeftBeforeBottomRightAngle30, ptBottomLeftSetTopLeftBeforeBottomRightAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(BeforeBottomRight)",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptSetTopLeftBeforeBottomRightAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptSetTopLeftBeforeBottomRightAngle30);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeforeBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeforeBottomRightAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeforeBottomRightAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeforeBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeforeBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeforeBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeforeBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeforeBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeforeBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeforeBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeforeBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(BeyondRightBorder)
    --------------------------------*/
    QPointF ptSetTopLeftBeyondRightBorderAngle30(470, bYAxisTopDown ? 400.0 : 200.0);
    physValAngleCorrected.setVal(120.0);
    double fWidthSetTopLeftBeyondRightBorderAngle30 = 48.4;
    double fHeightSetTopLeftBeyondRightBorderAngle30 = 97.2;
    QPointF ptCenterSetTopLeftBeyondRightBorderAngle30(415.8, 396.7);
    QPointF ptTopLeftSetTopLeftBeyondRightBorderAngle30 = ptSetTopLeftBeyondRightBorderAngle30;
    QPointF ptTopRightSetTopLeftBeyondRightBorderAngle30(445.8, 441.9);
    QPointF ptBottomRightSetTopLeftBeyondRightBorderAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetTopLeftBeyondRightBorderAngle30(385.8, 351.4);
    QPointF ptTopCenterSetTopLeftBeyondRightBorderAngle30 =
        QLineF(ptTopLeftSetTopLeftBeyondRightBorderAngle30, ptTopRightSetTopLeftBeyondRightBorderAngle30).center();
    QPointF ptRightCenterSetTopLeftBeyondRightBorderAngle30 =
        QLineF(ptTopRightSetTopLeftBeyondRightBorderAngle30, ptBottomRightSetTopLeftBeyondRightBorderAngle30).center();
    QPointF ptBottomCenterSetTopLeftBeyondRightBorderAngle30 =
        QLineF(ptBottomLeftSetTopLeftBeyondRightBorderAngle30, ptBottomRightSetTopLeftBeyondRightBorderAngle30).center();
    QPointF ptLeftCenterSetTopLeftBeyondRightBorderAngle30 =
        QLineF(ptTopLeftSetTopLeftBeyondRightBorderAngle30, ptBottomLeftSetTopLeftBeyondRightBorderAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(BeyondRightBorder)",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptSetTopLeftBeyondRightBorderAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptSetTopLeftBeyondRightBorderAngle30);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondRightBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondRightBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(BeyondBottomBorder)
    ---------------------------------*/
    QPointF ptSetTopLeftBeyondBottomBorderAngle30(250.0, bYAxisTopDown ? 380.0 : 220.0);
    physValAngleCorrected.setVal(300.0);
    double fWidthSetTopLeftBeyondBottomBorderAngle30 = 44.3;
    double fHeightSetTopLeftBeyondBottomBorderAngle30 = 103.3;
    QPointF ptCenterSetTopLeftBeyondBottomBorderAngle30(305.8, 386.7);
    QPointF ptTopLeftSetTopLeftBeyondBottomBorderAngle30 = ptSetTopLeftBeyondBottomBorderAngle30;
    QPointF ptTopRightSetTopLeftBeyondBottomBorderAngle30(272.1, 341.7);
    QPointF ptBottomRightSetTopLeftBeyondBottomBorderAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetTopLeftBeyondBottomBorderAngle30(339.499, 431.7);
    QPointF ptTopCenterSetTopLeftBeyondBottomBorderAngle30 =
        QLineF(ptTopLeftSetTopLeftBeyondBottomBorderAngle30, ptTopRightSetTopLeftBeyondBottomBorderAngle30).center();
    QPointF ptRightCenterSetTopLeftBeyondBottomBorderAngle30 =
        QLineF(ptTopRightSetTopLeftBeyondBottomBorderAngle30, ptBottomRightSetTopLeftBeyondBottomBorderAngle30).center();
    QPointF ptBottomCenterSetTopLeftBeyondBottomBorderAngle30 =
        QLineF(ptBottomLeftSetTopLeftBeyondBottomBorderAngle30, ptBottomRightSetTopLeftBeyondBottomBorderAngle30).center();
    QPointF ptLeftCenterSetTopLeftBeyondBottomBorderAngle30 =
        QLineF(ptTopLeftSetTopLeftBeyondBottomBorderAngle30, ptBottomLeftSetTopLeftBeyondBottomBorderAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(BeyondBottomBorder)",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptSetTopLeftBeyondBottomBorderAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptSetTopLeftBeyondBottomBorderAngle30);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondBottomBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondBottomBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(BeyondBottomRight)
    --------------------------------*/
    QPointF ptSetTopLeftBeyondBottomRightAngle30 = QPointF(400.0, bYAxisTopDown ? 530.0 : 50.0);
    physValAngleCorrected.setVal(210.0);
    double fWidthSetTopLeftBeyondBottomRightAngle30 = 101.6;
    double fHeightSetTopLeftBeyondBottomRightAngle30 = 99.2;
    QPointF ptCenterSetTopLeftBeyondBottomRightAngle30(380.8, 461.7);
    QPointF ptTopLeftSetTopLeftBeyondBottomRightAngle30 = ptSetTopLeftBeyondBottomRightAngle30;
    QPointF ptTopRightSetTopLeftBeyondBottomRightAngle30(312.0, 479.2);
    QPointF ptBottomRightSetTopLeftBeyondBottomRightAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetTopLeftBeyondBottomRightAngle30(449.6, 444.1);
    QPointF ptTopCenterSetTopLeftBeyondBottomRightAngle30 =
        QLineF(ptTopLeftSetTopLeftBeyondBottomRightAngle30, ptTopRightSetTopLeftBeyondBottomRightAngle30).center();
    QPointF ptRightCenterSetTopLeftBeyondBottomRightAngle30 =
        QLineF(ptTopRightSetTopLeftBeyondBottomRightAngle30, ptBottomRightSetTopLeftBeyondBottomRightAngle30).center();
    QPointF ptBottomCenterSetTopLeftBeyondBottomRightAngle30 =
        QLineF(ptBottomLeftSetTopLeftBeyondBottomRightAngle30, ptBottomRightSetTopLeftBeyondBottomRightAngle30).center();
    QPointF ptLeftCenterSetTopLeftBeyondBottomRightAngle30 =
        QLineF(ptTopLeftSetTopLeftBeyondBottomRightAngle30, ptBottomLeftSetTopLeftBeyondBottomRightAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(BeyondBottomRight)",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptSetTopLeftBeyondBottomRightAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptSetTopLeftBeyondBottomRightAngle30);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondBottomRightAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondBottomRightAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondBottomRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(BeforeBottomLeft)
    --------------------------------*/
    QPointF ptSetTopRightBeforeBottomLeftAngle30 = QPointF(350.0, bYAxisTopDown ? 200.0 : 400.0);
    double fWidthSetTopRightBeforeBottomLeftAngle30 = 93.3;
    double fHeightSetTopRightBeforeBottomLeftAngle30 = 161.6;
    QPointF ptCenterSetTopRightBeforeBottomLeftAngle30(269.2, 246.7);
    QPointF ptTopLeftSetTopRightBeforeBottomLeftAngle30(269.2, 153.4);
    QPointF ptTopRightSetTopRightBeforeBottomLeftAngle30 = ptSetTopRightBeforeBottomLeftAngle30;
    QPointF ptBottomRightSetTopRightBeforeBottomLeftAngle30(269.2, 340.0);
    QPointF ptBottomLeftSetTopRightBeforeBottomLeftAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetTopRightBeforeBottomLeftAngle30 =
        QLineF(ptTopLeftSetTopRightBeforeBottomLeftAngle30, ptTopRightSetTopRightBeforeBottomLeftAngle30).center();
    QPointF ptRightCenterSetTopRightBeforeBottomLeftAngle30 =
        QLineF(ptTopRightSetTopRightBeforeBottomLeftAngle30, ptBottomRightSetTopRightBeforeBottomLeftAngle30).center();
    QPointF ptBottomCenterSetTopRightBeforeBottomLeftAngle30 =
        QLineF(ptBottomLeftSetTopRightBeforeBottomLeftAngle30, ptBottomRightSetTopRightBeforeBottomLeftAngle30).center();
    QPointF ptLeftCenterSetTopRightBeforeBottomLeftAngle30 =
        QLineF(ptTopLeftSetTopRightBeforeBottomLeftAngle30, ptBottomLeftSetTopRightBeforeBottomLeftAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(BeforeBottomLeft)",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptSetTopRightBeforeBottomLeftAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptSetTopRightBeforeBottomLeftAngle30);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeforeBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeforeBottomLeftAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeforeBottomLeftAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeforeBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeforeBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeforeBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeforeBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeforeBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeforeBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeforeBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeforeBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(BeyondLeftBorder)
    ---------------------------------*/
    QPointF ptSetTopRightBeyondLeftBorderAngle30(150.0, bYAxisTopDown ? 200.0 : 400.0);
    physValAngleCorrected.setVal(300.0);
    double fWidthSetTopRightBeyondLeftBorderAngle30 = 61.6;
    double fHeightSetTopRightBeyondLeftBorderAngle30 = 79.9;
    QPointF ptCenterSetTopRightBeyondLeftBorderAngle30(169.2, 246.7);
    QPointF ptTopLeftSetTopRightBeyondLeftBorderAngle30(119.2, 253.4);
    QPointF ptTopRightSetTopRightBeyondLeftBorderAngle30 = ptSetTopRightBeyondLeftBorderAngle30;
    QPointF ptBottomRightSetTopRightBeyondLeftBorderAngle30(219.2, 240.0);
    QPointF ptBottomLeftSetTopRightBeyondLeftBorderAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetTopRightBeyondLeftBorderAngle30 =
        QLineF(ptTopLeftSetTopRightBeyondLeftBorderAngle30, ptTopRightSetTopRightBeyondLeftBorderAngle30).center();
    QPointF ptRightCenterSetTopRightBeyondLeftBorderAngle30 =
        QLineF(ptTopRightSetTopRightBeyondLeftBorderAngle30, ptBottomRightSetTopRightBeyondLeftBorderAngle30).center();
    QPointF ptBottomCenterSetTopRightBeyondLeftBorderAngle30 =
        QLineF(ptBottomLeftSetTopRightBeyondLeftBorderAngle30, ptBottomRightSetTopRightBeyondLeftBorderAngle30).center();
    QPointF ptLeftCenterSetTopRightBeyondLeftBorderAngle30 =
        QLineF(ptTopLeftSetTopRightBeyondLeftBorderAngle30, ptBottomLeftSetTopRightBeyondLeftBorderAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(BeyondLeftBorder)",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptSetTopRightBeyondLeftBorderAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptSetTopRightBeyondLeftBorderAngle30);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondLeftBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondLeftBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(BeyondBottomBorder)
    ----------------------------------*/
    QPointF ptSetTopRightBeyondBottomBorderAngle30(250.0, bYAxisTopDown ? 400.0 : 200.0);
    physValAngleCorrected.setVal(120.0);
    double fWidthSetTopRightBeyondBottomBorderAngle30 = 61.6;
    double fHeightSetTopRightBeyondBottomBorderAngle30 = 106.7;
    QPointF ptCenterSetTopRightBeyondBottomBorderAngle30(219.2, 346.7);
    QPointF ptTopLeftSetTopRightBeyondBottomBorderAngle30(280.8, 346.7);
    QPointF ptTopRightSetTopRightBeyondBottomBorderAngle30 = ptSetTopRightBeyondBottomBorderAngle30;
    QPointF ptBottomRightSetTopRightBeyondBottomBorderAngle30(157.6, 346.7);
    QPointF ptBottomLeftSetTopRightBeyondBottomBorderAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetTopRightBeyondBottomBorderAngle30 =
        QLineF(ptTopLeftSetTopRightBeyondBottomBorderAngle30, ptTopRightSetTopRightBeyondBottomBorderAngle30).center();
    QPointF ptRightCenterSetTopRightBeyondBottomBorderAngle30 =
        QLineF(ptTopRightSetTopRightBeyondBottomBorderAngle30, ptBottomRightSetTopRightBeyondBottomBorderAngle30).center();
    QPointF ptBottomCenterSetTopRightBeyondBottomBorderAngle30 =
        QLineF(ptBottomLeftSetTopRightBeyondBottomBorderAngle30, ptBottomRightSetTopRightBeyondBottomBorderAngle30).center();
    QPointF ptLeftCenterSetTopRightBeyondBottomBorderAngle30 =
        QLineF(ptTopLeftSetTopRightBeyondBottomBorderAngle30, ptBottomLeftSetTopRightBeyondBottomBorderAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(BeyondBottomBorder)",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptSetTopRightBeyondBottomBorderAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptSetTopRightBeyondBottomBorderAngle30);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondBottomBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondBottomBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondBottomBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(BeyondBottomLeft)
    --------------------------------*/
    QPointF ptSetTopRightBeyondBottomLeftAngle30 = QPointF(50.0, bYAxisTopDown ? 350.0 : 250.0);
    physValAngleCorrected.setVal(210.0);
    double fWidthSetTopRightBeyondBottomLeftAngle30 = 91.5;
    double fHeightSetTopRightBeyondBottomLeftAngle30 = 118.3;
    QPointF ptCenterSetTopRightBeyondBottomLeftAngle30(119.2, 321.7);
    QPointF ptTopLeftSetTopRightBeyondBottomLeftAngle30(129.2, 395.8);
    QPointF ptTopRightSetTopRightBeyondBottomLeftAngle30 = ptSetTopRightBeyondBottomLeftAngle30;
    QPointF ptBottomRightSetTopRightBeyondBottomLeftAngle30(109.2, 247.5);
    QPointF ptBottomLeftSetTopRightBeyondBottomLeftAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetTopRightBeyondBottomLeftAngle30 =
        QLineF(ptTopLeftSetTopRightBeyondBottomLeftAngle30, ptTopRightSetTopRightBeyondBottomLeftAngle30).center();
    QPointF ptRightCenterSetTopRightBeyondBottomLeftAngle30 =
        QLineF(ptTopRightSetTopRightBeyondBottomLeftAngle30, ptBottomRightSetTopRightBeyondBottomLeftAngle30).center();
    QPointF ptBottomCenterSetTopRightBeyondBottomLeftAngle30 =
        QLineF(ptBottomLeftSetTopRightBeyondBottomLeftAngle30, ptBottomRightSetTopRightBeyondBottomLeftAngle30).center();
    QPointF ptLeftCenterSetTopRightBeyondBottomLeftAngle30 =
        QLineF(ptTopLeftSetTopRightBeyondBottomLeftAngle30, ptBottomLeftSetTopRightBeyondBottomLeftAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(BeyondBottomLeft)",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptSetTopRightBeyondBottomLeftAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptSetTopRightBeyondBottomLeftAngle30);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondBottomLeftAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondBottomLeftAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondBottomLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight(BeforeTopLeft)
    --------------------------------*/
    QPointF ptSetSetBottomRightBeforeTopLeftAngle30 = QPointF(250.0, bYAxisTopDown ? 400.0 : 200.0);
    double fWidthSetBottomRightBeforeTopLeftAngle30 = 106.7;
    double fHeightSetBottomRightBeforeTopLeftAngle30 = 161.6;
    QPointF ptCenterSetBottomRightBeforeTopLeftAngle30 (244.2, 303.4);
    QPointF ptTopLeftSetBottomRightBeforeTopLeftAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetBottomRightBeforeTopLeftAngle30(330.8, 260.0);
    QPointF ptBottomRightSetBottomRightBeforeTopLeftAngle30 = ptSetSetBottomRightBeforeTopLeftAngle30;
    QPointF ptBottomLeftSetBottomRightBeforeTopLeftAngle30(157.6, 346.7);
    QPointF ptTopCenterSetBottomRightBeforeTopLeftAngle30 =
        QLineF(ptTopLeftSetBottomRightBeforeTopLeftAngle30, ptTopRightSetBottomRightBeforeTopLeftAngle30).center();
    QPointF ptRightCenterSetBottomRightBeforeTopLeftAngle30 =
        QLineF(ptTopRightSetBottomRightBeforeTopLeftAngle30, ptBottomRightSetBottomRightBeforeTopLeftAngle30).center();
    QPointF ptBottomCenterSetBottomRightBeforeTopLeftAngle30 =
        QLineF(ptBottomLeftSetBottomRightBeforeTopLeftAngle30, ptBottomRightSetBottomRightBeforeTopLeftAngle30).center();
    QPointF ptLeftCenterSetBottomRightBeforeTopLeftAngle30 =
        QLineF(ptTopLeftSetBottomRightBeforeTopLeftAngle30, ptBottomLeftSetBottomRightBeforeTopLeftAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(BeforeTopLeft)",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptSetSetBottomRightBeforeTopLeftAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptSetSetBottomRightBeforeTopLeftAngle30);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeforeTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeforeTopLeftAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeforeTopLeftAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeforeTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeforeTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeforeTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeforeTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeforeTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeforeTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeforeTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeforeTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight(BeyondTopBorder)
    ----------------------------------*/
    QPointF ptSetBottomRightBeyondTopBorderAngle30(400.0, bYAxisTopDown ? 200.0 : 400.0);
    physValAngleCorrected.setVal(300.0);
    double fWidthSetBottomRightBeyondTopBorderAngle30 = 86.6;
    double fHeightSetBottomRightBeyondTopBorderAngle30 = 136.6;
    QPointF ptCenterSetBottomRightBeyondTopBorderAngle30(319.2, 203.4);
    QPointF ptTopLeftSetBottomRightBeyondTopBorderAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetBottomRightBeyondTopBorderAngle30(281.7, 131.7);
    QPointF ptBottomRightSetBottomRightBeyondTopBorderAngle30 = ptSetBottomRightBeyondTopBorderAngle30;
    QPointF ptBottomLeftSetBottomRightBeyondTopBorderAngle30(356.7, 275.0);
    QPointF ptTopCenterSetBottomRightBeyondTopBorderAngle30 =
        QLineF(ptTopLeftSetBottomRightBeyondTopBorderAngle30, ptTopRightSetBottomRightBeyondTopBorderAngle30).center();
    QPointF ptRightCenterSetBottomRightBeyondTopBorderAngle30 =
        QLineF(ptTopRightSetBottomRightBeyondTopBorderAngle30, ptBottomRightSetBottomRightBeyondTopBorderAngle30).center();
    QPointF ptBottomCenterSetBottomRightBeyondTopBorderAngle30 =
        QLineF(ptBottomLeftSetBottomRightBeyondTopBorderAngle30, ptBottomRightSetBottomRightBeyondTopBorderAngle30).center();
    QPointF ptLeftCenterSetBottomRightBeyondTopBorderAngle30 =
        QLineF(ptTopLeftSetBottomRightBeyondTopBorderAngle30, ptBottomLeftSetBottomRightBeyondTopBorderAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(BeyondTopBorder)",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptSetBottomRightBeyondTopBorderAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptSetBottomRightBeyondTopBorderAngle30);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondTopBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondTopBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight(BeyondLeftBorder)
    -----------------------------------*/
    QPointF ptSetBottomRightBeyondLeftBorderAngle30(100.0, bYAxisTopDown ? 200.0 : 400.0);
    physValAngleCorrected.setVal(120.0);
    double fWidthSetBottomRightBeyondLeftBorderAngle30 = 63.4;
    double fHeightSetBottomRightBeyondLeftBorderAngle30 = 123.2;
    QPointF ptCenterSetBottomRightBeyondLeftBorderAngle30(169.2, 203.4);
    QPointF ptTopLeftSetBottomRightBeyondLeftBorderAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetBottomRightBeyondLeftBorderAngle30(206.7, 261.6);
    QPointF ptBottomRightSetBottomRightBeyondLeftBorderAngle30 = ptSetBottomRightBeyondLeftBorderAngle30;
    QPointF ptBottomLeftSetBottomRightBeyondLeftBorderAngle30(131.7, 145.1);
    QPointF ptTopCenterSetBottomRightBeyondLeftBorderAngle30 =
        QLineF(ptTopLeftSetBottomRightBeyondLeftBorderAngle30, ptTopRightSetBottomRightBeyondLeftBorderAngle30).center();
    QPointF ptRightCenterSetBottomRightBeyondLeftBorderAngle30 =
        QLineF(ptTopRightSetBottomRightBeyondLeftBorderAngle30, ptBottomRightSetBottomRightBeyondLeftBorderAngle30).center();
    QPointF ptBottomCenterSetBottomRightBeyondLeftBorderAngle30 =
        QLineF(ptBottomLeftSetBottomRightBeyondLeftBorderAngle30, ptBottomRightSetBottomRightBeyondLeftBorderAngle30).center();
    QPointF ptLeftCenterSetBottomRightBeyondLeftBorderAngle30 =
        QLineF(ptTopLeftSetBottomRightBeyondLeftBorderAngle30, ptBottomLeftSetBottomRightBeyondLeftBorderAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(BeyondLeftBorder)",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptSetBottomRightBeyondLeftBorderAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptSetBottomRightBeyondLeftBorderAngle30);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondLeftBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondLeftBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondLeftBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight(BeyondTopLeft)
    ---------------------------------*/
    QPointF ptSetBottomRightBeyondTopLeftAngle30 = QPointF(200.0, bYAxisTopDown ? 100.0 : 500.0);
    physValAngleCorrected.setVal(210.0);
    double fWidthSetBottomRightBeyondTopLeftAngle30 = 86.6;
    double fHeightSetBottomRightBeyondTopLeftAngle30 = 73.2;
    QPointF ptCenterSetBottomRightBeyondTopLeftAngle30(219.2, 153.4);
    QPointF ptTopLeftSetBottomRightBeyondTopLeftAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetBottomRightBeyondTopLeftAngle30(163.4, 163.4);
    QPointF ptBottomRightSetBottomRightBeyondTopLeftAngle30 = ptSetBottomRightBeyondTopLeftAngle30;
    QPointF ptBottomLeftSetBottomRightBeyondTopLeftAngle30(275.0, 143.3);
    QPointF ptTopCenterSetBottomRightBeyondTopLeftAngle30 =
        QLineF(ptTopLeftSetBottomRightBeyondTopLeftAngle30, ptTopRightSetBottomRightBeyondTopLeftAngle30).center();
    QPointF ptRightCenterSetBottomRightBeyondTopLeftAngle30 =
        QLineF(ptTopRightSetBottomRightBeyondTopLeftAngle30, ptBottomRightSetBottomRightBeyondTopLeftAngle30).center();
    QPointF ptBottomCenterSetBottomRightBeyondTopLeftAngle30 =
        QLineF(ptBottomLeftSetBottomRightBeyondTopLeftAngle30, ptBottomRightSetBottomRightBeyondTopLeftAngle30).center();
    QPointF ptLeftCenterSetBottomRightBeyondTopLeftAngle30 =
        QLineF(ptTopLeftSetBottomRightBeyondTopLeftAngle30, ptBottomLeftSetBottomRightBeyondTopLeftAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(BeyondTopLeft)",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptSetBottomRightBeyondTopLeftAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptSetBottomRightBeyondTopLeftAngle30);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondTopLeftAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondTopLeftAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondTopLeftAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft(BeforeTopRight)
    --------------------------------*/
    QPointF ptSetBottomLeftBeforeTopRightAngle30 = QPointF(250.0, bYAxisTopDown ? 400.0 : 200.0);
    double fWidthSetBottomLeftBeforeTopRightAngle30 = 93.3;
    double fHeightSetBottomLeftBeforeTopRightAngle30 = 161.6;
    QPointF ptCenterSetBottomLeftBeforeTopRightAngle30(330.8, 353.4);
    QPointF ptTopLeftSetBottomLeftBeforeTopRightAngle30(330.8, 260.0);
    QPointF ptTopRightSetBottomLeftBeforeTopRightAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetBottomLeftBeforeTopRightAngle30(330.8, 446.7);
    QPointF ptBottomLeftSetBottomLeftBeforeTopRightAngle30 = ptSetBottomLeftBeforeTopRightAngle30;
    QPointF ptTopCenterSetBottomLeftBeforeTopRightAngle30 =
        QLineF(ptTopLeftSetBottomLeftBeforeTopRightAngle30, ptTopRightSetBottomLeftBeforeTopRightAngle30).center();
    QPointF ptRightCenterSetBottomLeftBeforeTopRightAngle30 =
        QLineF(ptTopRightSetBottomLeftBeforeTopRightAngle30, ptBottomRightSetBottomLeftBeforeTopRightAngle30).center();
    QPointF ptBottomCenterSetBottomLeftBeforeTopRightAngle30 =
        QLineF(ptBottomLeftSetBottomLeftBeforeTopRightAngle30, ptBottomRightSetBottomLeftBeforeTopRightAngle30).center();
    QPointF ptLeftCenterSetBottomLeftBeforeTopRightAngle30 =
        QLineF(ptTopLeftSetBottomLeftBeforeTopRightAngle30, ptBottomLeftSetBottomLeftBeforeTopRightAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(BeforeTopRight)",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptSetBottomLeftBeforeTopRightAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptSetBottomLeftBeforeTopRightAngle30);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeforeTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeforeTopRightAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeforeTopRightAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngle.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeforeTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeforeTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeforeTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeforeTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeforeTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeforeTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeforeTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeforeTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft(BeyondTopBorder)
    ---------------------------------*/
    QPointF ptSetBottomLeftBeyondTopBorderAngle30(350.0, bYAxisTopDown ? 200.0 : 400.0);
    physValAngleCorrected.setVal(120.0);
    double fWidthSetBottomLeftBeyondTopBorderAngle30 = 61.6;
    double fHeightSetBottomLeftBeyondTopBorderAngle30 = 106.7;
    QPointF ptCenterSetBottomLeftBeyondTopBorderAngle30(380.8, 253.4);
    QPointF ptTopLeftSetBottomLeftBeyondTopBorderAngle30(442.4, 253.4);
    QPointF ptTopRightSetBottomLeftBeyondTopBorderAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetBottomLeftBeyondTopBorderAngle30(319.2, 253.4);
    QPointF ptBottomLeftSetBottomLeftBeyondTopBorderAngle30 = ptSetBottomLeftBeyondTopBorderAngle30;
    QPointF ptTopCenterSetBottomLeftBeyondTopBorderAngle30 =
        QLineF(ptTopLeftSetBottomLeftBeyondTopBorderAngle30, ptTopRightSetBottomLeftBeyondTopBorderAngle30).center();
    QPointF ptRightCenterSetBottomLeftBeyondTopBorderAngle30 =
        QLineF(ptTopRightSetBottomLeftBeyondTopBorderAngle30, ptBottomRightSetBottomLeftBeyondTopBorderAngle30).center();
    QPointF ptBottomCenterSetBottomLeftBeyondTopBorderAngle30 =
        QLineF(ptBottomLeftSetBottomLeftBeyondTopBorderAngle30, ptBottomRightSetBottomLeftBeyondTopBorderAngle30).center();
    QPointF ptLeftCenterSetBottomLeftBeyondTopBorderAngle30 =
        QLineF(ptTopLeftSetBottomLeftBeyondTopBorderAngle30, ptBottomLeftSetBottomLeftBeyondTopBorderAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(BeyondTopBorder)",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptSetBottomLeftBeyondTopBorderAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptSetBottomLeftBeyondTopBorderAngle30);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondTopBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondTopBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondTopBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft(BeyondRightBorder)
    -----------------------------------*/
    QPointF ptSetBottomLeftBeyondRightBorderAngle30(450.0, bYAxisTopDown ? 400.0 : 200.0);
    physValAngleCorrected.setVal(300.0);
    double fWidthSetBottomLeftBeyondRightBorderAngle30 = 61.6;
    double fHeightSetBottomLeftBeyondRightBorderAngle30 = 79.9;
    QPointF ptCenterSetBottomLeftBeyondRightBorderAngle30(430.8, 353.4);
    QPointF ptTopLeftSetBottomLeftBeyondRightBorderAngle30(380.8, 360.0);
    QPointF ptTopRightSetBottomLeftBeyondRightBorderAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetBottomLeftBeyondRightBorderAngle30(480.8, 346.7);
    QPointF ptBottomLeftSetBottomLeftBeyondRightBorderAngle30 = ptSetBottomLeftBeyondRightBorderAngle30;
    QPointF ptTopCenterSetBottomLeftBeyondRightBorderAngle30 =
        QLineF(ptTopLeftSetBottomLeftBeyondRightBorderAngle30, ptTopRightSetBottomLeftBeyondRightBorderAngle30).center();
    QPointF ptRightCenterSetBottomLeftBeyondRightBorderAngle30 =
        QLineF(ptTopRightSetBottomLeftBeyondRightBorderAngle30, ptBottomRightSetBottomLeftBeyondRightBorderAngle30).center();
    QPointF ptBottomCenterSetBottomLeftBeyondRightBorderAngle30 =
        QLineF(ptBottomLeftSetBottomLeftBeyondRightBorderAngle30, ptBottomRightSetBottomLeftBeyondRightBorderAngle30).center();
    QPointF ptLeftCenterSetBottomLeftBeyondRightBorderAngle30 =
        QLineF(ptTopLeftSetBottomLeftBeyondRightBorderAngle30, ptBottomLeftSetBottomLeftBeyondRightBorderAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(BeyondRightBorder)",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptSetBottomLeftBeyondRightBorderAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptSetBottomLeftBeyondRightBorderAngle30);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondRightBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondRightBorderAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondRightBorderAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft(BeyondTopRight)
    --------------------------------
    */
    QPointF ptSetBottomLeftBeyondTopRightAngle30 = QPointF(550.0, bYAxisTopDown ? 300.0 : 300.0);
    physValAngleCorrected.setVal(210.0);
    double fWidthSetBottomLeftBeyondTopRightAngle30 = 116.5;
    double fHeightSetBottomLeftBeyondTopRightAngle30 = 75.0;
    QPointF ptCenterSetBottomLeftBeyondTopRightAngle30(480.8, 303.4);
    QPointF ptTopLeftSetBottomLeftBeyondTopRightAngle30(512.5, 365.0);
    QPointF ptTopRightSetBottomLeftBeyondTopRightAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetBottomLeftBeyondTopRightAngle30(449.1, 241.7);
    QPointF ptBottomLeftSetBottomLeftBeyondTopRightAngle30 = ptSetBottomLeftBeyondTopRightAngle30;
    QPointF ptTopCenterSetBottomLeftBeyondTopRightAngle30 =
        QLineF(ptTopLeftSetBottomLeftBeyondTopRightAngle30, ptTopRightSetBottomLeftBeyondTopRightAngle30).center();
    QPointF ptRightCenterSetBottomLeftBeyondTopRightAngle30 =
        QLineF(ptTopRightSetBottomLeftBeyondTopRightAngle30, ptBottomRightSetBottomLeftBeyondTopRightAngle30).center();
    QPointF ptBottomCenterSetBottomLeftBeyondTopRightAngle30 =
        QLineF(ptBottomLeftSetBottomLeftBeyondTopRightAngle30, ptBottomRightSetBottomLeftBeyondTopRightAngle30).center();
    QPointF ptLeftCenterSetBottomLeftBeyondTopRightAngle30 =
        QLineF(ptTopLeftSetBottomLeftBeyondTopRightAngle30, ptBottomLeftSetBottomLeftBeyondTopRightAngle30).center();
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(BeyondTopRight)",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptSetBottomLeftBeyondTopRightAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create.topLeft", ptTLOrig);
    pTestStep->setConfigValue("create.size", sizeOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptSetBottomLeftBeyondTopRightAngle30);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondTopRightAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondTopRightAngle30, iDigits), 'f', iDigits) + " " + strUnit);
    strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
    strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
    strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondTopRightAngle30, ", ", 'f', iDigits) + "} " + strUnit);
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
    pTestStep->setConfigValue("Rect.topLeft", QPointF(0.0, 0.0));
    pTestStep->setConfigValue("Rect.size", QSizeF(350.0, 350.0));
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
    pTestStep->setConfigValue("Rect.topLeft", QPointF(70.0, 180.0));
    pTestStep->setConfigValue("Rect.size", QSizeF(60.0, 140.0));
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
    pTestStep->setConfigValue("Rect.topLeft", QPointF(0.0, fTLYPos_mm));
    pTestStep->setConfigValue("Rect.size", QSizeF(100.0, fabs(fBRYPos_mm-fTLYPos_mm)/2.0));
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
    pTestStep->setConfigValue("Rect.topLeft", QPointF(20.0, fTLYPos_mm));
    pTestStep->setConfigValue("Rect.size", QSizeF(17.1, fabs(fBRYPos_mm-fTLYPos_mm)/2.0));
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

    CDrawingSize drawingSize;
    drawingSize.setDimensionUnit(EScaleDimensionUnit::Pixels);
    drawingSize.setScreenResolutionInPxPerMM(1.0);
    drawingSize.setImageSize(CPhysVal(800, Units.Length.px), CPhysVal(600, Units.Length.px));

    CDrawGridSettings gridSettings;
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

    CDrawingSize drawingSize;
    drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
    drawingSize.setScreenResolutionInPxPerMM(1.0);
    drawingSize.setImageSize(CPhysVal(800, Units.Length.mm), CPhysVal(600, Units.Length.mm));
    drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::TopDown);

    CDrawGridSettings gridSettings;
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

    CDrawingSize drawingSize;
    drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
    drawingSize.setScreenResolutionInPxPerMM(1.0);
    drawingSize.setImageSize(CPhysVal(800, Units.Length.mm), CPhysVal(600, Units.Length.mm));
    drawingSize.setYScaleAxisOrientation(EYScaleAxisOrientation::BottomUp);

    CDrawGridSettings gridSettings;
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

    CDrawingSize drawingSize;

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

    CDrawingSize drawingSize;
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

    CDrawGridSettings gridSettings;
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
    CPhysValRect physValRectResult(*m_pDrawingScene);

    QPointF ptTL = i_pTestStep->getConfigValue("create.topLeft").toPointF();
    if (i_pTestStep->hasConfigValue("create.bottomRight")) {
        QPointF ptBR = i_pTestStep->getConfigValue("create.bottomRight").toPointF();
        if (i_pTestStep->hasConfigValue("create.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("create.unit").toString();
            CUnit unit = strUnitPoint;
            physValRectResult = CPhysValRect(*m_pDrawingScene, ptTL, ptBR, unit);
        }
        else {
            physValRectResult = CPhysValRect(*m_pDrawingScene, ptTL, ptBR);
        }
    }
    else if (i_pTestStep->hasConfigValue("create.size")) {
        QSizeF size = i_pTestStep->getConfigValue("create.size").toSizeF();
        if (i_pTestStep->hasConfigValue("create.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("create.unit").toString();
            CUnit unit = strUnitPoint;
            physValRectResult = CPhysValRect(*m_pDrawingScene, ptTL, size, unit);
        }
        else {
            physValRectResult = CPhysValRect(*m_pDrawingScene, ptTL, size);
        }
    }

    if (i_pTestStep->hasConfigValue("removeAndDeleteAllPhysValShapes")) {
        m_pDrawingScene->removeAndDeleteAllPhysValShapes();
    }
    if (i_pTestStep->hasConfigValue("addPhysValShape")) {
        CDrawSettings drawSettings(EGraphObjTypeRect);
        drawSettings.setPenColor(Qt::blue);
        CPhysValRect* pPhysValRect = new CPhysValRect(physValRectResult);
        m_pDrawingScene->addPhysValShape(pPhysValRect, drawSettings);
        pPhysValRect = nullptr;
    }
    if (i_pTestStep->hasConfigValue("setAngle")) {
        CPhysVal physValAngle = i_pTestStep->getConfigValue("setAngle").toString();
        physValAngle.setRes(0.1);
        physValRectResult.setAngle(physValAngle);
        if (physValAngle.getVal() != 0.0 && i_pTestStep->hasConfigValue("addPhysValShape")) {
            CDrawSettings drawSettings(EGraphObjTypeRect);
            drawSettings.setPenColor(Qt::darkGreen);
            CPhysValRect* pPhysValRect = new CPhysValRect(physValRectResult);
            m_pDrawingScene->addPhysValShape(pPhysValRect, drawSettings);
            pPhysValRect = nullptr;
        }
    }
    if (i_pTestStep->hasConfigValue("setCenter")) {
        QPointF pt = i_pTestStep->getConfigValue("setCenter").toPointF();
        if (i_pTestStep->hasConfigValue("setCenter.unit")) {
            QString strUnitPoint = i_pTestStep->getConfigValue("setCenter.unit").toString();
            CUnit unit = strUnitPoint;
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
            CUnit unit = strUnitPoint;
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
            CUnit unit = strUnitPoint;
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
            CUnit unit = strUnitPoint;
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
            CUnit unit = strUnitPoint;
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
            CUnit unit = strUnitPoint;
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
            CUnit unit = strUnitPoint;
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
            CUnit unit = strUnitPoint;
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
            CUnit unit = strUnitPoint;
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
            CUnit unit = strUnitPoint;
            CPhysValPoint physValPt(*m_pDrawingScene, pt, unit);
            physValRectResult.setBottomLeft(physValPt);
        }
        else {
            CPhysValPoint physValPt(*m_pDrawingScene, pt);
            physValRectResult.setBottomLeft(physValPt);
        }
    }
    if (i_pTestStep->hasConfigValue("addPhysValShape")) {
        CDrawSettings drawSettings(EGraphObjTypeRect);
        drawSettings.setPenColor(Qt::red);
        CPhysValRect* pPhysValRect = new CPhysValRect(physValRectResult);
        m_pDrawingScene->addPhysValShape(pPhysValRect, drawSettings);
        pPhysValRect = nullptr;
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
    QPointF ptTL = i_pTestStep->getConfigValue("Rect.topLeft").toPointF();
    QSizeF size = i_pTestStep->getConfigValue("Rect.size").toSizeF();
    CPhysValRect physValRectResult(*m_pDrawingScene);
    if (Units.Length.isMetricUnit(unitSource)) {
        CPhysValRect physValRect(*m_pDrawingScene, ptTL, size, unitSource);
        physValRectResult = m_pDrawingScene->convert(physValRect, unitDest);
    }
    else {
        physValRectResult = m_pDrawingScene->convert(QRectF(ptTL, size), unitDest);
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
