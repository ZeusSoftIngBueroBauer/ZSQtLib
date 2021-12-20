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

#if 0

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
#include "ZSDraw/ZSDrawGraphObjPolyline.h"
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
using namespace ZS::Trace;


/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

static QString   s_strGraphObjNamePolyline1;
static QPolygonF s_plg1Prev;
static QRectF    s_rctBoundingPlg1Prev;
static QPolygonF s_plg1Curr;
static QRectF    s_rctBoundingPlg1Curr;

static QString   s_strGraphObjNamePolyline2;
static QPolygonF s_plg2Prev;
static QRectF    s_rctBoundingPlg2Prev;
static QPolygonF s_plg2Curr;
static QRectF    s_rctBoundingPlg2Curr;

// Create_Polyline1_FirstLineSegment
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosPolyline1Start(400,200);

static const QSize s_arsizePolylinesCreatePolyline1FirstLineSegmentMovePt2TestSteps[] = {
    /*  0 */ QSize( 8, 20 ),
    /*  1 */ QSize( 8, 20 ),
    /*  2 */ QSize( 8, 20 ),
    /*  3 */ QSize( 8, 20 ),
    /*  4 */ QSize( 8, 20 )
};

static const int ETestStepLev3_Polylines_Create_Polyline1_FirstLineSegment_MovePt2_TestStepCount  = sizeof(s_arsizePolylinesCreatePolyline1FirstLineSegmentMovePt2TestSteps)/sizeof(s_arsizePolylinesCreatePolyline1FirstLineSegmentMovePt2TestSteps[0]);

// Create_Polyline1_SecondLineSegment
//------------------------------------------------------------------------------

static const QSize s_arsizePolylinesCreatePolyline1SecondLineSegmentMovePt3TestSteps[] = {
    /*  0 */ QSize( 12, -8 ),
    /*  1 */ QSize( 12, -8 ),
    /*  2 */ QSize( 12, -8 ),
    /*  3 */ QSize( 12, -8 ),
    /*  4 */ QSize( 12, -8 )
};

static const int ETestStepLev3_Polylines_Create_Polyline1_SecondLineSegment_MovePt3_TestStepCount = sizeof(s_arsizePolylinesCreatePolyline1SecondLineSegmentMovePt3TestSteps)/sizeof(s_arsizePolylinesCreatePolyline1SecondLineSegmentMovePt3TestSteps[0]);

// Create_Polyline2_FirstLineSegment
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosPolyline2Start(550,230);

static const QSize s_arsizePolylinesCreatePolyline2FirstLineSegmentMovePt2TestSteps[] = {
    /*  0 */ QSize( 6, 4 ),
    /*  1 */ QSize( 6, 4 ),
    /*  2 */ QSize( 6, 4 ),
    /*  3 */ QSize( 6, 4 ),
    /*  4 */ QSize( 6, 4 )
};

static const int ETestStepLev3_Polylines_Create_Polyline2_FirstLineSegment_MovePt2_TestStepCount  = sizeof(s_arsizePolylinesCreatePolyline2FirstLineSegmentMovePt2TestSteps)/sizeof(s_arsizePolylinesCreatePolyline2FirstLineSegmentMovePt2TestSteps[0]);

// Create_Polyline2_SecondLineSegment
//------------------------------------------------------------------------------

static const QSize s_arsizePolylinesCreatePolyline2SecondLineSegmentMovePt3TestSteps[] = {
    /*  0 */ QSize( 4, -10 ),
    /*  1 */ QSize( 4, -10 ),
    /*  2 */ QSize( 4, -10 ),
    /*  3 */ QSize( 4, -10 ),
    /*  4 */ QSize( 4, -10 )
};

static const int ETestStepLev3_Polylines_Create_Polyline2_SecondLineSegment_MovePt3_TestStepCount = sizeof(s_arsizePolylinesCreatePolyline2SecondLineSegmentMovePt3TestSteps)/sizeof(s_arsizePolylinesCreatePolyline2SecondLineSegmentMovePt3TestSteps[0]);

// Move_Polyline1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosMovePolyline1Start(421,251);

static const QSize s_arsizePolylinesMovePolyline1Steps[] = {
    /*  0 */ QSize(  -5,   0 ),
    /*  1 */ QSize(  -5,   0 ),
    /*  2 */ QSize(  -5,   0 ),
    /*  3 */ QSize(   0,  -5 ),
    /*  4 */ QSize(   0,  -5 ),
    /*  5 */ QSize(   0,  -5 ),
    /*  6 */ QSize(   5,   0 ),
    /*  7 */ QSize(   5,   0 ),
    /*  8 */ QSize(   5,   0 ),
    /*  9 */ QSize(   5,   0 ),
    /* 10 */ QSize(   5,   0 ),
    /* 11 */ QSize(   5,   0 ),
    /* 12 */ QSize(   0,   5 ),
    /* 13 */ QSize(   0,   5 ),
    /* 14 */ QSize(   0,   5 ),
    /* 15 */ QSize(   0,   5 ),
    /* 16 */ QSize(   0,   5 ),
    /* 17 */ QSize(   0,   5 ),
    /* 18 */ QSize(  -5,  -5 ),
    /* 19 */ QSize(  -5,  -5 ),
    /* 20 */ QSize(  -5,  -5 )
};

static const int ETestStepLev3_Polylines_Move_Polyline1_TestStepCount = sizeof(s_arsizePolylinesMovePolyline1Steps)/sizeof(s_arsizePolylinesMovePolyline1Steps[0]);

// Resize_Polyline1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizePolyline1Select(421,251);

// Resize_Polyline1_MoveBoundingRectPtBottomRight
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtBottomRightStart(500,300);

static const QSize s_arsizePolylinesResizePolyline1MoveBoundingRectPtBottomRightTestSteps[] = {
                                /* RectCoors (l,t,r,b,w,h) */
    /*  0 */ QSize( -25, -25 ), /* 400,200,475,275, 75, 75 */
    /*  1 */ QSize( -25, -25 ), /* 400,200,450,250, 50, 50 */
    /*  2 */ QSize( -25, -25 ), /* 400,200,425,225, 25, 25 */
    /*  3 */ QSize( -25, -25 ), /* 400,200,400,200,  0,  0 */
    /*  4 */ QSize( -25, -25 ), /* 375,175,400,200, 25, 25 */
    /*  5 */ QSize( -25, -25 ), /* 350,150,400,200, 50, 50 */
    /*  6 */ QSize( -25, -25 ), /* 325,125,400,200, 75, 75 */
    /*  7 */ QSize( -25, -25 )  /* 300,100,400,200,100,100 */
};

static const int ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomRight_TestStepCount = sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtBottomRightTestSteps)/sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtBottomRightTestSteps[0]);

// Resize_Polyline1_MoveBoundingRectPtTopLeft
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtTopLeftStart(300,100);

static const QSize s_arsizePolylinesResizePolyline1MoveBoundingRectPtTopLeftTestSteps[] = {
                              /* RectCoors (l,t,r,b,w,h) */
    /*  0 */ QSize( 25, 25 ), /* 325,125,400,200, 75, 75 */
    /*  1 */ QSize( 25, 25 ), /* 350,150,400,200, 50, 50 */
    /*  2 */ QSize( 25, 25 ), /* 375,175,400,200, 25, 25 */
    /*  3 */ QSize( 25, 25 ), /* 400,200,400,200,  0,  0 */
    /*  4 */ QSize( 25, 25 ), /* 400,200,425,225, 25, 25 */
    /*  5 */ QSize( 25, 25 ), /* 400,200,450,250, 50, 50 */
    /*  6 */ QSize( 25, 25 ), /* 400,200,475,275, 75, 75 */
    /*  7 */ QSize( 25, 25 )  /* 400,200,500,300,100,100 */
};

static const int ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopLeft_TestStepCount = sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtTopLeftTestSteps)/sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtTopLeftTestSteps[0]);

// Resize_Polyline1_MoveBoundingRectPtTopRight
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtTopRightStart(500,200);

static const QSize s_arsizePolylinesResizePolyline1MoveBoundingRectPtTopRightTestSteps[] = {
                               /* RectCoors (l,t,r,b,w,h) */
    /*  0 */ QSize( -25, 25 ), /* 400,200,475,300, 75, 75 */
    /*  1 */ QSize( -25, 25 ), /* 400,225,450,300, 50, 50 */
    /*  2 */ QSize( -25, 25 ), /* 400,250,425,300, 25, 25 */
    /*  3 */ QSize( -25, 25 ), /* 400,275,400,300,  0,  0 */
    /*  4 */ QSize( -25, 25 ), /* 375,300,400,325, 25, 25 */
    /*  5 */ QSize( -25, 25 ), /* 350,300,400,350, 50, 50 */
    /*  6 */ QSize( -25, 25 ), /* 325,300,400,375, 75, 75 */
    /*  7 */ QSize( -25, 25 )  /* 300,300,400,400,100,100 */
};

static const int ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopRight_TestStepCount = sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtTopRightTestSteps)/sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtTopRightTestSteps[0]);

