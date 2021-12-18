/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qfileinfo.h>
#include <QtCore/qtimer.h>

#include <QtGui/qevent.h>

#if QT_VERSION < 0x050000
#include <QtGui/qaction.h>
#else
#include <QtWidgets/qaction.h>
#endif

#include "Test.h"
#include "MainWindow.h"
#include "ZSDrawGraphObjWdgtCheckBox.h"
#include "ZSDrawGraphObjWdgtComboBox.h"
#include "ZSDrawGraphObjWdgtGroupBox.h"
#include "ZSDrawGraphObjWdgtLabel.h"
#include "ZSDrawGraphObjWdgtLineEdit.h"
#include "ZSDrawGraphObjWdgtPushButton.h"
#include "ZSDrawGraphObjElectricityResistor.h"
#include "ZSDrawGraphObjElectricityCapacitor.h"
#include "ZSDrawGraphObjElectricityInductor.h"
#include "ZSDrawGraphObjElectricitySwitch.h"
#include "ZSDrawGraphObjElectricityTransistor.h"

#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSDraw/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/ZSDrawGraphObjEllipse.h"
#include "ZSDraw/ZSDrawGraphObjGroup.h"
#include "ZSDraw/ZSDrawGraphObjImage.h"
#include "ZSDraw/ZSDrawGraphObjLine.h"
#include "ZSDraw/ZSDrawGraphObjPoint.h"
#include "ZSDraw/ZSDrawGraphObjPolygon.h"
#include "ZSDraw/ZSDrawGraphObjPolyline.h"
#include "ZSDraw/ZSDrawGraphObjRect.h"
#include "ZSDraw/ZSDrawGraphObjText.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Draw::Electricity;
using namespace ZS::Draw::QtWidgets;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Products::Draw;
using namespace ZS::Trace;


