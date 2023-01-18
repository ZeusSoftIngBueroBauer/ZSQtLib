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
#include "ZSDraw/ZSDrawGraphObjRect.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSDraw/ZSDrawUnits.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"

#if 0

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Products::Draw;


/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

static QRectF    s_rect1;
static QPolygonF s_plgRect1;
static QRectF    s_rect1OnMousePressEvent; // during mouse move events the item's position is not changed

// Create_Rectangle1_MovePtBottomRight
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosCreateRect1Start(100,100);

static const QSize s_arsizeRectanglesCreateRectangle1MovePtBottomRightTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize(   5,   5 ), /* 100, 100, 105, 105,   5,   5 */
    /*  1 */ QSize(   5,   5 ), /* 100, 100, 110, 110,  10,  10 */
    /*  2 */ QSize(   5,   5 ), /* 100, 100, 115, 115,  15,  15 */
    /*  3 */ QSize(  35,  35 ), /* 100, 100, 150, 150,  50,  50 */
    /*  3 */ QSize(  50,  50 )  /* 100, 100, 200, 200, 100, 100 */
};

static const int ETestStepLev3_Rectangles_Create_Rectangle1_MovePtBottomRight_TestStepCount = sizeof(s_arsizeRectanglesCreateRectangle1MovePtBottomRightTestSteps)/sizeof(s_arsizeRectanglesCreateRectangle1MovePtBottomRightTestSteps[0]);

// Move_Rectangle1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosMoveRect1Start(100,125);

static const QSize s_arsizeRectanglesMoveRectangle1TestSteps[] = {
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

static const int ETestStepLev3_Rectangles_Move_Rectangle1_TestStepCount = sizeof(s_arsizeRectanglesMoveRectangle1TestSteps)/sizeof(s_arsizeRectanglesMoveRectangle1TestSteps[0]);

// Resize_Rectangle1_MovePtBottomRight
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeRectangle1Select(150,120);

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtBottomRightStart(198,202); // The bottom right corner should not been hit exactly.

static const QSize s_arsizeRectanglesResizeRectangle1MovePtBottomRightTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize( -23, -27 ), /* 100, 100, 175, 175,  75,  75 */
    /*  1 */ QSize( -25, -25 ), /* 100, 100, 150, 150,  50,  50 */
    /*  2 */ QSize( -25, -25 ), /* 100, 100, 125, 125,  25,  25 */
    /*  3 */ QSize( -25, -25 ), /* 100, 100, 100, 100,   0,   0 */
    /*  4 */ QSize( -25, -25 ), /*  75,  75, 100, 100,  25,  25 */
    /*  5 */ QSize( -25, -25 ), /*  50,  50, 100, 100,  50,  50 */
    /*  6 */ QSize( -25, -25 ), /*  25,  25, 100, 100,  75,  75 */
    /*  7 */ QSize( -25, -25 )  /*   0,   0, 100, 100, 100, 100 */
};

static const int ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomRight_TestStepCount = sizeof(s_arsizeRectanglesResizeRectangle1MovePtBottomRightTestSteps)/sizeof(s_arsizeRectanglesResizeRectangle1MovePtBottomRightTestSteps[0]);

// Resize_Rectangle1_MovePtTopLeft
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtTopLeftStart(0,0);

static const QSize s_arsizeRectanglesResizeRectangle1MovePtTopLeftTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize(  50,  50 ), /*  50,  50, 100, 100,  50,  50 */
    /*  1 */ QSize(  50,  50 ), /* 100, 100, 100, 100,   0,   0 */
    /*  2 */ QSize(  50,  50 ), /* 100, 100, 150, 150,  50,  50 */
    /*  3 */ QSize(  50,  50 )  /* 100, 100, 200, 200, 100, 100 */
};

static const int ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopLeft_TestStepCount = sizeof(s_arsizeRectanglesResizeRectangle1MovePtTopLeftTestSteps)/sizeof(s_arsizeRectanglesResizeRectangle1MovePtTopLeftTestSteps[0]);

// Resize_Rectangle1_MovePtTopRight
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtTopRightStart(200,100);

static const QSize s_arsizeRectanglesResizeRectangle1MovePtTopRightTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize( -50,  50 ), /* 100, 150, 150, 200,  50,  50 */
    /*  1 */ QSize( -50,  50 ), /* 100, 200, 100, 200,   0,   0 */
    /*  2 */ QSize( -50,  50 ), /*  50, 200, 100, 250,  50,  50 */
    /*  3 */ QSize( -50,  50 )  /*   0, 200, 100, 300, 100, 100 */
};

static const int ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopRight_TestStepCount = sizeof(s_arsizeRectanglesResizeRectangle1MovePtTopRightTestSteps)/sizeof(s_arsizeRectanglesResizeRectangle1MovePtTopRightTestSteps[0]);

// Resize_Rectangle1_MovePtBottomLeft
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtBottomLeftStart(0,300);

static const QSize s_arsizeRectanglesResizeRectangle1MovePtBottomLeftTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize(  50, -50 ), /*  50, 200, 100, 250,  50,  50 */
    /*  1 */ QSize(  50, -50 ), /* 100, 200, 100, 200,   0,   0 */
    /*  2 */ QSize(  50, -50 ), /* 100, 150, 150, 200,  50,  50 */
    /*  3 */ QSize(  50, -50 )  /* 100, 100, 200, 200, 100, 100 */
};

static const int ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomLeft_TestStepCount = sizeof(s_arsizeRectanglesResizeRectangle1MovePtBottomLeftTestSteps)/sizeof(s_arsizeRectanglesResizeRectangle1MovePtBottomLeftTestSteps[0]);

// Resize_Rectangle1_MovePtBottomCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtBottomCenterStart(150,200);

static const QSize s_arsizeRectanglesResizeRectangle1MovePtBottomCenterTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize(   0, -50 ), /* 100, 100, 200, 150, 100,  50 */
    /*  1 */ QSize(   0, -50 ), /* 100, 100, 200, 100, 100,   0 */
    /*  2 */ QSize(   0, -50 ), /* 100,  50, 200, 100, 100,  50 */
    /*  3 */ QSize(   0, -50 )  /* 100,   0, 200, 100, 100, 100 */
};

