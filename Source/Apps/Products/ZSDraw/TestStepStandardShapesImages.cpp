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
#include "ZSDraw/ZSDrawGraphObjImage.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSPhysVal/ZSPhysVal.h"
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

static QRectF    s_rctImage1;
static QPolygonF s_plgImage1;

// Create_Image1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosCreateImage1Start(200,450);

// Move_Image1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosMoveImage1Start(216,466);

static const QSize s_arsizeMoveImage1TestSteps[] = {
    /*  0 */ QSize( -5,  0 ),
    /*  1 */ QSize( -5,  0 ),
    /*  2 */ QSize( -5,  0 ),
    /*  3 */ QSize(  0, -5 ),
    /*  4 */ QSize(  0, -5 ),
    /*  5 */ QSize(  0, -5 ),
    /*  6 */ QSize(  5,  0 ),
    /*  7 */ QSize(  5,  0 ),
    /*  8 */ QSize(  5,  0 ),
    /*  9 */ QSize(  5,  0 ),
    /* 10 */ QSize(  5,  0 ),
    /* 11 */ QSize(  5,  0 ),
    /* 12 */ QSize(  0,  5 ),
    /* 13 */ QSize(  0,  5 ),
    /* 14 */ QSize(  0,  5 ),
    /* 15 */ QSize(  0,  5 ),
    /* 16 */ QSize(  0,  5 ),
    /* 17 */ QSize(  0,  5 ),
    /* 18 */ QSize( -5, -5 ),
    /* 19 */ QSize( -5, -5 ),
    /* 20 */ QSize( -5, -5 )
};

static const int ETestStepLev3_Images_Move_Image1_TestStepCount = sizeof(s_arsizeMoveImage1TestSteps)/sizeof(s_arsizeMoveImage1TestSteps[0]);

// Resize_Image1_MovePtBottomRight
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeImage1Select(216,466);

static const QPoint s_ptDrawingSceneMousePosResizeImage1PtBottomRightStart(232,482); // The bottom right corner should not been hit exactly.

static const QSize s_arsizeResizeImage1MovePtBottomRightTestSteps[] = {
    /*  0 */ QSize(  16,  16 ),
    /*  1 */ QSize(  16,  16 ),
    /*  2 */ QSize( -16, -16 ),
    /*  3 */ QSize( -16, -16 ),
    /*  4 */ QSize( -16, -16 ),
    /*  5 */ QSize( -16, -16 ),
    /*  6 */ QSize( -16, -16 ),
    /*  7 */ QSize( -16, -16 ),
    /*  8 */ QSize( -16, -16 ),
    /*  9 */ QSize( -16, -16 ),
    /* 10 */ QSize(  16,  16 ),
    /* 11 */ QSize(  16,  16 ),
    /* 12 */ QSize(  16,  16 ),
    /* 13 */ QSize(  16,  16 ),
    /* 14 */ QSize(  16,  16 ),
    /* 15 */ QSize(  16,  16 )
};

static const int ETestStepLev3_Images_Resize_Image1_MovePtBottomRight_TestStepCount = sizeof(s_arsizeResizeImage1MovePtBottomRightTestSteps)/sizeof(s_arsizeResizeImage1MovePtBottomRightTestSteps[0]);

// Resize_Image1_MovePtTopLeft
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeImage1PtTopLeftStart(200,450);

static const QSize s_arsizeResizeImage1MovePtTopLeftTestSteps[] = {
    /*  0 */ QSize( -16, -16 ),
    /*  1 */ QSize( -16, -16 ),
    /*  2 */ QSize(  16,  16 ),
    /*  3 */ QSize(  16,  16 ),
    /*  4 */ QSize(  16,  16 ),
    /*  5 */ QSize(  16,  16 ),
    /*  6 */ QSize(  16,  16 ),
    /*  7 */ QSize(  16,  16 ),
    /*  8 */ QSize(  16,  16 ),
    /*  9 */ QSize(  16,  16 ),
    /* 10 */ QSize( -16, -16 ),
    /* 11 */ QSize( -16, -16 ),
    /* 12 */ QSize( -16, -16 ),
    /* 13 */ QSize( -16, -16 ),
    /* 14 */ QSize( -16, -16 ),
    /* 15 */ QSize( -16, -16 )
};

