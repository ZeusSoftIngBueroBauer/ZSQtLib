/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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
#include "ZSDraw/ZSDrawGraphObjPolygon.h"
#include "ZSDraw/ZSDrawObjFactory.h"
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

static QPolygonF s_plg1Prev;
static QRectF    s_rctBoundingPlg1Prev;
static QPolygonF s_plg1Curr;
static QRectF    s_rctBoundingPlg1Curr;

// Create_Polygon1_FirstLineSegment
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosPolygon1Start(600,230);

static const QSize s_arsizePolygonsCreatePolygon1FirstLineSegmentMovePt2TestSteps[] = {
    /*  0 */ QSize( 6, 4 ),
    /*  1 */ QSize( 6, 4 ),
    /*  2 */ QSize( 6, 4 ),
    /*  3 */ QSize( 6, 4 ),
    /*  4 */ QSize( 6, 4 )
};

static const int ETestStepLev3_Polygons_Create_Polygon1_FirstLineSegment_MovePt2_TestStepCount  = sizeof(s_arsizePolygonsCreatePolygon1FirstLineSegmentMovePt2TestSteps)/sizeof(s_arsizePolygonsCreatePolygon1FirstLineSegmentMovePt2TestSteps[0]);

// Create_Polygon1_SecondLineSegment
//------------------------------------------------------------------------------

static const QSize s_arsizePolygonsCreatePolygon1SecondLineSegmentMovePt3TestSteps[] = {
    /*  0 */ QSize( 4, -10 ),
    /*  1 */ QSize( 4, -10 ),
    /*  2 */ QSize( 4, -10 ),
    /*  3 */ QSize( 4, -10 ),
    /*  4 */ QSize( 4, -10 )
};

static const int ETestStepLev3_Polygons_Create_Polygon1_SecondLineSegment_MovePt3_TestStepCount = sizeof(s_arsizePolygonsCreatePolygon1SecondLineSegmentMovePt3TestSteps)/sizeof(s_arsizePolygonsCreatePolygon1SecondLineSegmentMovePt3TestSteps[0]);


// Test steps:
//------------------------------------------------------------------------------