// Resize_Polyline1_MoveBoundingRectPtBottomLeft
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtBottomLeftStart(300,400);

static const QSize s_arsizePolylinesResizePolyline1MoveBoundingRectPtBottomLeftTestSteps[] = {
                               /* RectCoors (l,t,r,b,w,h) */
    /*  0 */ QSize( 25, -25 ), /* 325,300,400,375, 75, 75 */
    /*  1 */ QSize( 25, -25 ), /* 350,300,400,350, 50, 50 */
    /*  2 */ QSize( 25, -25 ), /* 375,300,400,325, 25, 25 */
    /*  3 */ QSize( 25, -25 ), /* 400,300,400,300,  0,  0 */
    /*  4 */ QSize( 25, -25 ), /* 400,275,425,300, 25, 25 */
    /*  5 */ QSize( 25, -25 ), /* 400,250,450,300, 50, 50 */
    /*  6 */ QSize( 25, -25 ), /* 400,225,475,300, 75, 75 */
    /*  7 */ QSize( 25, -25 )  /* 400,200,500,300,100,100 */
};

static const int ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomLeft_TestStepCount = sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtBottomLeftTestSteps)/sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtBottomLeftTestSteps[0]);

// Resize_Polyline1_MovePtBottomCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtBottomCenterStart(450,300);

static const QSize s_arsizePolylinesResizePolyline1MoveBoundingRectPtBottomCenterTestSteps[] = {
                              /* RectCoors (l,t,r,b,w,h) */
    /*  0 */ QSize( 0, -25 ), /* 400,200,500,275,100, 75 */
    /*  1 */ QSize( 0, -25 ), /* 400,200,500,250,100, 50 */
    /*  2 */ QSize( 0, -25 ), /* 400,200,500,225,100, 25 */
    /*  3 */ QSize( 0, -25 ), /* 400,200,500,200,100,  0 */
    /*  4 */ QSize( 0, -25 ), /* 400,175,500,200,100, 25 */
    /*  5 */ QSize( 0, -25 ), /* 400,150,500,200,100, 50 */
    /*  6 */ QSize( 0, -25 ), /* 400,125,500,200,100, 75 */
    /*  7 */ QSize( 0, -25 )  /* 400,100,500,200,100,100 */
};

static const int ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomCenter_TestStepCount = sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtBottomCenterTestSteps)/sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtBottomCenterTestSteps[0]);

// Resize_Polyline1_MovePtTopCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtTopCenterStart(450,100);

static const QSize s_arsizePolylinesResizePolyline1MoveBoundingRectPtTopCenterTestSteps[] = {
                              /* RectCoors (l,t,r,b,w,h) */
    /*  0 */ QSize( 0, 25 ), /* 400,125,500,200,100, 75 */
    /*  1 */ QSize( 0, 25 ), /* 400,150,500,200,100, 50 */
    /*  2 */ QSize( 0, 25 ), /* 400,175,500,200,100, 25 */
    /*  3 */ QSize( 0, 25 ), /* 400,200,500,200,100,  0 */
    /*  4 */ QSize( 0, 25 ), /* 400,200,500,225,100, 25 */
    /*  5 */ QSize( 0, 25 ), /* 400,200,500,250,100, 50 */
    /*  6 */ QSize( 0, 25 ), /* 400,200,500,275,100, 75 */
    /*  7 */ QSize( 0, 25 )  /* 400,200,500,300,100,100 */
};

static const int ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopCenter_TestStepCount = sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtTopCenterTestSteps)/sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtTopCenterTestSteps[0]);

// Resize_Polyline1_MovePtRightCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtRightCenterStart(500,250);

static const QSize s_arsizePolylinesResizePolyline1MoveBoundingRectPtRightCenterTestSteps[] = {
                              /* RectCoors (l,t,r,b,w,h) */
    /*  0 */ QSize( -25, 0 ), /* 400,200,475,300, 75,100 */
    /*  1 */ QSize( -25, 0 ), /* 400,200,450,300, 50,100 */
    /*  2 */ QSize( -25, 0 ), /* 400,200,425,300, 25,100 */
    /*  3 */ QSize( -25, 0 ), /* 400,200,400,300,  0,100 */
    /*  4 */ QSize( -25, 0 ), /* 375,200,400,300, 25,100 */
    /*  5 */ QSize( -25, 0 ), /* 350,200,400,300, 50,100 */
    /*  6 */ QSize( -25, 0 ), /* 325,200,400,300, 75,100 */
    /*  7 */ QSize( -25, 0 )  /* 300,200,400,300,100,100 */
};

static const int ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtRightCenter_TestStepCount = sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtRightCenterTestSteps)/sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtRightCenterTestSteps[0]);

// Resize_Polyline1_MovePtLeftCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtLeftCenterStart(300,250);

static const QSize s_arsizePolylinesResizePolyline1MoveBoundingRectPtLeftCenterTestSteps[] = {
                             /* RectCoors (l,t,r,b,w,h) */
    /*  0 */ QSize( 25, 0 ), /* 325,200,400,300, 75,100 */
    /*  1 */ QSize( 25, 0 ), /* 350,200,400,300, 50,100 */
    /*  2 */ QSize( 25, 0 ), /* 375,200,400,300, 25,100 */
    /*  3 */ QSize( 25, 0 ), /* 400,200,400,300,  0,100 */
    /*  4 */ QSize( 25, 0 ), /* 400,200,425,300, 25,100 */
    /*  5 */ QSize( 25, 0 ), /* 400,200,450,300, 50,100 */
    /*  6 */ QSize( 25, 0 ), /* 400,200,475,300, 75,100 */
    /*  7 */ QSize( 25, 0 )  /* 400,200,500,300,100,100 */
};

static const int ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtLeftCenter_TestStepCount = sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtLeftCenterTestSteps)/sizeof(s_arsizePolylinesResizePolyline1MoveBoundingRectPtLeftCenterTestSteps[0]);

// RotateTo_Polyline1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosRotateToPolyline1Select(420,250);

static const QPoint s_ptDrawingSceneMousePosRotateToPolyline1MoveSelPtRotateTopStart(450,180);

static const double s_arfPolylinesRotateToPolyline1MoveSelPtRotateTopTestStepsAngles_degree[] = {
                   /* RectCoors (l,t,r,b,w,h) */
    /*  0 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /*  1 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /*  2 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /*  3 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /*  4 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /*  5 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /*  6 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /*  7 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /*  8 */ 10.0, /* 400,200,500,300,100,100 */ //  90°
    /*  9 */ 30.0, /*    ,   ,   ,   ,100,100 */
    /* 10 */ 30.0, /*    ,   ,   ,   ,100,100 */
    /* 11 */ 30.0, /* 400,200,500,300,100,100 */ // 180°
    /* 12 */ 45.0, /*    ,   ,   ,   ,100,100 */
    /* 13 */ 45.0, /* 400,200,500,300,100,100 */ // 270°
    /* 14 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /* 15 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /* 16 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /* 17 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /* 18 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /* 19 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /* 20 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /* 21 */ 10.0, /*    ,   ,   ,   ,100,100 */
    /* 22 */ 10.0  /* 400,200,500,300,100,100 */ // 360°
};

static const int ETestStepLev3_Polylines_RotateTo_Polyline1_MoveSelPtRotateTop_TestStepCount = sizeof(s_arfPolylinesRotateToPolyline1MoveSelPtRotateTopTestStepsAngles_degree)/sizeof(s_arfPolylinesRotateToPolyline1MoveSelPtRotateTopTestStepsAngles_degree[0]);

// RotateMoveResize_Polyline1
//------------------------------------------------------------------------------

struct SRotateMoveResizeOperation
{
    QPoint          m_ptDrawingSceneMousePos;
    QPoint          m_ptDrawingSceneRectCenterPos;
    int             m_iMouseEvent;
    EEditMode       m_operation;
    int             m_idxSelPtSelectedPolygon;
    ESelectionPoint m_selPtSelectedBoundingRect;
    QPointF         m_pt0PolygonResult;
    QPointF         m_pt1PolygonResult;
    QPointF         m_pt2PolygonResult;
    bool            m_bIgnoreTestResult;
};

#define ESPTL ESelectionPointTopLeft
#define ESPTR ESelectionPointTopRight
#define ESPBR ESelectionPointBottomRight
#define ESPBL ESelectionPointBottomLeft
#define ESPTC ESelectionPointTopCenter
#define ESPRC ESelectionPointRightCenter
#define ESPBC ESelectionPointBottomCenter
#define ESPLC ESelectionPointLeftCenter
#define ESPC  ESelectionPointCenter
#define ESPOT ESelectionPointRotateTop
#define ESPOB ESelectionPointRotateBottom
#define ESPUd ESelectionPointUndefined

