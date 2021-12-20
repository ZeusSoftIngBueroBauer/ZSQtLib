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
#if QT_VERSION >= 0x040000 && QT_VERSION < 0x050000
#include <QtGui/qgraphicssceneevent.h>
#elif QT_VERSION >= 0x050000 && QT_VERSION < 0x060000
#include <QtWidgets/qgraphicssceneevent.h>
#endif

#if QT_VERSION < 0x050000
#include <QtGui/qaction.h>
#else
#include <QtWidgets/qaction.h>
#endif

#include "Test.h"
#include "MainWindow.h"
#include "ZSDrawGraphObjElectricitySwitch.h"

#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawingView.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Draw::Electricity;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Products::Draw;
using namespace ZS::Trace;


/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

const int     c_iGraphObjType        = EGraphObjTypeUserDefined;
const QString c_strGraphObjNameSpace = "ZS::Draw::Electricity";
const QString c_strGraphObjClassName = "CGraphObjSwitch";
const QString c_strDrawingTool       = "Electricity::Switch";

static QRectF s_rctSwitch1;

// Create_Switch1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosCreateSwitch1(650,400);
static const QPoint s_ptDrawingSceneMousePosCreateSwitch1Finished(630,380);

// Resize_Switch1_MovePtBottomRight
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeSwitch1Select(662,405);

static const QPoint s_ptDrawingSceneMousePosResizeSwitch1PtBottomRightStart(674,410); // CGraphObjSwitch::c_sizInitial(24.0,10.0);

static const QSize s_arsizeGroupsResizeSwitch1MovePtBottomRightTestSteps[] = {
                      /* Switch1:      l,      t,      r,      b */
                               /* 650.00, 400.00, 674.00, 410.00 */
    /*  0 */ QSize(  12,  5 ), /* 650.00, 400.00, 686.00, 415.00 */
    /*  1 */ QSize(  12,  5 ), /* 650.00, 400.00, 698.00, 420.00 */
    /*  2 */ QSize( -12, -5 ), /* 650.00, 400.00, 686.00, 415.00 */
    /*  3 */ QSize( -12, -5 ), /* 650.00, 400.00, 674.00, 410.00 */
    /*  4 */ QSize( -12, -5 ), /* 650.00, 400.00, 662.00, 405.00 */
    /*  5 */ QSize( -12, -5 ), /* 650.00, 400.00, 650.00, 400.00 */
    /*  6 */ QSize( -12, -5 ), /* 638.00, 395.00, 650.00, 400.00 */
    /*  7 */ QSize( -12, -5 ), /* 626.00, 390.00, 650.00, 400.00 */
    /*  8 */ QSize(  12,  5 ), /* 638.00, 395.00, 650.00, 400.00 */
    /*  9 */ QSize(  12,  5 ), /* 650.00, 400.00, 650.00, 400.00 */
    /*  8 */ QSize(  12,  5 ), /* 650.00, 400.00, 662.00, 405.00 */
    /*  9 */ QSize(  12,  5 ), /* 650.00, 400.00, 674.00, 410.00 */
    /* 10 */ QSize(  12,  5 )  /* 650.00, 400.00, 686.00, 415.00 */
};

static const int ETestStepLev3_Switches_Resize_Switch1_MovePtBottomRight_TestStepCount = sizeof(s_arsizeGroupsResizeSwitch1MovePtBottomRightTestSteps)/sizeof(s_arsizeGroupsResizeSwitch1MovePtBottomRightTestSteps[0]);

// Test steps:
//------------------------------------------------------------------------------

