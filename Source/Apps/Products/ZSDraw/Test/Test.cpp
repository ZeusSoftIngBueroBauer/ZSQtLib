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

    for (const QString& strKey : s_hshGraphObjsInstCounts.keys()) {
        s_hshGraphObjsInstCounts[strKey] = 0;
    }

    CDrawingSize drawingSize("Test-PixelsDrawing");
    drawingSize.setDimensionUnit(EScaleDimensionUnit::Pixels);
    drawingSize.setScreenResolutionInPxPerMM(1.0);
    drawingSize.setImageSize(CPhysVal(800, Units.Length.px), CPhysVal(600, Units.Length.px));
    createTestGroupPrepareScene(pGrpPixelsDrawing, io_idxGroup, drawingSize);
    createTestGroupAddStandardShapes(pGrpPixelsDrawing, io_idxGroup);
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

    for (const QString& strKey : s_hshGraphObjsInstCounts.keys()) {
        s_hshGraphObjsInstCounts[strKey] = 0;
    }

    CDrawingSize drawingSize("Test-MetricsDrawing");
    drawingSize.setDimensionUnit(EScaleDimensionUnit::Metric);
    drawingSize.setScreenResolutionInPxPerMM(1.0);
    drawingSize.setImageSize(CPhysVal(800, Units.Length.mm), CPhysVal(600, Units.Length.mm));
    createTestGroupPrepareScene(pGrpMetricsDrawing, io_idxGroup, drawingSize);
    createTestGroupAddStandardShapes(pGrpMetricsDrawing, io_idxGroup);
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
ZS::Test::CTestStepGroup* CTest::createTestGroupAddStandardShapes(
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
        /* strMethod    */ "createTestGroupAddStandardShapes",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpAddStandardShapes = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Add " + CObjFactory::c_strGroupNameStandardShapes,
        /* pTSGrpParent */ i_pTestStepGroupParent );

    createTestGroupAddStandardShapesLines(pGrpAddStandardShapes, io_idxGroup);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpAddStandardShapes->path());
    }
    return pGrpAddStandardShapes;

} // createTestGroupAddStandardShapes

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
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Draw " + CObjFactory::c_strGroupNameStandardShapes,
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
ZS::Test::CTestStepGroup* CTest::createTestGroupAddStandardShapesLines(
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
        /* strMethod    */ "createTestGroupAddStandardShapesLines",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpAddLines = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Lines",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    QString strFactoryGroupName = CObjFactory::c_strGroupNameStandardShapes;
    QString strGraphObjType = graphObjType2Str(EGraphObjTypeLine);
    QString strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;
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

    // Group
    //------

    ZS::Test::CTestStepGroup* pGrpDrawGroups = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Groups",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);
    strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;
    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
    s_hshGraphObjsInstCounts[strInstCountKey] = s_hshGraphObjsInstCounts[strInstCountKey] + 1;

    ZS::Test::CTestStepGroup* pGrpDrawGroup0 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Create, Resize, Move " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawGroups );

    ZS::Test::CTestStepGroup* pGrpCreateGroup0 = createTestGroupDrawMouseEventsCreateGroup(
        pGrpDrawGroup0, io_idxGroup,
        strGraphObjName,
        QPoint(240, 240), QPoint(360, 360), 10, QPoint(370, 370));

    ZS::Test::CTestStepGroup* pGrpDrawResizeGroup0 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Resize " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawGroup0 );

    idxStep = 0;

    addTestStepDrawMouseEventsMove(
        pGrpDrawResizeGroup0, idxStep, QPoint(360, 360), QPoint(350, 350));
    addTestStepsDrawMouseEventsMovePressMoveRelease(
        pGrpDrawResizeGroup0, idxStep, QPoint(350, 350), QPoint(400, 400));
    addTestStepDrawMouseEventsMove(
        pGrpDrawResizeGroup0, idxStep, QPoint(450, 450), QPoint(460, 460));

    ZS::Test::CTestStepGroup* pGrpDrawMoveGroup0 = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Move " + strGraphObjName,
        /* pTSGrpParent */ pGrpDrawGroup0 );

    idxStep = 0;

    addTestStepDrawMouseEventsMove(
        pGrpDrawMoveGroup0, idxStep, QPoint(360, 360), QPoint(350, 350));
    addTestStepsDrawMouseEventsMovePressMoveRelease(
        pGrpDrawMoveGroup0, idxStep, QPoint(350, 350), QPoint(325, 325));
    addTestStepDrawMouseEventsMove(
        pGrpDrawMoveGroup0, idxStep, QPoint(325, 325), QPoint(310, 325));

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(pGrpAddLines->path());
    }
    return pGrpAddLines;

} // createTestGroupAddStandardShapesLines

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

    // Groups
    //-------

    ZS::Test::CTestStepGroup* pGrpDrawGroups = new ZS::Test::CTestStepGroup(
        /* pTest        */ this,
        /* strName      */ "Group " + QString::number(++io_idxGroup) + " Groups",
        /* pTSGrpParent */ i_pTestStepGroupParent );

    // Group0
    //-------

    strGraphObjType = graphObjType2Str(EGraphObjTypeGroup);
    strInstCountKey = strFactoryGroupName + "::" + strGraphObjType;
    strGraphObjName = strGraphObjType + QString::number(s_hshGraphObjsInstCounts[strInstCountKey]);
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
        mthTracer.setMethodReturn(pGrpDrawLines->path());
    }
    return pGrpDrawLines;

} // createTestGroupDrawStandardShapesLines

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
    EGraphObjType graphObjType = str2GraphObjType(strGraphObjType);
    CPhysValPoint physValPoint1(*m_pDrawingScene);
    physValPoint1 = i_pTestStep->getConfigValue("Point1").toString();

    i_pTestStep->setExpectedValue("");

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);

    if (pObjFactory != nullptr) {
        CDrawSettings drawSettings(graphObjType);
        CGraphObj* pGraphObj = pObjFactory->createGraphObj(m_pDrawingScene, physValPoint1, drawSettings);
        if (graphObjType == EGraphObjTypeLine) {
            CGraphObjLine* pGraphObjLine = dynamic_cast<CGraphObjLine*>(pGraphObj);
            if (pGraphObjLine != nullptr) {
                CPhysValPoint physValPoint2(*m_pDrawingScene);
                physValPoint2 = i_pTestStep->getConfigValue("Point2").toString();
                pGraphObjLine->setP2(physValPoint2);
            }
        }
        m_pDrawingScene->addGraphObj(pGraphObj);
    }

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
