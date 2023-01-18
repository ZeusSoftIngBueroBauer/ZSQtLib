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
#include "ZSDraw/ZSDrawGraphObjEllipse.h"
#include "ZSDraw/ZSDrawGraphObjGroup.h"
#include "ZSDraw/ZSDrawGraphObjRect.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSDraw/ZSDrawUnits.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysAux.h"
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

static QString s_strGroup1ObjName         = "Switch1";
static QString s_strGroup1Line1ObjName    = "SepTopLeft";
static QString s_strGroup1Line2ObjName    = "SepTopRight";
static QString s_strGroup1TriangleObjName = "Triangle";
static QString s_strGroup1RectObjName     = "Frame";
static QString s_strGroup1CnctPt1ObjName  = "In";
static QString s_strGroup1CnctPt2ObjName  = "Out1";
static QString s_strGroup1CnctPt3ObjName  = "Out2";
static QString s_strGroup1CnctPt4ObjName  = "Out3";
static QString s_strGroup1CnctLineObjName = "CnctLine";
static QString s_strGroup1CnctLineObjId   = "Switch1/CnctLine";

static QRectF        s_rctGroup1;
static QPolygonF     s_plgGroup1;
static QRectF        s_rctGroup1OnMousePressEvent; // during mouse move events the item's position is not changed
static QRectF        s_rctGroup1Rect1;
static QPolygonF     s_plgGroup1Rect1;
static QLineF        s_linGroup1SepTopLeft;
static QLineF        s_linGroup1SepTopRight;
static QPolygonF     s_plgGroup1Triangle;
static QList<QRectF> s_arrctGroup1CnctPts;
static QStringList   s_strlstGroup1CnctPtNames;
static QStringList   s_strlstGroup1GraphObjNames;

static QList<QRectF> s_arrctNoGroupCnctPts;
static QStringList   s_strlstNoGroupCnctPtNames;
static QPolygonF     s_plgNoGroupCnctLine1Curr;
static QRectF        s_rctBoundingNoGroupCnctLine1Curr;
static QPolygonF     s_plgNoGroupCnctLine2Curr;
static QRectF        s_rctBoundingNoGroupCnctLine2Curr;
static QPolygonF     s_plgNoGroupCnctLine3Curr;
static QRectF        s_rctBoundingNoGroupCnctLine3Curr;
static QPolygonF     s_plgNoGroupCnctLine4Curr;
static QRectF        s_rctBoundingNoGroupCnctLine4Curr;
static QPolygonF     s_plgNoGroupCnctLine5Curr;
static QRectF        s_rctBoundingNoGroupCnctLine5Curr;
static QStringList   s_strlstNoGroupGraphObjNames;

// Create_Group1_Rect1_MovePtBottomRight
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosCreateGroup1Rect1Start(400,450);

static const QSize s_arsizeCreateGroup1Rect1MovePtBottomRightTestSteps[] = {
                  /* RectCoors:   l,   t,   r,   b,  w,  h */
    /*  0 */ QSize( 5, 8 ), /*  400, 450, 405, 458,  5,  8 */
    /*  1 */ QSize( 5, 8 ), /*  400, 450, 410, 466, 10, 16 */
    /*  2 */ QSize( 5, 8 ), /*  400, 450, 415, 474, 15, 24 */
    /*  2 */ QSize( 5, 8 ), /*  400, 450, 420, 482, 20, 32 */
    /*  2 */ QSize( 5, 8 ), /*  400, 450, 425, 490, 25, 40 */
    /*  3 */ QSize( 5, 8 )  /*  400, 450, 430, 498, 30, 48 */
};

static const int ETestStepLev3_Groups_Create_Group1_Rect1_MovePtBottomRight_TestStepCount = sizeof(s_arsizeCreateGroup1Rect1MovePtBottomRightTestSteps)/sizeof(s_arsizeCreateGroup1Rect1MovePtBottomRightTestSteps[0]);

// Create_Group1_Line1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosCreateGroup1Line1Start(403,453);

static const QSize s_arsizeCreateGroup1Line1MovePt2TestSteps[] = {
    /*  0 */ QSize( 3, 0 ),
    /*  1 */ QSize( 3, 0 ),
    /*  2 */ QSize( 3, 0 )
};

static const int ETestStepLev3_Groups_Create_Group1_Line1_MovePt2_TestStepCount = sizeof(s_arsizeCreateGroup1Line1MovePt2TestSteps)/sizeof(s_arsizeCreateGroup1Line1MovePt2TestSteps[0]);

// Create_Group1_Line2
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosCreateGroup1Line2Start(427,453);

static const QSize s_arsizeCreateGroup1Line2MovePt2TestSteps[] = {
    /*  0 */ QSize( -3, 0 ),
    /*  1 */ QSize( -3, 0 ),
    /*  2 */ QSize( -3, 0 )
};

static const int ETestStepLev3_Groups_Create_Group1_Line2_MovePt2_TestStepCount = sizeof(s_arsizeCreateGroup1Line2MovePt2TestSteps)/sizeof(s_arsizeCreateGroup1Line2MovePt2TestSteps[0]);

// Create_Group1_Polygon1_FirstLineSegment
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosCreateGroup1Polygon1Start(403,495);

static const QSize s_arsizePolygonsCreateGroup1Polygon1FirstLineSegmentMovePt2TestSteps[] = {
    /*  0 */ QSize( 4, 0 ),
    /*  1 */ QSize( 4, 0 ),
    /*  2 */ QSize( 4, 0 )
};

static const int ETestStepLev3_Groups_Create_Group1_Polygon1_FirstLineSegment_MovePt2_TestStepCount  = sizeof(s_arsizePolygonsCreateGroup1Polygon1FirstLineSegmentMovePt2TestSteps)/sizeof(s_arsizePolygonsCreateGroup1Polygon1FirstLineSegmentMovePt2TestSteps[0]);

// Create_Group1_Polygon1_SecondLineSegment
//------------------------------------------------------------------------------

static const QSize s_arsizePolygonsCreateGroup1Polygon1SecondLineSegmentMovePt3TestSteps[] = {
    /*  0 */ QSize( -2, -2 ),
    /*  1 */ QSize( -2, -2 ),
    /*  1 */ QSize( -2, -2 )
};

static const int ETestStepLev3_Groups_Create_Group1_Polygon1_SecondLineSegment_MovePt3_TestStepCount = sizeof(s_arsizePolygonsCreateGroup1Polygon1SecondLineSegmentMovePt3TestSteps)/sizeof(s_arsizePolygonsCreateGroup1Polygon1SecondLineSegmentMovePt3TestSteps[0]);

// Create_Group1_CnctPt1
//------------------------------------------------------------------------------

static const QPointF s_ptDrawingSceneMousePosCreateGroup1CnctPt1 = QPointF(400,474);

// Create_Group1_CnctPt2
//------------------------------------------------------------------------------

static const QPointF s_ptDrawingSceneMousePosCreateGroup1CnctPt2 = QPointF(430,460);

// Create_Group1_CnctPt3
//------------------------------------------------------------------------------

static const QPointF s_ptDrawingSceneMousePosCreateGroup1CnctPt3 = QPointF(430,474);

// Create_Group1_CnctPt4
//------------------------------------------------------------------------------

static const QPointF s_ptDrawingSceneMousePosCreateGroup1CnctPt4 = QPointF(430,488);

// Create_Group1_CnctLine1Pt1Pt2
//------------------------------------------------------------------------------

static const QSize s_arsizeCreateGroup1CnctLine1Pt1Pt2MoveToPt2TestSteps[] = {
    /*  0 */ QSize( 5, -3 ), /*  405, 471 */
    /*  0 */ QSize( 5, -3 ), /*  410, 468 */
    /*  0 */ QSize( 5, -2 ), /*  415, 466 */
    /*  0 */ QSize( 5, -2 ), /*  420, 464 */
    /*  0 */ QSize( 5, -2 ), /*  425, 462 */
    /*  0 */ QSize( 5, -2 )  /*  430, 460 */
};

static const int ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_MoveToPt2_TestStepCount = sizeof(s_arsizeCreateGroup1CnctLine1Pt1Pt2MoveToPt2TestSteps)/sizeof(s_arsizeCreateGroup1CnctLine1Pt1Pt2MoveToPt2TestSteps[0]);

// Create_Group1_SelectObjects
//------------------------------------------------------------------------------

static const QSizeF s_sizeCreateGroup1SelectObjectsOffset = QSizeF(20.0,30.0);

static const int ETestStepLev3_Groups_Create_Group1_SelectObjects_TestStepCount = 10;

// Create_NoGroup_CnctPts
//------------------------------------------------------------------------------

static const QPointF s_arptDrawingSceneMousePosCreateNoGroupCnctPts[] = {
    /* 0 */ QPointF(350,474),
    /* 1 */ QPointF(530,460),
    /* 2 */ QPointF(530,474),
    /* 3 */ QPointF(530,488)
};

static const int ETestStepLev3_Groups_Create_NoGroup_CnctPts_TestStepCount = sizeof(s_arptDrawingSceneMousePosCreateNoGroupCnctPts)/sizeof(s_arptDrawingSceneMousePosCreateNoGroupCnctPts[0]);

// Create_NoGroup_CnctLine2Pt5Pt1
//------------------------------------------------------------------------------

static const int ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_MoveToPt1_TestStepCount = 5;

// Create_NoGroup_CnctLine3Pt2Pt6
//------------------------------------------------------------------------------

static const int ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_MoveToPt6_TestStepCount = 5;

// Create_NoGroup_CnctLine4Pt3Pt7
//------------------------------------------------------------------------------

static const int ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_MoveToPt7_TestStepCount = 5;

// Create_NoGroup_CnctLine5Pt4Pt8
//------------------------------------------------------------------------------

static const int ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_MoveToPt8_TestStepCount = 5;

// Move_Group1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosMoveGroup1Start(415,474);

static const QSize s_arsizeMoveGroup1TestSteps[] = {
    /*  0 */ QSize( -10,   0 ),
    /*  1 */ QSize(   0,  10 ),
    /*  2 */ QSize(  10,   0 ),
    /*  3 */ QSize(  10,   0 ),
    /*  4 */ QSize( -10, -10 )
};

static const int ETestStepLev3_Groups_Move_Group1_TestStepCount = sizeof(s_arsizeMoveGroup1TestSteps)/sizeof(s_arsizeMoveGroup1TestSteps[0]);

// Resize_Group1_MovePtBottomRight
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeGroup1Select(415,474);

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtBottomRightStart(435,498);

// Initial radius of connection points is 5 pixels. The radius got to be added
// to the left and right of Rect1 to get the groups bounding rectangle.
// The connection points are resized together with the group. As the group is
// resized equally in both directions (horizontally and vertically) the connection
// points remain circles and the radius increases and decreases corresponding to
// the transformation scale factors.
static const QSize s_arsizeGroupsResizeGroup1MovePtBottomRightTestSteps[] = {
                                /* Group1: l,   t,      r,      b,     w,     h */ /*                    CnctPt: Radius */ /* Rect: l,     t,      r,      b,     w,     h */
                                /* 395.00, 450.00, 435.00, 498.00, 40.00, 48.00 */ /* 40/40*5 = 48/48*5 = 1.00*5 = 5.00 */ /* 400.00, 450.00, 430.00, 498.00, 30.00, 48.00 */
    /*  0 */ QSize( -10, -12 ), /* 395.00, 450.00, 425.00, 486.00, 30.00, 36.00 */ /* 30/40*5 = 36/48*5 = 0.75*5 = 3.75 */ /* 398.75, 450.00, 421.25, 486.00, 22.50, 36.00 */
    /*  1 */ QSize( -10, -12 ), /* 395.00, 450.00, 415.00, 474.00, 20.00, 24.00 */ /* 20/40*5 = 24/48*5 = 0.50*5 = 2.50 */ /* 397.50, 450.00, 412.50, 474.00, 15.00, 24.00 */
    /*  2 */ QSize( -10, -12 ), /* 395.00, 450.00, 405.00, 462.00, 10.00, 12.00 */ /* 10/40*5 = 12/48*5 = 0.25*5 = 1.25 */ /* 396.25, 450.00, 403.75, 462.00,  7.50, 12.00 */
    /*  3 */ QSize(  10,  12 ), /* 395.00, 450.00, 415.00, 474.00, 20.00, 24.00 */ /* 20/40*5 = 24/48*5 = 0.50*5 = 2.50 */ /* 397.50, 450.00, 412.50, 474.00, 15.00, 24.00 */
    /*  4 */ QSize( -10, -12 ), /* 395.00, 450.00, 405.00, 462.00, 10.00, 12.00 */ /* 10/40*5 = 12/48*5 = 0.25*5 = 1.25 */ /* 396.26, 450.00, 403.75, 462.00,  7.50, 12.00 */
    /*  5 */ QSize( -10, -12 ), /* 395.00, 450.00, 395.00, 450.00,  0.00,  0.00 */ /*  0/40*5 =  0/48*5 = 0.00*5 = 0.00 */ /* 395.00, 450.00, 395.00, 450.00,  0.00,  0.00 */
    /*  6 */ QSize( -10, -12 ), /* 385.00, 438.00, 395.00, 450.00, 10.00, 12.00 */ /* 10/40*5 = 12/48*5 = 0.25*5 = 1.25 */ /* 386.25, 438.00, 393.75, 450.00,  7.50, 12.00 */
    /*  7 */ QSize( -10, -12 )  /* 375.00, 426.00, 395.00, 450.00, 20.00, 24.00 */ /* 20/40*5 = 24/48*5 = 0.50*5 = 2.50 */ /* 377.50, 426.00, 392.50, 450.00, 15.00, 24.00 */
};

static const int ETestStepLev3_Groups_Resize_Group1_MovePtBottomRight_TestStepCount = sizeof(s_arsizeGroupsResizeGroup1MovePtBottomRightTestSteps)/sizeof(s_arsizeGroupsResizeGroup1MovePtBottomRightTestSteps[0]);

// Resize_Group1_MovePtTopLeft
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtTopLeftStart(375,426);

static const QSize s_arsizeGroupsResizeGroup1MovePtTopLeftTestSteps[] = {
                        /* Group1: l,   t,   r,   b,  w,  h */
                              /* 375, 426, 395, 450, 20, 24 */
    /*  0 */ QSize( 10, 12 ), /* 385, 438, 395, 450, 10, 12 */
    /*  1 */ QSize( 10, 12 ), /* 395, 450, 395, 450,  0,  0 */
    /*  2 */ QSize( 10, 12 ), /* 395, 450, 405, 462, 10, 12 */
    /*  3 */ QSize( 10, 12 )  /* 395, 450, 415, 474, 20, 24 */
};

static const int ETestStepLev3_Groups_Resize_Group1_MovePtTopLeft_TestStepCount = sizeof(s_arsizeGroupsResizeGroup1MovePtTopLeftTestSteps)/sizeof(s_arsizeGroupsResizeGroup1MovePtTopLeftTestSteps[0]);

// Resize_Group1_MovePtTopRight
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtTopRightStart(415,450);

static const QSize s_arsizeGroupsResizeGroup1MovePtTopRightTestSteps[] = {
                        /* Group1:  l,   t,   r,   b,  w,  h */
                               /* 395, 450, 415, 474, 20, 24 */
    /*  0 */ QSize( -10, 12 ), /* 395, 462, 405, 474, 10, 12 */
    /*  1 */ QSize( -10, 12 ), /* 395, 474, 395, 474,  0,  0 */
    /*  2 */ QSize( -10, 12 ), /* 385, 474, 395, 486, 10, 12 */
    /*  3 */ QSize( -10, 12 )  /* 375, 474, 395, 498, 20, 24 */
};