enum
{
    ETestStepLev3_Switches_Create_Switch1_ActivateEditModeSelect,
    ETestStepLev3_Switches_Create_Switch1_ActivateDrawingTool,
    ETestStepLev3_Switches_Create_Switch1,
    ETestStepLev3_Switches_Create_Switch1_Finished,
    ETestStepLev3_Switches_Resize_Switch1_ActivateEditModeSelect,
    ETestStepLev3_Switches_Resize_Switch1_Select,
    ETestStepLev3_Switches_Resize_Switch1_MovePtBottomRight_Select,
    ETestStepLev3_Switches_Resize_Switch1_MovePtBottomRight_TestStepMin,
    ETestStepLev3_Switches_Resize_Switch1_MovePtBottomRight_TestStepMax = ETestStepLev3_Switches_Resize_Switch1_MovePtBottomRight_TestStepMin + ETestStepLev3_Switches_Resize_Switch1_MovePtBottomRight_TestStepCount - 1,
    ETestStepLev3_Switches_Resize_Switch1_MovePtBottomRight_Release,
    ETestStepLev3_Switches_Resize_Switch1_Finished,
    ETestStepLev3_Switches_Finished,
    ETestStepLev3_Switches_Count
};

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepElectricitySwitches( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
//------------------------------------------------------------------------------
{
    QMouseEvent*          pMouseEv = nullptr;
    QPoint                ptMousePosGlobal;
    QString               strCurrentDrawingTool;
    QString               strCurrentGraphObj;
    QString               strToolTip;
    ZS::Test::CTestStep*  pTestStep;
    QStringList           strlstExpectedValues;
    QStringList           strlstResultValues;
    CObjFactory*          pObjFactory;
    QList<QGraphicsItem*> arpGraphicsItems;
    CGraphObj*            pGraphObj;
    QGraphicsItem*        pGraphicsItem;

    ZS::Test::CTestStepGroup* pTSGrpSwitches       = getTestStepGroup( "Switches", i_pTestStepGrpParent );
    ZS::Test::CTestStepGroup* pTSGrpSwitchesCreate = getTestStepGroup( "Create", pTSGrpSwitches );
    ZS::Test::CTestStepGroup* pTSGrpSwitchesResize = getTestStepGroup( "Resize", pTSGrpSwitches );

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }

    if( !(m_pMainWindow->getAddedObjFactories() & CMainWindow::EAddObjFactoriesElectricity) )
    {
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else if( !pTSGrpSwitches->isEnabled() )
    {
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else if( !pTSGrpSwitchesCreate->isEnabled() )
    {
        // If the switch is not created none of the other sub tests may be performed.
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else // if( pTSGrpSwitchesCreate->isEnabled() )
    {
        if( m_iTestStepLev3 == ETestStepLev3_Switches_Create_Switch1_ActivateEditModeSelect )
        {
            pTestStep = getTestStep( "Create.S1.ActivateEditModeSelect", pTSGrpSwitchesCreate );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Switches_Create_Switch1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Switches_Create_Switch1_ActivateDrawingTool )
        {
            pTestStep = getTestStep( "Create.S1.ActivateDrawingTool", pTSGrpSwitchesCreate );

            strlstExpectedValues.append( "EditTool: Select" );
            strlstExpectedValues.append( "DrawingTool: " );

            pTestStep->setOperation( "ToolBoxTreeView.selectObjFactoryNode( Electricity/Switch )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            pObjFactory = CObjFactory::FindObjFactory(
                /* strGraphObjNameSpace */ c_strGraphObjNameSpace,
                /* strGraphObjClassName */ c_strGraphObjClassName,
                /* iGraphObjType        */ c_iGraphObjType );

            m_pMainWindow->selectTreeViewObjFactoryNode(pObjFactory);

            strlstResultValues.append( "EditTool: " + editTool2Str(m_pDrawingScene->getEditTool()) );
            strlstResultValues.append( "DrawingTool: " + strCurrentDrawingTool );

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Switches_Create_Switch1_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Switches_Create_Switch1 )
        {
            m_iElectricitySwitchesCount++;

            m_iTestStepLev4 = -1;

            strCurrentDrawingTool = c_strDrawingTool;

            strCurrentGraphObj = "S" + QString::number(m_iElectricitySwitchesCount);

            pTestStep = getTestStep( "Create." + strCurrentGraphObj, pTSGrpSwitchesCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateSwitch1;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctSwitch1 = QRectF( m_ptDrawingSceneMousePos, CGraphObjSwitch::c_sizInitial );

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctSwitch1) );

            pTestStep->setOperation( "view.dropEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentDrawingTool + ".drop( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            pObjFactory = CObjFactory::FindObjFactory(
                /* strGraphObjNameSpace */ c_strGraphObjNameSpace,
                /* strGraphObjClassName */ c_strGraphObjClassName,
                /* iGraphObjType        */ c_iGraphObjType );

            if( pObjFactory != nullptr )
            {
                CDrawSettings drawSettings;

                pGraphObj = pObjFactory->createGraphObj(
                    /* pDrawingScene */ m_pDrawingScene,
                    /* ptItemPos     */ QPointF(0.0,0.0),
                    /* drawSettings  */ drawSettings );

                pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);

                m_pDrawingScene->addItem(pGraphicsItem);

                pGraphicsItem->setPos(m_ptDrawingSceneMousePos);
                pGraphicsItem->setSelected(true);

                m_pDrawingScene->onGraphObjCreated(pGraphObj);

                m_pDrawingScene->setMode( EMode::Ignore, EEditToolSelect, EEditModeMove, EEditResizeModeUndefined, false );

                pGraphicsItem->setSelected(true);
            }

            pGraphObj = m_pDrawingScene->findGraphObj(strCurrentGraphObj);

            if( pGraphObj != nullptr )
            {
                strlstResultValues.append( strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString() );
                strToolTip = pGraphObj->getToolTip();
            }

            pTestStep->setResultValues(strlstResultValues);
            pTestStep->setToolTip(strToolTip);
            pTestStep->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Switches_Create_Switch1 )

        else if( m_iTestStepLev3 == ETestStepLev3_Switches_Create_Switch1_Finished )
        {
            strCurrentDrawingTool = c_strDrawingTool;

            strCurrentGraphObj = "S" + QString::number(m_iElectricitySwitchesCount);

            pTestStep = getTestStep( "Create." + strCurrentGraphObj + ".Finished", pTSGrpSwitchesCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateSwitch1Finished;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
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

            pTSGrpSwitchesCreate->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Switches_Create_Switch1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Switches_Resize_Switch1_ActivateEditModeSelect )
        {
            if( !pTSGrpSwitchesResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Switches_Resize_Switch1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpSwitchesResize->isEnabled() )
            {
                strCurrentGraphObj = "S" + QString::number(m_iElectricitySwitchesCount);

                pTestStep = getTestStep( "Resize." + strCurrentGraphObj + ".ActivateEditModeSelect", pTSGrpSwitchesResize );

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

            } // if( pTSGrpSwitchesResize->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Switches_Resize_Switch1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Switches_Resize_Switch1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "S" + QString::number(m_iElectricitySwitchesCount);

            pTestStep = getTestStep( "Resize." + strCurrentGraphObj + ".Select", pTSGrpSwitchesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeSwitch1Select;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointUndefined) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctSwitch1) );

            pTestStep->setOperation( "view.mouseClickEvent( ViewPos:" + point2Str(m_ptDrawingViewMousePos) + " )" );
            pTestStep->setDescription( "scene." + strCurrentGraphObj + ".select( ScenePos:" + point2Str(m_ptDrawingSceneMousePos) + " )" );
            pTestStep->setExpectedValues(strlstExpectedValues);
            pTestStep->testStarted();

            m_pDrawingScene->clearSelection();

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Switches_Resize_Switch1_Select )

        else if( m_iTestStepLev3 == ETestStepLev3_Switches_Resize_Switch1_MovePtBottomRight_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "S" + QString::number(m_iElectricitySwitchesCount);

            pTestStep = getTestStep( "Resize." + strCurrentGraphObj + ".MovePtBottomRight.Select", pTSGrpSwitchesResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeSwitch1PtBottomRightStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointBottomRight) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctSwitch1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Switches_Resize_Switch1_MovePtBottomRight_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Switches_Resize_Switch1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Switches_Resize_Switch1_MovePtBottomRight_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "S" + QString::number(m_iElectricitySwitchesCount);

            pTestStep = getTestStep( "Resize." + strCurrentGraphObj + ".MovePtBottomRight.Step" + QString::number(m_iTestStepLev4), pTSGrpSwitchesResize );

            m_fxOffsHor = s_arsizeGroupsResizeSwitch1MovePtBottomRightTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeGroupsResizeSwitch1MovePtBottomRightTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctSwitch1.setRight(m_ptDrawingSceneMousePos.x());
            s_rctSwitch1.setBottom(m_ptDrawingSceneMousePos.y());

            // Keep old size before resizing starts as the object may get 0 pixel size.
            // The size will be adjusted on releasing the mouse.
            QRectF rct = s_rctSwitch1;

            if( fabs(s_rctSwitch1.width()) > 0.0 || fabs(s_rctSwitch1.height()) > 0.0 )
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Switches_Resize_Switch1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Switches_Resize_Switch1_MovePtBottomRight_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Switches_Resize_Switch1_MovePtBottomRight_Release )
        {
            strCurrentGraphObj = "S" + QString::number(m_iElectricitySwitchesCount);

            pTestStep = getTestStep( "Resize." + strCurrentGraphObj + ".MovePtBottomRight.Release", pTSGrpSwitchesResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            // Old size before resizing starts has been kept as the object may get 0 pixel size.
            // The size will be adjusted on releasing the mouse.
            if( s_rctSwitch1.width() < 0 )
            {
                double fLeft = s_rctSwitch1.left();
                double fRight = s_rctSwitch1.right();
                s_rctSwitch1.setLeft(fRight);
                s_rctSwitch1.setRight(fLeft);
            }
            if( s_rctSwitch1.height() < 0 )
            {
                double fTop = s_rctSwitch1.top();
                double fBottom = s_rctSwitch1.bottom();
                s_rctSwitch1.setTop(fBottom);
                s_rctSwitch1.setBottom(fTop);
            }

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctSwitch1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Switches_Resize_Switch1_MovePtBottomRight_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Switches_Resize_Switch1_Finished )
        {
            pTSGrpSwitchesResize->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Switches_Resize_Switch1_Finished )

        else // if( m_iTestStepLev3 >= ETestStepLev3_Switches_Finished )
        {
            pTSGrpSwitches->testEnded();

            m_iTestStepLev2++; // Next test step
            m_iTestStepLev3 = 0;
            triggerDoTestStep();
        }

    } // if( pTSGrpSwitches->isEnabled() )

    delete pMouseEv;
    pMouseEv = nullptr;

} // doTestStepElectricitySwitches

#endif // #if 0
