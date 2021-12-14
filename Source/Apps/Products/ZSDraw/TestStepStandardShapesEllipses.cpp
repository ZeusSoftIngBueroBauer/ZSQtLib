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
#include "ZSDraw/ZSDrawGraphObjEllipse.h"
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

#if 0

static QRectF    s_rctEllipse1;
static QPolygonF s_plgEllipse1;
static QRectF    s_rctEllipse1OnMousePressEvent; // during mouse move events the item's position is not changed

// Create_Ellipse1_MovePtBottomRight
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosEllipse1Start(400,100);

static const QSize s_arsizeEllipsesCreateEllipse1MovePtBottomRightTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize(  20,  10 ), /* 400, 100, 420, 110,  20,  10 */
    /*  1 */ QSize(  20,  10 ), /* 400, 100, 440, 120,  40,  20 */
    /*  2 */ QSize(  20,  10 ), /* 400, 100, 460, 130,  60,  30 */
    /*  3 */ QSize(  20,  10 ), /* 400, 100, 480, 140,  80,  40 */
    /*  3 */ QSize(  20,  10 )  /* 400, 100, 500, 150, 100,  50 */
};

static const int ETestStepLev3_Ellipses_Create_Ellipse1_MovePtBottomRight_TestStepCount = sizeof(s_arsizeEllipsesCreateEllipse1MovePtBottomRightTestSteps)/sizeof(s_arsizeEllipsesCreateEllipse1MovePtBottomRightTestSteps[0]);

// Move_Ellipse1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosMoveEllipse1Start(420,145);

static const QSize s_arsizeEllipsesMoveEllipse1TestSteps[] = {
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

static const int ETestStepLev3_Ellipses_Move_Ellipse1_TestStepCount = sizeof(s_arsizeEllipsesMoveEllipse1TestSteps)/sizeof(s_arsizeEllipsesMoveEllipse1TestSteps[0]);

// Resize_Ellipse1_MovePtBottomRight
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeEllipse1Select(450,120);

static const QPoint s_ptDrawingSceneMousePosResizeEllipse1PtBottomRightStart(498,152); // The bottom right corner should not been hit exactly.

static const QSize s_arsizeEllipsesResizeEllipse1MovePtBottomRightTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize( -48, -27 ), /* 400, 100, 450, 125,  50,  25 */
    /*  1 */ QSize( -50, -25 ), /* 400, 100, 400, 100,   0,   0 */
    /*  2 */ QSize( -50, -25 ), /* 350,  75, 400,  75,  50,  25 */
    /*  3 */ QSize( -50, -25 )  /* 300,  50, 400,  50, 100,  50 */
};

static const int ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomRight_TestStepCount = sizeof(s_arsizeEllipsesResizeEllipse1MovePtBottomRightTestSteps)/sizeof(s_arsizeEllipsesResizeEllipse1MovePtBottomRightTestSteps[0]);

// Resize_Ellipse1_MovePtTopLeft
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeEllipse1PtTopLeftStart(300,50);

static const QSize s_arsizeEllipsesResizeEllipse1MovePtTopLeftTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize(  50,  25 ), /* 350,  75, 400, 100,  50,  25 */
    /*  1 */ QSize(  50,  25 ), /* 400, 100, 400, 100,   0,   0 */
    /*  2 */ QSize(  50,  25 ), /* 400, 100, 450, 125,  50,  25 */
    /*  3 */ QSize(  50,  25 )  /* 400, 100, 500, 150, 100,  50 */
};

static const int ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopLeft_TestStepCount = sizeof(s_arsizeEllipsesResizeEllipse1MovePtTopLeftTestSteps)/sizeof(s_arsizeEllipsesResizeEllipse1MovePtTopLeftTestSteps[0]);

// Resize_Ellipse1_MovePtTopRight
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeEllipse1PtTopRightStart(500,100);

static const QSize s_arsizeEllipsesResizeEllipse1MovePtTopRightTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize( -50,  25 ), /* 400, 125, 450, 150,  50,  25 */
    /*  1 */ QSize( -50,  25 ), /* 400, 150, 400, 150,   0,   0 */
    /*  2 */ QSize( -50,  25 ), /* 350, 150, 400, 175,  50,  25 */
    /*  3 */ QSize( -50,  25 )  /* 300, 150, 400, 200, 100,  50 */
};

static const int ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopRight_TestStepCount = sizeof(s_arsizeEllipsesResizeEllipse1MovePtTopRightTestSteps)/sizeof(s_arsizeEllipsesResizeEllipse1MovePtTopRightTestSteps[0]);

// Resize_Ellipse1_MovePtBottomLeft
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeEllipse1PtBottomLeftStart(300,200);

static const QSize s_arsizeEllipsesResizeEllipse1MovePtBottomLeftTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize(  50, -25 ), /* 350, 150, 400, 175,  50,  25 */
    /*  1 */ QSize(  50, -25 ), /* 400, 150, 400, 150,   0,   0 */
    /*  2 */ QSize(  50, -25 ), /* 400, 125, 450, 150,  50,  25 */
    /*  3 */ QSize(  50, -25 )  /* 400, 100, 500, 150, 100,  50 */
};

static const int ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomLeft_TestStepCount = sizeof(s_arsizeEllipsesResizeEllipse1MovePtBottomLeftTestSteps)/sizeof(s_arsizeEllipsesResizeEllipse1MovePtBottomLeftTestSteps[0]);

// Resize_Ellipse1_MovePtBottomCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeEllipse1PtBottomCenterStart(450,150);