static const SRotateMoveResizeOperation s_arRotateMoveResizePolyline1TestSteps[] = {
          // {        MousePos,       RectCenterPos,               MouseEvent,   Operation, idxSelPt, rctSelPt,  PolygonResult[0],     PolygonResult[1],     PolygonResult[2], Ignore }
    /*  0 */ { QPoint(421,251), QPoint(  0.0,  0.0), QEvent::MouseButtonPress,   EEditModeSelect, -1, ESPUd, QPointF(400.0,200.0), QPointF(440.0,300.0), QPointF(500.0,260.0), false  },
    /*  1 */ { QPoint(421,251), QPoint(  0.0,  0.0), QEvent::MouseButtonRelease, EEditModeSelect, -1, ESPUd, QPointF(400.0,200.0), QPointF(440.0,300.0), QPointF(500.0,260.0), false  },
    /*  2 */ { QPoint(450,180), QPoint(  0.0,  0.0), QEvent::MouseButtonPress,   EEditModeSelect, -1, ESPOT, QPointF(400.0,200.0), QPointF(440.0,300.0), QPointF(500.0,260.0), false  },
    /*  3 */ { QPoint(380,250), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeRotate, -1, ESPUd, QPointF(400.0,300.0), QPointF(500.0,260.0), QPointF(460.0,200.0), false  },  //  90°
    /*  4 */ { QPoint(380,250), QPoint(  0.0,  0.0), QEvent::MouseButtonRelease, EEditModeSelect, -1, ESPUd, QPointF(400.0,300.0), QPointF(500.0,260.0), QPointF(460.0,200.0), false  },
    /*  5 */ { QPoint(450,250), QPoint(  0.0,  0.0), QEvent::MouseButtonPress,   EEditModeSelect, -1, ESPC,  QPointF(400.0,300.0), QPointF(500.0,260.0), QPointF(460.0,200.0), false  },
    /*  6 */ { QPoint(550,350), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeMove,   -1, ESPUd, QPointF(500.0,400.0), QPointF(600.0,360.0), QPointF(560.0,300.0), false  },
    /*  7 */ { QPoint(550,350), QPoint(  0.0,  0.0), QEvent::MouseButtonRelease, EEditModeSelect, -1, ESPUd, QPointF(500.0,400.0), QPointF(600.0,360.0), QPointF(560.0,300.0), false  },
    /*  8 */ { QPoint(500,350), QPoint(  0.0,  0.0), QEvent::MouseButtonPress,   EEditModeSelect, -1, ESPTC, QPointF(500.0,400.0), QPointF(600.0,360.0), QPointF(560.0,300.0), false  },
    /*  9 */ { QPoint(510,350), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(510.0,400.0), QPointF(600.0,360.0), QPointF(564.0,300.0), false  },
    /* 10 */ { QPoint(510,350), QPoint(  0.0,  0.0), QEvent::MouseButtonRelease, EEditModeSelect, -1, ESPUd, QPointF(510.0,400.0), QPointF(600.0,360.0), QPointF(564.0,300.0), false  },
    /* 11 */ { QPoint(490,350), QPoint(  0.0,  0.0), QEvent::MouseButtonPress,   EEditModeSelect, -1, ESPOT, QPointF(510.0,400.0), QPointF(600.0,360.0), QPointF(564.0,300.0), false  },
    /* 12 */ { QPoint(555,280), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeRotate, -1, ESPUd, QPointF(505.0,305.0), QPointF(545.0,395.0), QPointF(605.0,359.0), false  },  //  0°
    /* 13 */ { QPoint(555,280), QPoint(  0.0,  0.0), QEvent::MouseButtonRelease, EEditModeSelect, -1, ESPUd, QPointF(505.0,305.0), QPointF(545.0,395.0), QPointF(605.0,359.0), false  },
    /* 14 */ { QPoint(555,305), QPoint(  0.0,  0.0), QEvent::MouseButtonPress,   EEditModeSelect, -1, ESPTC, QPointF(505.0,305.0), QPointF(545.0,395.0), QPointF(605.0,359.0), false  },
    /* 15 */ { QPoint(555,300), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(505.0,300.0), QPointF(545.0,395.0), QPointF(605.0,357.0), false  },
    /* 16 */ { QPoint(555,300), QPoint(  0.0,  0.0), QEvent::MouseButtonRelease, EEditModeSelect, -1, ESPUd, QPointF(505.0,300.0), QPointF(545.0,395.0), QPointF(605.0,357.0), false  },
    /* 17 */ { QPoint(555,395), QPoint(  0.0,  0.0), QEvent::MouseButtonPress,   EEditModeSelect, -1, ESPBC, QPointF(505.0,300.0), QPointF(545.0,395.0), QPointF(605.0,357.0), false  },
    /* 18 */ { QPoint(555,400), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(505.0,300.0), QPointF(545.0,400.0), QPointF(605.0,360.0), false  },
    /* 19 */ { QPoint(555,400), QPoint(  0.0,  0.0), QEvent::MouseButtonRelease, EEditModeSelect, -1, ESPUd, QPointF(505.0,300.0), QPointF(545.0,400.0), QPointF(605.0,360.0), false  },
    /* 20 */ { QPoint(555,350), QPoint(  0.0,  0.0), QEvent::MouseButtonPress,   EEditModeSelect, -1, ESPC,  QPointF(505.0,300.0), QPointF(545.0,400.0), QPointF(605.0,360.0), false  },
    /* 21 */ { QPoint(550,350), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeMove,   -1, ESPUd, QPointF(500.0,300.0), QPointF(540.0,400.0), QPointF(600.0,360.0), false  },
    /* 22 */ { QPoint(550,350), QPoint(  0.0,  0.0), QEvent::MouseButtonRelease, EEditModeSelect, -1, ESPUd, QPointF(500.0,300.0), QPointF(540.0,400.0), QPointF(600.0,360.0), false  },
    /* 23 */ { QPoint(550,280), QPoint(  0.0,  0.0), QEvent::MouseButtonPress,   EEditModeSelect, -1, ESPOT, QPointF(500.0,300.0), QPointF(540.0,400.0), QPointF(600.0,360.0), false  },
    /* 24 */ { QPoint(480,280), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeRotate, -1, ESPUd, QPointF(479.3,350.0), QPointF(578.3,392.4), QPointF(592.4,321.7), false  },  //  45°
    /* 25 */ { QPoint(480,280), QPoint(  0.0,  0.0), QEvent::MouseButtonRelease, EEditModeSelect, -1, ESPUd, QPointF(479.3,350.0), QPointF(578.3,392.4), QPointF(592.4,321.7), false  },
    /* 26 */ { QPoint(620,350), QPoint(535.9,357.1), QEvent::MouseButtonPress,   EEditModeSelect, -1, ESPBR, QPointF(479.3,350.0), QPointF(578.3,392.4), QPointF(592.4,321.7), false  },
    /* 27 */ { QPoint(610,360), QPoint(530.6,364.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(479.3,350.0), QPointF(573.8,396.2), QPointF(581.9,331.9), false  },
    /* 28 */ { QPoint(600,370), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(479.3,350.0), QPointF(569.8,400.2), QPointF(571.9,341.9), false  },
    /* 29 */ { QPoint(590,380), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(479.3,350.0), QPointF(565.8,404.2), QPointF(561.9,351.9), false  },
    /* 30 */ { QPoint(580,390), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(479.3,350.0), QPointF(561.8,408.2), QPointF(551.9,361.9), false  },
    /* 31 */ { QPoint(570,400), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(479.3,350.0), QPointF(557.8,412.2), QPointF(541.9,371.9), false  },
    /* 32 */ { QPoint(560,410), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(479.3,350.0), QPointF(553.8,416.2), QPointF(531.9,381.9), false  },
    /* 33 */ { QPoint(550,420), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(479.3,350.0), QPointF(549.8,420.2), QPointF(521.9,391.9), false  },
    /* 34 */ { QPoint(540,430), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(469.6,359.6), QPointF(543.9,426.1), QPointF(521.5,392.2), false  },
    /* 35 */ { QPoint(530,440), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(459.6,369.6), QPointF(537.9,432.1), QPointF(521.5,392.2), false  },
    /* 36 */ { QPoint(520,450), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(449.6,379.6), QPointF(531.9,438.1), QPointF(521.5,392.2), false  },
    /* 37 */ { QPoint(510,460), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(439.6,389.6), QPointF(525.9,444.1), QPointF(521.5,392.2), false  },
    /* 38 */ { QPoint(500,470), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(429.6,399.6), QPointF(519.9,450.1), QPointF(521.5,392.2), false  },
    /* 39 */ { QPoint(490,480), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(419.6,409.6), QPointF(513.9,456.1), QPointF(521.5,392.2), false  },
    /* 40 */ { QPoint(480,490), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeResize, -1, ESPUd, QPointF(409.6,419.6), QPointF(507.9,462.1), QPointF(521.5,392.2), false  },
    /* 41 */ { QPoint(480,490), QPoint(  0.0,  0.0), QEvent::MouseButtonRelease, EEditModeSelect, -1, ESPUd, QPointF(409.6,419.6), QPointF(507.9,462.1), QPointF(521.5,392.2), false  },
    /* 42 */ { QPoint(480,420), QPoint(  0.0,  0.0), QEvent::MouseButtonPress,   EEditModeSelect, -1, ESPC,  QPointF(409.6,419.6), QPointF(507.9,462.1), QPointF(521.5,392.2), false  },
    /* 43 */ { QPoint(550,350), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeMove,   -1, ESPUd, QPointF(479.6,349.6), QPointF(577.9,392.1), QPointF(591.5,322.2), false  },
    /* 44 */ { QPoint(550,350), QPoint(  0.0,  0.0), QEvent::MouseButtonRelease, EEditModeSelect, -1, ESPUd, QPointF(479.6,349.6), QPointF(577.9,392.1), QPointF(591.5,322.2), false  },
    /* 45 */ { QPoint(501,301), QPoint(  0.0,  0.0), QEvent::MouseButtonPress,   EEditModeSelect, -1, ESPOT, QPointF(479.6,349.6), QPointF(577.9,392.1), QPointF(591.5,322.2), false  },
    /* 46 */ { QPoint(550,280), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeRotate, -1, ESPUd, QPointF(500.4,300.3), QPointF(539.8,399.7), QPointF(598.9,359.9), false  },
    /* 47 */ { QPoint(550,280), QPoint(  0.0,  0.0), QEvent::MouseButtonRelease, EEditModeSelect, -1, ESPUd, QPointF(500.4,300.3), QPointF(539.8,399.7), QPointF(598.9,359.9), false  },
    /* 48 */ { QPoint(550,350), QPoint(  0.0,  0.0), QEvent::MouseButtonPress,   EEditModeSelect, -1, ESPC,  QPointF(500.4,300.3), QPointF(539.8,399.7), QPointF(598.9,359.9), false  },
    /* 49 */ { QPoint(450,250), QPoint(  0.0,  0.0), QEvent::MouseMove,          EEditModeMove,   -1, ESPUd, QPointF(400.4,200.3), QPointF(439.8,299.7), QPointF(498.9,259.9), false  },
    /* 50 */ { QPoint(450,250), QPoint(  0.0,  0.0), QEvent::MouseButtonRelease, EEditModeSelect, -1, ESPUd, QPointF(400.4,200.3), QPointF(439.8,299.7), QPointF(498.9,259.9), false  }
};

static const int ETestStepLev3_Polylines_RotateMoveResize_Polyline1_TestStepCount = sizeof(s_arRotateMoveResizePolyline1TestSteps)/sizeof(s_arRotateMoveResizePolyline1TestSteps[0]);


// Test steps:
//------------------------------------------------------------------------------

enum
{
    ETestStepLev3_Polylines_Create_Polyline1_ActivateDrawingTool,
    ETestStepLev3_Polylines_Create_Polyline1_FirstLineSegment,
    ETestStepLev3_Polylines_Create_Polyline1_FirstLineSegment_MovePt2_TestStepMin,
    ETestStepLev3_Polylines_Create_Polyline1_FirstLineSegment_MovePt2_TestStepMax = ETestStepLev3_Polylines_Create_Polyline1_FirstLineSegment_MovePt2_TestStepMin + ETestStepLev3_Polylines_Create_Polyline1_FirstLineSegment_MovePt2_TestStepCount - 1,
    ETestStepLev3_Polylines_Create_Polyline1_SecondLineSegment,
    ETestStepLev3_Polylines_Create_Polyline1_SecondLineSegment_MovePt3_TestStepMin,
    ETestStepLev3_Polylines_Create_Polyline1_SecondLineSegment_MovePt3_TestStepMax = ETestStepLev3_Polylines_Create_Polyline1_SecondLineSegment_MovePt3_TestStepMin + ETestStepLev3_Polylines_Create_Polyline1_SecondLineSegment_MovePt3_TestStepCount - 1,
    ETestStepLev3_Polylines_Create_Polyline1_Finished,
    ETestStepLev3_Polylines_Create_Polyline2_ActivateDrawingTool,
    ETestStepLev3_Polylines_Create_Polyline2_FirstLineSegment,
    ETestStepLev3_Polylines_Create_Polyline2_FirstLineSegment_MovePt2_TestStepMin,
    ETestStepLev3_Polylines_Create_Polyline2_FirstLineSegment_MovePt2_TestStepMax = ETestStepLev3_Polylines_Create_Polyline2_FirstLineSegment_MovePt2_TestStepMin + ETestStepLev3_Polylines_Create_Polyline2_FirstLineSegment_MovePt2_TestStepCount - 1,
    ETestStepLev3_Polylines_Create_Polyline2_SecondLineSegment,
    ETestStepLev3_Polylines_Create_Polyline2_SecondLineSegment_MovePt3_TestStepMin,
    ETestStepLev3_Polylines_Create_Polyline2_SecondLineSegment_MovePt3_TestStepMax = ETestStepLev3_Polylines_Create_Polyline2_SecondLineSegment_MovePt3_TestStepMin + ETestStepLev3_Polylines_Create_Polyline2_SecondLineSegment_MovePt3_TestStepCount - 1,
    ETestStepLev3_Polylines_Create_Polyline2_Finished,
    ETestStepLev3_Polylines_Move_Polyline1_ActivateEditModeSelect,
    ETestStepLev3_Polylines_Move_Polyline1_Select,
    ETestStepLev3_Polylines_Move_Polyline1_TestStepMin,
    ETestStepLev3_Polylines_Move_Polyline1_TestStepMax = ETestStepLev3_Polylines_Move_Polyline1_TestStepMin + ETestStepLev3_Polylines_Move_Polyline1_TestStepCount - 1,
    ETestStepLev3_Polylines_Move_Polyline1_Release,
    ETestStepLev3_Polylines_Move_Polyline1_Finished,
    ETestStepLev3_Polylines_Resize_Polyline1_ActivateEditModeSelect,
    ETestStepLev3_Polylines_Resize_Polyline1_Select,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomRight_Select,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomRight_TestStepMin,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomRight_TestStepMax = ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomRight_TestStepMin + ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomRight_TestStepCount - 1,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomRight_Release,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopLeft_Select,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopLeft_TestStepMin,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopLeft_TestStepMax = ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopLeft_TestStepMin + ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopLeft_TestStepCount - 1,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopLeft_Release,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopRight_Select,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopRight_TestStepMin,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopRight_TestStepMax = ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopRight_TestStepMin + ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopRight_TestStepCount - 1,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopRight_Release,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomLeft_Select,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomLeft_TestStepMin,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomLeft_TestStepMax = ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomLeft_TestStepMin + ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomLeft_TestStepCount - 1,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomLeft_Release,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomCenter_Select,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomCenter_TestStepMin,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomCenter_TestStepMax = ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomCenter_TestStepMin + ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomCenter_TestStepCount - 1,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomCenter_Release,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopCenter_Select,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopCenter_TestStepMin,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopCenter_TestStepMax = ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopCenter_TestStepMin + ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopCenter_TestStepCount - 1,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopCenter_Release,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtRightCenter_Select,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtRightCenter_TestStepMin,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtRightCenter_TestStepMax = ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtRightCenter_TestStepMin + ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtRightCenter_TestStepCount - 1,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtRightCenter_Release,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtLeftCenter_Select,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtLeftCenter_TestStepMin,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtLeftCenter_TestStepMax = ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtLeftCenter_TestStepMin + ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtLeftCenter_TestStepCount - 1,
    ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtLeftCenter_Release,
    ETestStepLev3_Polylines_Resize_Polyline1_Finished,
    ETestStepLev3_Polylines_RotateTo_Polyline1_ActivateEditModeSelect,
    ETestStepLev3_Polylines_RotateTo_Polyline1_Select,
    ETestStepLev3_Polylines_RotateTo_Polyline1_MoveSelPtRotateTop_Select,
    ETestStepLev3_Polylines_RotateTo_Polyline1_MoveSelPtRotateTop_TestStepMin,
    ETestStepLev3_Polylines_RotateTo_Polyline1_MoveSelPtRotateTop_TestStepMax = ETestStepLev3_Polylines_RotateTo_Polyline1_MoveSelPtRotateTop_TestStepMin + ETestStepLev3_Polylines_RotateTo_Polyline1_MoveSelPtRotateTop_TestStepCount - 1,
    ETestStepLev3_Polylines_RotateTo_Polyline1_Release,
    ETestStepLev3_Polylines_RotateTo_Polyline1_Finished,
    ETestStepLev3_Polylines_RotateMoveResize_Polyline1_ActivateEditModeSelect,
    ETestStepLev3_Polylines_RotateMoveResize_Polyline1_Start,
    ETestStepLev3_Polylines_RotateMoveResize_Polyline1_TestStepMin,
    ETestStepLev3_Polylines_RotateMoveResize_Polyline1_TestStepMax = ETestStepLev3_Polylines_RotateMoveResize_Polyline1_TestStepMin + ETestStepLev3_Polylines_RotateMoveResize_Polyline1_TestStepCount - 1,
    ETestStepLev3_Polylines_RotateMoveResize_Polyline1_Finished,
    ETestStepLev3_Polylines_Finished,
    ETestStepLev3_Polylines_Count
};

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepStandardShapesPolylines( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
//------------------------------------------------------------------------------
{
    QMouseEvent*         pMouseEv = nullptr;
    QPoint               ptMousePosGlobal;
    QString              strCurrentDrawingTool;
    QString              strCurrentGraphObj;
    QString              strToolTip;
    ZS::Test::CTestStep* pTestStep;
    QStringList          strlstExpectedValues;
    QStringList          strlstResultValues;
    int                  idxPt;
    QPointF              pt;

    ZS::Test::CTestStepGroup* pTSGrpPolylines                 = getTestStepGroup( "Polylines", i_pTestStepGrpParent );
    ZS::Test::CTestStepGroup* pTSGrpPolylinesCreate           = getTestStepGroup( "Create", pTSGrpPolylines );
    ZS::Test::CTestStepGroup* pTSGrpPolylinesMove             = getTestStepGroup( "Move", pTSGrpPolylines );
    ZS::Test::CTestStepGroup* pTSGrpPolylinesResize           = getTestStepGroup( "Resize", pTSGrpPolylines );
    ZS::Test::CTestStepGroup* pTSGrpPolylinesRotateTo         = getTestStepGroup( "RotateTo", pTSGrpPolylines );
    ZS::Test::CTestStepGroup* pTSGrpPolylinesRotateMoveResize = getTestStepGroup( "RotateMoveResize", pTSGrpPolylines );

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }

    if( !pTSGrpPolylines->isEnabled() )
    {
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else if( !pTSGrpPolylinesCreate->isEnabled() )
    {
        // If the polyline is not created none of the other sub tests may be performed.
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else // if( pTSGrpPolylines->isEnabled() )
    {
        if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline1_ActivateDrawingTool )
        {
            pTestStep = getTestStep( "ActivateDrawingTool", pTSGrpPolylinesCreate );

            strlstExpectedValues.append( "EditTool: CreateObjects" );
            strlstExpectedValues.append( "DrawingTool: Polyline" );

            pTestStep->setOperation( "drawing.setCurrentDrawingTool( Polyline )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            if( (m_pDrawingScene->getEditTool() != EEditToolCreateObjects) || (m_pDrawingScene->getCurrentDrawingToolGraphObjType() != EGraphObjTypePolyline) )
            {
                m_pMainWindow->setCheckedActionDrawStandardShapePolyline(true);
            }

            if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
            {
                strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
            }
            else
            {
                strCurrentDrawingTool = "";
            }

            strlstResultValues.append( "EditTool: " + editTool2Str(m_pDrawingScene->getEditTool()) );
            strlstResultValues.append( "DrawingTool: " + strCurrentDrawingTool );

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline1_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline1_FirstLineSegment )
        {
            m_iPolylinesCount++;

            m_iTestStepLev4 = -1;

            s_strGraphObjNamePolyline1 = "Polyline" + QString::number(m_iPolylinesCount);

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "FirstLineSegment", pTSGrpPolylinesCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosPolyline1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plg1Prev.clear();
            s_rctBoundingPlg1Prev = QRectF();

            s_plg1Curr.clear();
            s_plg1Curr.append(m_ptDrawingSceneMousePos);
            s_plg1Curr.append(m_ptDrawingSceneMousePos);
            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".create( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            m_pGraphObjCreating = m_pDrawingScene->getGraphObjCreating();

            if( m_pGraphObjCreating != nullptr )
            {
                strCurrentGraphObj = m_pGraphObjCreating->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline1_FirstLineSegment )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Create_Polyline1_FirstLineSegment_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Create_Polyline1_FirstLineSegment_MovePt2_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "FirstLineSegmentMovePt2Step" + QString::number(m_iTestStepLev4), pTSGrpPolylinesCreate );

            m_fxOffsHor = s_arsizePolylinesCreatePolyline1FirstLineSegmentMovePt2TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolylinesCreatePolyline1FirstLineSegmentMovePt2TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plg1Curr[1] = m_ptDrawingSceneMousePos;
            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "drawing.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingObject( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
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
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Create_Polyline1_FirstLineSegment_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Create_Polyline1_FirstLineSegment_MovePt2_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline1_SecondLineSegment )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "SecondLineSegment", pTSGrpPolylinesCreate );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plg1Curr[1] = m_ptDrawingSceneMousePos;
            s_plg1Curr.append(m_ptDrawingSceneMousePos);
            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingPathSegmentFinished( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            m_pGraphObjCreating = m_pDrawingScene->getGraphObjCreating();

            if( m_pGraphObjCreating != nullptr )
            {
                strCurrentGraphObj = m_pGraphObjCreating->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline1_SecondLineSegment )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Create_Polyline1_SecondLineSegment_MovePt3_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Create_Polyline1_SecondLineSegment_MovePt3_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "SecondLineSegmentMovePt3Step" + QString::number(m_iTestStepLev4), pTSGrpPolylinesCreate );

            m_fxOffsHor = s_arsizePolylinesCreatePolyline1SecondLineSegmentMovePt3TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolylinesCreatePolyline1SecondLineSegmentMovePt3TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plg1Curr[2] = m_ptDrawingSceneMousePos;
            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingObject( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
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
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Create_Polyline1_SecondLineSegment_MovePt3_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Create_Polyline1_SecondLineSegment_MovePt3_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline1_Finished )
        {
            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "Finish", pTSGrpPolylinesCreate );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plg1Curr[2] = m_ptDrawingSceneMousePos;
            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            pTestStep->setOperation( "view.mouseDoubleClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingObjectFinished( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            m_pGraphObjCreating = nullptr;

            m_pGraphObjCreating = m_pDrawingScene->getGraphObjCreating();

            if( m_pGraphObjCreating != nullptr )
            {
                strCurrentGraphObj = m_pGraphObjCreating->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            pTSGrpPolylinesCreate->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline2_ActivateDrawingTool )
        {
            pTestStep = getTestStep( "ActivateDrawingTool", pTSGrpPolylinesCreate );

            strlstExpectedValues.append( "EditTool: CreateObjects" );
            strlstExpectedValues.append( "DrawingTool: Polyline" );

            pTestStep->setOperation( "drawing.setCurrentDrawingTool( Polyline )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            if( (m_pDrawingScene->getEditTool() != EEditToolCreateObjects) || (m_pDrawingScene->getCurrentDrawingToolGraphObjType() != EGraphObjTypePolyline) )
            {
                m_pMainWindow->setCheckedActionDrawStandardShapePolyline(true);
            }

            if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
            {
                strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
            }
            else
            {
                strCurrentDrawingTool = "";
            }

            strlstResultValues.append( "EditTool: " + editTool2Str(m_pDrawingScene->getEditTool()) );
            strlstResultValues.append( "DrawingTool: " + strCurrentDrawingTool );

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline2_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline2_FirstLineSegment )
        {
            m_iPolylinesCount++;

            m_iTestStepLev4 = -1;

            s_strGraphObjNamePolyline2 = "Polyline" + QString::number(m_iPolylinesCount);

            strCurrentGraphObj = s_strGraphObjNamePolyline2;

            pTestStep = getTestStep( strCurrentGraphObj + "FirstLineSegment", pTSGrpPolylinesCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosPolyline2Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plg2Prev.clear();
            s_rctBoundingPlg2Prev = QRectF();

            s_plg2Curr.clear();
            s_plg2Curr.append(m_ptDrawingSceneMousePos);
            s_plg2Curr.append(m_ptDrawingSceneMousePos);
            s_rctBoundingPlg2Curr = s_plg2Curr.boundingRect();

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg2Curr) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".create( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            m_pGraphObjCreating = m_pDrawingScene->getGraphObjCreating();

            if( m_pGraphObjCreating != nullptr )
            {
                strCurrentGraphObj = m_pGraphObjCreating->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline2_FirstLineSegment )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Create_Polyline2_FirstLineSegment_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Create_Polyline2_FirstLineSegment_MovePt2_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGraphObjNamePolyline2;

            pTestStep = getTestStep( strCurrentGraphObj + "FirstLineSegmentMovePt2Step" + QString::number(m_iTestStepLev4), pTSGrpPolylinesCreate );

            m_fxOffsHor = s_arsizePolylinesCreatePolyline2FirstLineSegmentMovePt2TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolylinesCreatePolyline2FirstLineSegmentMovePt2TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plg2Curr[1] = m_ptDrawingSceneMousePos;
            s_rctBoundingPlg2Curr = s_plg2Curr.boundingRect();

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg2Curr) );

            pTestStep->setOperation( "drawing.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingObject( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
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
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Create_Polyline2_FirstLineSegment_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Create_Polyline2_FirstLineSegment_MovePt2_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline2_SecondLineSegment )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGraphObjNamePolyline2;

            pTestStep = getTestStep( strCurrentGraphObj + "SecondLineSegment", pTSGrpPolylinesCreate );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plg2Curr[1] = m_ptDrawingSceneMousePos;
            s_plg2Curr.append(m_ptDrawingSceneMousePos);
            s_rctBoundingPlg2Curr = s_plg2Curr.boundingRect();

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg2Curr) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingPathSegmentFinished( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            m_pGraphObjCreating = m_pDrawingScene->getGraphObjCreating();

            if( m_pGraphObjCreating != nullptr )
            {
                strCurrentGraphObj = m_pGraphObjCreating->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline2_SecondLineSegment )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Create_Polyline2_SecondLineSegment_MovePt3_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Create_Polyline2_SecondLineSegment_MovePt3_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGraphObjNamePolyline2;

            pTestStep = getTestStep( strCurrentGraphObj + "SecondLineSegmentMovePt3Step" + QString::number(m_iTestStepLev4), pTSGrpPolylinesCreate );

            m_fxOffsHor = s_arsizePolylinesCreatePolyline2SecondLineSegmentMovePt3TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolylinesCreatePolyline2SecondLineSegmentMovePt3TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plg2Curr[2] = m_ptDrawingSceneMousePos;
            s_rctBoundingPlg2Curr = s_plg2Curr.boundingRect();

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg2Curr) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingObject( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
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
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Create_Polyline2_SecondLineSegment_MovePt3_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Create_Polyline2_SecondLineSegment_MovePt3_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline2_Finished )
        {
            strCurrentGraphObj = s_strGraphObjNamePolyline2;

            pTestStep = getTestStep( strCurrentGraphObj + "Finish", pTSGrpPolylinesCreate );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plg2Curr[2] = m_ptDrawingSceneMousePos;
            s_rctBoundingPlg2Curr = s_plg2Curr.boundingRect();

            pTestStep->setOperation( "view.mouseDoubleClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingObjectFinished( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            m_pGraphObjCreating = nullptr;

            m_pGraphObjCreating = m_pDrawingScene->getGraphObjCreating();

            if( m_pGraphObjCreating != nullptr )
            {
                strCurrentGraphObj = m_pGraphObjCreating->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            pTSGrpPolylinesCreate->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Create_Polyline2_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Move_Polyline1_ActivateEditModeSelect )
        {
            if( !pTSGrpPolylinesMove->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Polylines_Move_Polyline1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpPolylinesMove->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpPolylinesMove );

                strlstExpectedValues.append( "EditTool: Select" );
                strlstExpectedValues.append( "DrawingTool: " );

                pTestStep->setOperation( "drawing.setCurrentDrawingTool( Select )" );
                pTestStep->setExpectedValues(strlstExpectedValues);
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

                strlstResultValues.append( "EditTool: " + editTool2Str(m_pDrawingScene->getEditTool()) );
                strlstResultValues.append( "DrawingTool: " + strCurrentDrawingTool );

                pTestStep->setResultValues(strlstResultValues);
                pTestStep->setToolTip(strToolTip);
                pTestStep->testEnded();

                m_iTestStepLev3++;  // Next sub test step
                triggerDoTestStep();

            } // if( pTSGrpPolylinesMove->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Move_Polyline1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Move_Polyline1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "Select", pTSGrpPolylinesMove );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosMovePolyline1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plg1Prev = s_plg1Curr;
            s_rctBoundingPlg1Prev = s_rctBoundingPlg1Curr;

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Move_Polyline1_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Move_Polyline1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Move_Polyline1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "Step" + QString::number(m_iTestStepLev4), pTSGrpPolylinesMove );

            m_fxOffsHor = s_arsizePolylinesMovePolyline1Steps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolylinesMovePolyline1Steps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            for( idxPt = 0; idxPt < s_plg1Curr.size(); idxPt++ )
            {
                pt = s_plg1Curr[idxPt];
                pt.setX( pt.x() + m_fxOffsHor );
                pt.setY( pt.y() + m_fyOffsHor );
                s_plg1Curr[idxPt] = pt;
            }
            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "drawing.mouseMoveEvent( " + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".moveTo( " + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Move_Polyline1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Move_Polyline1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Move_Polyline1_Release )
        {
            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "Release", pTSGrpPolylinesMove );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Move_Polyline1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Move_Polyline1_Finished )
        {
            pTSGrpPolylinesMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Move_Polyline1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_ActivateEditModeSelect )
        {
            if( !pTSGrpPolylinesResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Polylines_Resize_Polyline1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpPolylinesResize->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpPolylinesResize );

                strlstExpectedValues.append( "EditTool: Select" );
                strlstExpectedValues.append( "DrawingTool: " );

                pTestStep->setOperation( "drawing.setCurrentDrawingTool( Select )" );
                pTestStep->setExpectedValues(strlstExpectedValues);
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

                strlstResultValues.append( "EditTool: " + editTool2Str(m_pDrawingScene->getEditTool()) );
                strlstResultValues.append( "DrawingTool: " + strCurrentDrawingTool );

                pTestStep->setResultValues(strlstResultValues);
                pTestStep->setToolTip(strToolTip);
                pTestStep->testEnded();

                m_iTestStepLev3++;  // Next sub test step
                triggerDoTestStep();

            } // if( pTSGrpPolylinesResize->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "Select", pTSGrpPolylinesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizePolyline1Select;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            m_pGraphObjSelected = nullptr;

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_Select )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomRight_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightSelect", pTSGrpPolylinesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtBottomRightStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            s_plg1Prev = s_plg1Curr;
            s_rctBoundingPlg1Prev = s_rctBoundingPlg1Curr;

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointBottomRight) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomRight_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomRight_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightStep" + QString::number(m_iTestStepLev4), pTSGrpPolylinesResize );

            m_fxOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtBottomRightTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtBottomRightTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr.setRight( s_rctBoundingPlg1Curr.right() + m_fxOffsHor );
            s_rctBoundingPlg1Curr.setBottom( s_rctBoundingPlg1Curr.bottom() + m_fyOffsHor );

            QRectF rct = s_rctBoundingPlg1Curr;

            if( fabs(rct.width()) > 0.0 || fabs(rct.height()) > 0.0 )
            {
                if( rct.width() < 0 )
                {
                    double fLeft = rct.left();
                    double fRight = rct.right();
                    rct.setLeft(fRight);
                    rct.setRight(fLeft);
                }
                if( rct.height() < 0 )
                {
                    double fTop = rct.top();
                    double fBottom = rct.bottom();
                    rct.setTop(fBottom);
                    rct.setBottom(fTop);
                }
            }

            s_plg1Curr = resizePolygon( s_plg1Prev, rct, s_rctBoundingPlg1Prev );

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( scenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomRight_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomRight_Release )
        {
            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightRelease", pTSGrpPolylinesResize );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomRight_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopLeft_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopLeftSelect", pTSGrpPolylinesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtTopLeftStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            s_plg1Prev = s_plg1Curr;
            s_rctBoundingPlg1Prev = s_rctBoundingPlg1Curr;

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointTopLeft) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::AltModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopLeft_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopLeft_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopLeftStep" + QString::number(m_iTestStepLev4), pTSGrpPolylinesResize );

            m_fxOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtTopLeftTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtTopLeftTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr.setLeft( s_rctBoundingPlg1Curr.left() + m_fxOffsHor );
            s_rctBoundingPlg1Curr.setTop( s_rctBoundingPlg1Curr.top() + m_fyOffsHor );

            QRectF rct = s_rctBoundingPlg1Curr;

            if( fabs(rct.width()) > 0.0 || fabs(rct.height()) > 0.0 )
            {
                if( rct.width() < 0 )
                {
                    double fLeft = rct.left();
                    double fRight = rct.right();
                    rct.setLeft(fRight);
                    rct.setRight(fLeft);
                }
                if( rct.height() < 0 )
                {
                    double fTop = rct.top();
                    double fBottom = rct.bottom();
                    rct.setTop(fBottom);
                    rct.setBottom(fTop);
                }
            }

            s_plg1Curr = resizePolygon( s_plg1Prev, rct, s_rctBoundingPlg1Prev );

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( scenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopLeft_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopLeft_Release )
        {
            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopLeftRelease", pTSGrpPolylinesResize );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopLeft_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopRight_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopRightSelect", pTSGrpPolylinesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtTopRightStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            s_plg1Prev = s_plg1Curr;
            s_rctBoundingPlg1Prev = s_rctBoundingPlg1Curr;

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointTopRight) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::AltModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopRight_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopRight_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopRightStep" + QString::number(m_iTestStepLev4), pTSGrpPolylinesResize );

            m_fxOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtTopRightTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtTopRightTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr.setRight( s_rctBoundingPlg1Curr.right() + m_fxOffsHor );
            s_rctBoundingPlg1Curr.setTop( s_rctBoundingPlg1Curr.top() + m_fyOffsHor );

            QRectF rct = s_rctBoundingPlg1Curr;

            if( fabs(rct.width()) > 0.0 || fabs(rct.height()) > 0.0 )
            {
                if( rct.width() < 0 )
                {
                    double fLeft = rct.left();
                    double fRight = rct.right();
                    rct.setLeft(fRight);
                    rct.setRight(fLeft);
                }
                if( rct.height() < 0 )
                {
                    double fTop = rct.top();
                    double fBottom = rct.bottom();
                    rct.setTop(fBottom);
                    rct.setBottom(fTop);
                }
            }

            s_plg1Curr = resizePolygon( s_plg1Prev, rct, s_rctBoundingPlg1Prev );

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( scenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopRight_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopRight_Release )
        {
            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopRightRelease", pTSGrpPolylinesResize );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopRight_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomLeft_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomLeftSelect", pTSGrpPolylinesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtBottomLeftStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            s_plg1Prev = s_plg1Curr;
            s_rctBoundingPlg1Prev = s_rctBoundingPlg1Curr;

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointBottomLeft) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::AltModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomLeft_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomLeft_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomLeftStep" + QString::number(m_iTestStepLev4), pTSGrpPolylinesResize );

            m_fxOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtBottomLeftTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtBottomLeftTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr.setLeft( s_rctBoundingPlg1Curr.left() + m_fxOffsHor );
            s_rctBoundingPlg1Curr.setBottom( s_rctBoundingPlg1Curr.bottom() + m_fyOffsHor );

            QRectF rct = s_rctBoundingPlg1Curr;

            if( fabs(rct.width()) > 0.0 || fabs(rct.height()) > 0.0 )
            {
                if( rct.width() < 0 )
                {
                    double fLeft = rct.left();
                    double fRight = rct.right();
                    rct.setLeft(fRight);
                    rct.setRight(fLeft);
                }
                if( rct.height() < 0 )
                {
                    double fTop = rct.top();
                    double fBottom = rct.bottom();
                    rct.setTop(fBottom);
                    rct.setBottom(fTop);
                }
            }

            s_plg1Curr = resizePolygon( s_plg1Prev, rct, s_rctBoundingPlg1Prev );

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( scenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomLeft_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomLeft_Release )
        {
            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomLeftRelease", pTSGrpPolylinesResize );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomLeft_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomCenterSelect", pTSGrpPolylinesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtBottomCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            s_plg1Prev = s_plg1Curr;
            s_rctBoundingPlg1Prev = s_rctBoundingPlg1Curr;

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointBottomCenter) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::AltModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomCenterStep" + QString::number(m_iTestStepLev4), pTSGrpPolylinesResize );

            m_fxOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtBottomCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtBottomCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr.setLeft( s_rctBoundingPlg1Curr.left() + m_fxOffsHor );
            s_rctBoundingPlg1Curr.setBottom( s_rctBoundingPlg1Curr.bottom() + m_fyOffsHor );

            QRectF rct = s_rctBoundingPlg1Curr;

            if( fabs(rct.width()) > 0.0 || fabs(rct.height()) > 0.0 )
            {
                if( rct.width() < 0 )
                {
                    double fLeft = rct.left();
                    double fRight = rct.right();
                    rct.setLeft(fRight);
                    rct.setRight(fLeft);
                }
                if( rct.height() < 0 )
                {
                    double fTop = rct.top();
                    double fBottom = rct.bottom();
                    rct.setTop(fBottom);
                    rct.setBottom(fTop);
                }
            }

            s_plg1Curr = resizePolygon( s_plg1Prev, rct, s_rctBoundingPlg1Prev );

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( scenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomCenter_Release )
        {
            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomCenterRelease", pTSGrpPolylinesResize );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtBottomCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopCenterSelect", pTSGrpPolylinesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtTopCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            s_plg1Prev = s_plg1Curr;
            s_rctBoundingPlg1Prev = s_rctBoundingPlg1Curr;

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointTopCenter) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::AltModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopCenterStep" + QString::number(m_iTestStepLev4), pTSGrpPolylinesResize );

            m_fxOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtTopCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtTopCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr.setLeft( s_rctBoundingPlg1Curr.left() + m_fxOffsHor );
            s_rctBoundingPlg1Curr.setTop( s_rctBoundingPlg1Curr.top() + m_fyOffsHor );

            QRectF rct = s_rctBoundingPlg1Curr;

            if( fabs(rct.width()) > 0.0 || fabs(rct.height()) > 0.0 )
            {
                if( rct.width() < 0 )
                {
                    double fLeft = rct.left();
                    double fRight = rct.right();
                    rct.setLeft(fRight);
                    rct.setRight(fLeft);
                }
                if( rct.height() < 0 )
                {
                    double fTop = rct.top();
                    double fBottom = rct.bottom();
                    rct.setTop(fBottom);
                    rct.setBottom(fTop);
                }
            }

            s_plg1Curr = resizePolygon( s_plg1Prev, rct, s_rctBoundingPlg1Prev );

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( scenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopCenter_Release )
        {
            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopCenterRelease", pTSGrpPolylinesResize );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtTopCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtRightCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtRightCenterSelect", pTSGrpPolylinesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtRightCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            s_plg1Prev = s_plg1Curr;
            s_rctBoundingPlg1Prev = s_rctBoundingPlg1Curr;

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointRightCenter) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::AltModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtRightCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtRightCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtRightCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtRightCenterStep" + QString::number(m_iTestStepLev4), pTSGrpPolylinesResize );

            m_fxOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtRightCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtRightCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr.setRight( s_rctBoundingPlg1Curr.right() + m_fxOffsHor );
            s_rctBoundingPlg1Curr.setBottom( s_rctBoundingPlg1Curr.bottom() + m_fyOffsHor );

            QRectF rct = s_rctBoundingPlg1Curr;

            if( fabs(rct.width()) > 0.0 || fabs(rct.height()) > 0.0 )
            {
                if( rct.width() < 0 )
                {
                    double fLeft = rct.left();
                    double fRight = rct.right();
                    rct.setLeft(fRight);
                    rct.setRight(fLeft);
                }
                if( rct.height() < 0 )
                {
                    double fTop = rct.top();
                    double fBottom = rct.bottom();
                    rct.setTop(fBottom);
                    rct.setBottom(fTop);
                }
            }

            s_plg1Curr = resizePolygon( s_plg1Prev, rct, s_rctBoundingPlg1Prev );

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( scenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtRightCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtRightCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtRightCenter_Release )
        {
            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtRightCenterRelease", pTSGrpPolylinesResize );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtRightCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtLeftCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtLeftCenterSelect", pTSGrpPolylinesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizePolyline1MoveBoundingRectPtLeftCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            s_plg1Prev = s_plg1Curr;
            s_rctBoundingPlg1Prev = s_rctBoundingPlg1Curr;

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointLeftCenter) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseButtonPress,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::AltModifier );
            m_pDrawingView->mousePressEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            m_pGraphObjSelected = nullptr;

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtLeftCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtLeftCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtLeftCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtLeftCenterStep" + QString::number(m_iTestStepLev4), pTSGrpPolylinesResize );

            m_fxOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtLeftCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolylinesResizePolyline1MoveBoundingRectPtLeftCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr.setLeft( s_rctBoundingPlg1Curr.left() + m_fxOffsHor );
            s_rctBoundingPlg1Curr.setBottom( s_rctBoundingPlg1Curr.bottom() + m_fyOffsHor );

            QRectF rct = s_rctBoundingPlg1Curr;

            if( fabs(rct.width()) > 0.0 || fabs(rct.height()) > 0.0 )
            {
                if( rct.width() < 0 )
                {
                    double fLeft = rct.left();
                    double fRight = rct.right();
                    rct.setLeft(fRight);
                    rct.setRight(fLeft);
                }
                if( rct.height() < 0 )
                {
                    double fTop = rct.top();
                    double fBottom = rct.bottom();
                    rct.setTop(fBottom);
                    rct.setBottom(fTop);
                }
            }

            s_plg1Curr = resizePolygon( s_plg1Prev, rct, s_rctBoundingPlg1Prev );

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( scenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtLeftCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtLeftCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtLeftCenter_Release )
        {
            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtLeftCenterRelease", pTSGrpPolylinesResize );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_MoveBoundingRectPtLeftCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateTo_Polyline1_ActivateEditModeSelect )
        {
            if( !pTSGrpPolylinesRotateTo->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Polylines_RotateTo_Polyline1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpPolylinesRotateTo->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpPolylinesRotateTo );

                strlstExpectedValues.append( "EditTool: Select" );
                strlstExpectedValues.append( "DrawingTool: " );

                pTestStep->setOperation( "drawing.setCurrentDrawingTool( Select )" );
                pTestStep->setExpectedValues(strlstExpectedValues);
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

                strlstResultValues.append( "EditTool: " + editTool2Str(m_pDrawingScene->getEditTool()) );
                strlstResultValues.append( "DrawingTool: " + strCurrentDrawingTool );

                pTestStep->setResultValues(strlstResultValues);
                pTestStep->setToolTip(strToolTip);
                pTestStep->testEnded();

                m_iTestStepLev3++;  // Next sub test step
                triggerDoTestStep();

            } // if( pTSGrpPolylinesRotateTo->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateTo_Polyline1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateTo_Polyline1_Select )
        {
            m_iTestStepLev4 = -1;
            m_fAngle_degree = 0.0;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "Select", pTSGrpPolylinesRotateTo );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosRotateToPolyline1Select;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            s_plg1Prev = s_plg1Curr;
            s_rctBoundingPlg1Prev = s_rctBoundingPlg1Curr;

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            m_pGraphObjSelected = nullptr;

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateTo_Polyline1_Select )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateTo_Polyline1_MoveSelPtRotateTop_Select )
        {
            m_iTestStepLev4 = -1;
            m_fAngle_degree = 0.0;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "SelPtRotateTopSelect", pTSGrpPolylinesRotateTo );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosRotateToPolyline1MoveSelPtRotateTopStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            s_plg1Prev = s_plg1Curr;
            s_rctBoundingPlg1Prev = s_rctBoundingPlg1Curr;

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointRotateTop) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateTo_Polyline1_MoveSelPtRotateTop_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polylines_RotateTo_Polyline1_MoveSelPtRotateTop_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_RotateTo_Polyline1_MoveSelPtRotateTop_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "RotateToMovePtRotateTopStep" + QString::number(m_iTestStepLev4), pTSGrpPolylinesRotateTo );

            m_fAngle_degree += s_arfPolylinesRotateToPolyline1MoveSelPtRotateTopTestStepsAngles_degree[m_iTestStepLev4];

            QPointF ptCenter;
            QPointF ptSelPtRot = m_pDrawingView->mapFromScene(s_ptDrawingSceneMousePosRotateToPolyline1MoveSelPtRotateTopStart);
            double  fAngle_rad = Math::deg2Rad(m_fAngle_degree);

            CGraphObjPolyline* pGraphObjPolyline = dynamic_cast<CGraphObjPolyline*>(m_pGraphObjSelected);

            if( pGraphObjPolyline != nullptr )
            {
                QRectF rctBounding = pGraphObjPolyline->polygon().boundingRect();
                ptCenter = rctBounding.center();
                ptCenter = pGraphObjPolyline->mapToScene(ptCenter);
                ptCenter = m_pDrawingView->mapFromScene(ptCenter);
            }

            ptSelPtRot = rotatePoint( ptCenter, ptSelPtRot, fAngle_rad );

            m_ptDrawingViewMousePos = ptSelPtRot.toPoint();

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plg1Curr = rotatePolygon( s_rctBoundingPlg1Prev.center(), s_plg1Prev, fAngle_rad );

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".rotateTo( " + QString::number(m_fAngle_degree) + "° )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* globalPos */ ptMousePosGlobal,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );

            m_pGraphObjSelected = nullptr;

            m_pDrawingView->mouseMoveEvent(pMouseEv);

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);

            bool bIgnoreTestResult = false;

            if( static_cast<int>(m_fAngle_degree) % 90 > 1 )
            {
                bIgnoreTestResult = true;
            }
            pTestStep->testEnded(bIgnoreTestResult);

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_RotateTo_Polyline1_MoveSelPtRotateTop_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_RotateTo_Polyline1_MoveSelPtRotateTop_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateTo_Polyline1_Release )
        {
            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "Release", pTSGrpPolylinesRotateTo );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plg1Curr) );

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".release( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateTo_Polyline1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateTo_Polyline1_Finished )
        {
            pTSGrpPolylinesMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateTo_Polyline1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateMoveResize_Polyline1_ActivateEditModeSelect )
        {
            if( !pTSGrpPolylinesRotateMoveResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Polylines_RotateMoveResize_Polyline1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpPolylinesRotateMoveResize->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpPolylinesRotateMoveResize );

                strlstExpectedValues.append( "EditTool: Select" );
                strlstExpectedValues.append( "DrawingTool: " );

                pTestStep->setOperation( "drawing.setCurrentDrawingTool( Select )" );
                pTestStep->setExpectedValues(strlstExpectedValues);
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

                strlstResultValues.append( "EditTool: " + editTool2Str(m_pDrawingScene->getEditTool()) );
                strlstResultValues.append( "DrawingTool: " + strCurrentDrawingTool );

                pTestStep->setResultValues(strlstResultValues);
                pTestStep->setToolTip(strToolTip);
                pTestStep->testEnded();

                m_iTestStepLev3++;  // Next sub test step
                triggerDoTestStep();

            } // if( pTSGrpPolylinesRotateResizeMove->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateResizeMove_Polyline1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateMoveResize_Polyline1_Start )
        {
            m_iTestStepLev4 = -1;
            m_fAngle_degree = 0.0;

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

            s_rctBoundingPlg1Curr = s_plg1Curr.boundingRect();

            s_plg1Prev = s_plg1Curr;
            s_rctBoundingPlg1Prev = s_rctBoundingPlg1Curr;

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateMoveResize_Polyline1_Start )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polylines_RotateMoveResize_Polyline1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_RotateMoveResize_Polyline1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGraphObjNamePolyline1;

            pTestStep = getTestStep( strCurrentGraphObj + "RotateMoveResizeStep" + QString::number(m_iTestStepLev4), pTSGrpPolylinesRotateMoveResize );

            SRotateMoveResizeOperation op = s_arRotateMoveResizePolyline1TestSteps[m_iTestStepLev4];

            m_ptDrawingSceneMousePos = op.m_ptDrawingSceneMousePos;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            switch( op.m_iMouseEvent )
            {
                case QEvent::MouseButtonPress:
                {
                    pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
                    break;
                }
                case QEvent::MouseMove:
                {
                    pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
                    break;
                }
                case QEvent::MouseButtonRelease:
                {
                    pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
                    break;
                }
                default:
                {
                    pTestStep->setOperation( "view.mouse?Event( Pos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
                    break;
                }
            }

            switch( op.m_operation )
            {
                case EEditModeMove:
                {
                    pTestStep->setDescription( "scene." + strCurrentGraphObj + ".move( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
                    break;
                }
                case EEditModeResize:
                {
                    pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
                    break;
                }
                case EEditModeRotate:
                {
                    pTestStep->setDescription( "scene." + strCurrentGraphObj + ".rotateTo( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
                    break;
                }
                case EEditModeMoveShapePoint:
                {
                    break;
                }
                case EEditModeSelect:
                {
                    if( op.m_iMouseEvent == QEvent::MouseButtonPress )
                    {
                        pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );

                        if( op.m_idxSelPtSelectedPolygon >= 0 )
                        {
                            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(op.m_idxSelPtSelectedPolygon) );
                        }
                        else
                        {
                            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(op.m_selPtSelectedBoundingRect) );
                        }
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( !op.m_bIgnoreTestResult )
            {
                QPolygonF plg1;
                plg1.append(op.m_pt0PolygonResult);
                plg1.append(op.m_pt1PolygonResult);
                plg1.append(op.m_pt2PolygonResult);
                strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(plg1) );
            }

            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            m_pGraphObjSelected = nullptr;

            if( op.m_iMouseEvent == QEvent::MouseButtonPress )
            {
                pMouseEv = new QMouseEvent(
                    /* type      */ QEvent::MouseButtonPress,
                    /* pos       */ m_ptDrawingViewMousePos,
                    /* globalPos */ ptMousePosGlobal,
                    /* button    */ Qt::LeftButton,
                    /* buttons   */ Qt::LeftButton,
                    /* modifiers */ Qt::NoModifier );
                m_pDrawingView->mousePressEvent(pMouseEv);
            }
            else if( op.m_iMouseEvent == QEvent::MouseMove )
            {
                pMouseEv = new QMouseEvent(
                    /* type      */ QEvent::MouseMove,
                    /* pos       */ m_ptDrawingViewMousePos,
                    /* globalPos */ ptMousePosGlobal,
                    /* button    */ Qt::NoButton,
                    /* buttons   */ Qt::LeftButton,
                    /* modifiers */ Qt::NoModifier );
                m_pDrawingView->mouseMoveEvent(pMouseEv);
            }
            else if( op.m_iMouseEvent == QEvent::MouseButtonRelease )
            {
                pMouseEv = new QMouseEvent(
                    /* type      */ QEvent::MouseButtonRelease,
                    /* pos       */ m_ptDrawingViewMousePos,
                    /* globalPos */ ptMousePosGlobal,
                    /* button    */ Qt::LeftButton,
                    /* buttons   */ Qt::NoButton,
                    /* modifiers */ Qt::NoModifier );
                m_pDrawingView->mouseReleaseEvent(pMouseEv);
            }

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();

                if( op.m_iMouseEvent == QEvent::MouseButtonPress && op.m_operation == EEditModeSelect )
                {
                    if( m_pGraphObjSelected->getSelectedPolygonShapePointIndex() >= 0 )
                    {
                        strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(m_pGraphObjSelected->getSelectedPolygonShapePointIndex()) );
                    }
                    else
                    {
                        strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                    }
                }
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);

            pTestStep->testEnded(op.m_bIgnoreTestResult);

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_RotateMoveResize_Polyline1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polylines_RotateMoveResize_Polyline1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateMoveResize_Polyline1_Finished )
        {
            pTSGrpPolylinesMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_RotateMoveResize_Polyline1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_Finished )
        {
            pTSGrpPolylinesResize->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polylines_Resize_Polyline1_Finished )

        else // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Finished )
        {
            pTSGrpPolylines->testEnded();

            m_iTestStepLev2++; // Next test step
            m_iTestStepLev3 = 0;
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polylines_Finished )

    } // if( pTSGrpPolylines->isEnabled() )

    delete pMouseEv;
    pMouseEv = nullptr;

} // doTestStepStandardShapesPolylines

#endif // #if 0
