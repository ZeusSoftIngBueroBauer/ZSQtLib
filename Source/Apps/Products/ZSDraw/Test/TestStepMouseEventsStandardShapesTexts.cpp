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
#include "ZSDraw/ZSDrawGraphObjText.h"
#include "ZSDraw/ZSDrawObjFactory.h"
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

static QRectF    s_rctText1;
static QPolygonF s_plgText1;

// Create_Text1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosCreateText1Start(100,450);

struct SKeyInput
{
    QChar m_ch;
    int   m_iKey;
};

static const SKeyInput s_arKeyInputCreateText1TestSteps[] = {
    /*  0 */ { 'h',  Qt::Key_H      },
    /*  1 */ { 'e',  Qt::Key_E      },
    /*  2 */ { 'l',  Qt::Key_L      },
    /*  3 */ { 'l',  Qt::Key_L      },
    /*  4 */ { 'o',  Qt::Key_O      },
    /*  5 */ { '\n', Qt::Key_Return },
    /*  6 */ { 'w',  Qt::Key_W      },
    /*  7 */ { 'o',  Qt::Key_O      },
    /*  8 */ { 'r',  Qt::Key_R      },
    /*  9 */ { 'l',  Qt::Key_L      },
    /* 10 */ { 'd',  Qt::Key_D      }
};

static const int ETestStepLev3_Texts_Create_Text1_TestStepCount = sizeof(s_arKeyInputCreateText1TestSteps)/sizeof(s_arKeyInputCreateText1TestSteps[0]);

// Hover_Text1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosHoverText1Start(152,462);

static const QSize s_arsizeTextsHoverText1TestSteps[] = {
                    /* RectCoors:  l,   t,   r,   b,  w,  h */
    /*  0 */ QSize( -1,  1 ), /* 100, 450, 150, 475, 50, 25 */
    /*  1 */ QSize( -1, -1 ),
    /*  2 */ QSize( -1,  1 ),
    /*  3 */ QSize( -1, -1 ),
    /*  3 */ QSize( -1,  1 )
};

static const int ETestStepLev3_Texts_Hover_Text1_TestStepCount = sizeof(s_arsizeTextsHoverText1TestSteps)/sizeof(s_arsizeTextsHoverText1TestSteps[0]);

// Move_Text1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosMoveText1Start(134,465);