static const int ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomCenter_TestStepCount = sizeof(s_arsizeRectanglesResizeRectangle1MovePtBottomCenterTestSteps)/sizeof(s_arsizeRectanglesResizeRectangle1MovePtBottomCenterTestSteps[0]);

// Resize_Rectangle1_MovePtTopCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtTopCenterStart(150,0);

static const QSize s_arsizeRectanglesResizeRectangle1MovePtTopCenterTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize(   0,  50 ), /* 100,  50, 200, 100, 100,  50 */
    /*  1 */ QSize(   0,  50 ), /* 100, 100, 200, 100, 100,   0 */
    /*  2 */ QSize(   0,  50 ), /* 100, 100, 200, 150, 100,  50 */
    /*  3 */ QSize(   0,  50 )  /* 100, 100, 200, 200, 100, 100 */
};

static const int ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopCenter_TestStepCount = sizeof(s_arsizeRectanglesResizeRectangle1MovePtTopCenterTestSteps)/sizeof(s_arsizeRectanglesResizeRectangle1MovePtTopCenterTestSteps[0]);

// Resize_Rectangle1_MovePtRightCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtRightCenterStart(200,150);

static const QSize s_arsizeRectanglesResizeRectangle1MovePtRightCenterTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize( -50,   0 ), /* 100, 100, 150, 200,  50, 100 */
    /*  1 */ QSize( -50,   0 ), /* 100, 100, 100, 200,   0, 100 */
    /*  2 */ QSize( -50,   0 ), /*  50, 100, 100, 200,  50, 100 */
    /*  3 */ QSize( -50,   0 )  /*   0, 100, 100, 200, 100, 100 */
};

static const int ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtRightCenter_TestStepCount = sizeof(s_arsizeRectanglesResizeRectangle1MovePtRightCenterTestSteps)/sizeof(s_arsizeRectanglesResizeRectangle1MovePtRightCenterTestSteps[0]);

// Resize_Rectangle1_MovePtLeftCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtLeftCenterStart(0,150);

static const QSize s_arsizeRectanglesResizeRectangle1MovePtLeftCenterTestSteps[] = {
                      /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ QSize(  50,   0 ), /*  50, 100, 100, 200,  50, 100 */
    /*  1 */ QSize(  50,   0 ), /* 100, 100, 100, 200,   0, 100 */
    /*  2 */ QSize(  50,   0 ), /* 100, 100, 150, 200,  50, 100 */
    /*  3 */ QSize(  50,   0 )  /* 100, 100, 200, 200, 100, 100 */
};

static const int ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtLeftCenter_TestStepCount = sizeof(s_arsizeRectanglesResizeRectangle1MovePtLeftCenterTestSteps)/sizeof(s_arsizeRectanglesResizeRectangle1MovePtLeftCenterTestSteps[0]);

// RotateTo_Rectangle1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosRotateToRect1Select(120,150);

static const QPoint s_ptDrawingSceneMousePosRotateToRect1MoveSelPtRotateTopStart(150,80);

static const double s_arfRectanglesRotateToRectangle1MoveSelPtRotateTopTestStepsAngles_degree[] = {
         /* RectCoors:  l,   t,   r,   b,   w,   h */
    /*  0 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /*  1 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /*  2 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /*  3 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /*  4 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /*  5 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /*  6 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /*  7 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /*  8 */ 10.0, /* 100, 100, 200, 200, 100, 100 */ //  90°
    /*  9 */ 30.0, /*    ,    ,    ,    , 100, 100 */
    /* 10 */ 30.0, /*    ,    ,    ,    , 100, 100 */
    /* 11 */ 30.0, /* 100, 100, 200, 200, 100, 100 */ // 180°
    /* 12 */ 45.0, /*    ,    ,    ,    , 100, 100 */
    /* 13 */ 45.0, /* 100, 100, 200, 200, 100, 100 */ // 270°
    /* 14 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /* 15 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /* 16 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /* 17 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /* 18 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /* 19 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /* 20 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /* 21 */ 10.0, /*    ,    ,    ,    , 100, 100 */
    /* 22 */ 10.0  /* 100, 100, 200, 200, 100, 100 */ // 360°
};

static const int ETestStepLev3_Rectangles_RotateTo_Rectangle1_MoveSelPtRotateTop_TestStepCount = sizeof(s_arfRectanglesRotateToRectangle1MoveSelPtRotateTopTestStepsAngles_degree)/sizeof(s_arfRectanglesRotateToRectangle1MoveSelPtRotateTopTestStepsAngles_degree[0]);

// RotateMoveResize_Rectangle1
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