static const int ETestStepLev3_Groups_Resize_Group1_MovePtTopRight_TestStepCount = sizeof(s_arsizeGroupsResizeGroup1MovePtTopRightTestSteps)/sizeof(s_arsizeGroupsResizeGroup1MovePtTopRightTestSteps[0]);

// Resize_Group1_MovePtBottomLeft
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtBottomLeftStart(375,498);

static const QSize s_arsizeGroupsResizeGroup1MovePtBottomLeftTestSteps[] = {
                        /* Group1:  l,   t,   r,   b,  w,  h */
                               /* 375, 474, 395, 498, 20, 24 */
    /*  0 */ QSize( 10, -12 ), /* 385, 474, 395, 486, 10, 12 */
    /*  1 */ QSize( 10, -12 ), /* 395, 474, 395, 474,  0,  0 */
    /*  2 */ QSize( 10, -12 ), /* 395, 462, 405, 474, 10, 12 */
    /*  3 */ QSize( 10, -12 )  /* 395, 450, 415, 474, 20, 24 */
};

static const int ETestStepLev3_Groups_Resize_Group1_MovePtBottomLeft_TestStepCount = sizeof(s_arsizeGroupsResizeGroup1MovePtBottomLeftTestSteps)/sizeof(s_arsizeGroupsResizeGroup1MovePtBottomLeftTestSteps[0]);

// Resize_Group1_MovePtBottomCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtBottomCenterStart(405,474);

static const QSize s_arsizeGroupsResizeGroup1MovePtBottomCenterTestSteps[] = {
                       /* Group1:  l,   t,   r,   b,  w,  h */
                              /* 395, 450, 415, 474, 20, 24 */
    /*  0 */ QSize( 0, -12 ), /* 395, 450, 415, 462, 20, 12 */
    /*  1 */ QSize( 0, -12 ), /* 395, 450, 415, 450, 20,  0 */
    /*  2 */ QSize( 0, -12 ), /* 395, 438, 415, 450, 20, 12 */
    /*  3 */ QSize( 0, -12 )  /* 395, 426, 415, 450, 20, 24 */
};

static const int ETestStepLev3_Groups_Resize_Group1_MovePtBottomCenter_TestStepCount = sizeof(s_arsizeGroupsResizeGroup1MovePtBottomCenterTestSteps)/sizeof(s_arsizeGroupsResizeGroup1MovePtBottomCenterTestSteps[0]);

// Resize_Group1_MovePtTopCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtTopCenterStart(405,426);

static const QSize s_arsizeGroupsResizeGroup1MovePtTopCenterTestSteps[] = {
                      /* Group1:  l,   t,   r,   b,  w,  h */
                             /* 395, 426, 415, 450, 20, 24 */
    /*  0 */ QSize( 0, 12 ), /* 395, 438, 415, 450, 20, 12 */
    /*  1 */ QSize( 0, 12 ), /* 395, 450, 415, 450, 20,  0 */
    /*  2 */ QSize( 0, 12 ), /* 395, 450, 415, 462, 20, 12 */
    /*  3 */ QSize( 0, 12 ), /* 395, 450, 415, 474, 20, 24 */
    /*  4 */ QSize( 0, 12 ), /* 395, 450, 415, 486, 20, 36 */
    /*  5 */ QSize( 0, 12 )  /* 395, 450, 415, 498, 20, 48 */
};

static const int ETestStepLev3_Groups_Resize_Group1_MovePtTopCenter_TestStepCount = sizeof(s_arsizeGroupsResizeGroup1MovePtTopCenterTestSteps)/sizeof(s_arsizeGroupsResizeGroup1MovePtTopCenterTestSteps[0]);

// Resize_Group1_MovePtRightCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtRightCenterStart(415,474);

static const QSize s_arsizeGroupsResizeGroup1MovePtRightCenterTestSteps[] = {
                       /* Group1:  l,   t,   r,   b,  w,  h */
                              /* 395, 450, 415, 498, 20, 48 */
    /*  0 */ QSize( -10, 0 ), /* 395, 450, 405, 498, 10, 48 */
    /*  1 */ QSize( -10, 0 ), /* 395, 450, 395, 498,  0, 48 */
    /*  2 */ QSize( -10, 0 ), /* 385, 450, 395, 498, 10, 48 */
    /*  3 */ QSize( -10, 0 )  /* 375, 450, 395, 498, 20, 48 */
};

static const int ETestStepLev3_Groups_Resize_Group1_MovePtRightCenter_TestStepCount = sizeof(s_arsizeGroupsResizeGroup1MovePtRightCenterTestSteps)/sizeof(s_arsizeGroupsResizeGroup1MovePtRightCenterTestSteps[0]);

// Resize_Group1_MovePtLeftCenter
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeRect1PtLeftCenterStart(375,474);

static const QSize s_arsizeGroupsResizeGroup1MovePtLeftCenterTestSteps[] = {
                      /* Group1:  l,   t,   r,   b,  w,  h */
                             /* 375, 450, 395, 498, 20, 48 */
    /*  2 */ QSize( 10, 0 ), /* 385, 450, 395, 498, 10, 48 */
    /*  3 */ QSize( 10, 0 ), /* 395, 450, 395, 498,  0, 48 */
    /*  4 */ QSize( 10, 0 ), /* 395, 450, 405, 498, 10, 48 */
    /*  5 */ QSize( 10, 0 ), /* 395, 450, 415, 498, 20, 48 */
    /*  6 */ QSize( 10, 0 ), /* 395, 450, 425, 498, 30, 48 */
    /*  7 */ QSize( 10, 0 )  /* 395, 450, 435, 498, 40, 48 */
};

static const int ETestStepLev3_Groups_Resize_Group1_MovePtLeftCenter_TestStepCount = sizeof(s_arsizeGroupsResizeGroup1MovePtLeftCenterTestSteps)/sizeof(s_arsizeGroupsResizeGroup1MovePtLeftCenterTestSteps[0]);

// Rotate_Group1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosRotateGroup1Select(415,474);

static const QPoint s_ptDrawingSceneMousePosRotateGroup1MoveSelPtRotateTopStart(415,430);

static const double s_arfRotateGroup1MoveSelPtRotateTopTestStepsAngles_degree[] = {
    /*  0 */  10.0,
    /*  1 */  20.0,
    /*  2 */  30.0,
    /*  3 */  40.0,
    /*  4 */  50.0,
    /*  5 */  60.0,
    /*  6 */  70.0,
    /*  7 */  80.0,
    /*  8 */  90.0,
    /*  9 */ 120.0,
    /* 10 */ 150.0,
    /* 11 */ 180.0,
    /* 12 */ 225.0,
    /* 13 */ 270.0,
    /* 14 */ 280.0,
    /* 15 */ 290.0,
    /* 16 */ 300.0,
    /* 17 */ 310.0,
    /* 18 */ 320.0,
    /* 19 */ 330.0,
    /* 20 */ 340.0,
    /* 21 */ 350.0,
    /* 22 */ 360.0
};

static const int ETestStepLev3_Groups_Rotate_Group1_MoveSelPtRotateTop_TestStepCount = sizeof(s_arfRotateGroup1MoveSelPtRotateTopTestStepsAngles_degree)/sizeof(s_arfRotateGroup1MoveSelPtRotateTopTestStepsAngles_degree[0]);

// RotateMoveResize_Group1
//------------------------------------------------------------------------------

struct SRotateMoveResizeOperation
{
    QPoint    m_ptDrawingSceneMousePos;
    double    m_fRotAngle_degree;
    QPointF   m_ptDrawingSceneGroupCenterPos;
    int       m_iMouseEvent;
    EEditMode m_operation;
    QPointF   m_ptPos;
    QPointF   m_pt0PolygonGroupResult; // in case operation = Rotate and ptDrawingSceneMousePos = (-1/-1) TopLeft corner of rectangle to be rotated by RotAngle_degree
    QPointF   m_pt1PolygonGroupResult; // in case operation = Rotate and ptDrawingSceneMousePos = (-1/-1) Width and Height of rectangle to be rotated by RotAngle_degree
    QPointF   m_pt2PolygonGroupResult;
    QPointF   m_pt3PolygonGroupResult;
    bool      m_bIgnoreTestResult;
};

