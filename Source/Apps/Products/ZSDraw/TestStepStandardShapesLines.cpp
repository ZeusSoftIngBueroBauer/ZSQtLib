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
#include "ZSDraw/ZSDrawGraphObjLine.h"
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

static QLineF s_line1;

// Create_Line1_MovePt2
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosCreateLine1Start(150,50);

static const QSize s_arsizLinesCreateLine1MovePt2TestSteps[] = {
               /* LineCoors:       (P1),     (P2) */
    /*  0 */ QSize( 5, 5 ), /* (150,50), (155,55) */
    /*  1 */ QSize( 5, 5 ), /* (150,50), (160,60) */
    /*  2 */ QSize( 5, 5 )  /* (150,50), (165,65) */
};

static const int ETestStepLev3_Lines_Create_Line1_MovePt2_TestStepCount = sizeof(s_arsizLinesCreateLine1MovePt2TestSteps)/sizeof(s_arsizLinesCreateLine1MovePt2TestSteps[0]);

// Move_Line1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosMoveLine1Start(157,57);

static const QSize s_arsizMoveLine1TestSteps[] = {
                 /* LineCoors:       (P1),     (P2) */
    /*  0 */ QSize( -5,  0 ), /* (145,50), (160,65) */
    /*  1 */ QSize( -5,  0 ), /* (140,50), (155,65) */
    /*  2 */ QSize( -5,  0 ), /* (135,50), (150,65) */
    /*  3 */ QSize(  0, -5 ), /* (135,45), (150,60) */
    /*  4 */ QSize(  0, -5 ), /* (135,40), (150,55) */
    /*  5 */ QSize(  0, -5 ), /* (135,35), (150,50) */
    /*  6 */ QSize(  5,  0 ), /* (140,35), (155,50) */
    /*  7 */ QSize(  5,  0 ), /* (145,35), (160,50) */
    /*  8 */ QSize(  5,  0 ), /* (150,35), (165,50) */
    /*  9 */ QSize(  0,  5 ), /* (150,40), (165,55) */
    /* 10 */ QSize(  0,  5 ), /* (150,45), (165,60) */
    /* 11 */ QSize(  0,  5 )  /* (150,50), (165,65) */
};

static const int ETestStepLev3_Lines_Move_Line1_TestStepCount = sizeof(s_arsizMoveLine1TestSteps)/sizeof(s_arsizMoveLine1TestSteps[0]);

// Resize_Line1_MovePt1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeLine1Select(157,57);

static const QPoint s_ptDrawingSceneMousePosResizeLine1MovePt1Start(149,51); // The line end point should not been hit exactly.

static const QSize s_arsizResizeLine1MovePt1TestSteps[] = {
                /* LineCoors:       (P1),     (P2) */
    /*  0 */ QSize( -4, 4 ), /* (145,55), (165,65) */
    /*  1 */ QSize( -5, 5 ), /* (140,60), (165,65) */
    /*  2 */ QSize( -5, 5 ), /* (135,65), (165,65) */
};

static const int ETestStepLev3_Lines_Resize_Line1_MovePt1_TestStepCount = sizeof(s_arsizResizeLine1MovePt1TestSteps)/sizeof(s_arsizResizeLine1MovePt1TestSteps[0]);

// Resize_Line1_MovePt2
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeLine1MovePt2Start(164,64); // The line end point should not been hit exactly.

static const QSize s_arsizResizeLine1MovePt2TestSteps[] = {
                /* LineCoors:       (P1),     (P2) */
    /*  0 */ QSize( -4, -4 ), /* (135,65), (160,60) */
    /*  1 */ QSize( -5, -5 ), /* (135,65), (155,55) */
    /*  2 */ QSize( -5, -5 ), /* (135,65), (150,50) */
};

static const int ETestStepLev3_Lines_Resize_Line1_MovePt2_TestStepCount = sizeof(s_arsizResizeLine1MovePt2TestSteps)/sizeof(s_arsizResizeLine1MovePt2TestSteps[0]);

// Test steps:
//------------------------------------------------------------------------------