static const SRotateMoveResizeOperation s_arRotateMoveResizeRectangle1TestSteps[] = {
          // {        MousePos,       RectCenterPos,               MouseEvent,         Operation,                  Pos,       PolygonRect[0],       PolygonRect[1],       PolygonRect[2],       PolygonRect[3], Ignore }
    /*  0 */ { QPoint(150,120), QPoint(150.0,150.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(100.0,100.0), QPointF(100.0,100.0), QPointF(200.0,100.0), QPointF(200.0,200.0), QPointF(100.0,200.0), false  }, // Select
    /*  1 */ { QPoint( -1, -1), QPoint(150.0,150.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(100.0,100.0), QPointF(100.0,100.0), QPointF(200.0,100.0), QPointF(200.0,200.0), QPointF(100.0,200.0), false  },
    /*  2 */ { QPoint(150, 80), QPoint(150.0,150.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(100.0,100.0), QPointF(100.0,100.0), QPointF(200.0,100.0), QPointF(200.0,200.0), QPointF(100.0,200.0), false  }, // Select SelPt RotateTop
    /*  3 */ { QPoint( 80,150), QPoint(150.0,150.0), QEvent::MouseMove,          EEditModeRotate, QPointF(100.0,100.0), QPointF(100.0,200.0), QPointF(100.0,100.0), QPointF(200.0,100.0), QPointF(200.0,200.0), false  }, // Rotate by +90° to 90°
    /*  4 */ { QPoint( -1, -1), QPoint(150.0,150.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(100.0,100.0), QPointF(100.0,200.0), QPointF(100.0,100.0), QPointF(200.0,100.0), QPointF(200.0,200.0), false  },
    /*  5 */ { QPoint(105,105), QPoint(150.0,150.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(100.0,100.0), QPointF(100.0,200.0), QPointF(100.0,100.0), QPointF(200.0,100.0), QPointF(200.0,200.0), false  }, // Select
    /*  6 */ { QPoint(205,205), QPoint(250.0,250.0), QEvent::MouseMove,          EEditModeMove,   QPointF(200.0,200.0), QPointF(200.0,300.0), QPointF(200.0,200.0), QPointF(300.0,200.0), QPointF(300.0,300.0), false  }, // Move by 100/100
    /*  7 */ { QPoint( -1, -1), QPoint(250.0,250.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(200.0,200.0), QPointF(200.0,300.0), QPointF(200.0,200.0), QPointF(300.0,200.0), QPointF(300.0,300.0), false  },
    /*  8 */ { QPoint(200,250), QPoint(250.0,250.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(200.0,200.0), QPointF(200.0,300.0), QPointF(200.0,200.0), QPointF(300.0,200.0), QPointF(300.0,300.0), false  }, // Select SelPt TopCenter
    /*  9 */ { QPoint(210,250), QPoint(255.0,250.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,200.0), QPointF(210.0,300.0), QPointF(210.0,200.0), QPointF(300.0,200.0), QPointF(300.0,300.0), false  }, // ResizeBy 10/0 (decrease height by 10)
    /* 10 */ { QPoint( -1, -1), QPoint(255.0,250.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(205.0,205.0), QPointF(210.0,300.0), QPointF(210.0,200.0), QPointF(300.0,200.0), QPointF(300.0,300.0), false  },
    /* 11 */ { QPoint(190,250), QPoint(255.0,250.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(205.0,205.0), QPointF(210.0,300.0), QPointF(210.0,200.0), QPointF(300.0,200.0), QPointF(300.0,300.0), false  }, // Select SelPt RotateTop
    /* 12 */ { QPoint(255,180), QPoint(250.0,255.0), QEvent::MouseMove,          EEditModeRotate, QPointF(205.0,205.0), QPointF(205.0,205.0), QPointF(305.0,205.0), QPointF(305.0,295.0), QPointF(205.0,295.0), false  }, // Rotate by -90° to 0°
    /* 13 */ { QPoint( -1, -1), QPoint(250.0,255.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(205.0,205.0), QPointF(205.0,205.0), QPointF(305.0,205.0), QPointF(305.0,295.0), QPointF(205.0,295.0), false  },
    /* 14 */ { QPoint(255,205), QPoint(255.0,250.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(205.0,205.0), QPointF(205.0,205.0), QPointF(305.0,205.0), QPointF(305.0,295.0), QPointF(205.0,295.0), false  }, // Select SelPt TopCenter
    /* 15 */ { QPoint(255,200), QPoint(255.0,247.5), QEvent::MouseMove,          EEditModeResize, QPointF(205.0,205.0), QPointF(205.0,200.0), QPointF(305.0,200.0), QPointF(305.0,295.0), QPointF(205.0,295.0), false  }, // ResizeBy 0/5 (increase height by 5)
    /* 16 */ { QPoint( -1, -1), QPoint(255.0,247.5), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(205.0,200.0), QPointF(205.0,200.0), QPointF(305.0,200.0), QPointF(305.0,295.0), QPointF(205.0,295.0), false  },
    /* 17 */ { QPoint(255,295), QPoint(255.0,247.5), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(205.0,200.0), QPointF(205.0,200.0), QPointF(305.0,200.0), QPointF(305.0,295.0), QPointF(205.0,295.0), false  }, // Select SelPt BottomCenter
    /* 18 */ { QPoint(255,300), QPoint(255.0,250.0), QEvent::MouseMove,          EEditModeResize, QPointF(205.0,200.0), QPointF(205.0,200.0), QPointF(305.0,200.0), QPointF(305.0,300.0), QPointF(205.0,300.0), false  }, // ResizeBy 0/5 (increase height by 5)
    /* 19 */ { QPoint( -1, -1), QPoint(255.0,250.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(205.0,200.0), QPointF(205.0,200.0), QPointF(305.0,200.0), QPointF(305.0,300.0), QPointF(205.0,300.0), false  },
    /* 20 */ { QPoint(210,205), QPoint(255.0,247.5), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(205.0,200.0), QPointF(205.0,200.0), QPointF(305.0,200.0), QPointF(305.0,300.0), QPointF(205.0,300.0), false  }, // Select
    /* 21 */ { QPoint(205,205), QPoint(250.0,250.0), QEvent::MouseMove,          EEditModeMove,   QPointF(200.0,200.0), QPointF(200.0,200.0), QPointF(300.0,200.0), QPointF(300.0,300.0), QPointF(200.0,300.0), false  }, // Move by -5/0
    /* 22 */ { QPoint( -1, -1), QPoint(250.0,250.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(200.0,200.0), QPointF(200.0,200.0), QPointF(300.0,200.0), QPointF(300.0,300.0), QPointF(200.0,300.0), false  },
    /* 23 */ { QPoint(250,180), QPoint(250.0,250.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(200.0,200.0), QPointF(200.0,200.0), QPointF(300.0,200.0), QPointF(300.0,300.0), QPointF(200.0,300.0), false  }, // Select SelPt TopCenter
    /* 24 */ { QPoint(180,180), QPoint(250.0,250.0), QEvent::MouseMove,          EEditModeRotate, QPointF(200.0,200.0), QPointF(179.3,250.0), QPointF(250.0,179.3), QPointF(320.7,250.0), QPointF(250.0,320.7), false  }, // Rotate by 45° to 45°
    /* 25 */ { QPoint( -1, -1), QPoint(250.0,250.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(200.0,200.0), QPointF(179.3,250.0), QPointF(250.0,179.3), QPointF(320.7,250.0), QPointF(250.0,320.7), false  },
    /* 26 */ { QPoint(320,250), QPoint(250.0,250.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(200.0,200.0), QPointF(179.3,250.0), QPointF(250.0,179.3), QPointF(320.7,250.0), QPointF(250.0,320.7), false  }, // Select SelPt BottomRight
    /* 27 */ { QPoint(310,260), QPoint(244.6,255.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,200.0), QPointF(179.3,250.0), QPointF(239.6,189.6), QPointF(310.0,260.0), QPointF(249.6,320.4), false  }, // ResizeBy / (decrease width by  to )
    /* 28 */ { QPoint(300,270), QPoint(239.6,260.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,200.0), QPointF(179.3,250.0), QPointF(229.6,199.6), QPointF(300.0,270.0), QPointF(249.6,320.4), false  }, // ResizeBy / (decrease width by  to )
    /* 29 */ { QPoint(290,280), QPoint(234.6,265.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,200.0), QPointF(179.3,250.0), QPointF(219.6,209.6), QPointF(290.0,280.0), QPointF(249.6,320.4), false  }, // ResizeBy / (decrease width by  to )
    /* 30 */ { QPoint(280,290), QPoint(229.6,270.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,200.0), QPointF(179.3,250.0), QPointF(209.6,219.6), QPointF(280.0,290.0), QPointF(249.6,320.4), false  }, // ResizeBy / (decrease width by  to )
    /* 31 */ { QPoint(270,300), QPoint(224.6,275.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,200.0), QPointF(179.3,250.0), QPointF(199.6,229.6), QPointF(270.0,300.0), QPointF(249.6,320.4), false  }, // ResizeBy / (decrease width by  to )
    /* 32 */ { QPoint(260,310), QPoint(219.6,280.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,200.0), QPointF(179.3,250.0), QPointF(189.6,239.6), QPointF(260.0,310.0), QPointF(249.6,320.4), false  }, // ResizeBy / (decrease width by  to )
    /* 33 */ { QPoint(250,320), QPoint(214.6,285.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,200.0), QPointF(179.3,250.0), QPointF(179.6,249.6), QPointF(250.0,320.0), QPointF(249.6,320.4), false  }, // ResizeBy / (decrease width by  to 0)
    /* 34 */ { QPoint(240,330), QPoint(209.6,290.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,200.0), QPointF(169.6,259.6), QPointF(179.3,250.0), QPointF(249.6,320.4), QPointF(240.0,330.0), false  }, // ResizeBy / (increase width by  to )
    /* 35 */ { QPoint(230,340), QPoint(204.6,295.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,200.0), QPointF(159.6,269.6), QPointF(179.3,250.0), QPointF(249.6,320.4), QPointF(230.0,340.0), false  }, // ResizeBy / (increase width by  to )
    /* 36 */ { QPoint(220,350), QPoint(199.6,300.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,200.0), QPointF(149.6,279.6), QPointF(179.3,250.0), QPointF(249.6,320.4), QPointF(220.0,350.0), false  }, // ResizeBy / (increase width by  to )
    /* 37 */ { QPoint(210,360), QPoint(194.6,305.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,200.0), QPointF(139.6,289.6), QPointF(179.3,250.0), QPointF(249.6,320.4), QPointF(210.0,360.0), false  }, // ResizeBy / (increase width by  to )
    /* 38 */ { QPoint(200,370), QPoint(189.6,310.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,200.0), QPointF(129.6,299.6), QPointF(179.3,250.0), QPointF(249.6,320.4), QPointF(200.0,370.0), false  }, // ResizeBy / (increase width by  to )
    /* 39 */ { QPoint(190,380), QPoint(184.6,315.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,200.0), QPointF(119.6,309.6), QPointF(179.3,250.0), QPointF(249.6,320.4), QPointF(190.0,380.0), false  }, // ResizeBy / (increase width by  to )
    /* 40 */ { QPoint(180,390), QPoint(179.6,320.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,200.0), QPointF(109.6,319.6), QPointF(179.3,250.0), QPointF(249.6,320.4), QPointF(180.0,390.0), false  }, // ResizeBy / (increase width by  to 100)
    /* 41 */ { QPoint( -1, -1), QPoint(179.6,320.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(130.4,270.3), QPointF(109.6,319.6), QPointF(179.3,250.0), QPointF(249.6,320.4), QPointF(180.0,390.0), false  },
    /* 42 */ { QPoint(180,320), QPoint(179.6,320.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(130.4,270.3), QPointF(109.6,319.6), QPointF(179.3,250.0), QPointF(249.6,320.4), QPointF(180.0,390.0), false  }, // Select
    /* 43 */ { QPoint(250,250), QPoint(249.6,250.0), QEvent::MouseMove,          EEditModeMove,   QPointF(200.4,200.3), QPointF(179.6,249.6), QPointF(249.3,180.0), QPointF(319.6,250.4), QPointF(250.0,320.0), false  }, // Move by 70/70
    /* 44 */ { QPoint( -1, -1), QPoint(249.6,250.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(200.4,200.3), QPointF(179.6,249.6), QPointF(249.3,180.0), QPointF(319.6,250.4), QPointF(250.0,320.0), false  },
    /* 45 */ { QPoint(201,201), QPoint(179.6,320.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(200.4,200.3), QPointF(179.6,249.6), QPointF(249.3,180.0), QPointF(319.6,250.4), QPointF(250.0,320.0), false  }, // Select SelPt TopCenter
    /* 46 */ { QPoint(250,180), QPoint(249.6,250.0), QEvent::MouseMove,          EEditModeRotate, QPointF(200.4,200.3), QPointF(200.4,200.3), QPointF(298.9,200.3), QPointF(298.9,299.7), QPointF(200.4,299.7), false  }, // Rotate by -45° to 0°
    /* 47 */ { QPoint( -1, -1), QPoint(249.6,250.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(200.4,200.3), QPointF(200.4,200.3), QPointF(298.9,200.3), QPointF(298.9,299.7), QPointF(200.4,299.7), false  },
    /* 48 */ { QPoint(250,225), QPoint(249.6,250.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(200.4,200.3), QPointF(200.4,200.3), QPointF(298.9,200.3), QPointF(298.9,299.7), QPointF(200.4,299.7), false  }, // Select
    /* 49 */ { QPoint(150,125), QPoint(149.6,150.0), QEvent::MouseMove,          EEditModeMove,   QPointF(100.4,100.3), QPointF(100.4,100.3), QPointF(198.9,100.3), QPointF(198.9,199.7), QPointF(100.4,199.7), false  }, // Move by -100,-100
    /* 50 */ { QPoint( -1, -1), QPoint(149.6,150.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(100.4,100.3), QPointF(100.4,100.3), QPointF(198.9,100.3), QPointF(198.9,199.7), QPointF(100.4,199.7), false  }
};

static const int ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_TestStepCount = sizeof(s_arRotateMoveResizeRectangle1TestSteps)/sizeof(s_arRotateMoveResizeRectangle1TestSteps[0]);


// Test steps:
//------------------------------------------------------------------------------

enum
{
    ETestStepLev3_Rectangles_Create_Rectangle1_ActivateDrawingTool,
    ETestStepLev3_Rectangles_Create_Rectangle1,
    ETestStepLev3_Rectangles_Create_Rectangle1_MovePtBottomRight_TestStepMin,
    ETestStepLev3_Rectangles_Create_Rectangle1_MovePtBottomRight_TestStepMax = ETestStepLev3_Rectangles_Create_Rectangle1_MovePtBottomRight_TestStepMin + ETestStepLev3_Rectangles_Create_Rectangle1_MovePtBottomRight_TestStepCount - 1,
    ETestStepLev3_Rectangles_Create_Rectangle1_Release,
    ETestStepLev3_Rectangles_Create_Rectangle1_Finished,
    ETestStepLev3_Rectangles_Move_Rectangle1_ActivateEditModeSelect,
    ETestStepLev3_Rectangles_Move_Rectangle1_Select,
    ETestStepLev3_Rectangles_Move_Rectangle1_TestStepMin,
    ETestStepLev3_Rectangles_Move_Rectangle1_TestStepMax = ETestStepLev3_Rectangles_Move_Rectangle1_TestStepMin + ETestStepLev3_Rectangles_Move_Rectangle1_TestStepCount - 1,
    ETestStepLev3_Rectangles_Move_Rectangle1_Release,
    ETestStepLev3_Rectangles_Move_Rectangle1_Finished,
    ETestStepLev3_Rectangles_Resize_Rectangle1_ActivateEditModeSelect,
    ETestStepLev3_Rectangles_Resize_Rectangle1_Select,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomRight_Select,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomRight_TestStepMin,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomRight_TestStepMax = ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomRight_TestStepMin + ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomRight_TestStepCount - 1,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomRight_Release,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopLeft_Select,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopLeft_TestStepMin,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopLeft_TestStepMax = ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopLeft_TestStepMin + ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopLeft_TestStepCount - 1,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopLeft_Release,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopRight_Select,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopRight_TestStepMin,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopRight_TestStepMax = ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopRight_TestStepMin + ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopRight_TestStepCount - 1,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopRight_Release,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomLeft_Select,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomLeft_TestStepMin,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomLeft_TestStepMax = ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomLeft_TestStepMin + ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomLeft_TestStepCount - 1,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomLeft_Release,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomCenter_Select,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomCenter_TestStepMin,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomCenter_TestStepMax = ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomCenter_TestStepMin + ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomCenter_TestStepCount - 1,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomCenter_Release,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopCenter_Select,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopCenter_TestStepMin,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopCenter_TestStepMax = ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopCenter_TestStepMin + ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopCenter_TestStepCount - 1,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopCenter_Release,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtRightCenter_Select,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtRightCenter_TestStepMin,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtRightCenter_TestStepMax = ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtRightCenter_TestStepMin + ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtRightCenter_TestStepCount - 1,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtRightCenter_Release,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtLeftCenter_Select,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtLeftCenter_TestStepMin,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtLeftCenter_TestStepMax = ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtLeftCenter_TestStepMin + ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtLeftCenter_TestStepCount - 1,
    ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtLeftCenter_Release,
    ETestStepLev3_Rectangles_Resize_Rectangle1_Finished,
    ETestStepLev3_Rectangles_RotateTo_Rectangle1_ActivateEditModeSelect,
    ETestStepLev3_Rectangles_RotateTo_Rectangle1_Select,
    ETestStepLev3_Rectangles_RotateTo_Rectangle1_MoveSelPtRotateTop_Select,
    ETestStepLev3_Rectangles_RotateTo_Rectangle1_MoveSelPtRotateTop_TestStepMin,
    ETestStepLev3_Rectangles_RotateTo_Rectangle1_MoveSelPtRotateTop_TestStepMax = ETestStepLev3_Rectangles_RotateTo_Rectangle1_MoveSelPtRotateTop_TestStepMin + ETestStepLev3_Rectangles_RotateTo_Rectangle1_MoveSelPtRotateTop_TestStepCount - 1,
    ETestStepLev3_Rectangles_RotateTo_Rectangle1_Release,
    ETestStepLev3_Rectangles_RotateTo_Rectangle1_Finished,
    ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_ActivateEditModeSelect,
    ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_Start,
    ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_TestStepMin,
    ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_TestStepMax = ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_TestStepMin + ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_TestStepCount - 1,
    ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_Finished,
    ETestStepLev3_Rectangles_Count
};

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepStandardShapesRectangles( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
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

    ZS::Test::CTestStepGroup* pTSGrpRectangles                 = getTestStepGroup( "Rectangles", i_pTestStepGrpParent );
    ZS::Test::CTestStepGroup* pTSGrpRectanglesCreate           = getTestStepGroup( "Create", pTSGrpRectangles );
    ZS::Test::CTestStepGroup* pTSGrpRectanglesMove             = getTestStepGroup( "Move", pTSGrpRectangles );
    ZS::Test::CTestStepGroup* pTSGrpRectanglesResize           = getTestStepGroup( "Resize", pTSGrpRectangles );
    ZS::Test::CTestStepGroup* pTSGrpRectanglesRotateTo         = getTestStepGroup( "RotateTo", pTSGrpRectangles );
    ZS::Test::CTestStepGroup* pTSGrpRectanglesRotateMoveResize = getTestStepGroup( "RotateMoveResize", pTSGrpRectangles );

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }

    if( !pTSGrpRectangles->isEnabled() )
    {
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else if( !pTSGrpRectanglesCreate->isEnabled() )
    {
        // If the rectangle is not created none of the other sub tests may be performed.
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else // if( pTSGrpRectangles->isEnabled() )
    {
        if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Create_Rectangle1_ActivateDrawingTool )
        {
            if( !pTSGrpRectanglesCreate->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Rectangles_Create_Rectangle1_Finished+1; // Last create test step
                triggerDoTestStep();
            }
            else // if( pTSGrpRectanglesCreate->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateDrawingTool", pTSGrpRectanglesCreate );

                strlstDesiredValues.append( "EditTool: CreateObjects" );
                strlstDesiredValues.append( "DrawingTool: Rect" );

                pTestStep->setOperation( "drawing.setCurrentDrawingTool( Rect )" );
                pTestStep->setDesiredValues(strlstDesiredValues);
                pTestStep->testStarted();

                if( (m_pDrawingScene->getEditTool() != EEditToolCreateObjects) || (m_pDrawingScene->getCurrentDrawingToolGraphObjType() != EGraphObjTypeRect) )
                {
                    m_pMainWindow->setCheckedActionDrawStandardShapeRect(true);
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

            } // if( pTSGrpRectanglesCreate->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Create_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Create_Rectangle1 )
        {
            m_iRectanglesCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj, pTSGrpRectanglesCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateRect1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rect1 = QRectF( s_ptDrawingSceneMousePosCreateRect1Start.x(), s_ptDrawingSceneMousePosCreateRect1Start.y(), 1, 1 );

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".create( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Create_Rectangle1 )

        else if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Create_Rectangle1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Create_Rectangle1_MovePtBottomRight_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightStep" + QString::number(m_iTestStepLev4), pTSGrpRectanglesCreate );

            m_fxOffsHor = s_arsizeRectanglesCreateRectangle1MovePtBottomRightTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeRectanglesCreateRectangle1MovePtBottomRightTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rect1.setRight(m_ptDrawingSceneMousePos.x());
            s_rect1.setBottom(m_ptDrawingSceneMousePos.y());

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Create_Rectangle1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Create_Rectangle1_MovePtBottomRight_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Create_Rectangle1_Release )
        {
            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "Release", pTSGrpRectanglesCreate );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Create_Rectangle1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Create_Rectangle1_Finished )
        {
            pTSGrpRectanglesCreate->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Create_Rectangle1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Move_Rectangle1_ActivateEditModeSelect )
        {
            if( !pTSGrpRectanglesMove->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Rectangles_Move_Rectangle1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpRectanglesMove->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpRectanglesMove );

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

            } // if( pTSGrpRectanglesMove->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Move_Rectangle1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Move_Rectangle1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            s_rect1OnMousePressEvent = s_rect1;

            pTestStep = getTestStep( strCurrentGraphObj, pTSGrpRectanglesMove );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosMoveRect1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Move_Rectangle1_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Move_Rectangle1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Move_Rectangle1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MoveStep" + QString::number(m_iTestStepLev4), pTSGrpRectanglesMove );

            m_fxOffsHor = s_arsizeRectanglesMoveRectangle1TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeRectanglesMoveRectangle1TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rect1.moveLeft( s_rect1.left() + m_fxOffsHor );
            s_rect1.moveTop( s_rect1.top() + m_fyOffsHor );

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Move_Rectangle1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Move_Rectangle1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Move_Rectangle1_Release )
        {
            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "Release", pTSGrpRectanglesMove );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Move_Rectangle1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Move_Rectangle1_Finished )
        {
            pTSGrpRectanglesMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Move_Rectangle1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_ActivateEditModeSelect )
        {
            if( !pTSGrpRectanglesResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Rectangles_Resize_Rectangle1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpRectanglesResize->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpRectanglesResize );

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

            } // if( pTSGrpRectanglesResize->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            s_rect1OnMousePressEvent = s_rect1;

            pTestStep = getTestStep( strCurrentGraphObj + "Select", pTSGrpRectanglesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRectangle1Select;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_Select )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomRight_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            s_rect1OnMousePressEvent = s_rect1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightSelect", pTSGrpRectanglesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtBottomRightStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointBottomRight) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomRight_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomRight_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightStep" + QString::number(m_iTestStepLev4), pTSGrpRectanglesResize );

            m_fxOffsHor = s_arsizeRectanglesResizeRectangle1MovePtBottomRightTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeRectanglesResizeRectangle1MovePtBottomRightTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rect1.setRight(m_ptDrawingSceneMousePos.x());
            s_rect1.setBottom(m_ptDrawingSceneMousePos.y());

            // Keep old size before resizing starts as the object may get 0 pixel size.
            // The size will be adjusted on releasing the mouse.
            QRectF rct = s_rect1;

            if( fabs(s_rect1.width()) > 0.0 || fabs(s_rect1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomRight_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomRight_Release )
        {
            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightRelease", pTSGrpRectanglesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            // Old size before resizing starts has been kept as the object may get 0 pixel size.
            // The size will be adjusted on releasing the mouse.
            if( s_rect1.width() < 0 )
            {
                double fLeft = s_rect1.left();
                double fRight = s_rect1.right();
                s_rect1.setLeft(fRight);
                s_rect1.setRight(fLeft);
            }
            if( s_rect1.height() < 0 )
            {
                double fTop = s_rect1.top();
                double fBottom = s_rect1.bottom();
                s_rect1.setTop(fBottom);
                s_rect1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomRight_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopLeft_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            s_rect1OnMousePressEvent = s_rect1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopLeftSelect", pTSGrpRectanglesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtTopLeftStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointTopLeft) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopLeft_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopLeft_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopLeftStep" + QString::number(m_iTestStepLev4), pTSGrpRectanglesResize );

            m_fxOffsHor = s_arsizeRectanglesResizeRectangle1MovePtTopLeftTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeRectanglesResizeRectangle1MovePtTopLeftTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rect1.setLeft( s_rect1.left() + m_fxOffsHor );
            s_rect1.setTop( s_rect1.top() + m_fyOffsHor );

            QRectF rct = s_rect1;

            if( fabs(s_rect1.width()) > 0.0 || fabs(s_rect1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopLeft_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopLeft_Release )
        {
            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopLeftRelease", pTSGrpRectanglesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rect1.width() < 0 )
            {
                double fLeft = s_rect1.left();
                double fRight = s_rect1.right();
                s_rect1.setLeft(fRight);
                s_rect1.setRight(fLeft);
            }
            if( s_rect1.height() < 0 )
            {
                double fTop = s_rect1.top();
                double fBottom = s_rect1.bottom();
                s_rect1.setTop(fBottom);
                s_rect1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopLeft_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopRight_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            s_rect1OnMousePressEvent = s_rect1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopRightSelect", pTSGrpRectanglesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtTopRightStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointTopRight) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopRight_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopRight_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopRightStep" + QString::number(m_iTestStepLev4), pTSGrpRectanglesResize );

            m_fxOffsHor = s_arsizeRectanglesResizeRectangle1MovePtTopRightTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeRectanglesResizeRectangle1MovePtTopRightTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rect1.setRight( s_rect1.right() + m_fxOffsHor );
            s_rect1.setTop( s_rect1.top() + m_fyOffsHor );

            QRectF rct = s_rect1;

            if( fabs(s_rect1.width()) > 0.0 || fabs(s_rect1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopRight_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopRight_Release )
        {
            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopRightRelease", pTSGrpRectanglesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rect1.width() < 0 )
            {
                double fLeft = s_rect1.left();
                double fRight = s_rect1.right();
                s_rect1.setLeft(fRight);
                s_rect1.setRight(fLeft);
            }
            if( s_rect1.height() < 0 )
            {
                double fTop = s_rect1.top();
                double fBottom = s_rect1.bottom();
                s_rect1.setTop(fBottom);
                s_rect1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopRight_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomLeft_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            s_rect1OnMousePressEvent = s_rect1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomLeftSelect", pTSGrpRectanglesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtBottomLeftStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointBottomLeft) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomLeft_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomLeft_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomLeftStep" + QString::number(m_iTestStepLev4), pTSGrpRectanglesResize );

            m_fxOffsHor = s_arsizeRectanglesResizeRectangle1MovePtBottomLeftTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeRectanglesResizeRectangle1MovePtBottomLeftTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rect1.setLeft( s_rect1.left() + m_fxOffsHor );
            s_rect1.setBottom( s_rect1.bottom() + m_fyOffsHor );

            QRectF rct = s_rect1;

            if( fabs(s_rect1.width()) > 0.0 || fabs(s_rect1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomLeft_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomLeft_Release )
        {
            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomLeftRelease", pTSGrpRectanglesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rect1.width() < 0 )
            {
                double fLeft = s_rect1.left();
                double fRight = s_rect1.right();
                s_rect1.setLeft(fRight);
                s_rect1.setRight(fLeft);
            }
            if( s_rect1.height() < 0 )
            {
                double fTop = s_rect1.top();
                double fBottom = s_rect1.bottom();
                s_rect1.setTop(fBottom);
                s_rect1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomLeft_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            s_rect1OnMousePressEvent = s_rect1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomCenterSelect", pTSGrpRectanglesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtBottomCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointBottomCenter) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomCenterStep" + QString::number(m_iTestStepLev4), pTSGrpRectanglesResize );

            m_fxOffsHor = s_arsizeRectanglesResizeRectangle1MovePtBottomCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeRectanglesResizeRectangle1MovePtBottomCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rect1.setBottom( s_rect1.bottom() + m_fyOffsHor );

            QRectF rct = s_rect1;

            if( fabs(s_rect1.width()) > 0.0 || fabs(s_rect1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomCenter_Release )
        {
            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomCenterRelease", pTSGrpRectanglesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rect1.width() < 0 )
            {
                double fLeft = s_rect1.left();
                double fRight = s_rect1.right();
                s_rect1.setLeft(fRight);
                s_rect1.setRight(fLeft);
            }

            if( s_rect1.height() < 0 )
            {
                double fTop = s_rect1.top();
                double fBottom = s_rect1.bottom();
                s_rect1.setTop(fBottom);
                s_rect1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtBottomCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            s_rect1OnMousePressEvent = s_rect1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopCenterSelect", pTSGrpRectanglesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtTopCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointTopCenter) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopCenterStep" + QString::number(m_iTestStepLev4), pTSGrpRectanglesResize );

            m_fxOffsHor = s_arsizeRectanglesResizeRectangle1MovePtTopCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeRectanglesResizeRectangle1MovePtTopCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rect1.setTop( s_rect1.top() + m_fyOffsHor );

            QRectF rct = s_rect1;

            if( fabs(s_rect1.width()) > 0.0 || fabs(s_rect1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopCenter_Release )
        {
            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopCenterRelease", pTSGrpRectanglesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rect1.width() < 0 )
            {
                double fLeft = s_rect1.left();
                double fRight = s_rect1.right();
                s_rect1.setLeft(fRight);
                s_rect1.setRight(fLeft);
            }

            if( s_rect1.height() < 0 )
            {
                double fTop = s_rect1.top();
                double fBottom = s_rect1.bottom();
                s_rect1.setTop(fBottom);
                s_rect1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtTopCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtRightCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            s_rect1OnMousePressEvent = s_rect1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtRightCenterSelect", pTSGrpRectanglesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtRightCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointRightCenter) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtRightCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtRightCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtRightCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtRightCenterStep" + QString::number(m_iTestStepLev4), pTSGrpRectanglesResize );

            m_fxOffsHor = s_arsizeRectanglesResizeRectangle1MovePtRightCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeRectanglesResizeRectangle1MovePtRightCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rect1.setRight( s_rect1.right() + m_fxOffsHor );

            QRectF rct = s_rect1;

            if( fabs(s_rect1.width()) > 0.0 || fabs(s_rect1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtRightCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtRightCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtRightCenter_Release )
        {
            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtRightCenterRelease", pTSGrpRectanglesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rect1.width() < 0 )
            {
                double fLeft = s_rect1.left();
                double fRight = s_rect1.right();
                s_rect1.setLeft(fRight);
                s_rect1.setRight(fLeft);
            }
            if( s_rect1.height() < 0 )
            {
                double fTop = s_rect1.top();
                double fBottom = s_rect1.bottom();
                s_rect1.setTop(fBottom);
                s_rect1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtRightCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtLeftCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            s_rect1OnMousePressEvent = s_rect1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtLeftCenterSelect", pTSGrpRectanglesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtLeftCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointLeftCenter) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtLeftCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtLeftCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtLeftCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtLeftCenterStep" + QString::number(m_iTestStepLev4), pTSGrpRectanglesResize );

            m_fxOffsHor = s_arsizeRectanglesResizeRectangle1MovePtLeftCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeRectanglesResizeRectangle1MovePtLeftCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rect1.setLeft( s_rect1.left() + m_fxOffsHor );

            QRectF rct = s_rect1;

            if( fabs(s_rect1.width()) > 0.0 || fabs(s_rect1.height()) > 0.0 )
            {
                if( rct.width() < 0 )
                {
                    double fLeft = rct.left();
                    double fRight = rct.right();
                    rct.setLeft(fRight);
                    rct.setRight(fLeft);
                }
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtLeftCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtLeftCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtLeftCenter_Release )
        {
            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtLeftCenterRelease", pTSGrpRectanglesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rect1.width() < 0 )
            {
                double fLeft = s_rect1.left();
                double fRight = s_rect1.right();
                s_rect1.setLeft(fRight);
                s_rect1.setRight(fLeft);
            }
            if( s_rect1.height() < 0 )
            {
                double fTop = s_rect1.top();
                double fBottom = s_rect1.bottom();
                s_rect1.setTop(fBottom);
                s_rect1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_MovePtLeftCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_Finished )
        {
            pTSGrpRectanglesResize->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_Resize_Rectangle1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateTo_Rectangle1_ActivateEditModeSelect )
        {
            if( !pTSGrpRectanglesRotateTo->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Rectangles_RotateTo_Rectangle1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpRectanglesRotateTo->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpRectanglesRotateTo );

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

            } // if( pTSGrpRectanglesRotateTo->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateTo_Rectangle1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateTo_Rectangle1_Select )
        {
            m_iTestStepLev4 = -1;
            m_fAngle_degree = 0.0;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            s_rect1OnMousePressEvent = s_rect1;

            pTestStep = getTestStep( strCurrentGraphObj, pTSGrpRectanglesRotateTo );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosRotateToRect1Select;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateTo_Rectangle1_Select )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateTo_Rectangle1_MoveSelPtRotateTop_Select )
        {
            m_iTestStepLev4 = -1;
            m_fAngle_degree = 0.0;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            s_rect1OnMousePressEvent = s_rect1;

            pTestStep = getTestStep( strCurrentGraphObj + "SelPtRotateTopSelect", pTSGrpRectanglesRotateTo );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosRotateToRect1MoveSelPtRotateTopStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointRotateTop) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateTo_Rectangle1_MoveSelPtRotateTop_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_RotateTo_Rectangle1_MoveSelPtRotateTop_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_RotateTo_Rectangle1_MoveSelPtRotateTop_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "RotateToStep" + QString::number(m_iTestStepLev4), pTSGrpRectanglesRotateTo );

            m_fAngle_degree += s_arfRectanglesRotateToRectangle1MoveSelPtRotateTopTestStepsAngles_degree[m_iTestStepLev4];

            QPointF ptCenter;
            QPointF ptSelPtRot = m_pDrawingView->mapFromScene(s_ptDrawingSceneMousePosRotateToRect1MoveSelPtRotateTopStart);
            double  fAngle_rad = Math::deg2Rad(m_fAngle_degree);

            CGraphObjRect* pGraphObjRect = dynamic_cast<CGraphObjRect*>(m_pGraphObjSelected);

            if( pGraphObjRect != nullptr )
            {
                ptCenter = pGraphObjRect->rect().center();
                ptCenter = pGraphObjRect->mapToScene(ptCenter);
                ptCenter = m_pDrawingView->mapFromScene(ptCenter);
            }

            ptSelPtRot = rotatePoint( ptCenter, ptSelPtRot, fAngle_rad );

            m_ptDrawingViewMousePos = ptSelPtRot.toPoint();

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgRect1 = rotateRect( s_rect1.center(), s_rect1, fAngle_rad );
            s_plgRect1.append(s_plgRect1[0]);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgRect1) );

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

            bool bIgnoreTestResult = false;

            if( static_cast<int>(m_fAngle_degree) % 90 > 1 )
            {
                bIgnoreTestResult = true;
            }
            pTestStep->testEnded(bIgnoreTestResult);

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_RotateTo_Rectangle1_MoveSelPtRotateTop_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_RotateTo_Rectangle1_MoveSelPtRotateTop_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateTo_Rectangle1_Release )
        {
            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "Release", pTSGrpRectanglesRotateTo );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rect1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateTo_Rectangle1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateTo_Rectangle1_Finished )
        {
            pTSGrpRectanglesMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateTo_Rectangle1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_ActivateEditModeSelect )
        {
            if( !pTSGrpRectanglesRotateMoveResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpRectanglesRotateMoveResize->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpRectanglesRotateMoveResize );

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

            } // if( pTSGrpRectanglesRotateResizeMove->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateResizeMove_Rectangle1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_Start )
        {
            m_iTestStepLev4 = -1;
            m_fAngle_degree = 0.0;

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

            s_plgRect1 = s_rect1;

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_Start )

        else if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "RotateMoveResizeStep" + QString::number(m_iTestStepLev4), pTSGrpRectanglesRotateMoveResize );

            SRotateMoveResizeOperation op = s_arRotateMoveResizeRectangle1TestSteps[m_iTestStepLev4];

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
                QPolygonF plgRect1;
                plgRect1.append(op.m_pt0PolygonRectResult);
                plgRect1.append(op.m_pt1PolygonRectResult);
                plgRect1.append(op.m_pt2PolygonRectResult);
                plgRect1.append(op.m_pt3PolygonRectResult);
                plgRect1.append(op.m_pt0PolygonRectResult);
                strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(op.m_ptPos) );
                strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(plgRect1) );
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_Finished )
        {
            pTSGrpRectanglesRotateMoveResize->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Rectangles_RotateMoveResize_Rectangle1_Finished )

        else // start next test step
        {
            pTSGrpRectangles->testEnded();

            m_iTestStepLev2++; // Next test step
            m_iTestStepLev3 = 0;
            triggerDoTestStep();
        }

    } // if( pTSGrpRectangles->isEnabled() )

    delete pMouseEv;
    pMouseEv = nullptr;

} // doTestStepStandardShapesRectangles

#endif // #if 0
