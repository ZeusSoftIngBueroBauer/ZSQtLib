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

#include <QtCore/qtimer.h>
#include <QtGui/qevent.h>

#if QT_VERSION < 0x050000
#include <QtGui/qaction.h>
#else
#include <QtWidgets/qaction.h>
#endif

#include "Test.h"
#include "MainWindow.h"

#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawingView.h"
#include "ZSDraw/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Products::Draw;


/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

#if 0

static QRectF    s_rctCnctPt1;
static QRectF    s_rctCnctPt2;
static QRectF    s_rctCnctPt3;
static QPolygonF s_plgCnctLine1Prev;
static QPolygonF s_plgCnctLine1Curr;
static QPolygonF s_plgCnctLine2Prev;
static QPolygonF s_plgCnctLine2Curr;

// Create ConnectionPoint1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosCreateCnct1CnctPt1Start(150,250);

// Create ConnectionPoint2
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosCreateCnct1CnctPt2Start(150,350);

// Create ConnectionPoint3
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosCreateCnct1CnctPt3Start(150,380);

// Create Connection1 ConnectionLine1 CreateFirstLineSegment (at ConnectionPoint1)
//------------------------------------------------------------------------------

static const QSize s_arsizeConnectionsCreateCnct1CnctLine1CreateFirstLineSegmentTestSteps[] = {
                               /*   x,   y */
    /*  0 */ QSize( -10, 10 ), /* 140, 260 */
    /*  1 */ QSize( -10, 10 ), /* 130, 270 */
    /*  2 */ QSize( -10, 10 ), /* 120, 280 */
    /*  3 */ QSize( -10, 10 ), /* 110, 290 */
    /*  4 */ QSize( -10, 10 )  /* 100, 300 */
};

static const int ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateFirstLineSegment_TestStepCount = sizeof(s_arsizeConnectionsCreateCnct1CnctLine1CreateFirstLineSegmentTestSteps)/sizeof(s_arsizeConnectionsCreateCnct1CnctLine1CreateFirstLineSegmentTestSteps[0]);

// Create Connection1 ConnectionLine1 CreateSecondLineSegment
//------------------------------------------------------------------------------

static const QSize s_arsizeConnectionsCreateCnct1CnctLine1CreateSecondLineSegmentTestSteps[] = {
                              /*   x,   y */
    /*  0 */ QSize( 10, 10 ), /* 110, 310 */
    /*  1 */ QSize( 10, 10 ), /* 120, 320 */
    /*  2 */ QSize( 10, 10 ), /* 130, 330 */
    /*  3 */ QSize( 10, 10 ), /* 140, 340 */
    /*  4 */ QSize( 10, 10 )  /* 150, 350 */
};

static const int ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateSecondLineSegment_TestStepCount = sizeof(s_arsizeConnectionsCreateCnct1CnctLine1CreateSecondLineSegmentTestSteps)/sizeof(s_arsizeConnectionsCreateCnct1CnctLine1CreateSecondLineSegmentTestSteps[0]);

// Create Connection1 ConnectionLine2 CreateFirstLineSegment (at ConnectionPoint2)
//------------------------------------------------------------------------------

static const QSize s_arsizeConnectionsCreateCnct1CnctLine2CreateFirstLineSegmentTestSteps[] = {
                               /*   x,   y */
    /*  0 */ QSize( 10, -10 ), /* 160, 340 */
    /*  1 */ QSize( 10, -10 ), /* 170, 330 */
    /*  2 */ QSize( 10, -10 ), /* 180, 320 */
    /*  3 */ QSize( 10, -10 ), /* 190, 310 */
    /*  4 */ QSize( 10, -10 )  /* 200, 300 */
};

static const int ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFirstLineSegment_TestStepCount = sizeof(s_arsizeConnectionsCreateCnct1CnctLine2CreateFirstLineSegmentTestSteps)/sizeof(s_arsizeConnectionsCreateCnct1CnctLine2CreateFirstLineSegmentTestSteps[0]);

// Create Connection1 ConnectionLine2 CreateSecondLineSegment (connect at ConnectionPoint1)
//------------------------------------------------------------------------------

static const QSize s_arsizeConnectionsCreateCnct1CnctLine2CreateSecondLineSegmentTestSteps[] = {
                                /*   x,   y */
    /*  0 */ QSize( -10, -10 ), /* 190, 290 */
    /*  1 */ QSize( -10, -10 ), /* 180, 280 */
    /*  2 */ QSize( -10, -10 ), /* 170, 270 */
    /*  3 */ QSize( -10, -10 ), /* 160, 260 */
    /*  4 */ QSize( -10, -10 )  /* 150, 250 */
};

static const int ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateSecondLineSegment_TestStepCount = sizeof(s_arsizeConnectionsCreateCnct1CnctLine2CreateSecondLineSegmentTestSteps)/sizeof(s_arsizeConnectionsCreateCnct1CnctLine2CreateSecondLineSegmentTestSteps[0]);

// Create Connection1 ConnectionLine2 CreateThirdLineSegment (insert shape point)
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosCreateCnct1CnctLine2CreateThirdLineSegmentStart(175,275);

static const QSize s_arsizeConnectionsCreateCnct1CnctLine2CreateThirdLineSegmentTestSteps[] = {
                         /* CnctLine2: [0],       [1],       [2],       [3] */
    /*  0 */ QSize(  5, -5 ), /* (150/350), (200/300), (180/270), (150/250) */
    /*  1 */ QSize( 10, -5 ), /* (150/350), (200/300), (190/265), (150/250) */
    /*  2 */ QSize( 10, -5 )  /* (150/350), (200/300), (200/260), (150/250) */
};

static const int ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_TestStepCount = sizeof(s_arsizeConnectionsCreateCnct1CnctLine2CreateThirdLineSegmentTestSteps)/sizeof(s_arsizeConnectionsCreateCnct1CnctLine2CreateThirdLineSegmentTestSteps[0]);

// Resize Connection1 ConnectionLine2 CreateFourthLineSegment (insert shape point, normalize removing shape point 1)
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosCreateCnct1CnctLine2CreateFourthLineSegmentStart(175,325);

static const QSize s_arsizeConnectionsCreateCnct1CnctLine2CreateFourthLineSegmentTestSteps[] = {
                        /* CnctLine2: [0],       [1],       [2],       [3],       [4] */
    /*  0 */ QSize(  5, 5 ), /* (150/350), (180/330), (200/300), (180/270), (150/250) */
    /*  1 */ QSize( 10, 5 ), /* (150/350), (190/335), (200/300), (190/265), (150/250) */
    /*  2 */ QSize( 10, 5 )  /* (150/350), (200/340), (200/260), (150/250)            */
};

static const int ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_TestStepCount = sizeof(s_arsizeConnectionsCreateCnct1CnctLine2CreateFourthLineSegmentTestSteps)/sizeof(s_arsizeConnectionsCreateCnct1CnctLine2CreateFourthLineSegmentTestSteps[0]);

// Move Connection1 at CnctPt1
//------------------------------------------------------------------------------