static const QSize s_arsizeEllipsesResizeEllipse1MovePtBottomCenterTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize(   0, -25 ), /* 400, 100, 500, 125, 100,  25 */
    /*  1 */ QSize(   0, -25 ), /* 400, 100, 500, 100, 100,   0 */
    /*  2 */ QSize(   0, -25 ), /* 400,  75, 500, 100, 100,  25 */
    /*  3 */ QSize(   0, -25 )  /* 400,  50, 500, 100, 100,  50 */
};

static const int ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomCenter_TestStepCount = sizeof(s_arsizeEllipsesResizeEllipse1MovePtBottomCenterTestSteps)/sizeof(s_arsizeEllipsesResizeEllipse1MovePtBottomCenterTestSteps[0]);

// Resize_Ellipse1_MovePtTopCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeEllipse1PtTopCenterStart(450,50);

static const QSize s_arsizeEllipsesResizeEllipse1MovePtTopCenterTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize(   0,  25 ), /* 400,  75, 500, 100, 100,  25 */
    /*  1 */ QSize(   0,  25 ), /* 400, 100, 500, 100, 100,   0 */
    /*  2 */ QSize(   0,  25 ), /* 400, 100, 500, 125, 100,  25 */
    /*  3 */ QSize(   0,  25 )  /* 400, 100, 500, 150, 100,  50 */
};

static const int ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopCenter_TestStepCount = sizeof(s_arsizeEllipsesResizeEllipse1MovePtTopCenterTestSteps)/sizeof(s_arsizeEllipsesResizeEllipse1MovePtTopCenterTestSteps[0]);

// Resize_Ellipse1_MovePtRightCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeEllipse1PtRightCenterStart(500,125);

static const QSize s_arsizeEllipsesResizeEllipse1MovePtRightCenterTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize( -50,   0 ), /* 400, 100, 450, 150,  50,  50 */
    /*  1 */ QSize( -50,   0 ), /* 400, 100, 400, 150,   0,  50 */
    /*  2 */ QSize( -50,   0 ), /* 350, 100, 400, 150,  50,  50 */
    /*  3 */ QSize( -50,   0 )  /* 300, 100, 400, 150, 100,  50 */
};

static const int ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtRightCenter_TestStepCount = sizeof(s_arsizeEllipsesResizeEllipse1MovePtRightCenterTestSteps)/sizeof(s_arsizeEllipsesResizeEllipse1MovePtRightCenterTestSteps[0]);

// Resize_Ellipse1_MovePtLeftCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeEllipse1PtLeftCenterStart(300,125);

static const QSize s_arsizeEllipsesResizeEllipse1MovePtLeftCenterTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize(  50,   0 ), /* 350, 100, 400, 150,  50,  50 */
    /*  1 */ QSize(  50,   0 ), /* 400, 100, 400, 150,   0,  50 */
    /*  2 */ QSize(  50,   0 ), /* 400, 100, 450, 150,  50,  50 */
    /*  3 */ QSize(  50,   0 )  /* 400, 100, 500, 150, 100,  50 */
};

static const int ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtLeftCenter_TestStepCount = sizeof(s_arsizeEllipsesResizeEllipse1MovePtLeftCenterTestSteps)/sizeof(s_arsizeEllipsesResizeEllipse1MovePtLeftCenterTestSteps[0]);

// RotateTo_Ellipse1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosRotateToEllipse1Select(420,125);

static const QPoint s_ptDrawingSceneMousePosRotateToEllipse1MoveSelPtRotateTopStart(450,80);

static const double s_arfEllipsesRotateToEllipse1MoveSelPtRotateTopTestStepsAngles_degree[] = {
         /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /*  1 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /*  2 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /*  3 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /*  4 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /*  5 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /*  6 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /*  7 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /*  8 */ 10.0, /* 425,   0, 475, 200, 100,  50 */ //  90°
    /*  9 */ 30.0, /*    ,    ,    ,    , 100,  50 */
    /* 10 */ 30.0, /*    ,    ,    ,    , 100,  50 */
    /* 11 */ 30.0, /* 400, 100, 500, 150, 100,  50 */ // 180°
    /* 12 */ 45.0, /*    ,    ,    ,    , 100,  50 */
    /* 13 */ 45.0, /* 425,   0, 475, 200, 100,  50 */ // 270°
    /* 14 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /* 15 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /* 16 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /* 17 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /* 18 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /* 19 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /* 20 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /* 21 */ 10.0, /*    ,    ,    ,    , 100,  50 */
    /* 22 */ 10.0  /* 400, 100, 500, 150, 100,  50 */ // 360°
};

static const int ETestStepLev3_Ellipses_RotateTo_Ellipse1_MoveSelPtRotateTop_TestStepCount = sizeof(s_arfEllipsesRotateToEllipse1MoveSelPtRotateTopTestStepsAngles_degree)/sizeof(s_arfEllipsesRotateToEllipse1MoveSelPtRotateTopTestStepsAngles_degree[0]);

// RotateMoveResize_Ellipse1
//------------------------------------------------------------------------------

struct SRotateMoveResizeOperation
{
    QPoint    m_ptDrawingSceneMousePos;
    QPoint    m_ptDrawingSceneRectCenterPos;
    int       m_iMouseEvent;
    EEditMode m_operation;
    QPointF   m_ptPos;
    QPointF   m_pt0PolygonRectResult;
    QPointF   m_pt1PolygonRectResult;
    QPointF   m_pt2PolygonRectResult;
    QPointF   m_pt3PolygonRectResult;
    bool      m_bIgnoreTestResult;
};

