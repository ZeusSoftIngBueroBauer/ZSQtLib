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


/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest() :
//------------------------------------------------------------------------------
    ZS::Test::CTest("ZSDraw"),
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
    m_pTrcAdminObj(nullptr)
{
    // Get trace admin object
    //-----------------------

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::Draw", "CTest", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    int idxGroup = 0;

    // Grid Lines
    //-----------

    //createTestGroupAndroidWallpaperTemplate(idxGroup);

    // Mouse Events
    //-------------

    //createTestGroupMouseEvents(idxGroup);

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

    if(errResultInfo.isErrorResult())
    {
        if(CErrLog::GetInstance() != nullptr)
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    mthTracer.onAdminObjAboutToBeReleased();

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
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
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
protected: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CTest::createTestGroupMouseEvents( int& io_idxGroup )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    QString strMthOutArgs;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
//    {
//        strMthInArgs = "IdxGroup:" + QString::number(io_idxGroup);
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "createTestGroupMouseEvents",
//        /* strAddInfo   */ strMthInArgs );
//
//    ZS::Test::CTestStepGroup* pGrp = new ZS::Test::CTestStepGroup(
//        /* pTest           */ this,
//        /* strName         */ "Group " + QString::number(++io_idxGroup) + " Mouse Events",
//        /* pTSGrpParent    */ nullptr );
//
//    createTestGroupMouseEventsStandardShapes(pGrp, io_idxGroup);
//
//    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
//    {
//        strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
//        mthTracer.setMethodOutArgs(strMthOutArgs);
//    }
//
//} // createTestGroupMouseEvents

////------------------------------------------------------------------------------
//void CTest::createTestGroupMouseEventsStandardShapes( ZS::Test::CTestStepGroup* i_pTestGroupParent, int& io_idxGroup )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    QString strMthOutArgs;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
//    {
//        strMthInArgs = "Parent: " + QString(i_pTestGroupParent == nullptr ? "nullptr" : i_pTestGroupParent->path());
//        strMthInArgs += ", IdxGroup:" + QString::number(io_idxGroup);
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "createTestGroupMouseEventsStandardShapes",
//        /* strAddInfo   */ strMthInArgs );
//
//    ZS::Test::CTestStepGroup* pGrp = new ZS::Test::CTestStepGroup(
//        /* pTest           */ this,
//        /* strName         */ "Group " + QString::number(++io_idxGroup) + " Standard Shapes",
//        /* pTSGrpParent    */ i_pTestGroupParent );
//
//    createTestGroupMouseEventsStandardShapesPoints(pGrp, io_idxGroup);
//
//    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
//    {
//        strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
//        mthTracer.setMethodOutArgs(strMthOutArgs);
//    }
//
//} // createTestGroupMouseEventsStandardShapes