static const QSize s_arsizeConnectionsMoveCnct1CnctPt1TestSteps[] = {
                                /* xLT, yLT */
    /*  0 */ QSize( -10, -10 ), /* 140, 260 */
    /*  1 */ QSize( -10, -10 ), /* 130, 270 */
    /*  2 */ QSize( -10, -10 ), /* 120, 280 */
    /*  3 */ QSize( -10, -10 ), /* 110, 290 */
    /*  4 */ QSize( -10, -10 ), /* 100, 300 */
    /*  5 */ QSize(  10,  10 ), /* 110, 290 */
    /*  6 */ QSize(  10,  10 ), /* 120, 280 */
    /*  7 */ QSize(  10,  10 ), /* 130, 270 */
    /*  8 */ QSize(  10,  10 ), /* 140, 260 */
    /*  9 */ QSize(  10,  10 )  /* 150, 350 */
};

static const int ETestStepLev3_Connections_Move_Cnct1_CnctPt1_TestStepCount = sizeof(s_arsizeConnectionsMoveCnct1CnctPt1TestSteps)/sizeof(s_arsizeConnectionsMoveCnct1CnctPt1TestSteps[0]);

// Move Connection1 at CnctPt2
//------------------------------------------------------------------------------

static const QSize s_arsizeConnectionsMoveCnct1CnctPt2TestSteps[] = {
                                /* xLT, yLT */
    /*  0 */ QSize( -10,  10 ), /* 140, 360 */
    /*  1 */ QSize( -10,  10 ), /* 130, 370 */
    /*  2 */ QSize( -10,  10 ), /* 120, 380 */
    /*  3 */ QSize( -10,  10 ), /* 110, 390 */
    /*  4 */ QSize( -10,  10 ), /* 100, 400 */
    /*  5 */ QSize(  10, -10 ), /* 110, 390 */
    /*  6 */ QSize(  10, -10 ), /* 120, 380 */
    /*  7 */ QSize(  10, -10 ), /* 130, 370 */
    /*  8 */ QSize(  10, -10 ), /* 140, 360 */
    /*  9 */ QSize(  10, -10 )  /* 150, 350 */
};

static const int ETestStepLev3_Connections_Move_Cnct1_CnctPt2_TestStepCount = sizeof(s_arsizeConnectionsMoveCnct1CnctPt2TestSteps)/sizeof(s_arsizeConnectionsMoveCnct1CnctPt2TestSteps[0]);

// Resize Connection1 ConnectionLine2 MoveFirstShapePointFromCnctPt2
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt2SelectLine(200,300);

static const QPoint s_ptDrawingSceneMousePosResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt2SelectShapePoint(150,350);

// Resize Connection1 ConnectionLine2 MoveFirstShapePointFromCnctPt2
//------------------------------------------------------------------------------

static const QSize s_arsizeConnectionsResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt2TestSteps[] = {
                        /* CnctLine2: [0],       [1],       [2],       [3] */
    /*  0 */ QSize( 10, 0 ), /* (160/350), (200/340), (200/260), (150/250) */
    /*  1 */ QSize( 10, 0 ), /* (170/350), (200/340), (200/260), (150/250) */
    /*  2 */ QSize( 10, 0 ), /* (180/350), (200/340), (200/260), (150/250) */
    /*  3 */ QSize( 10, 0 ), /* (190/350), (200/340), (200/260), (150/250) */
    /*  4 */ QSize( 10, 0 )  /* (200/350), (200/340), (200/260), (150/250) */
};

static const int ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_TestStepCount = sizeof(s_arsizeConnectionsResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt2TestSteps)/sizeof(s_arsizeConnectionsResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt2TestSteps[0]);

// Resize Connection1 ConnectionLine2 MoveFirstShapePointToCnctPt3
//------------------------------------------------------------------------------

static const QSize s_arsizeConnectionsResizeCnct1CnctLine2MoveFirstShapePointToCnctPt3TestSteps[] = {
                         /* CnctLine2: [0],       [1],       [2],       [3] */
    /*  0 */ QSize( -10, 6 ), /* (190/356), (200/340), (200/260), (150/250) */
    /*  1 */ QSize( -10, 6 ), /* (180/362), (200/340), (200/260), (150/250) */
    /*  2 */ QSize( -10, 6 ), /* (190/368), (200/340), (200/260), (150/250) */
    /*  3 */ QSize( -10, 6 ), /* (100/374), (200/340), (200/260), (150/250) */
    /*  4 */ QSize( -10, 6 )  /* (150/380), (200/340), (200/260), (150/250) */
};

static const int ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointToCnctPt3_TestStepCount = sizeof(s_arsizeConnectionsResizeCnct1CnctLine2MoveFirstShapePointToCnctPt3TestSteps)/sizeof(s_arsizeConnectionsResizeCnct1CnctLine2MoveFirstShapePointToCnctPt3TestSteps[0]);

// Resize Connection1 ConnectionLine2 MoveLastShapePointFromCnctPt1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeCnct1CnctLine2MoveLastShapePointFromCnctPt1SelectLine(200,300);

static const QPoint s_ptDrawingSceneMousePosResizeCnct1CnctLine2MoveLastShapePointFromCnctPt1SelectShapePoint(150,250);

// Resize Connection1 ConnectionLine2 MoveLastShapePointFromCnctPt1
//------------------------------------------------------------------------------

static const QSize s_arsizeConnectionsResizeCnct1CnctLine2MoveLastShapePointFromCnctPt1TestSteps[] = {
                        /* CnctLine2: [0],       [1],       [2],       [3] */
    /*  0 */ QSize( 10, 0 ), /* (150/380), (200/340), (200/260), (160/250) */
    /*  1 */ QSize( 10, 0 ), /* (150/380), (200/340), (200/260), (170/250) */
    /*  2 */ QSize( 10, 0 ), /* (150/380), (200/340), (200/260), (180/250) */
    /*  3 */ QSize( 10, 0 ), /* (150/380), (200/340), (200/260), (190/250) */
    /*  4 */ QSize( 10, 0 )  /* (150/380), (200/340), (200/260), (200/250) */
};

static const int ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_TestStepCount = sizeof(s_arsizeConnectionsResizeCnct1CnctLine2MoveLastShapePointFromCnctPt1TestSteps)/sizeof(s_arsizeConnectionsResizeCnct1CnctLine2MoveLastShapePointFromCnctPt1TestSteps[0]);

// Resize Connection1 ConnectionLine2 MoveFirstShapePointFromCnctPt3
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt3SelectLine(200,300);

static const QPoint s_ptDrawingSceneMousePosResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt3SelectShapePoint(150,380);

// Resize Connection1 ConnectionLine2 MoveFirstShapePointFromCnctPt3
//------------------------------------------------------------------------------

static const QSize s_arsizeConnectionsResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt3TestSteps[] = {
                        /* CnctLine2: [0],       [1],       [2],       [3] */
    /*  0 */ QSize( 10, 0 ), /* (160/380), (200/340), (200/260), (160/250) */
    /*  1 */ QSize( 10, 0 ), /* (170/380), (200/340), (200/260), (170/250) */
    /*  2 */ QSize( 10, 0 ), /* (180/380), (200/340), (200/260), (180/250) */
    /*  3 */ QSize( 10, 0 ), /* (190/380), (200/340), (200/260), (190/250) */
    /*  4 */ QSize( 10, 0 )  /* (200/380), (200/340), (200/260), (200/250) */
};