enum
{
    ETestStepLev3_Lines_Create_Line1_ActivateDrawingTool,
    ETestStepLev3_Lines_Create_Line1,
    ETestStepLev3_Lines_Create_Line1_MovePt2_TestStepMin,
    ETestStepLev3_Lines_Create_Line1_MovePt2_TestStepMax = ETestStepLev3_Lines_Create_Line1_MovePt2_TestStepMin + ETestStepLev3_Lines_Create_Line1_MovePt2_TestStepCount - 1,
    ETestStepLev3_Lines_Create_Line1_Release,
    ETestStepLev3_Lines_Create_Line1_Finished,
    ETestStepLev3_Lines_Move_Line1_ActivateEditModeSelect,
    ETestStepLev3_Lines_Move_Line1_Select,
    ETestStepLev3_Lines_Move_Line1_TestStepMin,
    ETestStepLev3_Lines_Move_Line1_TestStepMax = ETestStepLev3_Lines_Move_Line1_TestStepMin + ETestStepLev3_Lines_Move_Line1_TestStepCount - 1,
    ETestStepLev3_Lines_Move_Line1_Release,
    ETestStepLev3_Lines_Move_Line1_Finished,
    ETestStepLev3_Lines_Resize_Line1_ActivateEditModeSelect,
    ETestStepLev3_Lines_Resize_Line1_Select,
    ETestStepLev3_Lines_Resize_Line1_MovePt1_Select,
    ETestStepLev3_Lines_Resize_Line1_MovePt1_TestStepMin,
    ETestStepLev3_Lines_Resize_Line1_MovePt1_TestStepMax = ETestStepLev3_Lines_Resize_Line1_MovePt1_TestStepMin + ETestStepLev3_Lines_Resize_Line1_MovePt1_TestStepCount - 1,
    ETestStepLev3_Lines_Resize_Line1_MovePt1_Release,
    ETestStepLev3_Lines_Resize_Line1_MovePt2_Select,
    ETestStepLev3_Lines_Resize_Line1_MovePt2_TestStepMin,
    ETestStepLev3_Lines_Resize_Line1_MovePt2_TestStepMax = ETestStepLev3_Lines_Resize_Line1_MovePt2_TestStepMin + ETestStepLev3_Lines_Resize_Line1_MovePt2_TestStepCount - 1,
    ETestStepLev3_Lines_Resize_Line1_MovePt2_Release,
    ETestStepLev3_Lines_Resize_Line1_Finished,
    ETestStepLev3_Lines_Finished,
    ETestStepLev3_Lines_Count
};

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepStandardShapesLines( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
//------------------------------------------------------------------------------
{
    QMouseEvent*         pMouseEv = nullptr;
    QPoint               ptMousePosGlobal;
    QString              strCurrentDrawingTool;
    QString              strCurrentGraphObj;
    QString              strToolTip;
    ZS::Test::CTestStep* pTestStep;
    QStringList          strlstDesiredValues;
    QStringList          strlstActualValues;

    ZS::Test::CTestStepGroup* pTSGrpLines          = getTestStepGroup( "Lines", i_pTestStepGrpParent );
    ZS::Test::CTestStepGroup* pTSGrpLinesCreate    = getTestStepGroup( "Create", pTSGrpLines );
    ZS::Test::CTestStepGroup* pTSGrpLinesMove      = getTestStepGroup( "Move", pTSGrpLines );
    ZS::Test::CTestStepGroup* pTSGrpLinesResize    = getTestStepGroup( "Resize", pTSGrpLines );

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }

    if( !pTSGrpLines->isEnabled() )
    {
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else if( !pTSGrpLinesCreate->isEnabled() )
    {
        // If no line is created none of the other sub tests may be performed.
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else // if( pTSGrpLines->isEnabled() )
    {
        if( m_iTestStepLev3 == ETestStepLev3_Lines_Create_Line1_ActivateDrawingTool )
        {
            pTestStep = getTestStep( "Create.Line1.ActivateDrawingTool", pTSGrpLinesCreate );

            strlstDesiredValues.append( "EditTool: CreateObjects" );
            strlstDesiredValues.append( "DrawingTool: Line" );

            pTestStep->setOperation( "drawing.setCurrentDrawingTool( Line )" );
            pTestStep->setDesiredValues(strlstDesiredValues);
            pTestStep->testStarted();

            m_pMainWindow->setCheckedActionDrawStandardShapeLine(true);

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Lines_Create_Line1_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Lines_Create_Line1 )
        {
            m_iLinesCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Create." + strCurrentGraphObj, pTSGrpLinesCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateLine1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_line1 = QLineF(
                s_ptDrawingSceneMousePosCreateLine1Start.x(), s_ptDrawingSceneMousePosCreateLine1Start.y(),
                s_ptDrawingSceneMousePosCreateLine1Start.x()+1, s_ptDrawingSceneMousePosCreateLine1Start.y()+1 );

            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_line1) );

            pTestStep->setOperation( "view.mousePressEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
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

        } // if( m_iTestStepLev3 == ETestStepLev3_Lines_Create_Line1 )

        else if( m_iTestStepLev3 >= ETestStepLev3_Lines_Create_Line1_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Lines_Create_Line1_MovePt2_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Create." + strCurrentGraphObj + ".MovePt2Step" + QString::number(m_iTestStepLev4), pTSGrpLinesCreate );

            m_fxOffsHor = s_arsizLinesCreateLine1MovePt2TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizLinesCreateLine1MovePt2TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_line1.setP2(m_ptDrawingSceneMousePos);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_line1) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".creatingObject( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Lines_Create_Line1_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Lines_Create_Line1_MovePt2_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Lines_Create_Line1_Release )
        {
            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Create." + strCurrentGraphObj + ".Release", pTSGrpLinesCreate );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Lines_Create_Line1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Lines_Create_Line1_Finished )
        {
            pTSGrpLinesCreate->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Lines_Create_Line1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Lines_Move_Line1_ActivateEditModeSelect )
        {
            if( !pTSGrpLinesMove->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Lines_Move_Line1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpLinesMove->isEnabled() )
            {
                strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

                pTestStep = getTestStep( "Move." + strCurrentGraphObj + ".ActivateEditModeSelect", pTSGrpLinesMove );

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

            } // if( pTSGrpLinesMove->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Lines_Move_Line1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Lines_Move_Line1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Move." + strCurrentGraphObj + ".Select", pTSGrpLinesMove );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosMoveLine1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_line1) );

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
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Lines_Move_Line1_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Lines_Move_Line1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Lines_Move_Line1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Move." + strCurrentGraphObj + ".MoveStep" + QString::number(m_iTestStepLev4), pTSGrpLinesMove );

            m_fxOffsHor = s_arsizMoveLine1TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizMoveLine1TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            QPointF p1( s_line1.p1().x() + m_fxOffsHor, s_line1.p1().y() + m_fyOffsHor );
            QPointF p2( s_line1.p2().x() + m_fxOffsHor, s_line1.p2().y() + m_fyOffsHor );

            s_line1.setP1(p1);
            s_line1.setP2(p2);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_line1) );

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
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Lines_Move_Line1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Lines_Move_Line1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Lines_Move_Line1_Release )
        {
            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Move." + strCurrentGraphObj + ".Release", pTSGrpLinesMove );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_line1) );

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
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Lines_Move_Line1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Lines_Move_Line1_Finished )
        {
            pTSGrpLinesMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Lines_Move_Line1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Lines_Resize_Line1_ActivateEditModeSelect )
        {
            if( !pTSGrpLinesResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Lines_Resize_Line1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpLinesResize->isEnabled() )
            {
                strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

                pTestStep = getTestStep( "Resize." + strCurrentGraphObj + ".ActivateEditModeSelect", pTSGrpLinesResize );

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

            } // if( pTSGrpLinesResize->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Lines_Resize_Line1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Lines_Resize_Line1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Resize." + strCurrentGraphObj + ".Select", pTSGrpLinesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeLine1Select;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(-1) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_line1) );

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
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(m_pGraphObjSelected->getSelectedPolygonShapePointIndex()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Lines_Resize_Line1_Select )

        else if( m_iTestStepLev3 == ETestStepLev3_Lines_Resize_Line1_MovePt1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Resize." + strCurrentGraphObj + "MovePt1.Select", pTSGrpLinesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeLine1MovePt1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(0) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_line1) );

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
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(m_pGraphObjSelected->getSelectedPolygonShapePointIndex()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Lines_Resize_Line1_MovePt1_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Lines_Resize_Line1_MovePt1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Lines_Resize_Line1_MovePt1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Resize." + strCurrentGraphObj + "MovePt1Step" + QString::number(m_iTestStepLev4), pTSGrpLinesResize );

            m_fxOffsHor = s_arsizResizeLine1MovePt1TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizResizeLine1MovePt1TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_line1.setP1(m_ptDrawingSceneMousePos);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_line1) );

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
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Lines_Resize_Line1_MovePt1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Lines_Resize_Line1_MovePt1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Lines_Resize_Line1_MovePt1_Release )
        {
            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Resize." + strCurrentGraphObj + "MovePt1.Release", pTSGrpLinesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_line1) );

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
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Lines_Resize_Line1_MovePt1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Lines_Resize_Line1_MovePt2_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Resize." + strCurrentGraphObj + "MovePt2.Select", pTSGrpLinesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeLine1MovePt2Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(1) );
            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_line1) );

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
                strlstActualValues.append( strCurrentGraphObj + ": SelPt:" + QString::number(m_pGraphObjSelected->getSelectedPolygonShapePointIndex()) );
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Lines_Resize_Line1_MovePt2_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Lines_Resize_Line1_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Lines_Resize_Line1_MovePt2_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Resize." + strCurrentGraphObj + "MovePt2Step" + QString::number(m_iTestStepLev4), pTSGrpLinesResize );

            m_fxOffsHor = s_arsizResizeLine1MovePt2TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizResizeLine1MovePt2TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_line1.setP2(m_ptDrawingSceneMousePos);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_line1) );

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
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Lines_Resize_Line1_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Lines_Resize_Line1_MovePt2_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Lines_Resize_Line1_MovePt2_Release )
        {
            strCurrentGraphObj = "Line" + QString::number(m_iLinesCount);

            pTestStep = getTestStep( "Resize." + strCurrentGraphObj + "MovePt2.Release", pTSGrpLinesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstDesiredValues.append( strCurrentGraphObj + ": " + line2Str(s_line1) );

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
                strlstActualValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Lines_Resize_Line1_MovePt2_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Lines_Resize_Line1_Finished )
        {
            pTSGrpLinesResize->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Lines_Resize_Line1_Finished )

        else if( m_iTestStepLev3 >= ETestStepLev3_Lines_Finished )
        {
            pTSGrpLines->testEnded();

            m_iTestStepLev2++; // Next test step
            m_iTestStepLev3 = 0;
            triggerDoTestStep();
        }

    } // if( pTSGrpLines->isEnabled() )

    delete pMouseEv;
    pMouseEv = nullptr;

} // doTestStepStandardShapesLines

#endif // #if 0