static const int ETestStepLev3_Images_Resize_Image1_MovePtTopLeft_TestStepCount = sizeof(s_arsizeResizeImage1MovePtTopLeftTestSteps)/sizeof(s_arsizeResizeImage1MovePtTopLeftTestSteps[0]);

// RotateTo_Image1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosRotateToImage1Select(216,466);

static const QPoint s_ptDrawingSceneMousePosRotateToImage1MoveSelPtRotateTopStart(216,430);

static const double s_arfRotateImage1MoveSelPtRotateTopTestStepsAngles_degree[] = {
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

static const int ETestStepLev3_Images_RotateTo_Image1_MoveSelPtRotateTop_TestStepCount = sizeof(s_arfRotateImage1MoveSelPtRotateTopTestStepsAngles_degree)/sizeof(s_arfRotateImage1MoveSelPtRotateTopTestStepsAngles_degree[0]);

// RotateResize_Image1
//------------------------------------------------------------------------------

struct SRotateResizeOperation
{
    QPoint    m_ptDrawingSceneMousePos;
    QPoint    m_ptDrawingSceneRectCenterPos;
    int       m_iMouseEvent;
    EEditMode m_operation;
    QPointF   m_pt0PolygonRectResult;
    QPointF   m_pt1PolygonRectResult;
    QPointF   m_pt2PolygonRectResult;
    QPointF   m_pt3PolygonRectResult;
    bool      m_bIgnoreTestResult;
};

static const SRotateResizeOperation s_arRotateResizeImage1TestSteps[] = {
          // {        MousePos,       RectCenterPos,               MouseEvent,         Operation,       PolygonRect[0],       PolygonRect[1],       PolygonRect[2],       PolygonRect[3], Ignore }
    /*  0 */ { QPoint(216,466), QPoint(216.0,466.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(200.0,450.0), QPointF(232.0,450.0), QPointF(232.0,482.0), QPointF(200.0,482.0), false  }, // Select
    /*  1 */ { QPoint( -1, -1), QPoint(216.0,466.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(200.0,450.0), QPointF(232.0,450.0), QPointF(232.0,482.0), QPointF(200.0,482.0), false  },
    /*  2 */ { QPoint(232,482), QPoint(216.0,466.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(200.0,450.0), QPointF(232.0,450.0), QPointF(232.0,482.0), QPointF(200.0,482.0), false  }, // Select SelPt BottomRight
    /*  3 */ { QPoint(248,498), QPoint(224.0,474.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,450.0), QPointF(248.0,450.0), QPointF(248.0,498.0), QPointF(200.0,498.0), false  }, // ResizeBy by 16/16 (increase width and height by 16)
    /*  4 */ { QPoint(264,514), QPoint(232.0,482.0), QEvent::MouseMove,          EEditModeResize, QPointF(200.0,450.0), QPointF(264.0,450.0), QPointF(264.0,514.0), QPointF(200.0,514.0), false  }, // ResizeBy by 16/16 (increase width and height by 16)
    /*  5 */ { QPoint( -1, -1), QPoint(232.0,482.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(200.0,450.0), QPointF(264.0,450.0), QPointF(264.0,514.0), QPointF(200.0,514.0), false  },
    /*  6 */ { QPoint(232,430), QPoint(232.0,482.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(200.0,450.0), QPointF(264.0,450.0), QPointF(264.0,514.0), QPointF(200.0,514.0), false  }, // Select SelPt RotateTop
    /*  7 */ { QPoint(195,445), QPoint(232.0,482.0), QEvent::MouseMove,          EEditModeRotate, QPointF(186.7,482.0), QPointF(232.0,436.7), QPointF(277.3,482.0), QPointF(232.0,527.3), false  }, // Rotate by 45° to 45°
    /*  8 */ { QPoint( -1, -1), QPoint(232.0,482.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(186.7,482.0), QPointF(232.0,436.7), QPointF(277.3,482.0), QPointF(232.0,527.3), false  },
    /*  9 */ { QPoint(187,482), QPoint(232.0,482.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(186.7,482.0), QPointF(232.0,436.7), QPointF(277.3,482.0), QPointF(232.0,527.3), false  }, // Select SelPt TopLeft
    /* 10 */ { QPoint(209,482), QPoint(243.0,482.0), QEvent::MouseMove,          EEditModeResize, QPointF(209.0,482.0), QPointF(243.1,447.9), QPointF(277.3,482.0), QPointF(243.1,516.1), false  }, // ResizeBy by -16/-16 (decrease width and height by 16)
    /* 11 */ { QPoint(232,482), QPoint(254.0,482.0), QEvent::MouseMove,          EEditModeResize, QPointF(232.0,482.0), QPointF(254.6,459.4), QPointF(277.3,482.0), QPointF(254.6,504.6), false  }, // ResizeBy by -16/-16 (decrease width and height by 16)
    /* 12 */ { QPoint( -1, -1), QPoint(254.0,482.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(232.0,482.0), QPointF(254.6,459.4), QPointF(277.3,482.0), QPointF(254.6,504.6), false  },
    /* 13 */ { QPoint(277,482), QPoint(254.0,482.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(232.0,482.0), QPointF(254.6,459.4), QPointF(277.3,482.0), QPointF(254.6,504.6), false  }, // Select SelPt BottomRight
    /* 14 */ { QPoint(255,482), QPoint(243.0,482.0), QEvent::MouseMove,          EEditModeResize, QPointF(232.0,482.0), QPointF(243.5,470.5), QPointF(255.0,482.0), QPointF(243.5,493.5), false  }, // ResizeBy by -16/-16 (decrease width and height by 16)
    /* 15 */ { QPoint(232,482), QPoint(232.0,482.0), QEvent::MouseMove,          EEditModeResize, QPointF(232.0,482.0), QPointF(232.0,482.0), QPointF(232.0,482.0), QPointF(232.0,482.0), false  }, // ResizeBy by -16/-16 (decrease width and height by 16)
    /* 16 */ { QPoint(209,482), QPoint(221.0,482.0), QEvent::MouseMove,          EEditModeResize, QPointF(209.0,482.0), QPointF(220.5,470.5), QPointF(232.0,482.0), QPointF(220.5,493.5), false  }, // ResizeBy by -16/-16 (decrease width and height by 16)
    /* 17 */ { QPoint(187,482), QPoint(210.0,482.0), QEvent::MouseMove,          EEditModeResize, QPointF(187.0,482.0), QPointF(209.5,459.5), QPointF(232.0,482.0), QPointF(209.5,504.5), false  }, // ResizeBy by -16/-16 (decrease width and height by 16)
    /* 18 */ { QPoint( -1, -1), QPoint(210.0,482.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(187.0,482.0), QPointF(209.5,459.5), QPointF(232.0,482.0), QPointF(209.5,504.5), false  },
    /* 19 */ { QPoint(184,457), QPoint(210.0,482.0), QEvent::MouseButtonPress,   EEditModeSelect, QPointF(187.0,482.0), QPointF(209.5,459.5), QPointF(232.0,482.0), QPointF(209.5,504.5), false  },
    /* 20 */ { QPoint(210,446), QPoint(210.0,482.0), QEvent::MouseMove,          EEditModeRotate, QPointF(193.9,465.8), QPointF(225.7,466.4), QPointF(225.1,498.2), QPointF(193.3,497.6), false  }, // Rotate by -45° to -45°
    /* 21 */ { QPoint( -1, -1), QPoint(210.0,482.0), QEvent::MouseButtonRelease, EEditModeSelect, QPointF(193.9,465.8), QPointF(225.7,466.4), QPointF(225.1,498.2), QPointF(193.3,497.6), false  }
};

static const int ETestStepLev3_Images_RotateResize_Image1_TestStepCount = sizeof(s_arRotateResizeImage1TestSteps)/sizeof(s_arRotateResizeImage1TestSteps[0]);

// Test steps:
//------------------------------------------------------------------------------

enum
{
    ETestStepLev3_Images_Create_Image1_ActivateDrawingTool,
    ETestStepLev3_Images_Create_Image1_Start,
    ETestStepLev3_Images_Create_Image1_Finished,
    ETestStepLev3_Images_Move_Image1_ActivateEditModeSelect,
    ETestStepLev3_Images_Move_Image1_Select,
    ETestStepLev3_Images_Move_Image1_TestStepMin,
    ETestStepLev3_Images_Move_Image1_TestStepMax = ETestStepLev3_Images_Move_Image1_TestStepMin + ETestStepLev3_Images_Move_Image1_TestStepCount - 1,
    ETestStepLev3_Images_Move_Image1_Release,
    ETestStepLev3_Images_Move_Image1_Finished,
    ETestStepLev3_Images_Resize_Image1_ActivateEditModeSelect,
    ETestStepLev3_Images_Resize_Image1_Select,
    ETestStepLev3_Images_Resize_Image1_MovePtBottomRight_Select,
    ETestStepLev3_Images_Resize_Image1_MovePtBottomRight_TestStepMin,
    ETestStepLev3_Images_Resize_Image1_MovePtBottomRight_TestStepMax = ETestStepLev3_Images_Resize_Image1_MovePtBottomRight_TestStepMin + ETestStepLev3_Images_Resize_Image1_MovePtBottomRight_TestStepCount - 1,
    ETestStepLev3_Images_Resize_Image1_MovePtBottomRight_Release,
    ETestStepLev3_Images_Resize_Image1_MovePtTopLeft_Select,
    ETestStepLev3_Images_Resize_Image1_MovePtTopLeft_TestStepMin,
    ETestStepLev3_Images_Resize_Image1_MovePtTopLeft_TestStepMax = ETestStepLev3_Images_Resize_Image1_MovePtTopLeft_TestStepMin + ETestStepLev3_Images_Resize_Image1_MovePtTopLeft_TestStepCount - 1,
    ETestStepLev3_Images_Resize_Image1_MovePtTopLeft_Release,
    ETestStepLev3_Images_Resize_Image1_Finished,
    ETestStepLev3_Images_RotateTo_Image1_ActivateEditModeSelect,
    ETestStepLev3_Images_RotateTo_Image1_Select,
    ETestStepLev3_Images_RotateTo_Image1_MoveSelPtRotateTop_Select,
    ETestStepLev3_Images_RotateTo_Image1_MoveSelPtRotateTop_TestStepMin,
    ETestStepLev3_Images_RotateTo_Image1_MoveSelPtRotateTop_TestStepMax = ETestStepLev3_Images_RotateTo_Image1_MoveSelPtRotateTop_TestStepMin + ETestStepLev3_Images_RotateTo_Image1_MoveSelPtRotateTop_TestStepCount - 1,
    ETestStepLev3_Images_RotateTo_Image1_Release,
    ETestStepLev3_Images_RotateTo_Image1_Finished,
    ETestStepLev3_Images_RotateResize_Image1_ActivateEditModeSelect,
    ETestStepLev3_Images_RotateResize_Image1_Start,
    ETestStepLev3_Images_RotateResize_Image1_TestStepMin,
    ETestStepLev3_Images_RotateResize_Image1_TestStepMax = ETestStepLev3_Images_RotateResize_Image1_TestStepMin + ETestStepLev3_Images_RotateResize_Image1_TestStepCount - 1,
    ETestStepLev3_Images_RotateResize_Image1_Finished,
    ETestStepLev3_Images_Count
};

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepStandardShapesImages( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
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
    CObjFactory*         pObjFactory;
    CGraphObj*           pGraphObj;
    CGraphObjImage*      pGraphObjImage;

    ZS::Test::CTestStepGroup* pTSGrpImages             = getTestStepGroup( "Images", i_pTestStepGrpParent );
    ZS::Test::CTestStepGroup* pTSGrpImagesCreate       = getTestStepGroup( "Create", pTSGrpImages );
    ZS::Test::CTestStepGroup* pTSGrpImagesMove         = getTestStepGroup( "Move", pTSGrpImages );
    ZS::Test::CTestStepGroup* pTSGrpImagesResize       = getTestStepGroup( "Resize", pTSGrpImages );
    ZS::Test::CTestStepGroup* pTSGrpImagesRotateTo     = getTestStepGroup( "RotateTo", pTSGrpImages );
    ZS::Test::CTestStepGroup* pTSGrpImagesRotateResize = getTestStepGroup( "RotateResize", pTSGrpImages );

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }

    if( !pTSGrpImages->isEnabled() )
    {
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else if( !pTSGrpImagesCreate->isEnabled() )
    {
        // If the rectangle is not created none of the other sub tests may be performed.
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else // if( pTSGrpImages->isEnabled() )
    {
        if( m_iTestStepLev3 == ETestStepLev3_Images_Create_Image1_ActivateDrawingTool )
        {
            if( !pTSGrpImagesCreate->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Images_Create_Image1_Finished+1; // Last create test step
                triggerDoTestStep();
            }
            else // if( pTSGrpImagesCreate->isEnabled() )
            {
                strCurrentGraphObj = "Image1";

                pTestStep = getTestStep( "Create." + strCurrentGraphObj + ".ActivateDrawingTool", pTSGrpImagesCreate );

                strlstExpectedValues.append( "EditTool: CreateObjects" );
                strlstExpectedValues.append( "DrawingTool: Image" );

                pTestStep->setOperation( "drawing.setCurrentDrawingTool( Image )" );
                pTestStep->setExpectedValues(strlstExpectedValues);
                pTestStep->testStarted();

                pObjFactory = CObjFactory::FindObjFactory(
                    /* strGraphObjNameSpace */ "ZS::Draw",
                    /* strGraphObjClassName */ "CGraphObjImage",
                    /* iGraphObjType        */ EGraphObjTypeImage );

                if( pObjFactory != nullptr )
                {
                    pObjFactory->setFileName(":/ZS/App/Zeus32x32.bmp");
                }

                m_pDrawingScene->setCurrentDrawingTool(pObjFactory);

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

            } // if( pTSGrpImagesCreate->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_Create_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_Create_Image1_Start )
        {
            m_iImagesCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

            pTestStep = getTestStep( "Create." + strCurrentGraphObj + ".Start", pTSGrpImagesCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateImage1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctImage1 = QRectF( QPoint(0.0,0.0), QPixmap(":/ZS/App/Zeus32x32.bmp").size() );

            strlstExpectedValues.append( "ImageSize: " + size2Str(s_rctImage1.size()) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".create( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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
                /* button    */ Qt::LeftButton,
                /* buttons   */ Qt::NoButton,
                /* modifiers */ Qt::NoModifier );
            m_pDrawingView->mouseReleaseEvent(pMouseEv);
            delete pMouseEv;
            pMouseEv = nullptr;

            pGraphObj = m_pDrawingScene->findGraphObj(strCurrentGraphObj);

            pGraphObjImage = dynamic_cast<CGraphObjImage*>(pGraphObj);

            if( pGraphObjImage != nullptr )
            {
                strlstResultValues.append( "ImageSize: " + size2Str(pGraphObjImage->pixmap().size(),'f',1) );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_Create_Image1_Start )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_Create_Image1_Finished )
        {
            pTSGrpImagesCreate->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_Create_Image1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_Move_Image1_ActivateEditModeSelect )
        {
            if( !pTSGrpImagesMove->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Images_Move_Image1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpImagesMove->isEnabled() )
            {
                strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

                pTestStep = getTestStep( "Move." + strCurrentGraphObj + ".ActivateEditModeSelect", pTSGrpImagesMove );

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

            } // if( pTSGrpImagesMove->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_Move_Image1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_Move_Image1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

            pTestStep = getTestStep( "Move." + strCurrentGraphObj + ".Select", pTSGrpImagesMove );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosMoveImage1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointCenter) );

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
            s_rctImage1 = QRectF();

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strToolTip = m_pGraphObjSelected->getToolTip();

                pGraphObjImage = dynamic_cast<CGraphObjImage*>(m_pGraphObjSelected);
                if( pGraphObjImage != nullptr )
                {
                    s_rctImage1 = QRectF( QPointF(0.0,0.0), pGraphObjImage->getSize() );
                    s_rctImage1 = pGraphObjImage->mapToScene(s_rctImage1).boundingRect();
                }
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_Move_Image1_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Images_Move_Image1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Images_Move_Image1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

            pTestStep = getTestStep( "Move." + strCurrentGraphObj + ".Step" + QString::number(m_iTestStepLev4), pTSGrpImagesMove );

            m_fxOffsHor = s_arsizeMoveImage1TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeMoveImage1TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctImage1.moveLeft( s_rctImage1.left() + m_fxOffsHor );
            s_rctImage1.moveTop( s_rctImage1.top() + m_fyOffsHor );

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctImage1) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".move( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Images_Move_Image1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Images_Move_Image1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_Move_Image1_Release )
        {
            strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

            pTestStep = getTestStep( "Move." + strCurrentGraphObj + ".Release", pTSGrpImagesMove );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctImage1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_Move_Image1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_Move_Image1_Finished )
        {
            pTSGrpImagesMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_Move_Image1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_Resize_Image1_ActivateEditModeSelect )
        {
            if( !pTSGrpImagesResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Images_Resize_Image1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpImagesResize->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpImagesResize );

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

            } // if( pTSGrpImagesResize->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_Resize_Image1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_Resize_Image1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "Select", pTSGrpImagesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeImage1Select;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );

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
            s_rctImage1 = QRectF();

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strToolTip = m_pGraphObjSelected->getToolTip();

                pGraphObjImage = dynamic_cast<CGraphObjImage*>(m_pGraphObjSelected);
                if( pGraphObjImage != nullptr )
                {
                    s_rctImage1 = QRectF( QPointF(0.0,0.0), pGraphObjImage->getSize() );
                    s_rctImage1 = pGraphObjImage->mapToScene(s_rctImage1).boundingRect();
                }
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_Resize_Image1_Select )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_Resize_Image1_MovePtBottomRight_Select )
        {
            if( !pTSGrpImagesResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Images_Resize_Image1_Finished+1; // Last resize test step
                triggerDoTestStep();
            }
            else // if( pTSGrpImagesResize->isEnabled() )
            {
                m_iTestStepLev4 = -1;

                strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

                pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightSelect", pTSGrpImagesResize );

                m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeImage1PtBottomRightStart;

                m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

                ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
                ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
                ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

                strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointBottomRight) );
                strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctImage1) );

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

            } // if( pTSGrpImagesResize->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_Resize_Image1_MovePtBottomRight_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Images_Resize_Image1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Images_Resize_Image1_MovePtBottomRight_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightStep" + QString::number(m_iTestStepLev4), pTSGrpImagesResize );

            m_fxOffsHor = s_arsizeResizeImage1MovePtBottomRightTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeResizeImage1MovePtBottomRightTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctImage1.setRight(m_ptDrawingSceneMousePos.x());
            s_rctImage1.setBottom(m_ptDrawingSceneMousePos.y());

            // Keep old size before resizing starts as the object may get 0 pixel size.
            // The size will be adjusted on releasing the mouse.
            QRectF rct = s_rctImage1;

            if( fabs(s_rctImage1.width()) > 0.0 || fabs(s_rctImage1.height()) > 0.0 )
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

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(rct) );

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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Images_Resize_Image1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Images_Resize_Image1_MovePtBottomRight_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_Resize_Image1_MovePtBottomRight_Release )
        {
            strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtBottomRightRelease", pTSGrpImagesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            // Old size before resizing starts has been kept as the object may get 0 pixel size.
            // The size will be adjusted on releasing the mouse.
            if( s_rctImage1.width() < 0 )
            {
                double fLeft = s_rctImage1.left();
                double fRight = s_rctImage1.right();
                s_rctImage1.setLeft(fRight);
                s_rctImage1.setRight(fLeft);
            }
            if( s_rctImage1.height() < 0 )
            {
                double fTop = s_rctImage1.top();
                double fBottom = s_rctImage1.bottom();
                s_rctImage1.setTop(fBottom);
                s_rctImage1.setBottom(fTop);
            }

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctImage1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_Resize_Image1_MovePtBottomRight_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_Resize_Image1_MovePtTopLeft_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopLeftSelect", pTSGrpImagesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeImage1PtTopLeftStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointTopLeft) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctImage1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_Resize_Image1_MovePtTopLeft_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Images_Resize_Image1_MovePtTopLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Images_Resize_Image1_MovePtTopLeft_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopLeftStep" + QString::number(m_iTestStepLev4), pTSGrpImagesResize );

            m_fxOffsHor = s_arsizeResizeImage1MovePtTopLeftTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeResizeImage1MovePtTopLeftTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctImage1.setLeft( s_rctImage1.left() + m_fxOffsHor );
            s_rctImage1.setTop( s_rctImage1.top() + m_fyOffsHor );

            QRectF rct = s_rctImage1;

            if( fabs(s_rctImage1.width()) > 0.0 || fabs(s_rctImage1.height()) > 0.0 )
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

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(rct) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".resize( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Images_Resize_Image1_MovePtTopLeft_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Images_Resize_Image1_MovePtTopLeft_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_Resize_Image1_MovePtTopLeft_Release )
        {
            strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

            pTestStep = getTestStep( strCurrentGraphObj + "MovePtTopLeftRelease", pTSGrpImagesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            if( s_rctImage1.width() < 0 )
            {
                double fLeft = s_rctImage1.left();
                double fRight = s_rctImage1.right();
                s_rctImage1.setLeft(fRight);
                s_rctImage1.setRight(fLeft);
            }
            if( s_rctImage1.height() < 0 )
            {
                double fTop = s_rctImage1.top();
                double fBottom = s_rctImage1.bottom();
                s_rctImage1.setTop(fBottom);
                s_rctImage1.setBottom(fTop);
            }

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctImage1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_Resize_Image1_MovePtTopLeft_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_Resize_Image1_Finished )
        {
            pTSGrpImagesResize->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_Resize_Image1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_RotateTo_Image1_ActivateEditModeSelect )
        {
            if( !pTSGrpImagesRotateTo->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Images_RotateTo_Image1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpImagesRotateTo->isEnabled() )
            {
                strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

                pTestStep = getTestStep( "RotateTo." + strCurrentGraphObj + ".ActivateEditModeSelect", pTSGrpImagesRotateTo );

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

            } // if( pTSGrpImagesRotateTo->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_RotateTo_Image1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_RotateTo_Image1_Select )
        {
            m_iTestStepLev4 = -1;
            m_fAngle_degree = 0.0;

            strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

            pTestStep = getTestStep( "RotateTo." + strCurrentGraphObj + ".Select", pTSGrpImagesRotateTo );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosRotateToImage1Select;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );

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
            s_rctImage1 = QRectF();

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strToolTip = m_pGraphObjSelected->getToolTip();

                pGraphObjImage = dynamic_cast<CGraphObjImage*>(m_pGraphObjSelected);
                if( pGraphObjImage != nullptr )
                {
                    s_rctImage1 = QRectF( QPointF(0.0,0.0), pGraphObjImage->getSize() );
                    s_rctImage1 = pGraphObjImage->mapToScene(s_rctImage1).boundingRect();
                }
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_RotateTo_Image1_Select )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_RotateTo_Image1_MoveSelPtRotateTop_Select )
        {
            m_iTestStepLev4 = -1;
            m_fAngle_degree = 0.0;

            strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

            pTestStep = getTestStep( "RotateTo." + strCurrentGraphObj + ".SelectSelPtRotateTop", pTSGrpImagesRotateTo );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosRotateToImage1MoveSelPtRotateTopStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointRotateTop) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctImage1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_RotateTo_Image1_MoveSelPtRotateTop_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Images_RotateTo_Image1_MoveSelPtRotateTop_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Images_RotateTo_Image1_MoveSelPtRotateTop_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

            pTestStep = getTestStep( "RotateTo." + strCurrentGraphObj + ".Step" + QString::number(m_iTestStepLev4), pTSGrpImagesRotateTo );

            m_fAngle_degree = s_arfRotateImage1MoveSelPtRotateTopTestStepsAngles_degree[m_iTestStepLev4];

            QPointF ptCenter   = s_rctImage1.center();
            QPointF ptSelPtRot = s_ptDrawingSceneMousePosRotateToImage1MoveSelPtRotateTopStart;
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

            s_plgImage1 = rotateRect( s_rctImage1.center(), s_rctImage1, fAngle_rad );
            s_plgImage1.append(s_plgImage1[0]);

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgImage1) );

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

            bool bIgnoreTestResult = false;

            if( static_cast<int>(m_fAngle_degree) % 90 > 1 )
            {
                //bIgnoreTestResult = true;
            }
            pTestStep->testEnded(bIgnoreTestResult);

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Images_RotateTo_Image1_MoveSelPtRotateTop_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Images_RotateTo_Image1_MoveSelPtRotateTop_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_RotateTo_Image1_Release )
        {
            strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

            pTestStep = getTestStep( "RotateTo." + strCurrentGraphObj + ".Release", pTSGrpImagesRotateTo );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctImage1) );

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

                pGraphObjImage = dynamic_cast<CGraphObjImage*>(m_pGraphObjSelected);
                if( pGraphObjImage != nullptr )
                {
                    pGraphObjImage->setRotationAngleInDegree(0.0);
                }

                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_RotateTo_Image1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_RotateTo_Image1_Finished )
        {
            pTSGrpImagesMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_RotateTo_Image1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_RotateResize_Image1_ActivateEditModeSelect )
        {
            if( !pTSGrpImagesRotateResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Images_RotateResize_Image1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpImagesRotateResize->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateEditModeSelect", pTSGrpImagesRotateResize );

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

            } // if( pTSGrpImagesRotateResizeMove->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_RotateResizeMove_Image1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_RotateResize_Image1_Start )
        {
            m_iTestStepLev4 = -1;
            m_fAngle_degree = 0.0;

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

            s_plgImage1 = s_rctImage1;

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_RotateResize_Image1_Start )

        else if( m_iTestStepLev3 >= ETestStepLev3_Images_RotateResize_Image1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Images_RotateResize_Image1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Image" + QString::number(m_iImagesCount);

            pTestStep = getTestStep( "RotateResize." + strCurrentGraphObj + "Step" + QString::number(m_iTestStepLev4), pTSGrpImagesRotateResize );

            SRotateResizeOperation op = s_arRotateResizeImage1TestSteps[m_iTestStepLev4];

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
                QPolygonF plgImage1;
                plgImage1.append(op.m_pt0PolygonRectResult);
                plgImage1.append(op.m_pt1PolygonRectResult);
                plgImage1.append(op.m_pt2PolygonRectResult);
                plgImage1.append(op.m_pt3PolygonRectResult);
                plgImage1.append(op.m_pt0PolygonRectResult);
                strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(plgImage1) );
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
                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);

            pTestStep->testEnded(op.m_bIgnoreTestResult);

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Images_RotateResize_Image1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Images_RotateResize_Image1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Images_RotateResize_Image1_Finished )
        {
            pTSGrpImagesRotateResize->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Images_RotateResize_Image1_Finished )

        else // start next test step
        {
            pTSGrpImages->testEnded();

            m_iTestStepLev2++; // Next test step
            m_iTestStepLev3 = 0;
            triggerDoTestStep();
        }

    } // if( pTSGrpImages->isEnabled() )

    delete pMouseEv;
    pMouseEv = nullptr;

} // doTestStepStandardShapesImages

#endif // #if 0
