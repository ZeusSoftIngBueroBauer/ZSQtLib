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

#ifndef ZSApps_Draw_Test_h
#define ZSApps_Draw_Test_h

#include <QtCore/qstring.h>
#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>
#include <QtCore/qsize.h>
#include <QtGui/qpolygon.h>

#include "ZSTest/ZSTest.h"

class QGraphicsItem;
class QPainterPath;

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawingScene;
class CDrawingView;
class CGraphObj;
}

namespace Apps
{
namespace Products
{
namespace Draw
{
class CMainWindow;

//******************************************************************************
class CTest : public ZS::Test::CTest
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Products::Draw"; }
public: // ctors and dtor
    CTest();
    ~CTest();
public: // instance methods
    void setMainWindow( CMainWindow* i_pMainWindow );
protected: // instance methods
    void createTestGroupDrawStandardShapes(int& io_idxGroup);
    void createTestGroupDrawStandardShapesLines(ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup);
protected: // instance methods
    //void createTestGroupMouseEventsStandardShapes( ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup );
protected: // instance methods
    //void createTestGroupMouseEventsStandardShapesPoints( ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup );
protected slots:
    void doTestStepMainWindowSetGeometry( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepClearDrawingScene( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSetDrawingSize( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSetGridSettings( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepLoadFile( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSaveFile( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSetCurrentDrawingTool( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepUnsetCurrentDrawingTool( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepMousePressEvent( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepMouseMoveEvent( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepMouseReleaseEvent( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepMouseEventsStandardShapesPointsClearDrawingScene( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepMouseEventsStandardShapesPointsActivateDrawingTool( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepMouseEventsStandardShapesPointsActivateEditSelect( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepMouseEventsStandardShapesPointsTriggerEditGroup( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepMouseEventsStandardShapesPointsTriggerEditUngroup( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepMouseEventsStandardShapesPointsCheckResult( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepMouseEventsStandardShapesPoints( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    //void doTestStepStandardShapesLines( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    //void doTestStepStandardShapesRectangles( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    //void doTestStepStandardShapesEllipses( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    //void doTestStepStandardShapesPolylines( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    //void doTestStepStandardShapesPolygons( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    //void doTestStepStandardShapesTexts( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    //void doTestStepStandardShapesImages( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    //void doTestStepStandardShapesConnections( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    //void doTestStepStandardShapesGroups( ZS::Test::CTestStep* i_pTestStep );
protected: // instance methods
    //void createTestGroupElectricityResistors();
protected slots:
    //void doTestStepElectricityResistors( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepElectricityCapacitors( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepElectricityInductors( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepElectricityTransistors( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepElectricitySwitches( ZS::Test::CTestStep* i_pTestStep );
public: // instance members
    CMainWindow* m_pMainWindow;
    ZS::Draw::CDrawingView*  m_pDrawingView;
    ZS::Draw::CDrawingScene* m_pDrawingScene;
    //QPoint                   m_ptDrawingSceneMousePos;
    //QPoint                   m_ptDrawingViewMousePos;
    //ZS::Draw::CGraphObj*     m_pGraphObjCreating;
    //ZS::Draw::CGraphObj*     m_pGraphObjSelected;
    //int                      m_iTestStepLev1;   // Standard, QtWidgets, Electricity
    //int                      m_iTestStepLev2;   // Standard/Lines, ...
    //int                      m_iTestStepLev3;   // Standard/Lines/Create, ...
    //int                      m_iTestStepLev4;   // Standard/Lines/Create/Step0, ...
    //double                   m_fAngle_degree;
    //double                   m_fxOffsHor;
    //double                   m_fyOffsHor;
    //int                      m_iPointsCount;
    //int                      m_iLinesCount;
    //int                      m_iRectanglesCount;
    //int                      m_iEllipsesCount;
    //int                      m_iPolylinesCount;
    //int                      m_iPolygonsCount;
    //int                      m_iTextsCount;
    //int                      m_iImagesCount;
    //int                      m_iConnectionPointsCount;
    //int                      m_iConnectionLinesCount;
    //int                      m_iGroupsCount;
    //int                      m_iElectricityResistorsCount;
    //int                      m_iElectricityCapacitorsCount;
    //int                      m_iElectricityInductorsCount;
    //int                      m_iElectricityTransistorsCount;
    //int                      m_iElectricitySwitchesCount;
    //int                      m_iWdgtCheckBoxesCount;
    //int                      m_iWdgtComboBoxesCount;
    //int                      m_iWdgtGroupBoxesCount;
    //int                      m_iWdgtLabelsCount;
    //int                      m_iWdgtLineEditsCount;
    //int                      m_iWdgtPushButtonsCount;

}; // class CTest

} // namespace Draw

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_Draw_Test_h