static const SRotateMoveResizeOperation s_arRotateMoveResizeGroup1TestSteps[] = {
          // { MousePos,        Angle, GroupCenterPos,      MouseEvent,                 Operation,                        Pos,      PolygonGroup[0],      PolygonGroup[1],      PolygonGroup[2],      PolygonGroup[3], Ignore }
    //                            0.0, QPoint(415.0,474.0),                                              QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(435.0,450.0), QPointF(435.0,498.0), QPointF(395.0,498.0)
    /*  0 */ { QPoint(415,465),   0.0, QPoint(415.0,474.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(435.0,450.0), QPointF(435.0,498.0), QPointF(395.0,498.0), false  }, // Select
    /*  1 */ { QPoint( -1, -1),   0.0, QPoint(415.0,474.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(435.0,450.0), QPointF(435.0,498.0), QPointF(395.0,498.0), false  },
    /*  2 */ { QPoint(435,498),   0.0, QPoint(415.0,474.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(435.0,450.0), QPointF(435.0,498.0), QPointF(395.0,498.0), false  }, // Select SelPt BottomRight
    /*  3 */ { QPoint(445,510),   0.0, QPoint(420.0,480.0), QEvent::MouseMove,          EEditModeResize, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(445.0,450.0), QPointF(445.0,510.0), QPointF(395.0,510.0), false  }, // ResizeBy 10/12
    /*  4 */ { QPoint(455,522),   0.0, QPoint(425.0,486.0), QEvent::MouseMove,          EEditModeResize, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(455.0,450.0), QPointF(455.0,522.0), QPointF(395.0,522.0), false  }, // ResizeBy 10/12
    /*  5 */ { QPoint(465,534),   0.0, QPoint(430.0,492.0), QEvent::MouseMove,          EEditModeResize, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(465.0,450.0), QPointF(465.0,534.0), QPointF(395.0,534.0), false  }, // ResizeBy 10/12
    /*  6 */ { QPoint(475,546),   0.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeResize, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(475.0,450.0), QPointF(475.0,546.0), QPointF(395.0,546.0), false  }, // ResizeBy 10/12
    /*  7 */ { QPoint(485,558),   0.0, QPoint(440.0,504.0), QEvent::MouseMove,          EEditModeResize, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(485.0,450.0), QPointF(485.0,558.0), QPointF(395.0,558.0), false  }, // ResizeBy 10/12
    /*  8 */ { QPoint( -1, -1),   0.0, QPoint(440.0,504.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(485.0,450.0), QPointF(485.0,558.0), QPointF(395.0,558.0), false  },
    /*  9 */ { QPoint(485,558),   0.0, QPoint(440.0,504.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(485.0,450.0), QPointF(485.0,558.0), QPointF(395.0,558.0), false  }, // Select SelPt BottomRight
    /* 10 */ { QPoint(475,546),   0.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeResize, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(475.0,450.0), QPointF(475.0,546.0), QPointF(395.0,546.0), false  }, // ResizeBy -10/-12
    /* 11 */ { QPoint( -1, -1),   0.0, QPoint(435.0,498.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(475.0,450.0), QPointF(475.0,546.0), QPointF(395.0,546.0), false  },
    /* 12 */ { QPoint(435,430),   0.0, QPoint(435.0,498.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(475.0,450.0), QPointF(475.0,546.0), QPointF(395.0,546.0), false  }, // Select SelPt RotateTop
    /* 13 */ { QPoint( -1, -1),  10.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 10°
    /* 14 */ { QPoint( -1, -1),  20.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 20°
    /* 15 */ { QPoint( -1, -1),  30.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 30°
    /* 16 */ { QPoint( -1, -1),  40.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 40°
    /* 17 */ { QPoint( -1, -1),  50.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 50°
    /* 18 */ { QPoint( -1, -1),  60.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 60°
    /* 19 */ { QPoint( -1, -1),  70.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 70°
    /* 20 */ { QPoint( -1, -1),  80.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 80°
    /* 21 */ { QPoint( -1, -1),  90.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 90°
    /* 22 */ { QPoint( -1, -1),   0.0, QPoint(435.0,498.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(395.0,450.0), QPointF(387.0,538.0), QPointF(387.0,458.0), QPointF(483.0,458.0), QPointF(483.0,538.0), false  },
    /* 23 */ { QPoint(367,498),   0.0, QPoint(435.0,498.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(395.0,450.0), QPointF(387.0,538.0), QPointF(387.0,458.0), QPointF(483.0,458.0), QPointF(483.0,538.0), false  }, // Select SelPt RotateTop
    /* 24 */ { QPoint( -1, -1),   0.0, QPoint(435.0,498.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(395.0,450.0), QPointF(387.0,538.0), QPointF(387.0,458.0), QPointF(483.0,458.0), QPointF(483.0,538.0), false  },
    /* 25 */ { QPoint(367,498),   0.0, QPoint(435.0,498.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(395.0,450.0), QPointF(387.0,538.0), QPointF(387.0,458.0), QPointF(483.0,458.0), QPointF(483.0,538.0), false  }, // Select SelPt RotateTop
    /* 26 */ { QPoint( -1, -1),  80.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 80°
    /* 27 */ { QPoint( -1, -1),  70.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 70°
    /* 28 */ { QPoint( -1, -1),  60.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 60°
    /* 29 */ { QPoint( -1, -1),  50.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 50°
    /* 30 */ { QPoint( -1, -1),  40.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 40°
    /* 31 */ { QPoint( -1, -1),  30.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 30°
    /* 32 */ { QPoint( -1, -1),  20.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 20°
    /* 33 */ { QPoint( -1, -1),  10.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to 10°
    /* 34 */ { QPoint( -1, -1),   0.0, QPoint(435.0,498.0), QEvent::MouseMove,          EEditModeRotate, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF( 80.0, 96.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to  0°
    /* 35 */ { QPoint( -1, -1),   0.0, QPoint(435.0,498.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(475.0,450.0), QPointF(475.0,546.0), QPointF(395.0,546.0), false  },
    /* 36 */ { QPoint(395,546),   0.0, QPoint(435.0,498.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(395.0,450.0), QPointF(395.0,450.0), QPointF(475.0,450.0), QPointF(475.0,546.0), QPointF(395.0,546.0), false  }, // Select SelPt BottomLeft
    /* 37 */ { QPoint(405,534),   0.0, QPoint(440.0,492.0), QEvent::MouseMove,          EEditModeResize, QPointF(395.0,450.0), QPointF(405.0,450.0), QPointF(475.0,450.0), QPointF(475.0,534.0), QPointF(405.0,534.0), false  }, // ResizeBy -10/-12
    /* 38 */ { QPoint(415,522),   0.0, QPoint(445.0,486.0), QEvent::MouseMove,          EEditModeResize, QPointF(395.0,450.0), QPointF(415.0,450.0), QPointF(475.0,450.0), QPointF(475.0,522.0), QPointF(415.0,522.0), false  }, // ResizeBy -10/-12
    /* 39 */ { QPoint( -1, -1),   0.0, QPoint(445.0,486.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(415.0,450.0), QPointF(415.0,450.0), QPointF(475.0,450.0), QPointF(475.0,522.0), QPointF(415.0,522.0), false  },
    /* 40 */ { QPoint(425,470),   0.0, QPoint(445.0,486.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(415.0,450.0), QPointF(415.0,450.0), QPointF(475.0,450.0), QPointF(475.0,522.0), QPointF(415.0,522.0), false  }, // Select
    /* 41 */ { QPoint(445,490),   0.0, QPoint(465.0,506.0), QEvent::MouseMove,          EEditModeMove,   QPointF(435.0,470.0), QPointF(435.0,470.0), QPointF(495.0,470.0), QPointF(495.0,542.0), QPointF(435.0,542.0), false  }, // Move by 20/20
    /* 42 */ { QPoint( -1, -1),   0.0, QPoint(465.0,506.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(435.0,470.0), QPointF(435.0,470.0), QPointF(495.0,470.0), QPointF(495.0,542.0), QPointF(435.0,542.0), false  },
    /* 43 */ { QPoint(465,470),   0.0, QPoint(465.0,506.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(435.0,470.0), QPointF(435.0,470.0), QPointF(495.0,470.0), QPointF(495.0,542.0), QPointF(435.0,542.0), false  }, // Select SelPt TopCenter
    /* 44 */ { QPoint(465,482),   0.0, QPoint(465.0,512.0), QEvent::MouseMove,          EEditModeResize, QPointF(435.0,470.0), QPointF(435.0,482.0), QPointF(495.0,482.0), QPointF(495.0,542.0), QPointF(435.0,542.0), false  }, // ResizeBy 0/-12 (decrease height by 12)
    /* 45 */ { QPoint( -1, -1),   0.0, QPoint(465.0,512.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(435.0,482.0), QPointF(435.0,482.0), QPointF(495.0,482.0), QPointF(495.0,542.0), QPointF(435.0,542.0), false  },
    /* 46 */ { QPoint(465,462),   0.0, QPoint(465.0,512.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(435.0,482.0), QPointF(435.0,482.0), QPointF(495.0,482.0), QPointF(495.0,542.0), QPointF(435.0,542.0), false  }, // Select SelPt RotateTop
    /* 47 */ { QPoint( -1, -1), -15.0, QPoint(465.0,512.0), QEvent::MouseMove,          EEditModeRotate, QPointF(435.0,482.0), QPointF(435.0,482.0), QPointF( 60.0, 60.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to -15°
    /* 48 */ { QPoint( -1, -1), -30.0, QPoint(465.0,512.0), QEvent::MouseMove,          EEditModeRotate, QPointF(435.0,482.0), QPointF(435.0,482.0), QPointF( 60.0, 60.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to -30°
    /* 49 */ { QPoint( -1, -1), -45.0, QPoint(465.0,512.0), QEvent::MouseMove,          EEditModeRotate, QPointF(435.0,482.0), QPointF(435.0,482.0), QPointF( 60.0, 60.0), QPointF(000.0,000.0), QPointF(000.0,000.0), false  }, // Rotate to -45°
    /* 50 */ { QPoint( -1, -1),   0.0, QPoint(465.0,512.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(435.0,482.0), QPointF(465.0,469.6), QPointF(507.4,512.0), QPointF(465.0,554.4), QPointF(422.6,512.0), false  },
    /* 51 */ { QPoint(465,554),   0.0, QPoint(000.0,000.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(435.0,482.0), QPointF(465.0,469.6), QPointF(507.4,512.0), QPointF(465.0,554.4), QPointF(422.6,512.0), false  }, // Select SelPt BottomRight
    /* 52 */ { QPoint(458,547),   0.0, QPoint(000.0,000.0), QEvent::MouseMove,          EEditModeResize, QPointF(435.0,482.0), QPointF(465.0,469.6), QPointF(500.2,504.8), QPointF(458.0,547.0), QPointF(422.8,511.8), false  }, // ResizeBy -10/0 (decrease width by 10 to 50)
    /* 53 */ { QPoint(451,540),   0.0, QPoint(000.0,000.0), QEvent::MouseMove,          EEditModeResize, QPointF(435.0,482.0), QPointF(465.0,469.6), QPointF(493.2,497.8), QPointF(451.0,540.0), QPointF(422.8,511.8), false  }, // ResizeBy -10/0 (decrease width by 10 to 40)
    /* 54 */ { QPoint( -1, -1),   0.0, QPoint(000.0,000.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(438.1,474.9), QPointF(465.0,469.6), QPointF(493.2,497.8), QPointF(451.0,540.0), QPointF(422.8,511.8), false  },
    /* 55 */ { QPoint(493,470),   0.0, QPoint(458.0,505.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(438.1,474.9), QPointF(465.0,469.6), QPointF(493.2,497.8), QPointF(451.0,540.0), QPointF(422.8,511.8), false  }, // Select SelPt RotateTop
    /* 56 */ { QPoint(458,455),   0.0, QPoint(458.0,505.0), QEvent::MouseMove,          EEditModeRotate, QPointF(438.1,474.9), QPointF(438.1,474.9), QPointF(477.9,474.9), QPointF(477.9,534.6), QPointF(438.1,534.6), false  }, // Rotate to -45°
    /* 57 */ { QPoint( -1, -1),   0.0, QPoint(458.0,505.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(438.1,474.9), QPointF(438.1,474.9), QPointF(477.9,474.9), QPointF(477.9,534.6), QPointF(438.1,534.6), false  },
    /* 58 */ { QPoint(458,475),   0.0, QPoint(458.0,505.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(438.1,474.9), QPointF(438.1,474.9), QPointF(477.9,474.9), QPointF(477.9,534.6), QPointF(438.1,534.6), false  }, // Select SelPt TopCenter
    /* 59 */ { QPoint(458,487),   0.0, QPoint(458.0,511.0), QEvent::MouseMove,          EEditModeResize, QPointF(438.1,474.9), QPointF(438.1,487.0), QPointF(477.9,487.0), QPointF(477.9,534.6), QPointF(438.1,534.6), false  }, // ResizeBy 0/-12 (decrease height by 12)
    /* 50 */ { QPoint( -1, -1),   0.0, QPoint(458.0,511.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(438.1,487.0), QPointF(438.1,487.0), QPointF(477.9,487.0), QPointF(477.9,534.6), QPointF(438.1,534.6), false  },
    /* 61 */ { QPoint(458,497),   0.0, QPoint(458.0,511.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(438.1,487.0), QPointF(438.1,487.0), QPointF(477.9,487.0), QPointF(477.9,534.6), QPointF(438.1,534.6), false  }, // Select
    /* 62 */ { QPoint(415,460),   0.0, QPoint(415.0,474.0), QEvent::MouseMove,          EEditModeMove,   QPointF(395.1,450.0), QPointF(395.1,450.0), QPointF(434.9,450.0), QPointF(434.9,497.6), QPointF(395.1,497.6), false  }, // Move by -43/-37
    /* 63 */ { QPoint( -1, -1),   0.0, QPoint(415.0,474.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(395.1,450.0), QPointF(395.1,450.0), QPointF(434.9,450.0), QPointF(434.9,497.6), QPointF(395.1,497.6), false  }
};

static const int ETestStepLev3_Groups_RotateMoveResize_Group1_TestStepCount = sizeof(s_arRotateMoveResizeGroup1TestSteps)/sizeof(s_arRotateMoveResizeGroup1TestSteps[0]);

// Simulation_Group1
//------------------------------------------------------------------------------

static const QPoint s_arptDrawingSceneMousePosSimulationGroup1TestSteps[] = {
    QPoint(415,474),
    QPoint(415,474),
    QPoint(415,474)
};

static const int ETestStepLev3_Groups_Simulation_Group1_TestStepCount = sizeof(s_arptDrawingSceneMousePosSimulationGroup1TestSteps)/sizeof(s_arptDrawingSceneMousePosSimulationGroup1TestSteps[0]);

static void fctGroup1MouseReleaseEvent( void* i_pvThis, void* i_pvData, CGraphObj* i_pGraphObj, QGraphicsSceneMouseEvent* i_pEv );

// Test steps:
//------------------------------------------------------------------------------

enum
{
    ETestStepLev3_Groups_Create_Group1_Rect1_ActivateDrawingTool,
    ETestStepLev3_Groups_Create_Group1_Rect1,
    ETestStepLev3_Groups_Create_Group1_Rect1_MovePtBottomRight_TestStepMin,
    ETestStepLev3_Groups_Create_Group1_Rect1_MovePtBottomRight_TestStepMax = ETestStepLev3_Groups_Create_Group1_Rect1_MovePtBottomRight_TestStepMin + ETestStepLev3_Groups_Create_Group1_Rect1_MovePtBottomRight_TestStepCount - 1,
    ETestStepLev3_Groups_Create_Group1_Rect1_Release,
    ETestStepLev3_Groups_Create_Group1_Line1_ActivateDrawingTool,
    ETestStepLev3_Groups_Create_Group1_Line1,
    ETestStepLev3_Groups_Create_Group1_Line1_MovePt2_TestStepMin,
    ETestStepLev3_Groups_Create_Group1_Line1_MovePt2_TestStepMax = ETestStepLev3_Groups_Create_Group1_Line1_MovePt2_TestStepMin + ETestStepLev3_Groups_Create_Group1_Line1_MovePt2_TestStepCount - 1,
    ETestStepLev3_Groups_Create_Group1_Line1_Release,
    ETestStepLev3_Groups_Create_Group1_Line2,
    ETestStepLev3_Groups_Create_Group1_Line2_MovePt2_TestStepMin,
    ETestStepLev3_Groups_Create_Group1_Line2_MovePt2_TestStepMax = ETestStepLev3_Groups_Create_Group1_Line2_MovePt2_TestStepMin + ETestStepLev3_Groups_Create_Group1_Line2_MovePt2_TestStepCount - 1,
    ETestStepLev3_Groups_Create_Group1_Line2_Release,
    ETestStepLev3_Groups_Create_Group1_Polygon1_ActivateDrawingTool,
    ETestStepLev3_Groups_Create_Group1_Polygon1_FirstLineSegment,
    ETestStepLev3_Groups_Create_Group1_Polygon1_FirstLineSegment_MovePt2_TestStepMin,
    ETestStepLev3_Groups_Create_Group1_Polygon1_FirstLineSegment_MovePt2_TestStepMax = ETestStepLev3_Groups_Create_Group1_Polygon1_FirstLineSegment_MovePt2_TestStepMin + ETestStepLev3_Groups_Create_Group1_Polygon1_FirstLineSegment_MovePt2_TestStepCount - 1,
    ETestStepLev3_Groups_Create_Group1_Polygon1_SecondLineSegment,
    ETestStepLev3_Groups_Create_Group1_Polygon1_SecondLineSegment_MovePt3_TestStepMin,
    ETestStepLev3_Groups_Create_Group1_Polygon1_SecondLineSegment_MovePt3_TestStepMax = ETestStepLev3_Groups_Create_Group1_Polygon1_SecondLineSegment_MovePt3_TestStepMin + ETestStepLev3_Groups_Create_Group1_Polygon1_SecondLineSegment_MovePt3_TestStepCount - 1,
    ETestStepLev3_Groups_Create_Group1_Polygon1_Release,
    ETestStepLev3_Groups_Create_Group1_CnctPts_ActivateDrawingTool,
    ETestStepLev3_Groups_Create_Group1_CnctPt1,
    ETestStepLev3_Groups_Create_Group1_CnctPt2,
    ETestStepLev3_Groups_Create_Group1_CnctPt3,
    ETestStepLev3_Groups_Create_Group1_CnctPt4,
    ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_ActivateDrawingTool,
    ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_ConnectAtPt1,
    ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_MoveToPt2_TestStepMin,
    ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_MoveToPt2_TestStepMax = ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_MoveToPt2_TestStepMin + ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_MoveToPt2_TestStepCount - 1,
    ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_ConnectAtPt2,
    ETestStepLev3_Groups_Create_Group1_SelectObjects_ActivateDrawingTool,
    ETestStepLev3_Groups_Create_Group1_SelectObjects_Start,
    ETestStepLev3_Groups_Create_Group1_SelectObjects_TestStepMin,
    ETestStepLev3_Groups_Create_Group1_SelectObjects_TestStepMax = ETestStepLev3_Groups_Create_Group1_SelectObjects_TestStepMin + ETestStepLev3_Groups_Create_Group1_SelectObjects_TestStepCount - 1,
    ETestStepLev3_Groups_Create_Group1_SelectObjects_Release,
    ETestStepLev3_Groups_Create_Group1_GroupObjects,
    ETestStepLev3_Groups_Create_NoGroup_CnctPts_ActivateDrawingTool,
    ETestStepLev3_Groups_Create_NoGroup_CnctPts_TestStepMin,
    ETestStepLev3_Groups_Create_NoGroup_CnctPts_TestStepMax = ETestStepLev3_Groups_Create_NoGroup_CnctPts_TestStepMin + ETestStepLev3_Groups_Create_NoGroup_CnctPts_TestStepCount - 1,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_ActivateDrawingTool,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_ConnectAtPt5,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_MoveToPt1_TestStepMin,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_MoveToPt1_TestStepMax = ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_MoveToPt1_TestStepMin + ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_MoveToPt1_TestStepCount - 1,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_ConnectAtPt1,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_ActivateDrawingTool,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_ConnectAtPt2,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_MoveToPt6_TestStepMin,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_MoveToPt6_TestStepMax = ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_MoveToPt6_TestStepMin + ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_MoveToPt6_TestStepCount - 1,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_ConnectAtPt6,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_ActivateDrawingTool,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_ConnectAtPt3,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_MoveToPt7_TestStepMin,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_MoveToPt7_TestStepMax = ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_MoveToPt7_TestStepMin + ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_MoveToPt7_TestStepCount - 1,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_ConnectAtPt7,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_ActivateDrawingTool,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_ConnectAtPt5,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_MoveToPt8_TestStepMin,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_MoveToPt8_TestStepMax = ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_MoveToPt8_TestStepMin + ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_MoveToPt8_TestStepCount - 1,
    ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_ConnectAtPt8,
    ETestStepLev3_Groups_Create_Finished,
    ETestStepLev3_Groups_Move_Group1_ActivateDrawingTool,
    ETestStepLev3_Groups_Move_Group1_Select,
    ETestStepLev3_Groups_Move_Group1_TestStepMin,
    ETestStepLev3_Groups_Move_Group1_TestStepMax = ETestStepLev3_Groups_Move_Group1_TestStepMin + ETestStepLev3_Groups_Move_Group1_TestStepCount - 1,
    ETestStepLev3_Groups_Move_Group1_Release,
    ETestStepLev3_Groups_Move_Group1_Finished,
    ETestStepLev3_Groups_Resize_Group1_ActivateEditModeSelect,
    ETestStepLev3_Groups_Resize_Group1_Select,
    ETestStepLev3_Groups_Resize_Group1_MovePtBottomRight_Select,
    ETestStepLev3_Groups_Resize_Group1_MovePtBottomRight_TestStepMin,
    ETestStepLev3_Groups_Resize_Group1_MovePtBottomRight_TestStepMax = ETestStepLev3_Groups_Resize_Group1_MovePtBottomRight_TestStepMin + ETestStepLev3_Groups_Resize_Group1_MovePtBottomRight_TestStepCount - 1,
    ETestStepLev3_Groups_Resize_Group1_MovePtBottomRight_Release,
    ETestStepLev3_Groups_Resize_Group1_MovePtTopLeft_Select,
    ETestStepLev3_Groups_Resize_Group1_MovePtTopLeft_TestStepMin,
    ETestStepLev3_Groups_Resize_Group1_MovePtTopLeft_TestStepMax = ETestStepLev3_Groups_Resize_Group1_MovePtTopLeft_TestStepMin + ETestStepLev3_Groups_Resize_Group1_MovePtTopLeft_TestStepCount - 1,
    ETestStepLev3_Groups_Resize_Group1_MovePtTopLeft_Release,
    ETestStepLev3_Groups_Resize_Group1_MovePtTopRight_Select,
    ETestStepLev3_Groups_Resize_Group1_MovePtTopRight_TestStepMin,
    ETestStepLev3_Groups_Resize_Group1_MovePtTopRight_TestStepMax = ETestStepLev3_Groups_Resize_Group1_MovePtTopRight_TestStepMin + ETestStepLev3_Groups_Resize_Group1_MovePtTopRight_TestStepCount - 1,
    ETestStepLev3_Groups_Resize_Group1_MovePtTopRight_Release,
    ETestStepLev3_Groups_Resize_Group1_MovePtBottomLeft_Select,
    ETestStepLev3_Groups_Resize_Group1_MovePtBottomLeft_TestStepMin,
    ETestStepLev3_Groups_Resize_Group1_MovePtBottomLeft_TestStepMax = ETestStepLev3_Groups_Resize_Group1_MovePtBottomLeft_TestStepMin + ETestStepLev3_Groups_Resize_Group1_MovePtBottomLeft_TestStepCount - 1,
    ETestStepLev3_Groups_Resize_Group1_MovePtBottomLeft_Release,
    ETestStepLev3_Groups_Resize_Group1_MovePtBottomCenter_Select,
    ETestStepLev3_Groups_Resize_Group1_MovePtBottomCenter_TestStepMin,
    ETestStepLev3_Groups_Resize_Group1_MovePtBottomCenter_TestStepMax = ETestStepLev3_Groups_Resize_Group1_MovePtBottomCenter_TestStepMin + ETestStepLev3_Groups_Resize_Group1_MovePtBottomCenter_TestStepCount - 1,
    ETestStepLev3_Groups_Resize_Group1_MovePtBottomCenter_Release,
    ETestStepLev3_Groups_Resize_Group1_MovePtTopCenter_Select,
    ETestStepLev3_Groups_Resize_Group1_MovePtTopCenter_TestStepMin,
    ETestStepLev3_Groups_Resize_Group1_MovePtTopCenter_TestStepMax = ETestStepLev3_Groups_Resize_Group1_MovePtTopCenter_TestStepMin + ETestStepLev3_Groups_Resize_Group1_MovePtTopCenter_TestStepCount - 1,
    ETestStepLev3_Groups_Resize_Group1_MovePtTopCenter_Release,
    ETestStepLev3_Groups_Resize_Group1_MovePtRightCenter_Select,
    ETestStepLev3_Groups_Resize_Group1_MovePtRightCenter_TestStepMin,
    ETestStepLev3_Groups_Resize_Group1_MovePtRightCenter_TestStepMax = ETestStepLev3_Groups_Resize_Group1_MovePtRightCenter_TestStepMin + ETestStepLev3_Groups_Resize_Group1_MovePtRightCenter_TestStepCount - 1,
    ETestStepLev3_Groups_Resize_Group1_MovePtRightCenter_Release,
    ETestStepLev3_Groups_Resize_Group1_MovePtLeftCenter_Select,
    ETestStepLev3_Groups_Resize_Group1_MovePtLeftCenter_TestStepMin,
    ETestStepLev3_Groups_Resize_Group1_MovePtLeftCenter_TestStepMax = ETestStepLev3_Groups_Resize_Group1_MovePtLeftCenter_TestStepMin + ETestStepLev3_Groups_Resize_Group1_MovePtLeftCenter_TestStepCount - 1,
    ETestStepLev3_Groups_Resize_Group1_MovePtLeftCenter_Release,
    ETestStepLev3_Groups_Resize_Group1_Finished,
    ETestStepLev3_Groups_Rotate_Group1_ActivateEditModeSelect,
    ETestStepLev3_Groups_Rotate_Group1_Select,
    ETestStepLev3_Groups_Rotate_Group1_MoveSelPtRotateTop_Select,
    ETestStepLev3_Groups_Rotate_Group1_MoveSelPtRotateTop_TestStepMin,
    ETestStepLev3_Groups_Rotate_Group1_MoveSelPtRotateTop_TestStepMax = ETestStepLev3_Groups_Rotate_Group1_MoveSelPtRotateTop_TestStepMin + ETestStepLev3_Groups_Rotate_Group1_MoveSelPtRotateTop_TestStepCount - 1,
    ETestStepLev3_Groups_Rotate_Group1_Release,
    ETestStepLev3_Groups_Rotate_Group1_Finished,
    ETestStepLev3_Groups_RotateMoveResize_Group1_ActivateEditModeSelect,
    ETestStepLev3_Groups_RotateMoveResize_Group1_TestStepMin,
    ETestStepLev3_Groups_RotateMoveResize_Group1_TestStepMax = ETestStepLev3_Groups_RotateMoveResize_Group1_TestStepMin + ETestStepLev3_Groups_RotateMoveResize_Group1_TestStepCount - 1,
    ETestStepLev3_Groups_RotateMoveResize_Group1_Finished,
    ETestStepLev3_Groups_Simulation_ActivateSimulationMode,
    ETestStepLev3_Groups_Simulation_Group1_AddMouseEventFunction,
    ETestStepLev3_Groups_Simulation_Group1_TestStepMin,
    ETestStepLev3_Groups_Simulation_Group1_TestStepMax = ETestStepLev3_Groups_Simulation_Group1_TestStepMin + ETestStepLev3_Groups_Simulation_Group1_TestStepCount - 1,
    ETestStepLev3_Groups_Simulation_Group1_RemoveMouseEventFunction,
    ETestStepLev3_Groups_Simulation_DeactivateSimulationMode,
    ETestStepLev3_Groups_Simulation_Finished,
    ETestStepLev3_Groups_Finished,
    ETestStepLev3_Groups_Count
};

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepStandardShapesGroups( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
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
    QList<CGraphObj*>         arpGraphObjs;
    QList<QGraphicsItem*>     arpGraphicsItems;
    QGraphicsItem*            pGraphicsItem;
    CGraphObj*                pGraphObj;
    CGraphObjGroup*           pGraphObjGroup;
    CGraphObjConnectionLine*  pGraphObjCnctLine;
    CGraphObjConnectionPoint* pGraphObjCnctPt;
    int                       idxGraphObj;

    ZS::Test::CTestStepGroup* pTSGrpGroups                 = getTestStepGroup( "Groups",           i_pTestStepGrpParent );
    ZS::Test::CTestStepGroup* pTSGrpGroupsCreate           = getTestStepGroup( "Create",           pTSGrpGroups );
    ZS::Test::CTestStepGroup* pTSGrpGroupsMove             = getTestStepGroup( "Move",             pTSGrpGroups );
    ZS::Test::CTestStepGroup* pTSGrpGroupsResize           = getTestStepGroup( "Resize",           pTSGrpGroups );
    ZS::Test::CTestStepGroup* pTSGrpGroupsRotate           = getTestStepGroup( "Rotate",           pTSGrpGroups );
    ZS::Test::CTestStepGroup* pTSGrpGroupsRotateMoveResize = getTestStepGroup( "RotateMoveResize", pTSGrpGroups );
    ZS::Test::CTestStepGroup* pTSGrpGroupsSimulation       = getTestStepGroup( "Simulation",       pTSGrpGroups );

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }

    if( !pTSGrpGroups->isEnabled() )
    {
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else if( !pTSGrpGroupsCreate->isEnabled() )
    {
        // If the group is not created none of the other sub tests may be performed.
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else // if( pTSGrpGroups->isEnabled() )
    {
        if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Rect1_ActivateDrawingTool )
        {
            s_rctGroup1 = QRectF();
            s_plgGroup1.clear();
            s_rctGroup1Rect1 = QRectF();
            s_plgGroup1Rect1.clear();
            s_linGroup1SepTopLeft = QLineF();
            s_linGroup1SepTopRight = QLineF();
            s_arrctGroup1CnctPts.clear();
            s_strlstGroup1CnctPtNames.clear();
            s_strlstGroup1GraphObjNames.clear();

            s_arrctNoGroupCnctPts.clear();
            s_strlstNoGroupCnctPtNames.clear();
            s_plgNoGroupCnctLine1Curr.clear();
            s_rctBoundingNoGroupCnctLine1Curr = QRectF();
            s_plgNoGroupCnctLine2Curr.clear();
            s_rctBoundingNoGroupCnctLine2Curr = QRectF();
            s_plgNoGroupCnctLine3Curr.clear();
            s_rctBoundingNoGroupCnctLine3Curr = QRectF();
            s_plgNoGroupCnctLine4Curr.clear();
            s_rctBoundingNoGroupCnctLine4Curr = QRectF();
            s_plgNoGroupCnctLine5Curr.clear();
            s_rctBoundingNoGroupCnctLine5Curr = QRectF();
            s_strlstNoGroupGraphObjNames.clear();


            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( "Group1." + strCurrentGraphObj + ".ActivateDrawingTool", pTSGrpGroupsCreate );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Rect1_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Rect1 )
        {
            m_iRectanglesCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( "Group1." + strCurrentGraphObj + ".Create", pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateGroup1Rect1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctGroup1Rect1 = QRectF( m_ptDrawingSceneMousePos.x(), m_ptDrawingSceneMousePos.y(), 1, 1 );

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1Rect1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Rect1 )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_Group1_Rect1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_Group1_Rect1_MovePtBottomRight_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Rect" + QString::number(m_iRectanglesCount);

            pTestStep = getTestStep( "Group1." + strCurrentGraphObj + ".Create.MovePtBottomRightStep" + QString::number(m_iTestStepLev4), pTSGrpGroupsCreate );

            m_fxOffsHor = s_arsizeCreateGroup1Rect1MovePtBottomRightTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeCreateGroup1Rect1MovePtBottomRightTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctGroup1Rect1.setRight(m_ptDrawingSceneMousePos.x());
            s_rctGroup1Rect1.setBottom(m_ptDrawingSceneMousePos.y());

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1Rect1) );

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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_Group1_Rect1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_Group1_Rect1_MovePtBottomRight_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Rect1_Release )
        {
            strCurrentGraphObj = s_strGroup1RectObjName;

            s_strlstGroup1GraphObjNames.append(strCurrentGraphObj);

            pTestStep = getTestStep( "Group1." + strCurrentGraphObj + ".Release", pTSGrpGroupsCreate );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1Rect1) );

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

            pGraphObj = m_pDrawingScene->findGraphObj( "Rect" + QString::number(m_iRectanglesCount) );

            if( pGraphObj != nullptr )
            {
                pGraphObj->setObjName(strCurrentGraphObj);
                strCurrentGraphObj = pGraphObj->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Rect1_Release )

        if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Line1_ActivateDrawingTool )
        {
            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Group1." + strCurrentGraphObj + ".ActivateDrawingTool", pTSGrpGroupsCreate );

            strlstDesiredValues.append( "EditTool: CreateObjects" );
            strlstDesiredValues.append( "DrawingTool: Line" );

            pTestStep->setOperation( "drawing.setCurrentDrawingTool( Line )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            if( (m_pDrawingScene->getEditTool() != EEditToolCreateObjects) || (m_pDrawingScene->getCurrentDrawingToolGraphObjType() != EGraphObjTypeLine) )
            {
                m_pMainWindow->setCheckedActionDrawStandardShapeLine(true);
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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Line1_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Line1 )
        {
            m_iLinesCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Group1." + strCurrentGraphObj + ".Create", pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateGroup1Line1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            QPointF pt2( m_ptDrawingSceneMousePos.x()+1, m_ptDrawingSceneMousePos.y()+1 );
            s_linGroup1SepTopLeft = QLineF( m_ptDrawingSceneMousePos, pt2 );

            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_linGroup1SepTopLeft) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Line1 )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_Group1_Line1_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_Group1_Line1_MovePt2_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Group1." + strCurrentGraphObj + ".Create.MovePt2Step" + QString::number(m_iTestStepLev4), pTSGrpGroupsCreate );

            m_fxOffsHor = s_arsizeCreateGroup1Line1MovePt2TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeCreateGroup1Line1MovePt2TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_linGroup1SepTopLeft.setP2(m_ptDrawingSceneMousePos);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_linGroup1SepTopLeft) );

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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_Group1_Line1_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_Group1_Line1_MovePt2_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Line1_Release )
        {
            strCurrentGraphObj = s_strGroup1Line1ObjName;

            s_strlstGroup1GraphObjNames.append(strCurrentGraphObj);

            pTestStep = getTestStep( "Group1." + strCurrentGraphObj + ".Release", pTSGrpGroupsCreate );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_linGroup1SepTopLeft) );

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

            pGraphObj = m_pDrawingScene->findGraphObj( "Line" + QString::number(m_iLinesCount) );

            if( pGraphObj != nullptr )
            {
                pGraphObj->setObjName(strCurrentGraphObj);
                pGraphObj->setStackingOrderValue(1.0);
                strCurrentGraphObj = pGraphObj->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Line1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Line2 )
        {
            m_iLinesCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Group1." + strCurrentGraphObj + ".Create", pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateGroup1Line2Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            QPointF pt2( m_ptDrawingSceneMousePos.x()+1, m_ptDrawingSceneMousePos.y()+1 );
            s_linGroup1SepTopRight = QLineF( m_ptDrawingSceneMousePos, pt2 );

            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_linGroup1SepTopRight) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Line2 )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_Group1_Line2_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_Group1_Line2_MovePt2_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Group1." + strCurrentGraphObj + ".Create.MovePt2Step" + QString::number(m_iTestStepLev4), pTSGrpGroupsCreate );

            m_fxOffsHor = s_arsizeCreateGroup1Line2MovePt2TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeCreateGroup1Line2MovePt2TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_linGroup1SepTopRight.setP2(m_ptDrawingSceneMousePos);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_linGroup1SepTopRight) );

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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_Group1_Line2_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_Group1_Line2_MovePt2_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Line2_Release )
        {
            strCurrentGraphObj = s_strGroup1Line2ObjName;

            s_strlstGroup1GraphObjNames.append(strCurrentGraphObj);

            pTestStep = getTestStep( "Group1." + strCurrentGraphObj + ".Release", pTSGrpGroupsCreate );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_linGroup1SepTopRight) );

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

            pGraphObj = m_pDrawingScene->findGraphObj( "Line" + QString::number(m_iLinesCount) );

            if( pGraphObj != nullptr )
            {
                pGraphObj->setObjName(strCurrentGraphObj);
                pGraphObj->setStackingOrderValue(1.0);
                strCurrentGraphObj = pGraphObj->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Line2_Release )

        if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Polygon1_ActivateDrawingTool )
        {
            pTestStep = getTestStep( "ActivateDrawingTool", pTSGrpGroupsCreate );

            strlstDesiredValues.append( "EditTool: CreateObjects" );
            strlstDesiredValues.append( "DrawingTool: Polygon" );

            pTestStep->setOperation( "drawing.setCurrentDrawingTool( Polygon )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            if( (m_pDrawingScene->getEditTool() != EEditToolCreateObjects) || (m_pDrawingScene->getCurrentDrawingToolGraphObjType() != EGraphObjTypePolygon) )
            {
                m_pMainWindow->setCheckedActionDrawStandardShapePolygon(true);
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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Polygon1_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Polygon1_FirstLineSegment )
        {
            m_iPolygonsCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Polygon" + QString::number(m_iPolygonsCount);

            pTestStep = getTestStep( strCurrentGraphObj + "FirstLineSegment", pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateGroup1Polygon1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgGroup1Triangle.clear();
            s_plgGroup1Triangle.append(m_ptDrawingSceneMousePos);
            s_plgGroup1Triangle.append(m_ptDrawingSceneMousePos);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgGroup1Triangle) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Polygon1_FirstLineSegment )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_Group1_Polygon1_FirstLineSegment_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_Group1_Polygon1_FirstLineSegment_MovePt2_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Polygon" + QString::number(m_iPolygonsCount);

            pTestStep = getTestStep( strCurrentGraphObj + "FirstLineSegmentMovePt2Step" + QString::number(m_iTestStepLev4), pTSGrpGroupsCreate );

            m_fxOffsHor = s_arsizePolygonsCreateGroup1Polygon1FirstLineSegmentMovePt2TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolygonsCreateGroup1Polygon1FirstLineSegmentMovePt2TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgGroup1Triangle[1] = m_ptDrawingSceneMousePos;

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgGroup1Triangle) );

            pTestStep->setOperation( "drawing.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingObject( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
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
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_Group1_Polygon1_FirstLineSegment_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_Group1_Polygon1_FirstLineSegment_MovePt2_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Polygon1_SecondLineSegment )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Polygon" + QString::number(m_iPolygonsCount);

            pTestStep = getTestStep( strCurrentGraphObj + "SecondLineSegment", pTSGrpGroupsCreate );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgGroup1Triangle[1] = m_ptDrawingSceneMousePos;
            s_plgGroup1Triangle.append(m_ptDrawingSceneMousePos);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgGroup1Triangle) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingPathSegmentFinished( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Polygon1_SecondLineSegment )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_Group1_Polygon1_SecondLineSegment_MovePt3_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_Group1_Polygon1_SecondLineSegment_MovePt3_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Polygon" + QString::number(m_iPolygonsCount);

            pTestStep = getTestStep( strCurrentGraphObj + "SecondLineSegmentMovePt3Step" + QString::number(m_iTestStepLev4), pTSGrpGroupsCreate );

            m_fxOffsHor = s_arsizePolygonsCreateGroup1Polygon1SecondLineSegmentMovePt3TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolygonsCreateGroup1Polygon1SecondLineSegmentMovePt3TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgGroup1Triangle[2] = m_ptDrawingSceneMousePos;

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgGroup1Triangle) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingObject( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
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
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjCreating->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjCreating->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_Group1_Polygon1_SecondLineSegment_MovePt3_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_Group1_Polygon1_SecondLineSegment_MovePt3_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Polygon1_Release )
        {
            strCurrentGraphObj = s_strGroup1TriangleObjName;

            s_strlstGroup1GraphObjNames.append(strCurrentGraphObj);

            pTestStep = getTestStep( "Group1." + strCurrentGraphObj + ".Release", pTSGrpGroupsCreate );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgGroup1Triangle[2] = m_ptDrawingSceneMousePos;

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgGroup1Triangle) );

            pTestStep->setOperation( "view.mouseDoubleClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingObjectFinished( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

            m_pGraphObjCreating = nullptr;

            pGraphObj = m_pDrawingScene->findGraphObj( "Polygon" + QString::number(m_iPolygonsCount) );

            if( pGraphObj != nullptr )
            {
                pGraphObj->setObjName(strCurrentGraphObj);
                pGraphObj->setStackingOrderValue(1.0);
                strCurrentGraphObj = pGraphObj->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_Polygon1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctPts_ActivateDrawingTool )
        {
            m_iTestStepLev4 = -1;

            pTestStep = getTestStep( "Group1.CnctPts.ActivateDrawingTool", pTSGrpGroupsCreate );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctPts_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctPt1 )
        {
            m_iConnectionPointsCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGroup1CnctPt1ObjName;

            s_strlstGroup1GraphObjNames.append(strCurrentGraphObj);

            pTestStep = getTestStep( strCurrentGraphObj + ".Create", pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateGroup1CnctPt1.toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            QRectF rctCnctPt = QRectF(
                /* left   */ m_ptDrawingSceneMousePos.x()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* top    */ m_ptDrawingSceneMousePos.y()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* width  */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* height */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx() );
            s_arrctGroup1CnctPts.append(rctCnctPt);
            s_strlstGroup1CnctPtNames.append(strCurrentGraphObj);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_arrctGroup1CnctPts[0]) );

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

            m_pGraphObjCreating = nullptr;

            pGraphObj = m_pDrawingScene->findGraphObj( "ConnectionPoint" + QString::number(m_iConnectionPointsCount) );

            if( pGraphObj != nullptr )
            {
                pGraphObj->setObjName(strCurrentGraphObj);
                pGraphObj->setStackingOrderValue(1.0);
                strCurrentGraphObj = pGraphObj->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctPt1 )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctPt2 )
        {
            m_iConnectionPointsCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGroup1CnctPt2ObjName;

            s_strlstGroup1GraphObjNames.append(strCurrentGraphObj);

            pTestStep = getTestStep( strCurrentGraphObj + ".Create", pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateGroup1CnctPt2.toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            QRectF rctCnctPt = QRectF(
                /* left   */ m_ptDrawingSceneMousePos.x()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* top    */ m_ptDrawingSceneMousePos.y()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* width  */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* height */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx() );
            s_arrctGroup1CnctPts.append(rctCnctPt);
            s_strlstGroup1CnctPtNames.append(strCurrentGraphObj);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_arrctGroup1CnctPts[1]) );

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

            m_pGraphObjCreating = nullptr;

            pGraphObj = m_pDrawingScene->findGraphObj( "ConnectionPoint" + QString::number(m_iConnectionPointsCount) );

            if( pGraphObj != nullptr )
            {
                pGraphObj->setObjName(strCurrentGraphObj);
                pGraphObj->setStackingOrderValue(1.0);
                strCurrentGraphObj = pGraphObj->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctPt2 )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctPt3 )
        {
            m_iConnectionPointsCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGroup1CnctPt3ObjName;

            s_strlstGroup1GraphObjNames.append(strCurrentGraphObj);

            pTestStep = getTestStep( strCurrentGraphObj + ".Create", pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateGroup1CnctPt3.toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            QRectF rctCnctPt = QRectF(
                /* left   */ m_ptDrawingSceneMousePos.x()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* top    */ m_ptDrawingSceneMousePos.y()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* width  */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* height */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx() );
            s_arrctGroup1CnctPts.append(rctCnctPt);
            s_strlstGroup1CnctPtNames.append(strCurrentGraphObj);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_arrctGroup1CnctPts[2]) );

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

            m_pGraphObjCreating = nullptr;

            pGraphObj = m_pDrawingScene->findGraphObj( "ConnectionPoint" + QString::number(m_iConnectionPointsCount) );

            if( pGraphObj != nullptr )
            {
                pGraphObj->setObjName(strCurrentGraphObj);
                pGraphObj->setStackingOrderValue(1.0);
                strCurrentGraphObj = pGraphObj->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctPt3 )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctPt4 )
        {
            m_iConnectionPointsCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGroup1CnctPt4ObjName;

            s_strlstGroup1GraphObjNames.append(strCurrentGraphObj);

            pTestStep = getTestStep( strCurrentGraphObj + ".Create", pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateGroup1CnctPt4.toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            QRectF rctCnctPt = QRectF(
                /* left   */ m_ptDrawingSceneMousePos.x()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* top    */ m_ptDrawingSceneMousePos.y()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* width  */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* height */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx() );
            s_arrctGroup1CnctPts.append(rctCnctPt);
            s_strlstGroup1CnctPtNames.append(strCurrentGraphObj);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_arrctGroup1CnctPts[3]) );

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

            m_pGraphObjCreating = nullptr;

            pGraphObj = m_pDrawingScene->findGraphObj( "ConnectionPoint" + QString::number(m_iConnectionPointsCount) );

            if( pGraphObj != nullptr )
            {
                pGraphObj->setObjName(strCurrentGraphObj);
                pGraphObj->setStackingOrderValue(1.0);
                strCurrentGraphObj = pGraphObj->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctPt4 )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_ActivateDrawingTool )
        {
            pTestStep = getTestStep( "Group1.ConnectionLine1.ActivateDrawingTool", pTSGrpGroupsCreate );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_ConnectAtPt1 )
        {
            m_iConnectionLinesCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( strCurrentGraphObj + ".CreateAtPt1", pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos = s_arrctGroup1CnctPts[0].center().toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgNoGroupCnctLine1Curr.clear();
            s_plgNoGroupCnctLine1Curr.append(m_ptDrawingSceneMousePos);
            s_plgNoGroupCnctLine1Curr.append(m_ptDrawingSceneMousePos);
            s_rctBoundingNoGroupCnctLine1Curr = s_plgNoGroupCnctLine1Curr.boundingRect();

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgNoGroupCnctLine1Curr) );

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

            pGraphObj = nullptr;
            arpGraphicsItems = m_pDrawingScene->items();

            if( arpGraphicsItems.size() > 0 )
            {
                CGraphObj* pGraphObjTmp;

                for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
                {
                    pGraphicsItem = arpGraphicsItems[idxGraphObj];
                    pGraphObjTmp = dynamic_cast<CGraphObj*>(pGraphicsItem);

                    if( pGraphObjTmp->getObjName() == strCurrentGraphObj )
                    {
                        pGraphObj = pGraphObjTmp;
                        break;
                    }
                }
            }

            if( pGraphObj != nullptr )
            {
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_ConnectAtPt1 )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_MoveToPt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_MoveToPt2_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "Group1." + strCurrentGraphObj + ".MoveToPt2.Step" + QString::number(m_iTestStepLev4), pTSGrpGroupsCreate );

            m_fxOffsHor = s_arsizeCreateGroup1CnctLine1Pt1Pt2MoveToPt2TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeCreateGroup1CnctLine1Pt1Pt2MoveToPt2TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgNoGroupCnctLine1Curr[1] = m_ptDrawingSceneMousePos;
            s_rctBoundingNoGroupCnctLine1Curr = s_plgNoGroupCnctLine1Curr.boundingRect();

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgNoGroupCnctLine1Curr) );

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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_MoveToPt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_MoveToPt2_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_ConnectAtPt2 )
        {
            strCurrentGraphObj = s_strGroup1CnctLineObjName;

            // Connection lines don't belong to groups (their connection points may belong to different groups).
            //s_strlstGroup1GraphObjNames.append(strCurrentGraphObj);

            pTestStep = getTestStep( "Group1." + strCurrentGraphObj + ".ConnectAtPt2", pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos = s_arrctGroup1CnctPts[1].center().toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( s_strGroup1CnctLineObjName + ": " + polygon2Str(s_plgNoGroupCnctLine1Curr) );
            strlstDesiredValues.append( s_strGroup1CnctPt1ObjName + ": " + polygon2Str(s_arrctGroup1CnctPts[0]) );
            strlstDesiredValues.append( s_strGroup1CnctPt2ObjName + ": " + polygon2Str(s_arrctGroup1CnctPts[1]) );

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

            pGraphObj = m_pDrawingScene->findGraphObj( "ConnectionLine" + QString::number(m_iConnectionLinesCount) );

            pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj);

            if( pGraphObjCnctLine != nullptr )
            {
                pGraphObjCnctLine->setObjName(strCurrentGraphObj);
                pGraphObjCnctLine->setObjId(s_strGroup1CnctLineObjId);
                pGraphObjCnctLine->setStackingOrderValue(2.0);
                strlstActualValues.append( pGraphObjCnctLine->getObjName() + ": " + pGraphObjCnctLine->getScenePolygonShapePointsString() );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_CnctLine1Pt1Pt2_ConnectAtPt2 )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_SelectObjects_ActivateDrawingTool )
        {
            pTestStep = getTestStep( "Group1.SelectObjects.ActivateDrawingTool", pTSGrpGroupsCreate );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_SelectObjects_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_SelectObjects_Start )
        {
            m_iTestStepLev4 = -1;

            pTestStep = getTestStep( "Group1.SelectObjects.Start", pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos.setX( s_rctGroup1Rect1.left() - s_sizeCreateGroup1SelectObjectsOffset.width() );
            m_ptDrawingSceneMousePos.setY( s_rctGroup1Rect1.top() - s_sizeCreateGroup1SelectObjectsOffset.height() );

            m_fxOffsHor = (s_rctGroup1Rect1.right() + s_sizeCreateGroup1SelectObjectsOffset.width() - m_ptDrawingSceneMousePos.x()) / ETestStepLev3_Groups_Create_Group1_SelectObjects_TestStepCount;
            m_fyOffsHor = (s_rctGroup1Rect1.bottom() + s_sizeCreateGroup1SelectObjectsOffset.height() - m_ptDrawingSceneMousePos.y()) / ETestStepLev3_Groups_Create_Group1_SelectObjects_TestStepCount;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene.Group1.select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_SelectObjects_Start )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_Group1_SelectObjects_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_Group1_SelectObjects_TestStepMax )
        {
            m_iTestStepLev4++;

            pTestStep = getTestStep( "Group1.SelectObjects.Step" + QString::number(m_iTestStepLev4), pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene.Group1.select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pMouseEv = new QMouseEvent(
                /* type      */ QEvent::MouseMove,
                /* pos       */ m_ptDrawingViewMousePos,
                /* button    */ Qt::NoButton,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseMoveEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_Group1_SelectObjects_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_Group1_SelectObjects_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_SelectObjects_Release )
        {
            pTestStep = getTestStep( "Group1.SelectObjects.Release", pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos.setX( s_rctGroup1Rect1.right() + s_sizeCreateGroup1SelectObjectsOffset.width() );
            m_ptDrawingSceneMousePos.setY( s_rctGroup1Rect1.bottom() + s_sizeCreateGroup1SelectObjectsOffset.height() );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            for( idxGraphObj = 0; idxGraphObj < s_strlstGroup1GraphObjNames.size(); idxGraphObj++ )
            {
                strlstDesiredValues.append( s_strlstGroup1GraphObjNames[idxGraphObj] + ".isSelected" );
            }

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene.Group1.select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

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

            QList<QGraphicsItem*> arpGraphicItemsSelected = m_pDrawingScene->selectedItems();

            for( idxGraphObj = 0; idxGraphObj < s_strlstGroup1GraphObjNames.size(); idxGraphObj++ )
            {
                strCurrentGraphObj = s_strlstGroup1GraphObjNames[idxGraphObj];

                for( int idxGraphObj2 = 0; idxGraphObj2 < arpGraphicItemsSelected.size(); idxGraphObj2++ )
                {
                    pGraphicsItem = arpGraphicItemsSelected[idxGraphObj2];

                    pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                    if( pGraphObj != nullptr && pGraphObj->getObjName() == strCurrentGraphObj )
                    {
                        strlstActualValues.append( strCurrentGraphObj + ".isSelected" );
                        break;
                    }
                }
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_SelectObjects_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_GroupObjects )
        {
            m_iGroupsCount++;

            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + ".GroupObjects", pTSGrpGroupsCreate );

            s_rctGroup1 = s_rctGroup1Rect1;

            for( idxGraphObj = 0; idxGraphObj < s_arrctGroup1CnctPts.size(); idxGraphObj++ )
            {
                s_rctGroup1 |= s_arrctGroup1CnctPts[idxGraphObj];
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

            for( idxGraphObj = 0; idxGraphObj < s_strlstGroup1GraphObjNames.size(); idxGraphObj++ )
            {
                strlstDesiredValues.append( strCurrentGraphObj + "." + s_strlstGroup1GraphObjNames[idxGraphObj] );
            }

            pTestStep->setOperation( "drawing.groupObjects()" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            m_pMainWindow->triggerActionEditGroup();

            pGraphObj = m_pDrawingScene->findGraphObj( "Group" + QString::number(m_iGroupsCount) );

            pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphObj);

            if( pGraphObjGroup != nullptr )
            {
                pGraphObjGroup->setObjName(strCurrentGraphObj);
                pGraphObjGroup->setObjId(strCurrentGraphObj);
                strlstActualValues.append( pGraphObjGroup->getObjName() + ": " + pGraphObjGroup->getScenePolygonShapePointsString() );

                QList<QGraphicsItem*> arpGraphObjsChilds = pGraphObjGroup->childItems();

                for( idxGraphObj = 0; idxGraphObj < s_strlstGroup1GraphObjNames.size(); idxGraphObj++ )
                {
                    strCurrentGraphObj = s_strlstGroup1GraphObjNames[idxGraphObj];

                    for( int idxGraphObj2 = 0; idxGraphObj2 < arpGraphObjsChilds.size(); idxGraphObj2++ )
                    {
                        pGraphicsItem = arpGraphObjsChilds[idxGraphObj2];

                        pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                        if( pGraphObj != nullptr && pGraphObj->getObjName() == strCurrentGraphObj )
                        {
                            strlstActualValues.append( pGraphObjGroup->getObjName() + "." + pGraphObj->getObjName() );
                            break;
                        }
                    }
                }
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Group1_GroupObjects )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctPts_ActivateDrawingTool )
        {
            m_iTestStepLev4 = -1;

            pTestStep = getTestStep( "NoGroup.CnctPts.ActivateDrawingTool", pTSGrpGroupsCreate );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctPts_ActivateDrawingTool )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_NoGroup_CnctPts_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_NoGroup_CnctPts_TestStepMax )
        {
            m_iConnectionPointsCount++;
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionPoint" + QString::number(m_iConnectionPointsCount);

            s_strlstNoGroupGraphObjNames.append(strCurrentGraphObj);

            pTestStep = getTestStep( "NoGroup." + strCurrentGraphObj + ".Create", pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos = s_arptDrawingSceneMousePosCreateNoGroupCnctPts[m_iTestStepLev4].toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            QRectF rctCnctPt = QRectF(
                /* left   */ m_ptDrawingSceneMousePos.x()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* top    */ m_ptDrawingSceneMousePos.y()-CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* width  */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx(),
                /* height */ 2.0*CGraphObjConnectionPoint::GetDefaultOuterCircleRadiusInPx() );
            s_arrctNoGroupCnctPts.append(rctCnctPt);
            s_strlstNoGroupCnctPtNames.append(strCurrentGraphObj);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_arrctNoGroupCnctPts[m_iTestStepLev4]) );

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

            QList<QGraphicsItem*> arpGraphObjs = m_pDrawingScene->items(m_ptDrawingSceneMousePos);

            if( arpGraphObjs.size() >= 1 )
            {
                pGraphObjCreated = dynamic_cast<CGraphObj*>(arpGraphObjs[0]);
                strCurrentGraphObj = pGraphObjCreated->getObjName();
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObjCreated->getScenePolygonShapePointsString() );
                strToolTip = pGraphObjCreated->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_NoGroup_CnctPts_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_NoGroup_CnctPts_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_ActivateDrawingTool )
        {
            pTestStep = getTestStep( "NoGroup.ConnectionLine2.ActivateDrawingTool", pTSGrpGroupsCreate );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_ConnectAtPt5 )
        {
            m_iConnectionLinesCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "NoGroup." + strCurrentGraphObj + ".CreateAtPt5", pTSGrpGroupsCreate );

            QPointF pt1 = s_arrctGroup1CnctPts[0].center();
            QPointF pt5 = s_arrctNoGroupCnctPts[0].center();

            double fdx = pt1.x() - pt5.x();
            double fdy = pt1.y() - pt5.y();

            m_fxOffsHor = fdx / ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_MoveToPt1_TestStepCount;
            m_fyOffsHor = fdy / ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_MoveToPt1_TestStepCount;

            m_ptDrawingSceneMousePos = pt5.toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgNoGroupCnctLine2Curr.clear();
            s_plgNoGroupCnctLine2Curr.append(m_ptDrawingSceneMousePos);
            s_plgNoGroupCnctLine2Curr.append(m_ptDrawingSceneMousePos);
            s_rctBoundingNoGroupCnctLine2Curr = s_plgNoGroupCnctLine2Curr.boundingRect();

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgNoGroupCnctLine2Curr) );

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

            pGraphObj = nullptr;
            arpGraphicsItems = m_pDrawingScene->items();

            if( arpGraphicsItems.size() > 0 )
            {
                CGraphObj* pGraphObjTmp;

                for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
                {
                    pGraphicsItem = arpGraphicsItems[idxGraphObj];
                    pGraphObjTmp = dynamic_cast<CGraphObj*>(pGraphicsItem);

                    if( pGraphObjTmp->getObjName() == strCurrentGraphObj )
                    {
                        pGraphObj = pGraphObjTmp;
                        break;
                    }
                }
            }

            if( pGraphObj != nullptr )
            {
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_ConnectAtPt5 )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_MoveToPt1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_MoveToPt1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "NoGroup." + strCurrentGraphObj + ".MoveToPt1.Step" + QString::number(m_iTestStepLev4), pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgNoGroupCnctLine2Curr[1] = m_ptDrawingSceneMousePos;
            s_rctBoundingNoGroupCnctLine2Curr = s_plgNoGroupCnctLine2Curr.boundingRect();

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgNoGroupCnctLine2Curr) );

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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_MoveToPt1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_MoveToPt1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_ConnectAtPt1 )
        {
            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            // Connection lines don't belong to groups (their connection points may belong to different groups).
            //s_strlstNoGroupGraphObjNames.append(strCurrentGraphObj);

            pTestStep = getTestStep( "NoGroup." + strCurrentGraphObj + ".ConnectAtPt1", pTSGrpGroupsCreate );

            QPointF pt1 = s_arrctGroup1CnctPts[0].center();
            //QPointF pt5 = s_arrctNoGroupCnctPts[0].center();

            m_ptDrawingSceneMousePos = pt1.toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgNoGroupCnctLine2Curr[1] = m_ptDrawingSceneMousePos;
            s_rctBoundingNoGroupCnctLine2Curr = s_plgNoGroupCnctLine2Curr.boundingRect();

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgNoGroupCnctLine2Curr) );
            strlstDesiredValues.append( s_strlstNoGroupCnctPtNames[0] + ": " + polygon2Str(s_arrctNoGroupCnctPts[0]) );
            strlstDesiredValues.append( s_strlstGroup1CnctPtNames[0] + ": " + polygon2Str(s_arrctGroup1CnctPts[0]) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine2Pt5Pt1_ConnectAtPt1 )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_ActivateDrawingTool )
        {
            pTestStep = getTestStep( "NoGroup.ConnectionLine3.ActivateDrawingTool", pTSGrpGroupsCreate );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_ConnectAtPt2 )
        {
            m_iConnectionLinesCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "NoGroup." + strCurrentGraphObj + ".CreateAtPt2", pTSGrpGroupsCreate );

            QPointF pt2 = s_arrctGroup1CnctPts[1].center();
            QPointF pt6 = s_arrctNoGroupCnctPts[1].center();

            double fdx = pt6.x() - pt2.x();
            double fdy = pt6.y() - pt2.y();

            m_fxOffsHor = fdx / ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_MoveToPt6_TestStepCount;
            m_fyOffsHor = fdy / ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_MoveToPt6_TestStepCount;

            m_ptDrawingSceneMousePos = pt2.toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgNoGroupCnctLine3Curr.clear();
            s_plgNoGroupCnctLine3Curr.append(m_ptDrawingSceneMousePos);
            s_plgNoGroupCnctLine3Curr.append(m_ptDrawingSceneMousePos);
            s_rctBoundingNoGroupCnctLine3Curr = s_plgNoGroupCnctLine3Curr.boundingRect();

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgNoGroupCnctLine3Curr) );

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

            pGraphObj = nullptr;
            arpGraphicsItems = m_pDrawingScene->items();

            if( arpGraphicsItems.size() > 0 )
            {
                CGraphObj* pGraphObjTmp;

                for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
                {
                    pGraphicsItem = arpGraphicsItems[idxGraphObj];
                    pGraphObjTmp = dynamic_cast<CGraphObj*>(pGraphicsItem);

                    if( pGraphObjTmp->getObjName() == strCurrentGraphObj )
                    {
                        pGraphObj = pGraphObjTmp;
                        break;
                    }
                }
            }

            if( pGraphObj != nullptr )
            {
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_ConnectAtPt2 )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_MoveToPt6_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_MoveToPt6_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "NoGroup." + strCurrentGraphObj + ".MoveToPt6.Step" + QString::number(m_iTestStepLev4), pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgNoGroupCnctLine3Curr[1] = m_ptDrawingSceneMousePos;
            s_rctBoundingNoGroupCnctLine3Curr = s_plgNoGroupCnctLine3Curr.boundingRect();

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgNoGroupCnctLine3Curr) );

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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_MoveToPt6_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_MoveToPt6_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_ConnectAtPt6 )
        {
            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            // Connection lines don't belong to groups (their connection points may belong to different groups).
            //s_strlstNoGroupGraphObjNames.append(strCurrentGraphObj);

            pTestStep = getTestStep( "NoGroup." + strCurrentGraphObj + ".ConnectAtPt6", pTSGrpGroupsCreate );

            //QPointF pt2 = s_arrctGroup1CnctPts[1].center();
            QPointF pt6 = s_arrctNoGroupCnctPts[1].center();

            m_ptDrawingSceneMousePos = pt6.toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgNoGroupCnctLine3Curr[1] = m_ptDrawingSceneMousePos;
            s_rctBoundingNoGroupCnctLine3Curr = s_plgNoGroupCnctLine3Curr.boundingRect();

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgNoGroupCnctLine3Curr) );
            strlstDesiredValues.append( s_strlstGroup1CnctPtNames[1] + ": " + polygon2Str(s_arrctGroup1CnctPts[1]) );
            strlstDesiredValues.append( s_strlstNoGroupCnctPtNames[1] + ": " + polygon2Str(s_arrctNoGroupCnctPts[1]) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine3Pt2Pt6_ConnectAtPt6 )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_ActivateDrawingTool )
        {
            pTestStep = getTestStep( "NoGroup.ConnectionLine4.ActivateDrawingTool", pTSGrpGroupsCreate );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_ConnectAtPt3 )
        {
            m_iConnectionLinesCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "NoGroup." + strCurrentGraphObj + ".CreateAtPt3", pTSGrpGroupsCreate );

            QPointF pt3 = s_arrctGroup1CnctPts[2].center();
            QPointF pt7 = s_arrctNoGroupCnctPts[2].center();

            double fdx = pt7.x() - pt3.x();
            double fdy = pt7.y() - pt3.y();

            m_fxOffsHor = fdx / ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_MoveToPt7_TestStepCount;
            m_fyOffsHor = fdy / ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_MoveToPt7_TestStepCount;

            m_ptDrawingSceneMousePos = pt3.toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgNoGroupCnctLine4Curr.clear();
            s_plgNoGroupCnctLine4Curr.append(m_ptDrawingSceneMousePos);
            s_plgNoGroupCnctLine4Curr.append(m_ptDrawingSceneMousePos);
            s_rctBoundingNoGroupCnctLine4Curr = s_plgNoGroupCnctLine4Curr.boundingRect();

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgNoGroupCnctLine4Curr) );

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

            pGraphObj = nullptr;
            arpGraphicsItems = m_pDrawingScene->items();

            if( arpGraphicsItems.size() > 0 )
            {
                CGraphObj* pGraphObjTmp;

                for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
                {
                    pGraphicsItem = arpGraphicsItems[idxGraphObj];
                    pGraphObjTmp = dynamic_cast<CGraphObj*>(pGraphicsItem);

                    if( pGraphObjTmp->getObjName() == strCurrentGraphObj )
                    {
                        pGraphObj = pGraphObjTmp;
                        break;
                    }
                }
            }

            if( pGraphObj != nullptr )
            {
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_ConnectAtPt3 )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_MoveToPt7_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_MoveToPt7_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "NoGroup." + strCurrentGraphObj + ".MoveToPt7.Step" + QString::number(m_iTestStepLev4), pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgNoGroupCnctLine4Curr[1] = m_ptDrawingSceneMousePos;
            s_rctBoundingNoGroupCnctLine4Curr = s_plgNoGroupCnctLine4Curr.boundingRect();

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgNoGroupCnctLine4Curr) );

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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_MoveToPt7_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_MoveToPt7_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_ConnectAtPt7 )
        {
            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            // Connection lines don't belong to groups (their connection points may belong to different groups).
            //s_strlstNoGroupGraphObjNames.append(strCurrentGraphObj);

            pTestStep = getTestStep( "NoGroup." + strCurrentGraphObj + ".ConnectAtPt7", pTSGrpGroupsCreate );

            //QPointF pt3 = s_arrctGroup1CnctPts[2].center();
            QPointF pt7 = s_arrctNoGroupCnctPts[2].center();

            m_ptDrawingSceneMousePos = pt7.toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgNoGroupCnctLine4Curr[1] = m_ptDrawingSceneMousePos;
            s_rctBoundingNoGroupCnctLine4Curr = s_plgNoGroupCnctLine4Curr.boundingRect();

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgNoGroupCnctLine4Curr) );
            strlstDesiredValues.append( s_strlstGroup1CnctPtNames[2] + ": " + polygon2Str(s_arrctGroup1CnctPts[2]) );
            strlstDesiredValues.append( s_strlstNoGroupCnctPtNames[2] + ": " + polygon2Str(s_arrctNoGroupCnctPts[2]) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine4Pt3Pt7_ConnectAtPt7 )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_ActivateDrawingTool )
        {
            pTestStep = getTestStep( "NoGroup.ConnectionLine5.ActivateDrawingTool", pTSGrpGroupsCreate );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_ConnectAtPt5 )
        {
            m_iConnectionLinesCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "NoGroup." + strCurrentGraphObj + ".CreateAtPt5", pTSGrpGroupsCreate );

            QPointF pt4 = s_arrctGroup1CnctPts[3].center();
            QPointF pt8 = s_arrctNoGroupCnctPts[3].center();

            double fdx = pt8.x() - pt4.x();
            double fdy = pt8.y() - pt4.y();

            m_fxOffsHor = fdx / ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_MoveToPt8_TestStepCount;
            m_fyOffsHor = fdy / ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_MoveToPt8_TestStepCount;

            m_ptDrawingSceneMousePos = pt4.toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgNoGroupCnctLine5Curr.clear();
            s_plgNoGroupCnctLine5Curr.append(m_ptDrawingSceneMousePos);
            s_plgNoGroupCnctLine5Curr.append(m_ptDrawingSceneMousePos);
            s_rctBoundingNoGroupCnctLine5Curr = s_plgNoGroupCnctLine5Curr.boundingRect();

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgNoGroupCnctLine5Curr) );

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

            pGraphObj = nullptr;
            arpGraphicsItems = m_pDrawingScene->items();

            if( arpGraphicsItems.size() > 0 )
            {
                CGraphObj* pGraphObjTmp;

                for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
                {
                    pGraphicsItem = arpGraphicsItems[idxGraphObj];
                    pGraphObjTmp = dynamic_cast<CGraphObj*>(pGraphicsItem);

                    if( pGraphObjTmp->getObjName() == strCurrentGraphObj )
                    {
                        pGraphObj = pGraphObjTmp;
                        break;
                    }
                }
            }

            if( pGraphObj != nullptr )
            {
                strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_ConnectAtPt5 )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_MoveToPt8_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_MoveToPt8_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            pTestStep = getTestStep( "NoGroup." + strCurrentGraphObj + ".MoveToPt8.Step" + QString::number(m_iTestStepLev4), pTSGrpGroupsCreate );

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgNoGroupCnctLine5Curr[1] = m_ptDrawingSceneMousePos;
            s_rctBoundingNoGroupCnctLine5Curr = s_plgNoGroupCnctLine5Curr.boundingRect();

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgNoGroupCnctLine5Curr) );

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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_MoveToPt8_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_MoveToPt8_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_ConnectAtPt8 )
        {
            strCurrentGraphObj = "ConnectionLine" + QString::number(m_iConnectionLinesCount);

            // Connection lines don't belong to groups (their connection points may belong to different groups).
            //s_strlstNoGroupGraphObjNames.append(strCurrentGraphObj);

            pTestStep = getTestStep( "NoGroup." + strCurrentGraphObj + ".ConnectAtPt8", pTSGrpGroupsCreate );

            //QPointF pt4 = s_arrctGroup1CnctPts[3].center();
            QPointF pt8 = s_arrctNoGroupCnctPts[3].center();

            m_ptDrawingSceneMousePos = pt8.toPoint();

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgNoGroupCnctLine5Curr[1] = m_ptDrawingSceneMousePos;
            s_rctBoundingNoGroupCnctLine5Curr = s_plgNoGroupCnctLine5Curr.boundingRect();

            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgNoGroupCnctLine5Curr) );
            strlstDesiredValues.append( s_strlstGroup1CnctPtNames[3] + ": " + polygon2Str(s_arrctGroup1CnctPts[3]) );
            strlstDesiredValues.append( s_strlstNoGroupCnctPtNames[3] + ": " + polygon2Str(s_arrctNoGroupCnctPts[3]) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_NoGroup_CnctLine5Pt4Pt8_ConnectAtPt8 )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Finished )
        {
            pTSGrpGroupsCreate->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Create_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Move_Group1_ActivateDrawingTool )
        {
            if( !pTSGrpGroupsMove->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Groups_Move_Group1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpGroupsMove->isEnabled() )
            {
                m_iTestStepLev4 = -1;

                pTestStep = getTestStep( "Move.Group1.ActivateDrawingTool", pTSGrpGroupsMove );

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

            } // if( pTSGrpGroupsMove->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Move_Group1_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Move_Group1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGroup1ObjName;

            s_rctGroup1OnMousePressEvent = s_rctGroup1;

            pTestStep = getTestStep( strCurrentGraphObj, pTSGrpGroupsMove );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosMoveGroup1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointCenter) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Move_Group1_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Move_Group1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Move_Group1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + ".Move.Step" + QString::number(m_iTestStepLev4), pTSGrpGroupsMove );

            m_fxOffsHor = s_arsizeMoveGroup1TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeMoveGroup1TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctGroup1.moveLeft( s_rctGroup1.left() + m_fxOffsHor );
            s_rctGroup1.moveTop( s_rctGroup1.top() + m_fyOffsHor );

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Move_Group1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Move_Group1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Move_Group1_Release )
        {
            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "Release", pTSGrpGroupsMove );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Move_Group1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Move_Group1_Finished )
        {
            pTSGrpGroupsMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Move_Group1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_ActivateEditModeSelect )
        {
            if( !pTSGrpGroupsResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Groups_Resize_Group1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpGroupsResize->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpGroupsResize );

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

            } // if( pTSGrpGroupsResize->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGroup1ObjName;

            s_rctGroup1OnMousePressEvent = s_rctGroup1;

            pTestStep = getTestStep( strCurrentGraphObj + ".Select", pTSGrpGroupsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeGroup1Select;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_Select )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtBottomRight_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGroup1ObjName;

            s_rctGroup1OnMousePressEvent = s_rctGroup1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightSelect", pTSGrpGroupsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtBottomRightStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointBottomRight) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtBottomRight_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtBottomRight_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightStep" + QString::number(m_iTestStepLev4), pTSGrpGroupsResize );

            m_fxOffsHor = s_arsizeGroupsResizeGroup1MovePtBottomRightTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeGroupsResizeGroup1MovePtBottomRightTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctGroup1.setRight(m_ptDrawingSceneMousePos.x());
            s_rctGroup1.setBottom(m_ptDrawingSceneMousePos.y());

            // Keep old size before resizing starts as the object may get 0 pixel size.
            // The size will be adjusted on releasing the mouse.
            QRectF rct = s_rctGroup1;

            if( fabs(s_rctGroup1.width()) > 0.0 || fabs(s_rctGroup1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtBottomRight_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtBottomRight_Release )
        {
            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightRelease", pTSGrpGroupsResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            // Old size before resizing starts has been kept as the object may get 0 pixel size.
            // The size will be adjusted on releasing the mouse.
            if( s_rctGroup1.width() < 0 )
            {
                double fLeft = s_rctGroup1.left();
                double fRight = s_rctGroup1.right();
                s_rctGroup1.setLeft(fRight);
                s_rctGroup1.setRight(fLeft);
            }
            if( s_rctGroup1.height() < 0 )
            {
                double fTop = s_rctGroup1.top();
                double fBottom = s_rctGroup1.bottom();
                s_rctGroup1.setTop(fBottom);
                s_rctGroup1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1.topLeft()) ); // on releasing the mouse the item's position is changed
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtBottomRight_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtTopLeft_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGroup1ObjName;

            s_rctGroup1OnMousePressEvent = s_rctGroup1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopLeftSelect", pTSGrpGroupsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtTopLeftStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointTopLeft) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtTopLeft_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtTopLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtTopLeft_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopLeftStep" + QString::number(m_iTestStepLev4), pTSGrpGroupsResize );

            m_fxOffsHor = s_arsizeGroupsResizeGroup1MovePtTopLeftTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeGroupsResizeGroup1MovePtTopLeftTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctGroup1.setLeft( s_rctGroup1.left() + m_fxOffsHor );
            s_rctGroup1.setTop( s_rctGroup1.top() + m_fyOffsHor );

            QRectF rct = s_rctGroup1;

            if( fabs(s_rctGroup1.width()) > 0.0 || fabs(s_rctGroup1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtTopLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtTopLeft_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtTopLeft_Release )
        {
            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopLeftRelease", pTSGrpGroupsResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rctGroup1.width() < 0 )
            {
                double fLeft = s_rctGroup1.left();
                double fRight = s_rctGroup1.right();
                s_rctGroup1.setLeft(fRight);
                s_rctGroup1.setRight(fLeft);
            }
            if( s_rctGroup1.height() < 0 )
            {
                double fTop = s_rctGroup1.top();
                double fBottom = s_rctGroup1.bottom();
                s_rctGroup1.setTop(fBottom);
                s_rctGroup1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtTopLeft_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtTopRight_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGroup1ObjName;

            s_rctGroup1OnMousePressEvent = s_rctGroup1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopRightSelect", pTSGrpGroupsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtTopRightStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointTopRight) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtTopRight_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtTopRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtTopRight_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopRightStep" + QString::number(m_iTestStepLev4), pTSGrpGroupsResize );

            m_fxOffsHor = s_arsizeGroupsResizeGroup1MovePtTopRightTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeGroupsResizeGroup1MovePtTopRightTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctGroup1.setRight( s_rctGroup1.right() + m_fxOffsHor );
            s_rctGroup1.setTop( s_rctGroup1.top() + m_fyOffsHor );

            QRectF rct = s_rctGroup1;

            if( fabs(s_rctGroup1.width()) > 0.0 || fabs(s_rctGroup1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtTopRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtTopRight_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtTopRight_Release )
        {
            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopRightRelease", pTSGrpGroupsResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rctGroup1.width() < 0 )
            {
                double fLeft = s_rctGroup1.left();
                double fRight = s_rctGroup1.right();
                s_rctGroup1.setLeft(fRight);
                s_rctGroup1.setRight(fLeft);
            }
            if( s_rctGroup1.height() < 0 )
            {
                double fTop = s_rctGroup1.top();
                double fBottom = s_rctGroup1.bottom();
                s_rctGroup1.setTop(fBottom);
                s_rctGroup1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtTopRight_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtBottomLeft_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGroup1ObjName;

            s_rctGroup1OnMousePressEvent = s_rctGroup1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomLeftSelect", pTSGrpGroupsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtBottomLeftStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointBottomLeft) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtBottomLeft_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtBottomLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtBottomLeft_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomLeftStep" + QString::number(m_iTestStepLev4), pTSGrpGroupsResize );

            m_fxOffsHor = s_arsizeGroupsResizeGroup1MovePtBottomLeftTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeGroupsResizeGroup1MovePtBottomLeftTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctGroup1.setLeft( s_rctGroup1.left() + m_fxOffsHor );
            s_rctGroup1.setBottom( s_rctGroup1.bottom() + m_fyOffsHor );

            QRectF rct = s_rctGroup1;

            if( fabs(s_rctGroup1.width()) > 0.0 || fabs(s_rctGroup1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtBottomLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtBottomLeft_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtBottomLeft_Release )
        {
            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomLeftRelease", pTSGrpGroupsResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rctGroup1.width() < 0 )
            {
                double fLeft = s_rctGroup1.left();
                double fRight = s_rctGroup1.right();
                s_rctGroup1.setLeft(fRight);
                s_rctGroup1.setRight(fLeft);
            }
            if( s_rctGroup1.height() < 0 )
            {
                double fTop = s_rctGroup1.top();
                double fBottom = s_rctGroup1.bottom();
                s_rctGroup1.setTop(fBottom);
                s_rctGroup1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtBottomLeft_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtBottomCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGroup1ObjName;

            s_rctGroup1OnMousePressEvent = s_rctGroup1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomCenterSelect", pTSGrpGroupsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtBottomCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointBottomCenter) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtBottomCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtBottomCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtBottomCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomCenterStep" + QString::number(m_iTestStepLev4), pTSGrpGroupsResize );

            m_fxOffsHor = s_arsizeGroupsResizeGroup1MovePtBottomCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeGroupsResizeGroup1MovePtBottomCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctGroup1.setBottom( s_rctGroup1.bottom() + m_fyOffsHor );

            QRectF rct = s_rctGroup1;

            if( fabs(s_rctGroup1.width()) > 0.0 || fabs(s_rctGroup1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtBottomCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtBottomCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtBottomCenter_Release )
        {
            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomCenterRelease", pTSGrpGroupsResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rctGroup1.width() < 0 )
            {
                double fLeft = s_rctGroup1.left();
                double fRight = s_rctGroup1.right();
                s_rctGroup1.setLeft(fRight);
                s_rctGroup1.setRight(fLeft);
            }

            if( s_rctGroup1.height() < 0 )
            {
                double fTop = s_rctGroup1.top();
                double fBottom = s_rctGroup1.bottom();
                s_rctGroup1.setTop(fBottom);
                s_rctGroup1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtBottomCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtTopCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGroup1ObjName;

            s_rctGroup1OnMousePressEvent = s_rctGroup1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopCenterSelect", pTSGrpGroupsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtTopCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointTopCenter) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtTopCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtTopCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtTopCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopCenterStep" + QString::number(m_iTestStepLev4), pTSGrpGroupsResize );

            m_fxOffsHor = s_arsizeGroupsResizeGroup1MovePtTopCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeGroupsResizeGroup1MovePtTopCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctGroup1.setTop( s_rctGroup1.top() + m_fyOffsHor );

            QRectF rct = s_rctGroup1;

            if( fabs(s_rctGroup1.width()) > 0.0 || fabs(s_rctGroup1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtTopCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtTopCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtTopCenter_Release )
        {
            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopCenterRelease", pTSGrpGroupsResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rctGroup1.width() < 0 )
            {
                double fLeft = s_rctGroup1.left();
                double fRight = s_rctGroup1.right();
                s_rctGroup1.setLeft(fRight);
                s_rctGroup1.setRight(fLeft);
            }

            if( s_rctGroup1.height() < 0 )
            {
                double fTop = s_rctGroup1.top();
                double fBottom = s_rctGroup1.bottom();
                s_rctGroup1.setTop(fBottom);
                s_rctGroup1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtTopCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtRightCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGroup1ObjName;

            s_rctGroup1OnMousePressEvent = s_rctGroup1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtRightCenterSelect", pTSGrpGroupsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtRightCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointRightCenter) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtRightCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtRightCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtRightCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtRightCenterStep" + QString::number(m_iTestStepLev4), pTSGrpGroupsResize );

            m_fxOffsHor = s_arsizeGroupsResizeGroup1MovePtRightCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeGroupsResizeGroup1MovePtRightCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctGroup1.setRight( s_rctGroup1.right() + m_fxOffsHor );

            QRectF rct = s_rctGroup1;

            if( fabs(s_rctGroup1.width()) > 0.0 || fabs(s_rctGroup1.height()) > 0.0 )
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

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtRightCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtRightCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtRightCenter_Release )
        {
            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtRightCenterRelease", pTSGrpGroupsResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rctGroup1.width() < 0 )
            {
                double fLeft = s_rctGroup1.left();
                double fRight = s_rctGroup1.right();
                s_rctGroup1.setLeft(fRight);
                s_rctGroup1.setRight(fLeft);
            }
            if( s_rctGroup1.height() < 0 )
            {
                double fTop = s_rctGroup1.top();
                double fBottom = s_rctGroup1.bottom();
                s_rctGroup1.setTop(fBottom);
                s_rctGroup1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtRightCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtLeftCenter_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGroup1ObjName;

            s_rctGroup1OnMousePressEvent = s_rctGroup1;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtLeftCenterSelect", pTSGrpGroupsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeRect1PtLeftCenterStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointLeftCenter) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtLeftCenter_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtLeftCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtLeftCenter_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtLeftCenterStep" + QString::number(m_iTestStepLev4), pTSGrpGroupsResize );

            m_fxOffsHor = s_arsizeGroupsResizeGroup1MovePtLeftCenterTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeGroupsResizeGroup1MovePtLeftCenterTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctGroup1.setLeft( s_rctGroup1.left() + m_fxOffsHor );

            QRectF rct = s_rctGroup1;

            if( fabs(s_rctGroup1.width()) > 0.0 || fabs(s_rctGroup1.height()) > 0.0 )
            {
                if( rct.width() < 0 )
                {
                    double fLeft = rct.left();
                    double fRight = rct.right();
                    rct.setLeft(fRight);
                    rct.setRight(fLeft);
                }
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Resize_Group1_MovePtLeftCenter_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Resize_Group1_MovePtLeftCenter_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtLeftCenter_Release )
        {
            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtLeftCenterRelease", pTSGrpGroupsResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rctGroup1.width() < 0 )
            {
                double fLeft = s_rctGroup1.left();
                double fRight = s_rctGroup1.right();
                s_rctGroup1.setLeft(fRight);
                s_rctGroup1.setRight(fLeft);
            }
            if( s_rctGroup1.height() < 0 )
            {
                double fTop = s_rctGroup1.top();
                double fBottom = s_rctGroup1.bottom();
                s_rctGroup1.setTop(fBottom);
                s_rctGroup1.setBottom(fTop);
            }

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_MovePtLeftCenter_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_Finished )
        {
            pTSGrpGroupsResize->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Resize_Group1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Rotate_Group1_ActivateEditModeSelect )
        {
            if( !pTSGrpGroupsRotate->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Groups_Rotate_Group1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpGroupsRotate->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpGroupsRotate );

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

            } // if( pTSGrpGroupsRotate->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Rotate_Group1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Rotate_Group1_Select )
        {
            m_iTestStepLev4 = -1;
            m_fAngle_degree = 0.0;

            strCurrentGraphObj = s_strGroup1ObjName;

            s_rctGroup1OnMousePressEvent = s_rctGroup1;

            pTestStep = getTestStep( strCurrentGraphObj, pTSGrpGroupsRotate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosRotateGroup1Select;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Rotate_Group1_Select )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Rotate_Group1_MoveSelPtRotateTop_Select )
        {
            m_iTestStepLev4 = -1;
            m_fAngle_degree = 0.0;

            strCurrentGraphObj = s_strGroup1ObjName;

            s_rctGroup1OnMousePressEvent = s_rctGroup1;

            pTestStep = getTestStep( strCurrentGraphObj + "SelPtRotateTopSelect", pTSGrpGroupsRotate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosRotateGroup1MoveSelPtRotateTopStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointRotateTop) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Rotate_Group1_MoveSelPtRotateTop_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Rotate_Group1_MoveSelPtRotateTop_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Rotate_Group1_MoveSelPtRotateTop_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "RotateStep" + QString::number(m_iTestStepLev4), pTSGrpGroupsRotate );

            m_fAngle_degree = s_arfRotateGroup1MoveSelPtRotateTopTestStepsAngles_degree[m_iTestStepLev4];

            QPointF ptCenter   = s_rctGroup1.center();
            QPointF ptSelPtRot = s_ptDrawingSceneMousePosRotateGroup1MoveSelPtRotateTopStart;
            double  fAngle_rad = Math::deg2Rad(m_fAngle_degree);

            ptSelPtRot = rotatePoint( ptCenter, ptSelPtRot, fAngle_rad );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptSelPtRot);
            ptSelPtRot = m_pDrawingView->mapToScene(m_ptDrawingViewMousePos);
            fAngle_rad = getAngleRad( ptCenter, ptSelPtRot );
            m_fAngle_degree = Math::rad2Deg(fAngle_rad);
            m_fAngle_degree -= 90.0; // Top center rotation point was selected
            m_fAngle_degree = Math::round2Res( m_fAngle_degree, m_pDrawingScene->getRotationAngleResolutionInDegree() );
            fAngle_rad = Math::deg2Rad(m_fAngle_degree);

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(ptSelPtRot);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_plgGroup1 = rotateRect( s_rctGroup1.center(), s_rctGroup1, fAngle_rad );
            s_plgGroup1.append(s_plgGroup1[0]);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1OnMousePressEvent.topLeft()) ); // as long as the mouse is not release the item's position is not changed
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgGroup1) );

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
                //bIgnoreTestResult = true;
            }
            pTestStep->testEnded(bIgnoreTestResult);

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Rotate_Group1_MoveSelPtRotateTop_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Rotate_Group1_MoveSelPtRotateTop_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Rotate_Group1_Release )
        {
            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "Release", pTSGrpGroupsRotate );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_rctGroup1.topLeft()) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctGroup1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Rotate_Group1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Rotate_Group1_Finished )
        {
            pTSGrpGroupsMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Rotate_Group1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_RotateMoveResize_Group1_ActivateEditModeSelect )
        {
            if( !pTSGrpGroupsRotateMoveResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Groups_RotateMoveResize_Group1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpGroupsRotateMoveResize->isEnabled() )
            {
                m_iTestStepLev4 = -1;
                m_fAngle_degree = 0.0;
                s_plgGroup1 = s_rctGroup1;

                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpGroupsRotateMoveResize );

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

            } // if( pTSGrpGroupsRotateResizeMove->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_RotateResizeMove_Group1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_RotateMoveResize_Group1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_RotateMoveResize_Group1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + "RotateMoveResizeStep" + QString::number(m_iTestStepLev4), pTSGrpGroupsRotateMoveResize );

            SRotateMoveResizeOperation op = s_arRotateMoveResizeGroup1TestSteps[m_iTestStepLev4];

            if( op.m_ptDrawingSceneMousePos.x() >= 0 && op.m_ptDrawingSceneMousePos.y() >= 0 )
            {
                s_plgGroup1.clear();
                s_plgGroup1.append(op.m_pt0PolygonGroupResult);
                s_plgGroup1.append(op.m_pt1PolygonGroupResult);
                s_plgGroup1.append(op.m_pt2PolygonGroupResult);
                s_plgGroup1.append(op.m_pt3PolygonGroupResult);
                s_plgGroup1.append(op.m_pt0PolygonGroupResult);

                m_ptDrawingSceneMousePos = op.m_ptDrawingSceneMousePos;
            }
            else if( op.m_operation == EEditModeRotate )
            {
                m_fAngle_degree = op.m_fRotAngle_degree;

                double fAngle_rad = Math::deg2Rad(m_fAngle_degree);

                s_rctGroup1.setX( op.m_pt0PolygonGroupResult.x() );
                s_rctGroup1.setY( op.m_pt0PolygonGroupResult.y() );
                s_rctGroup1.setWidth( op.m_pt1PolygonGroupResult.x() );
                s_rctGroup1.setHeight( op.m_pt1PolygonGroupResult.y() );

                s_plgGroup1 = rotateRect( s_rctGroup1.center(), s_rctGroup1, fAngle_rad );
                s_plgGroup1.append(s_plgGroup1[0]);

                QPointF ptSelPtRot = getSelectionPoint( s_rctGroup1, ESelectionPointRotateTop );

                m_ptDrawingSceneMousePos = rotatePoint( s_rctGroup1.center(), ptSelPtRot, fAngle_rad ).toPoint();
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
                    pTestStep->setDescription( "scene." + strCurrentGraphObj + ".rotateTo( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + ", " + QString::number(m_fAngle_degree) + "° )" );
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
                strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(op.m_ptPos) );
                strlstDesiredValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgGroup1) );
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_RotateMoveResize_Group1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_RotateMoveResize_Group1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_RotateMoveResize_Group1_Finished )
        {
            pTSGrpGroupsRotateMoveResize->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_RotateMoveResize_Group1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Simulation_ActivateSimulationMode )
        {
            if( !pTSGrpGroupsSimulation->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Groups_Simulation_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpGroupsSimulation->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateSimulationMode", pTSGrpGroupsSimulation );

                strlstDesiredValues.append( "Mode: " + CEnumMode::toString(EMode::Simulation) );
                strlstDesiredValues.append( "DrawingTool: " );

                pTestStep->setOperation( "drawing.setMode( " + CEnumMode::toString(EMode::Simulation) + " )" );
                pTestStep->setDesiredValues(strlstDesiredValues);
                pTestStep->testStarted();

                if( m_pDrawingScene->getMode() != EMode::Simulation )
                {
                    m_pMainWindow->setCheckedActionModeSimulation(true);
                }

                if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
                {
                    strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
                }
                else
                {
                    strCurrentDrawingTool = "";
                }

                strlstActualValues.append( "Mode: " + CEnumMode::toString(m_pDrawingScene->getMode()) );
                strlstActualValues.append( "DrawingTool: " + strCurrentDrawingTool );

                pTestStep->setActualValues(strlstActualValues);
                pTestStep->testEnded();

                m_iTestStepLev3++;  // Next sub test step
                triggerDoTestStep();

            } // if( pTSGrpGroupsSimulation->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Simulation_ActivateSimulationMode )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Simulation_Group1_AddMouseEventFunction )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + ".AddMouseEventFunction", pTSGrpGroupsSimulation );

            pTestStep->setOperation( strCurrentGraphObj + ".addMouseEventFunction()" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            pGraphObj = m_pDrawingScene->findGraphObj(strCurrentGraphObj);

            pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphObj);

            if( pGraphObjGroup != nullptr )
            {
                pGraphObjGroup->addMouseReleaseEventFunction( fctGroup1MouseReleaseEvent, this, nullptr );
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Simulation_Group1_AddMouseEventFunction )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Simulation_Group1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Simulation_Group1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = s_strGroup1ObjName;

            pTestStep = getTestStep( strCurrentGraphObj + ".Step" + QString::number(m_iTestStepLev4), pTSGrpGroupsSimulation );

            m_ptDrawingSceneMousePos = s_arptDrawingSceneMousePosSimulationGroup1TestSteps[m_iTestStepLev4];

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( s_strGroup1CnctPt1ObjName );

            if( m_iTestStepLev4 % 3 == 0 )
            {
                strlstDesiredValues.append( s_strGroup1CnctPt3ObjName ); // Out2
            }
            else if( m_iTestStepLev4 % 3 == 1 )
            {
                strlstDesiredValues.append( s_strGroup1CnctPt4ObjName ); // Out3
            }
            else if( m_iTestStepLev4 % 3 == 2 )
            {
                strlstDesiredValues.append( s_strGroup1CnctPt2ObjName ); // Out1
            }

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos: " + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".mouseClick( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

            // Please note that connection lines are never children of other objects and are therefore also
            // never childrens of groups as they may link two connection points in different groups.
            pGraphObj = m_pDrawingScene->findGraphObj(s_strGroup1CnctLineObjId);

            pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj);

            if( pGraphObjCnctLine != nullptr )
            {
                pGraphObjCnctPt = pGraphObjCnctLine->getConnectionPoint(ELinePointStart);
                if( pGraphObjCnctPt != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctPt->getObjName() );
                }
                pGraphObjCnctPt = pGraphObjCnctLine->getConnectionPoint(ELinePointEnd);
                if( pGraphObjCnctPt != nullptr )
                {
                    strlstActualValues.append( pGraphObjCnctPt->getObjName() );
                }
            } // if( pGraphObjCnctLine != nullptr )

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Simulation_Group1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Groups_Simulation_Group1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Simulation_Group1_RemoveMouseEventFunction )
        {
            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Simulation_Group1_RemoveMouseEventFunction )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Simulation_DeactivateSimulationMode )
        {
            pTestStep = getTestStep( "DeactivateSimulationMode", pTSGrpGroupsSimulation );

            strlstDesiredValues.append( "Mode: " + CEnumMode::toString(EMode::Edit) );
            strlstDesiredValues.append( "DrawingTool: " );

            pTestStep->setOperation( "drawing.setMode( " + CEnumMode::toString(EMode::Edit) + " )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            if( m_pDrawingScene->getMode() != EMode::Edit )
            {
                m_pMainWindow->setCheckedActionModeEdit(true);
            }

            if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
            {
                strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
            }
            else
            {
                strCurrentDrawingTool = "";
            }

            strlstActualValues.append( "Mode: " + CEnumMode::toString(m_pDrawingScene->getMode()) );
            strlstActualValues.append( "DrawingTool: " + strCurrentDrawingTool );

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Simulation_DeactivateSimulationMode )

        else if( m_iTestStepLev3 == ETestStepLev3_Groups_Simulation_Finished )
        {
            pTSGrpGroupsSimulation->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Groups_Simulation_Finished )

        else if( m_iTestStepLev3 >= ETestStepLev3_Groups_Finished )
        {
            // deselect object and start next test step
            m_ptDrawingSceneMousePos = QPoint(275,275);

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

            pTSGrpGroups->testEnded();

            m_iTestStepLev2++; // Next test step
            m_iTestStepLev3 = 0;
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Groups_Finished )

    } // if( pTSGrpGroups->isEnabled() )

    delete pMouseEv;
    pMouseEv = nullptr;

} // doTestStepStandardShapesGroups