static const int ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_TestStepCount = sizeof(s_arsizeConnectionsResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt3TestSteps)/sizeof(s_arsizeConnectionsResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt3TestSteps[0]);


// Test steps:
//------------------------------------------------------------------------------

enum
{
    ETestStepLev3_Connections_Create_Cnct1_CnctPt1_ActivateDrawingTool,
    ETestStepLev3_Connections_Create_Cnct1_CnctPt1,
    ETestStepLev3_Connections_Create_Cnct1_CnctPt2,
    ETestStepLev3_Connections_Create_Cnct1_CnctPt3,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine1_ActivateDrawingTool,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine1_ConnectAtPt1,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateFirstLineSegment_TestStepMin,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateFirstLineSegment_TestStepMax = ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateFirstLineSegment_TestStepMin + ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateFirstLineSegment_TestStepCount - 1,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateFirstLineSegment_Finished,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateSecondLineSegment_TestStepMin,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateSecondLineSegment_TestStepMax = ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateSecondLineSegment_TestStepMin + ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateSecondLineSegment_TestStepCount - 1,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine1_ConnectAtPt2,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_ConnectAtPt2,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFirstLineSegment_TestStepMin,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFirstLineSegment_TestStepMax = ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFirstLineSegment_TestStepMin + ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFirstLineSegment_TestStepCount - 1,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFirstLineSegment_Finished,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateSecondLineSegment_TestStepMin,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateSecondLineSegment_TestStepMax = ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateSecondLineSegment_TestStepMin + ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateSecondLineSegment_TestStepCount - 1,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_ConnectAtPt1,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_ActivateEditModeSelect,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_Select,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_TestStepMin,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_TestStepMax =  ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_TestStepMin + ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_TestStepCount - 1,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_Release,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_Finished,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_Select,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_TestStepMin,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_TestStepMax =  ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_TestStepMin + ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_TestStepCount - 1,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_Release,
    ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_Finished,
    ETestStepLev3_Connections_Move_Cnct1_ActivateEditModeSelect,
    ETestStepLev3_Connections_Move_Cnct1_CnctPt1_Select,
    ETestStepLev3_Connections_Move_Cnct1_CnctPt1_TestStepMin,
    ETestStepLev3_Connections_Move_Cnct1_CnctPt1_TestStepMax = ETestStepLev3_Connections_Move_Cnct1_CnctPt1_TestStepMin + ETestStepLev3_Connections_Move_Cnct1_CnctPt1_TestStepCount - 1,
    ETestStepLev3_Connections_Move_Cnct1_CnctPt1_Release,
    ETestStepLev3_Connections_Move_Cnct1_CnctPt2_Select,
    ETestStepLev3_Connections_Move_Cnct1_CnctPt2_TestStepMin,
    ETestStepLev3_Connections_Move_Cnct1_CnctPt2_TestStepMax = ETestStepLev3_Connections_Move_Cnct1_CnctPt2_TestStepMin + ETestStepLev3_Connections_Move_Cnct1_CnctPt2_TestStepCount - 1,
    ETestStepLev3_Connections_Move_Cnct1_CnctPt2_Release,
    ETestStepLev3_Connections_Move_Cnct1_Finished,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_ActivateEditModeSelect,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_SelectLine,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_SelectShapePoint,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_TestStepMin,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_TestStepMax =  ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_TestStepMin + ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_TestStepCount - 1,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_Finished,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointToCnctPt3_TestStepMin,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointToCnctPt3_TestStepMax =  ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointToCnctPt3_TestStepMin + ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointToCnctPt3_TestStepCount - 1,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointToCnctPt3_Finished,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_SelectLine,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_SelectShapePoint,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_TestStepMin,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_TestStepMax =  ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_TestStepMin + ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_TestStepCount - 1,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_Finished,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_SelectLine,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_SelectShapePoint,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_TestStepMin,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_TestStepMax =  ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_TestStepMin + ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_TestStepCount - 1,
    ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_Finished,
    ETestStepLev3_Connections_Resize_Finished,
    ETestStepLev3_Connections_Count
};

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepStandardShapesConnections( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
//------------------------------------------------------------------------------
{
    QMouseEvent*              pMouseEv = nullptr;
    QPoint                    ptMousePosGlobal;
    QString                   strCurrentDrawingTool;
    QString                   strCurrentGraphObj;
    QString                   strToolTip;
    ZS::Test::CTestStep*      pTestStep;
    QStringList               strlstDesiredValues;
    QStringList               strlstActualValues;
    CGraphObj*                pGraphObj;
    QList<QGraphicsItem*>     arpGraphicsItems;
    CGraphObjConnectionLine*  pGraphObjCnctLine;
    CGraphObjConnectionPoint* pGraphObjCnctPt;
    int                       idxGraphObj;

    ZS::Test::CTestStepGroup* pTSGrpConnections       = getTestStepGroup( "Connections", i_pTestStepGrpParent );
    ZS::Test::CTestStepGroup* pTSGrpConnectionsCreate = getTestStepGroup( "Create", pTSGrpConnections );
    ZS::Test::CTestStepGroup* pTSGrpConnectionsMove   = getTestStepGroup( "Move", pTSGrpConnections );
    ZS::Test::CTestStepGroup* pTSGrpConnectionsResize = getTestStepGroup( "Resize", pTSGrpConnections );

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }

    if( !pTSGrpConnections->isEnabled() )
    {
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else if( !pTSGrpConnectionsCreate->isEnabled() )
    {
        // If no connection is created none of the other sub tests may be performed.
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else // if( pTSGrpConnections->isEnabled() )
    {
        if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctPt1_ActivateDrawingTool )
        {
            pTestStep = getTestStep( "Connection1.ConnectionPoint1.ActivateDrawingToolConnectionPoint", pTSGrpConnectionsCreate );

            strlstDesiredValues.append( "EditTool: CreateObjects" );
            strlstDesiredValues.append( "DrawingTool: ConnectionPoint" );

            pTestStep->setOperation( "drawing.setCurrentDrawingTool( ConnectionPoint )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            if( (m_pDrawingScene->getEditTool() != EEditToolCreateObjects) || (m_pDrawingScene->getCurrentDrawingToolGraphObjType() != EGraphObjTypeConnectionPoint) )
            {
                m_pMainWindow->setCheckedActionDrawConnectionPoint(true);
            }

            if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
            {
                strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
            }
            else
            {
                strCurrentDrawingTool = "";
            }

            strlstActualValues.append( "EditTool: " + editTool2Str(m_pDrawingScene->getEditTool()) );
            strlstActualValues.append( "DrawingTool: " + strCurrentDrawingTool );

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctPt1_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctPt1 )
        {
            m_iConnectionPointsCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionPoint" + QString::number(m_iConnectionPointsCount);

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".Create", pTSGrpConnectionsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateCnct1CnctPt1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctCnctPt1 = QRectF(
                /* left   */ m_ptDrawingSceneMousePos.x()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* top    */ m_ptDrawingSceneMousePos.y()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* width  */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* height */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx() );

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctCnctPt1) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".create( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            CGraphObj* pGraphObjCreated = nullptr;

            arpGraphicsItems = m_pDrawingScene->items(m_ptDrawingSceneMousePos);

            if( arpGraphicsItems.size() >= 1 )
            {
                pGraphObjCreated = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = pGraphObjCreated->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObjCreated->getScenePolygonShapePointsString() );
                strToolTip = pGraphObjCreated->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctPt1 )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctPt2 )
        {
            m_iConnectionPointsCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionPoint" + QString::number(m_iConnectionPointsCount);

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".Create", pTSGrpConnectionsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateCnct1CnctPt2Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctCnctPt2 = QRectF(
                /* left   */ m_ptDrawingSceneMousePos.x()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* top    */ m_ptDrawingSceneMousePos.y()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* width  */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* height */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx() );

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctCnctPt2) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".create( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            CGraphObj* pGraphObjCreated = nullptr;

            arpGraphicsItems = m_pDrawingScene->items(m_ptDrawingSceneMousePos);

            if( arpGraphicsItems.size() >= 1 )
            {
                pGraphObjCreated = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = pGraphObjCreated->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObjCreated->getScenePolygonShapePointsString() );
                strToolTip = pGraphObjCreated->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctPt2 )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctPt3 )
        {
            m_iConnectionPointsCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionPoint" + QString::number(m_iConnectionPointsCount);

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".Create", pTSGrpConnectionsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateCnct1CnctPt3Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctCnctPt3 = QRectF(
                /* left   */ m_ptDrawingSceneMousePos.x()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* top    */ m_ptDrawingSceneMousePos.y()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* width  */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* height */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx() );

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctCnctPt3) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".create( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            CGraphObj* pGraphObjCreated = nullptr;

            arpGraphicsItems = m_pDrawingScene->items(m_ptDrawingSceneMousePos);

            if( arpGraphicsItems.size() >= 1 )
            {
                pGraphObjCreated = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = pGraphObjCreated->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObjCreated->getScenePolygonShapePointsString() );
                strToolTip = pGraphObjCreated->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctPt3 )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine1_ActivateDrawingTool )
        {
            pTestStep = getTestStep( "Connection1.ConnectionLine1.ActivateDrawingToolConnectionLine", pTSGrpConnectionsCreate );

            strlstDesiredValues.append( "EditTool: CreateObjects" );
            strlstDesiredValues.append( "DrawingTool: ConnectionLine" );

            pTestStep->setOperation( "drawing.setCurrentDrawingTool( ConnectionLine )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            if( (m_pDrawingScene->getEditTool() != EEditToolCreateObjects) || (m_pDrawingScene->getCurrentDrawingToolGraphObjType() != EGraphObjTypeConnectionLine) )
            {
                m_pMainWindow->setCheckedActionDrawConnectionLine(true);
            }

            if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
            {
                strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
            }
            else
            {
                strCurrentDrawingTool = "";
            }

            strlstActualValues.append( "EditTool: " + editTool2Str(m_pDrawingScene->getEditTool()) );
            strlstActualValues.append( "DrawingTool: " + strCurrentDrawingTool );

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine1_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine1_ConnectAtPt1 )
        {
            m_iConnectionLinesCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".CreateAtPt1", pTSGrpConnectionsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateCnct1CnctPt1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine1Prev.clear();

            s_plgCnctLine1Curr.clear();
            s_plgCnctLine1Curr.append(m_ptDrawingSceneMousePos);
            s_plgCnctLine1Curr.append(m_ptDrawingSceneMousePos);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine1Curr) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".create( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            CGraphObj* pGraphObj = nullptr;

            arpGraphicsItems = m_pDrawingScene->items();

            for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
            {
                pGraphObj = dynamic_cast<CGraphObj*>(arpGraphicsItems[idxGraphObj]);

                if( pGraphObj->getObjName() == strCurrentGraphObj )
                {
                    strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                    strToolTip = pGraphObj->getToolTip();
                }
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine1_ConnectAtPt1 )

        else if( m_iTestStepLev3 >= ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateFirstLineSegment_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateFirstLineSegment_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".CreateFirstLineSegment.Step" + QString::number(m_iTestStepLev4), pTSGrpConnectionsCreate );

            m_fxOffsHor = s_arsizeConnectionsCreateCnct1CnctLine1CreateFirstLineSegmentTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeConnectionsCreateCnct1CnctLine1CreateFirstLineSegmentTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine1Curr[1] = m_ptDrawingSceneMousePos;

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine1Curr) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".moveTo( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseMoveEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjCreating = m_pDrawingScene->getGraphObjCreating();

            if( m_pGraphObjCreating != nullptr )
            {
                strCurrentGraphObj = m_pGraphObjCreating->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateFirstLineSegment_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateFirstLineSegment_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateFirstLineSegment_Finished )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".CreateFirstLineSegment.Finished", pTSGrpConnectionsCreate );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine1Curr.append(m_ptDrawingSceneMousePos);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine1Curr) );

            pTestStep->setOperation( "view.mouseClickEvent( " + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingLineSegmentFinished( " + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjCreating = m_pDrawingScene->getGraphObjCreating();

            if( m_pGraphObjCreating != nullptr )
            {
                strCurrentGraphObj = m_pGraphObjCreating->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateFirstLineSegment_Finished )

        else if( m_iTestStepLev3 >= ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateSecondLineSegment_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateSecondLineSegment_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".CreateSecondLineSegment.Step" + QString::number(m_iTestStepLev4), pTSGrpConnectionsCreate );

            m_fxOffsHor = s_arsizeConnectionsCreateCnct1CnctLine1CreateSecondLineSegmentTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeConnectionsCreateCnct1CnctLine1CreateSecondLineSegmentTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine1Curr[2] = m_ptDrawingSceneMousePos;

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine1Curr) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".moveTo( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseMoveEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjCreating = m_pDrawingScene->getGraphObjCreating();

            if( m_pGraphObjCreating != nullptr )
            {
                strCurrentGraphObj = m_pGraphObjCreating->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateSecondLineSegment_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Create_Cnct1_CnctLine1_CreateSecondLineSegment_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine1_ConnectAtPt2 )
        {
            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".ConnectAtPt2", pTSGrpConnectionsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateCnct1CnctPt2Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine1Curr[2] = m_ptDrawingSceneMousePos;

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine1Curr) );
            strlstDesiredValues.append( "ConnectionPoint1: " + polygon2Str(s_rctCnctPt1) );
            strlstDesiredValues.append( "ConnectionPoint2: " + polygon2Str(s_rctCnctPt2) );

            pTestStep->setOperation( "view.mouseDoubleClickEvent( " + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingObjectFinished( " + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            // Same events as in reality (mouse double click is preceeded by mouse press and mouse release event)
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonDblClick,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseDoubleClickEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            pGraphObj = m_pDrawingScene->findGraphObj(strCurrentGraphObj);

            pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj);

            if( pGraphObjCnctLine != nullptr )
            {
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObjCnctLine->getScenePolygonShapePointsString() );

                pGraphObjCnctPt = pGraphObjCnctLine->getConnectionPoint(ELinePointStart);
                if( pGraphObjCnctPt != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctPt->getObjName() + ": " + pGraphObjCnctPt->getScenePolygonShapePointsString() );
                }

                pGraphObjCnctPt = pGraphObjCnctLine->getConnectionPoint(ELinePointEnd);
                if( pGraphObjCnctPt != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctPt->getObjName() + ": " + pGraphObjCnctPt->getScenePolygonShapePointsString() );
                }

                strToolTip = pGraphObjCnctLine->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine1_ConnectAtPt2 )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_ConnectAtPt2 )
        {
            m_iConnectionLinesCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".CreateAtPt2", pTSGrpConnectionsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateCnct1CnctPt2Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Prev.clear();

            s_plgCnctLine2Curr.clear();
            s_plgCnctLine2Curr.append(m_ptDrawingSceneMousePos);
            s_plgCnctLine2Curr.append(m_ptDrawingSceneMousePos);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".create( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            CGraphObj* pGraphObj = nullptr;

            arpGraphicsItems = m_pDrawingScene->items();

            for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
            {
                pGraphObj = dynamic_cast<CGraphObj*>(arpGraphicsItems[idxGraphObj]);

                if( pGraphObj->getObjName() == strCurrentGraphObj )
                {
                    strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                    strToolTip = pGraphObj->getToolTip();
                }
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_ConnectAtPt2 )

        else if( m_iTestStepLev3 >= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFirstLineSegment_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFirstLineSegment_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".CreateFirstLineSegment.Step" + QString::number(m_iTestStepLev4), pTSGrpConnectionsCreate );

            m_fxOffsHor = s_arsizeConnectionsCreateCnct1CnctLine2CreateFirstLineSegmentTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeConnectionsCreateCnct1CnctLine2CreateFirstLineSegmentTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Curr[1] = m_ptDrawingSceneMousePos;

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".moveTo( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseMoveEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjCreating = m_pDrawingScene->getGraphObjCreating();

            if( m_pGraphObjCreating != nullptr )
            {
                strCurrentGraphObj = m_pGraphObjCreating->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFirstLineSegment_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFirstLineSegment_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFirstLineSegment_Finished )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".CreateFirstLineSegment.Finished", pTSGrpConnectionsCreate );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Curr.append(m_ptDrawingSceneMousePos);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "view.mouseClickEvent( " + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingLineSegmentFinished( " + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjCreating = m_pDrawingScene->getGraphObjCreating();

            if( m_pGraphObjCreating != nullptr )
            {
                strCurrentGraphObj = m_pGraphObjCreating->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFirstLineSegment_Finished )

        else if( m_iTestStepLev3 >= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateSecondLineSegment_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateSecondLineSegment_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".CreateSecondLineSegment.Step" + QString::number(m_iTestStepLev4), pTSGrpConnectionsCreate );

            m_fxOffsHor = s_arsizeConnectionsCreateCnct1CnctLine2CreateSecondLineSegmentTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeConnectionsCreateCnct1CnctLine2CreateSecondLineSegmentTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Curr[2] = m_ptDrawingSceneMousePos;

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".moveTo( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseMoveEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjCreating = m_pDrawingScene->getGraphObjCreating();

            if( m_pGraphObjCreating != nullptr )
            {
                strCurrentGraphObj = m_pGraphObjCreating->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateSecondLineSegment_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateSecondLineSegment_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_ConnectAtPt1 )
        {
            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".ConnectAtPt1", pTSGrpConnectionsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateCnct1CnctPt1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Curr[2] = m_ptDrawingSceneMousePos;

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );
            strlstDesiredValues.append( "ConnectionPoint2: " + polygon2Str(s_rctCnctPt2) );
            strlstDesiredValues.append( "ConnectionPoint1: " + polygon2Str(s_rctCnctPt1) );

            pTestStep->setOperation( "view.mouseDoubleClickEvent( " + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingObjectFinished( " + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            // Same events as in reality (mouse double click is preceeded by mouse press and mouse release event)
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonDblClick,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseDoubleClickEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            pGraphObj = m_pDrawingScene->findGraphObj(strCurrentGraphObj);

            pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj);

            if( pGraphObjCnctLine != nullptr )
            {
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObjCnctLine->getScenePolygonShapePointsString() );

                pGraphObjCnctPt = pGraphObjCnctLine->getConnectionPoint(ELinePointStart);
                if( pGraphObjCnctPt != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctPt->getObjName() + ": " + pGraphObjCnctPt->getScenePolygonShapePointsString() );
                }

                pGraphObjCnctPt = pGraphObjCnctLine->getConnectionPoint(ELinePointEnd);
                if( pGraphObjCnctPt != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctPt->getObjName() + ": " + pGraphObjCnctPt->getScenePolygonShapePointsString() );
                }

                strToolTip = pGraphObjCnctLine->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_ConnectAtPt1 )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_ActivateEditModeSelect )
        {
            pTestStep = getTestStep( "Connection1.ConnectionLine2.ActivateEditModeSelect", pTSGrpConnectionsCreate );

            strlstDesiredValues.append( "EditTool: Select" );
            strlstDesiredValues.append( "DrawingTool: " );

            pTestStep->setOperation( "drawing.setCurrentDrawingTool( Select )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            if( m_pDrawingScene->getEditTool() != EEditToolSelect )
            {
                m_pMainWindow->setCheckedActionEditSelect(true);
            }

            if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
            {
                strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
            }
            else
            {
                strCurrentDrawingTool = "";
            }

            strlstActualValues.append( "EditTool: " + editTool2Str(m_pDrawingScene->getEditTool()) );
            strlstActualValues.append( "DrawingTool: " + strCurrentDrawingTool );

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".CreateThirdLineSegment.Select", pTSGrpConnectionsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateCnct1CnctLine2CreateThirdLineSegmentStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Prev = s_plgCnctLine2Curr;

            s_plgCnctLine2Curr.insert( 2, m_ptDrawingSceneMousePos );

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(2) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(m_pGraphObjSelected->getSelectedPolygonShapePointIndex()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".CreateThirdLineSegment.MovePt2.Step" + QString::number(m_iTestStepLev4), pTSGrpConnectionsCreate );

            m_fxOffsHor = s_arsizeConnectionsCreateCnct1CnctLine2CreateThirdLineSegmentTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeConnectionsCreateCnct1CnctLine2CreateThirdLineSegmentTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Curr[2] = m_ptDrawingSceneMousePos;

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "drawing.mouseMoveEvent( " + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".moveTo( " + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseMoveEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_Release )
        {
            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".CreateThirdLineSegment.Release", pTSGrpConnectionsCreate );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_Finished )
        {
            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateThirdLineSegment_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".CreateFourthLineSegment.Select", pTSGrpConnectionsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateCnct1CnctLine2CreateFourthLineSegmentStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Prev = s_plgCnctLine2Curr;

            s_plgCnctLine2Curr.insert( 1, m_ptDrawingSceneMousePos );

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(1) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(m_pGraphObjSelected->getSelectedPolygonShapePointIndex()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".CreateFourthLineSegment.MovePt1.Step" + QString::number(m_iTestStepLev4), pTSGrpConnectionsCreate );

            m_fxOffsHor = s_arsizeConnectionsCreateCnct1CnctLine2CreateFourthLineSegmentTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeConnectionsCreateCnct1CnctLine2CreateFourthLineSegmentTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Curr[1] = m_ptDrawingSceneMousePos;

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "drawing.mouseMoveEvent( " + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".moveTo( " + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseMoveEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_Release )
        {
            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".CreateFourthLineSegment.Release", pTSGrpConnectionsCreate );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Prev = s_plgCnctLine2Curr;

            s_plgCnctLine2Curr.remove(2);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_Finished )
        {
            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Create_Cnct1_CnctLine2_CreateFourthLineSegment_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Move_Cnct1_ActivateEditModeSelect )
        {
            if( !pTSGrpConnectionsMove->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Connections_Move_Cnct1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpConnectionsMove->isEnabled() )
            {
                pTestStep = getTestStep( "Connection1.ActivateEditModeSelect", pTSGrpConnectionsMove );

                strlstDesiredValues.append( "EditTool: Select" );
                strlstDesiredValues.append( "DrawingTool: " );

                pTestStep->setOperation( "drawing.setCurrentDrawingTool( Select )" );
                pTestStep->setDesiredValues(strlstDesiredValues);
                pTestStep->testStarted();

                if( m_pDrawingScene->getEditTool() != EEditToolSelect )
                {
                    m_pMainWindow->setCheckedActionEditSelect(true);
                }

                if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
                {
                    strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
                }
                else
                {
                    strCurrentDrawingTool = "";
                }

                strlstActualValues.append( "EditTool: " + editTool2Str(m_pDrawingScene->getEditTool()) );
                strlstActualValues.append( "DrawingTool: " + strCurrentDrawingTool );

                pTestStep->setActualValues(strlstActualValues);
                pTestStep->setToolTip(strToolTip);
                pTestStep->testEnded();

                m_iTestStepLev3++;  // Next sub test step
                triggerDoTestStep();

            } // if( pTSGrpConnectionsMove->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Move_Cnct1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Move_Cnct1_CnctPt1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionPoint1";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".Select", pTSGrpConnectionsMove );

            // Deselect connection line:
            m_ptDrawingSceneMousePos = QPoint(1,1);

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            // Click on connection point:
            m_ptDrawingSceneMousePos = s_rctCnctPt1.center().toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctCnctPt1) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Move_Cnct1_CnctPt1_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Connections_Move_Cnct1_CnctPt1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Move_Cnct1_CnctPt1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionPoint1";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + "Move.Step" + QString::number(m_iTestStepLev4), pTSGrpConnectionsMove );

            m_fxOffsHor = s_arsizeConnectionsMoveCnct1CnctPt1TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeConnectionsMoveCnct1CnctPt1TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctCnctPt1 = QRectF(
                /* left   */ m_ptDrawingSceneMousePos.x()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* top    */ m_ptDrawingSceneMousePos.y()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* width  */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* height */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx() );

            s_plgCnctLine1Curr[0] = s_rctCnctPt1.center();

            if( s_plgCnctLine2Curr.size() >= 4 )
            {
                s_plgCnctLine2Curr[3] = s_rctCnctPt1.center();
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctCnctPt1) );
            strlstDesiredValues.append( "ConnectionLine1: " + polygon2Str(s_plgCnctLine1Curr) );
            strlstDesiredValues.append( "ConnectionLine2: " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "drawing.mouseMoveEvent( " + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".moveTo( " + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseMoveEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pGraphObj = m_pDrawingScene->findGraphObj("ConnectionPoint1");

            pGraphObjCnctPt = dynamic_cast<CGraphObjConnectionPoint*>(pGraphObj);

            if( pGraphObjCnctPt != nullptr )
            {
                pGraphObjCnctLine = pGraphObjCnctPt->getConnectionLine(0);

                if( pGraphObjCnctLine != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctLine->getObjName() + ": " + pGraphObjCnctLine->getScenePolygonShapePointsString() );
                }

                pGraphObjCnctLine = pGraphObjCnctPt->getConnectionLine(1);

                if( pGraphObjCnctLine != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctLine->getObjName() + ": " + pGraphObjCnctLine->getScenePolygonShapePointsString() );
                }
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Connections_Move_Cnct1_CnctPt1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Move_Cnct1_CnctPt1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Move_Cnct1_CnctPt1_Release )
        {
            strCurrentGraphObj = "ConnectionPoint1";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".Release", pTSGrpConnectionsMove );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctCnctPt1) );
            strlstDesiredValues.append( "ConnectionLine1: " + polygon2Str(s_plgCnctLine1Curr) );
            strlstDesiredValues.append( "ConnectionLine2: " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pGraphObj = m_pDrawingScene->findGraphObj("ConnectionPoint1");

            pGraphObjCnctPt = dynamic_cast<CGraphObjConnectionPoint*>(pGraphObj);

            if( pGraphObjCnctPt != nullptr )
            {
                pGraphObjCnctLine = pGraphObjCnctPt->getConnectionLine(0);

                if( pGraphObjCnctLine != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctLine->getObjName() + ": " + pGraphObjCnctLine->getScenePolygonShapePointsString() );
                }

                pGraphObjCnctLine = pGraphObjCnctPt->getConnectionLine(1);

                if( pGraphObjCnctLine != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctLine->getObjName() + ": " + pGraphObjCnctLine->getScenePolygonShapePointsString() );
                }
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Move_Cnct1_CnctPt1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Move_Cnct1_CnctPt2_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionPoint2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".Select", pTSGrpConnectionsMove );

            m_ptDrawingSceneMousePos = s_rctCnctPt2.center().toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctCnctPt2) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Move_Cnct1_CnctPt2_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Connections_Move_Cnct1_CnctPt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Move_Cnct1_CnctPt2_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionPoint2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + "Move.Step" + QString::number(m_iTestStepLev4), pTSGrpConnectionsMove );

            m_fxOffsHor = s_arsizeConnectionsMoveCnct1CnctPt2TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeConnectionsMoveCnct1CnctPt2TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctCnctPt2 = QRectF(
                /* left   */ m_ptDrawingSceneMousePos.x()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* top    */ m_ptDrawingSceneMousePos.y()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* width  */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* height */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx() );

            if( s_plgCnctLine1Curr.size() >= 3 )
            {
                s_plgCnctLine1Curr[2] = s_rctCnctPt2.center();
            }
            s_plgCnctLine2Curr[0] = s_rctCnctPt2.center();

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctCnctPt2) );
            strlstDesiredValues.append( "ConnectionLine1: " + polygon2Str(s_plgCnctLine1Curr) );
            strlstDesiredValues.append( "ConnectionLine2: " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "drawing.mouseMoveEvent( " + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".moveTo( " + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseMoveEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pGraphObj = m_pDrawingScene->findGraphObj("ConnectionPoint2");

            pGraphObjCnctPt = dynamic_cast<CGraphObjConnectionPoint*>(pGraphObj);

            if( pGraphObjCnctPt != nullptr )
            {
                pGraphObjCnctLine = pGraphObjCnctPt->getConnectionLine(0);

                if( pGraphObjCnctLine != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctLine->getObjName() + ": " + pGraphObjCnctLine->getScenePolygonShapePointsString() );
                }

                pGraphObjCnctLine = pGraphObjCnctPt->getConnectionLine(1);

                if( pGraphObjCnctLine != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctLine->getObjName() + ": " + pGraphObjCnctLine->getScenePolygonShapePointsString() );
                }
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Connections_Move_Cnct1_CnctPt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Move_Cnct1_CnctPt2_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Move_Cnct1_CnctPt2_Release )
        {
            strCurrentGraphObj = "ConnectionPoint2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".Release", pTSGrpConnectionsMove );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctCnctPt2) );
            strlstDesiredValues.append( "ConnectionLine1: " + polygon2Str(s_plgCnctLine1Curr) );
            strlstDesiredValues.append( "ConnectionLine2: " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pGraphObj = m_pDrawingScene->findGraphObj("ConnectionPoint2");

            pGraphObjCnctPt = dynamic_cast<CGraphObjConnectionPoint*>(pGraphObj);

            if( pGraphObjCnctPt != nullptr )
            {
                pGraphObjCnctLine = pGraphObjCnctPt->getConnectionLine(0);

                if( pGraphObjCnctLine != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctLine->getObjName() + ": " + pGraphObjCnctLine->getScenePolygonShapePointsString() );
                }

                pGraphObjCnctLine = pGraphObjCnctPt->getConnectionLine(1);

                if( pGraphObjCnctLine != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctLine->getObjName() + ": " + pGraphObjCnctLine->getScenePolygonShapePointsString() );
                }
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Move_Cnct1_CnctPt2_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Move_Cnct1_Finished )
        {
            pTSGrpConnectionsMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Move_Cnct1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_ActivateEditModeSelect )
        {
            if( !pTSGrpConnectionsResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Connections_Resize_Finished+1; // Last resize test step
                triggerDoTestStep();
            }
            else // if( pTSGrpConnectionsResize->isEnabled() )
            {
                pTestStep = getTestStep( "Connection1.ConnectionLine1.ActivateEditModeSelect", pTSGrpConnectionsResize );

                strlstDesiredValues.append( "EditTool: Select" );
                strlstDesiredValues.append( "DrawingTool: " );

                pTestStep->setOperation( "drawing.setCurrentDrawingTool( Select )" );
                pTestStep->setDesiredValues(strlstDesiredValues);
                pTestStep->testStarted();

                if( m_pDrawingScene->getEditTool() != EEditToolSelect )
                {
                    m_pMainWindow->setCheckedActionEditSelect(true);
                }

                if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
                {
                    strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
                }
                else
                {
                    strCurrentDrawingTool = "";
                }

                strlstActualValues.append( "EditTool: " + editTool2Str(m_pDrawingScene->getEditTool()) );
                strlstActualValues.append( "DrawingTool: " + strCurrentDrawingTool );

                pTestStep->setActualValues(strlstActualValues);
                pTestStep->setToolTip(strToolTip);
                pTestStep->testEnded();

                m_iTestStepLev3++;  // Next sub test step
                triggerDoTestStep();

            } // if( pTSGrpConnectionsResize->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_SelectLine )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".MoveFirstShapePointFromCnctPt2.SelectLine", pTSGrpConnectionsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt2SelectLine;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append(strCurrentGraphObj);

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append(strCurrentGraphObj);
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePoint_SelectLine )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_SelectShapePoint )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".MoveFirstShapePointFromCnctPt2.SelectShapePoint", pTSGrpConnectionsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt2SelectShapePoint;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(0) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(m_pGraphObjSelected->getSelectedPolygonShapePointIndex()) );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_SelectShapePoint )

        else if( m_iTestStepLev3 >= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".MoveFirstShapePointFromCnctPt2.Step" + QString::number(m_iTestStepLev4), pTSGrpConnectionsResize );

            m_fxOffsHor = s_arsizeConnectionsResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt2TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeConnectionsResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt2TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Curr[0] = m_ptDrawingSceneMousePos;

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "drawing.mouseMoveEvent( " + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".moveTo( " + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseMoveEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_Finished )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".MoveFirstShapePointFromCnctPt2.Finished", pTSGrpConnectionsResize );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Curr.insert( 0, m_ptDrawingSceneMousePos );

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt2_Finished )

        else if( m_iTestStepLev3 >= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointToCnctPt3_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointToCnctPt3_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".MoveFirstShapePointToCnctPt3.Step" + QString::number(m_iTestStepLev4), pTSGrpConnectionsResize );

            m_fxOffsHor = s_arsizeConnectionsResizeCnct1CnctLine2MoveFirstShapePointToCnctPt3TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeConnectionsResizeCnct1CnctLine2MoveFirstShapePointToCnctPt3TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Curr[0] = m_ptDrawingSceneMousePos;

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "drawing.mouseMoveEvent( " + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".moveTo( " + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseMoveEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointToCnctPt3_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointToCnctPt3_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointToCnctPt3_Finished )
        {
            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".MoveFirstShapePointToCnctPt3.ConnectAtPt3" + QString::number(m_iTestStepLev4), pTSGrpConnectionsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateCnct1CnctPt3Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Curr[0] = m_ptDrawingSceneMousePos;

            s_plgCnctLine2Curr = normalizePolygon( s_plgCnctLine2Curr, m_pDrawingScene->getHitToleranceInPx() );

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );
            strlstDesiredValues.append( "ConnectionPoint3: " + polygon2Str(s_rctCnctPt3) );
            strlstDesiredValues.append( "ConnectionPoint1: " + polygon2Str(s_rctCnctPt1) );

            pTestStep->setOperation( "view.mouseDoubleClickEvent( " + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".movingShapePointFinished( " + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            // Same events as in reality (mouse double click is preceeded by mouse press and mouse release event)
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            // The mouse button has been released on a connection point and the
            // connection line has already been connected with this connection point.
            // A double click now would open the format object dialog. This is not
            // what we want here.
            //pMouseEv = new QMouseEvent(
            //    /* type      */ QEvent::MouseButtonDblClick,
            //    /* pos       */ m_ptDrawingViewMousePos,
            //    /* globalPos */ ptMousePosGlobal,
            //    /* button    */ Qt::LeftButton,
            //    /* buttons   */ Qt::NoButton,
            //    /* modifiers */ Qt::NoModifier );
            //m_pDrawingView->mouseDoubleClickEvent(pMouseEv);
            //delete pMouseEv;
            //pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            pGraphObj = m_pDrawingScene->findGraphObj(strCurrentGraphObj);

            pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj);

            if( pGraphObjCnctLine != nullptr )
            {
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObjCnctLine->getScenePolygonShapePointsString() );

                pGraphObjCnctPt = pGraphObjCnctLine->getConnectionPoint(ELinePointStart);
                if( pGraphObjCnctPt != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctPt->getObjName() + ": " + pGraphObjCnctPt->getScenePolygonShapePointsString() );
                }

                pGraphObjCnctPt = pGraphObjCnctLine->getConnectionPoint(ELinePointEnd);
                if( pGraphObjCnctPt != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctPt->getObjName() + ": " + pGraphObjCnctPt->getScenePolygonShapePointsString() );
                }

                strToolTip = pGraphObjCnctLine->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointToCnctPt3_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_SelectLine )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".MoveLastShapePointFromCnctPt1.SelectLine", pTSGrpConnectionsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeCnct1CnctLine2MoveLastShapePointFromCnctPt1SelectLine;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append(strCurrentGraphObj);

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append(strCurrentGraphObj);
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_SelectLine )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_SelectShapePoint )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".MoveLastShapePointFromCnctPt1.SelectShapePoint", pTSGrpConnectionsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeCnct1CnctLine2MoveLastShapePointFromCnctPt1SelectShapePoint;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Prev = s_plgCnctLine2Curr;

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(s_plgCnctLine2Curr.size()-1) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(m_pGraphObjSelected->getSelectedPolygonShapePointIndex()) );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_SelectShapePoint )

        else if( m_iTestStepLev3 >= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".MoveLastShapePointFromCnctPt1.Step" + QString::number(m_iTestStepLev4), pTSGrpConnectionsResize );

            m_fxOffsHor = s_arsizeConnectionsResizeCnct1CnctLine2MoveLastShapePointFromCnctPt1TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeConnectionsResizeCnct1CnctLine2MoveLastShapePointFromCnctPt1TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_plgCnctLine2Curr.size() == 4 )
            {
                s_plgCnctLine2Curr[3] = m_ptDrawingSceneMousePos;
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "drawing.mouseMoveEvent( " + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".moveTo( " + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseMoveEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_Finished )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".MoveLastShapePointFromCnctPt1.Finished", pTSGrpConnectionsResize );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_plgCnctLine2Curr.size() == 4 )
            {
                s_plgCnctLine2Curr[2] = m_ptDrawingSceneMousePos;
                s_plgCnctLine2Curr[3] = s_rctCnctPt1.center();
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );
            strlstDesiredValues.append( "ConnectionPoint3: " + polygon2Str(s_rctCnctPt3) );
            strlstDesiredValues.append( "ConnectionPoint1: " + polygon2Str(s_rctCnctPt1) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            // Same events as in reality (mouse double click is preceeded by mouse press and mouse release event)
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonDblClick,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseDoubleClickEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            pGraphObj = m_pDrawingScene->findGraphObj(strCurrentGraphObj);

            pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj);

            if( pGraphObjCnctLine != nullptr )
            {
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObjCnctLine->getScenePolygonShapePointsString() );

                pGraphObjCnctPt = pGraphObjCnctLine->getConnectionPoint(ELinePointStart);
                if( pGraphObjCnctPt != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctPt->getObjName() + ": " + pGraphObjCnctPt->getScenePolygonShapePointsString() );
                }

                pGraphObjCnctPt = pGraphObjCnctLine->getConnectionPoint(ELinePointEnd);
                if( pGraphObjCnctPt != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctPt->getObjName() + ": " + pGraphObjCnctPt->getScenePolygonShapePointsString() );
                }

                strToolTip = pGraphObjCnctLine->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveLastShapePointFromCnctPt1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_SelectLine )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".MoveFirstShapePointFromCnctPt3.SelectLine", pTSGrpConnectionsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt3SelectLine;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append(strCurrentGraphObj);

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append(strCurrentGraphObj);
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_SelectLine )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_SelectShapePoint )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".MoveFirstShapePointFromCnctPt3.SelectShapePoint", pTSGrpConnectionsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt3SelectShapePoint;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Prev = s_plgCnctLine2Curr;

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(0) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(m_pGraphObjSelected->getSelectedPolygonShapePointIndex()) );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_SelectShapePoint )

        else if( m_iTestStepLev3 >= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".MoveFirstShapePointFromCnctPt3.Step" + QString::number(m_iTestStepLev4), pTSGrpConnectionsResize );

            m_fxOffsHor = s_arsizeConnectionsResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt3TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeConnectionsResizeCnct1CnctLine2MoveFirstShapePointFromCnctPt3TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Curr[0] = m_ptDrawingSceneMousePos;

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );

            pTestStep->setOperation( "drawing.mouseMoveEvent( " + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".moveTo( " + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseMoveEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            arpGraphicsItems = m_pDrawingScene->selectedItems();

            if( arpGraphicsItems.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphicsItems[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_Finished )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine2";

            pTestStep = getTestStep( "Connection1." + strCurrentGraphObj + ".MoveFirstShapePointFromCnctPt3.Finished", pTSGrpConnectionsResize );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgCnctLine2Curr[0] = s_rctCnctPt3.center();
            s_plgCnctLine2Curr[1] = m_ptDrawingSceneMousePos;

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgCnctLine2Curr) );
            strlstDesiredValues.append( "ConnectionPoint3: " + polygon2Str(s_rctCnctPt3) );
            strlstDesiredValues.append( "ConnectionPoint1: " + polygon2Str(s_rctCnctPt1) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            // Same events as in reality (mouse double click is preceeded by mouse press and mouse release event)
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonRelease,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;
            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonDblClick,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseDoubleClickEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            pGraphObj = m_pDrawingScene->findGraphObj(strCurrentGraphObj);

            pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj);

            if( pGraphObjCnctLine != nullptr )
            {
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObjCnctLine->getScenePolygonShapePointsString() );

                pGraphObjCnctPt = pGraphObjCnctLine->getConnectionPoint(ELinePointStart);
                if( pGraphObjCnctPt != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctPt->getObjName() + ": " + pGraphObjCnctPt->getScenePolygonShapePointsString() );
                }

                pGraphObjCnctPt = pGraphObjCnctLine->getConnectionPoint(ELinePointEnd);
                if( pGraphObjCnctPt != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctPt->getObjName() + ": " + pGraphObjCnctPt->getScenePolygonShapePointsString() );
                }

                strToolTip = pGraphObjCnctLine->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Cnct1_CnctLine2_MoveFirstShapePointFromCnctPt3_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Finished )
        {
            pTSGrpConnectionsResize->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Connections_Resize_Finished )

        else
        {
            pTSGrpConnections->testEnded();

            m_iTestStepLev2++; // Next test step
            m_iTestStepLev3 = 0;
            triggerDoTestStep();
        }

    } // if( pTSGrpConnections->isEnabled() )

    delete pMouseEv;
    pMouseEv = nullptr;

} // doTestStepStandardShapesConnections

#endif // #if 0
