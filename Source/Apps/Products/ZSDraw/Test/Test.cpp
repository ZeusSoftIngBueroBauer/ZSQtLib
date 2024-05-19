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
    CPhysVal physValAngleCorrected(0.0, Units.Angle.Degree, 0.1);

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
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("addPhysValShape", "");
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
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setCenter", ptCenter);
    pTestStep->setConfigValue("setCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
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
    double fxCenterSetSizeAngle0 = fLeftSetAngle0 + size300x200.width() / 2.0;
    double fyCenterSetSizeAngle0 = fTopSetAngle0 + size300x200.height() / 2.0;
    double fWidthSetSizeAngle0 = size300x200.width();
    double fHeightSetSizeAngle0 = size300x200.height();
    double fLeftSetSizeAngle0 = fxCenterSetSizeAngle0 - fWidthSetSizeAngle0 / 2.0;
    double fRightSetSizeAngle0 = fxCenterSetSizeAngle0 + fWidthSetSizeAngle0 / 2.0;
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
        /* strName         */ "Step " + QString::number(++idxStep) + " setSize(" + qSize2Str(size300x200) + " " + unit.symbol() + ")",
        /* strOperation    */ "setSize(" + qSize2Str(size300x200) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setSize", size300x200);
    pTestStep->setConfigValue("setSize.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetSizeAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetSizeAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetSizeAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetSizeAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetSizeAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetSizeAngle0, 1), 'f', 1) + " mm");
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
    double fxCenterSetWidthAngle0 = fLeftSetAngle0 + physValWidth300.getVal() / 2.0;
    double fyCenterSetWidthAngle0 = fyCenterSetAngle0;
    double fWidthSetWidthAngle0 = physValWidth300.getVal();
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
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidth(" + physValWidth300.toString() + ")",
        /* strOperation    */ "setWidth(" + physValWidth300.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidth", physValWidth300.toString());
    pTestStep->setConfigValue("setWidth.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthAngle0, 1), 'f', 1) + " mm");
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

    /* setWidthByMovingLeftCenter(100)
    ----------------------------------
        200   250   300   350   400          300   350   400  BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 +-----+-----+-----+-----+        350 +-----+-----+      250
    */
    QPointF ptMoved = ptTopCenterSetAngle0;
    double fxCenterSetWidthMoveLeftCenterAngle0 = fRightSetAngle0 - (fRightSetAngle0 - ptMoved.x()) / 2.0;
    double fyCenterSetWidthMoveLeftCenterAngle0 = fyCenterSetAngle0;
    double fWidthSetWidthMoveLeftCenterAngle0 = fabs(fRightSetAngle0 - ptMoved.x());
    double fHeightSetWidthMoveLeftCenterAngle0 = fHeightSetAngle0;
    double fLeftSetWidthMoveLeftCenterAngle0 = fxCenterSetWidthMoveLeftCenterAngle0 - fWidthSetWidthMoveLeftCenterAngle0 / 2.0;
    double fRightSetWidthMoveLeftCenterAngle0 = fxCenterSetWidthMoveLeftCenterAngle0 + fWidthSetWidthMoveLeftCenterAngle0 / 2.0;
    double fTopSetWidthMoveLeftCenterAngle0 = fyCenterSetWidthMoveLeftCenterAngle0 - fHeightSetWidthMoveLeftCenterAngle0 / 2.0;
    double fBottomSetWidthMoveLeftCenterAngle0 = fyCenterSetWidthMoveLeftCenterAngle0 + fHeightSetWidthMoveLeftCenterAngle0 / 2.0;
    QPointF ptCenterSetWidthMoveLeftCenterAngle0(fxCenterSetWidthMoveLeftCenterAngle0, fyCenterSetWidthMoveLeftCenterAngle0);
    QPointF ptTopLeftSetWidthMoveLeftCenterAngle0(fLeftSetWidthMoveLeftCenterAngle0, fTopSetWidthMoveLeftCenterAngle0);
    QPointF ptTopRightSetWidthMoveLeftCenterAngle0(fRightSetWidthMoveLeftCenterAngle0, fTopSetWidthMoveLeftCenterAngle0);
    QPointF ptBottomRightSetWidthMoveLeftCenterAngle0(fRightSetWidthMoveLeftCenterAngle0, fBottomSetWidthMoveLeftCenterAngle0);
    QPointF ptBottomLeftSetWidthMoveLeftCenterAngle0(fLeftSetWidthMoveLeftCenterAngle0, fBottomSetWidthMoveLeftCenterAngle0);
    QPointF ptTopCenterSetWidthMoveLeftCenterAngle0(fxCenterSetWidthMoveLeftCenterAngle0, fTopSetWidthMoveLeftCenterAngle0);
    QPointF ptRightCenterSetWidthMoveLeftCenterAngle0(fRightSetWidthMoveLeftCenterAngle0, fyCenterSetWidthMoveLeftCenterAngle0);
    QPointF ptBottomCenterSetWidthMoveLeftCenterAngle0(fxCenterSetWidthMoveLeftCenterAngle0, fBottomSetWidthMoveLeftCenterAngle0);
    QPointF ptLeftCenterSetWidthMoveLeftCenterAngle0(fLeftSetWidthMoveLeftCenterAngle0, fyCenterSetWidthMoveLeftCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingLeftCenter(500, 250)
    ---------------------------------------
        200   250   300   350   400          400   450   500  BottomUp
     250 TL----+-----+-----+----TR        250 BR----+----BL      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 BL----+-----+-----+----BR        350 TR----+----TL      250
    */
    ptMoved = QPointF(500, 250);
    physValAngleCorrected.setVal(180.0);
    double fxCenterSetWidthMoveLeftCenterBeyondRightAngle0 = fRightSetAngle0 + (ptMoved.x()- fRightSetAngle0) / 2.0;
    double fyCenterSetWidthMoveLeftCenterBeyondRightAngle0 = fyCenterSetAngle0;
    double fWidthSetWidthMoveLeftCenterBeyondRightAngle0 = fabs(fRightSetAngle0 - ptMoved.x());
    double fHeightSetWidthMoveLeftCenterBeyondRightAngle0 = fHeightSetAngle0;
    double fLeftSetWidthMoveLeftCenterBeyondRightAngle0 = fxCenterSetWidthMoveLeftCenterBeyondRightAngle0 + fWidthSetWidthMoveLeftCenterBeyondRightAngle0 / 2.0;
    double fRightSetWidthMoveLeftCenterBeyondRightAngle0 = fxCenterSetWidthMoveLeftCenterBeyondRightAngle0 - fWidthSetWidthMoveLeftCenterBeyondRightAngle0 / 2.0;
    double fTopSetWidthMoveLeftCenterBeyondRightAngle0 = fyCenterSetWidthMoveLeftCenterBeyondRightAngle0 + fHeightSetWidthMoveLeftCenterBeyondRightAngle0 / 2.0;
    double fBottomSetWidthMoveLeftCenterBeyondRightAngle0 = fyCenterSetWidthMoveLeftCenterBeyondRightAngle0 - fHeightSetWidthMoveLeftCenterBeyondRightAngle0 / 2.0;
    QPointF ptCenterSetWidthMoveLeftCenterBeyondRightAngle0(fxCenterSetWidthMoveLeftCenterBeyondRightAngle0, fyCenterSetWidthMoveLeftCenterBeyondRightAngle0);
    QPointF ptTopLeftSetWidthMoveLeftCenterBeyondRightAngle0(fLeftSetWidthMoveLeftCenterBeyondRightAngle0, fTopSetWidthMoveLeftCenterBeyondRightAngle0);
    QPointF ptTopRightSetWidthMoveLeftCenterBeyondRightAngle0(fRightSetWidthMoveLeftCenterBeyondRightAngle0, fTopSetWidthMoveLeftCenterBeyondRightAngle0);
    QPointF ptBottomRightSetWidthMoveLeftCenterBeyondRightAngle0(fRightSetWidthMoveLeftCenterBeyondRightAngle0, fBottomSetWidthMoveLeftCenterBeyondRightAngle0);
    QPointF ptBottomLeftSetWidthMoveLeftCenterBeyondRightAngle0(fLeftSetWidthMoveLeftCenterBeyondRightAngle0, fBottomSetWidthMoveLeftCenterBeyondRightAngle0);
    QPointF ptTopCenterSetWidthMoveLeftCenterBeyondRightAngle0(fxCenterSetWidthMoveLeftCenterBeyondRightAngle0, fTopSetWidthMoveLeftCenterBeyondRightAngle0);
    QPointF ptRightCenterSetWidthMoveLeftCenterBeyondRightAngle0(fRightSetWidthMoveLeftCenterBeyondRightAngle0, fyCenterSetWidthMoveLeftCenterBeyondRightAngle0);
    QPointF ptBottomCenterSetWidthMoveLeftCenterBeyondRightAngle0(fxCenterSetWidthMoveLeftCenterBeyondRightAngle0, fBottomSetWidthMoveLeftCenterBeyondRightAngle0);
    QPointF ptLeftCenterSetWidthMoveLeftCenterBeyondRightAngle0(fLeftSetWidthMoveLeftCenterBeyondRightAngle0, fyCenterSetWidthMoveLeftCenterBeyondRightAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterBeyondRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterBeyondRightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterBeyondRightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterBeyondRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterBeyondRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterBeyondRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterBeyondRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterBeyondRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterBeyondRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterBeyondRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterBeyondRightAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterBeyondRightAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterBeyondRightAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterBeyondRightAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterBeyondRightAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterBeyondRightAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterBeyondRightAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterBeyondRightAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterBeyondRightAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterBeyondRightAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterBeyondRightAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterBeyondRightAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingRightCenter(100)
    -----------------------------------
        200   250   300   350   400          200   250   300  BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 +-----+-----+-----+-----+        350 +-----+-----+      250
    */
    ptMoved = ptTopCenterSetAngle0;
    double fxCenterSetWidthMoveRightCenterAngle0 = fLeftSetAngle0 + (ptMoved.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetWidthMoveRightCenterAngle0 = fyCenterSetAngle0;
    double fWidthSetWidthMoveRightCenterAngle0 = fabs(ptMoved.x() - fLeftSetAngle0);
    double fHeightSetWidthMoveRightCenterAngle0 = fHeightSetAngle0;
    double fLeftSetWidthMoveRightCenterAngle0 = fxCenterSetWidthMoveRightCenterAngle0 - fWidthSetWidthMoveRightCenterAngle0 / 2.0;
    double fRightSetWidthMoveRightCenterAngle0 = fxCenterSetWidthMoveRightCenterAngle0 + fWidthSetWidthMoveRightCenterAngle0 / 2.0;
    double fTopSetWidthMoveRightCenterAngle0 = fyCenterSetWidthMoveRightCenterAngle0 - fHeightSetWidthMoveRightCenterAngle0 / 2.0;
    double fBottomSetWidthMoveRightCenterAngle0 = fyCenterSetWidthMoveRightCenterAngle0 + fHeightSetWidthMoveRightCenterAngle0 / 2.0;
    QPointF ptCenterSetWidthMoveRightCenterAngle0(fxCenterSetWidthMoveRightCenterAngle0, fyCenterSetWidthMoveRightCenterAngle0);
    QPointF ptTopLeftSetWidthMoveRightCenterAngle0(fLeftSetWidthMoveRightCenterAngle0, fTopSetWidthMoveRightCenterAngle0);
    QPointF ptTopRightSetWidthMoveRightCenterAngle0(fRightSetWidthMoveRightCenterAngle0, fTopSetWidthMoveRightCenterAngle0);
    QPointF ptBottomRightSetWidthMoveRightCenterAngle0(fRightSetWidthMoveRightCenterAngle0, fBottomSetWidthMoveRightCenterAngle0);
    QPointF ptBottomLeftSetWidthMoveRightCenterAngle0(fLeftSetWidthMoveRightCenterAngle0, fBottomSetWidthMoveRightCenterAngle0);
    QPointF ptTopCenterSetWidthMoveRightCenterAngle0(fxCenterSetWidthMoveRightCenterAngle0, fTopSetWidthMoveRightCenterAngle0);
    QPointF ptRightCenterSetWidthMoveRightCenterAngle0(fRightSetWidthMoveRightCenterAngle0, fyCenterSetWidthMoveRightCenterAngle0);
    QPointF ptBottomCenterSetWidthMoveRightCenterAngle0(fxCenterSetWidthMoveRightCenterAngle0, fBottomSetWidthMoveRightCenterAngle0);
    QPointF ptLeftCenterSetWidthMoveRightCenterAngle0(fLeftSetWidthMoveRightCenterAngle0, fyCenterSetWidthMoveRightCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingRightCenter(100, 250)
    ----------------------------------------
        200   250   300   350   400          100   150   200  BottomUp
     250 TL----+-----+-----+----TR        250 BR----+----BL      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 BL----+-----+-----+----BR        350 TR----+----TL      250
    */
    ptMoved = QPointF(100.0, 250.0);
    physValAngleCorrected.setVal(180.0);
    double fxCenterSetWidthMoveRightCenterBeyondLeftAngle0 = fLeftSetAngle0 + (ptMoved.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetWidthMoveRightCenterBeyondLeftAngle0 = fyCenterSetAngle0;
    double fWidthSetWidthMoveRightCenterBeyondLeftAngle0 = fabs(ptMoved.x() - fLeftSetAngle0);
    double fHeightSetWidthMoveRightCenterBeyondLeftAngle0 = fHeightSetAngle0;
    double fLeftSetWidthMoveRightCenterBeyondLeftAngle0 = fxCenterSetWidthMoveRightCenterBeyondLeftAngle0 + fWidthSetWidthMoveRightCenterBeyondLeftAngle0 / 2.0;
    double fRightSetWidthMoveRightCenterBeyondLeftAngle0 = fxCenterSetWidthMoveRightCenterBeyondLeftAngle0 - fWidthSetWidthMoveRightCenterBeyondLeftAngle0 / 2.0;
    double fTopSetWidthMoveRightCenterBeyondLeftAngle0 = fyCenterSetWidthMoveRightCenterBeyondLeftAngle0 + fHeightSetWidthMoveRightCenterBeyondLeftAngle0 / 2.0;
    double fBottomSetWidthMoveRightCenterBeyondLeftAngle0 = fyCenterSetWidthMoveRightCenterBeyondLeftAngle0 - fHeightSetWidthMoveRightCenterBeyondLeftAngle0 / 2.0;
    QPointF ptCenterSetWidthMoveRightCenterBeyondLeftAngle0(fxCenterSetWidthMoveRightCenterBeyondLeftAngle0, fyCenterSetWidthMoveRightCenterBeyondLeftAngle0);
    QPointF ptTopLeftSetWidthMoveRightCenterBeyondLeftAngle0(fLeftSetWidthMoveRightCenterBeyondLeftAngle0, fTopSetWidthMoveRightCenterBeyondLeftAngle0);
    QPointF ptTopRightSetWidthMoveRightCenterBeyondLeftAngle0(fRightSetWidthMoveRightCenterBeyondLeftAngle0, fTopSetWidthMoveRightCenterBeyondLeftAngle0);
    QPointF ptBottomRightSetWidthMoveRightCenterBeyondLeftAngle0(fRightSetWidthMoveRightCenterBeyondLeftAngle0, fBottomSetWidthMoveRightCenterBeyondLeftAngle0);
    QPointF ptBottomLeftSetWidthMoveRightCenterBeyondLeftAngle0(fLeftSetWidthMoveRightCenterBeyondLeftAngle0, fBottomSetWidthMoveRightCenterBeyondLeftAngle0);
    QPointF ptTopCenterSetWidthMoveRightCenterBeyondLeftAngle0(fxCenterSetWidthMoveRightCenterBeyondLeftAngle0, fTopSetWidthMoveRightCenterBeyondLeftAngle0);
    QPointF ptRightCenterSetWidthMoveRightCenterBeyondLeftAngle0(fRightSetWidthMoveRightCenterBeyondLeftAngle0, fyCenterSetWidthMoveRightCenterBeyondLeftAngle0);
    QPointF ptBottomCenterSetWidthMoveRightCenterBeyondLeftAngle0(fxCenterSetWidthMoveRightCenterBeyondLeftAngle0, fBottomSetWidthMoveRightCenterBeyondLeftAngle0);
    QPointF ptLeftCenterSetWidthMoveRightCenterBeyondLeftAngle0(fLeftSetWidthMoveRightCenterBeyondLeftAngle0, fyCenterSetWidthMoveRightCenterBeyondLeftAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterBeyondLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterBeyondLeftAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterBeyondLeftAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterBeyondLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterBeyondLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterBeyondLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterBeyondLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterBeyondLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterBeyondLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterBeyondLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterBeyondLeftAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterBeyondLeftAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterBeyondLeftAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterBeyondLeftAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterBeyondLeftAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterBeyondLeftAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterBeyondLeftAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterBeyondLeftAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterBeyondLeftAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterBeyondLeftAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterBeyondLeftAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterBeyondLeftAngle0) + "} mm");
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
    double fxCenterSetHeightAngle0 = fxCenterSetAngle0;
    double fyCenterSetHeightAngle0 = fyCenterSetAngle0 + (physValHeight200.getVal() - fHeightSetAngle0) / 2.0;
    double fWidthSetHeightAngle0 = fWidthSetAngle0;
    double fHeightSetHeightAngle0 = physValHeight200.getVal();
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
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeight(" + physValHeight200.toString() + ")",
        /* strOperation    */ "setHeight(" + physValHeight200.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeight", physValHeight200.toString());
    pTestStep->setConfigValue("setHeight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightAngle0, 1), 'f', 1) + " mm");
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

    /* setHeightByMovingTopCenter(50)
    ---------------------------------------
        200   250   300   350   400          200   250   300   350   400   BottomUp
     250 +-----+-----+-----+-----+        250                                350
         |                       |
     300 +           X           +        300 +-----+-----+-----+-----+      300
         |                       |   =>       |           X           |
     350 +-----+-----+-----+-----+        350 +-----+-----+-----+-----+
    */
    ptMoved = ptRightCenterSetAngle0;
    double fxCenterSetHeightMoveTopCenterAngle0 = fxCenterSetAngle0;
    double fyCenterSetHeightMoveTopCenterAngle0 = fBottomSetAngle0 - (fBottomSetAngle0 - ptMoved.y()) / 2.0;
    double fWidthSetHeightMoveTopCenterAngle0 = fWidthSetAngle0;
    double fHeightSetHeightMoveTopCenterAngle0 = fabs(fBottomSetAngle0 - ptMoved.y());
    double fLeftSetHeightMoveTopCenterAngle0 = fxCenterSetHeightMoveTopCenterAngle0 - fWidthSetHeightMoveTopCenterAngle0 / 2.0;
    double fRightSetHeightMoveTopCenterAngle0 = fxCenterSetHeightMoveTopCenterAngle0 + fWidthSetHeightMoveTopCenterAngle0 / 2.0;
    double fTopSetHeightMoveTopCenterAngle0 = fyCenterSetHeightMoveTopCenterAngle0 - fHeightSetHeightMoveTopCenterAngle0 / 2.0;
    double fBottomSetHeightMoveTopCenterAngle0 = fyCenterSetHeightMoveTopCenterAngle0 + fHeightSetHeightMoveTopCenterAngle0 / 2.0;
    QPointF ptCenterSetHeightMoveTopCenterAngle0(fxCenterSetHeightMoveTopCenterAngle0, fyCenterSetHeightMoveTopCenterAngle0);
    QPointF ptTopLeftSetHeightMoveTopCenterAngle0(fLeftSetHeightMoveTopCenterAngle0, fTopSetHeightMoveTopCenterAngle0);
    QPointF ptTopRightSetHeightMoveTopCenterAngle0(fRightSetHeightMoveTopCenterAngle0, fTopSetHeightMoveTopCenterAngle0);
    QPointF ptBottomRightSetHeightMoveTopCenterAngle0(fRightSetHeightMoveTopCenterAngle0, fBottomSetHeightMoveTopCenterAngle0);
    QPointF ptBottomLeftSetHeightMoveTopCenterAngle0(fLeftSetHeightMoveTopCenterAngle0, fBottomSetHeightMoveTopCenterAngle0);
    QPointF ptTopCenterSetHeightMoveTopCenterAngle0(fxCenterSetHeightMoveTopCenterAngle0, fTopSetHeightMoveTopCenterAngle0);
    QPointF ptRightCenterSetHeightMoveTopCenterAngle0(fRightSetHeightMoveTopCenterAngle0, fyCenterSetHeightMoveTopCenterAngle0);
    QPointF ptBottomCenterSetHeightMoveTopCenterAngle0(fxCenterSetHeightMoveTopCenterAngle0, fBottomSetHeightMoveTopCenterAngle0);
    QPointF ptLeftCenterSetHeightMoveTopCenterAngle0(fLeftSetHeightMoveTopCenterAngle0, fyCenterSetHeightMoveTopCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingTopCenter(400, 400)
    ---------------------------------------
        200   250   300   350   400          200   250   300   350   400   BottomUp
     250 TL----+-----+-----+----TR                                           350
         |                       |
     300 +           X           +                                           300
         |                       |
     350 BL----+-----+-----+----BR        350 BR----+-----+-----+----BL      250
                                              |           X           |
                                          400 TR----+-----+-----+----TL      200
    */
    ptMoved = QPointF(400, 400);
    physValAngleCorrected.setVal(180.0);
    double fxCenterSetHeightMoveTopCenterBeyondBottomAngle0 = fxCenterSetAngle0;
    double fyCenterSetHeightMoveTopCenterBeyondBottomAngle0 = fBottomSetAngle0 - (fBottomSetAngle0 - ptMoved.y()) / 2.0;
    double fWidthSetHeightMoveTopCenterBeyondBottomAngle0 = fWidthSetAngle0;
    double fHeightSetHeightMoveTopCenterBeyondBottomAngle0 = fabs(fBottomSetAngle0 - ptMoved.y());
    double fLeftSetHeightMoveTopCenterBeyondBottomAngle0 = fxCenterSetHeightMoveTopCenterBeyondBottomAngle0 + fWidthSetHeightMoveTopCenterBeyondBottomAngle0 / 2.0;
    double fRightSetHeightMoveTopCenterBeyondBottomAngle0 = fxCenterSetHeightMoveTopCenterBeyondBottomAngle0 - fWidthSetHeightMoveTopCenterBeyondBottomAngle0 / 2.0;
    double fTopSetHeightMoveTopCenterBeyondBottomAngle0 = fyCenterSetHeightMoveTopCenterBeyondBottomAngle0 + fHeightSetHeightMoveTopCenterBeyondBottomAngle0 / 2.0;
    double fBottomSetHeightMoveTopCenterBeyondBottomAngle0 = fyCenterSetHeightMoveTopCenterBeyondBottomAngle0 - fHeightSetHeightMoveTopCenterBeyondBottomAngle0 / 2.0;
    QPointF ptCenterSetHeightMoveTopCenterBeyondBottomAngle0(fxCenterSetHeightMoveTopCenterBeyondBottomAngle0, fyCenterSetHeightMoveTopCenterBeyondBottomAngle0);
    QPointF ptTopLeftSetHeightMoveTopCenterBeyondBottomAngle0(fLeftSetHeightMoveTopCenterBeyondBottomAngle0, fTopSetHeightMoveTopCenterBeyondBottomAngle0);
    QPointF ptTopRightSetHeightMoveTopCenterBeyondBottomAngle0(fRightSetHeightMoveTopCenterBeyondBottomAngle0, fTopSetHeightMoveTopCenterBeyondBottomAngle0);
    QPointF ptBottomRightSetHeightMoveTopCenterBeyondBottomAngle0(fRightSetHeightMoveTopCenterBeyondBottomAngle0, fBottomSetHeightMoveTopCenterBeyondBottomAngle0);
    QPointF ptBottomLeftSetHeightMoveTopCenterBeyondBottomAngle0(fLeftSetHeightMoveTopCenterBeyondBottomAngle0, fBottomSetHeightMoveTopCenterBeyondBottomAngle0);
    QPointF ptTopCenterSetHeightMoveTopCenterBeyondBottomAngle0(fxCenterSetHeightMoveTopCenterBeyondBottomAngle0, fTopSetHeightMoveTopCenterBeyondBottomAngle0);
    QPointF ptRightCenterSetHeightMoveTopCenterBeyondBottomAngle0(fRightSetHeightMoveTopCenterBeyondBottomAngle0, fyCenterSetHeightMoveTopCenterBeyondBottomAngle0);
    QPointF ptBottomCenterSetHeightMoveTopCenterBeyondBottomAngle0(fxCenterSetHeightMoveTopCenterBeyondBottomAngle0, fBottomSetHeightMoveTopCenterBeyondBottomAngle0);
    QPointF ptLeftCenterSetHeightMoveTopCenterBeyondBottomAngle0(fLeftSetHeightMoveTopCenterBeyondBottomAngle0, fyCenterSetHeightMoveTopCenterBeyondBottomAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterBeyondBottomAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterBeyondBottomAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterBeyondBottomAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterBeyondBottomAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterBeyondBottomAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterBeyondBottomAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterBeyondBottomAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterBeyondBottomAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterBeyondBottomAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterBeyondBottomAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterBeyondBottomAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterBeyondBottomAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterBeyondBottomAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterBeyondBottomAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterBeyondBottomAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterBeyondBottomAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterBeyondBottomAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterBeyondBottomAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterBeyondBottomAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterBeyondBottomAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterBeyondBottomAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterBeyondBottomAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingBottomCenter(50)
    ------------------------------------------
        200   250   300   350   400          200   250   300   350   400   BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+-----+-----+      350
         |                       |   =>       |           X           |
     300 +           X           +        300 +-----+-----+-----+-----+      300
         |                       |
     350 +-----+-----+-----+-----+
    */
    ptMoved = ptRightCenterSetAngle0;
    double fxCenterSetHeightMoveBottomCenterAngle0 = fxCenterSetAngle0;
    double fyCenterSetHeightMoveBottomCenterAngle0 = fTopSetAngle0 + (ptMoved.y() - fTopSetAngle0) / 2.0;
    double fWidthSetHeightMoveBottomCenterAngle0 = fWidthSetAngle0;
    double fHeightSetHeightMoveBottomCenterAngle0 = fabs(ptMoved.y() - fTopSetAngle0);
    double fLeftSetHeightMoveBottomCenterAngle0 = fxCenterSetHeightMoveBottomCenterAngle0 - fWidthSetHeightMoveBottomCenterAngle0 / 2.0;
    double fRightSetHeightMoveBottomCenterAngle0 = fxCenterSetHeightMoveBottomCenterAngle0 + fWidthSetHeightMoveBottomCenterAngle0 / 2.0;
    double fTopSetHeightMoveBottomCenterAngle0 = fyCenterSetHeightMoveBottomCenterAngle0 - fHeightSetHeightMoveBottomCenterAngle0 / 2.0;
    double fBottomSetHeightMoveBottomCenterAngle0 = fyCenterSetHeightMoveBottomCenterAngle0 + fHeightSetHeightMoveBottomCenterAngle0 / 2.0;
    QPointF ptCenterSetHeightMoveBottomCenterAngle0(fxCenterSetHeightMoveBottomCenterAngle0, fyCenterSetHeightMoveBottomCenterAngle0);
    QPointF ptTopLeftSetHeightMoveBottomCenterAngle0(fLeftSetHeightMoveBottomCenterAngle0, fTopSetHeightMoveBottomCenterAngle0);
    QPointF ptTopRightSetHeightMoveBottomCenterAngle0(fRightSetHeightMoveBottomCenterAngle0, fTopSetHeightMoveBottomCenterAngle0);
    QPointF ptBottomRightSetHeightMoveBottomCenterAngle0(fRightSetHeightMoveBottomCenterAngle0, fBottomSetHeightMoveBottomCenterAngle0);
    QPointF ptBottomLeftSetHeightMoveBottomCenterAngle0(fLeftSetHeightMoveBottomCenterAngle0, fBottomSetHeightMoveBottomCenterAngle0);
    QPointF ptTopCenterSetHeightMoveBottomCenterAngle0(fxCenterSetHeightMoveBottomCenterAngle0, fTopSetHeightMoveBottomCenterAngle0);
    QPointF ptRightCenterSetHeightMoveBottomCenterAngle0(fRightSetHeightMoveBottomCenterAngle0, fyCenterSetHeightMoveBottomCenterAngle0);
    QPointF ptBottomCenterSetHeightMoveBottomCenterAngle0(fxCenterSetHeightMoveBottomCenterAngle0, fBottomSetHeightMoveBottomCenterAngle0);
    QPointF ptLeftCenterSetHeightMoveBottomCenterAngle0(fLeftSetHeightMoveBottomCenterAngle0, fyCenterSetHeightMoveBottomCenterAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenterAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenterAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenterAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenterAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenterAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingBottomCenter(400, 200)
    ------------------------------------------


        200   250   300   350   400          200   250   300   350   400   BottomUp
                                          200 BR-----+-----+-----+----BL      400
                                              |           X           |
     250 +-----+-----+-----+-----+        250 TR-----+-----+-----+----TL      350
         |                       |
     300 +           X           +                                           300
         |                       |
     350 +-----+-----+-----+-----+
    */
    ptMoved = QPointF(400.0, 200.0);
    physValAngleCorrected.setVal(180.0);
    double fxCenterSetHeightMoveBottomCenterBeyondTopAngle0 = fxCenterSetAngle0;
    double fyCenterSetHeightMoveBottomCenterBeyondTopAngle0 = fTopSetAngle0 + (ptMoved.y() - fTopSetAngle0) / 2.0;
    double fWidthSetHeightMoveBottomCenterBeyondTopAngle0 = fWidthSetAngle0;
    double fHeightSetHeightMoveBottomCenterBeyondTopAngle0 = fabs(ptMoved.y() - fTopSetAngle0);
    double fLeftSetHeightMoveBottomCenterBeyondTopAngle0 = fxCenterSetHeightMoveBottomCenterBeyondTopAngle0 + fWidthSetHeightMoveBottomCenterBeyondTopAngle0 / 2.0;
    double fRightSetHeightMoveBottomCenterBeyondTopAngle0 = fxCenterSetHeightMoveBottomCenterBeyondTopAngle0 - fWidthSetHeightMoveBottomCenterBeyondTopAngle0 / 2.0;
    double fTopSetHeightMoveBottomCenterBeyondTopAngle0 = fyCenterSetHeightMoveBottomCenterBeyondTopAngle0 + fHeightSetHeightMoveBottomCenterBeyondTopAngle0 / 2.0;
    double fBottomSetHeightMoveBottomCenterBeyondTopAngle0 = fyCenterSetHeightMoveBottomCenterBeyondTopAngle0 - fHeightSetHeightMoveBottomCenterBeyondTopAngle0 / 2.0;
    QPointF ptCenterSetHeightMoveBottomCenterBeyondTopAngle0(fxCenterSetHeightMoveBottomCenterBeyondTopAngle0, fyCenterSetHeightMoveBottomCenterBeyondTopAngle0);
    QPointF ptTopLeftSetHeightMoveBottomCenterBeyondTopAngle0(fLeftSetHeightMoveBottomCenterBeyondTopAngle0, fTopSetHeightMoveBottomCenterBeyondTopAngle0);
    QPointF ptTopRightSetHeightMoveBottomCenterBeyondTopAngle0(fRightSetHeightMoveBottomCenterBeyondTopAngle0, fTopSetHeightMoveBottomCenterBeyondTopAngle0);
    QPointF ptBottomRightSetHeightMoveBottomCenterBeyondTopAngle0(fRightSetHeightMoveBottomCenterBeyondTopAngle0, fBottomSetHeightMoveBottomCenterBeyondTopAngle0);
    QPointF ptBottomLeftSetHeightMoveBottomCenterBeyondTopAngle0(fLeftSetHeightMoveBottomCenterBeyondTopAngle0, fBottomSetHeightMoveBottomCenterBeyondTopAngle0);
    QPointF ptTopCenterSetHeightMoveBottomCenterBeyondTopAngle0(fxCenterSetHeightMoveBottomCenterBeyondTopAngle0, fTopSetHeightMoveBottomCenterBeyondTopAngle0);
    QPointF ptRightCenterSetHeightMoveBottomCenterBeyondTopAngle0(fRightSetHeightMoveBottomCenterBeyondTopAngle0, fyCenterSetHeightMoveBottomCenterBeyondTopAngle0);
    QPointF ptBottomCenterSetHeightMoveBottomCenterBeyondTopAngle0(fxCenterSetHeightMoveBottomCenterBeyondTopAngle0, fBottomSetHeightMoveBottomCenterBeyondTopAngle0);
    QPointF ptLeftCenterSetHeightMoveBottomCenterBeyondTopAngle0(fLeftSetHeightMoveBottomCenterBeyondTopAngle0, fyCenterSetHeightMoveBottomCenterBeyondTopAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenterBeyondTopAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenterBeyondTopAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenterBeyondTopAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenterBeyondTopAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenterBeyondTopAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenterBeyondTopAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenterBeyondTopAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenterBeyondTopAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenterBeyondTopAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenterBeyondTopAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenterBeyondTopAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenterBeyondTopAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenterBeyondTopAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenterBeyondTopAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenterBeyondTopAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenterBeyondTopAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenterBeyondTopAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenterBeyondTopAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenterBeyondTopAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenterBeyondTopAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenterBeyondTopAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenterBeyondTopAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(300, 200)
    -----------------------
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
    QPointF ptTopLeftAngle0(300.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopLeftAngle0.setY(400.0);
    }
    double fxCenterSetTopLeftAngle0 = ptTopLeftAngle0.x() + (fRightSetAngle0 - ptTopLeftAngle0.x()) / 2.0;
    double fyCenterSetTopLeftAngle0 = ptTopLeftAngle0.y() + (fBottomSetAngle0 - ptTopLeftAngle0.y()) / 2.0;
    double fWidthSetTopLeftAngle0 = fabs(fRightSetAngle0 - ptTopLeftAngle0.x());
    double fHeightSetTopLeftAngle0 = fabs(fBottomSetAngle0 - ptTopLeftAngle0.y());
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
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(" + qPoint2Str(ptTopLeftAngle0) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptTopLeftAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptTopLeftAngle0);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftAngle0, 1), 'f', 1) + " mm");
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

    /* setTopLeft(500, 300)
    -----------------------
            200   250   300   350   400   450   500
    TopDown                                            BottomUp
      200                                                400

      250    TL---------------------TR                   350
             |                       |
      300    |           X          BL'---------TL'      300
             |                       |     X'    |
      350    BL---------------------BR----------TR'      250
    */
    QPointF ptSetTopLeftBeyondRightBorderAngle0(500.0, 300.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptSetTopLeftBeyondRightBorderAngle0.setY(300.0);
    }
    physValAngleCorrected.setVal(90.0);
    double fxCenterSetTopLeftBeyondRightBorderAngle0 = ptSetTopLeftBeyondRightBorderAngle0.x() + (fRightSetAngle0 - ptSetTopLeftBeyondRightBorderAngle0.x()) / 2.0;
    double fyCenterSetTopLeftBeyondRightBorderAngle0 = ptSetTopLeftBeyondRightBorderAngle0.y() + (fBottomSetAngle0 - ptSetTopLeftBeyondRightBorderAngle0.y()) / 2.0;
    double fWidthSetTopLeftBeyondRightBorderAngle0 = fabs(fRightSetAngle0 - ptSetTopLeftBeyondRightBorderAngle0.x());
    double fHeightSetTopLeftBeyondRightBorderAngle0 = fabs(fBottomSetAngle0 - ptSetTopLeftBeyondRightBorderAngle0.y());
    double fLeftSetTopLeftBeyondRightBorderAngle0 = fxCenterSetTopLeftBeyondRightBorderAngle0 - fWidthSetTopLeftBeyondRightBorderAngle0 / 2.0;
    double fRightSetTopLeftBeyondRightBorderAngle0 = fxCenterSetTopLeftBeyondRightBorderAngle0 + fWidthSetTopLeftBeyondRightBorderAngle0 / 2.0;
    double fTopSetTopLeftBeyondRightBorderAngle0 = fyCenterSetTopLeftBeyondRightBorderAngle0 - fHeightSetTopLeftBeyondRightBorderAngle0 / 2.0;
    double fBottomSetTopLeftBeyondRightBorderAngle0 = fyCenterSetTopLeftBeyondRightBorderAngle0 + fHeightSetTopLeftBeyondRightBorderAngle0 / 2.0;
    QPointF ptCenterSetTopLeftBeyondRightBorderAngle0(fxCenterSetTopLeftBeyondRightBorderAngle0, fyCenterSetTopLeftBeyondRightBorderAngle0);
    QPointF ptTopLeftSetTopLeftBeyondRightBorderAngle0(fLeftSetTopLeftBeyondRightBorderAngle0, fTopSetTopLeftBeyondRightBorderAngle0);
    QPointF ptTopRightSetTopLeftBeyondRightBorderAngle0(fRightSetTopLeftBeyondRightBorderAngle0, fTopSetTopLeftBeyondRightBorderAngle0);
    QPointF ptBottomRightSetTopLeftBeyondRightBorderAngle0(fRightSetTopLeftBeyondRightBorderAngle0, fBottomSetTopLeftBeyondRightBorderAngle0);
    QPointF ptBottomLeftSetTopLeftBeyondRightBorderAngle0(fLeftSetTopLeftBeyondRightBorderAngle0, fBottomSetTopLeftBeyondRightBorderAngle0);
    QPointF ptTopCenterSetTopLeftBeyondRightBorderAngle0(fxCenterSetTopLeftBeyondRightBorderAngle0, fTopSetTopLeftBeyondRightBorderAngle0);
    QPointF ptRightCenterSetTopLeftBeyondRightBorderAngle0(fRightSetTopLeftBeyondRightBorderAngle0, fyCenterSetTopLeftBeyondRightBorderAngle0);
    QPointF ptBottomCenterSetTopLeftBeyondRightBorderAngle0(fxCenterSetTopLeftBeyondRightBorderAngle0, fBottomSetTopLeftBeyondRightBorderAngle0);
    QPointF ptLeftCenterSetTopLeftBeyondRightBorderAngle0(fLeftSetTopLeftBeyondRightBorderAngle0, fyCenterSetTopLeftBeyondRightBorderAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(" + qPoint2Str(ptSetTopLeftBeyondRightBorderAngle0) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptSetTopLeftBeyondRightBorderAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptSetTopLeftBeyondRightBorderAngle0);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondRightBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondRightBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondRightBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondRightBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondRightBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondRightBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondRightBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondRightBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondRightBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondRightBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondRightBorderAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondRightBorderAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondRightBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondRightBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondRightBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondRightBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondRightBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondRightBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondRightBorderAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondRightBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondRightBorderAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondRightBorderAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(300, 400)
    -----------------------
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
    QPointF ptSetTopLeftBeyondBottomBorderAngle0(300.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptSetTopLeftBeyondBottomBorderAngle0.setY(300.0);
    }
    physValAngleCorrected.setVal(270.0);
    double fxCenterSetTopLeftBeyondBottomBorderAngle0 = ptSetTopLeftBeyondBottomBorderAngle0.x() + (fRightSetAngle0 - ptSetTopLeftBeyondBottomBorderAngle0.x()) / 2.0;
    double fyCenterSetTopLeftBeyondBottomBorderAngle0 = ptSetTopLeftBeyondBottomBorderAngle0.y() + (fBottomSetAngle0 - ptSetTopLeftBeyondBottomBorderAngle0.y()) / 2.0;
    double fWidthSetTopLeftBeyondBottomBorderAngle0 = fabs(fRightSetAngle0 - ptSetTopLeftBeyondBottomBorderAngle0.x());
    double fHeightSetTopLeftBeyondBottomBorderAngle0 = fabs(fBottomSetAngle0 - ptSetTopLeftBeyondBottomBorderAngle0.y());
    double fLeftSetTopLeftBeyondBottomBorderAngle0 = fxCenterSetTopLeftBeyondBottomBorderAngle0 - fWidthSetTopLeftBeyondBottomBorderAngle0 / 2.0;
    double fRightSetTopLeftBeyondBottomBorderAngle0 = fxCenterSetTopLeftBeyondBottomBorderAngle0 + fWidthSetTopLeftBeyondBottomBorderAngle0 / 2.0;
    double fTopSetTopLeftBeyondBottomBorderAngle0 = fyCenterSetTopLeftBeyondBottomBorderAngle0 - fHeightSetTopLeftBeyondBottomBorderAngle0 / 2.0;
    double fBottomSetTopLeftBeyondBottomBorderAngle0 = fyCenterSetTopLeftBeyondBottomBorderAngle0 + fHeightSetTopLeftBeyondBottomBorderAngle0 / 2.0;
    QPointF ptCenterSetTopLeftBeyondBottomBorderAngle0(fxCenterSetTopLeftBeyondBottomBorderAngle0, fyCenterSetTopLeftBeyondBottomBorderAngle0);
    QPointF ptTopLeftSetTopLeftBeyondBottomBorderAngle0(fLeftSetTopLeftBeyondBottomBorderAngle0, fTopSetTopLeftBeyondBottomBorderAngle0);
    QPointF ptTopRightSetTopLeftBeyondBottomBorderAngle0(fRightSetTopLeftBeyondBottomBorderAngle0, fTopSetTopLeftBeyondBottomBorderAngle0);
    QPointF ptBottomRightSetTopLeftBeyondBottomBorderAngle0(fRightSetTopLeftBeyondBottomBorderAngle0, fBottomSetTopLeftBeyondBottomBorderAngle0);
    QPointF ptBottomLeftSetTopLeftBeyondBottomBorderAngle0(fLeftSetTopLeftBeyondBottomBorderAngle0, fBottomSetTopLeftBeyondBottomBorderAngle0);
    QPointF ptTopCenterSetTopLeftBeyondBottomBorderAngle0(fxCenterSetTopLeftBeyondBottomBorderAngle0, fTopSetTopLeftBeyondBottomBorderAngle0);
    QPointF ptRightCenterSetTopLeftBeyondBottomBorderAngle0(fRightSetTopLeftBeyondBottomBorderAngle0, fyCenterSetTopLeftBeyondBottomBorderAngle0);
    QPointF ptBottomCenterSetTopLeftBeyondBottomBorderAngle0(fxCenterSetTopLeftBeyondBottomBorderAngle0, fBottomSetTopLeftBeyondBottomBorderAngle0);
    QPointF ptLeftCenterSetTopLeftBeyondBottomBorderAngle0(fLeftSetTopLeftBeyondBottomBorderAngle0, fyCenterSetTopLeftBeyondBottomBorderAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(" + qPoint2Str(ptSetTopLeftBeyondBottomBorderAngle0) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptSetTopLeftBeyondBottomBorderAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptSetTopLeftBeyondBottomBorderAngle0);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondBottomBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondBottomBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondBottomBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondBottomBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondBottomBorderAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(500, 450)
    -----------------------
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
    QPointF ptTopLeftBeyondBottomRightAngle0(500.0, 450.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopLeftBeyondBottomRightAngle0.setY(150.0);
    }
    physValAngleCorrected.setVal(180.0);
    double fxCenterSetTopLeftBeyondBottomRightAngle0 = ptTopLeftBeyondBottomRightAngle0.x() + (fRightSetAngle0 - ptTopLeftBeyondBottomRightAngle0.x()) / 2.0;
    double fyCenterSetTopLeftBeyondBottomRightAngle0 = ptTopLeftBeyondBottomRightAngle0.y() + (fBottomSetAngle0 - ptTopLeftBeyondBottomRightAngle0.y()) / 2.0;
    double fWidthSetTopLeftBeyondBottomRightAngle0 = fabs(fRightSetAngle0 - ptTopLeftBeyondBottomRightAngle0.x());
    double fHeightSetTopLeftBeyondBottomRightAngle0 = fabs(fBottomSetAngle0 - ptTopLeftBeyondBottomRightAngle0.y());
    double fLeftSetTopLeftBeyondBottomRightAngle0 = fxCenterSetTopLeftBeyondBottomRightAngle0 + fWidthSetTopLeftBeyondBottomRightAngle0 / 2.0;
    double fRightSetTopLeftBeyondBottomRightAngle0 = fxCenterSetTopLeftBeyondBottomRightAngle0 - fWidthSetTopLeftBeyondBottomRightAngle0 / 2.0;
    double fTopSetTopLeftBeyondBottomRightAngle0 = fyCenterSetTopLeftBeyondBottomRightAngle0 + fHeightSetTopLeftBeyondBottomRightAngle0 / 2.0;
    double fBottomSetTopLeftBeyondBottomRightAngle0 = fyCenterSetTopLeftBeyondBottomRightAngle0 - fHeightSetTopLeftBeyondBottomRightAngle0 / 2.0;
    QPointF ptCenterSetTopLeftBeyondBottomRightAngle0(fxCenterSetTopLeftBeyondBottomRightAngle0, fyCenterSetTopLeftBeyondBottomRightAngle0);
    QPointF ptTopLeftSetTopLeftBeyondBottomRightAngle0(fLeftSetTopLeftBeyondBottomRightAngle0, fTopSetTopLeftBeyondBottomRightAngle0);
    QPointF ptTopRightSetTopLeftBeyondBottomRightAngle0(fRightSetTopLeftBeyondBottomRightAngle0, fTopSetTopLeftBeyondBottomRightAngle0);
    QPointF ptBottomRightSetTopLeftBeyondBottomRightAngle0(fRightSetTopLeftBeyondBottomRightAngle0, fBottomSetTopLeftBeyondBottomRightAngle0);
    QPointF ptBottomLeftSetTopLeftBeyondBottomRightAngle0(fLeftSetTopLeftBeyondBottomRightAngle0, fBottomSetTopLeftBeyondBottomRightAngle0);
    QPointF ptTopCenterSetTopLeftBeyondBottomRightAngle0(fxCenterSetTopLeftBeyondBottomRightAngle0, fTopSetTopLeftBeyondBottomRightAngle0);
    QPointF ptRightCenterSetTopLeftBeyondBottomRightAngle0(fRightSetTopLeftBeyondBottomRightAngle0, fyCenterSetTopLeftBeyondBottomRightAngle0);
    QPointF ptBottomCenterSetTopLeftBeyondBottomRightAngle0(fxCenterSetTopLeftBeyondBottomRightAngle0, fBottomSetTopLeftBeyondBottomRightAngle0);
    QPointF ptLeftCenterSetTopLeftBeyondBottomRightAngle0(fLeftSetTopLeftBeyondBottomRightAngle0, fyCenterSetTopLeftBeyondBottomRightAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(" + qPoint2Str(ptTopLeftBeyondBottomRightAngle0) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptTopLeftBeyondBottomRightAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptTopLeftBeyondBottomRightAngle0);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondBottomRightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondBottomRightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondBottomRightAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondBottomRightAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondBottomRightAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondBottomRightAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(300, 200)
    ------------------------
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
    QPointF ptTopRightAngle0(300.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopRightAngle0.setY(400.0);
    }
    double fxCenterSetTopRightAngle0 = fLeftSetAngle0 + (ptTopRightAngle0.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetTopRightAngle0 = ptTopRightAngle0.y() + (fBottomSetAngle0 - ptTopRightAngle0.y()) / 2.0;
    double fWidthSetTopRightAngle0 = fabs(ptTopRightAngle0.x() - fLeftSetAngle0);
    double fHeightSetTopRightAngle0 = fabs(fBottomSetAngle0 - ptTopRightAngle0.y());
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
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(" + qPoint2Str(ptTopRightAngle0) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptTopRightAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptTopRightAngle0);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightAngle0, 1), 'f', 1) + " mm");
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

    /* setTopRight(100, 350)
    ------------------------
             100   150  200   250   300   350   400
    TopDown                                            BottomUp
     250                TL-----------------------TR      350
                         |                       |
     300    TR'---------BR'          X           |       300
             |     X'    |                       |
     350    TL'----------BL----------------------BR      250
    */
    QPointF ptTopRightBeyondLeftBorderAngle0(100.0, 350.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopRightBeyondLeftBorderAngle0.setY(250.0);
    }
    physValAngleCorrected.setVal(270.0);
    double fxCenterSetTopRightBeyondLeftBorderAngle0 = fLeftSetAngle0 + (ptTopRightBeyondLeftBorderAngle0.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetTopRightBeyondLeftBorderAngle0 = ptTopRightBeyondLeftBorderAngle0.y() + (fBottomSetAngle0 - ptTopRightBeyondLeftBorderAngle0.y()) / 2.0;
    double fWidthSetTopRightBeyondLeftBorderAngle0 = fabs(ptTopRightBeyondLeftBorderAngle0.x() - fLeftSetAngle0);
    double fHeightSetTopRightBeyondLeftBorderAngle0 = fabs(fBottomSetAngle0 - ptTopRightBeyondLeftBorderAngle0.y());
    double fLeftSetTopRightBeyondLeftBorderAngle0 = fxCenterSetTopRightBeyondLeftBorderAngle0 + fWidthSetTopRightBeyondLeftBorderAngle0 / 2.0;
    double fRightSetTopRightBeyondLeftBorderAngle0 = fxCenterSetTopRightBeyondLeftBorderAngle0 - fWidthSetTopRightBeyondLeftBorderAngle0 / 2.0;
    double fTopSetTopRightBeyondLeftBorderAngle0 = fyCenterSetTopRightBeyondLeftBorderAngle0 + fHeightSetTopRightBeyondLeftBorderAngle0 / 2.0;
    double fBottomSetTopRightBeyondLeftBorderAngle0 = fyCenterSetTopRightBeyondLeftBorderAngle0 - fHeightSetTopRightBeyondLeftBorderAngle0 / 2.0;
    QPointF ptCenterSetTopRightBeyondLeftBorderAngle0(fxCenterSetTopRightBeyondLeftBorderAngle0, fyCenterSetTopRightBeyondLeftBorderAngle0);
    QPointF ptTopLeftSetTopRightBeyondLeftBorderAngle0(fLeftSetTopRightBeyondLeftBorderAngle0, fTopSetTopRightBeyondLeftBorderAngle0);
    QPointF ptTopRightSetTopRightBeyondLeftBorderAngle0(fRightSetTopRightBeyondLeftBorderAngle0, fTopSetTopRightBeyondLeftBorderAngle0);
    QPointF ptBottomRightSetTopRightBeyondLeftBorderAngle0(fRightSetTopRightBeyondLeftBorderAngle0, fBottomSetTopRightBeyondLeftBorderAngle0);
    QPointF ptBottomLeftSetTopRightBeyondLeftBorderAngle0(fLeftSetTopRightBeyondLeftBorderAngle0, fBottomSetTopRightBeyondLeftBorderAngle0);
    QPointF ptTopCenterSetTopRightBeyondLeftBorderAngle0(fxCenterSetTopRightBeyondLeftBorderAngle0, fTopSetTopRightBeyondLeftBorderAngle0);
    QPointF ptRightCenterSetTopRightBeyondLeftBorderAngle0(fRightSetTopRightBeyondLeftBorderAngle0, fyCenterSetTopRightBeyondLeftBorderAngle0);
    QPointF ptBottomCenterSetTopRightBeyondLeftBorderAngle0(fxCenterSetTopRightBeyondLeftBorderAngle0, fBottomSetTopRightBeyondLeftBorderAngle0);
    QPointF ptLeftCenterSetTopRightBeyondLeftBorderAngle0(fLeftSetTopRightBeyondLeftBorderAngle0, fyCenterSetTopRightBeyondLeftBorderAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(" + qPoint2Str(ptTopRightBeyondLeftBorderAngle0) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptTopRightBeyondLeftBorderAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptTopRightBeyondLeftBorderAngle0);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondLeftBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondLeftBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondLeftBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondLeftBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondLeftBorderAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(300, 400)
    ------------------------
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
    QPointF ptTopRightBeyondBottomBorderAngle0(300.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopRightBeyondBottomBorderAngle0.setY(200.0);
    }
    physValAngleCorrected.setVal(90.0);
    double fxCenterSetTopRightBeyondBottomBorderAngle0 = fLeftSetAngle0 + (ptTopRightBeyondBottomBorderAngle0.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetTopRightBeyondBottomBorderAngle0 = ptTopRightBeyondBottomBorderAngle0.y() + (fBottomSetAngle0 - ptTopRightBeyondBottomBorderAngle0.y()) / 2.0;
    double fWidthSetTopRightBeyondBottomBorderAngle0 = fabs(ptTopRightBeyondBottomBorderAngle0.x() - fLeftSetAngle0);
    double fHeightSetTopRightBeyondBottomBorderAngle0 = fabs(fBottomSetAngle0 - ptTopRightBeyondBottomBorderAngle0.y());
    double fLeftSetTopRightBeyondBottomBorderAngle0 = fxCenterSetTopRightBeyondBottomBorderAngle0 + fWidthSetTopRightBeyondBottomBorderAngle0 / 2.0;
    double fRightSetTopRightBeyondBottomBorderAngle0 = fxCenterSetTopRightBeyondBottomBorderAngle0 - fWidthSetTopRightBeyondBottomBorderAngle0 / 2.0;
    double fTopSetTopRightBeyondBottomBorderAngle0 = fyCenterSetTopRightBeyondBottomBorderAngle0 + fHeightSetTopRightBeyondBottomBorderAngle0 / 2.0;
    double fBottomSetTopRightBeyondBottomBorderAngle0 = fyCenterSetTopRightBeyondBottomBorderAngle0 - fHeightSetTopRightBeyondBottomBorderAngle0 / 2.0;
    QPointF ptCenterSetTopRightBeyondBottomBorderAngle0(fxCenterSetTopRightBeyondBottomBorderAngle0, fyCenterSetTopRightBeyondBottomBorderAngle0);
    QPointF ptTopLeftSetTopRightBeyondBottomBorderAngle0(fLeftSetTopRightBeyondBottomBorderAngle0, fTopSetTopRightBeyondBottomBorderAngle0);
    QPointF ptTopRightSetTopRightBeyondBottomBorderAngle0(fRightSetTopRightBeyondBottomBorderAngle0, fTopSetTopRightBeyondBottomBorderAngle0);
    QPointF ptBottomRightSetTopRightBeyondBottomBorderAngle0(fRightSetTopRightBeyondBottomBorderAngle0, fBottomSetTopRightBeyondBottomBorderAngle0);
    QPointF ptBottomLeftSetTopRightBeyondBottomBorderAngle0(fLeftSetTopRightBeyondBottomBorderAngle0, fBottomSetTopRightBeyondBottomBorderAngle0);
    QPointF ptTopCenterSetTopRightBeyondBottomBorderAngle0(fxCenterSetTopRightBeyondBottomBorderAngle0, fTopSetTopRightBeyondBottomBorderAngle0);
    QPointF ptRightCenterSetTopRightBeyondBottomBorderAngle0(fRightSetTopRightBeyondBottomBorderAngle0, fyCenterSetTopRightBeyondBottomBorderAngle0);
    QPointF ptBottomCenterSetTopRightBeyondBottomBorderAngle0(fxCenterSetTopRightBeyondBottomBorderAngle0, fBottomSetTopRightBeyondBottomBorderAngle0);
    QPointF ptLeftCenterSetTopRightBeyondBottomBorderAngle0(fLeftSetTopRightBeyondBottomBorderAngle0, fyCenterSetTopRightBeyondBottomBorderAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(" + qPoint2Str(ptTopRightBeyondBottomBorderAngle0) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptTopRightBeyondBottomBorderAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptTopRightBeyondBottomBorderAngle0);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondBottomBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondBottomBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondBottomBorderAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondBottomBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondBottomBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondBottomBorderAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondBottomBorderAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(100, 450)
    ------------------------
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
    QPointF ptTopRightBeyondBottomLeftAngle0(100.0, 450.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopRightBeyondBottomLeftAngle0.setY(150.0);
    }
    physValAngleCorrected.setVal(180.0);
    double fxCenterSetTopRightBeyondBottomLeftAngle0 = fLeftSetAngle0 + (ptTopRightBeyondBottomLeftAngle0.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetTopRightBeyondBottomLeftAngle0 = ptTopRightBeyondBottomLeftAngle0.y() + (fBottomSetAngle0 - ptTopRightBeyondBottomLeftAngle0.y()) / 2.0;
    double fWidthSetTopRightBeyondBottomLeftAngle0 = fabs(ptTopRightBeyondBottomLeftAngle0.x() - fLeftSetAngle0);
    double fHeightSetTopRightBeyondBottomLeftAngle0 = fabs(fBottomSetAngle0 - ptTopRightBeyondBottomLeftAngle0.y());
    double fLeftSetTopRightBeyondBottomLeftAngle0 = fxCenterSetTopRightBeyondBottomLeftAngle0 + fWidthSetTopRightBeyondBottomLeftAngle0 / 2.0;
    double fRightSetTopRightBeyondBottomLeftAngle0 = fxCenterSetTopRightBeyondBottomLeftAngle0 - fWidthSetTopRightBeyondBottomLeftAngle0 / 2.0;
    double fTopSetTopRightBeyondBottomLeftAngle0 = fyCenterSetTopRightBeyondBottomLeftAngle0 + fHeightSetTopRightBeyondBottomLeftAngle0 / 2.0;
    double fBottomSetTopRightBeyondBottomLeftAngle0 = fyCenterSetTopRightBeyondBottomLeftAngle0 - fHeightSetTopRightBeyondBottomLeftAngle0 / 2.0;
    QPointF ptCenterSetTopRightBeyondBottomLeftAngle0(fxCenterSetTopRightBeyondBottomLeftAngle0, fyCenterSetTopRightBeyondBottomLeftAngle0);
    QPointF ptTopLeftSetTopRightBeyondBottomLeftAngle0(fLeftSetTopRightBeyondBottomLeftAngle0, fTopSetTopRightBeyondBottomLeftAngle0);
    QPointF ptTopRightSetTopRightBeyondBottomLeftAngle0(fRightSetTopRightBeyondBottomLeftAngle0, fTopSetTopRightBeyondBottomLeftAngle0);
    QPointF ptBottomRightSetTopRightBeyondBottomLeftAngle0(fRightSetTopRightBeyondBottomLeftAngle0, fBottomSetTopRightBeyondBottomLeftAngle0);
    QPointF ptBottomLeftSetTopRightBeyondBottomLeftAngle0(fLeftSetTopRightBeyondBottomLeftAngle0, fBottomSetTopRightBeyondBottomLeftAngle0);
    QPointF ptTopCenterSetTopRightBeyondBottomLeftAngle0(fxCenterSetTopRightBeyondBottomLeftAngle0, fTopSetTopRightBeyondBottomLeftAngle0);
    QPointF ptRightCenterSetTopRightBeyondBottomLeftAngle0(fRightSetTopRightBeyondBottomLeftAngle0, fyCenterSetTopRightBeyondBottomLeftAngle0);
    QPointF ptBottomCenterSetTopRightBeyondBottomLeftAngle0(fxCenterSetTopRightBeyondBottomLeftAngle0, fBottomSetTopRightBeyondBottomLeftAngle0);
    QPointF ptLeftCenterSetTopRightBeyondBottomLeftAngle0(fLeftSetTopRightBeyondBottomLeftAngle0, fyCenterSetTopRightBeyondBottomLeftAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(" + qPoint2Str(ptTopRightBeyondBottomLeftAngle0) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptTopRightBeyondBottomLeftAngle0) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptTopRightBeyondBottomLeftAngle0);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondBottomLeftAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondBottomLeftAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondBottomLeftAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondBottomLeftAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondBottomLeftAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondBottomLeftAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight(300, 400)
    ---------------------------
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
    QPointF ptBottomRight(300.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomRight.setY(200.0);
    }
    double fxCenterSetBottomRightAngle0 = fLeftSetAngle0 + (ptBottomRight.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetBottomRightAngle0 = fTopSetAngle0 + (ptBottomRight.y() - fTopSetAngle0) / 2.0;
    double fWidthSetBottomRightAngle0 = fabs(ptBottomRight.x() - fLeftSetAngle0);
    double fHeightSetBottomRightAngle0 = fabs(ptBottomRight.y() - fTopSetAngle0);
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
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptBottomRight);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightAngle0, 1), 'f', 1) + " mm");
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

    /* setBottomRight(300, 200)
    ---------------------------
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
    QPointF ptBottomRightBeyondTopBorder(300.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomRightBeyondTopBorder.setY(400.0);
    }
    physValAngleCorrected.setVal(270.0);
    double fxCenterSetBottomRightBeyondTopBorderAngle0 = fLeftSetAngle0 + (ptBottomRightBeyondTopBorder.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetBottomRightBeyondTopBorderAngle0 = fTopSetAngle0 + (ptBottomRightBeyondTopBorder.y() - fTopSetAngle0) / 2.0;
    double fWidthSetBottomRightBeyondTopBorderAngle0 = fabs(ptBottomRightBeyondTopBorder.x() - fLeftSetAngle0);
    double fHeightSetBottomRightBeyondTopBorderAngle0 = fabs(ptBottomRightBeyondTopBorder.y() - fTopSetAngle0);
    double fLeftSetBottomRightBeyondTopBorderAngle0 = fxCenterSetBottomRightBeyondTopBorderAngle0 + fWidthSetBottomRightBeyondTopBorderAngle0 / 2.0;
    double fRightSetBottomRightBeyondTopBorderAngle0 = fxCenterSetBottomRightBeyondTopBorderAngle0 - fWidthSetBottomRightBeyondTopBorderAngle0 / 2.0;
    double fTopSetBottomRightBeyondTopBorderAngle0 = fyCenterSetBottomRightBeyondTopBorderAngle0 + fHeightSetBottomRightBeyondTopBorderAngle0 / 2.0;
    double fBottomSetBottomRightBeyondTopBorderAngle0 = fyCenterSetBottomRightBeyondTopBorderAngle0 - fHeightSetBottomRightBeyondTopBorderAngle0 / 2.0;
    QPointF ptCenterSetBottomRightBeyondTopBorderAngle0(fxCenterSetBottomRightBeyondTopBorderAngle0, fyCenterSetBottomRightBeyondTopBorderAngle0);
    QPointF ptTopLeftSetBottomRightBeyondTopBorderAngle0(fLeftSetBottomRightBeyondTopBorderAngle0, fTopSetBottomRightBeyondTopBorderAngle0);
    QPointF ptTopRightSetBottomRightBeyondTopBorderAngle0(fRightSetBottomRightBeyondTopBorderAngle0, fTopSetBottomRightBeyondTopBorderAngle0);
    QPointF ptBottomRightSetBottomRightBeyondTopBorderAngle0(fRightSetBottomRightBeyondTopBorderAngle0, fBottomSetBottomRightBeyondTopBorderAngle0);
    QPointF ptBottomLeftSetBottomRightBeyondTopBorderAngle0(fLeftSetBottomRightBeyondTopBorderAngle0, fBottomSetBottomRightBeyondTopBorderAngle0);
    QPointF ptTopCenterSetBottomRightBeyondTopBorderAngle0(fxCenterSetBottomRightBeyondTopBorderAngle0, fTopSetBottomRightBeyondTopBorderAngle0);
    QPointF ptRightCenterSetBottomRightBeyondTopBorderAngle0(fRightSetBottomRightBeyondTopBorderAngle0, fyCenterSetBottomRightBeyondTopBorderAngle0);
    QPointF ptBottomCenterSetBottomRightBeyondTopBorderAngle0(fxCenterSetBottomRightBeyondTopBorderAngle0, fBottomSetBottomRightBeyondTopBorderAngle0);
    QPointF ptLeftCenterSetBottomRightBeyondTopBorderAngle0(fLeftSetBottomRightBeyondTopBorderAngle0, fyCenterSetBottomRightBeyondTopBorderAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(" + qPoint2Str(ptBottomRightBeyondTopBorder) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptBottomRightBeyondTopBorder) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptBottomRightBeyondTopBorder);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondTopBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondTopBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondTopBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondTopBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondTopBorderAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight(100, 300)
    ---------------------------
             100   150  200   250   300   350   400
    TopDown                                           BottomUp
     250     BL'--------TL-----------------------TR      350
              |          |                       |
     300     BR'--------TR'          X           |       300
                         |                       |
     350                BL-----------------------BR      250
    */
    QPointF ptBottomRightBeyondLeftBorder(100.0, 300.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomRightBeyondLeftBorder.setY(300.0);
    }
    physValAngleCorrected.setVal(90.0);
    double fxCenterSetBottomRightBeyondLeftBorderAngle0 = fLeftSetAngle0 + (ptBottomRightBeyondLeftBorder.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetBottomRightBeyondLeftBorderAngle0 = fTopSetAngle0 + (ptBottomRightBeyondLeftBorder.y() - fTopSetAngle0) / 2.0;
    double fWidthSetBottomRightBeyondLeftBorderAngle0 = fabs(ptBottomRightBeyondLeftBorder.x() - fLeftSetAngle0);
    double fHeightSetBottomRightBeyondLeftBorderAngle0 = fabs(ptBottomRightBeyondLeftBorder.y() - fTopSetAngle0);
    double fLeftSetBottomRightBeyondLeftBorderAngle0 = fxCenterSetBottomRightBeyondLeftBorderAngle0 + fWidthSetBottomRightBeyondLeftBorderAngle0 / 2.0;
    double fRightSetBottomRightBeyondLeftBorderAngle0 = fxCenterSetBottomRightBeyondLeftBorderAngle0 - fWidthSetBottomRightBeyondLeftBorderAngle0 / 2.0;
    double fTopSetBottomRightBeyondLeftBorderAngle0 = fyCenterSetBottomRightBeyondLeftBorderAngle0 + fHeightSetBottomRightBeyondLeftBorderAngle0 / 2.0;
    double fBottomSetBottomRightBeyondLeftBorderAngle0 = fyCenterSetBottomRightBeyondLeftBorderAngle0 - fHeightSetBottomRightBeyondLeftBorderAngle0 / 2.0;
    QPointF ptCenterSetBottomRightBeyondLeftBorderAngle0(fxCenterSetBottomRightBeyondLeftBorderAngle0, fyCenterSetBottomRightBeyondLeftBorderAngle0);
    QPointF ptTopLeftSetBottomRightBeyondLeftBorderAngle0(fLeftSetBottomRightBeyondLeftBorderAngle0, fTopSetBottomRightBeyondLeftBorderAngle0);
    QPointF ptTopRightSetBottomRightBeyondLeftBorderAngle0(fRightSetBottomRightBeyondLeftBorderAngle0, fTopSetBottomRightBeyondLeftBorderAngle0);
    QPointF ptBottomRightSetBottomRightBeyondLeftBorderAngle0(fRightSetBottomRightBeyondLeftBorderAngle0, fBottomSetBottomRightBeyondLeftBorderAngle0);
    QPointF ptBottomLeftSetBottomRightBeyondLeftBorderAngle0(fLeftSetBottomRightBeyondLeftBorderAngle0, fBottomSetBottomRightBeyondLeftBorderAngle0);
    QPointF ptTopCenterSetBottomRightBeyondLeftBorderAngle0(fxCenterSetBottomRightBeyondLeftBorderAngle0, fTopSetBottomRightBeyondLeftBorderAngle0);
    QPointF ptRightCenterSetBottomRightBeyondLeftBorderAngle0(fRightSetBottomRightBeyondLeftBorderAngle0, fyCenterSetBottomRightBeyondLeftBorderAngle0);
    QPointF ptBottomCenterSetBottomRightBeyondLeftBorderAngle0(fxCenterSetBottomRightBeyondLeftBorderAngle0, fBottomSetBottomRightBeyondLeftBorderAngle0);
    QPointF ptLeftCenterSetBottomRightBeyondLeftBorderAngle0(fLeftSetBottomRightBeyondLeftBorderAngle0, fyCenterSetBottomRightBeyondLeftBorderAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(" + qPoint2Str(ptBottomRightBeyondLeftBorder) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptBottomRightBeyondLeftBorder) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptBottomRightBeyondLeftBorder);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondLeftBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondLeftBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondLeftBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondLeftBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondLeftBorderAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight(100, 150)
    ---------------------------
             100   150  200   250   300   350   400
    TopDown                                           BottomUp
     150     BR'--------BL'                              450
              |          |
     200      |     X'   |                               400
              |          |
     250     TR'--------TL-----------------------TR      350
                         |                       |
     300                 |           X           |       300
                         |                       |
     350                BL-----------------------BR      250
    */
    QPointF ptBottomRightBeyondTopLeft(100.0, 150.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomRightBeyondTopLeft.setY(450.0);
    }
    physValAngleCorrected.setVal(180.0);
    double fxCenterSetBottomRightBeyondTopLeftAngle0 = fLeftSetAngle0 + (ptBottomRightBeyondTopLeft.x() - fLeftSetAngle0) / 2.0;
    double fyCenterSetBottomRightBeyondTopLeftAngle0 = fTopSetAngle0 + (ptBottomRightBeyondTopLeft.y() - fTopSetAngle0) / 2.0;
    double fWidthSetBottomRightBeyondTopLeftAngle0 = fabs(ptBottomRightBeyondTopLeft.x() - fLeftSetAngle0);
    double fHeightSetBottomRightBeyondTopLeftAngle0 = fabs(ptBottomRightBeyondTopLeft.y() - fTopSetAngle0);
    double fLeftSetBottomRightBeyondTopLeftAngle0 = fxCenterSetBottomRightBeyondTopLeftAngle0 + fWidthSetBottomRightBeyondTopLeftAngle0 / 2.0;
    double fRightSetBottomRightBeyondTopLeftAngle0 = fxCenterSetBottomRightBeyondTopLeftAngle0 - fWidthSetBottomRightBeyondTopLeftAngle0 / 2.0;
    double fTopSetBottomRightBeyondTopLeftAngle0 = fyCenterSetBottomRightBeyondTopLeftAngle0 + fHeightSetBottomRightBeyondTopLeftAngle0 / 2.0;
    double fBottomSetBottomRightBeyondTopLeftAngle0 = fyCenterSetBottomRightBeyondTopLeftAngle0 - fHeightSetBottomRightBeyondTopLeftAngle0 / 2.0;
    QPointF ptCenterSetBottomRightBeyondTopLeftAngle0(fxCenterSetBottomRightBeyondTopLeftAngle0, fyCenterSetBottomRightBeyondTopLeftAngle0);
    QPointF ptTopLeftSetBottomRightBeyondTopLeftAngle0(fLeftSetBottomRightBeyondTopLeftAngle0, fTopSetBottomRightBeyondTopLeftAngle0);
    QPointF ptTopRightSetBottomRightBeyondTopLeftAngle0(fRightSetBottomRightBeyondTopLeftAngle0, fTopSetBottomRightBeyondTopLeftAngle0);
    QPointF ptBottomRightSetBottomRightBeyondTopLeftAngle0(fRightSetBottomRightBeyondTopLeftAngle0, fBottomSetBottomRightBeyondTopLeftAngle0);
    QPointF ptBottomLeftSetBottomRightBeyondTopLeftAngle0(fLeftSetBottomRightBeyondTopLeftAngle0, fBottomSetBottomRightBeyondTopLeftAngle0);
    QPointF ptTopCenterSetBottomRightBeyondTopLeftAngle0(fxCenterSetBottomRightBeyondTopLeftAngle0, fTopSetBottomRightBeyondTopLeftAngle0);
    QPointF ptRightCenterSetBottomRightBeyondTopLeftAngle0(fRightSetBottomRightBeyondTopLeftAngle0, fyCenterSetBottomRightBeyondTopLeftAngle0);
    QPointF ptBottomCenterSetBottomRightBeyondTopLeftAngle0(fxCenterSetBottomRightBeyondTopLeftAngle0, fBottomSetBottomRightBeyondTopLeftAngle0);
    QPointF ptLeftCenterSetBottomRightBeyondTopLeftAngle0(fLeftSetBottomRightBeyondTopLeftAngle0, fyCenterSetBottomRightBeyondTopLeftAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(" + qPoint2Str(ptBottomRightBeyondTopLeft) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptBottomRightBeyondTopLeft) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptBottomRightBeyondTopLeft);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondTopLeftAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondTopLeftAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondTopLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondTopLeftAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondTopLeftAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondTopLeftAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondTopLeftAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondTopLeftAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft(300, 400)
    --------------------------
            200   250   300   350   400
    TopDown                              BottomUp
      250   TL----------TL'---------TR     350
             |           |           |
      300    |           |     X     |     300
             |     X'    |           |
      350   BL-----------|----------BR     250
                         |           |
      400               BL'---------BR'    200
    */
    QPointF ptBottomLeft(300.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomLeft.setY(200.0);
    }
    double fxCenterSetBottomLeftAngle0 = ptBottomLeft.x() + (fRightSetAngle0 - ptBottomLeft.x()) / 2.0;
    double fyCenterSetBottomLeftAngle0 = fTopSetAngle0 + (ptBottomLeft.y() - fTopSetAngle0) / 2.0;
    double fWidthSetBottomLeftAngle0 = fabs(fRightSetAngle0 - ptBottomLeft.x());
    double fHeightSetBottomLeftAngle0 = fabs(ptBottomLeft.y() - fTopSetAngle0);
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
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptBottomLeft);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftAngle0, 1), 'f', 1) + " mm");
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

    /* setBottomLeft(300, 200)
    --------------------------
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
    QPointF ptBottomLeftBeyondTopBorder(300.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomLeftBeyondTopBorder.setY(400.0);
    }
    physValAngleCorrected.setVal(90.0);
    double fxCenterSetBottomLeftBeyondTopBorderAngle0 = ptBottomLeftBeyondTopBorder.x() + (fRightSetAngle0 - ptBottomLeftBeyondTopBorder.x()) / 2.0;
    double fyCenterSetBottomLeftBeyondTopBorderAngle0 = fTopSetAngle0 + (ptBottomLeftBeyondTopBorder.y() - fTopSetAngle0) / 2.0;
    double fWidthSetBottomLeftBeyondTopBorderAngle0 = fabs(fRightSetAngle0 - ptBottomLeftBeyondTopBorder.x());
    double fHeightSetBottomLeftBeyondTopBorderAngle0 = fabs(ptBottomLeftBeyondTopBorder.y() - fTopSetAngle0);
    double fLeftSetBottomLeftBeyondTopBorderAngle0 = fxCenterSetBottomLeftBeyondTopBorderAngle0 + fWidthSetBottomLeftBeyondTopBorderAngle0 / 2.0;
    double fRightSetBottomLeftBeyondTopBorderAngle0 = fxCenterSetBottomLeftBeyondTopBorderAngle0 - fWidthSetBottomLeftBeyondTopBorderAngle0 / 2.0;
    double fTopSetBottomLeftBeyondTopBorderAngle0 = fyCenterSetBottomLeftBeyondTopBorderAngle0 + fHeightSetBottomLeftBeyondTopBorderAngle0 / 2.0;
    double fBottomSetBottomLeftBeyondTopBorderAngle0 = fyCenterSetBottomLeftBeyondTopBorderAngle0 - fHeightSetBottomLeftBeyondTopBorderAngle0 / 2.0;
    QPointF ptCenterSetBottomLeftBeyondTopBorderAngle0(fxCenterSetBottomLeftBeyondTopBorderAngle0, fyCenterSetBottomLeftBeyondTopBorderAngle0);
    QPointF ptTopLeftSetBottomLeftBeyondTopBorderAngle0(fLeftSetBottomLeftBeyondTopBorderAngle0, fTopSetBottomLeftBeyondTopBorderAngle0);
    QPointF ptTopRightSetBottomLeftBeyondTopBorderAngle0(fRightSetBottomLeftBeyondTopBorderAngle0, fTopSetBottomLeftBeyondTopBorderAngle0);
    QPointF ptBottomRightSetBottomLeftBeyondTopBorderAngle0(fRightSetBottomLeftBeyondTopBorderAngle0, fBottomSetBottomLeftBeyondTopBorderAngle0);
    QPointF ptBottomLeftSetBottomLeftBeyondTopBorderAngle0(fLeftSetBottomLeftBeyondTopBorderAngle0, fBottomSetBottomLeftBeyondTopBorderAngle0);
    QPointF ptTopCenterSetBottomLeftBeyondTopBorderAngle0(fxCenterSetBottomLeftBeyondTopBorderAngle0, fTopSetBottomLeftBeyondTopBorderAngle0);
    QPointF ptRightCenterSetBottomLeftBeyondTopBorderAngle0(fRightSetBottomLeftBeyondTopBorderAngle0, fyCenterSetBottomLeftBeyondTopBorderAngle0);
    QPointF ptBottomCenterSetBottomLeftBeyondTopBorderAngle0(fxCenterSetBottomLeftBeyondTopBorderAngle0, fBottomSetBottomLeftBeyondTopBorderAngle0);
    QPointF ptLeftCenterSetBottomLeftBeyondTopBorderAngle0(fLeftSetBottomLeftBeyondTopBorderAngle0, fyCenterSetBottomLeftBeyondTopBorderAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(" + qPoint2Str(ptBottomLeftBeyondTopBorder) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptBottomLeftBeyondTopBorder) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptBottomLeftBeyondTopBorder);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondTopBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondTopBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondTopBorderAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondTopBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondTopBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondTopBorderAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondTopBorderAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft(500, 300)
    --------------------------
            200   250   300   350   400   450   500
    TopDown                                          BottomUp
      250   TL----------------------TR----------BR'    350
             |                       |     X'    |
      300    |           X'         TL'---------BL'    300
             |                       |
      350   BL----------------------BR                 250
    */
    QPointF ptBottomLeftBeyondLeftBorder(500.0, 300.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomLeftBeyondLeftBorder.setY(300.0);
    }
    physValAngleCorrected.setVal(270);
    double fxCenterSetBottomLeftBeyondLeftBorderAngle0 = ptBottomLeftBeyondLeftBorder.x() + (fRightSetAngle0 - ptBottomLeftBeyondLeftBorder.x()) / 2.0;
    double fyCenterSetBottomLeftBeyondLeftBorderAngle0 = fTopSetAngle0 + (ptBottomLeftBeyondLeftBorder.y() - fTopSetAngle0) / 2.0;
    double fWidthSetBottomLeftBeyondLeftBorderAngle0 = fabs(fRightSetAngle0 - ptBottomLeftBeyondLeftBorder.x());
    double fHeightSetBottomLeftBeyondLeftBorderAngle0 = fabs(ptBottomLeftBeyondLeftBorder.y() - fTopSetAngle0);
    double fLeftSetBottomLeftBeyondLeftBorderAngle0 = fxCenterSetBottomLeftBeyondLeftBorderAngle0 + fWidthSetBottomLeftBeyondLeftBorderAngle0 / 2.0;
    double fRightSetBottomLeftBeyondLeftBorderAngle0 = fxCenterSetBottomLeftBeyondLeftBorderAngle0 - fWidthSetBottomLeftBeyondLeftBorderAngle0 / 2.0;
    double fTopSetBottomLeftBeyondLeftBorderAngle0 = fyCenterSetBottomLeftBeyondLeftBorderAngle0 + fHeightSetBottomLeftBeyondLeftBorderAngle0 / 2.0;
    double fBottomSetBottomLeftBeyondLeftBorderAngle0 = fyCenterSetBottomLeftBeyondLeftBorderAngle0 - fHeightSetBottomLeftBeyondLeftBorderAngle0 / 2.0;
    QPointF ptCenterSetBottomLeftBeyondLeftBorderAngle0(fxCenterSetBottomLeftBeyondLeftBorderAngle0, fyCenterSetBottomLeftBeyondLeftBorderAngle0);
    QPointF ptTopLeftSetBottomLeftBeyondLeftBorderAngle0(fLeftSetBottomLeftBeyondLeftBorderAngle0, fTopSetBottomLeftBeyondLeftBorderAngle0);
    QPointF ptTopRightSetBottomLeftBeyondLeftBorderAngle0(fRightSetBottomLeftBeyondLeftBorderAngle0, fTopSetBottomLeftBeyondLeftBorderAngle0);
    QPointF ptBottomRightSetBottomLeftBeyondLeftBorderAngle0(fRightSetBottomLeftBeyondLeftBorderAngle0, fBottomSetBottomLeftBeyondLeftBorderAngle0);
    QPointF ptBottomLeftSetBottomLeftBeyondLeftBorderAngle0(fLeftSetBottomLeftBeyondLeftBorderAngle0, fBottomSetBottomLeftBeyondLeftBorderAngle0);
    QPointF ptTopCenterSetBottomLeftBeyondLeftBorderAngle0(fxCenterSetBottomLeftBeyondLeftBorderAngle0, fTopSetBottomLeftBeyondLeftBorderAngle0);
    QPointF ptRightCenterSetBottomLeftBeyondLeftBorderAngle0(fRightSetBottomLeftBeyondLeftBorderAngle0, fyCenterSetBottomLeftBeyondLeftBorderAngle0);
    QPointF ptBottomCenterSetBottomLeftBeyondLeftBorderAngle0(fxCenterSetBottomLeftBeyondLeftBorderAngle0, fBottomSetBottomLeftBeyondLeftBorderAngle0);
    QPointF ptLeftCenterSetBottomLeftBeyondLeftBorderAngle0(fLeftSetBottomLeftBeyondLeftBorderAngle0, fyCenterSetBottomLeftBeyondLeftBorderAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(" + qPoint2Str(ptBottomLeftBeyondLeftBorder) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptBottomLeftBeyondLeftBorder) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptBottomLeftBeyondLeftBorder);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondLeftBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondLeftBorderAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondLeftBorderAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondLeftBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondLeftBorderAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondLeftBorderAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondLeftBorderAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft(500, 150)
    --------------------------
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
    QPointF ptBottomLeftBeyondTopRight(500.0, 150.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomLeftBeyondTopRight.setY(450.0);
    }
    physValAngleCorrected.setVal(180.0);
    double fxCenterSetBottomLeftBeyondTopRightAngle0 = ptBottomLeftBeyondTopRight.x() + (fRightSetAngle0 - ptBottomLeftBeyondTopRight.x()) / 2.0;
    double fyCenterSetBottomLeftBeyondTopRightAngle0 = fTopSetAngle0 + (ptBottomLeftBeyondTopRight.y() - fTopSetAngle0) / 2.0;
    double fWidthSetBottomLeftBeyondTopRightAngle0 = fabs(fRightSetAngle0 - ptBottomLeftBeyondTopRight.x());
    double fHeightSetBottomLeftBeyondTopRightAngle0 = fabs(ptBottomLeftBeyondTopRight.y() - fTopSetAngle0);
    double fLeftSetBottomLeftBeyondTopRightAngle0 = fxCenterSetBottomLeftBeyondTopRightAngle0 + fWidthSetBottomLeftBeyondTopRightAngle0 / 2.0;
    double fRightSetBottomLeftBeyondTopRightAngle0 = fxCenterSetBottomLeftBeyondTopRightAngle0 - fWidthSetBottomLeftBeyondTopRightAngle0 / 2.0;
    double fTopSetBottomLeftBeyondTopRightAngle0 = fyCenterSetBottomLeftBeyondTopRightAngle0 + fHeightSetBottomLeftBeyondTopRightAngle0 / 2.0;
    double fBottomSetBottomLeftBeyondTopRightAngle0 = fyCenterSetBottomLeftBeyondTopRightAngle0 - fHeightSetBottomLeftBeyondTopRightAngle0 / 2.0;
    QPointF ptCenterSetBottomLeftBeyondTopRightAngle0(fxCenterSetBottomLeftBeyondTopRightAngle0, fyCenterSetBottomLeftBeyondTopRightAngle0);
    QPointF ptTopLeftSetBottomLeftBeyondTopRightAngle0(fLeftSetBottomLeftBeyondTopRightAngle0, fTopSetBottomLeftBeyondTopRightAngle0);
    QPointF ptTopRightSetBottomLeftBeyondTopRightAngle0(fRightSetBottomLeftBeyondTopRightAngle0, fTopSetBottomLeftBeyondTopRightAngle0);
    QPointF ptBottomRightSetBottomLeftBeyondTopRightAngle0(fRightSetBottomLeftBeyondTopRightAngle0, fBottomSetBottomLeftBeyondTopRightAngle0);
    QPointF ptBottomLeftSetBottomLeftBeyondTopRightAngle0(fLeftSetBottomLeftBeyondTopRightAngle0, fBottomSetBottomLeftBeyondTopRightAngle0);
    QPointF ptTopCenterSetBottomLeftBeyondTopRightAngle0(fxCenterSetBottomLeftBeyondTopRightAngle0, fTopSetBottomLeftBeyondTopRightAngle0);
    QPointF ptRightCenterSetBottomLeftBeyondTopRightAngle0(fRightSetBottomLeftBeyondTopRightAngle0, fyCenterSetBottomLeftBeyondTopRightAngle0);
    QPointF ptBottomCenterSetBottomLeftBeyondTopRightAngle0(fxCenterSetBottomLeftBeyondTopRightAngle0, fBottomSetBottomLeftBeyondTopRightAngle0);
    QPointF ptLeftCenterSetBottomLeftBeyondTopRightAngle0(fLeftSetBottomLeftBeyondTopRightAngle0, fyCenterSetBottomLeftBeyondTopRightAngle0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(" + qPoint2Str(ptBottomLeftBeyondTopRight) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptBottomLeftBeyondTopRight) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_0_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptBottomLeftBeyondTopRight);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondTopRightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondTopRightAngle0, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondTopRightAngle0, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondTopRightAngle0, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondTopRightAngle0) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondTopRightAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondTopRightAngle0, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondTopRightAngle0) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondTopRightAngle0) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondTopRightAngle0) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondTopRightAngle0) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondTopRightAngle0) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondTopRightAngle0) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondTopRightAngle0) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondTopRightAngle0) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    //--------------------------------------------------------------------------
    // Rotation Angle: 180.0°
    //--------------------------------------------------------------------------

    physValAngle.setVal(180.0);
    physValAngleCorrected.setVal(0.0);

    ZS::Test::CTestStepGroup* pGrpTransformRectAngle_180_Degree = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Angle(" + physValAngle.toString() + ")",
        /* pTSGrpParent */ pGrpTransformRect );

    /* setAngle(180°)
    ----------------
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
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("addPhysValShape", "");
    // Center, Width and Height must remain the same for all following setAngle test steps
    double fxCenterSetAngle180 = rectFOrig.center().x();
    double fyCenterSetAngle180 = rectFOrig.center().y();
    double fWidthSetAngle180 = rectFOrig.width();
    double fHeightSetAngle180 = rectFOrig.height();
    // When setting the width the left edge must remain the same.
    double fLeftSetAngle180 = fxCenterSetAngle180 - fWidthSetAngle180 / 2.0;
    // When setting the width by moving the left center point the right edge must remain the same.
    double fRightSetAngle180 = fxCenterSetAngle180 + fWidthSetAngle180 / 2.0;
    // When setting the height the top edge must remain the same.
    double fTopSetAngle180 = fyCenterSetAngle180 - fHeightSetAngle180 / 2.0;
    // When setting the height by moving the top center point the bottom edge must remain the same.
    double fBottomSetAngle180 = fyCenterSetAngle180 + fHeightSetAngle180 / 2.0;
    QPointF ptCenterSetAngle180(fxCenterSetAngle180, fyCenterSetAngle180);
    QPointF ptTopLeftSetAngle180(fLeftSetAngle180, fTopSetAngle180);
    QPointF ptTopRightSetAngle180(fRightSetAngle180, fTopSetAngle180);
    QPointF ptBottomRightSetAngle180(fRightSetAngle180, fBottomSetAngle180);
    QPointF ptBottomLeftSetAngle180(fLeftSetAngle180, fBottomSetAngle180);
    QPointF ptTopCenterSetAngle180(fxCenterSetAngle180, fTopSetAngle180);
    QPointF ptRightCenterSetAngle180(fRightSetAngle180, fyCenterSetAngle180);
    QPointF ptBottomCenterSetAngle180(fxCenterSetAngle180, fBottomSetAngle180);
    QPointF ptLeftCenterSetAngle180(fLeftSetAngle180, fyCenterSetAngle180);
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetAngle180) + "} mm");
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
    ptCenter = QPointF(400.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptCenter.setY(200.0);
    }
    fdx = ptCenter.x() - ptCenterSetAngle180.x();
    fdy = ptCenter.y() - ptCenterSetAngle180.y();
    double fWidthSetCenterAngle180 = fWidthSetAngle180;
    double fHeightSetCenterAngle180 = fHeightSetAngle180;
    QPointF ptCenterSetCenterAngle180 = ptCenter;
    QPointF ptTopLeftSetCenterAngle180(ptTopLeftSetAngle180.x() + fdx, ptTopLeftSetAngle180.y() + fdy);
    QPointF ptTopRightSetCenterAngle180(ptTopRightSetAngle180.x() + fdx, ptTopRightSetAngle180.y() + fdy);
    QPointF ptBottomRightSetCenterAngle180(ptBottomRightSetAngle180.x() + fdx, ptBottomRightSetAngle180.y() + fdy);
    QPointF ptBottomLeftSetCenterAngle180(ptBottomLeftSetAngle180.x() + fdx, ptBottomLeftSetAngle180.y() + fdy);
    QPointF ptTopCenterSetCenterAngle180(ptTopCenterSetAngle180.x() + fdx, ptTopCenterSetAngle180.y() + fdy);
    QPointF ptRightCenterSetCenterAngle180(ptRightCenterSetAngle180.x() + fdx, ptRightCenterSetAngle180.y() + fdy);
    QPointF ptBottomCenterSetCenterAngle180(ptBottomCenterSetAngle180.x() + fdx, ptBottomCenterSetAngle180.y() + fdy);
    QPointF ptLeftCenterSetCenterAngle180(ptLeftCenterSetAngle180.x() + fdx, ptLeftCenterSetAngle180.y() + fdy);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setCenter(" + qPoint2Str(ptCenter) + " " + unit.symbol() + ")",
        /* strOperation    */ "setCenter(" + qPoint2Str(ptCenter) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setCenter", ptCenter);
    pTestStep->setConfigValue("setCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetCenterAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetCenterAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetCenterAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetCenterAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetCenterAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetCenterAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetCenterAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetCenterAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetCenterAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetCenterAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetCenterAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetCenterAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetCenterAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetCenterAngle180) + "} mm");
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
    double fxCenterSetSizeAngle180 = fLeftSetAngle180 + size300x200.width() / 2.0;
    double fyCenterSetSizeAngle180 = fTopSetAngle180 + size300x200.height() / 2.0;
    double fWidthSetSizeAngle180 = size300x200.width();
    double fHeightSetSizeAngle180 = size300x200.height();
    double fLeftSetSizeAngle180 = fxCenterSetSizeAngle180 - fWidthSetSizeAngle180 / 2.0;
    double fRightSetSizeAngle180 = fxCenterSetSizeAngle180 + fWidthSetSizeAngle180 / 2.0;
    double fTopSetSizeAngle180 = fyCenterSetSizeAngle180 - fHeightSetSizeAngle180 / 2.0;
    double fBottomSetSizeAngle180 = fyCenterSetSizeAngle180 + fHeightSetSizeAngle180 / 2.0;
    QPointF ptCenterSetSizeAngle180(fxCenterSetSizeAngle180, fyCenterSetSizeAngle180);
    QPointF ptTopLeftSetSizeAngle180(fLeftSetSizeAngle180, fTopSetSizeAngle180);
    QPointF ptTopRightSetSizeAngle180(fRightSetSizeAngle180, fTopSetSizeAngle180);
    QPointF ptBottomRightSetSizeAngle180(fRightSetSizeAngle180, fBottomSetSizeAngle180);
    QPointF ptBottomLeftSetSizeAngle180(fLeftSetSizeAngle180, fBottomSetSizeAngle180);
    QPointF ptTopCenterSetSizeAngle180(fxCenterSetSizeAngle180, fTopSetSizeAngle180);
    QPointF ptRightCenterSetSizeAngle180(fRightSetSizeAngle180, fyCenterSetSizeAngle180);
    QPointF ptBottomCenterSetSizeAngle180(fxCenterSetSizeAngle180, fBottomSetSizeAngle180);
    QPointF ptLeftCenterSetSizeAngle180(fLeftSetSizeAngle180, fyCenterSetSizeAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setSize(" + qSize2Str(size300x200) + " " + unit.symbol() + ")",
        /* strOperation    */ "setSize(" + qSize2Str(size300x200) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setSize", size300x200);
    pTestStep->setConfigValue("setSize.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetSizeAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetSizeAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetSizeAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetSizeAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetSizeAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetSizeAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetSizeAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetSizeAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetSizeAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetSizeAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetSizeAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetSizeAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetSizeAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetSizeAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetSizeAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetSizeAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetSizeAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetSizeAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetSizeAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetSizeAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetSizeAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetSizeAngle180) + "} mm");
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
    double fxCenterSetWidthAngle180 = fLeftSetAngle180 + physValWidth300.getVal() / 2.0;
    double fyCenterSetWidthAngle180 = fyCenterSetAngle180;
    double fWidthSetWidthAngle180 = physValWidth300.getVal();
    double fHeightSetWidthAngle180 = fHeightSetAngle180;
    double fLeftSetWidthAngle180 = fxCenterSetWidthAngle180 - fWidthSetWidthAngle180 / 2.0;
    double fRightSetWidthAngle180 = fxCenterSetWidthAngle180 + fWidthSetWidthAngle180 / 2.0;
    double fTopSetWidthAngle180 = fyCenterSetWidthAngle180 - fHeightSetWidthAngle180 / 2.0;
    double fBottomSetWidthAngle180 = fyCenterSetWidthAngle180 + fHeightSetWidthAngle180 / 2.0;
    QPointF ptCenterSetWidthAngle180(fxCenterSetWidthAngle180, fyCenterSetWidthAngle180);
    QPointF ptTopLeftSetWidthAngle180(fLeftSetWidthAngle180, fTopSetWidthAngle180);
    QPointF ptTopRightSetWidthAngle180(fRightSetWidthAngle180, fTopSetWidthAngle180);
    QPointF ptBottomRightSetWidthAngle180(fRightSetWidthAngle180, fBottomSetWidthAngle180);
    QPointF ptBottomLeftSetWidthAngle180(fLeftSetWidthAngle180, fBottomSetWidthAngle180);
    QPointF ptTopCenterSetWidthAngle180(fxCenterSetWidthAngle180, fTopSetWidthAngle180);
    QPointF ptRightCenterSetWidthAngle180(fRightSetWidthAngle180, fyCenterSetWidthAngle180);
    QPointF ptBottomCenterSetWidthAngle180(fxCenterSetWidthAngle180, fBottomSetWidthAngle180);
    QPointF ptLeftCenterSetWidthAngle180(fLeftSetWidthAngle180, fyCenterSetWidthAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidth(" + physValWidth300.toString() + ")",
        /* strOperation    */ "setWidth(" + physValWidth300.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidth", physValWidth300.toString());
    pTestStep->setConfigValue("setWidth.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthAngle180) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingLeftCenter(100)
    ----------------------------------
        200   250   300   350   400          300   350   400  BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 +-----+-----+-----+-----+        350 +-----+-----+      250
    */
    ptMoved = ptTopCenterSetAngle180;
    double fxCenterSetWidthMoveLeftCenterAngle180 = fRightSetAngle180 - (fRightSetAngle180 - ptMoved.x()) / 2.0;
    double fyCenterSetWidthMoveLeftCenterAngle180 = fyCenterSetAngle180;
    double fWidthSetWidthMoveLeftCenterAngle180 = fabs(fRightSetAngle180 - ptMoved.x());
    double fHeightSetWidthMoveLeftCenterAngle180 = fHeightSetAngle180;
    double fLeftSetWidthMoveLeftCenterAngle180 = fxCenterSetWidthMoveLeftCenterAngle180 - fWidthSetWidthMoveLeftCenterAngle180 / 2.0;
    double fRightSetWidthMoveLeftCenterAngle180 = fxCenterSetWidthMoveLeftCenterAngle180 + fWidthSetWidthMoveLeftCenterAngle180 / 2.0;
    double fTopSetWidthMoveLeftCenterAngle180 = fyCenterSetWidthMoveLeftCenterAngle180 - fHeightSetWidthMoveLeftCenterAngle180 / 2.0;
    double fBottomSetWidthMoveLeftCenterAngle180 = fyCenterSetWidthMoveLeftCenterAngle180 + fHeightSetWidthMoveLeftCenterAngle180 / 2.0;
    QPointF ptCenterSetWidthMoveLeftCenterAngle180(fxCenterSetWidthMoveLeftCenterAngle180, fyCenterSetWidthMoveLeftCenterAngle180);
    QPointF ptTopLeftSetWidthMoveLeftCenterAngle180(fLeftSetWidthMoveLeftCenterAngle180, fTopSetWidthMoveLeftCenterAngle180);
    QPointF ptTopRightSetWidthMoveLeftCenterAngle180(fRightSetWidthMoveLeftCenterAngle180, fTopSetWidthMoveLeftCenterAngle180);
    QPointF ptBottomRightSetWidthMoveLeftCenterAngle180(fRightSetWidthMoveLeftCenterAngle180, fBottomSetWidthMoveLeftCenterAngle180);
    QPointF ptBottomLeftSetWidthMoveLeftCenterAngle180(fLeftSetWidthMoveLeftCenterAngle180, fBottomSetWidthMoveLeftCenterAngle180);
    QPointF ptTopCenterSetWidthMoveLeftCenterAngle180(fxCenterSetWidthMoveLeftCenterAngle180, fTopSetWidthMoveLeftCenterAngle180);
    QPointF ptRightCenterSetWidthMoveLeftCenterAngle180(fRightSetWidthMoveLeftCenterAngle180, fyCenterSetWidthMoveLeftCenterAngle180);
    QPointF ptBottomCenterSetWidthMoveLeftCenterAngle180(fxCenterSetWidthMoveLeftCenterAngle180, fBottomSetWidthMoveLeftCenterAngle180);
    QPointF ptLeftCenterSetWidthMoveLeftCenterAngle180(fLeftSetWidthMoveLeftCenterAngle180, fyCenterSetWidthMoveLeftCenterAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterAngle180) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingLeftCenter(500, 250)
    ---------------------------------------
        200   250   300   350   400          400   450   500  BottomUp
     250 TL----+-----+-----+----TR        250 BR----+----BL      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 BL----+-----+-----+----BR        350 TR----+----TL      250
    */
    ptMoved = QPointF(500, 250);
    double fxCenterSetWidthMoveLeftCenterBeyondRightAngle180 = fRightSetAngle180 + (ptMoved.x()- fRightSetAngle180) / 2.0;
    double fyCenterSetWidthMoveLeftCenterBeyondRightAngle180 = fyCenterSetAngle180;
    double fWidthSetWidthMoveLeftCenterBeyondRightAngle180 = fabs(fRightSetAngle180 - ptMoved.x());
    double fHeightSetWidthMoveLeftCenterBeyondRightAngle180 = fHeightSetAngle180;
    double fLeftSetWidthMoveLeftCenterBeyondRightAngle180 = fxCenterSetWidthMoveLeftCenterBeyondRightAngle180 + fWidthSetWidthMoveLeftCenterBeyondRightAngle180 / 2.0;
    double fRightSetWidthMoveLeftCenterBeyondRightAngle180 = fxCenterSetWidthMoveLeftCenterBeyondRightAngle180 - fWidthSetWidthMoveLeftCenterBeyondRightAngle180 / 2.0;
    double fTopSetWidthMoveLeftCenterBeyondRightAngle180 = fyCenterSetWidthMoveLeftCenterBeyondRightAngle180 + fHeightSetWidthMoveLeftCenterBeyondRightAngle180 / 2.0;
    double fBottomSetWidthMoveLeftCenterBeyondRightAngle180 = fyCenterSetWidthMoveLeftCenterBeyondRightAngle180 - fHeightSetWidthMoveLeftCenterBeyondRightAngle180 / 2.0;
    QPointF ptCenterSetWidthMoveLeftCenterBeyondRightAngle180(fxCenterSetWidthMoveLeftCenterBeyondRightAngle180, fyCenterSetWidthMoveLeftCenterBeyondRightAngle180);
    QPointF ptTopLeftSetWidthMoveLeftCenterBeyondRightAngle180(fLeftSetWidthMoveLeftCenterBeyondRightAngle180, fTopSetWidthMoveLeftCenterBeyondRightAngle180);
    QPointF ptTopRightSetWidthMoveLeftCenterBeyondRightAngle180(fRightSetWidthMoveLeftCenterBeyondRightAngle180, fTopSetWidthMoveLeftCenterBeyondRightAngle180);
    QPointF ptBottomRightSetWidthMoveLeftCenterBeyondRightAngle180(fRightSetWidthMoveLeftCenterBeyondRightAngle180, fBottomSetWidthMoveLeftCenterBeyondRightAngle180);
    QPointF ptBottomLeftSetWidthMoveLeftCenterBeyondRightAngle180(fLeftSetWidthMoveLeftCenterBeyondRightAngle180, fBottomSetWidthMoveLeftCenterBeyondRightAngle180);
    QPointF ptTopCenterSetWidthMoveLeftCenterBeyondRightAngle180(fxCenterSetWidthMoveLeftCenterBeyondRightAngle180, fTopSetWidthMoveLeftCenterBeyondRightAngle180);
    QPointF ptRightCenterSetWidthMoveLeftCenterBeyondRightAngle180(fRightSetWidthMoveLeftCenterBeyondRightAngle180, fyCenterSetWidthMoveLeftCenterBeyondRightAngle180);
    QPointF ptBottomCenterSetWidthMoveLeftCenterBeyondRightAngle180(fxCenterSetWidthMoveLeftCenterBeyondRightAngle180, fBottomSetWidthMoveLeftCenterBeyondRightAngle180);
    QPointF ptLeftCenterSetWidthMoveLeftCenterBeyondRightAngle180(fLeftSetWidthMoveLeftCenterBeyondRightAngle180, fyCenterSetWidthMoveLeftCenterBeyondRightAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterBeyondRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterBeyondRightAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterBeyondRightAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterBeyondRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterBeyondRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterBeyondRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterBeyondRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterBeyondRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterBeyondRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterBeyondRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterBeyondRightAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterBeyondRightAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterBeyondRightAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterBeyondRightAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterBeyondRightAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterBeyondRightAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterBeyondRightAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterBeyondRightAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterBeyondRightAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterBeyondRightAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterBeyondRightAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterBeyondRightAngle180) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingRightCenter(100)
    -----------------------------------
        200   250   300   350   400          200   250   300  BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+      350
         |                       |            |           |
     300 +           X           +   =>   300 +     X     +      300
         |                       |            |           |
     350 +-----+-----+-----+-----+        350 +-----+-----+      250
    */
    ptMoved = ptTopCenterSetAngle180;
    double fxCenterSetWidthMoveRightCenterAngle180 = fLeftSetAngle180 + (ptMoved.x() - fLeftSetAngle180) / 2.0;
    double fyCenterSetWidthMoveRightCenterAngle180 = fyCenterSetAngle180;
    double fWidthSetWidthMoveRightCenterAngle180 = fabs(ptMoved.x() - fLeftSetAngle180);
    double fHeightSetWidthMoveRightCenterAngle180 = fHeightSetAngle180;
    double fLeftSetWidthMoveRightCenterAngle180 = fxCenterSetWidthMoveRightCenterAngle180 - fWidthSetWidthMoveRightCenterAngle180 / 2.0;
    double fRightSetWidthMoveRightCenterAngle180 = fxCenterSetWidthMoveRightCenterAngle180 + fWidthSetWidthMoveRightCenterAngle180 / 2.0;
    double fTopSetWidthMoveRightCenterAngle180 = fyCenterSetWidthMoveRightCenterAngle180 - fHeightSetWidthMoveRightCenterAngle180 / 2.0;
    double fBottomSetWidthMoveRightCenterAngle180 = fyCenterSetWidthMoveRightCenterAngle180 + fHeightSetWidthMoveRightCenterAngle180 / 2.0;
    QPointF ptCenterSetWidthMoveRightCenterAngle180(fxCenterSetWidthMoveRightCenterAngle180, fyCenterSetWidthMoveRightCenterAngle180);
    QPointF ptTopLeftSetWidthMoveRightCenterAngle180(fLeftSetWidthMoveRightCenterAngle180, fTopSetWidthMoveRightCenterAngle180);
    QPointF ptTopRightSetWidthMoveRightCenterAngle180(fRightSetWidthMoveRightCenterAngle180, fTopSetWidthMoveRightCenterAngle180);
    QPointF ptBottomRightSetWidthMoveRightCenterAngle180(fRightSetWidthMoveRightCenterAngle180, fBottomSetWidthMoveRightCenterAngle180);
    QPointF ptBottomLeftSetWidthMoveRightCenterAngle180(fLeftSetWidthMoveRightCenterAngle180, fBottomSetWidthMoveRightCenterAngle180);
    QPointF ptTopCenterSetWidthMoveRightCenterAngle180(fxCenterSetWidthMoveRightCenterAngle180, fTopSetWidthMoveRightCenterAngle180);
    QPointF ptRightCenterSetWidthMoveRightCenterAngle180(fRightSetWidthMoveRightCenterAngle180, fyCenterSetWidthMoveRightCenterAngle180);
    QPointF ptBottomCenterSetWidthMoveRightCenterAngle180(fxCenterSetWidthMoveRightCenterAngle180, fBottomSetWidthMoveRightCenterAngle180);
    QPointF ptLeftCenterSetWidthMoveRightCenterAngle180(fLeftSetWidthMoveRightCenterAngle180, fyCenterSetWidthMoveRightCenterAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterAngle180) + "} mm");
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
    double fxCenterSetWidthMoveRightCenterBeyondLeftAngle180 = fLeftSetAngle180 + (ptMoved.x() - fLeftSetAngle180) / 2.0;
    double fyCenterSetWidthMoveRightCenterBeyondLeftAngle180 = fyCenterSetAngle180;
    double fWidthSetWidthMoveRightCenterBeyondLeftAngle180 = fabs(ptMoved.x() - fLeftSetAngle180);
    double fHeightSetWidthMoveRightCenterBeyondLeftAngle180 = fHeightSetAngle180;
    double fLeftSetWidthMoveRightCenterBeyondLeftAngle180 = fxCenterSetWidthMoveRightCenterBeyondLeftAngle180 - fWidthSetWidthMoveRightCenterBeyondLeftAngle180 / 2.0;
    double fRightSetWidthMoveRightCenterBeyondLeftAngle180 = fxCenterSetWidthMoveRightCenterBeyondLeftAngle180 + fWidthSetWidthMoveRightCenterBeyondLeftAngle180 / 2.0;
    double fTopSetWidthMoveRightCenterBeyondLeftAngle180 = fyCenterSetWidthMoveRightCenterBeyondLeftAngle180 - fHeightSetWidthMoveRightCenterBeyondLeftAngle180 / 2.0;
    double fBottomSetWidthMoveRightCenterBeyondLeftAngle180 = fyCenterSetWidthMoveRightCenterBeyondLeftAngle180 + fHeightSetWidthMoveRightCenterBeyondLeftAngle180 / 2.0;
    QPointF ptCenterSetWidthMoveRightCenterBeyondLeftAngle180(fxCenterSetWidthMoveRightCenterBeyondLeftAngle180, fyCenterSetWidthMoveRightCenterBeyondLeftAngle180);
    QPointF ptTopLeftSetWidthMoveRightCenterBeyondLeftAngle180(fLeftSetWidthMoveRightCenterBeyondLeftAngle180, fTopSetWidthMoveRightCenterBeyondLeftAngle180);
    QPointF ptTopRightSetWidthMoveRightCenterBeyondLeftAngle180(fRightSetWidthMoveRightCenterBeyondLeftAngle180, fTopSetWidthMoveRightCenterBeyondLeftAngle180);
    QPointF ptBottomRightSetWidthMoveRightCenterBeyondLeftAngle180(fRightSetWidthMoveRightCenterBeyondLeftAngle180, fBottomSetWidthMoveRightCenterBeyondLeftAngle180);
    QPointF ptBottomLeftSetWidthMoveRightCenterBeyondLeftAngle180(fLeftSetWidthMoveRightCenterBeyondLeftAngle180, fBottomSetWidthMoveRightCenterBeyondLeftAngle180);
    QPointF ptTopCenterSetWidthMoveRightCenterBeyondLeftAngle180(fxCenterSetWidthMoveRightCenterBeyondLeftAngle180, fTopSetWidthMoveRightCenterBeyondLeftAngle180);
    QPointF ptRightCenterSetWidthMoveRightCenterBeyondLeftAngle180(fRightSetWidthMoveRightCenterBeyondLeftAngle180, fyCenterSetWidthMoveRightCenterBeyondLeftAngle180);
    QPointF ptBottomCenterSetWidthMoveRightCenterBeyondLeftAngle180(fxCenterSetWidthMoveRightCenterBeyondLeftAngle180, fBottomSetWidthMoveRightCenterBeyondLeftAngle180);
    QPointF ptLeftCenterSetWidthMoveRightCenterBeyondLeftAngle180(fLeftSetWidthMoveRightCenterBeyondLeftAngle180, fyCenterSetWidthMoveRightCenterBeyondLeftAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterBeyondLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterBeyondLeftAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterBeyondLeftAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterBeyondLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterBeyondLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterBeyondLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterBeyondLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterBeyondLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterBeyondLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterBeyondLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterBeyondLeftAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterBeyondLeftAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterBeyondLeftAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterBeyondLeftAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterBeyondLeftAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterBeyondLeftAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterBeyondLeftAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterBeyondLeftAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterBeyondLeftAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterBeyondLeftAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterBeyondLeftAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterBeyondLeftAngle180) + "} mm");
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
    double fxCenterSetHeightAngle180 = fxCenterSetAngle180;
    double fyCenterSetHeightAngle180 = fyCenterSetAngle180 + (physValHeight200.getVal() - fHeightSetAngle180) / 2.0;
    double fWidthSetHeightAngle180 = fWidthSetAngle180;
    double fHeightSetHeightAngle180 = physValHeight200.getVal();
    double fLeftSetHeightAngle180 = fxCenterSetHeightAngle180 - fWidthSetHeightAngle180 / 2.0;
    double fRightSetHeightAngle180 = fxCenterSetHeightAngle180 + fWidthSetHeightAngle180 / 2.0;
    double fTopSetHeightAngle180 = fyCenterSetHeightAngle180 - fHeightSetHeightAngle180 / 2.0;
    double fBottomSetHeightAngle180 = fyCenterSetHeightAngle180 + fHeightSetHeightAngle180 / 2.0;
    QPointF ptCenterSetHeightAngle180(fxCenterSetHeightAngle180, fyCenterSetHeightAngle180);
    QPointF ptTopLeftSetHeightAngle180(fLeftSetHeightAngle180, fTopSetHeightAngle180);
    QPointF ptTopRightSetHeightAngle180(fRightSetHeightAngle180, fTopSetHeightAngle180);
    QPointF ptBottomRightSetHeightAngle180(fRightSetHeightAngle180, fBottomSetHeightAngle180);
    QPointF ptBottomLeftSetHeightAngle180(fLeftSetHeightAngle180, fBottomSetHeightAngle180);
    QPointF ptTopCenterSetHeightAngle180(fxCenterSetHeightAngle180, fTopSetHeightAngle180);
    QPointF ptRightCenterSetHeightAngle180(fRightSetHeightAngle180, fyCenterSetHeightAngle180);
    QPointF ptBottomCenterSetHeightAngle180(fxCenterSetHeightAngle180, fBottomSetHeightAngle180);
    QPointF ptLeftCenterSetHeightAngle180(fLeftSetHeightAngle180, fyCenterSetHeightAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeight(" + physValHeight200.toString() + ")",
        /* strOperation    */ "setHeight(" + physValHeight200.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeight", physValHeight200.toString());
    pTestStep->setConfigValue("setHeight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightAngle180) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingTopCenter(50)
    ---------------------------------------
        200   250   300   350   400          200   250   300   350   400   BottomUp
     250 +-----+-----+-----+-----+        250                                350
         |                       |
     300 +           X           +        300 +-----+-----+-----+-----+      300
         |                       |   =>       |           X           |
     350 +-----+-----+-----+-----+        350 +-----+-----+-----+-----+
    */
    ptMoved = ptRightCenterSetAngle180;
    double fxCenterSetHeightMoveTopCenterAngle180 = fxCenterSetAngle180;
    double fyCenterSetHeightMoveTopCenterAngle180 = fBottomSetAngle180 - (fBottomSetAngle180 - ptMoved.y()) / 2.0;
    double fWidthSetHeightMoveTopCenterAngle180 = fWidthSetAngle180;
    double fHeightSetHeightMoveTopCenterAngle180 = fabs(fBottomSetAngle180 - ptMoved.y());
    double fLeftSetHeightMoveTopCenterAngle180 = fxCenterSetHeightMoveTopCenterAngle180 - fWidthSetHeightMoveTopCenterAngle180 / 2.0;
    double fRightSetHeightMoveTopCenterAngle180 = fxCenterSetHeightMoveTopCenterAngle180 + fWidthSetHeightMoveTopCenterAngle180 / 2.0;
    double fTopSetHeightMoveTopCenterAngle180 = fyCenterSetHeightMoveTopCenterAngle180 - fHeightSetHeightMoveTopCenterAngle180 / 2.0;
    double fBottomSetHeightMoveTopCenterAngle180 = fyCenterSetHeightMoveTopCenterAngle180 + fHeightSetHeightMoveTopCenterAngle180 / 2.0;
    QPointF ptCenterSetHeightMoveTopCenterAngle180(fxCenterSetHeightMoveTopCenterAngle180, fyCenterSetHeightMoveTopCenterAngle180);
    QPointF ptTopLeftSetHeightMoveTopCenterAngle180(fLeftSetHeightMoveTopCenterAngle180, fTopSetHeightMoveTopCenterAngle180);
    QPointF ptTopRightSetHeightMoveTopCenterAngle180(fRightSetHeightMoveTopCenterAngle180, fTopSetHeightMoveTopCenterAngle180);
    QPointF ptBottomRightSetHeightMoveTopCenterAngle180(fRightSetHeightMoveTopCenterAngle180, fBottomSetHeightMoveTopCenterAngle180);
    QPointF ptBottomLeftSetHeightMoveTopCenterAngle180(fLeftSetHeightMoveTopCenterAngle180, fBottomSetHeightMoveTopCenterAngle180);
    QPointF ptTopCenterSetHeightMoveTopCenterAngle180(fxCenterSetHeightMoveTopCenterAngle180, fTopSetHeightMoveTopCenterAngle180);
    QPointF ptRightCenterSetHeightMoveTopCenterAngle180(fRightSetHeightMoveTopCenterAngle180, fyCenterSetHeightMoveTopCenterAngle180);
    QPointF ptBottomCenterSetHeightMoveTopCenterAngle180(fxCenterSetHeightMoveTopCenterAngle180, fBottomSetHeightMoveTopCenterAngle180);
    QPointF ptLeftCenterSetHeightMoveTopCenterAngle180(fLeftSetHeightMoveTopCenterAngle180, fyCenterSetHeightMoveTopCenterAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterAngle180) + "} mm");
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
    double fxCenterSetHeightMoveTopCenterBeyondBottomAngle180 = fxCenterSetAngle180;
    double fyCenterSetHeightMoveTopCenterBeyondBottomAngle180 = fBottomSetAngle180 - (fBottomSetAngle180 - ptMoved.y()) / 2.0;
    double fWidthSetHeightMoveTopCenterBeyondBottomAngle180 = fWidthSetAngle180;
    double fHeightSetHeightMoveTopCenterBeyondBottomAngle180 = fabs(fBottomSetAngle180 - ptMoved.y());
    double fLeftSetHeightMoveTopCenterBeyondBottomAngle180 = fxCenterSetHeightMoveTopCenterBeyondBottomAngle180 - fWidthSetHeightMoveTopCenterBeyondBottomAngle180 / 2.0;
    double fRightSetHeightMoveTopCenterBeyondBottomAngle180 = fxCenterSetHeightMoveTopCenterBeyondBottomAngle180 + fWidthSetHeightMoveTopCenterBeyondBottomAngle180 / 2.0;
    double fTopSetHeightMoveTopCenterBeyondBottomAngle180 = fyCenterSetHeightMoveTopCenterBeyondBottomAngle180 - fHeightSetHeightMoveTopCenterBeyondBottomAngle180 / 2.0;
    double fBottomSetHeightMoveTopCenterBeyondBottomAngle180 = fyCenterSetHeightMoveTopCenterBeyondBottomAngle180 + fHeightSetHeightMoveTopCenterBeyondBottomAngle180 / 2.0;
    QPointF ptCenterSetHeightMoveTopCenterBeyondBottomAngle180(fxCenterSetHeightMoveTopCenterBeyondBottomAngle180, fyCenterSetHeightMoveTopCenterBeyondBottomAngle180);
    QPointF ptTopLeftSetHeightMoveTopCenterBeyondBottomAngle180(fLeftSetHeightMoveTopCenterBeyondBottomAngle180, fTopSetHeightMoveTopCenterBeyondBottomAngle180);
    QPointF ptTopRightSetHeightMoveTopCenterBeyondBottomAngle180(fRightSetHeightMoveTopCenterBeyondBottomAngle180, fTopSetHeightMoveTopCenterBeyondBottomAngle180);
    QPointF ptBottomRightSetHeightMoveTopCenterBeyondBottomAngle180(fRightSetHeightMoveTopCenterBeyondBottomAngle180, fBottomSetHeightMoveTopCenterBeyondBottomAngle180);
    QPointF ptBottomLeftSetHeightMoveTopCenterBeyondBottomAngle180(fLeftSetHeightMoveTopCenterBeyondBottomAngle180, fBottomSetHeightMoveTopCenterBeyondBottomAngle180);
    QPointF ptTopCenterSetHeightMoveTopCenterBeyondBottomAngle180(fxCenterSetHeightMoveTopCenterBeyondBottomAngle180, fTopSetHeightMoveTopCenterBeyondBottomAngle180);
    QPointF ptRightCenterSetHeightMoveTopCenterBeyondBottomAngle180(fRightSetHeightMoveTopCenterBeyondBottomAngle180, fyCenterSetHeightMoveTopCenterBeyondBottomAngle180);
    QPointF ptBottomCenterSetHeightMoveTopCenterBeyondBottomAngle180(fxCenterSetHeightMoveTopCenterBeyondBottomAngle180, fBottomSetHeightMoveTopCenterBeyondBottomAngle180);
    QPointF ptLeftCenterSetHeightMoveTopCenterBeyondBottomAngle180(fLeftSetHeightMoveTopCenterBeyondBottomAngle180, fyCenterSetHeightMoveTopCenterBeyondBottomAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterBeyondBottomAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterBeyondBottomAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterBeyondBottomAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterBeyondBottomAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterBeyondBottomAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterBeyondBottomAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterBeyondBottomAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterBeyondBottomAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterBeyondBottomAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterBeyondBottomAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterBeyondBottomAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterBeyondBottomAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterBeyondBottomAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterBeyondBottomAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterBeyondBottomAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterBeyondBottomAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterBeyondBottomAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterBeyondBottomAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterBeyondBottomAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterBeyondBottomAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterBeyondBottomAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterBeyondBottomAngle180) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingBottomCenter(50)
    ------------------------------------------
        200   250   300   350   400          200   250   300   350   400   BottomUp
     250 +-----+-----+-----+-----+        250 +-----+-----+-----+-----+      350
         |                       |   =>       |           X           |
     300 +           X           +        300 +-----+-----+-----+-----+      300
         |                       |
     350 +-----+-----+-----+-----+
    */
    ptMoved = ptRightCenterSetAngle180;
    double fxCenterSetHeightMoveBottomCenterAngle180 = fxCenterSetAngle180;
    double fyCenterSetHeightMoveBottomCenterAngle180 = fTopSetAngle180 + (ptMoved.y() - fTopSetAngle180) / 2.0;
    double fWidthSetHeightMoveBottomCenterAngle180 = fWidthSetAngle180;
    double fHeightSetHeightMoveBottomCenterAngle180 = fabs(ptMoved.y() - fTopSetAngle180);
    double fLeftSetHeightMoveBottomCenterAngle180 = fxCenterSetHeightMoveBottomCenterAngle180 - fWidthSetHeightMoveBottomCenterAngle180 / 2.0;
    double fRightSetHeightMoveBottomCenterAngle180 = fxCenterSetHeightMoveBottomCenterAngle180 + fWidthSetHeightMoveBottomCenterAngle180 / 2.0;
    double fTopSetHeightMoveBottomCenterAngle180 = fyCenterSetHeightMoveBottomCenterAngle180 - fHeightSetHeightMoveBottomCenterAngle180 / 2.0;
    double fBottomSetHeightMoveBottomCenterAngle180 = fyCenterSetHeightMoveBottomCenterAngle180 + fHeightSetHeightMoveBottomCenterAngle180 / 2.0;
    QPointF ptCenterSetHeightMoveBottomCenterAngle180(fxCenterSetHeightMoveBottomCenterAngle180, fyCenterSetHeightMoveBottomCenterAngle180);
    QPointF ptTopLeftSetHeightMoveBottomCenterAngle180(fLeftSetHeightMoveBottomCenterAngle180, fTopSetHeightMoveBottomCenterAngle180);
    QPointF ptTopRightSetHeightMoveBottomCenterAngle180(fRightSetHeightMoveBottomCenterAngle180, fTopSetHeightMoveBottomCenterAngle180);
    QPointF ptBottomRightSetHeightMoveBottomCenterAngle180(fRightSetHeightMoveBottomCenterAngle180, fBottomSetHeightMoveBottomCenterAngle180);
    QPointF ptBottomLeftSetHeightMoveBottomCenterAngle180(fLeftSetHeightMoveBottomCenterAngle180, fBottomSetHeightMoveBottomCenterAngle180);
    QPointF ptTopCenterSetHeightMoveBottomCenterAngle180(fxCenterSetHeightMoveBottomCenterAngle180, fTopSetHeightMoveBottomCenterAngle180);
    QPointF ptRightCenterSetHeightMoveBottomCenterAngle180(fRightSetHeightMoveBottomCenterAngle180, fyCenterSetHeightMoveBottomCenterAngle180);
    QPointF ptBottomCenterSetHeightMoveBottomCenterAngle180(fxCenterSetHeightMoveBottomCenterAngle180, fBottomSetHeightMoveBottomCenterAngle180);
    QPointF ptLeftCenterSetHeightMoveBottomCenterAngle180(fLeftSetHeightMoveBottomCenterAngle180, fyCenterSetHeightMoveBottomCenterAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenterAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenterAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenterAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenterAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenterAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenterAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenterAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenterAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenterAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenterAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenterAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenterAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenterAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenterAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenterAngle180) + "} mm");
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
    double fxCenterSetHeightMoveBottomCenterBeyondTopAngle180 = fxCenterSetAngle180;
    double fyCenterSetHeightMoveBottomCenterBeyondTopAngle180 = fTopSetAngle180 + (ptMoved.y() - fTopSetAngle180) / 2.0;
    double fWidthSetHeightMoveBottomCenterBeyondTopAngle180 = fWidthSetAngle180;
    double fHeightSetHeightMoveBottomCenterBeyondTopAngle180 = fabs(ptMoved.y() - fTopSetAngle180);
    double fLeftSetHeightMoveBottomCenterBeyondTopAngle180 = fxCenterSetHeightMoveBottomCenterBeyondTopAngle180 - fWidthSetHeightMoveBottomCenterBeyondTopAngle180 / 2.0;
    double fRightSetHeightMoveBottomCenterBeyondTopAngle180 = fxCenterSetHeightMoveBottomCenterBeyondTopAngle180 + fWidthSetHeightMoveBottomCenterBeyondTopAngle180 / 2.0;
    double fTopSetHeightMoveBottomCenterBeyondTopAngle180 = fyCenterSetHeightMoveBottomCenterBeyondTopAngle180 - fHeightSetHeightMoveBottomCenterBeyondTopAngle180 / 2.0;
    double fBottomSetHeightMoveBottomCenterBeyondTopAngle180 = fyCenterSetHeightMoveBottomCenterBeyondTopAngle180 + fHeightSetHeightMoveBottomCenterBeyondTopAngle180 / 2.0;
    QPointF ptCenterSetHeightMoveBottomCenterBeyondTopAngle180(fxCenterSetHeightMoveBottomCenterBeyondTopAngle180, fyCenterSetHeightMoveBottomCenterBeyondTopAngle180);
    QPointF ptTopLeftSetHeightMoveBottomCenterBeyondTopAngle180(fLeftSetHeightMoveBottomCenterBeyondTopAngle180, fTopSetHeightMoveBottomCenterBeyondTopAngle180);
    QPointF ptTopRightSetHeightMoveBottomCenterBeyondTopAngle180(fRightSetHeightMoveBottomCenterBeyondTopAngle180, fTopSetHeightMoveBottomCenterBeyondTopAngle180);
    QPointF ptBottomRightSetHeightMoveBottomCenterBeyondTopAngle180(fRightSetHeightMoveBottomCenterBeyondTopAngle180, fBottomSetHeightMoveBottomCenterBeyondTopAngle180);
    QPointF ptBottomLeftSetHeightMoveBottomCenterBeyondTopAngle180(fLeftSetHeightMoveBottomCenterBeyondTopAngle180, fBottomSetHeightMoveBottomCenterBeyondTopAngle180);
    QPointF ptTopCenterSetHeightMoveBottomCenterBeyondTopAngle180(fxCenterSetHeightMoveBottomCenterBeyondTopAngle180, fTopSetHeightMoveBottomCenterBeyondTopAngle180);
    QPointF ptRightCenterSetHeightMoveBottomCenterBeyondTopAngle180(fRightSetHeightMoveBottomCenterBeyondTopAngle180, fyCenterSetHeightMoveBottomCenterBeyondTopAngle180);
    QPointF ptBottomCenterSetHeightMoveBottomCenterBeyondTopAngle180(fxCenterSetHeightMoveBottomCenterBeyondTopAngle180, fBottomSetHeightMoveBottomCenterBeyondTopAngle180);
    QPointF ptLeftCenterSetHeightMoveBottomCenterBeyondTopAngle180(fLeftSetHeightMoveBottomCenterBeyondTopAngle180, fyCenterSetHeightMoveBottomCenterBeyondTopAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenterBeyondTopAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenterBeyondTopAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenterBeyondTopAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenterBeyondTopAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenterBeyondTopAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenterBeyondTopAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenterBeyondTopAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenterBeyondTopAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenterBeyondTopAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenterBeyondTopAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenterBeyondTopAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenterBeyondTopAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenterBeyondTopAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenterBeyondTopAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenterBeyondTopAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenterBeyondTopAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenterBeyondTopAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenterBeyondTopAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenterBeyondTopAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenterBeyondTopAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenterBeyondTopAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenterBeyondTopAngle180) + "} mm");
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
    QPointF ptTopLeftAngle180(300.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopLeftAngle180.setY(400.0);
    }
    double fxCenterSetTopLeftAngle180 = ptTopLeftAngle180.x() + (fRightSetAngle180 - ptTopLeftAngle180.x()) / 2.0;
    double fyCenterSetTopLeftAngle180 = ptTopLeftAngle180.y() + (fBottomSetAngle180 - ptTopLeftAngle180.y()) / 2.0;
    double fWidthSetTopLeftAngle180 = fabs(fRightSetAngle180 - ptTopLeftAngle180.x());
    double fHeightSetTopLeftAngle180 = fabs(fBottomSetAngle180 - ptTopLeftAngle180.y());
    double fLeftSetTopLeftAngle180 = fxCenterSetTopLeftAngle180 - fWidthSetTopLeftAngle180 / 2.0;
    double fRightSetTopLeftAngle180 = fxCenterSetTopLeftAngle180 + fWidthSetTopLeftAngle180 / 2.0;
    double fTopSetTopLeftAngle180 = fyCenterSetTopLeftAngle180 - fHeightSetTopLeftAngle180 / 2.0;
    double fBottomSetTopLeftAngle180 = fyCenterSetTopLeftAngle180 + fHeightSetTopLeftAngle180 / 2.0;
    QPointF ptCenterSetTopLeftAngle180(fxCenterSetTopLeftAngle180, fyCenterSetTopLeftAngle180);
    QPointF ptTopLeftSetTopLeftAngle180(fLeftSetTopLeftAngle180, fTopSetTopLeftAngle180);
    QPointF ptTopRightSetTopLeftAngle180(fRightSetTopLeftAngle180, fTopSetTopLeftAngle180);
    QPointF ptBottomRightSetTopLeftAngle180(fRightSetTopLeftAngle180, fBottomSetTopLeftAngle180);
    QPointF ptBottomLeftSetTopLeftAngle180(fLeftSetTopLeftAngle180, fBottomSetTopLeftAngle180);
    QPointF ptTopCenterSetTopLeftAngle180(fxCenterSetTopLeftAngle180, fTopSetTopLeftAngle180);
    QPointF ptRightCenterSetTopLeftAngle180(fRightSetTopLeftAngle180, fyCenterSetTopLeftAngle180);
    QPointF ptBottomCenterSetTopLeftAngle180(fxCenterSetTopLeftAngle180, fBottomSetTopLeftAngle180);
    QPointF ptLeftCenterSetTopLeftAngle180(fLeftSetTopLeftAngle180, fyCenterSetTopLeftAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(" + qPoint2Str(ptTopLeftAngle180) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptTopLeftAngle180) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptTopLeftAngle180);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftAngle180) + "} mm");
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
    QPointF ptTopLeftBeyondBottomRightAngle180(500.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopLeftBeyondBottomRightAngle180.setY(200.0);
    }
    double fxCenterSetTopLeftBeyondBottomRightAngle180 = ptTopLeftBeyondBottomRightAngle180.x() + (fRightSetAngle180 - ptTopLeftBeyondBottomRightAngle180.x()) / 2.0;
    double fyCenterSetTopLeftBeyondBottomRightAngle180 = ptTopLeftBeyondBottomRightAngle180.y() + (fBottomSetAngle180 - ptTopLeftBeyondBottomRightAngle180.y()) / 2.0;
    double fWidthSetTopLeftBeyondBottomRightAngle180 = fabs(fRightSetAngle180 - ptTopLeftBeyondBottomRightAngle180.x());
    double fHeightSetTopLeftBeyondBottomRightAngle180 = fabs(fBottomSetAngle180 - ptTopLeftBeyondBottomRightAngle180.y());
    double fLeftSetTopLeftBeyondBottomRightAngle180 = fxCenterSetTopLeftBeyondBottomRightAngle180 - fWidthSetTopLeftBeyondBottomRightAngle180 / 2.0;
    double fRightSetTopLeftBeyondBottomRightAngle180 = fxCenterSetTopLeftBeyondBottomRightAngle180 + fWidthSetTopLeftBeyondBottomRightAngle180 / 2.0;
    double fTopSetTopLeftBeyondBottomRightAngle180 = fyCenterSetTopLeftBeyondBottomRightAngle180 - fHeightSetTopLeftBeyondBottomRightAngle180 / 2.0;
    double fBottomSetTopLeftBeyondBottomRightAngle180 = fyCenterSetTopLeftBeyondBottomRightAngle180 + fHeightSetTopLeftBeyondBottomRightAngle180 / 2.0;
    QPointF ptCenterSetTopLeftBeyondBottomRightAngle180(fxCenterSetTopLeftBeyondBottomRightAngle180, fyCenterSetTopLeftBeyondBottomRightAngle180);
    QPointF ptTopLeftSetTopLeftBeyondBottomRightAngle180(fLeftSetTopLeftBeyondBottomRightAngle180, fTopSetTopLeftBeyondBottomRightAngle180);
    QPointF ptTopRightSetTopLeftBeyondBottomRightAngle180(fRightSetTopLeftBeyondBottomRightAngle180, fTopSetTopLeftBeyondBottomRightAngle180);
    QPointF ptBottomRightSetTopLeftBeyondBottomRightAngle180(fRightSetTopLeftBeyondBottomRightAngle180, fBottomSetTopLeftBeyondBottomRightAngle180);
    QPointF ptBottomLeftSetTopLeftBeyondBottomRightAngle180(fLeftSetTopLeftBeyondBottomRightAngle180, fBottomSetTopLeftBeyondBottomRightAngle180);
    QPointF ptTopCenterSetTopLeftBeyondBottomRightAngle180(fxCenterSetTopLeftBeyondBottomRightAngle180, fTopSetTopLeftBeyondBottomRightAngle180);
    QPointF ptRightCenterSetTopLeftBeyondBottomRightAngle180(fRightSetTopLeftBeyondBottomRightAngle180, fyCenterSetTopLeftBeyondBottomRightAngle180);
    QPointF ptBottomCenterSetTopLeftBeyondBottomRightAngle180(fxCenterSetTopLeftBeyondBottomRightAngle180, fBottomSetTopLeftBeyondBottomRightAngle180);
    QPointF ptLeftCenterSetTopLeftBeyondBottomRightAngle180(fLeftSetTopLeftBeyondBottomRightAngle180, fyCenterSetTopLeftBeyondBottomRightAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(" + qPoint2Str(ptTopLeftBeyondBottomRightAngle180) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptTopLeftBeyondBottomRightAngle180) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptTopLeftBeyondBottomRightAngle180);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondBottomRightAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondBottomRightAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondBottomRightAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondBottomRightAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondBottomRightAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondBottomRightAngle180) + "} mm");
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
    QPointF ptTopRightAngle180(300.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopRightAngle180.setY(400.0);
    }
    double fxCenterSetTopRightAngle180 = fLeftSetAngle180 + (ptTopRightAngle180.x() - fLeftSetAngle180) / 2.0;
    double fyCenterSetTopRightAngle180 = ptTopRightAngle180.y() + (fBottomSetAngle180 - ptTopRightAngle180.y()) / 2.0;
    double fWidthSetTopRightAngle180 = fabs(ptTopRightAngle180.x() - fLeftSetAngle180);
    double fHeightSetTopRightAngle180 = fabs(fBottomSetAngle180 - ptTopRightAngle180.y());
    double fLeftSetTopRightAngle180 = fxCenterSetTopRightAngle180 - fWidthSetTopRightAngle180 / 2.0;
    double fRightSetTopRightAngle180 = fxCenterSetTopRightAngle180 + fWidthSetTopRightAngle180 / 2.0;
    double fTopSetTopRightAngle180 = fyCenterSetTopRightAngle180 - fHeightSetTopRightAngle180 / 2.0;
    double fBottomSetTopRightAngle180 = fyCenterSetTopRightAngle180 + fHeightSetTopRightAngle180 / 2.0;
    QPointF ptCenterSetTopRightAngle180(fxCenterSetTopRightAngle180, fyCenterSetTopRightAngle180);
    QPointF ptTopLeftSetTopRightAngle180(fLeftSetTopRightAngle180, fTopSetTopRightAngle180);
    QPointF ptTopRightSetTopRightAngle180(fRightSetTopRightAngle180, fTopSetTopRightAngle180);
    QPointF ptBottomRightSetTopRightAngle180(fRightSetTopRightAngle180, fBottomSetTopRightAngle180);
    QPointF ptBottomLeftSetTopRightAngle180(fLeftSetTopRightAngle180, fBottomSetTopRightAngle180);
    QPointF ptTopCenterSetTopRightAngle180(fxCenterSetTopRightAngle180, fTopSetTopRightAngle180);
    QPointF ptRightCenterSetTopRightAngle180(fRightSetTopRightAngle180, fyCenterSetTopRightAngle180);
    QPointF ptBottomCenterSetTopRightAngle180(fxCenterSetTopRightAngle180, fBottomSetTopRightAngle180);
    QPointF ptLeftCenterSetTopRightAngle180(fLeftSetTopRightAngle180, fyCenterSetTopRightAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(" + qPoint2Str(ptTopRightAngle180) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptTopRightAngle180) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptTopRightAngle180);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightAngle180) + "} mm");
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
    QPointF ptTopRightBeyondBottomLeftAngle180(100.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopRightBeyondBottomLeftAngle180.setY(200.0);
    }
    double fxCenterSetTopRightBeyondBottomLeftAngle180 = fLeftSetAngle180 + (ptTopRightBeyondBottomLeftAngle180.x() - fLeftSetAngle180) / 2.0;
    double fyCenterSetTopRightBeyondBottomLeftAngle180 = ptTopRightBeyondBottomLeftAngle180.y() + (fBottomSetAngle180 - ptTopRightBeyondBottomLeftAngle180.y()) / 2.0;
    double fWidthSetTopRightBeyondBottomLeftAngle180 = fabs(ptTopRightBeyondBottomLeftAngle180.x() - fLeftSetAngle180);
    double fHeightSetTopRightBeyondBottomLeftAngle180 = fabs(fBottomSetAngle180 - ptTopRightBeyondBottomLeftAngle180.y());
    double fLeftSetTopRightBeyondBottomLeftAngle180 = fxCenterSetTopRightBeyondBottomLeftAngle180 - fWidthSetTopRightBeyondBottomLeftAngle180 / 2.0;
    double fRightSetTopRightBeyondBottomLeftAngle180 = fxCenterSetTopRightBeyondBottomLeftAngle180 + fWidthSetTopRightBeyondBottomLeftAngle180 / 2.0;
    double fTopSetTopRightBeyondBottomLeftAngle180 = fyCenterSetTopRightBeyondBottomLeftAngle180 - fHeightSetTopRightBeyondBottomLeftAngle180 / 2.0;
    double fBottomSetTopRightBeyondBottomLeftAngle180 = fyCenterSetTopRightBeyondBottomLeftAngle180 + fHeightSetTopRightBeyondBottomLeftAngle180 / 2.0;
    QPointF ptCenterSetTopRightBeyondBottomLeftAngle180(fxCenterSetTopRightBeyondBottomLeftAngle180, fyCenterSetTopRightBeyondBottomLeftAngle180);
    QPointF ptTopLeftSetTopRightBeyondBottomLeftAngle180(fLeftSetTopRightBeyondBottomLeftAngle180, fTopSetTopRightBeyondBottomLeftAngle180);
    QPointF ptTopRightSetTopRightBeyondBottomLeftAngle180(fRightSetTopRightBeyondBottomLeftAngle180, fTopSetTopRightBeyondBottomLeftAngle180);
    QPointF ptBottomRightSetTopRightBeyondBottomLeftAngle180(fRightSetTopRightBeyondBottomLeftAngle180, fBottomSetTopRightBeyondBottomLeftAngle180);
    QPointF ptBottomLeftSetTopRightBeyondBottomLeftAngle180(fLeftSetTopRightBeyondBottomLeftAngle180, fBottomSetTopRightBeyondBottomLeftAngle180);
    QPointF ptTopCenterSetTopRightBeyondBottomLeftAngle180(fxCenterSetTopRightBeyondBottomLeftAngle180, fTopSetTopRightBeyondBottomLeftAngle180);
    QPointF ptRightCenterSetTopRightBeyondBottomLeftAngle180(fRightSetTopRightBeyondBottomLeftAngle180, fyCenterSetTopRightBeyondBottomLeftAngle180);
    QPointF ptBottomCenterSetTopRightBeyondBottomLeftAngle180(fxCenterSetTopRightBeyondBottomLeftAngle180, fBottomSetTopRightBeyondBottomLeftAngle180);
    QPointF ptLeftCenterSetTopRightBeyondBottomLeftAngle180(fLeftSetTopRightBeyondBottomLeftAngle180, fyCenterSetTopRightBeyondBottomLeftAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(" + qPoint2Str(ptTopRightBeyondBottomLeftAngle180) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptTopRightBeyondBottomLeftAngle180) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptTopRightBeyondBottomLeftAngle180);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondBottomLeftAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondBottomLeftAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondBottomLeftAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondBottomLeftAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondBottomLeftAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondBottomLeftAngle180) + "} mm");
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
    ptBottomRight = QPointF(300.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomRight.setY(200.0);
    }
    double fxCenterSetBottomRightAngle180 = fLeftSetAngle180 + (ptBottomRight.x() - fLeftSetAngle180) / 2.0;
    double fyCenterSetBottomRightAngle180 = fTopSetAngle180 + (ptBottomRight.y() - fTopSetAngle180) / 2.0;
    double fWidthSetBottomRightAngle180 = fabs(ptBottomRight.x() - fLeftSetAngle180);
    double fHeightSetBottomRightAngle180 = fabs(ptBottomRight.y() - fTopSetAngle180);
    double fLeftSetBottomRightAngle180 = fxCenterSetBottomRightAngle180 - fWidthSetBottomRightAngle180 / 2.0;
    double fRightSetBottomRightAngle180 = fxCenterSetBottomRightAngle180 + fWidthSetBottomRightAngle180 / 2.0;
    double fTopSetBottomRightAngle180 = fyCenterSetBottomRightAngle180 - fHeightSetBottomRightAngle180 / 2.0;
    double fBottomSetBottomRightAngle180 = fyCenterSetBottomRightAngle180 + fHeightSetBottomRightAngle180 / 2.0;
    QPointF ptCenterSetBottomRightAngle180(fxCenterSetBottomRightAngle180, fyCenterSetBottomRightAngle180);
    QPointF ptTopLeftSetBottomRightAngle180(fLeftSetBottomRightAngle180, fTopSetBottomRightAngle180);
    QPointF ptTopRightSetBottomRightAngle180(fRightSetBottomRightAngle180, fTopSetBottomRightAngle180);
    QPointF ptBottomRightSetBottomRightAngle180(fRightSetBottomRightAngle180, fBottomSetBottomRightAngle180);
    QPointF ptBottomLeftSetBottomRightAngle180(fLeftSetBottomRightAngle180, fBottomSetBottomRightAngle180);
    QPointF ptTopCenterSetBottomRightAngle180(fxCenterSetBottomRightAngle180, fTopSetBottomRightAngle180);
    QPointF ptRightCenterSetBottomRightAngle180(fRightSetBottomRightAngle180, fyCenterSetBottomRightAngle180);
    QPointF ptBottomCenterSetBottomRightAngle180(fxCenterSetBottomRightAngle180, fBottomSetBottomRightAngle180);
    QPointF ptLeftCenterSetBottomRightAngle180(fLeftSetBottomRightAngle180, fyCenterSetBottomRightAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(" + qPoint2Str(ptBottomRight) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptBottomRight) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptBottomRight);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightAngle180) + "} mm");
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
    ptBottomRightBeyondTopLeft = QPointF(100.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomRightBeyondTopLeft.setY(400.0);
    }
    double fxCenterSetBottomRightBeyondTopLeftAngle180 = fLeftSetAngle180 + (ptBottomRightBeyondTopLeft.x() - fLeftSetAngle180) / 2.0;
    double fyCenterSetBottomRightBeyondTopLeftAngle180 = fTopSetAngle180 + (ptBottomRightBeyondTopLeft.y() - fTopSetAngle180) / 2.0;
    double fWidthSetBottomRightBeyondTopLeftAngle180 = fabs(ptBottomRightBeyondTopLeft.x() - fLeftSetAngle180);
    double fHeightSetBottomRightBeyondTopLeftAngle180 = fabs(ptBottomRightBeyondTopLeft.y() - fTopSetAngle180);
    double fLeftSetBottomRightBeyondTopLeftAngle180 = fxCenterSetBottomRightBeyondTopLeftAngle180 - fWidthSetBottomRightBeyondTopLeftAngle180 / 2.0;
    double fRightSetBottomRightBeyondTopLeftAngle180 = fxCenterSetBottomRightBeyondTopLeftAngle180 + fWidthSetBottomRightBeyondTopLeftAngle180 / 2.0;
    double fTopSetBottomRightBeyondTopLeftAngle180 = fyCenterSetBottomRightBeyondTopLeftAngle180 - fHeightSetBottomRightBeyondTopLeftAngle180 / 2.0;
    double fBottomSetBottomRightBeyondTopLeftAngle180 = fyCenterSetBottomRightBeyondTopLeftAngle180 + fHeightSetBottomRightBeyondTopLeftAngle180 / 2.0;
    QPointF ptCenterSetBottomRightBeyondTopLeftAngle180(fxCenterSetBottomRightBeyondTopLeftAngle180, fyCenterSetBottomRightBeyondTopLeftAngle180);
    QPointF ptTopLeftSetBottomRightBeyondTopLeftAngle180(fLeftSetBottomRightBeyondTopLeftAngle180, fTopSetBottomRightBeyondTopLeftAngle180);
    QPointF ptTopRightSetBottomRightBeyondTopLeftAngle180(fRightSetBottomRightBeyondTopLeftAngle180, fTopSetBottomRightBeyondTopLeftAngle180);
    QPointF ptBottomRightSetBottomRightBeyondTopLeftAngle180(fRightSetBottomRightBeyondTopLeftAngle180, fBottomSetBottomRightBeyondTopLeftAngle180);
    QPointF ptBottomLeftSetBottomRightBeyondTopLeftAngle180(fLeftSetBottomRightBeyondTopLeftAngle180, fBottomSetBottomRightBeyondTopLeftAngle180);
    QPointF ptTopCenterSetBottomRightBeyondTopLeftAngle180(fxCenterSetBottomRightBeyondTopLeftAngle180, fTopSetBottomRightBeyondTopLeftAngle180);
    QPointF ptRightCenterSetBottomRightBeyondTopLeftAngle180(fRightSetBottomRightBeyondTopLeftAngle180, fyCenterSetBottomRightBeyondTopLeftAngle180);
    QPointF ptBottomCenterSetBottomRightBeyondTopLeftAngle180(fxCenterSetBottomRightBeyondTopLeftAngle180, fBottomSetBottomRightBeyondTopLeftAngle180);
    QPointF ptLeftCenterSetBottomRightBeyondTopLeftAngle180(fLeftSetBottomRightBeyondTopLeftAngle180, fyCenterSetBottomRightBeyondTopLeftAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(" + qPoint2Str(ptBottomRightBeyondTopLeft) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptBottomRightBeyondTopLeft) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptBottomRightBeyondTopLeft);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondTopLeftAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondTopLeftAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondTopLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondTopLeftAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondTopLeftAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondTopLeftAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondTopLeftAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondTopLeftAngle180) + "} mm");
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
    ptBottomLeft = QPointF(300.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomLeft.setY(200.0);
    }
    double fxCenterSetBottomLeftAngle180 = ptBottomLeft.x() + (fRightSetAngle180 - ptBottomLeft.x()) / 2.0;
    double fyCenterSetBottomLeftAngle180 = fTopSetAngle180 + (ptBottomLeft.y() - fTopSetAngle180) / 2.0;
    double fWidthSetBottomLeftAngle180 = fabs(fRightSetAngle180 - ptBottomLeft.x());
    double fHeightSetBottomLeftAngle180 = fabs(ptBottomLeft.y() - fTopSetAngle180);
    double fLeftSetBottomLeftAngle180 = fxCenterSetBottomLeftAngle180 - fWidthSetBottomLeftAngle180 / 2.0;
    double fRightSetBottomLeftAngle180 = fxCenterSetBottomLeftAngle180 + fWidthSetBottomLeftAngle180 / 2.0;
    double fTopSetBottomLeftAngle180 = fyCenterSetBottomLeftAngle180 - fHeightSetBottomLeftAngle180 / 2.0;
    double fBottomSetBottomLeftAngle180 = fyCenterSetBottomLeftAngle180 + fHeightSetBottomLeftAngle180 / 2.0;
    QPointF ptCenterSetBottomLeftAngle180(fxCenterSetBottomLeftAngle180, fyCenterSetBottomLeftAngle180);
    QPointF ptTopLeftSetBottomLeftAngle180(fLeftSetBottomLeftAngle180, fTopSetBottomLeftAngle180);
    QPointF ptTopRightSetBottomLeftAngle180(fRightSetBottomLeftAngle180, fTopSetBottomLeftAngle180);
    QPointF ptBottomRightSetBottomLeftAngle180(fRightSetBottomLeftAngle180, fBottomSetBottomLeftAngle180);
    QPointF ptBottomLeftSetBottomLeftAngle180(fLeftSetBottomLeftAngle180, fBottomSetBottomLeftAngle180);
    QPointF ptTopCenterSetBottomLeftAngle180(fxCenterSetBottomLeftAngle180, fTopSetBottomLeftAngle180);
    QPointF ptRightCenterSetBottomLeftAngle180(fRightSetBottomLeftAngle180, fyCenterSetBottomLeftAngle180);
    QPointF ptBottomCenterSetBottomLeftAngle180(fxCenterSetBottomLeftAngle180, fBottomSetBottomLeftAngle180);
    QPointF ptLeftCenterSetBottomLeftAngle180(fLeftSetBottomLeftAngle180, fyCenterSetBottomLeftAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(" + qPoint2Str(ptBottomLeft) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptBottomLeft) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptBottomLeft);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftAngle180) + "} mm");
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
    ptBottomLeftBeyondTopRight = QPointF(500.0, 200.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomLeftBeyondTopRight.setY(400.0);
    }
    double fxCenterSetBottomLeftBeyondTopRightAngle180 = ptBottomLeftBeyondTopRight.x() + (fRightSetAngle180 - ptBottomLeftBeyondTopRight.x()) / 2.0;
    double fyCenterSetBottomLeftBeyondTopRightAngle180 = fTopSetAngle180 + (ptBottomLeftBeyondTopRight.y() - fTopSetAngle180) / 2.0;
    double fWidthSetBottomLeftBeyondTopRightAngle180 = fabs(fRightSetAngle180 - ptBottomLeftBeyondTopRight.x());
    double fHeightSetBottomLeftBeyondTopRightAngle180 = fabs(ptBottomLeftBeyondTopRight.y() - fTopSetAngle180);
    double fLeftSetBottomLeftBeyondTopRightAngle180 = fxCenterSetBottomLeftBeyondTopRightAngle180 - fWidthSetBottomLeftBeyondTopRightAngle180 / 2.0;
    double fRightSetBottomLeftBeyondTopRightAngle180 = fxCenterSetBottomLeftBeyondTopRightAngle180 + fWidthSetBottomLeftBeyondTopRightAngle180 / 2.0;
    double fTopSetBottomLeftBeyondTopRightAngle180 = fyCenterSetBottomLeftBeyondTopRightAngle180 - fHeightSetBottomLeftBeyondTopRightAngle180 / 2.0;
    double fBottomSetBottomLeftBeyondTopRightAngle180 = fyCenterSetBottomLeftBeyondTopRightAngle180 + fHeightSetBottomLeftBeyondTopRightAngle180 / 2.0;
    QPointF ptCenterSetBottomLeftBeyondTopRightAngle180(fxCenterSetBottomLeftBeyondTopRightAngle180, fyCenterSetBottomLeftBeyondTopRightAngle180);
    QPointF ptTopLeftSetBottomLeftBeyondTopRightAngle180(fLeftSetBottomLeftBeyondTopRightAngle180, fTopSetBottomLeftBeyondTopRightAngle180);
    QPointF ptTopRightSetBottomLeftBeyondTopRightAngle180(fRightSetBottomLeftBeyondTopRightAngle180, fTopSetBottomLeftBeyondTopRightAngle180);
    QPointF ptBottomRightSetBottomLeftBeyondTopRightAngle180(fRightSetBottomLeftBeyondTopRightAngle180, fBottomSetBottomLeftBeyondTopRightAngle180);
    QPointF ptBottomLeftSetBottomLeftBeyondTopRightAngle180(fLeftSetBottomLeftBeyondTopRightAngle180, fBottomSetBottomLeftBeyondTopRightAngle180);
    QPointF ptTopCenterSetBottomLeftBeyondTopRightAngle180(fxCenterSetBottomLeftBeyondTopRightAngle180, fTopSetBottomLeftBeyondTopRightAngle180);
    QPointF ptRightCenterSetBottomLeftBeyondTopRightAngle180(fRightSetBottomLeftBeyondTopRightAngle180, fyCenterSetBottomLeftBeyondTopRightAngle180);
    QPointF ptBottomCenterSetBottomLeftBeyondTopRightAngle180(fxCenterSetBottomLeftBeyondTopRightAngle180, fBottomSetBottomLeftBeyondTopRightAngle180);
    QPointF ptLeftCenterSetBottomLeftBeyondTopRightAngle180(fLeftSetBottomLeftBeyondTopRightAngle180, fyCenterSetBottomLeftBeyondTopRightAngle180);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(" + qPoint2Str(ptBottomLeftBeyondTopRight) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptBottomLeftBeyondTopRight) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_180_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptBottomLeftBeyondTopRight);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondTopRightAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondTopRightAngle180, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondTopRightAngle180, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondTopRightAngle180, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondTopRightAngle180) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondTopRightAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondTopRightAngle180, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondTopRightAngle180) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondTopRightAngle180) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondTopRightAngle180) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondTopRightAngle180) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondTopRightAngle180) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondTopRightAngle180) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondTopRightAngle180) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondTopRightAngle180) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    //--------------------------------------------------------------------------
    // Rotation Angle: 30.0°
    //--------------------------------------------------------------------------

    physValAngle.setVal(30.0);
    physValAngleCorrected.setVal(210.0);

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
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("addPhysValShape", "");
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
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setCenter", ptCenter);
    pTestStep->setConfigValue("setCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
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
    double fWidthSetSizeAngle30 = size300x200.width();
    double fHeightSetSizeAngle30 = size300x200.height();
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
        /* strName         */ "Step " + QString::number(++idxStep) + " setSize(" + qSize2Str(size300x200) + " " + unit.symbol() + ")",
        /* strOperation    */ "setSize(" + qSize2Str(size300x200) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setSize", size300x200);
    pTestStep->setConfigValue("setSize.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetSizeAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetSizeAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetSizeAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetSizeAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetSizeAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetSizeAngle30, 1), 'f', 1) + " mm");
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

    /* setWidth(300)
    ----------------
    */
    double fWidthSetWidthAngle30 = physValWidth300.getVal();
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
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidth(" + physValWidth300.toString() + ")",
        /* strOperation    */ "setWidth(" + physValWidth300.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidth", physValWidth300.toString());
    pTestStep->setConfigValue("setWidth.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthAngle30, 1), 'f', 1) + " mm");
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

    /* setWidthByMovingLeftCenter(100)
    ---------------------------------------
    */
    // Set width to half of original width. Thats been done by moving to the top center point of the original rectangle.
    // The resulting center point, when resizing the rectangle by moving the left edge to a new width of 100.0, is the
    // same as setting the width to 300.0 by moving the right edge of the rectangle. But only in this special case.
    ptMoved = ptTopCenterSetAngle30;
    double fWidthSetWidthMoveLeftCenterAngle30 = fWidthSetAngle30 / 2.0;
    double fHeightSetWidthMoveLeftCenterAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetWidthMoveLeftCenterAngle30 = ptCenterSetWidthAngle30;
    QPointF ptTopLeftSetWidthMoveLeftCenterAngle30 = ptTopCenterSetAngle30;
    QPointF ptTopRightSetWidthMoveLeftCenterAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetWidthMoveLeftCenterAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetWidthMoveLeftCenterAngle30 = ptBottomCenterSetAngle30;
    QPointF ptTopCenterSetWidthMoveLeftCenterAngle30 = QPointF((ptTopRightSetWidthMoveLeftCenterAngle30.x() + ptTopLeftSetWidthMoveLeftCenterAngle30.x()) / 2.0,
                                                       (ptTopRightSetWidthMoveLeftCenterAngle30.y() + ptTopLeftSetWidthMoveLeftCenterAngle30.y()) / 2.0);
    QPointF ptRightCenterSetWidthMoveLeftCenterAngle30 = ptRightCenterSetAngle30;
    QPointF ptBottomCenterSetWidthMoveLeftCenterAngle30 = QPointF((ptBottomRightSetWidthMoveLeftCenterAngle30.x() + ptBottomLeftSetWidthMoveLeftCenterAngle30.x()) / 2.0,
                                                          (ptBottomRightSetWidthMoveLeftCenterAngle30.y() + ptBottomLeftSetWidthMoveLeftCenterAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetWidthMoveLeftCenterAngle30 = QPointF((ptTopLeftSetWidthMoveLeftCenterAngle30.x() + ptBottomLeftSetWidthMoveLeftCenterAngle30.x()) / 2.0,
                                                        (ptBottomLeftSetWidthMoveLeftCenterAngle30.y() + ptTopLeftSetWidthMoveLeftCenterAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingLeftCenter(100)
    ---------------------------------------
    */
    // Set width to half of original width. Thats been done by moving to the top center point of the original rectangle.
    // The resulting center point, when resizing the rectangle by moving the left edge to a new width of 100.0, is the
    // same as setting the width to 300.0 by moving the right edge of the rectangle. But only in this special case.
    ptMoved = QPointF(ptTopRightSetAngle30.x() + (ptTopRightSetAngle30.x() - ptTopLeftSetAngle30.x()) / 2.0,
                      ptTopRightSetAngle30.y() + (ptTopRightSetAngle30.y() - ptTopLeftSetAngle30.y()) / 2.0);
    double fWidthSetWidthMoveLeftCenterBeyondRightAngle30 = fWidthSetAngle30 / 2.0;
    double fHeightSetWidthMoveLeftCenterBeyondRightAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetWidthMoveLeftCenterBeyondRightAngle30(429.9, 375.0);
    QPointF ptTopLeftSetWidthMoveLeftCenterBeyondRightAngle30 = ptMoved;
    QPointF ptTopRightSetWidthMoveLeftCenterBeyondRightAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetWidthMoveLeftCenterBeyondRightAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetWidthMoveLeftCenterBeyondRightAngle30(448.2, 443.3);
    QPointF ptTopCenterSetWidthMoveLeftCenterBeyondRightAngle30 = QPointF((ptTopRightSetWidthMoveLeftCenterBeyondRightAngle30.x() + ptTopLeftSetWidthMoveLeftCenterBeyondRightAngle30.x()) / 2.0,
                                                       (ptTopRightSetWidthMoveLeftCenterBeyondRightAngle30.y() + ptTopLeftSetWidthMoveLeftCenterBeyondRightAngle30.y()) / 2.0);
    QPointF ptRightCenterSetWidthMoveLeftCenterBeyondRightAngle30 = ptRightCenterSetAngle30;
    QPointF ptBottomCenterSetWidthMoveLeftCenterBeyondRightAngle30 = QPointF((ptBottomRightSetWidthMoveLeftCenterBeyondRightAngle30.x() + ptBottomLeftSetWidthMoveLeftCenterBeyondRightAngle30.x()) / 2.0,
                                                          (ptBottomRightSetWidthMoveLeftCenterBeyondRightAngle30.y() + ptBottomLeftSetWidthMoveLeftCenterBeyondRightAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetWidthMoveLeftCenterBeyondRightAngle30 = QPointF((ptTopLeftSetWidthMoveLeftCenterBeyondRightAngle30.x() + ptBottomLeftSetWidthMoveLeftCenterBeyondRightAngle30.x()) / 2.0,
                                                        (ptBottomLeftSetWidthMoveLeftCenterBeyondRightAngle30.y() + ptTopLeftSetWidthMoveLeftCenterBeyondRightAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingLeftCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingLeftCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingLeftCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterBeyondRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterBeyondRightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterBeyondRightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterBeyondRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterBeyondRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterBeyondRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterBeyondRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterBeyondRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterBeyondRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterBeyondRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterBeyondRightAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveLeftCenterBeyondRightAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveLeftCenterBeyondRightAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveLeftCenterBeyondRightAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveLeftCenterBeyondRightAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveLeftCenterBeyondRightAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveLeftCenterBeyondRightAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveLeftCenterBeyondRightAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveLeftCenterBeyondRightAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveLeftCenterBeyondRightAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveLeftCenterBeyondRightAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveLeftCenterBeyondRightAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingRightCenter(100)
    ----------------------------------------
    */
    // Same width chosen by moving right center as with setWidth call above.
    ptMoved = ptTopCenterSetAngle30;
    double fWidthSetWidthMoveRightCenterAngle30 = fWidthSetAngle30 / 2.0;
    double fHeightSetWidthMoveRightCenterAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetWidthMoveRightCenterAngle30(256.7, 275.0);
    QPointF ptTopLeftSetWidthMoveRightCenterAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetWidthMoveRightCenterAngle30 = ptTopCenterSetAngle30;
    QPointF ptBottomRightSetWidthMoveRightCenterAngle30 = ptBottomCenterSetAngle30;
    QPointF ptBottomLeftSetWidthMoveRightCenterAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetWidthMoveRightCenterAngle30 = QPointF((ptTopRightSetWidthMoveRightCenterAngle30.x() + ptTopLeftSetWidthMoveRightCenterAngle30.x()) / 2.0,
                                                        (ptTopRightSetWidthMoveRightCenterAngle30.y() + ptTopLeftSetWidthMoveRightCenterAngle30.y()) / 2.0);
    QPointF ptRightCenterSetWidthMoveRightCenterAngle30 = QPointF((ptTopRightSetWidthMoveRightCenterAngle30.x() + ptBottomRightSetWidthMoveRightCenterAngle30.x()) / 2.0,
                                                          (ptTopRightSetWidthMoveRightCenterAngle30.y() + ptBottomRightSetWidthMoveRightCenterAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetWidthMoveRightCenterAngle30 = QPointF((ptBottomRightSetWidthMoveRightCenterAngle30.x() + ptBottomLeftSetWidthMoveRightCenterAngle30.x()) / 2.0,
                                                           (ptBottomRightSetWidthMoveRightCenterAngle30.y() + ptBottomLeftSetWidthMoveRightCenterAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetWidthMoveRightCenterAngle30 = ptLeftCenterSetAngle30;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setWidthByMovingRightCenter(100)
    ----------------------------------------
    */
    ptMoved = QPointF(ptTopLeftSetAngle30.x() - (ptTopRightSetAngle30.x() - ptTopLeftSetAngle30.x()) / 2.0,
                      ptTopLeftSetAngle30.y() - (ptTopRightSetAngle30.y() - ptTopLeftSetAngle30.y()) / 2.0);
    double fWidthSetWidthMoveRightCenterBeyondLeftAngle30 = fWidthSetAngle30 / 2.0;
    double fHeightSetWidthMoveRightCenterBeyondLeftAngle30 = fHeightSetAngle30;
    QPointF ptCenterSetWidthMoveRightCenterBeyondLeftAngle30(170.1, 225.0);
    QPointF ptTopLeftSetWidthMoveRightCenterBeyondLeftAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetWidthMoveRightCenterBeyondLeftAngle30 = ptMoved;
    QPointF ptBottomRightSetWidthMoveRightCenterBeyondLeftAngle30(101.8, 243.3);
    QPointF ptBottomLeftSetWidthMoveRightCenterBeyondLeftAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetWidthMoveRightCenterBeyondLeftAngle30 = QPointF((ptTopRightSetWidthMoveRightCenterBeyondLeftAngle30.x() + ptTopLeftSetWidthMoveRightCenterBeyondLeftAngle30.x()) / 2.0,
                                                        (ptTopRightSetWidthMoveRightCenterBeyondLeftAngle30.y() + ptTopLeftSetWidthMoveRightCenterBeyondLeftAngle30.y()) / 2.0);
    QPointF ptRightCenterSetWidthMoveRightCenterBeyondLeftAngle30 = QPointF((ptTopRightSetWidthMoveRightCenterBeyondLeftAngle30.x() + ptBottomRightSetWidthMoveRightCenterBeyondLeftAngle30.x()) / 2.0,
                                                          (ptTopRightSetWidthMoveRightCenterBeyondLeftAngle30.y() + ptBottomRightSetWidthMoveRightCenterBeyondLeftAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetWidthMoveRightCenterBeyondLeftAngle30 = QPointF((ptBottomRightSetWidthMoveRightCenterBeyondLeftAngle30.x() + ptBottomLeftSetWidthMoveRightCenterBeyondLeftAngle30.x()) / 2.0,
                                                           (ptBottomRightSetWidthMoveRightCenterBeyondLeftAngle30.y() + ptBottomLeftSetWidthMoveRightCenterBeyondLeftAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetWidthMoveRightCenterBeyondLeftAngle30 = ptLeftCenterSetAngle30;
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setWidthByMovingRightCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setWidthByMovingRightCenter", ptMoved);
    pTestStep->setConfigValue("setWidthByMovingRightCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterBeyondLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterBeyondLeftAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterBeyondLeftAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterBeyondLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterBeyondLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterBeyondLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterBeyondLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterBeyondLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterBeyondLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterBeyondLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterBeyondLeftAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetWidthMoveRightCenterBeyondLeftAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetWidthMoveRightCenterBeyondLeftAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetWidthMoveRightCenterBeyondLeftAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetWidthMoveRightCenterBeyondLeftAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetWidthMoveRightCenterBeyondLeftAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetWidthMoveRightCenterBeyondLeftAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetWidthMoveRightCenterBeyondLeftAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetWidthMoveRightCenterBeyondLeftAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetWidthMoveRightCenterBeyondLeftAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetWidthMoveRightCenterBeyondLeftAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetWidthMoveRightCenterBeyondLeftAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeight(200)
    -----------------
    */
    double fWidthSetHeightAngle30 = fWidthSetAngle30;
    double fHeightSetHeightAngle30 = physValHeight200.getVal();
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
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeight(" + physValHeight200.toString() + ")",
        /* strOperation    */ "setHeight(" + physValHeight200.toString() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeight", physValHeight200.toString());
    pTestStep->setConfigValue("setHeight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightAngle30, 1), 'f', 1) + " mm");
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

    /* setHeightByMovingTopCenter(50)
    ---------------------------------------
    */
    ptMoved = ptRightCenterSetAngle30;
    double fWidthSetHeightMoveTopCenterAngle30 = fWidthSetAngle30;
    double fHeightSetHeightMoveTopCenterAngle30 = fHeightSetAngle30 / 2.0;
    QPointF ptCenterSetHeightMoveTopCenterAngle30(287.49, 321.7);
    QPointF ptTopLeftSetHeightMoveTopCenterAngle30 = ptLeftCenterSetAngle30;
    QPointF ptTopRightSetHeightMoveTopCenterAngle30 = ptRightCenterSetAngle30;
    QPointF ptBottomRightSetHeightMoveTopCenterAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetHeightMoveTopCenterAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetHeightMoveTopCenterAngle30 = QPointF((ptTopRightSetHeightMoveTopCenterAngle30.x() + ptTopLeftSetHeightMoveTopCenterAngle30.x()) / 2.0,
                                                      (ptTopRightSetHeightMoveTopCenterAngle30.y() + ptTopLeftSetHeightMoveTopCenterAngle30.y()) / 2.0);
    QPointF ptRightCenterSetHeightMoveTopCenterAngle30 = QPointF((ptTopRightSetHeightMoveTopCenterAngle30.x() + ptBottomRightSetHeightMoveTopCenterAngle30.x()) / 2.0,
                                                        (ptTopRightSetHeightMoveTopCenterAngle30.y() + ptBottomRightSetHeightMoveTopCenterAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetHeightMoveTopCenterAngle30 = ptBottomCenterSetAngle30;
    QPointF ptLeftCenterSetHeightMoveTopCenterAngle30 = QPointF((ptTopLeftSetHeightMoveTopCenterAngle30.x() + ptBottomLeftSetHeightMoveTopCenterAngle30.x()) / 2.0,
                                                       (ptBottomLeftSetHeightMoveTopCenterAngle30.y() + ptTopLeftSetHeightMoveTopCenterAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingTopCenter(50)
    ---------------------------------------
    */
    ptMoved = QPointF(ptBottomRightSetAngle30.x() - (ptTopRightSetAngle30.x() - ptBottomRightSetAngle30.x()) / 2.0,
                      ptBottomRightSetAngle30.y() + (ptBottomRightSetAngle30.y() - ptTopRightSetAngle30.y()) / 2.0);
    double fWidthSetHeightMoveTopCenterBeyondBottomAngle30 = fWidthSetAngle30;
    double fHeightSetHeightMoveTopCenterBeyondBottomAngle30 = fHeightSetAngle30 / 2.0;
    QPointF ptCenterSetHeightMoveTopCenterBeyondBottomAngle30(262.5, 365.0);
    QPointF ptTopLeftSetHeightMoveTopCenterBeyondBottomAngle30(163.4, 336.6);
    QPointF ptTopRightSetHeightMoveTopCenterBeyondBottomAngle30 = ptMoved;
    QPointF ptBottomRightSetHeightMoveTopCenterBeyondBottomAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetHeightMoveTopCenterBeyondBottomAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetHeightMoveTopCenterBeyondBottomAngle30 = QPointF((ptTopRightSetHeightMoveTopCenterBeyondBottomAngle30.x() + ptTopLeftSetHeightMoveTopCenterBeyondBottomAngle30.x()) / 2.0,
                                                      (ptTopRightSetHeightMoveTopCenterBeyondBottomAngle30.y() + ptTopLeftSetHeightMoveTopCenterBeyondBottomAngle30.y()) / 2.0);
    QPointF ptRightCenterSetHeightMoveTopCenterBeyondBottomAngle30(349.1, 415.0); // not calculated because of inaccuracies
    QPointF ptBottomCenterSetHeightMoveTopCenterBeyondBottomAngle30 = ptBottomCenterSetAngle30;
    QPointF ptLeftCenterSetHeightMoveTopCenterBeyondBottomAngle30(175.9, 315.0);  // not calculated because of inaccuracies
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingTopCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingTopCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingTopCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterBeyondBottomAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterBeyondBottomAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterBeyondBottomAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterBeyondBottomAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterBeyondBottomAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterBeyondBottomAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterBeyondBottomAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterBeyondBottomAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterBeyondBottomAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterBeyondBottomAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterBeyondBottomAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveTopCenterBeyondBottomAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveTopCenterBeyondBottomAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveTopCenterBeyondBottomAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveTopCenterBeyondBottomAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveTopCenterBeyondBottomAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveTopCenterBeyondBottomAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveTopCenterBeyondBottomAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveTopCenterBeyondBottomAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveTopCenterBeyondBottomAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveTopCenterBeyondBottomAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveTopCenterBeyondBottomAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingBottomCenter(50)
    ------------------------------------------
    */
    ptMoved = ptRightCenterSetAngle30;
    double fWidthSetHeightMoveBottomCenter30 = fWidthSetHeightAngle30;
    double fHeightSetHeightMoveBottomCenter30 = fHeightSetAngle30 / 2.0;
    QPointF ptCenterSetHeightMoveBottomCenter30(312.49, 278.4);
    QPointF ptTopLeftSetHeightMoveBottomCenter30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetHeightMoveBottomCenter30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetHeightMoveBottomCenter30 = ptRightCenterSetAngle30;
    QPointF ptBottomLeftSetHeightMoveBottomCenter30 = ptLeftCenterSetAngle30;
    QPointF ptTopCenterSetHeightMoveBottomCenter30 = ptTopCenterSetAngle30;
    QPointF ptRightCenterSetHeightMoveBottomCenter30 = QPointF((ptTopRightSetHeightMoveBottomCenter30.x() + ptBottomRightSetHeightMoveBottomCenter30.x()) / 2.0,
                                                           (ptTopRightSetHeightMoveBottomCenter30.y() + ptBottomRightSetHeightMoveBottomCenter30.y()) / 2.0);
    QPointF ptBottomCenterSetHeightMoveBottomCenter30 = QPointF((ptBottomLeftSetHeightMoveBottomCenter30.x() + ptBottomRightSetHeightMoveBottomCenter30.x()) / 2.0,
                                                            (ptBottomLeftSetHeightMoveBottomCenter30.y() + ptBottomRightSetHeightMoveBottomCenter30.y()) / 2.0);;
    QPointF ptLeftCenterSetHeightMoveBottomCenter30 = QPointF((ptTopLeftSetHeightMoveBottomCenter30.x() + ptBottomLeftSetHeightMoveBottomCenter30.x()) / 2.0,
                                                          (ptBottomLeftSetHeightMoveBottomCenter30.y() + ptTopLeftSetHeightMoveBottomCenter30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenter30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenter30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenter30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenter30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenter30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenter30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenter30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setHeightByMovingBottomCenter(50)
    ------------------------------------------
    */
    ptMoved = QPointF(ptTopRightSetAngle30.x() + (ptTopRightSetAngle30.x() - ptBottomRightSetAngle30.x()) / 2.0,
                      ptTopRightSetAngle30.y() - (ptBottomRightSetAngle30.y() - ptTopRightSetAngle30.y()) / 2.0);
    double fWidthSetHeightMoveBottomCenterBeyondTop30 = fWidthSetHeightAngle30;
    double fHeightSetHeightMoveBottomCenterBeyondTop30 = fHeightSetAngle30 / 2.0;
    QPointF ptCenterSetHeightMoveBottomCenterBeyondTop30(337.5, 235.1);
    QPointF ptTopLeftSetHeightMoveBottomCenterBeyondTop30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetHeightMoveBottomCenterBeyondTop30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetHeightMoveBottomCenterBeyondTop30(436.6, 263.4);
    QPointF ptBottomLeftSetHeightMoveBottomCenterBeyondTop30(263.4, 163.4);
    QPointF ptTopCenterSetHeightMoveBottomCenterBeyondTop30 = ptTopCenterSetAngle30;
    QPointF ptRightCenterSetHeightMoveBottomCenterBeyondTop30 = QPointF((ptTopRightSetHeightMoveBottomCenterBeyondTop30.x() + ptBottomRightSetHeightMoveBottomCenterBeyondTop30.x()) / 2.0,
                                                           (ptTopRightSetHeightMoveBottomCenterBeyondTop30.y() + ptBottomRightSetHeightMoveBottomCenterBeyondTop30.y()) / 2.0);
    QPointF ptBottomCenterSetHeightMoveBottomCenterBeyondTop30 = QPointF((ptBottomLeftSetHeightMoveBottomCenterBeyondTop30.x() + ptBottomRightSetHeightMoveBottomCenterBeyondTop30.x()) / 2.0,
                                                            (ptBottomLeftSetHeightMoveBottomCenterBeyondTop30.y() + ptBottomRightSetHeightMoveBottomCenterBeyondTop30.y()) / 2.0);;
    QPointF ptLeftCenterSetHeightMoveBottomCenterBeyondTop30 = QPointF((ptTopLeftSetHeightMoveBottomCenterBeyondTop30.x() + ptBottomLeftSetHeightMoveBottomCenterBeyondTop30.x()) / 2.0,
                                                          (ptBottomLeftSetHeightMoveBottomCenterBeyondTop30.y() + ptTopLeftSetHeightMoveBottomCenterBeyondTop30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* strOperation    */ "setHeightByMovingBottomCenter(" + qPoint2Str(ptMoved) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setHeightByMovingBottomCenter", ptMoved);
    pTestStep->setConfigValue("setHeightByMovingBottomCenter.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenterBeyondTop30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenterBeyondTop30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenterBeyondTop30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenterBeyondTop30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenterBeyondTop30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenterBeyondTop30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenterBeyondTop30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenterBeyondTop30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenterBeyondTop30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenterBeyondTop30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenterBeyondTop30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetHeightMoveBottomCenterBeyondTop30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetHeightMoveBottomCenterBeyondTop30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetHeightMoveBottomCenterBeyondTop30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetHeightMoveBottomCenterBeyondTop30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetHeightMoveBottomCenterBeyondTop30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetHeightMoveBottomCenterBeyondTop30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetHeightMoveBottomCenterBeyondTop30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetHeightMoveBottomCenterBeyondTop30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetHeightMoveBottomCenterBeyondTop30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetHeightMoveBottomCenterBeyondTop30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetHeightMoveBottomCenterBeyondTop30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopLeft(200, 100)
    -----------------------
    */
    QPointF ptTopLeftAngle30 = QPointF(200.0, 100.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopLeftAngle30.setY(500.0);
    }
    double fWidthSetTopLeftAngle30 = 286.6;
    double fHeightSetTopLeftAngle30 = 173.2;
    QPointF ptCenterSetTopLeftAngle30(280.8, 246.7);
    QPointF ptTopLeftSetTopLeftAngle30 = ptTopLeftAngle30;
    QPointF ptTopRightSetTopLeftAngle30(448.2, 243.3);
    QPointF ptBottomRightSetTopLeftAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetTopLeftAngle30(113.4, 250.0);
    QPointF ptTopCenterSetTopLeftAngle30 = QPointF((ptTopRightSetTopLeftAngle30.x() + ptTopLeftSetTopLeftAngle30.x()) / 2.0,
                                                (ptTopRightSetTopLeftAngle30.y() + ptTopLeftSetTopLeftAngle30.y()) / 2.0);
    QPointF ptRightCenterSetTopLeftAngle30 = QPointF((ptTopRightSetTopLeftAngle30.x() + ptBottomRightSetTopLeftAngle30.x()) / 2.0,
                                                  (ptBottomRightSetTopLeftAngle30.y() + ptTopRightSetTopLeftAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetTopLeftAngle30(237.49, 321.7); // not calucated because of inaccuracy
    QPointF ptLeftCenterSetTopLeftAngle30 = QPointF((ptTopLeftSetTopLeftAngle30.x() + ptBottomLeftSetTopLeftAngle30.x()) / 2.0,
                                                 (ptBottomLeftSetTopLeftAngle30.y() + ptTopLeftSetTopLeftAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(" + qPoint2Str(ptTopLeftAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptTopLeftAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptTopLeftAngle30);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftAngle30, 1), 'f', 1) + " mm");
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

    /* setTopLeft(450, 550)
    -----------------------
    */
    QPointF ptTopLeftBeyondBottomRightAngle30 = QPointF(450.0, 550.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopLeftBeyondBottomRightAngle30.setY(50.0);
    }
    double fWidthSetTopLeftBeyondBottomRightAngle30 = 154.9;
    double fHeightSetTopLeftBeyondBottomRightAngle30 = 91.5;
    QPointF ptCenterSetTopLeftBeyondBottomRightAngle30(405.8, 471.7);
    QPointF ptTopLeftSetTopLeftBeyondBottomRightAngle30 = ptTopLeftBeyondBottomRightAngle30;
    QPointF ptTopRightSetTopLeftBeyondBottomRightAngle30(315.9, 472.5);
    QPointF ptBottomRightSetTopLeftBeyondBottomRightAngle30 = ptBottomRightSetAngle30;
    QPointF ptBottomLeftSetTopLeftBeyondBottomRightAngle30(495.8, 470.8);
    QPointF ptTopCenterSetTopLeftBeyondBottomRightAngle30 = QPointF(382.9, 511.3);
    QPointF ptRightCenterSetTopLeftBeyondBottomRightAngle30 = QPointF(338.7, 432.9);
    QPointF ptBottomCenterSetTopLeftBeyondBottomRightAngle30 = QPointF(428.7, 432.0);
    QPointF ptLeftCenterSetTopLeftBeyondBottomRightAngle30 = QPointF(472.9, 510.4);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopLeft(" + qPoint2Str(ptTopLeftBeyondBottomRightAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopLeft(" + qPoint2Str(ptTopLeftBeyondBottomRightAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopLeft", ptTopLeftBeyondBottomRightAngle30);
    pTestStep->setConfigValue("setTopLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondBottomRightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondBottomRightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondBottomRightAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopLeftBeyondBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopLeftBeyondBottomRightAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopLeftBeyondBottomRightAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopLeftBeyondBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopLeftBeyondBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopLeftBeyondBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopLeftBeyondBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopLeftBeyondBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopLeftBeyondBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopLeftBeyondBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopLeftBeyondBottomRightAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setTopRight(550, 250)
    ------------------------
    */
    QPointF ptTopRightAngle30 = QPointF(550.0, 250.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopRightAngle30.setY(350.0);
    }
    double fWidthSetTopRightAngle30 = 291.5;
    double fHeightSetTopRightAngle30 = 218.3;
    QPointF ptCenterSetTopRightAngle30(369.2, 271.7);
    QPointF ptTopLeftSetTopRightAngle30(297.5, 104.2);
    QPointF ptTopRightSetTopRightAngle30 = ptTopRightAngle30;
    QPointF ptBottomRightSetTopRightAngle30(440.9, 439.1);
    QPointF ptBottomLeftSetTopRightAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetTopRightAngle30 = QPointF((ptTopRightSetTopRightAngle30.x() + ptTopLeftSetTopRightAngle30.x()) / 2.0,
                                                (ptTopRightSetTopRightAngle30.y() + ptTopLeftSetTopRightAngle30.y()) / 2.0);
    QPointF ptRightCenterSetTopRightAngle30(495.4, 344.5);  // not calculated because of inaccuracy
    QPointF ptBottomCenterSetTopRightAngle30(314.6, 366.2); // not calculated because of inaccuracy
    QPointF ptLeftCenterSetTopRightAngle30(243.0, 198.8);   // not calculated because of inaccuracy
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(" + qPoint2Str(ptTopRightAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptTopRightAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptTopRightAngle30);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightAngle30, 1), 'f', 1) + " mm");
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

    /* setTopRight(100, 400)
    ------------------------
    */
    QPointF ptTopRightBeyondBottomLeftAngle30 = QPointF(100.0, 400.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptTopRightBeyondBottomLeftAngle30.setY(200.0);
    }
    double fWidthSetTopRightBeyondBottomLeftAngle30 = 291.5;
    double fHeightSetTopRightBeyondBottomLeftAngle30 = 218.3;
    QPointF ptCenterSetTopRightBeyondBottomLeftAngle30(369.2, 271.7);
    QPointF ptTopLeftSetTopRightBeyondBottomLeftAngle30(297.5, 104.2);
    QPointF ptTopRightSetTopRightBeyondBottomLeftAngle30 = ptTopRightBeyondBottomLeftAngle30;
    QPointF ptBottomRightSetTopRightBeyondBottomLeftAngle30(440.9, 439.1);
    QPointF ptBottomLeftSetTopRightBeyondBottomLeftAngle30 = ptBottomLeftSetAngle30;
    QPointF ptTopCenterSetTopRightBeyondBottomLeftAngle30 = QPointF((ptTopRightSetTopRightBeyondBottomLeftAngle30.x() + ptTopLeftSetTopRightBeyondBottomLeftAngle30.x()) / 2.0,
                                                (ptTopRightSetTopRightBeyondBottomLeftAngle30.y() + ptTopLeftSetTopRightBeyondBottomLeftAngle30.y()) / 2.0);
    QPointF ptRightCenterSetTopRightBeyondBottomLeftAngle30(495.4, 344.5);  // not calculated because of inaccuracy
    QPointF ptBottomCenterSetTopRightBeyondBottomLeftAngle30(314.6, 366.2); // not calculated because of inaccuracy
    QPointF ptLeftCenterSetTopRightBeyondBottomLeftAngle30(243.0, 198.8);   // not calculated because of inaccuracy
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setTopRight(" + qPoint2Str(ptTopRightBeyondBottomLeftAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setTopRight(" + qPoint2Str(ptTopRightBeyondBottomLeftAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setTopRight", ptTopRightBeyondBottomLeftAngle30);
    pTestStep->setConfigValue("setTopRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondBottomLeftAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondBottomLeftAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondBottomLeftAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetTopRightBeyondBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetTopRightBeyondBottomLeftAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetTopRightBeyondBottomLeftAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetTopRightBeyondBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetTopRightBeyondBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetTopRightBeyondBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetTopRightBeyondBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetTopRightBeyondBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetTopRightBeyondBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetTopRightBeyondBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetTopRightBeyondBottomLeftAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomRight(400, 500)
    ---------------------------
    */
    QPointF ptBottomRightAngle30 = QPointF(400.0, 500.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomRightAngle30.setY(100.0);
    }
    double fWidthSetBottomRightAngle30 = 286.6;
    double fHeightSetBottomRightAngle30 = 173.2;
    QPointF ptCenterSetBottomRightAngle30 (319.2, 353.4);
    QPointF ptTopLeftSetBottomRightAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetBottomRightAngle30(486.6, 350.0);
    QPointF ptBottomRightSetBottomRightAngle30 = ptBottomRightAngle30;
    QPointF ptBottomLeftSetBottomRightAngle30(151.8, 356.7);
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
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(" + qPoint2Str(ptBottomRightAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptBottomRightAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptBottomRightAngle30);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightAngle30, 1), 'f', 1) + " mm");
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

    /* setBottomRight(250, 50)
    ---------------------------
    */
    QPointF ptBottomRightBeyondTopLeftAngle30 = QPointF(250.0, 50.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomRightBeyondTopLeftAngle30.setY(550.0);
    }
    double fWidthSetBottomRightBeyondTopLeftAngle30 = 286.6;
    double fHeightSetBottomRightBeyondTopLeftAngle30 = 173.2;
    QPointF ptCenterSetBottomRightBeyondTopLeftAngle30 (319.2, 353.4);
    QPointF ptTopLeftSetBottomRightBeyondTopLeftAngle30 = ptTopLeftSetAngle30;
    QPointF ptTopRightSetBottomRightBeyondTopLeftAngle30(486.6, 350.0);
    QPointF ptBottomRightSetBottomRightBeyondTopLeftAngle30 = ptBottomRightBeyondTopLeftAngle30;
    QPointF ptBottomLeftSetBottomRightBeyondTopLeftAngle30(151.8, 356.7);
    QPointF ptTopCenterSetBottomRightBeyondTopLeftAngle30 = QPointF((ptTopRightSetBottomRightBeyondTopLeftAngle30.x() + ptTopLeftSetBottomRightBeyondTopLeftAngle30.x()) / 2.0,
                                                (ptTopRightSetBottomRightBeyondTopLeftAngle30.y() + ptTopLeftSetBottomRightBeyondTopLeftAngle30.y()) / 2.0);
    QPointF ptRightCenterSetBottomRightBeyondTopLeftAngle30 = QPointF((ptTopRightSetBottomRightBeyondTopLeftAngle30.x() + ptBottomRightSetBottomRightBeyondTopLeftAngle30.x()) / 2.0,
                                                  (ptBottomRightSetBottomRightBeyondTopLeftAngle30.y() + ptTopRightSetBottomRightBeyondTopLeftAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetBottomRightBeyondTopLeftAngle30 = QPointF((ptBottomRightSetBottomRightBeyondTopLeftAngle30.x() + ptBottomLeftSetBottomRightBeyondTopLeftAngle30.x()) / 2.0,
                                                   (ptBottomRightSetBottomRightBeyondTopLeftAngle30.y() + ptBottomLeftSetBottomRightBeyondTopLeftAngle30.y()) / 2.0);
    QPointF ptLeftCenterSetBottomRightBeyondTopLeftAngle30 = QPointF((ptTopLeftSetBottomRightBeyondTopLeftAngle30.x() + ptBottomLeftSetBottomRightBeyondTopLeftAngle30.x()) / 2.0,
                                                 (ptBottomLeftSetBottomRightBeyondTopLeftAngle30.y() + ptTopLeftSetBottomRightBeyondTopLeftAngle30.y()) / 2.0);
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomRight(" + qPoint2Str(ptBottomRightBeyondTopLeftAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomRight(" + qPoint2Str(ptBottomRightBeyondTopLeftAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomRight", ptBottomRightBeyondTopLeftAngle30);
    pTestStep->setConfigValue("setBottomRight.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondTopLeftAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondTopLeftAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondTopLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondTopLeftAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomRightBeyondTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomRightBeyondTopLeftAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomRightBeyondTopLeftAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomRightBeyondTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomRightBeyondTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomRightBeyondTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomRightBeyondTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomRightBeyondTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomRightBeyondTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomRightBeyondTopLeftAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomRightBeyondTopLeftAngle30) + "} mm");
    }
    pTestStep->setExpectedValues(strlstExpectedValues);

    /* setBottomLeft(100, 350)
    --------------------------
    */
    QPointF ptBottomLeftAngle30 = QPointF(100.0, 350.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomLeftAngle30.setY(250.0);
    }
    double fWidthSetBottomLeftAngle30 = 248.2;
    double fHeightSetBottomLeftAngle30 = 193.3;
    QPointF ptCenterSetBottomLeftAngle30(255.8, 328.4);
    QPointF ptTopLeftSetBottomLeftAngle30(196.7, 182.6);
    QPointF ptTopRightSetBottomLeftAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetBottomLeftAngle30(315.0, 474.1);
    QPointF ptBottomLeftSetBottomLeftAngle30 = ptBottomLeftAngle30;
    QPointF ptTopCenterSetBottomLeftAngle30(304.1, 244.6); // because of calculation inaccurracy
    QPointF ptRightCenterSetBottomLeftAngle30 = QPointF((ptTopRightSetBottomLeftAngle30.x() + ptBottomRightSetBottomLeftAngle30.x()) / 2.0,
                                                  (ptBottomRightSetBottomLeftAngle30.y() + ptTopRightSetBottomLeftAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetBottomLeftAngle30(207.49, 412.1); // because of calculation inaccurracy
    QPointF ptLeftCenterSetBottomLeftAngle30(148.3, 266.3); // because of calculation inaccurracy
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(" + qPoint2Str(ptBottomLeftAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptBottomLeftAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptBottomLeftAngle30);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngle.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftAngle30, 1), 'f', 1) + " mm");
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

    /* setBottomLeft(700, 350)
    --------------------------
    */
    QPointF ptBottomLeftBeyondTopRightAngle30 = QPointF(700.0, 350.0);
    if (i_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
        ptBottomLeftBeyondTopRightAngle30.setY(250.0);
    }
    double fWidthSetBottomLeftBeyondTopRightAngle30 = 248.2;
    double fHeightSetBottomLeftBeyondTopRightAngle30 = 193.3;
    QPointF ptCenterSetBottomLeftBeyondTopRightAngle30(255.8, 328.4);
    QPointF ptTopLeftSetBottomLeftBeyondTopRightAngle30(196.7, 182.6);
    QPointF ptTopRightSetBottomLeftBeyondTopRightAngle30 = ptTopRightSetAngle30;
    QPointF ptBottomRightSetBottomLeftBeyondTopRightAngle30(315.0, 474.1);
    QPointF ptBottomLeftSetBottomLeftBeyondTopRightAngle30 = ptBottomLeftBeyondTopRightAngle30;
    QPointF ptTopCenterSetBottomLeftBeyondTopRightAngle30(304.1, 244.6); // because of calculation inaccurracy
    QPointF ptRightCenterSetBottomLeftBeyondTopRightAngle30 = QPointF((ptTopRightSetBottomLeftBeyondTopRightAngle30.x() + ptBottomRightSetBottomLeftBeyondTopRightAngle30.x()) / 2.0,
                                                  (ptBottomRightSetBottomLeftBeyondTopRightAngle30.y() + ptTopRightSetBottomLeftBeyondTopRightAngle30.y()) / 2.0);
    QPointF ptBottomCenterSetBottomLeftBeyondTopRightAngle30(207.49, 412.1); // because of calculation inaccurracy
    QPointF ptLeftCenterSetBottomLeftBeyondTopRightAngle30(148.3, 266.3); // because of calculation inaccurracy
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " setBottomLeft(" + qPoint2Str(ptBottomLeftBeyondTopRightAngle30) + " " + unit.symbol() + ")",
        /* strOperation    */ "setBottomLeft(" + qPoint2Str(ptBottomLeftBeyondTopRightAngle30) + " " + unit.symbol() + ")",
        /* pGrpParent      */ pGrpTransformRectAngle_30_Degree,
        /* szDoTestStepFct */ SLOT(doTestStepTransformPhysValRect(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("removeAndDeleteAllPhysValShapes", "");
    pTestStep->setConfigValue("create", rectFOrig);
    pTestStep->setConfigValue("create.unit", unit.symbol());
    pTestStep->setConfigValue("setAngle", physValAngle.toString());
    pTestStep->setConfigValue("setBottomLeft", ptBottomLeftBeyondTopRightAngle30);
    pTestStep->setConfigValue("setBottomLeft.unit", unit.symbol());
    pTestStep->setConfigValue("addPhysValShape", "");
    strlstExpectedValues.clear();
    if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondTopRightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondTopRightAngle30, 0), 'f', 0) + " px");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondTopRightAngle30, ", ", 'f', 0) + "} px");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondTopRightAngle30, ", ", 'f', 0) + "} px");
    }
    else {
        strlstExpectedValues.append("Center {" + qPoint2Str(ptCenterSetBottomLeftBeyondTopRightAngle30) + "} mm");
        strlstExpectedValues.append("Width: " + QString::number(Math::round2Nearest(fWidthSetBottomLeftBeyondTopRightAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Height: " + QString::number(Math::round2Nearest(fHeightSetBottomLeftBeyondTopRightAngle30, 1), 'f', 1) + " mm");
        strlstExpectedValues.append("Angle: " + physValAngleCorrected.toString());
        strlstExpectedValues.append("TopLeft {" + qPoint2Str(ptTopLeftSetBottomLeftBeyondTopRightAngle30) + "} mm");
        strlstExpectedValues.append("TopRight {" + qPoint2Str(ptTopRightSetBottomLeftBeyondTopRightAngle30) + "} mm");
        strlstExpectedValues.append("BottomRight {" + qPoint2Str(ptBottomRightSetBottomLeftBeyondTopRightAngle30) + "} mm");
        strlstExpectedValues.append("BottomLeft {" + qPoint2Str(ptBottomLeftSetBottomLeftBeyondTopRightAngle30) + "} mm");
        strlstExpectedValues.append("TopCenter {" + qPoint2Str(ptTopCenterSetBottomLeftBeyondTopRightAngle30) + "} mm");
        strlstExpectedValues.append("RightCenter {" + qPoint2Str(ptRightCenterSetBottomLeftBeyondTopRightAngle30) + "} mm");
        strlstExpectedValues.append("BottomCenter {" + qPoint2Str(ptBottomCenterSetBottomLeftBeyondTopRightAngle30) + "} mm");
        strlstExpectedValues.append("LeftCenter {" + qPoint2Str(ptLeftCenterSetBottomLeftBeyondTopRightAngle30) + "} mm");
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
    CUnit unit = drawingSize.unit();

    QRectF rectF = i_pTestStep->getConfigValue("create").toRectF();
    if (i_pTestStep->hasConfigValue("create.unit")) {
        QString strUnitPoint = i_pTestStep->getConfigValue("create.unit").toString();
        unit = strUnitPoint;
    }

    CPhysValRect physValRectResult(*m_pDrawingScene, rectF, unit);

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
