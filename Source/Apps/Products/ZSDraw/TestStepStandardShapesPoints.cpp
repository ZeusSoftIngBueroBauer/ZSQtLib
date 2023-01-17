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
#include "ZSDraw/ZSDrawGraphObjPoint.h"
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

static const QPoint s_ptDrawingSceneMousePosCreatePoint1(10,10);

static const QSize s_arsizePointsCreateFurtherPointsMoveSteps[] = {
                                /* xOffs, yOffs: xLT, yLT */
    /*  0 */ QSize(  50,  50 ), /*     0,     0:  50,  50 */ // Coordinates for FirstPoint (m_iMoveStep == 0)
    /*  1 */ QSize( -15,   0 ), /*   -15,     0:  35,  50 */ // TestStepMin (m_iMoveStep == 1)
    /*  2 */ QSize(   0,  -5 ), /*   -15,    -5:  35,  45 */
    /*  3 */ QSize(   0,  -5 ), /*   -15,   -10:  35,  40 */
    /*  4 */ QSize(   0,  -5 ), /*   -15,   -15:  35,  35 */
    /*  5 */ QSize(   5,   0 ), /*   -10,   -15:  40,  35 */
    /*  6 */ QSize(   5,   0 ), /*    -5,   -15:  45,  35 */
    /*  7 */ QSize(   5,   0 ), /*     0,   -15:  50,  35 */
    /*  8 */ QSize(   5,   0 ), /*     5,   -15:  55,  35 */
    /*  9 */ QSize(   5,   0 ), /*    10,   -15:  60,  35 */
    /* 10 */ QSize(   5,   0 ), /*    15,   -15:  65,  35 */
    /* 11 */ QSize(   0,   5 ), /*    15,   -10:  65,  40 */
    /* 12 */ QSize(   0,   5 ), /*    15,    -5:  65,  45 */
    /* 13 */ QSize(   0,   5 ), /*    15,     0:  65,  50 */
    /* 14 */ QSize(   0,   5 ), /*    15,     5:  65,  55 */
    /* 15 */ QSize(   0,   5 ), /*    15,    10:  65,  60 */
    /* 16 */ QSize(   0,   5 ), /*    15,    15:  65,  65 */
    /* 17 */ QSize(  -5,   0 ), /*    10,    15:  60,  65 */
    /* 18 */ QSize(  -5,   0 ), /*     5,    15:  55,  65 */
    /* 19 */ QSize(  -5,   0 ), /*     0,    15:  50,  65 */
    /* 20 */ QSize(  -5,   0 ), /*    -5,    15:  45,  65 */
    /* 21 */ QSize(  -5,   0 ), /*   -10,    15:  40,  65 */
    /* 22 */ QSize(  -5,   0 ), /*   -15,    15:  35,  65 */
    /* 23 */ QSize(   0,  -5 ), /*   -15,    10:  35,  60 */
    /* 24 */ QSize(   0,  -5 )  /*   -15,     5:  35,  55 */ // TestStepMax (m_iMoveStep == 24)
};

static const int ETestStepLev3_Points_Create_FurtherPoints_TestStepCount = sizeof(s_arsizePointsCreateFurtherPointsMoveSteps)/sizeof(s_arsizePointsCreateFurtherPointsMoveSteps[0]);

static QList<QPointF> s_arPts;

// Test steps:
//------------------------------------------------------------------------------