/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest( const QString& i_strTestStepsFileName ) :
//------------------------------------------------------------------------------
    ZS::Test::CTest(
        /* strName              */ "Draw",
        /* strTestStepsFileName */ i_strTestStepsFileName,
        /* iTestStepInterval_ms */ 100 ),
    m_pMainWindow(nullptr),
    m_pDrawingView(nullptr),
    m_pDrawingScene(nullptr),
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
    m_fInterferenceChecksDrawingFreqMin_Hz(899.5e6),
    m_fInterferenceChecksDrawingFreqMax_Hz(900.5e6),
    m_fInterferenceChecksDrawingLevelMin_dBm(0.0),
    m_fInterferenceChecksDrawingLevelMax_dBm(50.0)
{
    // Get trace admin object
    //-----------------------

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::Draw", "CTest", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    int idxSubGroup;
    int idxStep;

    // Sub-Group: Server
    //==================

    idxSubGroup = 0;

    ZS::Test::CTestStepGroup* pGrpInterferenceChecks = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Interference Checks",
        /* pTSGrpParent    */ nullptr );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": [TE1] LOCK? BOX1, 100, OFF, 899900000.000000, 210.0e3, 33.0, 24.028359, 1",
        /* strOperation    */ "Rect {899.9e6, 0.21e6, 33.0, 24.028359}",
        /* pTSGrpParent    */ pGrpInterferenceChecks,
        /* szDoTestStepFct */ SLOT(doTestStepInterferenceChecksI1L1(ZS::Test::CTestStep*)) );

    // Recall test step settings
    //--------------------------

    QFileInfo fileInfo(i_strTestStepsFileName);

    if( fileInfo.exists() )
    {
        SErrResultInfo errResultInfo = recall(i_strTestStepsFileName);

        if(errResultInfo.isErrorResult())
        {
            if(CErrLog::GetInstance() != nullptr)
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
    }

} // ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    SErrResultInfo errResultInfo = save();

    if(errResultInfo.isErrorResult())
    {
        if(CErrLog::GetInstance() != nullptr)
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

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
    m_fInterferenceChecksDrawingFreqMin_Hz = 0.0;
    m_fInterferenceChecksDrawingFreqMax_Hz = 0.0;
    m_fInterferenceChecksDrawingLevelMin_dBm = 0.0;
    m_fInterferenceChecksDrawingLevelMax_dBm = 0.0;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class Test::CTest
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::setMainWindow( CMainWindow* i_pMainWindow )
//------------------------------------------------------------------------------
{
    m_pMainWindow = i_pMainWindow;
    m_pDrawingView = m_pMainWindow->getDrawingView();
    m_pDrawingScene = m_pMainWindow->getDrawingScene();
}

/*==============================================================================
public: // overridables of base class Test::CTest
==============================================================================*/

////------------------------------------------------------------------------------
//void CTest::start()
////------------------------------------------------------------------------------
//{
//    ZS::Test::CTestStepGroup* pTSGrpDraw = getTestStepGroup("Draw",nullptr);
//
//    if( pTSGrpDraw->isEnabled() )
//    {
//        //m_pMainWindow->setGeometry(50,50,1400,800);
//
//        m_pDrawingScene->clear();
//        m_pDrawingScene->setMode( EMode::Edit, EEditToolIgnore, EEditModeIgnore, EEditResizeModeIgnore, false );
//
//        CGraphObjPoint::ResetCtorsDtorsCounters();            m_iPointsCount = 0;
//        CGraphObjLine::ResetCtorsDtorsCounters();             m_iLinesCount = 0;
//        CGraphObjRect::ResetCtorsDtorsCounters();             m_iRectanglesCount = 0;
//        CGraphObjEllipse::ResetCtorsDtorsCounters();          m_iEllipsesCount = 0;
//        CGraphObjPolyline::ResetCtorsDtorsCounters();         m_iPolylinesCount = 0;
//        CGraphObjPolygon::ResetCtorsDtorsCounters();          m_iPolygonsCount = 0;
//        CGraphObjText::ResetCtorsDtorsCounters();             m_iTextsCount = 0;
//        CGraphObjImage::ResetCtorsDtorsCounters();            m_iImagesCount = 0;
//        CGraphObjConnectionPoint::ResetCtorsDtorsCounters();  m_iConnectionPointsCount = 0;
//        CGraphObjConnectionLine::ResetCtorsDtorsCounters();   m_iConnectionLinesCount = 0;
//        CGraphObjGroup::ResetCtorsDtorsCounters();            m_iGroupsCount = 0;
//
//        CGraphObjResistor::ResetCtorsDtorsCounters();         m_iElectricityResistorsCount = 0;
//        CGraphObjCapacitor::ResetCtorsDtorsCounters();        m_iElectricityCapacitorsCount = 0;
//        CGraphObjInductor::ResetCtorsDtorsCounters();         m_iElectricityInductorsCount = 0;
//        CGraphObjTransistor::ResetCtorsDtorsCounters();       m_iElectricityTransistorsCount = 0;
//        CGraphObjSwitch::ResetCtorsDtorsCounters();           m_iElectricitySwitchesCount = 0;
//
//        CGraphObjWdgtCheckBox::ResetCtorsDtorsCounters();     m_iWdgtCheckBoxesCount = 0;
//        CGraphObjWdgtComboBox::ResetCtorsDtorsCounters();     m_iWdgtComboBoxesCount = 0;
//        CGraphObjWdgtGroupBox::ResetCtorsDtorsCounters();     m_iWdgtGroupBoxesCount = 0;
//        CGraphObjWdgtLabel::ResetCtorsDtorsCounters();        m_iWdgtLabelsCount = 0;
//        CGraphObjWdgtLineEdit::ResetCtorsDtorsCounters();     m_iWdgtLineEditsCount = 0;
//        CGraphObjWdgtPushButton::ResetCtorsDtorsCounters();   m_iWdgtPushButtonsCount = 0;
//
//        m_iTestStepLev1 = ETestStepLev1StandardShapes;
//        m_iTestStepLev2 = 0;
//        m_iTestStepLev3 = 0;
//        m_iTestStepLev4 = 0;
//
//        ZS::Test::CTest::start();
//    }
//
//} // start

/*==============================================================================
protected slots: // overridables of base class Test::CTest
==============================================================================*/

////------------------------------------------------------------------------------
//void CTest::doTestStep()
////------------------------------------------------------------------------------
//{
//    m_bDoTestStepPending = false;
//
//    if( m_iTestStepLev1 == ETestStepLev1StandardShapes )
//    {
//        doTestStepStandardShapes();
//    }
//    else if( m_iTestStepLev1 == ETestStepLev1Electricity )
//    {
//        doTestStepElectricity();
//    }
//    else
//    {
//        if( m_state == EStateRunning ) // not Paused or already Stopped
//        {
//            stop();
//        }
//    }
//
//} // doTestStep