static const QSize s_arsizeTextsMoveText1TestSteps[] = {
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

static const int ETestStepLev3_Texts_Move_Text1_TestStepCount = sizeof(s_arsizeTextsMoveText1TestSteps)/sizeof(s_arsizeTextsMoveText1TestSteps[0]);

// RotateTo_Text1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosRotateToText1Select(134,465);

static const QPoint s_ptDrawingSceneMousePosRotateToText1MoveSelPtRotateTopStart(120,430);

static const double s_arfRotateText1MoveSelPtRotateTopTestStepsAngles_degree[] = {
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

static const int ETestStepLev3_Texts_RotateTo_Text1_MoveSelPtRotateTop_TestStepCount = sizeof(s_arfRotateText1MoveSelPtRotateTopTestStepsAngles_degree)/sizeof(s_arfRotateText1MoveSelPtRotateTopTestStepsAngles_degree[0]);

// Test steps:
//------------------------------------------------------------------------------

enum
{
    ETestStepLev3_Texts_Create_Text1_ActivateDrawingTool,
    ETestStepLev3_Texts_Create_Text1_Start,
    ETestStepLev3_Texts_Create_Text1_TestStepMin,
    ETestStepLev3_Texts_Create_Text1_TestStepMax = ETestStepLev3_Texts_Create_Text1_TestStepMin + ETestStepLev3_Texts_Create_Text1_TestStepCount - 1,
    ETestStepLev3_Texts_Create_Text1_Finished,
    ETestStepLev3_Texts_Hover_Text1_ActivateEditModeSelect,
    ETestStepLev3_Texts_Hover_Text1_TestStepMin,
    ETestStepLev3_Texts_Hover_Text1_TestStepMax = ETestStepLev3_Texts_Hover_Text1_TestStepMin + ETestStepLev3_Texts_Hover_Text1_TestStepCount - 1,
    ETestStepLev3_Texts_Hover_Text1_Finished,
    ETestStepLev3_Texts_Move_Text1_ActivateEditModeSelect,
    ETestStepLev3_Texts_Move_Text1_Select,
    ETestStepLev3_Texts_Move_Text1_TestStepMin,
    ETestStepLev3_Texts_Move_Text1_TestStepMax = ETestStepLev3_Texts_Move_Text1_TestStepMin + ETestStepLev3_Texts_Move_Text1_TestStepCount - 1,
    ETestStepLev3_Texts_Move_Text1_Release,
    ETestStepLev3_Texts_Move_Text1_Finished,
    ETestStepLev3_Texts_RotateTo_Text1_ActivateEditModeSelect,
    ETestStepLev3_Texts_RotateTo_Text1_Select,
    ETestStepLev3_Texts_RotateTo_Text1_MoveSelPtRotateTop_Select,
    ETestStepLev3_Texts_RotateTo_Text1_MoveSelPtRotateTop_TestStepMin,
    ETestStepLev3_Texts_RotateTo_Text1_MoveSelPtRotateTop_TestStepMax = ETestStepLev3_Texts_RotateTo_Text1_MoveSelPtRotateTop_TestStepMin + ETestStepLev3_Texts_RotateTo_Text1_MoveSelPtRotateTop_TestStepCount - 1,
    ETestStepLev3_Texts_RotateTo_Text1_Release,
    ETestStepLev3_Texts_RotateTo_Text1_Finished,
    ETestStepLev3_Texts_Count
};

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepStandardShapesTexts( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
//------------------------------------------------------------------------------
{
    QMouseEvent*         pMouseEv = nullptr;
    QKeyEvent*           pKeyEv = nullptr;
    QPoint               ptMousePosGlobal;
    QString              strCurrentDrawingTool;
    QString              strCurrentGraphObj;
    QString              strExpectedPainterPath;
    QString              strToolTip;
    ZS::Test::CTestStep* pTestStep;
    QStringList          strlstExpectedValues;
    QStringList          strlstResultValues;
    QList<CGraphObj*>    arpGraphObjs;
    CGraphObj*           pGraphObj;
    CGraphObjText*       pGraphObjText;

    ZS::Test::CTestStepGroup* pTSGrpTexts         = getTestStepGroup( "Texts", i_pTestStepGrpParent );
    ZS::Test::CTestStepGroup* pTSGrpTextsCreate   = getTestStepGroup( "Create", pTSGrpTexts );
    ZS::Test::CTestStepGroup* pTSGrpTextsHover    = getTestStepGroup( "Hover", pTSGrpTexts );
    ZS::Test::CTestStepGroup* pTSGrpTextsMove     = getTestStepGroup( "Move", pTSGrpTexts );
    ZS::Test::CTestStepGroup* pTSGrpTextsRotateTo = getTestStepGroup( "RotateTo", pTSGrpTexts );

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }

    if( !pTSGrpTexts->isEnabled() )
    {
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else if( !pTSGrpTextsCreate->isEnabled() )
    {
        // If the rectangle is not created none of the other sub tests may be performed.
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else // if( pTSGrpTexts->isEnabled() )
    {
        if( m_iTestStepLev3 == ETestStepLev3_Texts_Create_Text1_ActivateDrawingTool )
        {
            if( !pTSGrpTextsCreate->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Texts_Create_Text1_Finished+1; // Last create test step
                triggerDoTestStep();
            }
            else // if( pTSGrpTextsCreate->isEnabled() )
            {
                strCurrentGraphObj = "Text1";

                pTestStep = getTestStep( "Create." + strCurrentGraphObj + ".ActivateDrawingTool", pTSGrpTextsCreate );

                strlstExpectedValues.append( "EditTool: CreateObjects" );
                strlstExpectedValues.append( "DrawingTool: Text" );

                pTestStep->setOperation( "drawing.setCurrentDrawingTool( Text )" );
                pTestStep->setExpectedValues(strlstExpectedValues);
                pTestStep->testStarted();

                if( (m_pDrawingScene->getEditTool() != EEditTool::CreateObjects) || (m_pDrawingScene->getCurrentDrawingToolGraphObjType() != EGraphObjTypeText) )
                {
                    m_pMainWindow->setCheckedActionDrawStandardShapeText(true);
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

            } // if( pTSGrpTextsCreate->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Texts_Create_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Texts_Create_Text1_Start )
        {
            m_iTextsCount++;

            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Text" + QString::number(m_iTextsCount);

            pTestStep = getTestStep( "Create." + strCurrentGraphObj + ".Start", pTSGrpTextsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateText1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctText1 = QRectF( s_ptDrawingSceneMousePosCreateText1Start.x(), s_ptDrawingSceneMousePosCreateText1Start.y(), 8, 22 );

            strlstExpectedValues.append( "Text: " );

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

            pGraphObjText = dynamic_cast<CGraphObjText*>(pGraphObj);

            if( pGraphObjText != nullptr )
            {
                strlstResultValues.append( "Text: " + pGraphObjText->toPlainText() );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Texts_Create_Text1_Start )

        else if( m_iTestStepLev3 >= ETestStepLev3_Texts_Create_Text1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Texts_Create_Text1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Text" + QString::number(m_iTextsCount);

            QChar ch   = s_arKeyInputCreateText1TestSteps[m_iTestStepLev4].m_ch;
            int   iKey = s_arKeyInputCreateText1TestSteps[m_iTestStepLev4].m_iKey;

            pTestStep = getTestStep( "Create." + strCurrentGraphObj + ".KeyEnterStep" + QString::number(m_iTestStepLev4), pTSGrpTextsCreate );

            QString strText;

            for( int idx = 0; idx <= m_iTestStepLev4; idx++ )
            {
                strText += s_arKeyInputCreateText1TestSteps[idx].m_ch;
            }

            strlstExpectedValues.append( "Text: " + strText );

            pTestStep->setOperation( "view.keyPressEvent( " + QString(ch) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".keyPressEvent( " + QString(ch) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            pKeyEv = new QKeyEvent(
                /* type       */ QEvent::KeyPress,
                /* key        */ iKey,
                /* modifiers  */ Qt::NoModifier,
                /* text       */ ch,
                /* autoRepeat */ false,
                /* count      */ 1 );
            m_pDrawingView->keyPressEvent(pKeyEv);
            delete pKeyEv;
            pKeyEv = nullptr;

            pKeyEv = new QKeyEvent(
                /* type       */ QEvent::KeyRelease,
                /* key        */ iKey,
                /* modifiers  */ Qt::NoModifier,
                /* text       */ ch,
                /* autoRepeat */ false,
                /* count      */ 1 );
            m_pDrawingView->keyReleaseEvent(pKeyEv);
            delete pKeyEv;
            pKeyEv = nullptr;

            pGraphObj = m_pDrawingScene->findGraphObj(strCurrentGraphObj);

            pGraphObjText = dynamic_cast<CGraphObjText*>(pGraphObj);

            if( pGraphObjText != nullptr )
            {
                strlstResultValues.append( "Text: " + pGraphObjText->toPlainText() );
                strToolTip = pGraphObj->getToolTip();

                s_rctText1 = pGraphObjText->rect();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Texts_Create_Text1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Texts_Create_Text1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Texts_Create_Text1_Finished )
        {
            pTSGrpTextsCreate->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Texts_Create_Text1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Texts_Hover_Text1_ActivateEditModeSelect )
        {
            if( !pTSGrpTextsHover->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Texts_Hover_Text1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpTextsHover->isEnabled() )
            {
                m_iTestStepLev4 = -1;

                strCurrentGraphObj = "Text" + QString::number(m_iTextsCount);

                pTestStep = getTestStep( "Hover." + strCurrentGraphObj + ".ActivateEditModeSelect", pTSGrpTextsHover );

                strlstExpectedValues.append( "EditTool: Select" );
                strlstExpectedValues.append( "DrawingTool: " );

                pTestStep->setOperation( "drawing.setCurrentDrawingTool( Select )" );
                pTestStep->setExpectedValues(strlstExpectedValues);
                pTestStep->testStarted();

                if( m_pDrawingScene->getEditTool() != EEditTool::Select )
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

                m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosHoverText1Start;

                strlstResultValues.append( "EditTool: " + editTool2Str(m_pDrawingScene->getEditTool()) );
                strlstResultValues.append( "DrawingTool: " + strCurrentDrawingTool );

                pTestStep->setResultValues(strlstResultValues);
                pTestStep->setToolTip(strToolTip);
                pTestStep->testEnded();

                m_iTestStepLev3++;  // Next sub test step
                triggerDoTestStep();

            } // if( pTSGrpTextsHover->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Texts_Hover_Text1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 >= ETestStepLev3_Texts_Hover_Text1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Texts_Hover_Text1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Text" + QString::number(m_iTextsCount);

            pTestStep = getTestStep( "Hover." + strCurrentGraphObj + ".Step" + QString::number(m_iTestStepLev4), pTSGrpTextsHover );

            m_fxOffsHor = s_arsizeTextsHoverText1TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeTextsHoverText1TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

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

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded(true);

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 >= ETestStepLev3_Texts_Hover_Text1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Texts_Hover_Text1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Texts_Hover_Text1_Finished )
        {
            pTSGrpTextsHover->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Texts_Hover_Text1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Texts_Move_Text1_ActivateEditModeSelect )
        {
            if( !pTSGrpTextsMove->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Texts_Move_Text1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpTextsMove->isEnabled() )
            {
                strCurrentGraphObj = "Text" + QString::number(m_iTextsCount);

                pTestStep = getTestStep( "Move." + strCurrentGraphObj + ".ActivateEditModeSelect", pTSGrpTextsMove );

                strlstExpectedValues.append( "EditTool: Select" );
                strlstExpectedValues.append( "DrawingTool: " );

                pTestStep->setOperation( "drawing.setCurrentDrawingTool( Select )" );
                pTestStep->setExpectedValues(strlstExpectedValues);
                pTestStep->testStarted();

                if( m_pDrawingScene->getEditTool() != EEditTool::Select )
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

            } // if( pTSGrpTextsMove->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Texts_Move_Text1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Texts_Move_Text1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "Text" + QString::number(m_iTextsCount);

            pTestStep = getTestStep( "Move." + strCurrentGraphObj + ".Select", pTSGrpTextsMove );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosMoveText1Start;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPoint::Undefined) );

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
            s_rctText1 = QRectF();

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strToolTip = m_pGraphObjSelected->getToolTip();

                pGraphObjText = dynamic_cast<CGraphObjText*>(m_pGraphObjSelected);
                if( pGraphObjText != nullptr )
                {
                    s_rctText1 = pGraphObjText->rect();
                    s_rctText1 = pGraphObjText->mapToScene(s_rctText1).boundingRect();
                }
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Texts_Move_Text1_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Texts_Move_Text1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Texts_Move_Text1_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Text" + QString::number(m_iTextsCount);

            pTestStep = getTestStep( "Move." + strCurrentGraphObj + ".Step" + QString::number(m_iTestStepLev4), pTSGrpTextsMove );

            m_fxOffsHor = s_arsizeTextsMoveText1TestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeTextsMoveText1TestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctText1.moveLeft( s_rctText1.left() + m_fxOffsHor );
            s_rctText1.moveTop( s_rctText1.top() + m_fyOffsHor );

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctText1) );

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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Texts_Move_Text1_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Texts_Move_Text1_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Texts_Move_Text1_Release )
        {
            strCurrentGraphObj = "Text" + QString::number(m_iTextsCount);

            pTestStep = getTestStep( "Move." + strCurrentGraphObj + ".Release", pTSGrpTextsMove );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctText1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Texts_Move_Text1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Texts_Move_Text1_Finished )
        {
            pTSGrpTextsMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Texts_Move_Text1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Texts_RotateTo_Text1_ActivateEditModeSelect )
        {
            if( !pTSGrpTextsRotateTo->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Texts_RotateTo_Text1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpTextsRotateTo->isEnabled() )
            {
                strCurrentGraphObj = "Text" + QString::number(m_iTextsCount);

                pTestStep = getTestStep( "RotateTo." + strCurrentGraphObj + ".ActivateEditModeSelect", pTSGrpTextsRotateTo );

                strlstExpectedValues.append( "EditTool: Select" );
                strlstExpectedValues.append( "DrawingTool: " );

                pTestStep->setOperation( "drawing.setCurrentDrawingTool( Select )" );
                pTestStep->setExpectedValues(strlstExpectedValues);
                pTestStep->testStarted();

                if( m_pDrawingScene->getEditTool() != EEditTool::Select )
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

            } // if( pTSGrpTextsRotateTo->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Texts_RotateTo_Text1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Texts_RotateTo_Text1_Select )
        {
            m_iTestStepLev4 = -1;
            m_fAngle_degree = 0.0;

            strCurrentGraphObj = "Text" + QString::number(m_iTextsCount);

            pTestStep = getTestStep( "RotateTo." + strCurrentGraphObj + ".Select", pTSGrpTextsRotateTo );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosRotateToText1Select;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPoint::Undefined) );

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
            s_rctText1 = QRectF();

            QList<QGraphicsItem*> arpGraphObjsSelected = m_pDrawingScene->selectedItems();

            if( arpGraphObjsSelected.size() == 1 )
            {
                m_pGraphObjSelected = dynamic_cast<CGraphObj*>(arpGraphObjsSelected[0]);
                strCurrentGraphObj = m_pGraphObjSelected->getObjName();
                strlstResultValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(m_pGraphObjSelected->getSelectedBoundingRectPoint()) );
                strToolTip = m_pGraphObjSelected->getToolTip();

                pGraphObjText = dynamic_cast<CGraphObjText*>(m_pGraphObjSelected);
                if( pGraphObjText != nullptr )
                {
                    s_rctText1 = pGraphObjText->rect();
                    s_rctText1 = pGraphObjText->mapToScene(s_rctText1).boundingRect();
                }
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Texts_RotateTo_Text1_Select )

        else if( m_iTestStepLev3 == ETestStepLev3_Texts_RotateTo_Text1_MoveSelPtRotateTop_Select )
        {
            m_iTestStepLev4 = -1;
            m_fAngle_degree = 0.0;

            strCurrentGraphObj = "Text" + QString::number(m_iTextsCount);

            pTestStep = getTestStep( "RotateTo." + strCurrentGraphObj + ".SelectSelPtRotateTop", pTSGrpTextsRotateTo );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosRotateToText1MoveSelPtRotateTopStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPoint::RotateTop) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctText1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Texts_RotateTo_Text1_MoveSelPtRotateTop_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Texts_RotateTo_Text1_MoveSelPtRotateTop_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Texts_RotateTo_Text1_MoveSelPtRotateTop_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "Text" + QString::number(m_iTextsCount);

            pTestStep = getTestStep( "RotateTo." + strCurrentGraphObj + ".Step" + QString::number(m_iTestStepLev4), pTSGrpTextsRotateTo );

            m_fAngle_degree = s_arfRotateText1MoveSelPtRotateTopTestStepsAngles_degree[m_iTestStepLev4];

            QPointF ptCenter   = s_rctText1.center();
            QPointF ptSelPtRot = s_ptDrawingSceneMousePosRotateToText1MoveSelPtRotateTopStart;
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

            s_plgText1 = rotateRect( s_rctText1.center(), s_rctText1, fAngle_rad );
            s_plgText1.append(s_plgText1[0]);

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_plgText1) );

            pTestStep->setOperation( "view.mouseMoveEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".rotateTo( " + QString::number(m_fAngle_degree) + " " +  + " " + ZS::PhysVal::c_strSymbolDegree + ")" );
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Texts_RotateTo_Text1_MoveSelPtRotateTop_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Texts_RotateTo_Text1_MoveSelPtRotateTop_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Texts_RotateTo_Text1_Release )
        {
            strCurrentGraphObj = "Text" + QString::number(m_iTextsCount);

            pTestStep = getTestStep( "RotateTo." + strCurrentGraphObj + ".Release", pTSGrpTextsRotateTo );

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctText1) );

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

                pGraphObjText = dynamic_cast<CGraphObjText*>(m_pGraphObjSelected);
                if( pGraphObjText != nullptr )
                {
                    pGraphObjText->setRotationAngleInDegree(0.0);
                }

                strlstResultValues.append( strCurrentGraphObj + ": " + m_pGraphObjSelected->getScenePolygonShapePointsString() );
                strToolTip = m_pGraphObjSelected->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Texts_RotateTo_Text1_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Texts_RotateTo_Text1_Finished )
        {
            pTSGrpTextsMove->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Texts_RotateTo_Text1_Finished )

        else // start next test step
        {
            pTSGrpTexts->testEnded();

            m_iTestStepLev2++; // Next test step
            m_iTestStepLev3 = 0;
            triggerDoTestStep();
        }

    } // if( pTSGrpTexts->isEnabled() )

    delete pMouseEv;
    pMouseEv = nullptr;

} // doTestStepStandardShapesTexts

#endif // #if 0