static const SRotateMoveResizeOperation s_arRotateMoveResizeEllipse1TestSteps[] = {
          // {        MousePos,       RectCenterPos,               MouseEvent,         Operation,                  Pos,       PolygonRect[0],       PolygonRect[1],       PolygonRect[2],       PolygonRect[3], Ignore }
    /*  0 */ { QPoint(450,110), QPoint(450.0,125.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(400.0,100.0), QPointF(400.0,100.0), QPointF(500.0,100.0), QPointF(500.0,150.0), QPointF(400.0,150.0), false  },
    /*  1 */ { QPoint( -1, -1), QPoint(450.0,125.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(400.0,100.0), QPointF(400.0,100.0), QPointF(500.0,100.0), QPointF(500.0,150.0), QPointF(400.0,150.0), false  },
    /*  2 */ { QPoint(450, 80), QPoint(450.0,125.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(400.0,100.0), QPointF(400.0,100.0), QPointF(500.0,100.0), QPointF(500.0,150.0), QPointF(400.0,150.0), false  },
    /*  3 */ { QPoint(380,125), QPoint(450.0,125.0), QEvent::MouseMove,          EEditModeRotate, QPointF(400.0,100.0), QPointF(425.0,175.0), QPointF(425.0, 75.0), QPointF(475.0, 75.0), QPointF(475.0,175.0), false  },
    /*  4 */ { QPoint( -1, -1), QPoint(450.0,125.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(400.0,100.0), QPointF(425.0,175.0), QPointF(425.0, 75.0), QPointF(475.0, 75.0), QPointF(475.0,175.0), false  },
    /*  5 */ { QPoint(450,125), QPoint(450.0,125.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(400.0,100.0), QPointF(425.0,175.0), QPointF(425.0, 75.0), QPointF(475.0, 75.0), QPointF(475.0,175.0), false  },
    /*  6 */ { QPoint(500,225), QPoint(500.0,225.0), QEvent::MouseMove,          EEditModeMove,   QPointF(450.0,200.0), QPointF(475.0,275.0), QPointF(475.0,175.0), QPointF(525.0,175.0), QPointF(525.0,275.0), false  },
    /*  7 */ { QPoint( -1, -1), QPoint(500.0,225.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(450.0,200.0), QPointF(475.0,275.0), QPointF(475.0,175.0), QPointF(525.0,175.0), QPointF(525.0,275.0), false  },
    /*  8 */ { QPoint(475,225), QPoint(500.0,225.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(450.0,200.0), QPointF(475.0,275.0), QPointF(475.0,175.0), QPointF(525.0,175.0), QPointF(525.0,275.0), false  },
    /*  9 */ { QPoint(485,225), QPoint(505.0,225.0), QEvent::MouseMove,          EEditModeResize, QPointF(450.0,200.0), QPointF(485.0,275.0), QPointF(485.0,175.0), QPointF(525.0,175.0), QPointF(525.0,275.0), false  },
    /* 10 */ { QPoint( -1, -1), QPoint(505.0,225.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(455.0,205.0), QPointF(485.0,275.0), QPointF(485.0,175.0), QPointF(525.0,175.0), QPointF(525.0,275.0), false  },
    /* 11 */ { QPoint(465,225), QPoint(505.0,225.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(455.0,205.0), QPointF(485.0,275.0), QPointF(485.0,175.0), QPointF(525.0,175.0), QPointF(525.0,275.0), false  },
    /* 12 */ { QPoint(505,185), QPoint(505.0,225.0), QEvent::MouseMove,          EEditModeRotate, QPointF(455.0,205.0), QPointF(455.0,205.0), QPointF(555.0,205.0), QPointF(555.0,245.0), QPointF(455.0,245.0), false  },
    /* 13 */ { QPoint( -1, -1), QPoint(505.0,225.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(455.0,205.0), QPointF(455.0,205.0), QPointF(555.0,205.0), QPointF(555.0,245.0), QPointF(455.0,245.0), false  },
    /* 14 */ { QPoint(505,205), QPoint(505.0,225.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(455.0,205.0), QPointF(455.0,205.0), QPointF(555.0,205.0), QPointF(555.0,245.0), QPointF(455.0,245.0), false  },
    /* 15 */ { QPoint(505,195), QPoint(505.0,220.0), QEvent::MouseMove,          EEditModeResize, QPointF(455.0,205.0), QPointF(455.0,195.0), QPointF(555.0,195.0), QPointF(555.0,245.0), QPointF(455.0,245.0), false  },
    /* 16 */ { QPoint( -1, -1), QPoint(505.0,220.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(455.0,195.0), QPointF(455.0,195.0), QPointF(555.0,195.0), QPointF(555.0,245.0), QPointF(455.0,245.0), false  },
    /* 17 */ { QPoint(505,220), QPoint(505.0,220.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(455.0,195.0), QPointF(455.0,195.0), QPointF(555.0,195.0), QPointF(555.0,245.0), QPointF(455.0,245.0), false  },
    /* 18 */ { QPoint(450, 75), QPoint(450.0,100.0), QEvent::MouseMove,          EEditModeMove,   QPointF(400.0, 50.0), QPointF(400.0, 50.0), QPointF(500.0, 50.0), QPointF(500.0,100.0), QPointF(400.0,100.0), false  },
    /* 19 */ { QPoint( -1, -1), QPoint(450.0,100.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(400.0, 50.0), QPointF(400.0, 50.0), QPointF(500.0, 50.0), QPointF(500.0,100.0), QPointF(400.0,100.0), false  }
};

static const int ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_TestStepCount = sizeof(s_arRotateMoveResizeEllipse1TestSteps)/sizeof(s_arRotateMoveResizeEllipse1TestSteps[0]);


// Test steps:
//------------------------------------------------------------------------------

enum
{
    ETestStepLev3_Ellipses_Create_Ellipse1_ActivateDrawingTool,
    ETestStepLev3_Ellipses_Create_Ellipse1,
    ETestStepLev3_Ellipses_Create_Ellipse1_MovePtBottomRight_TestStepMin,
    ETestStepLev3_Ellipses_Create_Ellipse1_MovePtBottomRight_TestStepMax = ETestStepLev3_Ellipses_Create_Ellipse1_MovePtBottomRight_TestStepMin + ETestStepLev3_Ellipses_Create_Ellipse1_MovePtBottomRight_TestStepCount - 1,
    ETestStepLev3_Ellipses_Create_Ellipse1_Release,
    ETestStepLev3_Ellipses_Create_Ellipse1_Finished,
    ETestStepLev3_Ellipses_Move_Ellipse1_ActivateEditModeSelect,
    ETestStepLev3_Ellipses_Move_Ellipse1_Select,
    ETestStepLev3_Ellipses_Move_Ellipse1_TestStepMin,
    ETestStepLev3_Ellipses_Move_Ellipse1_TestStepMax = ETestStepLev3_Ellipses_Move_Ellipse1_TestStepMin + ETestStepLev3_Ellipses_Move_Ellipse1_TestStepCount - 1,
    ETestStepLev3_Ellipses_Move_Ellipse1_Release,
    ETestStepLev3_Ellipses_Move_Ellipse1_Finished,
    ETestStepLev3_Ellipses_Resize_Ellipse1_ActivateEditModeSelect,
    ETestStepLev3_Ellipses_Resize_Ellipse1_Select,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomRight_Select,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomRight_TestStepMin,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomRight_TestStepMax = ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomRight_TestStepMin + ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomRight_TestStepCount - 1,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomRight_Release,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopLeft_Select,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopLeft_TestStepMin,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopLeft_TestStepMax = ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopLeft_TestStepMin + ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopLeft_TestStepCount - 1,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopLeft_Release,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopRight_Select,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopRight_TestStepMin,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopRight_TestStepMax = ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopRight_TestStepMin + ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopRight_TestStepCount - 1,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopRight_Release,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomLeft_Select,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomLeft_TestStepMin,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomLeft_TestStepMax = ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomLeft_TestStepMin + ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomLeft_TestStepCount - 1,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomLeft_Release,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomCenter_Select,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomCenter_TestStepMin,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomCenter_TestStepMax = ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomCenter_TestStepMin + ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomCenter_TestStepCount - 1,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomCenter_Release,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopCenter_Select,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopCenter_TestStepMin,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopCenter_TestStepMax = ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopCenter_TestStepMin + ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopCenter_TestStepCount - 1,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopCenter_Release,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtRightCenter_Select,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtRightCenter_TestStepMin,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtRightCenter_TestStepMax = ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtRightCenter_TestStepMin + ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtRightCenter_TestStepCount - 1,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtRightCenter_Release,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtLeftCenter_Select,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtLeftCenter_TestStepMin,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtLeftCenter_TestStepMax = ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtLeftCenter_TestStepMin + ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtLeftCenter_TestStepCount - 1,
    ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtLeftCenter_Release,
    ETestStepLev3_Ellipses_Resize_Ellipse1_Finished,
    ETestStepLev3_Ellipses_RotateTo_Ellipse1_ActivateEditModeSelect,
    ETestStepLev3_Ellipses_RotateTo_Ellipse1_Select,
    ETestStepLev3_Ellipses_RotateTo_Ellipse1_MoveSelPtRotateTop_Select,
    ETestStepLev3_Ellipses_RotateTo_Ellipse1_MoveSelPtRotateTop_TestStepMin,
    ETestStepLev3_Ellipses_RotateTo_Ellipse1_MoveSelPtRotateTop_TestStepMax = ETestStepLev3_Ellipses_RotateTo_Ellipse1_MoveSelPtRotateTop_TestStepMin + ETestStepLev3_Ellipses_RotateTo_Ellipse1_MoveSelPtRotateTop_TestStepCount - 1,
    ETestStepLev3_Ellipses_RotateTo_Ellipse1_Release,
    ETestStepLev3_Ellipses_RotateTo_Ellipse1_Finished,
    ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_ActivateEditModeSelect,
    ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_Start,
    ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_TestStepMin,
    ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_TestStepMax = ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_TestStepMin + ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_TestStepCount - 1,
    ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_Finished,
    ETestStepLev3_Ellipses_Count
};

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepStandardShapesEllipses( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
//------------------------------------------------------------------------------
{
    QMouseEvent*         pMouseEv = nullptr;
    QPoint               ptMousePosGlobal;
    QString              strCurrentDrawingTool;
    QString              strCurrentGraphObj;
    QString              strDesiredPainterPath;
    QString              strToolTip;
    ZS::Test::CTestStep* pTestStep;
    QStringList          strlstDesiredValues;
    QStringList          strlstActualValues;

    ZS::Test::CTestStepGroup* pTSGrpEllipses                 = getTestStepGroup( "Ellipses", i_pTestStepGrpParent );
    ZS::Test::CTestStepGroup* pTSGrpEllipsesCreate           = getTestStepGroup( "Create", pTSGrpEllipses );
    ZS::Test::CTestStepGroup* pTSGrpEllipsesMove             = getTestStepGroup( "Move", pTSGrpEllipses );
    ZS::Test::CTestStepGroup* pTSGrpEllipsesResize           = getTestStepGroup( "Resize", pTSGrpEllipses );
    ZS::Test::CTestStepGroup* pTSGrpEllipsesRotateTo         = getTestStepGroup( "RotateTo", pTSGrpEllipses );
    ZS::Test::CTestStepGroup* pTSGrpEllipsesRotateMoveResize = getTestStepGroup( "RotateMoveResize", pTSGrpEllipses );

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }

    if( !pTSGrpEllipses->isEnabled() )
    {
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else if( !pTSGrpEllipsesCreate->isEnabled() )
    {
        // If the ellipse is not created none of the other sub tests may be performed.
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else // if( pTSGrpEllipses->isEnabled() )
    {
        if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Create_Ellipse1_ActivateDrawingTool )
        {
            if( !pTSGrpEllipsesCreate->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Ellipses_Create_Ellipse1_Finished+1; // Last create test step
                triggerDoTestStep();
            }
            else // if( pTSGrpEllipsesCreate->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateDrawingTool", pTSGrpEllipsesCreate );

                strlstDesiredValues.append( "EditTool: CreateObjects" );
                strlstDesiredValues.append( "DrawingTool: Ellipse" );

                pTestStep->setOperation( "drawing.setCurrentDrawingTool( Ellipse )" );
                pTestStep->setDesiredValues(strlstDesiredValues);
                pTestStep->testStarted();

                if( (m_pDrawingScene->getEditTool() != EEditToolCreateObjects) || (m_pDrawingScene->getCurrentDrawingToolGraphObjType() != EGraphObjTypeEllipse) )
                {
                    m_pMainWindow->setCheckedActionDrawStandardShapeEllipse(true);
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

            } // if( pTSGrpEllipsesCreate->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Create_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Create_Ellipse1 )
        {
            m_iEllipsesCount = 1;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj, pTSGrpEllipsesCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosEllipse1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctEllipse1 = QRectF( s_ptDrawingSceneMousePosEllipse1Start.x(), s_ptDrawingSceneMousePosEllipse1Start.y(), 1, 1 );

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".createGraphObj( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Create_Ellipse1 )

        else if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Create_Ellipse1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Create_Ellipse1_MovePtBottomRight_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightStep" + QString::number(m_iTestStepLev4), pTSGrpEllipsesCreate );

            m_fxOffsHor = s_arsizeEllipsesCreateEllipse1MovePtBottomRightTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeEllipsesCreateEllipse1MovePtBottomRightTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctEllipse1.setRight(m_ptDrawingSceneMousePos.x());
            s_rctEllipse1.setBottom(m_ptDrawingSceneMousePos.y());

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Create_Ellipse1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Create_Ellipse1_MovePtBottomRight_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Create_Ellipse1_Release )
        {
            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "Release", pTSGrpEllipsesCreate );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

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

            m_pGraphObjCreating = nullptr;

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Create_Ellipse1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Create_Ellipse1_Finished )
        {
            pTSGrpEllipsesCreate->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Create_Ellipse1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Move_Ellipse1_ActivateEditModeSelect )
        {
            if( !pTSGrpEllipsesMove->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Ellipses_Move_Ellipse1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpEllipsesMove->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpEllipsesMove );

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

            } // if( pTSGrpEllipsesMove->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Move_Ellipse1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Move_Ellipse1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            s_rctEllipse1OnMousePressEvent = s_rctEllipse1;

            pTestStep = getTestStep( strCurrentGraphObj, pTSGrpEllipsesMove );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosMoveEllipse1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Move_Ellipse1_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Move_Ellipse1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Move_Ellipse1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MoveStep" + QString::number(m_iTestStepLev4), pTSGrpEllipsesMove );

            m_fxOffsHor = s_arsizeEllipsesMoveEllipse1TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeEllipsesMoveEllipse1TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctEllipse1.moveLeft( s_rctEllipse1.left() + m_fxOffsHor );
            s_rctEllipse1.moveTop( s_rctEllipse1.top() + m_fyOffsHor );

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".move( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Move_Ellipse1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Move_Ellipse1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Move_Ellipse1_Release )
        {
            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "Release", pTSGrpEllipsesMove );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Move_Ellipse1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Move_Ellipse1_Finished )
        {
            pTSGrpEllipsesMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Move_Ellipse1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_ActivateEditModeSelect )
        {
            if( !pTSGrpEllipsesResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Ellipses_Resize_Ellipse1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpEllipsesResize->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpEllipsesResize );

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

            } // if( pTSGrpEllipsesResize->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            s_rctEllipse1OnMousePressEvent = s_rctEllipse1;

            pTestStep = getTestStep( strCurrentGraphObj + "Select", pTSGrpEllipsesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeEllipse1Select;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
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
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_Select )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomRight_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            s_rctEllipse1OnMousePressEvent = s_rctEllipse1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightSelect", pTSGrpEllipsesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeEllipse1PtBottomRightStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointBottomRight) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomRight_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomRight_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightStep" + QString::number(m_iTestStepLev4), pTSGrpEllipsesResize );

            m_fxOffsHor = s_arsizeEllipsesResizeEllipse1MovePtBottomRightTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeEllipsesResizeEllipse1MovePtBottomRightTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctEllipse1.setRight(m_ptDrawingSceneMousePos.x());
            s_rctEllipse1.setBottom(m_ptDrawingSceneMousePos.y());

            // Keep old size before resizing starts as the object may get 0 pixel size.
            // The size will be adjusted on releasing the mouse.
            QRectF rct = s_rctEllipse1;

            if( fabs(s_rctEllipse1.width()) > 0.0 || fabs(s_rctEllipse1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(rct) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( scenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomRight_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomRight_Release )
        {
            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightRelease", pTSGrpEllipsesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            // Old size before resizing starts has been kept as the object may get 0 pixel size.
            // The size will be adjusted on releasing the mouse.
            if( s_rctEllipse1.width() < 0 )
            {
                double fLeft = s_rctEllipse1.left();
                double fRight = s_rctEllipse1.right();
                s_rctEllipse1.setLeft(fRight);
                s_rctEllipse1.setRight(fLeft);
            }
            if( s_rctEllipse1.height() < 0 )
            {
                double fTop = s_rctEllipse1.top();
                double fBottom = s_rctEllipse1.bottom();
                s_rctEllipse1.setTop(fBottom);
                s_rctEllipse1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomRight_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopLeft_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            s_rctEllipse1OnMousePressEvent = s_rctEllipse1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopLeftSelect", pTSGrpEllipsesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeEllipse1PtTopLeftStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointTopLeft) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopLeft_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopLeft_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopLeftStep" + QString::number(m_iTestStepLev4), pTSGrpEllipsesResize );

            m_fxOffsHor = s_arsizeEllipsesResizeEllipse1MovePtTopLeftTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeEllipsesResizeEllipse1MovePtTopLeftTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctEllipse1.setLeft( s_rctEllipse1.left() + m_fxOffsHor );
            s_rctEllipse1.setTop( s_rctEllipse1.top() + m_fyOffsHor );

            QRectF rct = s_rctEllipse1;

            if( fabs(s_rctEllipse1.width()) > 0.0 || fabs(s_rctEllipse1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(rct) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopLeft_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopLeft_Release )
        {
            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopLeftRelease", pTSGrpEllipsesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rctEllipse1.width() < 0 )
            {
                double fLeft = s_rctEllipse1.left();
                double fRight = s_rctEllipse1.right();
                s_rctEllipse1.setLeft(fRight);
                s_rctEllipse1.setRight(fLeft);
            }
            if( s_rctEllipse1.height() < 0 )
            {
                double fTop = s_rctEllipse1.top();
                double fBottom = s_rctEllipse1.bottom();
                s_rctEllipse1.setTop(fBottom);
                s_rctEllipse1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopLeft_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopRight_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            s_rctEllipse1OnMousePressEvent = s_rctEllipse1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopRightSelect", pTSGrpEllipsesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeEllipse1PtTopRightStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointTopRight) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopRight_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopRight_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopRightStep" + QString::number(m_iTestStepLev4), pTSGrpEllipsesResize );

            m_fxOffsHor = s_arsizeEllipsesResizeEllipse1MovePtTopRightTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeEllipsesResizeEllipse1MovePtTopRightTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctEllipse1.setRight( s_rctEllipse1.right() + m_fxOffsHor );
            s_rctEllipse1.setTop( s_rctEllipse1.top() + m_fyOffsHor );

            QRectF rct = s_rctEllipse1;

            if( fabs(s_rctEllipse1.width()) > 0.0 || fabs(s_rctEllipse1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(rct) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopRight_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopRight_Release )
        {
            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopRightRelease", pTSGrpEllipsesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rctEllipse1.width() < 0 )
            {
                double fLeft = s_rctEllipse1.left();
                double fRight = s_rctEllipse1.right();
                s_rctEllipse1.setLeft(fRight);
                s_rctEllipse1.setRight(fLeft);
            }
            if( s_rctEllipse1.height() < 0 )
            {
                double fTop = s_rctEllipse1.top();
                double fBottom = s_rctEllipse1.bottom();
                s_rctEllipse1.setTop(fBottom);
                s_rctEllipse1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopRight_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomLeft_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            s_rctEllipse1OnMousePressEvent = s_rctEllipse1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomLeftSelect", pTSGrpEllipsesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeEllipse1PtBottomLeftStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointBottomLeft) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomLeft_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomLeft_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomLeftStep" + QString::number(m_iTestStepLev4), pTSGrpEllipsesResize );

            m_fxOffsHor = s_arsizeEllipsesResizeEllipse1MovePtBottomLeftTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeEllipsesResizeEllipse1MovePtBottomLeftTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctEllipse1.setLeft( s_rctEllipse1.left() + m_fxOffsHor );
            s_rctEllipse1.setBottom( s_rctEllipse1.bottom() + m_fyOffsHor );

            QRectF rct = s_rctEllipse1;

            if( fabs(s_rctEllipse1.width()) > 0.0 || fabs(s_rctEllipse1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(rct) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomLeft_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomLeft_Release )
        {
            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomLeftRelease", pTSGrpEllipsesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rctEllipse1.width() < 0 )
            {
                double fLeft = s_rctEllipse1.left();
                double fRight = s_rctEllipse1.right();
                s_rctEllipse1.setLeft(fRight);
                s_rctEllipse1.setRight(fLeft);
            }
            if( s_rctEllipse1.height() < 0 )
            {
                double fTop = s_rctEllipse1.top();
                double fBottom = s_rctEllipse1.bottom();
                s_rctEllipse1.setTop(fBottom);
                s_rctEllipse1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomLeft_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            s_rctEllipse1OnMousePressEvent = s_rctEllipse1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomCenterSelect", pTSGrpEllipsesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeEllipse1PtBottomCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointBottomCenter) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomCenterStep" + QString::number(m_iTestStepLev4), pTSGrpEllipsesResize );

            m_fxOffsHor = s_arsizeEllipsesResizeEllipse1MovePtBottomCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeEllipsesResizeEllipse1MovePtBottomCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctEllipse1.setBottom( s_rctEllipse1.bottom() + m_fyOffsHor );

            QRectF rct = s_rctEllipse1;

            if( fabs(s_rctEllipse1.width()) > 0.0 || fabs(s_rctEllipse1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(rct) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomCenter_Release )
        {
            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomCenterRelease", pTSGrpEllipsesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rctEllipse1.width() < 0 )
            {
                double fLeft = s_rctEllipse1.left();
                double fRight = s_rctEllipse1.right();
                s_rctEllipse1.setLeft(fRight);
                s_rctEllipse1.setRight(fLeft);
            }

            if( s_rctEllipse1.height() < 0 )
            {
                double fTop = s_rctEllipse1.top();
                double fBottom = s_rctEllipse1.bottom();
                s_rctEllipse1.setTop(fBottom);
                s_rctEllipse1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtBottomCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            s_rctEllipse1OnMousePressEvent = s_rctEllipse1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopCenterSelect", pTSGrpEllipsesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeEllipse1PtTopCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointTopCenter) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopCenterStep" + QString::number(m_iTestStepLev4), pTSGrpEllipsesResize );

            m_fxOffsHor = s_arsizeEllipsesResizeEllipse1MovePtTopCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeEllipsesResizeEllipse1MovePtTopCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctEllipse1.setTop( s_rctEllipse1.top() + m_fyOffsHor );

            QRectF rct = s_rctEllipse1;

            if( fabs(s_rctEllipse1.width()) > 0.0 || fabs(s_rctEllipse1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(rct) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopCenter_Release )
        {
            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopCenterRelease", pTSGrpEllipsesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rctEllipse1.width() < 0 )
            {
                double fLeft = s_rctEllipse1.left();
                double fRight = s_rctEllipse1.right();
                s_rctEllipse1.setLeft(fRight);
                s_rctEllipse1.setRight(fLeft);
            }

            if( s_rctEllipse1.height() < 0 )
            {
                double fTop = s_rctEllipse1.top();
                double fBottom = s_rctEllipse1.bottom();
                s_rctEllipse1.setTop(fBottom);
                s_rctEllipse1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtTopCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtRightCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            s_rctEllipse1OnMousePressEvent = s_rctEllipse1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtRightCenterSelect", pTSGrpEllipsesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeEllipse1PtRightCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointRightCenter) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtRightCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtRightCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtRightCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtRightCenterStep" + QString::number(m_iTestStepLev4), pTSGrpEllipsesResize );

            m_fxOffsHor = s_arsizeEllipsesResizeEllipse1MovePtRightCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeEllipsesResizeEllipse1MovePtRightCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctEllipse1.setRight( s_rctEllipse1.right() + m_fxOffsHor );

            QRectF rct = s_rctEllipse1;

            if( fabs(s_rctEllipse1.width()) > 0.0 || fabs(s_rctEllipse1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(rct) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtRightCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtRightCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtRightCenter_Release )
        {
            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtRightCenterRelease", pTSGrpEllipsesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rctEllipse1.width() < 0 )
            {
                double fLeft = s_rctEllipse1.left();
                double fRight = s_rctEllipse1.right();
                s_rctEllipse1.setLeft(fRight);
                s_rctEllipse1.setRight(fLeft);
            }
            if( s_rctEllipse1.height() < 0 )
            {
                double fTop = s_rctEllipse1.top();
                double fBottom = s_rctEllipse1.bottom();
                s_rctEllipse1.setTop(fBottom);
                s_rctEllipse1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtRightCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtLeftCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            s_rctEllipse1OnMousePressEvent = s_rctEllipse1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtLeftCenterSelect", pTSGrpEllipsesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeEllipse1PtLeftCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointLeftCenter) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtLeftCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtLeftCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtLeftCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtLeftCenterStep" + QString::number(m_iTestStepLev4), pTSGrpEllipsesResize );

            m_fxOffsHor = s_arsizeEllipsesResizeEllipse1MovePtLeftCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeEllipsesResizeEllipse1MovePtLeftCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctEllipse1.setLeft( s_rctEllipse1.left() + m_fxOffsHor );

            QRectF rct = s_rctEllipse1;

            if( fabs(s_rctEllipse1.width()) > 0.0 || fabs(s_rctEllipse1.height()) > 0.0 )
            {
                if( rct.width() < 0 )
                {
                    double fLeft = rct.left();
                    double fRight = rct.right();
                    rct.setLeft(fRight);
                    rct.setRight(fLeft);
                }
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(rct) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtLeftCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtLeftCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtLeftCenter_Release )
        {
            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtLeftCenterRelease", pTSGrpEllipsesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rctEllipse1.width() < 0 )
            {
                double fLeft = s_rctEllipse1.left();
                double fRight = s_rctEllipse1.right();
                s_rctEllipse1.setLeft(fRight);
                s_rctEllipse1.setRight(fLeft);
            }
            if( s_rctEllipse1.height() < 0 )
            {
                double fTop = s_rctEllipse1.top();
                double fBottom = s_rctEllipse1.bottom();
                s_rctEllipse1.setTop(fBottom);
                s_rctEllipse1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_MovePtLeftCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_Finished )
        {
            pTSGrpEllipsesResize->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_Resize_Ellipse1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateTo_Ellipse1_ActivateEditModeSelect )
        {
            if( !pTSGrpEllipsesRotateTo->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Ellipses_RotateTo_Ellipse1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpEllipsesRotateTo->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpEllipsesRotateTo );

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

            } // if( pTSGrpEllipsesRotateTo->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateTo_Ellipse1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateTo_Ellipse1_Select )
        {
            m_iTestStepLev4 = -1;
            m_fAngle_degree = 0.0;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            s_rctEllipse1OnMousePressEvent = s_rctEllipse1;

            pTestStep = getTestStep( strCurrentGraphObj, pTSGrpEllipsesRotateTo );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosRotateToEllipse1Select;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
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
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateTo_Ellipse1_Select )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateTo_Ellipse1_MoveSelPtRotateTop_Select )
        {
            m_iTestStepLev4 = -1;
            m_fAngle_degree = 0.0;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            s_rctEllipse1OnMousePressEvent = s_rctEllipse1;

            pTestStep = getTestStep( strCurrentGraphObj + "SelPtRotateTopSelect", pTSGrpEllipsesRotateTo );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosRotateToEllipse1MoveSelPtRotateTopStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointRotateTop) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateTo_Ellipse1_MoveSelPtRotateTop_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_RotateTo_Ellipse1_MoveSelPtRotateTop_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_RotateTo_Ellipse1_MoveSelPtRotateTop_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "RotateToStep" + QString::number(m_iTestStepLev4), pTSGrpEllipsesRotateTo );

            m_fAngle_degree += s_arfEllipsesRotateToEllipse1MoveSelPtRotateTopTestStepsAngles_degree[m_iTestStepLev4];

            QPointF ptCenter;
            QPointF ptSelPtRot = m_pDrawingView->mapFromScene(s_ptDrawingSceneMousePosRotateToEllipse1MoveSelPtRotateTopStart);
            double  fAngle_rad = Math::deg2Rad(m_fAngle_degree);

            CGraphObjEllipse* pGraphObjEllipse = dynamic_cast<CGraphObjEllipse*>(m_pGraphObjSelected);

            if( pGraphObjEllipse != nullptr )
            {
                ptCenter = pGraphObjEllipse->rect().center();
                ptCenter = pGraphObjEllipse->mapToScene(ptCenter);
                ptCenter = m_pDrawingView->mapFromScene(ptCenter);
            }

            ptSelPtRot = rotatePoint( ptCenter, ptSelPtRot, fAngle_rad );

            m_ptDrawingViewMousePos = ptSelPtRot.toPoint();

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgEllipse1 = rotateRect( s_rctEllipse1.center(), s_rctEllipse1, fAngle_rad );
            s_plgEllipse1.append(s_plgEllipse1[0]);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgEllipse1) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".rotateTo( " + QString::number(m_fAngle_degree) + "° )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
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
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);

            bool bIgnoreTestResult = false;

            if( static_cast<int>(m_fAngle_degree) % 90 > 1 )
            {
                bIgnoreTestResult = true;
            }
            pTestStep->testEnded(bIgnoreTestResult);

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_RotateTo_Ellipse1_MoveSelPtRotateTop_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_RotateTo_Ellipse1_MoveSelPtRotateTop_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateTo_Ellipse1_Release )
        {
            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "Release", pTSGrpEllipsesRotateTo );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctEllipse1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctEllipse1) );

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

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateTo_Ellipse1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateTo_Ellipse1_Finished )
        {
            pTSGrpEllipsesMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateTo_Ellipse1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_ActivateEditModeSelect )
        {
            if( !pTSGrpEllipsesRotateMoveResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpEllipsesRotateMoveResize->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpEllipsesRotateMoveResize );

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

            } // if( pTSGrpEllipsesRotateResizeMove->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateResizeMove_Ellipse1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_Start )
        {
            if( !pTSGrpEllipsesRotateMoveResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_Finished+1; // Last rotate to test step
                triggerDoTestStep();
            }
            else // if( pTSGrpEllipsesRotateMoveResize->isEnabled() )
            {
                m_iTestStepLev4 = -1;
                m_fAngle_degree = 0.0;

                m_iTestStepLev3++;  // Next sub test step
                triggerDoTestStep();

                s_plgEllipse1 = s_rctEllipse1;

            } // if( pTSGrpEllipsesRotateMoveResize->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_Start )

        else if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Ellipse" + QString::number(m_iEllipsesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "RotateMoveResizeStep" + QString::number(m_iTestStepLev4), pTSGrpEllipsesRotateMoveResize );

            SRotateMoveResizeOperation op = s_arRotateMoveResizeEllipse1TestSteps[m_iTestStepLev4];

            if( op.m_ptDrawingSceneMousePos.x() >= 0 && op.m_ptDrawingSceneMousePos.y() >= 0 )
            {
                m_ptDrawingSceneMousePos = op.m_ptDrawingSceneMousePos;
            }

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
                QPolygonF plgEllipse1;
                plgEllipse1.append(op.m_pt0PolygonRectResult);
                plgEllipse1.append(op.m_pt1PolygonRectResult);
                plgEllipse1.append(op.m_pt2PolygonRectResult);
                plgEllipse1.append(op.m_pt3PolygonRectResult);
                plgEllipse1.append(op.m_pt0PolygonRectResult);
                strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(op.m_ptPos) );
                strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(plgEllipse1) );
            }

            pTestStep->setDesiredValues(strlstDesiredValues);
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
                strlstActualValues.append( strCurrentGraphObj + ": " + point2Str(arpGraphObjsSelected[0]->pos()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);

            pTestStep->testEnded(op.m_bIgnoreTestResult);

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_Finished )
        {
            pTSGrpEllipsesMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Ellipses_RotateMoveResize_Ellipse1_Finished )

        else // start next test step
        {
            pTSGrpEllipses->testEnded();

            m_iTestStepLev2++; // Next test step
            m_iTestStepLev3 = 0;
            triggerDoTestStep();
        }

    } // if( pTSGrpEllipses->isEnabled() )

    delete pMouseEv;
    pMouseEv = nullptr;

} // doTestStepStandardShapesEllipses

#endif // #if 0