//------------------------------------------------------------------------------
void fctGroup1MouseReleaseEvent( void* i_pvThis, void* /*i_pvData*/, CGraphObj* i_pGraphObj, QGraphicsSceneMouseEvent* /*i_pEv*/ )
//------------------------------------------------------------------------------
{
    CTest*          pThis = reinterpret_cast<CTest*>(i_pvThis);
    CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(i_pGraphObj);
    CDrawingScene*  pDrawingScene = pGraphObjGroup->getDrawingScene();

    if( pThis != nullptr && pGraphObjGroup != nullptr && pDrawingScene != nullptr )
    {
        //CGraphObjConnectionPoint* pGraphObjCnctPtIn   = dynamic_cast<CGraphObjConnectionPoint*>(pGraphObjGroup->findGraphObj(s_strGroup1CnctPt1ObjName));
        CGraphObjConnectionPoint* pGraphObjCnctPtOut1 = dynamic_cast<CGraphObjConnectionPoint*>(pGraphObjGroup->findGraphObj(s_strGroup1CnctPt2ObjName));
        CGraphObjConnectionPoint* pGraphObjCnctPtOut2 = dynamic_cast<CGraphObjConnectionPoint*>(pGraphObjGroup->findGraphObj(s_strGroup1CnctPt3ObjName));
        CGraphObjConnectionPoint* pGraphObjCnctPtOut3 = dynamic_cast<CGraphObjConnectionPoint*>(pGraphObjGroup->findGraphObj(s_strGroup1CnctPt4ObjName));

        // Please note that connection lines are never children of other objects and are therefore also
        // never childrens of groups as they may link two connection points in different groups.
        CGraphObj* pGraphObj = pDrawingScene->findGraphObj(s_strGroup1CnctLineObjId);
        CGraphObjConnectionLine* pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj);

        if( pGraphObjCnctLine != nullptr )
        {
            CGraphObjConnectionPoint* pGraphObjCnctPt = pGraphObjCnctLine->getConnectionPoint(ELinePointEnd);

            if( pGraphObjCnctPt != nullptr )
            {
                if( pGraphObjCnctPt == pGraphObjCnctPtOut1 )
                {
                    pGraphObjCnctLine->setConnectionPoint( ELinePointEnd, pGraphObjCnctPtOut2 );
                }
                else if( pGraphObjCnctPt == pGraphObjCnctPtOut2 )
                {
                    pGraphObjCnctLine->setConnectionPoint( ELinePointEnd, pGraphObjCnctPtOut3 );
                }
                else if( pGraphObjCnctPt == pGraphObjCnctPtOut3 )
                {
                    pGraphObjCnctLine->setConnectionPoint( ELinePointEnd, pGraphObjCnctPtOut1 );
                }
                pGraphObjCnctLine->update();

            } // if( pGraphObjCnctPt != nullptr )
        } // if( pGraphObjCnctLine != nullptr )
    } // if( pThis != nullptr )

} // fctGroup1MouseReleaseEvent

#endif // #if 0