enum
{
    ETestStepLev3_Points_Create_ActivateDrawingTool,
    ETestStepLev3_Points_Create_FirstPoint,
    ETestStepLev3_Points_Create_FurtherPoints_TestStepMin,
    ETestStepLev3_Points_Create_FurtherPoints_TestStepMax = ETestStepLev3_Points_Create_FurtherPoints_TestStepMin + ETestStepLev3_Points_Create_FurtherPoints_TestStepCount - 1,
    ETestStepLev3_Points_Create_Release,
    ETestStepLev3_Points_Create_Finished,
    ETestStepLev3_Points_Count
};

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepStandardShapesPoints( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
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
    QGraphicsItem*       pGraphicsItem;
    CGraphObj*           pGraphObj;
    int                  idxGraphObj;

    ZS::Test::CTestStepGroup* pTSGrpPoints         = getTestStepGroup( "Points", i_pTestStepGrpParent );
    ZS::Test::CTestStepGroup* pTSGrpPointsCreate   = getTestStepGroup( "Create", pTSGrpPoints );

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }

    if( !pTSGrpPoints->isEnabled() )
    {
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else if( !pTSGrpPointsCreate->isEnabled() )
    {
        // If no points are created none of the other sub tests may be performed.
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else // if( pTSGrpPoints->isEnabled() )
    {
        if( m_iTestStepLev3 == ETestStepLev3_Points_Create_ActivateDrawingTool )
        {
            if( !pTSGrpPointsCreate->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Points_Create_Finished+1; // Last create test step
                triggerDoTestStep();
            }
            else // if( pTSGrpPointsCreate->isEnabled() )
            {
                pTestStep = getTestStep( "ActivateDrawingTool", pTSGrpPointsCreate );

                strlstDesiredValues.append( "EditTool: CreateObjects" );
                strlstDesiredValues.append( "DrawingTool: Point" );

                pTestStep->setOperation( "drawing.setCurrentDrawingTool( Point )" );
                pTestStep->setDesiredValues(strlstDesiredValues);
                pTestStep->testStarted();

                m_pMainWindow->setCheckedActionDrawStandardShapePoint(true);

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

            } // if( pTSGrpPointsCreate->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Points_Create_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Points_Create_FirstPoint )
        {
            m_iPointsCount = 1;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Point" + QString::number(m_iPointsCount);

            pTestStep = getTestStep( strCurrentGraphObj, pTSGrpPointsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreatePoint1;
            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_arPts.append(s_ptDrawingSceneMousePosCreatePoint1);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_arPts[m_iPointsCount-1]) );

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

            QList<QGraphicsItem*> arpGraphicsItems = m_pDrawingScene->items();

            for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
            {
                pGraphicsItem = arpGraphicsItems[idxGraphObj];
                pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                if( pGraphicsItem != nullptr && pGraphObj != nullptr )
                {
                    QPointF ptPos = pGraphicsItem->scenePos();

                    if( ptPos == m_ptDrawingSceneMousePos )
                    {
                        strCurrentGraphObj = pGraphObj->getObjName();
                        strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                        strToolTip = pGraphicsItem->toolTip();
                        break;
                    }
                }
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Points_Create_FirstPoint )

        else if( m_iTestStepLev3 >= ETestStepLev3_Points_Create_FurtherPoints_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Points_Create_FurtherPoints_TestStepMax )
        {
            m_iPointsCount++;

            m_iTestStepLev4++;

            strCurrentGraphObj = "Point" + QString::number(m_iPointsCount);

            pTestStep = getTestStep( strCurrentGraphObj, pTSGrpPointsCreate );

            m_fxOffsHor = s_arsizePointsCreateFurtherPointsMoveSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizePointsCreateFurtherPointsMoveSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_arPts.append(m_ptDrawingSceneMousePos);

            strlstDesiredValues.append( strCurrentGraphObj + ": " + point2Str(s_arPts[m_iPointsCount-1]) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".create( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

            QList<QGraphicsItem*> arpGraphicsItems = m_pDrawingScene->items();

            for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
            {
                pGraphicsItem = arpGraphicsItems[idxGraphObj];
                pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                if( pGraphicsItem != nullptr && pGraphObj != nullptr )
                {
                    QPointF ptPos = pGraphicsItem->scenePos();

                    if( ptPos == m_ptDrawingSceneMousePos )
                    {
                        strCurrentGraphObj = pGraphObj->getObjName();
                        strlstActualValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                        strToolTip = pGraphicsItem->toolTip();
                        break;
                    }
                }
            }

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Points_Create_FurtherPoints_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Points_Create_FurtherPoints_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Points_Create_Release )
        {
            pTestStep = getTestStep( "Release", pTSGrpPointsCreate );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            pTestStep->setOperation( "view.mouseReleaseEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene.mouseReleaseEvent( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
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

            pTestStep->setActualValues(strlstActualValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Points_Create_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Points_Create_Finished )
        {
            pTSGrpPointsCreate->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Points_Create_Finished )

        else // start next test step
        {
            pTSGrpPoints->testEnded();

            m_iTestStepLev2++; // Next test step
            m_iTestStepLev3 = 0;
            triggerDoTestStep();
        }

    } // if( pTSGrpPoints->isEnabled() )

    delete pMouseEv;
    pMouseEv = nullptr;

} // doTestStepStandardShapesPoints

#endif // #if 0
