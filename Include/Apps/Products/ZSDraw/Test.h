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
namespace Trace
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
public: // type definitions and constants
    //enum ETestStepLev1
    //{
    //    ETestStepLev1StandardShapes = 0,
    //    ETestStepLev1Electricity    = 1,
    //    ETestStepLev1Count
    //};
    //enum ETestStepLev2StandardShapes
    //{
    //    ETestStepLev2StandardShapesPoints      = 0,
    //    ETestStepLev2StandardShapesLines       = 1,
    //    ETestStepLev2StandardShapesRectangles  = 2,
    //    ETestStepLev2StandardShapesEllipses    = 3,
    //    ETestStepLev2StandardShapesPolylines   = 4,
    //    ETestStepLev2StandardShapesPolygons    = 5,
    //    ETestStepLev2StandardShapesTexts       = 6,
    //    ETestStepLev2StandardShapesImages      = 7,
    //    ETestStepLev2StandardShapesConnections = 8,
    //    ETestStepLev2StandardShapesGroups      = 9,
    //    ETestStepLev2StandardShapesCount
    //};
    //enum ETestStepLev2Electricity
    //{
    //    ETestStepLev2ElectricityResistors   = 0,
    //    ETestStepLev2ElectricityCapacitors  = 1,
    //    ETestStepLev2ElectricityInductors   = 2,
    //    ETestStepLev2ElectricityTransistors = 3,
    //    ETestStepLev2ElectricitySwitches    = 4,
    //    ETestStepLev2ElectricityCount
    //};
public: // ctors and dtor
    CTest( const QString& i_strTestStepsFileName );
    ~CTest();
public: // instance methods
    void setMainWindow( CMainWindow* i_pMainWindow );
public: // overridables of base class Test::CTest
    //virtual void start();
protected slots: // overridables of base class Test::CTest
    //virtual void doTestStep();
protected: // instance methods
    void createTestGroupAndroidWallpaperTemplate( int& io_idxGroup );
protected slots: // test step methods
    void doTestStepAndroidWallpaperTemplatePageSetup( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepAndroidWallpaperTemplateDrawGridLines( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepAndroidWallpaperTemplateSaveAsWallpaperPng( ZS::Test::CTestStep* i_pTestStep );
protected: // instance methods
    //virtual void doTestStepStandardShapes();
    //virtual void doTestStepStandardShapesPoints( ZS::Test::CTestStepGroup* i_pTestStep );
    //virtual void doTestStepStandardShapesLines( ZS::Test::CTestStepGroup* i_pTestStep );
    //virtual void doTestStepStandardShapesRectangles( ZS::Test::CTestStepGroup* i_pTestStep );
    //virtual void doTestStepStandardShapesEllipses( ZS::Test::CTestStepGroup* i_pTestStep );
    //virtual void doTestStepStandardShapesPolylines( ZS::Test::CTestStepGroup* i_pTestStep );
    //virtual void doTestStepStandardShapesPolygons( ZS::Test::CTestStepGroup* i_pTestStep );
    //virtual void doTestStepStandardShapesTexts( ZS::Test::CTestStepGroup* i_pTestStep );
    //virtual void doTestStepStandardShapesImages( ZS::Test::CTestStepGroup* i_pTestStep );
    //virtual void doTestStepStandardShapesConnections( ZS::Test::CTestStepGroup* i_pTestStep );
    //virtual void doTestStepStandardShapesGroups( ZS::Test::CTestStepGroup* i_pTestStep );
protected: // instance methods
    //virtual void doTestStepElectricity();
    //virtual void doTestStepElectricityResistors( ZS::Test::CTestStepGroup* i_pTestStep );
    //virtual void doTestStepElectricityCapacitors( ZS::Test::CTestStepGroup* i_pTestStep );
    //virtual void doTestStepElectricityInductors( ZS::Test::CTestStepGroup* i_pTestStep );
    //virtual void doTestStepElectricityTransistors( ZS::Test::CTestStepGroup* i_pTestStep );
    //virtual void doTestStepElectricitySwitches( ZS::Test::CTestStepGroup* i_pTestStep );
public: // instance members
    CMainWindow*             m_pMainWindow;
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
    ZS::Trace::CTrcAdminObj*   m_pTrcAdminObj;

}; // class CTest

} // namespace Draw

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_Draw_Test_h