enum
{
    ETestStepLev3_Polygons_Create_Polygon1_ActivateDrawingTool,
    ETestStepLev3_Polygons_Create_Polygon1_FirstLineSegment,
    ETestStepLev3_Polygons_Create_Polygon1_FirstLineSegment_MovePt2_TestStepMin,
    ETestStepLev3_Polygons_Create_Polygon1_FirstLineSegment_MovePt2_TestStepMax = ETestStepLev3_Polygons_Create_Polygon1_FirstLineSegment_MovePt2_TestStepMin + ETestStepLev3_Polygons_Create_Polygon1_FirstLineSegment_MovePt2_TestStepCount - 1,
    ETestStepLev3_Polygons_Create_Polygon1_SecondLineSegment,
    ETestStepLev3_Polygons_Create_Polygon1_SecondLineSegment_MovePt3_TestStepMin,
    ETestStepLev3_Polygons_Create_Polygon1_SecondLineSegment_MovePt3_TestStepMax = ETestStepLev3_Polygons_Create_Polygon1_SecondLineSegment_MovePt3_TestStepMin + ETestStepLev3_Polygons_Create_Polygon1_SecondLineSegment_MovePt3_TestStepCount - 1,
    ETestStepLev3_Polygons_Create_Polygon1_Finished,
    ETestStepLev3_Polygons_Count
};

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepStandardShapesPolygons( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
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

    ZS::Test::CTestStepGroup* pTSGrpPolygons       = getTestStepGroup( "Polygons", i_pTestStepGrpParent );
    ZS::Test::CTestStepGroup* pTSGrpPolygonsCreate = getTestStepGroup( "Create", pTSGrpPolygons );

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }

    if( !pTSGrpPolygons->isEnabled() )
    {
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else if( !pTSGrpPolygonsCreate->isEnabled() )
    {
        // If the polyline is not created none of the other sub tests may be performed.
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else // if( pTSGrpPolygons->isEnabled() )
    {
        if( m_iTestStepLev3 == ETestStepLev3_Polygons_Create_Polygon1_ActivateDrawingTool )
        {
            pTestStep = getTestStep( "ActivateDrawingTool", pTSGrpPolygonsCreate );

            strlstExpectedValues.append( "EditTool: CreateObjects" );
            strlstExpectedValues.append( "DrawingTool: Polygon" );

            pTestStep->setOperation( "drawing.setCurrentDrawingTool( Polygon )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            if( (m_pDrawingScene->getEditTool() != EEditTool::CreateObjects) || (m_pDrawingScene->getCurrentDrawingToolGraphObjType() != EGraphObjTypePolygon) )
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

            strlstResultValues.append( "EditTool: " + editTool2Str(m_pDrawingScene->getEditTool()) );
            strlstResultValues.append( "DrawingTool: " + strCurrentDrawingTool );

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polygons_Create_Polygon1_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Polygons_Create_Polygon1_FirstLineSegment )
        {
            m_iPolygonsCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Polygon" + QString::number(m_iPolygonsCount);

            pTestStep = getTestStep( strCurrentGraphObj + "FirstLineSegment", pTSGrpPolygonsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosPolygon1Start;

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Polygons_Create_Polygon1_FirstLineSegment )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polygons_Create_Polygon1_FirstLineSegment_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polygons_Create_Polygon1_FirstLineSegment_MovePt2_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Polygon" + QString::number(m_iPolygonsCount);

            pTestStep = getTestStep( strCurrentGraphObj + "FirstLineSegmentMovePt2Step" + QString::number(m_iTestStepLev4), pTSGrpPolygonsCreate );

            m_fxOffsHor = s_arsizePolygonsCreatePolygon1FirstLineSegmentMovePt2TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolygonsCreatePolygon1FirstLineSegmentMovePt2TestSteps[m_iTestStepLev4].height();

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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polygons_Create_Polygon1_FirstLineSegment_MovePt2_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polygons_Create_Polygon1_FirstLineSegment_MovePt2_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polygons_Create_Polygon1_SecondLineSegment )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Polygon" + QString::number(m_iPolygonsCount);

            pTestStep = getTestStep( strCurrentGraphObj + "SecondLineSegment", pTSGrpPolygonsCreate );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Polygons_Create_Polygon1_SecondLineSegment )

        else if( m_iTestStepLev3 >= ETestStepLev3_Polygons_Create_Polygon1_SecondLineSegment_MovePt3_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polygons_Create_Polygon1_SecondLineSegment_MovePt3_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Polygon" + QString::number(m_iPolygonsCount);

            pTestStep = getTestStep( strCurrentGraphObj + "SecondLineSegmentMovePt3Step" + QString::number(m_iTestStepLev4), pTSGrpPolygonsCreate );

            m_fxOffsHor = s_arsizePolygonsCreatePolygon1SecondLineSegmentMovePt3TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePolygonsCreatePolygon1SecondLineSegmentMovePt3TestSteps[m_iTestStepLev4].height();

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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Polygons_Create_Polygon1_SecondLineSegment_MovePt3_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Polygons_Create_Polygon1_SecondLineSegment_MovePt3_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Polygons_Create_Polygon1_Finished )
        {
            strCurrentGraphObj = "Polygon" + QString::number(m_iPolygonsCount);

            pTestStep = getTestStep( strCurrentGraphObj + "Finish", pTSGrpPolygonsCreate );

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

            pTSGrpPolygonsCreate->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Polygons_Create_Polygon1_Finished )

        else // start next test step
        {
            pTSGrpPolygons->testEnded();

            m_iTestStepLev2++; // Next test step
            m_iTestStepLev3 = 0;
            triggerDoTestStep();
        }

    } // if( pTSGrpPolygons->isEnabled() )

    delete pMouseEv;
    pMouseEv = nullptr;

} // doTestStepStandardShapesPolygons

#endif // #if 0
